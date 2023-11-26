#ifndef AST_H
#define AST_H


struct CAstList;

struct CAstItem
{
    bool                   isAstList;
    CAstList              *pList;
    bool                   isBreakSymbol;
    std::string            literal;
    std::string::size_type atPos;

    CAstItem();
    ~CAstItem();
    CAstItem(bool ibs, std::string l, std::string::size_type ap);
    CAstItem(CAstList* al);
    CAstItem(const CAstItem &ai);
    CAstItem& operator=(const CAstItem &ai);
}; // struct CAstItem

struct CAstList
{
    std::string             beginStr;
    std::string             endStr;
    std::string::size_type  beginAtPos;
    std::string::size_type  endAtPos;

    std::vector<CAstItem>   items;

    CAstList() : beginStr(), endStr(), 
                 beginAtPos(std::string::npos),
                 endAtPos(std::string::npos),
                 items() 
            {}

}; // struct CAstList


/*---------------------------------------------------------------------------*/
inline
CAstItem::~CAstItem()
   {
    if (pList) { delete pList; pList = 0; }
   }

/*---------------------------------------------------------------------------*/
inline
CAstItem::CAstItem()
   : isBreakSymbol(false), literal(), atPos(0), pList(0), isAstList(false)
   {}

/*---------------------------------------------------------------------------*/
inline
CAstItem::CAstItem(CAstList* al)
   : isBreakSymbol(false), literal(), atPos(0), pList(0), isAstList(true)
   {   
    if (al) pList = new CAstList(*al);
   }
/*---------------------------------------------------------------------------*/
inline
CAstItem::CAstItem(bool ibs, std::string l, std::string::size_type ap)
   : isBreakSymbol(ibs), literal(l), atPos(ap), pList(0), isAstList(false)
   {}

/*---------------------------------------------------------------------------*/
inline
CAstItem::CAstItem(const CAstItem &ai)
   : isBreakSymbol(ai.isBreakSymbol), literal(ai.literal), atPos(ai.atPos), isAstList(ai.isAstList), pList(0)
   {
    if (ai.pList) pList = new CAstList(*ai.pList);
   }

/*---------------------------------------------------------------------------*/
inline
CAstItem& CAstItem::operator=(const CAstItem &ai)
   {
    if (&ai==this) return *this;
    isBreakSymbol = ai.isBreakSymbol;
    literal = ai.literal;
    atPos = ai.atPos;
    isAstList = ai.isAstList;
    if (pList) { delete pList; pList = 0; }
    if (ai.pList) pList = new CAstList(*ai.pList);
    return *this;
   }

/*---------------------------------------------------------------------------*/


const int astItemType_begin = 0;
const int astItemType_end   = 1;
const int astItemType_break = 2;
const int astItemType_item  = 3;

class CLineAstParser
{
    CBreakSymbols           breakSymbols;
    std::string             line;
    std::string::size_type  curPos;
    std::string::size_type  size;

    CAstList                ast;


/* struct CAstList
 * {
 *     std::string             beginStr;
 *     std::string             endStr;
 *     std::string::size_type  beginAtPos;
 *     std::string::size_type  endAtPos;
 *  
 *     std::vector<CAstItem>   items;
 */

/* struct CAstItem
 * {
 *     bool                   isAstList;
 *     CAstList              *pList;
 *     bool                   isBreakSymbol;
 *     std::string            literal;
 *     std::string::size_type atPos;
 */


    struct iterator_stack_item
    {
        const CAstList *pAst;
        int       idx;

        void swap(iterator_stack_item &i)
           {
            std::swap(pAst, i.pAst);
            std::swap(idx , i.idx);
           }

        iterator_stack_item() : pAst(0), idx(0) {}

        iterator_stack_item(const iterator_stack_item &i)
           : pAst(i.pAst), idx(i.idx) {}

        iterator_stack_item(const CAstList *pa, int i)
           : pAst(pa), idx(i) {}

