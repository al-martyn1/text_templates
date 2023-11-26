


#if !defined(_IOSTREAM_) && !defined(_STLP_IOSTREAM) && !defined(__STD_IOSTREAM__) && !defined(_CPP_IOSTREAM) && !defined(_GLIBCXX_IOSTREAM)
    #include <iostream>
#endif

#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

#if !defined(_ALGORITHM_) && !defined(_STLP_ALGORITHM) && !defined(__STD_ALGORITHM) && !defined(_CPP_ALGORITHM) && !defined(_GLIBCXX_ALGORITHM)
    #include <algorithm>
#endif

#if !defined(_FUNCTIONAL_) && !defined(_STLP_FUNCTIONAL) && !defined(__STD_FUNCTIONAL__) && !defined(_CPP_FUNCTIONAL) && !defined(_GLIBCXX_FUNCTIONAL)
    #include <functional>
#endif

#if !defined(_VECTOR_) && !defined(_STLP_VECTOR) && !defined(__STD_VECTOR__) && !defined(_CPP_VECTOR) && !defined(_GLIBCXX_VECTOR)
    #include <vector>
#endif

#if !defined(_STACK_) && !defined(_STLP_STACK) && !defined(__STD_STACK__) && !defined(_CPP_STACK) && !defined(_GLIBCXX_STACK)
    #include <stack>
#endif

#if !defined(_EXCEPTION_) && !defined(__EXCEPTION__) && !defined(_STLP_EXCEPTION) && !defined(__STD_EXCEPTION)
    #include <exception>
#endif

#if !defined(_STDEXCEPT_) && !defined(_STLP_STDEXCEPT) && !defined(__STD_STDEXCEPT) && !defined(_CPP_STDEXCEPT) && !defined(_GLIBCXX_STDEXCEPT)
    #include <stdexcept>
#endif


//-----------------------------------------------------------------------------
bool comparePartOfString(const std::string &str, 
                         std::string::size_type pos,
                         const std::string &strToCompare,
                         bool fwd
                         );
bool comparePartOfString(const std::string &str,
                         std::string::size_type pos,
                         const std::string &strToCompare,
                         bool fwd
                         )
   {
    if (fwd)
       {
        if ((pos+strToCompare.size()) > str.size()) return false;
        return str.compare(pos, strToCompare.size(), strToCompare)==0; 
       }
    else
       {
        if (pos<strToCompare.size()) return false;
        return str.compare(pos+1-strToCompare.size(), strToCompare.size(), strToCompare)==0; 
       }
   }


//-----------------------------------------------------------------------------
struct CBreakSymbolPair
{
    std::string begin;
    std::string end;
    CBreakSymbolPair(const std::string& b, const std::string& e)
                    : begin(b), end(e) {} 
    CBreakSymbolPair(const CBreakSymbolPair &bp)
                    : begin(bp.begin), end(bp.end) {} 
    CBreakSymbolPair& operator=(const CBreakSymbolPair &bp)
       {
        if (&bp==this) return *this;
        begin = bp.begin;
        end = bp.end;
        return *this;
       } 

    bool operator< (const CBreakSymbolPair &bp) const
       {
        if (begin.size() > bp.begin.size()) return true;
        if (end.size()   > bp.end.size())   return true;
        return false;
       }

}; // CBreakSymbolPair

//-----------------------------------------------------------------------------
bool lessByLen(const std::string &s1, const std::string &s2)
   {
    return s1.size() > s2.size();
   }

//-----------------------------------------------------------------------------
struct CBreakSymbols
{
    std::string                        file_type;
    std::vector<std::string>           singles;
    std::vector<CBreakSymbolPair>      pairs;
    bool                               escapeBs;
    bool                               escapeDblQuot;
    bool                               singleQuoteStrings;
    bool                               doubleQuoteStrings;

    void sortSymbols()
       {
        std::stable_sort(singles.begin(), singles.end(), lessByLen);
        std::stable_sort(pairs.begin(), pairs.end() );

       }

}; // CSingleBreakSymbolReader



//-----------------------------------------------------------------------------
bool isBreakSymbol(const std::string &str, 
                   const CBreakSymbols &bs,
                   bool  bForward,
                   std::string::size_type fromPos, 
                   bool &bSingle, bool &bBegin,
                   std::string &begin, std::string &end
                  );