        iterator_stack_item& operator=(const iterator_stack_item &i)
           {
            iterator_stack_item tmp(i);
            swap(tmp);
            return *this;
           }
    };

public:

    struct IteratorData
    {
     int                     type; // astItemType_*
     std::string::size_type  pos;
     unsigned                level;
     std::string             text;    
    };



    class const_iterator
    {
     std::stack<iterator_stack_item>   iterator_stack;
     const CAstList                   *pHeadAst;
     const CAstList                   *pCurAst;
     int                               curIdx;
     IteratorData                      data;

     public:

     const_iterator(const CAstList *pa) 
        : pHeadAst(pa), 
          pCurAst(pa),
          curIdx(-1),
          iterator_stack(),
          data()
        {
         if (isNpos()) inc();
         updateIteratorData();
        }
    
     const_iterator(const CAstList *pa, int fictive) // construct end iterator
        : pHeadAst(pa), 
          pCurAst(0),
          curIdx(-1),
          iterator_stack(),
          data()
        {}

     const_iterator(const const_iterator &it)
        : iterator_stack(it.iterator_stack)
        , pHeadAst(it.pHeadAst)
        , pCurAst(it.pCurAst)
        , curIdx(it.curIdx)
        , data(it.data)
        {}

     void swap(const_iterator &it)
        {
         std::swap(iterator_stack, it.iterator_stack);
         std::swap(pHeadAst      , it.pHeadAst);
         std::swap(pCurAst       , it.pCurAst);
         std::swap(curIdx        , it.curIdx);
         std::swap(data          , it.data);
        }

     const_iterator& operator=(const const_iterator &it)
        {
         const_iterator tmp(it);
         swap(tmp);
         return *this;
        }

     const IteratorData* operator->() const
        {
         return &data;
        }

     bool isBeginIterator() const
        {
         if (pHeadAst==pCurAst && curIdx<0) return true;
         return false;
        }

     bool isEndIterator() const
        {
         if (!pCurAst) return true;
         return false;
        }

     const_iterator& operator++() // prefix
        {
         do{
            inc();
           } while(isNpos() && !isEndIterator());
         //if (isNpos()) inc();
         updateIteratorData();
         return *this;
        }

     const_iterator operator++(int) // postfix
        {
         const_iterator tmp(*this);
         do{
            inc();
           } while(isNpos() && !isEndIterator());
         //if (isNpos()) inc();
         updateIteratorData();
         return tmp;
        }

     const_iterator& operator--() // prefix
        {
         do{
            dec();
           } while(isNpos() && !isBeginIterator());
         //dec();
         //if (isNpos()) dec();
         updateIteratorData();
         return *this;
        }

     const_iterator operator--(int) // postfix
        {
         const_iterator tmp(*this);
         //dec();
         //if (isNpos()) dec();
         do{
            dec();
           } while(isNpos() && !isBeginIterator());
         updateIteratorData();
         return tmp;
        }

     bool operator==(const const_iterator &it) const
        {
         return isEqual(it);
        }

     bool operator!=(const const_iterator &it) const
        {
         return !isEqual(it);
        }

     void flipInCurrentLevel()
        {
         if (!pCurAst) 
            throw std::exception("flipInLevel failed with iterator at end");
         if (curIdx<0)
            {
             curIdx = (int)pCurAst->items.size();
             if (isNpos())
                dec();
            }
         else
            {
             curIdx = -1;
             if (isNpos())
                inc();
            }
         updateIteratorData();
        }
    

     
     protected:

     bool isNposBegin()
        {
         if (!pCurAst) return false;
         if (curIdx<0 && pCurAst->beginAtPos==std::string::npos) return true;
         return false;
        }

     bool isNposEnd()
        {
         if (!pCurAst) return false;
         if (curIdx>=(int)pCurAst->items.size() && pCurAst->endAtPos==std::string::npos) return true;
         return false;
        }

     bool isNpos()
        {
         return isNposBegin() || isNposEnd();
        }


     void updateIteratorData()
        {
         if (!pCurAst)
            {
             return;
             // throw std::exception("cant updateIteratorData on end iterator");
            }

         if (curIdx<0)
            {
             data.type  = astItemType_begin;
             data.pos   = pCurAst->beginAtPos;
             data.level = (int)iterator_stack.size();
             data.text  = pCurAst->beginStr;
            }
         else if (curIdx>=(int)pCurAst->items.size())
            {
             data.type  = astItemType_end;
             data.pos   = pCurAst->endAtPos;
             data.level = (int)iterator_stack.size();
             data.text  = pCurAst->endStr;
            }
         else
            {
             data.type  = pCurAst->items[curIdx].isBreakSymbol ? astItemType_break : astItemType_item;
             data.pos   = pCurAst->items[curIdx].atPos;
             data.level = (int)iterator_stack.size();
             data.text  = pCurAst->items[curIdx].literal;            
            }
        }

     void inc()
        {
         if (!pCurAst) 
            throw std::exception("cant increment iterator at end");

         if (curIdx < (int)pCurAst->items.size())
            {
             ++curIdx;  // next point
             if (curIdx < (int)pCurAst->items.size() && pCurAst->items[curIdx].isAstList)
                { // next point is sublist
                 iterator_stack.push(iterator_stack_item(pCurAst, curIdx));
                 pCurAst = pCurAst->items[curIdx].pList;
                 curIdx = -1;
                }
             return;
            }

         // cur list at end
         if (iterator_stack.empty())
            {
             pCurAst = 0; // make iterator is end
             return;
            }

         // get prev location from stack
         pCurAst = iterator_stack.top().pAst;
         curIdx  = iterator_stack.top().idx;
         iterator_stack.pop();
         ++curIdx;
         // next point
         if (curIdx < (int)pCurAst->items.size() && pCurAst->items[curIdx].isAstList)
            { // next point is sublist
             iterator_stack.push(iterator_stack_item(pCurAst, curIdx));
             pCurAst = pCurAst->items[curIdx].pList;
             curIdx = -1;
            }
        }


     void dec()
        {
         if (pCurAst==pHeadAst && curIdx<0)
            throw std::exception("cant decrement iterator at begin");

         if (!pCurAst) // decrease end iterator
            {
             pCurAst = pHeadAst;
             curIdx  = (int)pHeadAst->items.size();
             return;
            }

         if (curIdx<0)
            {
             if (iterator_stack.empty())
                throw std::exception("iterator_stack is empty");
             pCurAst = iterator_stack.top().pAst;
             curIdx  = iterator_stack.top().idx;
             iterator_stack.pop();
            }

         --curIdx;

         if (curIdx>=0 && pCurAst->items[curIdx].isAstList)
            { // prev point is sublist
             iterator_stack.push(iterator_stack_item(pCurAst, curIdx));
             pCurAst = pCurAst->items[curIdx].pList;
             curIdx = (int)pCurAst->items.size();
            }
        }

     bool isEqual(const const_iterator &it) const
        {
         if (!pCurAst && !it.pCurAst) return true; // both are end
         return pCurAst==it.pCurAst && curIdx==it.curIdx;
        }





    };


    template<typename T>
    void for_each_in_ast(unsigned level, const CAstList &astList, const T &t)
       {
        if (!astList.beginStr.empty())
           t(astItemType_begin, level, astList.beginAtPos, astList.beginStr);

        std::vector<CAstItem>::const_iterator it = astList.items.begin();
        for(; it!=astList.items.end(); ++it)
           {
            if (it->isAstList)
               for_each_in_ast(level+1, *(it->pList), t);
            else
               t(it->isBreakSymbol ? astItemType_break : astItemType_item,
                 level, it->atPos, it->literal);
           }

        if (!astList.endStr.empty())
           t(astItemType_end, level, astList.endAtPos, astList.endStr);
       }