bool isBreakSymbol(const std::string &str, 
                   const CBreakSymbols &bs,
                   bool  bForward,
                   std::string::size_type fromPos, 
                   bool &bSingle, bool &bBegin,
                   std::string &begin, std::string &end
                  )
   {
    std::vector<CBreakSymbolPair>::const_iterator pIt = bs.pairs.begin();
    for(; pIt!=bs.pairs.end(); ++pIt)
       {

        if (comparePartOfString(str, fromPos, pIt->begin, bForward))
           {
            bSingle = false;
            begin = pIt->begin;
            end   = pIt->end;
            bBegin = true;
            return true;
           }

        if (comparePartOfString(str, fromPos, pIt->end, bForward))
           {
            bSingle = false;
            begin = pIt->begin;
            end   = pIt->end;
            bBegin = false;
            return true;
           }
       }

    std::vector<std::string>::const_iterator sIt = bs.singles.begin();
    for(; sIt!=bs.singles.end(); ++sIt)
       {
        if (comparePartOfString(str, fromPos, *sIt, bForward))
           {
            bSingle = true;
            begin = *sIt;
            return true;
           }
       }

    return false;
   }
//-----------------------------------------------------------------------------

/*
void test(std::string &testStr, const std::string &searchTo, bool bFwd)
   {
    std::cout<<"Search in ["<<testStr<<"], looking for ["<<searchTo<<"]\n";

    std::string::size_type pos = 0, size = testStr.size();
    for(;pos<size; ++pos)
       {
        if (comparePartOfString(testStr, pos, searchTo, bFwd))
           {
            std::cout<<"Found at pos:"<<(unsigned)pos<<"\n";
            testStr.erase(bFwd ? pos : pos+1-searchTo.size(), searchTo.size());
            return;
           }
       }
   }
*/
//-----------------------------------------------------------------------------
void expandTabs(std::string &str);
void expandTabs(std::string &str)
   {
    const int tabSize = 4; // edinfo.TabSize
    std::string res;
    res.reserve(str.size());
    std::string::size_type pos = 0, size = str.size();
    for(; pos<size; ++pos)
       {
        if (str[pos]=='\t')
           res.append((std::string::size_type)tabSize, ' ');
        else
           res.append(1, str[pos]);
       }
    res.swap(str);
   }

//-----------------------------------------------------------------------------

#include "../src/ast.h"


void astPrintFunctor(int type, unsigned level, std::string::size_type pos, const std::string &str)
   {
    switch(type)
       {
        case astItemType_begin: std::cout<<"+  "; break;
        case astItemType_end:   std::cout<<"-  "; break;
        case astItemType_break: std::cout<<"!  "; break;
        default: std::cout<<"   "; break;
       }
    std::cout<<std::string((std::string::size_type)level*4, ' ');
    std::cout<<"<"<<(unsigned)pos<<":"<<str<<">\n";
   }
//-----------------------------------------------------------------------------
std::string makeSelMaskString(int start, int end, int totalLen)
   {
    std::string res((std::string::size_type)start, ' ');
    res.append((std::string::size_type)end-start, '*');
    res.append((std::string::size_type)totalLen-end, ' ');
    return res;
   }