    template<typename T>
    void for_each_in_ast(unsigned level, const CAstList &astList, T &t)
       {
        if (!astList.beginStr.empty())
           t(astItemType_begin, level, astList.beginAtPos, astList.beginStr);

        std::vector<CAstItem>::const_iterator it = astList.items.begin();
        for(; it!=astList.items.end(); ++it)
           {
            if (it->isAstList)
               for_each_in_ast(level+1, *(it->pList), t);
            else
               t(it->isBreakSymbol ? astItemType_break : astItemType_item,
                 level, it->atPos, it->literal);
           }

        if (!astList.endStr.empty())
           t(astItemType_end, level, astList.endAtPos, astList.endStr);
       }




public:

    template<typename T>
    void for_each_in_ast(const T &t)
       {
        for_each_in_ast(0, ast, t);
       }


    template<typename T>
    void for_each_in_ast(T &t)
       {
        for_each_in_ast(0, ast, t);
       }

    CLineAstParser(const CBreakSymbols &bs, const std::string &l)
       : breakSymbols(bs), line(l), curPos(0), size(line.size()), ast()
       {}

    const_iterator begin() const
       {
        return const_iterator(&ast);
       }

    const_iterator end() const
       {
        return const_iterator(&ast, 0);
       }

private:


    /*---------------------------------------------------------------------------*/
    // increments curPos also
    void readStringLiteralToAst(CAstList &astList)
       {
        std::string curLiteral;

        astList.beginStr    = std::string(1, line[curPos]);
        astList.beginAtPos  = curPos++;
        astList.endStr      = astList.beginStr;
        bool prevBs = false;
        while(curPos<size)
           {
            char ch = line[curPos];
            if (ch==' ' || ch=='\t')
               { // push current literal to list
                if (!curLiteral.empty()) 
                   astList.items.push_back(CAstItem(false, curLiteral, curPos-curLiteral.size()));
                curLiteral.clear();
                ++curPos;
                continue;
               }

            if ( ((ch=='\"' && breakSymbols.doubleQuoteStrings) ||
                       (ch=='\'' && breakSymbols.singleQuoteStrings)) && prevBs)
               {
                prevBs = false;
                curLiteral.append(1, ch);
                ++curPos;
                continue;
               }
            else if (ch=='\\' && breakSymbols.escapeBs)
               {
                prevBs = true;
                curLiteral.append(1, ch);
                ++curPos;
                continue;
               }
            else if ( ((ch=='\"' && breakSymbols.doubleQuoteStrings) ||
                       (ch=='\'' && breakSymbols.singleQuoteStrings)) && breakSymbols.escapeDblQuot)
               {
                prevBs = true;
                curLiteral.append(1, ch);
                ++curPos;
                continue;
               }
            else
               {
                prevBs = false;
               }

            if (((ch=='\"' && breakSymbols.doubleQuoteStrings) ||
                 (ch=='\'' && breakSymbols.singleQuoteStrings)) && astList.beginStr==std::string(1, line[curPos]))
               { // push current literal to list
                if (!curLiteral.empty()) 
                   astList.items.push_back(CAstItem(false, curLiteral, curPos-curLiteral.size()));
                astList.endAtPos  = curPos++;
                return;
               }

            curLiteral.append(1, ch);
            ++curPos;
           
           }       

        if (!curLiteral.empty()) 
           astList.items.push_back(CAstItem(false, curLiteral, curPos-curLiteral.size()));
       }