int main(int argc, char* argv[])
   {
    CBreakSymbols breakSymbols;

    breakSymbols.escapeBs           = false;
    breakSymbols.singleQuoteStrings = true;
    breakSymbols.doubleQuoteStrings = true;
    breakSymbols.escapeDblQuot      = false;

    breakSymbols.singles.push_back(":");
    breakSymbols.singles.push_back("=");
    breakSymbols.pairs.push_back(CBreakSymbolPair("<",">"));
    breakSymbols.pairs.push_back(CBreakSymbolPair("<!--","-->"));
    //breakSymbols.pairs.push_back(CBreakSymbolPair("",""));

    breakSymbols.sortSymbols();

    std::vector<std::string>::const_iterator sIt = breakSymbols.singles.begin();
    for(; sIt!=breakSymbols.singles.end(); ++sIt)
       {
        std::cout<<*sIt<<", ";
       }
    std::cout<<"\n";
    std::vector<CBreakSymbolPair>::const_iterator pIt = breakSymbols.pairs.begin();
    for(; pIt!=breakSymbols.pairs.end(); ++pIt)
       {
        std::cout<<"pair("<<pIt->begin<<","<<pIt->end<<"); ";
       }
    std::cout<<"\n";

    std::string scaleDecades = "0         1         2         3         4         5         6         7         8";
    std::string scaleOnes    = "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";


    //std::string str = "  numSubs += ((RE) currentToken).getNumSubs(\"some quoted \\\"string\\\"\");  ";
    std::string str = "            <!-- <block begin=\"{\" end=\"}\">";
    expandTabs(str);
    std::cout<<"Original string:\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";

    CLineAstParser parser(breakSymbols, str);
    parser.buildAst();

    std::cout<<"\nuse for_each_in_ast\n";
    parser.for_each_in_ast(astPrintFunctor);

    std::cout<<"\nuse iterators\n";
    CLineAstParser::const_iterator astIt = parser.begin();
    for(; astIt!=parser.end(); ++astIt)
       {
        switch(astIt->type)
           {
            case astItemType_begin: std::cout<<"+  "; break;
            case astItemType_end:   std::cout<<"-  "; break;
            case astItemType_break: std::cout<<"!  "; break;
            default: std::cout<<"   "; break;
           }
        std::cout<<std::string((std::string::size_type)(astIt->level)*4, ' ');
        std::cout<<"<"<<(unsigned)astIt->pos<<":"<<astIt->text<<">\n";
       }


    astIt = parser.findPosFwd(16);
    if (astIt!=parser.end())
       {
        std::cout<<"At pos 16: ";
        switch(astIt->type)
           {
            case astItemType_begin: std::cout<<"+  "; break;
            case astItemType_end:   std::cout<<"-  "; break;
            case astItemType_break: std::cout<<"!  "; break;
            default: std::cout<<"   "; break;
           }
        std::cout<<std::string((std::string::size_type)(astIt->level)*4, ' ');
        std::cout<<"<"<<(unsigned)astIt->pos<<":"<<astIt->text<<">\n";
       }


    int maxStringLen = (int)str.size() + 1;
    int maxTries = 0, i = 0;

    int startSel = 10;
    int endSel = startSel;
    maxTries = 6;
    maxStringLen = (int)str.size() + 1;

    std::cout<<"\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";
    for(i=0; i<maxTries; ++i)
       {
        parser.incrementalSelect(startSel, endSel, (int)str.size());
        std::cout<<makeSelMaskString(startSel, endSel, maxStringLen)<<"|\n";
       }
    
    endSel = startSel = 12;
    maxTries = 10;
    std::cout<<"\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";
    for(i=0; i<maxTries; ++i)
       {
        parser.incrementalSelect(startSel, endSel, (int)str.size());
        std::cout<<makeSelMaskString(startSel, endSel, maxStringLen)<<"|\n";
       }

    endSel = startSel = 13;
    maxTries = 10;
    std::cout<<"\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";
    for(i=0; i<maxTries; ++i)
       {
        parser.incrementalSelect(startSel, endSel, (int)str.size());
        std::cout<<makeSelMaskString(startSel, endSel, maxStringLen)<<"|\n";
       }

    endSel = startSel = 17;
    maxTries = 10;
    std::cout<<"\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";
    for(i=0; i<maxTries; ++i)
       {
        parser.incrementalSelect(startSel, endSel, (int)str.size());
        std::cout<<makeSelMaskString(startSel, endSel, maxStringLen)<<"|\n";
       }

    endSel = startSel = 20;
    maxTries = 10;
    std::cout<<"\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";
    for(i=0; i<maxTries; ++i)
       {
        parser.incrementalSelect(startSel, endSel, (int)str.size());
        std::cout<<makeSelMaskString(startSel, endSel, maxStringLen)<<"|\n";
       }

    endSel = startSel = 26;
    maxTries = 10;
    std::cout<<"\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";
    for(i=0; i<maxTries; ++i)
       {
        parser.incrementalSelect(startSel, endSel, (int)str.size());
        std::cout<<makeSelMaskString(startSel, endSel, maxStringLen)<<"|\n";
       }

    endSel = startSel = 29;
    maxTries = 10;
    std::cout<<"\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";
    for(i=0; i<maxTries; ++i)
       {
        parser.incrementalSelect(startSel, endSel, (int)str.size());
        std::cout<<makeSelMaskString(startSel, endSel, maxStringLen)<<"|\n";
       }

    endSel = startSel = 30;
    maxTries = 10;
    std::cout<<"\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";
    for(i=0; i<maxTries; ++i)
       {
        parser.incrementalSelect(startSel, endSel, (int)str.size());
        std::cout<<makeSelMaskString(startSel, endSel, maxStringLen)<<"|\n";
       }

    endSel = startSel = 31;
    maxTries = 10;
    std::cout<<"\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";
    for(i=0; i<maxTries; ++i)
       {
        parser.incrementalSelect(startSel, endSel, (int)str.size());
        std::cout<<makeSelMaskString(startSel, endSel, maxStringLen)<<"|\n";
       }

    endSel = startSel = 35;
    maxTries = 10;
    std::cout<<"\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";
    for(i=0; i<maxTries; ++i)
       {
        parser.incrementalSelect(startSel, endSel, (int)str.size());
        std::cout<<makeSelMaskString(startSel, endSel, maxStringLen)<<"|\n";
       }

    endSel = startSel = 38;
    maxTries = 10;
    std::cout<<"\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";
    for(i=0; i<maxTries; ++i)
       {
        parser.incrementalSelect(startSel, endSel, (int)str.size());
        std::cout<<makeSelMaskString(startSel, endSel, maxStringLen)<<"|\n";
       }

    endSel = startSel = 41;
    maxTries = 10;
    std::cout<<"\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";
    for(i=0; i<maxTries; ++i)
       {
        parser.incrementalSelect(startSel, endSel, (int)str.size());
        std::cout<<makeSelMaskString(startSel, endSel, maxStringLen)<<"|\n";
       }




    return 0;
   }