    /*---------------------------------------------------------------------------*/
    void buildAst(CAstList &astList)
       {
        std::string curLiteral;
        while(curPos<size)
           {
            char ch = line[curPos];
            if (ch=='\"' && breakSymbols.doubleQuoteStrings)
               { // push current literal to list
                if (!curLiteral.empty()) 
                   astList.items.push_back(CAstItem(false, curLiteral, curPos-curLiteral.size()));
                curLiteral.clear();
                // read string literal and push it to list
                CAstList tmpAl;
                readStringLiteralToAst(tmpAl);
                astList.items.push_back(CAstItem(&tmpAl));
                continue;
               }
            if (ch=='\'' && breakSymbols.singleQuoteStrings)
               { // push current literal to list
                if (!curLiteral.empty()) 
                   astList.items.push_back(CAstItem(false, curLiteral, curPos-curLiteral.size()));
                curLiteral.clear();
                // read string literal and push it to list
                CAstList tmpAl;
                readStringLiteralToAst(tmpAl);
                astList.items.push_back(CAstItem(&tmpAl));
                continue;
               }

            bool bSingle, bBegin;
            std::string begin, end;
            if (isBreakSymbol( line, breakSymbols, 
                               true, curPos,
                               bSingle, bBegin,
                               begin, end))
               { // push current literal to list
                if (!curLiteral.empty()) 
                   astList.items.push_back(CAstItem(false, curLiteral, curPos-curLiteral.size()));
                curLiteral.clear();

                if (bSingle || (!bBegin && end!=astList.endStr))
                   {
                    astList.items.push_back(CAstItem(true, begin, curPos));
                    curPos += begin.size();
                    continue;
                   }
                
                if (!bBegin)
                   { // return from reading current expression
                    astList.endAtPos = curPos;
                    curPos += end.size();
                    return;
                   }

                CAstList tmpAl;
                tmpAl.beginStr = begin;
                tmpAl.endStr   = end;
                tmpAl.beginAtPos = curPos;
                curPos += begin.size();
                buildAst(tmpAl);
                astList.items.push_back(CAstItem(&tmpAl));
                continue;
               }


            if (ch==' ' || ch=='\t')
               { // push current literal to list
                if (!curLiteral.empty()) 
                   astList.items.push_back(CAstItem(false, curLiteral, curPos-curLiteral.size()));
                curLiteral.clear();
                ++curPos;
                continue;
               }

            curLiteral.append(1, ch);
            ++curPos;
           }

        if (!curLiteral.empty()) 
           astList.items.push_back(CAstItem(false, curLiteral, curPos-curLiteral.size()));
           
       }

public:

    void buildAst()
       {
        ast.beginAtPos  = std::string::npos;
        ast.endAtPos    = std::string::npos;
        buildAst(ast);
       }


    const_iterator findPosAux(int pos)
       {
        const_iterator it = begin();
        for(; it!=end(); ++it)
           {
            int atPos = (int)it->pos;
            int size = (int)it->text.size();

            if (pos>=atPos && pos<(atPos+size))
               return it; // exact match
           }
        return end();
       }

    int getMaxPos()
       {
        const_iterator it = end();
        --it;
        if (it==begin()) return 0;
        return (int)(it->pos + it->text.size());
       }

    const_iterator findPosFwd(int pos)
       {
        int maxPos = getMaxPos();
        const_iterator res = findPosAux(pos++);
        while(res==end() && pos<maxPos)
           res = findPosAux(pos++);
        return res;
       }

    const_iterator findPosRwd(int pos)
       {
        const_iterator res = findPosAux(pos--);
        while(res==end() && pos>=0)
           res = findPosAux(pos--);
        return res;
       }
/*
    const_iterator findPosFwdWithType(int pos, int type)
       {
        const_iterator res = findPosRwd(pos);
        while(res!=end() && res->type!=type)
           res++;
        return res;
       }
*/    