/*

int main(int argc, char* argv[])
   {
    CBreakSymbols breakSymbols;

    breakSymbols.escapeBs           = true;
    breakSymbols.singleQuoteStrings = true;
    breakSymbols.doubleQuoteStrings = true;
    breakSymbols.escapeDblQuot      = false;

    breakSymbols.singles.push_back(";");
    breakSymbols.singles.push_back(":");
    breakSymbols.singles.push_back("::");
    breakSymbols.singles.push_back(",");
    breakSymbols.singles.push_back(".");
    breakSymbols.singles.push_back("->");
    breakSymbols.singles.push_back("+=");
    breakSymbols.singles.push_back("-=");
    breakSymbols.singles.push_back("*=");
    breakSymbols.singles.push_back("/=");
    breakSymbols.singles.push_back("&=");
    breakSymbols.singles.push_back("|=");
    breakSymbols.singles.push_back("^=");
    breakSymbols.singles.push_back("=");
    breakSymbols.singles.push_back("+");
    breakSymbols.singles.push_back("-");
    breakSymbols.singles.push_back("/");
    breakSymbols.singles.push_back("*");
    breakSymbols.singles.push_back("&");
    breakSymbols.singles.push_back("|");
    breakSymbols.singles.push_back("^");
    breakSymbols.pairs.push_back(CBreakSymbolPair("(",")"));
    breakSymbols.pairs.push_back(CBreakSymbolPair("/ *","* /"));
    breakSymbols.pairs.push_back(CBreakSymbolPair("{","}"));
    breakSymbols.pairs.push_back(CBreakSymbolPair("[","]"));
    breakSymbols.pairs.push_back(CBreakSymbolPair("<",">"));
    //breakSymbols.pairs.push_back(CBreakSymbolPair("",""));

    breakSymbols.sortSymbols();

    std::vector<std::string>::const_iterator sIt = breakSymbols.singles.begin();
    for(; sIt!=breakSymbols.singles.end(); ++sIt)
       {
        std::cout<<*sIt<<", ";
       }
    std::cout<<"\n";
    std::vector<CBreakSymbolPair>::const_iterator pIt = breakSymbols.pairs.begin();
    for(; pIt!=breakSymbols.pairs.end(); ++pIt)
       {
        std::cout<<"pair("<<pIt->begin<<","<<pIt->end<<"); ";
       }
    std::cout<<"\n";

    std::string scaleDecades = "0         1         2         3         4         5         6         7         8";
    std::string scaleOnes    = "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";


    std::string str = "  numSubs += ((RE) currentToken).getNumSubs(\"some quoted \\\"string\\\"\");  ";
    expandTabs(str);
    std::cout<<"Original string:\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";

    CLineAstParser parser(breakSymbols, str);
    parser.buildAst();

    std::cout<<"\nuse for_each_in_ast\n";
    parser.for_each_in_ast(astPrintFunctor);

    std::cout<<"\nuse iterators\n";
    CLineAstParser::const_iterator astIt = parser.begin();
    for(; astIt!=parser.end(); ++astIt)
       {
        switch(astIt->type)
           {
            case astItemType_begin: std::cout<<"+  "; break;
            case astItemType_end:   std::cout<<"-  "; break;
            case astItemType_break: std::cout<<"!  "; break;
            default: std::cout<<"   "; break;
           }
        std::cout<<std::string((std::string::size_type)(astIt->level)*4, ' ');
        std::cout<<"<"<<(unsigned)astIt->pos<<":"<<astIt->text<<">\n";
       }


    astIt = parser.findPosFwd(16);
    if (astIt!=parser.end())
       {
        std::cout<<"At pos 16: ";
        switch(astIt->type)
           {
            case astItemType_begin: std::cout<<"+  "; break;
            case astItemType_end:   std::cout<<"-  "; break;
            case astItemType_break: std::cout<<"!  "; break;
            default: std::cout<<"   "; break;
           }
        std::cout<<std::string((std::string::size_type)(astIt->level)*4, ' ');
        std::cout<<"<"<<(unsigned)astIt->pos<<":"<<astIt->text<<">\n";
       }


    int maxStringLen = (int)str.size() + 1;
    int maxTries = 0, i = 0;

    int startSel = 15;
    int endSel = startSel;
    maxTries = 11;
    maxStringLen = (int)str.size() + 1;

    std::cout<<"\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";
    for(i=0; i<maxTries; ++i)
       {
        parser.incrementalSelect(startSel, endSel, (int)str.size());
        std::cout<<makeSelMaskString(startSel, endSel, maxStringLen)<<"|\n";
       }
    
    endSel = startSel = 24;
    maxTries = 10;
    std::cout<<"\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";
    for(i=0; i<maxTries; ++i)
       {
        parser.incrementalSelect(startSel, endSel, (int)str.size());
        std::cout<<makeSelMaskString(startSel, endSel, maxStringLen)<<"|\n";
       }

    endSel = startSel = 10;
    maxTries = 8;
    std::cout<<"\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";
    for(i=0; i<maxTries; ++i)
       {
        parser.incrementalSelect(startSel, endSel, (int)str.size());
        std::cout<<makeSelMaskString(startSel, endSel, maxStringLen)<<"|\n";
       }

    endSel = startSel = 31;
    maxTries = 8;
    std::cout<<"\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";
    for(i=0; i<maxTries; ++i)
       {
        parser.incrementalSelect(startSel, endSel, (int)str.size());
        std::cout<<makeSelMaskString(startSel, endSel, maxStringLen)<<"|\n";
       }

    endSel = startSel = 37;
    maxTries = 8;
    std::cout<<"\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";
    for(i=0; i<maxTries; ++i)
       {
        parser.incrementalSelect(startSel, endSel, (int)str.size());
        std::cout<<makeSelMaskString(startSel, endSel, maxStringLen)<<"|\n";
       }

    endSel = startSel = 52;
    maxTries = 12;
    std::cout<<"\n"<<scaleDecades<<"\n"<<scaleOnes<<"\n"<<str<<"\n";
    for(i=0; i<maxTries; ++i)
       {
        parser.incrementalSelect(startSel, endSel, (int)str.size());
        std::cout<<makeSelMaskString(startSel, endSel, maxStringLen)<<"|\n";
       }


    return 0;
   }





*/