    void incrementalSelect(int &startSel, int &endSel, int maxLen)
       {
        if (startSel==endSel)
           {
            const_iterator it = findPosFwd(startSel);
            if (it==end())
                it = findPosFwd(0);

            if (it==end())
                return;

            switch(it->type)
               {
                case astItemType_begin:
                   {
                    const_iterator itE = it;
                    itE.flipInCurrentLevel();
                    if (itE!=end())
                       endSel   = (int)(itE->pos + itE->text.size());
                    startSel = (int)(it->pos);

                    /*
                    for(; itE!=end(); ++itE)
                       {
                        if (itE->type==astItemType_end)
                           {
                            break;
                           }
                       }
                    if (itE==end()) --itE;
                    endSel = (int)(itE->pos + itE->text.size());
                    */
                    return;
                   }

                case astItemType_end:
                   {
                    const_iterator itB = it;
                    itB.flipInCurrentLevel();
                    if (itB!=end()/* && itB!=begin()*/)
                        startSel = (int)(itB->pos);
                    endSel = (int)(it->pos + it->text.size());
                    /*
                    for(; itB!=begin(); --itB)
                       {
                        if (itB->type==astItemType_begin)
                           {
                            break;
                           }
                       }
                    //if (itE==end()) --itE;
                    startSel = (int)(itB->pos);
                    endSel = (int)(it->pos + it->text.size());
                    */
                    return;
                   }

                case astItemType_break:
                default: 
                   {
                    startSel = (int)(it->pos);
                    endSel   = (int)(it->pos + it->text.size());
                    return;
                   }
               }
           }

/* const int astItemType_begin = 0;
 * const int astItemType_end   = 1;
 * const int astItemType_break = 2;
 * const int astItemType_item  = 3;
 */

        const_iterator itE = findPosFwd(endSel);
        if (itE!=end() && (itE->type==astItemType_break || itE->type==astItemType_item))
           {
            endSel   = (int)(itE->pos + itE->text.size());
            return;
           }

        if (itE!=end() && itE->type==astItemType_begin)
           {
            itE.flipInCurrentLevel();
            if (itE!=end())
               endSel   = (int)(itE->pos + itE->text.size());
            return;
           }

        const_iterator itB = findPosRwd(startSel-1);
        if (itB!=end() && (itB->type==astItemType_break || itB->type==astItemType_item))
           {
            startSel = (int)(itB->pos);
            return;
           }

        if (itB!=end() && itB->type==astItemType_end)
           {
            itB.flipInCurrentLevel();
            if (itB!=end()/* && itB!=begin()*/)
                startSel = (int)(itB->pos);
            return;
           }

        if (itB!=end() && itB->type==astItemType_begin && itE!=end() &&  itE->type==astItemType_end)
           { 
            endSel   = (int)(itE->pos + itE->text.size());
            startSel = (int)(itB->pos);
            return;
           }

        if (itE!=end() && itE->type==astItemType_end && itB==end())
           { 
            endSel   = (int)(itE->pos + itE->text.size());
            return;
           }

        if (itB!=end() && itB->type==astItemType_begin && itE==end())
           { 
            startSel = (int)(itB->pos);
            return;
           }

        if (itE!=end())
           {
            int newEndSel   = (int)(itE->pos + itE->text.size());
            if (newEndSel==endSel)
               {
                itE = findPosFwd(endSel+1);
                if (itE!=end())
                   {
                    endSel   = (int)(itE->pos + itE->text.size());
                    return;
                   }
               }
           }

        if (itB!=end())
           startSel = (int)(itB->pos);
/*        
        if (itB!=end())
           {
            newStartSel = (int)(itB->pos);
            if (newStartSel==startSel)
               {
                itB = findPosRwd(startSel-1);
               }
           }
*/

        /* const_iterator itB = findPosRwd(startSel);
         * const_iterator itE = findPosFwd(endSel-1);
         */

/*
        const_iterator itB = findPosRwd(startSel-1);
        const_iterator itE = findPosFwd(endSel);

        if (itB==end())
           {
            if (itE!=end()) ++itE;
           }
        if (itE==end())
           {
            if (itB!=end() && itB!=begin()) --itB;
           }
        if (itB!=end() && itB->type==astItemType_begin && itE!=end() &&  itE->type==astItemType_end)
           { 
            //if (itE!=end()) ++itE;
           }
        else if (itB!=end() && itB->type==astItemType_begin)
           {
            //if (itE!=end()) ++itE;
            if (itB!=end()) ++itB;
            //if (itB!=end() && itB!=begin()) ++itB;
           }
        else if (itE!=end() &&  itE->type==astItemType_end)
           {
            if (itE!=begin()) --itE;
            //if (itB!=end() && itB!=begin()) --itB;
           }

        int newStartSel = startSel;
        if (itB!=end())
           newStartSel = (int)(itB->pos);

        int newEndSel = endSel;
        if (itE!=end())
           newEndSel   = (int)(itE->pos + itE->text.size());

        //if (newStartSel<startSel || newEndSel>endSel)
           {
            startSel = newStartSel;
            endSel   = newEndSel;
        //    return;
           }
*/

/*
        itB = findPosRwd(startSel-1);
        itE = findPosFwd(endSel);

        if (itB==end())
           {
            if (itE!=end()) ++itE;
           }
        if (itE==end())
           {
            if (itB!=end() && itB!=begin()) --itB;
           }
        if (itB!=end() && itB->type==astItemType_begin && itE!=end() &&  itE->type==astItemType_end)
           { 
            //if (itE!=end()) ++itE;
           }
        else if (itB!=end() && itB->type==astItemType_begin)
           {
            //if (itE!=end()) ++itE;
            if (itB!=end()) ++itB;
            //if (itB!=end() && itB!=begin()) ++itB;
           }
        else if (itE!=end() &&  itE->type==astItemType_end)
           {
            if (itE!=begin()) --itE;
            //if (itB!=end() && itB!=begin()) --itB;
           }

        if (itB!=end())
           startSel = (int)(itB->pos);

        if (itE!=end())
           endSel   = (int)(itE->pos + itE->text.size());
*/



/*
        const_iterator itB = findPosRwd(startSel-1);
        const_iterator itE = findPosFwd(endSel);

        if (itB==end())
           {
            if (itE!=end()) ++itE;
           }
        if (itE==end())
           {
            if (itB!=end() && itB!=begin()) --itB;
           }
        if (itB!=end() && itB->type==astItemType_begin && itE!=end() &&  itE->type==astItemType_end)
           { 
            //if (itE!=end()) ++itE;
           }
        else if (itB!=end() && itB->type==astItemType_begin)
           {
            //if (itE!=end()) ++itE;
            if (itB!=end()) ++itB;
            //if (itB!=end() && itB!=begin()) ++itB;
           }
        else if (itE!=end() &&  itE->type==astItemType_end)
           {
            if (itE!=begin()) --itE;
            //if (itB!=end() && itB!=begin()) --itB;
           }

        if (itB!=end())
           startSel = (int)(itB->pos);

        if (itE!=end())
           endSel   = (int)(itE->pos + itE->text.size());
*/

       }


}; // class CLineAstParser


/* struct CBreakSymbols
 * {
 *     std::string                        file_type;
 *     std::vector<std::string>           singles;
 *     std::vector<CBreakSymbolPair>      pairs;
 *     bool                               escapeBs;
 *     bool                               singleQuoteStrings;
 *     bool                               doubleQuoteStrings;
 */


/* struct CAstList
 * {
 *     std::string             beginStr;
 *     std::string             endStr;
 *     std::string::size_type  beginAtPos;
 *     std::string::size_type  endAtPos;
 *  
 *     std::vector<CAstItem>   items;
 */

/* struct CAstItem
 * {
 *     bool                   isAstList;
 *     CAstList              *pList;
 *     bool                   isBreakSymbol;
 *     std::string            literal;
 *     std::string::size_type atPos;
 */



#endif /* AST_H */

