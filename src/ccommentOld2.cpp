#include <windows.h>
#include <objbase.h>

#include <string.h>
#include <wchar.h>
#include <float.h>



#if !defined(_FUNCTIONAL_) && !defined(_STLP_FUNCTIONAL) && !defined(__STD_FUNCTIONAL__) && !defined(_CPP_FUNCTIONAL) && !defined(_GLIBCXX_FUNCTIONAL)
    #include <functional>
#endif

#if !defined(_ALGORITHM_) && !defined(_STLP_ALGORITHM) && !defined(__STD_ALGORITHM) && !defined(_CPP_ALGORITHM) && !defined(_GLIBCXX_ALGORITHM)
    #include <algorithm>
#endif

#if !defined(_INC_MALLOC) && !defined(_MALLOC_H_) && !defined(_MALLOC_H)
    #include <malloc.h>
#endif

#if !defined(_EXCEPTION_) && !defined(__EXCEPTION__) && !defined(_STLP_EXCEPTION) && !defined(__STD_EXCEPTION)
    #include <exception>
#endif

#if !defined(_STDEXCEPT_) && !defined(_STLP_STDEXCEPT) && !defined(__STD_STDEXCEPT) && !defined(_CPP_STDEXCEPT) && !defined(_GLIBCXX_STDEXCEPT)
    #include <stdexcept>
#endif

#if !defined(_VECTOR_) && !defined(_STLP_VECTOR) && !defined(__STD_VECTOR__) && !defined(_CPP_VECTOR) && !defined(_GLIBCXX_VECTOR)
    #include <vector>
#endif

#if !defined(_MAP_) && !defined(_STLP_MAP) && !defined(__STD_MAP__) && !defined(_CPP_MAP) && !defined(_GLIBCXX_MAP)
    #include <map>
#endif

#if !defined(_MEMORY_) && !defined(_STLP_MEMORY) && !defined(__STD_MEMORY__) && !defined(_CPP_MEMORY) && !defined(_GLIBCXX_MEMORY)
    #include <memory>
#endif






#if !defined(_SSTREAM_) && !defined(_STLP_SSTREAM) && !defined(__STD_SSTREAM__) && !defined(_CPP_SSTREAM) && !defined(_GLIBCXX_SSTREAM)
    #include <sstream>
#endif



#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit

#include <atlbase.h>
#include <atldef.h>


//#include <stdio.h>
//#include "far/plugin.hpp"
#include "cfarcls.h"
#include "ttpconf.h"

#include "ttprc.h"
#include "tpl.h"
#include "utils.h"
#include "ttpinput.h"
#include "settings.h"
#include "autocom.h"
#include "ccomment.h"

#include "far/farkeys.hpp"

#define SS std::stringstream ss


extern Far::CPluginStartupInfo         Info;
extern TextTP::TTPConfig               Config;

namespace TextTP
{


void commentText(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos);
void uncommentText(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos);
void SelectCommentedText(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos);

void indentText(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos);
void unindentText(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos);
void indentText1(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos);
void unindentText1(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos);

void selectLinesBlockUp(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos);
void selectLinesBlockDown(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos);
void selectCodeBlock(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos);
void selectCodeBlockAndHeader(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos);

void indentTextAux(int useTabs, int tabSize,
                   int startLine, int startPos,
                   int endLine, int endPos);
void unindentTextAux(int useTabs, int tabSize,
                   int startLine, int startPos,
                   int endLine, int endPos);

void commentSingleLine( const EditorInfo &edinfo,
                        bool selected,
                        int startLine, int startPos,
                        int endLine, int endPos,
                        int leftMostPos,
                        int blockWidth,
                        bool sequenceOnly,
                        const CommentInfo &ci                        
                      );



/*
#define INIT_MENU_STRING(var, rcId, str)   \
        if (!var.Text)                     \
           {                               \
            var.Text = ::Info.GetMsg(rcId);\
            if (!var.Text)                 \
               var = str;                  \
           }

*/

#define INIT_MENU_STRING(var, rcId, str)      \
        do {                                  \
            const char *tmp = ::Info.GetMsg(rcId);  \
            lstrcpy(var, tmp!=0 ? tmp : str); \
           } while(0)

//-----------------------------------------------------------------------------
void doCommentingJob()
   {
    struct FarMenuItem menuItems[12] = { 0 };
    INIT_MENU_STRING(menuItems[0].Text,  IDM_COMMENT_SELECTION, "&C Comment selection");
    INIT_MENU_STRING(menuItems[1].Text,  IDM_UNCOMMENT_SELECTION, "&U Uncomment selection");
    INIT_MENU_STRING(menuItems[2].Text,  IDM_SELECT_COMMENTED, "&S Select commented");
    menuItems[3].Separator = 1;
    INIT_MENU_STRING(menuItems[4].Text,  IDM_INC_INDENT, "&I Increase indent");
    INIT_MENU_STRING(menuItems[5].Text,  IDM_DEC_INDENT, "&D Decrease indent");
    INIT_MENU_STRING(menuItems[6].Text,  IDM_INC_INDENT1, "&+ Increase indent (1 ws)");
    INIT_MENU_STRING(menuItems[7].Text,  IDM_DEC_INDENT1, "&- Decrease indent (1 ws)");
    menuItems[8].Separator = 1;
    INIT_MENU_STRING(menuItems[9].Text,  IDM_SELECT_LINES_BLOCK_UP, "&L Select lines block (up)");
    INIT_MENU_STRING(menuItems[10].Text,  IDM_SELECT_LINES_BLOCK_DOWN, "&L Select lines block (down)");
    INIT_MENU_STRING(menuItems[11].Text, IDM_SELECT_CODE_BLOCK, "&B Select code block");
    INIT_MENU_STRING(menuItems[12].Text, IDM_SELECT_CODE_BLOCK_AND_HEADER, "&H Select code block and header");
    //INIT_MENU_STRING(menuItems[].Text, , );

    char title[128];
    INIT_MENU_STRING(title, IDM_COMMENTING_CODE, "Commenting code");

    int menuRes = ::Info.Menu( -1, -1, 0,
                               FMENU_WRAPMODE, title, 0,
                               0, /* Help topic */
                               0, 0, 
                               menuItems,
                               sizeof(menuItems)/sizeof(menuItems[0])
                             );
    if (menuRes<0 || menuRes>sizeof(menuItems)/sizeof(menuItems[0]))
       {
        return;
       }

    EditorInfo edinfo;
    ::Info.EditorCtrlGetInfo( &edinfo );

/*
    std::stringstream ss;

    ss<<"FileName  : "<<edinfo.FileName<<"\n"
      <<"TotalLines: "<<edinfo.TotalLines<<"\n"
      <<"CurLine   : "<<edinfo.CurLine<<", CurPos: "<<edinfo.CurPos<<"\n";
    ss<<"BlockType : ";
    switch(edinfo.BlockType)
        {
         case BTYPE_NONE:    ss<<"BTYPE_NONE\n"; break;
         case BTYPE_STREAM:  ss<<"BTYPE_STREAM\n"; break;
         case BTYPE_COLUMN:  ss<<"BTYPE_COLUMN\n"; break;
         default:            ss<<"UNKNOWN\n";
        };
    ss<<"BlockStartLine: "<<edinfo.BlockStartLine<<"\n"
      <<"TabSize: "<<edinfo.TabSize<<", ExpandTabs: ";
    if (edinfo.Options&EOPT_EXPANDTABS) ss<<"true\n";
    else                                ss<<"false\n";    
*/

    // endLine - line after last selected
    int startLine = edinfo.CurLine, endLine = edinfo.CurLine+1;
    int startPos = 0, endPos = 0;
    bool selected = false;

    if (edinfo.BlockType==BTYPE_STREAM || edinfo.BlockType==BTYPE_COLUMN)
       {
        selected = true;
        startLine = edinfo.BlockStartLine;
        for(endLine=startLine; endLine<edinfo.TotalLines; ++endLine)
           {
            EditorGetString egs = { endLine, 0, 0, 0, 0, 0};
            if (!::Info.EditorCtrlGetString(&egs))
               {
                return; //failed to get info about string
               }

            if (egs.SelStart<0) // Line does not contain selection
               break;

            if (egs.SelEnd>=0)
               endPos = egs.SelEnd;

            if (endLine==startLine)
               startPos = egs.SelStart;

            //SS;
            //ss<<"Main, selected, startPos: "<<startPos<<",  endPos: "<<endPos;
            //Warning(IDM_COMMENTING_CODE, ss.str());
           }
        if (endPos==0)
           {
            endPos = -1; // последняя строка выделена до конца
            --endLine;
           }
        if (startLine==(endLine-1) && startPos==endPos)
           {
            edinfo.BlockType=BTYPE_NONE;
            selected = false;
           }
       }

    //SS;
    //ss<<"Main, selected, startPos: "<<startPos<<",  endPos: "<<endPos<<",  startLine: "<<startLine<<",  endLine: "<<endLine;
    //Warning(IDM_COMMENTING_CODE, ss.str());

    switch(menuRes)
       {
        case 0:
               commentText(edinfo, selected, startLine, startPos, endLine, endPos);
               break;
        case 1:
               uncommentText(edinfo, selected, startLine, startPos, endLine, endPos);
               break;
        case 2:
               SelectCommentedText(edinfo, selected, startLine, startPos, endLine, endPos);
               break;

        case 3: // separator
               break;

        case 4:
               indentText(edinfo, selected, startLine, startPos, endLine, endPos);
               break;
        case 5:
               unindentText(edinfo, selected, startLine, startPos, endLine, endPos);
               break;
        case 6:
               indentText1(edinfo, selected, startLine, startPos, endLine, endPos);
               break;
        case 7:
               unindentText1(edinfo, selected, startLine, startPos, endLine, endPos);
               break;

        case 8: // separator
               break;

        case 9:
               selectLinesBlockUp(edinfo, selected, startLine, startPos, endLine, endPos);
               break;
        case 10:
               selectLinesBlockDown(edinfo, selected, startLine, startPos, endLine, endPos);
               break;
        case 11:
               selectCodeBlock(edinfo, selected, startLine, startPos, endLine, endPos);
               break;
        case 12:
               selectCodeBlockAndHeader(edinfo, selected, startLine, startPos, endLine, endPos);
               break;
       }
   }
//-----------------------------------------------------------------------------
int findFirstNonSpace(const EditorInfo &edinfo, const char *str, int strLen)
   {
    //int res = 0;
    for(int pos=0; pos<strLen; ++pos, ++str)
       {
        if (*str!=' ' && *str!='\t') return pos;
       }
    return -1;
   }
//-----------------------------------------------------------------------------
int findLeftmostNonSpace(const EditorInfo &edinfo, int startLine, int endLine)
   {
    int res = 32767; // line max size
    for(;startLine!=endLine; ++startLine)
       {
        EditorGetString egs = { startLine, 0, 0, 0, 0, 0};
        if (!::Info.EditorCtrlGetString(&egs))
           return 0; //failed to get info about string

        if (!egs.StringText)
           continue;

        int tmpRes = findFirstNonSpace(edinfo, 
                                       egs.StringText, 
                                       egs.StringLength);
        if (tmpRes<0) continue;
        if (res>tmpRes) res = tmpRes;
       }
    if (res==32767) res = 0;
    return res;
   }
//-----------------------------------------------------------------------------
int findLastNonSpace(const EditorInfo &edinfo, const char *str, int strLen)
   {
    for(int pos=strLen; pos; --pos)
       {
        if (str[pos-1]!=' ' && str[pos-1]!='\t')
           break; // return pos;
       }
    return pos;
   }
//-----------------------------------------------------------------------------
int findMaxStringLen(const EditorInfo &edinfo, int startLine, int endLine)
   {
    int res = 0; // line max size
    for(;startLine!=endLine; ++startLine)
       {
        EditorGetString egs = { startLine, 0, 0, 0, 0, 0};
        if (!::Info.EditorCtrlGetString(&egs))
           return res; //failed to get info about string
        int tmpRes = findLastNonSpace( edinfo, 
                                       egs.StringText, 
                                       egs.StringLength);
        if (res<tmpRes) res = tmpRes;
        //if (res<egs.StringLength) res = egs.StringLength;
       }
    return res;
   }
//-----------------------------------------------------------------------------
void indentTextAux(int useTabs, int tabSize,
                   int startLine, int startPos,
                   int endLine, int endPos)
   {
    for(;startLine!=endLine; ++startLine)
       {
        EditorGetString egs = { startLine, 0, 0, 0, 0, 0};
        if (!::Info.EditorCtrlGetString(&egs))
           return; //failed to get info about string

        if (!egs.StringText)
           continue;

        char *buf = (char*)_alloca(egs.StringLength+tabSize+1);
        char *buf2 = buf;
        int newSize = egs.StringLength;
        if (useTabs)
           {
            *buf2++ = '\t';
            ++newSize;
           }
        else
           {
            for(int i=0; i<tabSize; ++i)
               *buf2++ = ' ';
            newSize += tabSize;
           }
        memcpy((void*)buf2, (void*)egs.StringText, (size_t)egs.StringLength);
        buf2[egs.StringLength] = 0;
   
        EditorSetString ess = { startLine, buf, 
                                (char*)egs.StringEOL, 
                                newSize };
        ::Info.EditorCtrlSetString(&ess);
       }
   }
//-----------------------------------------------------------------------------
void indentText(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
    indentTextAux( (edinfo.Options&EOPT_EXPANDTABS) ? 0 : 1,
                   edinfo.TabSize, startLine, startPos, endLine, endPos);
   }
//-----------------------------------------------------------------------------
void indentText1(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
    indentTextAux( 0, 1, startLine, startPos, endLine, endPos);
   }
//-----------------------------------------------------------------------------
void unindentTextAux(int useTabs, int tabSize,
                     int startLine, int startPos,
                     int endLine, int endPos)
   {
    for(;startLine!=endLine; ++startLine)
       {
        EditorGetString egs = { startLine, 0, 0, 0, 0, 0};
        if (!::Info.EditorCtrlGetString(&egs))
           return; //failed to get info about string

        if (!egs.StringText) continue;
   
        const char *pTxt = egs.StringText;
        int i = 0;
        for( ; i<tabSize; ++i, ++pTxt)
           {
            if (*pTxt==' ') continue;
            if (*pTxt=='\t')
               { ++pTxt; ++i; }
            break;
           }
   
        EditorSetString ess = { startLine, (char*)pTxt, 
                                (char*)egs.StringEOL, 
                                egs.StringLength - i
                              };
        ::Info.EditorCtrlSetString(&ess);
       }
   }
//-----------------------------------------------------------------------------
void unindentText(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
    unindentTextAux( (edinfo.Options&EOPT_EXPANDTABS) ? 0 : 1,
                     edinfo.TabSize, startLine, startPos, endLine, endPos);
   }
//-----------------------------------------------------------------------------
void unindentText1(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
    unindentTextAux( 0, 1, startLine, startPos, endLine, endPos);
   }
//-----------------------------------------------------------------------------
void selectLinesBlock(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
    std::string line;
    if (!::Info.EditorCtrlGetString(edinfo.CurLine, line)) 
       return;

    if (Utils::isSpaceOnlyString(line)) 
       return; // do nothing

    int selStartLine = edinfo.CurLine;
    for (; selStartLine>=0; --selStartLine)
        {
         if (!::Info.EditorCtrlGetString(selStartLine, line)) 
             return;
         if (Utils::isSpaceOnlyString(line)) break;
        }
    selStartLine++;

    int selEndLine = edinfo.CurLine;
    for (; selEndLine<edinfo.TotalLines; ++selEndLine)
        {
         if (!::Info.EditorCtrlGetString(selEndLine, line)) 
             return;
         if (Utils::isSpaceOnlyString(line)) break;
        }

    ::Info.EditorCtrlSelect( BTYPE_STREAM, selStartLine, 
                             0, -1, selEndLine - selStartLine);
   }
//-----------------------------------------------------------------------------
void selectLinesBlockUp(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
   }
//-----------------------------------------------------------------------------
void selectLinesBlockDown(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
   }
//-----------------------------------------------------------------------------

/* return 1 if block begin
          0 if nothing was found
         -1 if block end

*/
int selBlockTestString(const SelectionBlockInfo &sbi, const std::string &str, bool caseSens);
int selBlockTestString(const SelectionBlockInfo &sbi, const std::string &line, bool caseSens)
   {
    std::string str = Utils::ltrim(line, ::Info.StdFn());

    std::string beginStr = sbi.begin;
    std::string endStr = sbi.end;

    if (!caseSens)
       {
        str      = Utils::strlwr(str, ::Info.StdFn());
        beginStr = Utils::strlwr(beginStr, ::Info.StdFn());
        endStr   = Utils::strlwr(endStr, ::Info.StdFn());
       }
    if (Utils::strStartsWith(str, beginStr)) return 1;
    if (Utils::strStartsWith(str, endStr))   return -1;
    return 0;
   }
//-----------------------------------------------------------------------------
struct FoundBlockBeginInfo
{
    SelectionBlockInfo  blockInfo;
    int                 foundAtLine;

    FoundBlockBeginInfo(const SelectionBlockInfo &sbi, int lineNo)
        : blockInfo(sbi), foundAtLine(lineNo) {}
    FoundBlockBeginInfo(const FoundBlockBeginInfo &fbbi)
        : blockInfo(fbbi.blockInfo), foundAtLine(fbbi.foundAtLine) {}
    FoundBlockBeginInfo& operator=(const FoundBlockBeginInfo &fbbi)
        {
         if (&fbbi==this) return *this;
         blockInfo = fbbi.blockInfo;
         foundAtLine = fbbi.foundAtLine;
         return *this;
        }
    bool operator<(const FoundBlockBeginInfo &fbbi) const
        {
         return foundAtLine<fbbi.foundAtLine;
        }
    bool operator>(const FoundBlockBeginInfo &fbbi) const
        {
         return foundAtLine>fbbi.foundAtLine;
        }
};
//-----------------------------------------------------------------------------
bool findBlockBegin(FoundBlockBeginInfo &fbbi, bool caseSens);
bool findBlockBegin(FoundBlockBeginInfo &fbbi, bool caseSens)
   {
    int cnt = 0;
    for(; fbbi.foundAtLine>=0; --fbbi.foundAtLine)
       {
        std::string lineText;
        if (!::Info.EditorCtrlGetString(fbbi.foundAtLine, lineText)) 
           {
            fbbi.foundAtLine = -1;
            return false;
           }

        int strType = selBlockTestString(fbbi.blockInfo, lineText, caseSens);

        if (!strType) 
           continue; // line has no type

        if (strType<0)
           {
            --cnt;
            continue;
           }

        if (!cnt) return true;
        ++cnt;
       }
    fbbi.foundAtLine = -1;
    return false;
   }
//-----------------------------------------------------------------------------
int findBlockEnd(const FoundBlockBeginInfo &fbbi, bool caseSens, int fromLine, int totalLines);
int findBlockEnd(const FoundBlockBeginInfo &fbbi, bool caseSens, int fromLine, int totalLines)
   {
    int cnt = 0;
    for(; fromLine<totalLines; ++fromLine)
       {
        std::string lineText;
        if (!::Info.EditorCtrlGetString(fromLine, lineText)) 
            return -1;

        int strType = selBlockTestString(fbbi.blockInfo, lineText, caseSens);
        if (!strType) 
           continue; // line has no type

        if (strType>0)
           {
            ++cnt;
            continue;
           }
        if (!cnt) return fromLine;
        --cnt;
       }
    return -1;
   }
//-----------------------------------------------------------------------------
bool selectCodeBlockAux(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos,
                 int &retSelStart, int &retSelEnd,
                 SelectionInfo &selInfo);
bool selectCodeBlockAux(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos,
                 int &retSelStart, int &retSelEnd,
                 SelectionInfo &selInfo)
   {
    if (!edinfo.FileName) return false;

    if (selected)
       {
        --startLine;
        ++endLine;
       }

    TextTP::TTPConfig  cfgCopy = Config;
    readCustomConfig(edinfo.FileName, cfgCopy);
    std::string editorFileType;
    if (!lookupForFileType(cfgCopy, edinfo.FileName, editorFileType))
        {
         selectLinesBlock( edinfo, selected, 
                           startLine, startPos, 
                           endLine, endPos);
         return false;
        } 

    std::vector<SelectionInfo> selectionInfoVec;
    findInfo(editorFileType, cfgCopy.selection_info.begin(), cfgCopy.selection_info.end(), selectionInfoVec);
    if (selectionInfoVec.empty())
        {
         selectLinesBlock( edinfo, selected, 
                           startLine, startPos, 
                           endLine, endPos);
         return false;
        } 

    //SelectionInfo selInfo(selectionInfoVec[0]);
    selInfo = selectionInfoVec[0];
    //caseSens

    std::vector<FoundBlockBeginInfo> foundBlockBeginsVec;
    std::vector<SelectionBlockInfo>::const_iterator bIt =  selInfo.selBlockInfoVec.begin();
    for(; bIt!=selInfo.selBlockInfoVec.end(); ++bIt)
       {
        FoundBlockBeginInfo fbbi(*bIt, startLine);
        if (!findBlockBegin(fbbi, selInfo.case_sensitive)) continue;
        foundBlockBeginsVec.push_back(fbbi);
       }
    std::sort(foundBlockBeginsVec.begin(), foundBlockBeginsVec.end(), std::greater<FoundBlockBeginInfo>());

    std::vector<FoundBlockBeginInfo>::const_iterator dbbIt = foundBlockBeginsVec.begin();
    int fbeRes = -1;
    for(; dbbIt!=foundBlockBeginsVec.end(); ++dbbIt)
       {
        fbeRes = findBlockEnd(*dbbIt, selInfo.case_sensitive, endLine, edinfo.TotalLines);
        if (fbeRes>=0) break;
       }

    if (fbeRes<0) return false;
    
    retSelStart = dbbIt->foundAtLine;
    retSelEnd   = fbeRes;
    return true;
   }
//-----------------------------------------------------------------------------
void selectCodeBlock(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
    int retSelStart = -1, retSelEnd = -1;
    SelectionInfo selInfo;
    if (!selectCodeBlockAux( edinfo, selected, 
                             startLine, startPos,
                             endLine, endPos,
                             retSelStart, retSelEnd,
                             selInfo))
       return;

    //SS;
    //ss<<"Found block, start line: "<<retSelStart<<"\n"
    //  <<"edinfo.CurLine: "<<edinfo.CurLine<<"\n"
    //  <<"startLine: "<<startLine<<",  startPos: "<<startPos<<"\n"
    //  <<"endLine  : "<<endLine  <<",  endPos  : "<<endPos<<"\n"
    //  <<"selected: "<<selected<<",  ";
    //  if (edinfo.BlockType==BTYPE_STREAM) ss<<"BlockType==BTYPE_STREAM\n";
    //  else if (edinfo.BlockType==BTYPE_COLUMN) ss<<"BlockType==BTYPE_COLUMN\n";
    //  else ss<<"BlockType==BTYPE_NONE\n";
    //Warning(IDM_COMMENTING_CODE, ss.str());

    ::Info.EditorCtrlSelect( BTYPE_STREAM, retSelStart, 
                             0, -1, retSelEnd - retSelStart + 1);
   }
//-----------------------------------------------------------------------------
bool selBlockAndHeaderTestString(const std::vector<SelectionBlockInfo> &vec, const std::string &line, bool caseSens);
bool selBlockAndHeaderTestString(const std::vector<SelectionBlockInfo> &vec, const std::string &line, bool caseSens)
   {
    if (Utils::isSpaceOnlyString(line)) 
       {
        //Warning(IDM_COMMENTING_CODE, "stops at space only string");
        return true; // stops at space only string
       }

    std::vector<SelectionBlockInfo>::const_iterator it = vec.begin();
    for(; it!=vec.end(); ++it)
       {
        int sbtRes = selBlockTestString(*it, line, caseSens);
        if (sbtRes)
           {
            //SS;
            //ss<<"stops at begin/end block string, "<<sbtRes/*<<" at line "<<line*/<<"\n";
            //ss<<"BlockInfo.begin: ["<<it->begin<<"],  BlockInfo.end: "<<it->end;
            //Warning(IDM_COMMENTING_CODE, ss.str());
            return true; // stops at begin/end block string
           }
       }
    return false;
   }
//-----------------------------------------------------------------------------
void selectCodeBlockAndHeader(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
    int retSelStart = -1, retSelEnd = -1;
    SelectionInfo selInfo;
    if (!selectCodeBlockAux( edinfo, selected, 
                             startLine, startPos,
                             endLine, endPos,
                             retSelStart, retSelEnd,
                             selInfo))
       return;
    
    int i = retSelStart - 1;

    //SS;
    //ss<<"Found block, start line: "<<retSelStart<<",  i: "<<i<<"\n"
    //  <<"edinfo.CurLine: "<<edinfo.CurLine<<"\n"
    //  <<"startLine: "<<startLine<<",  startPos: "<<startPos<<"\n"
    //  <<"endLine  : "<<endLine  <<",  endPos  : "<<endPos<<"\n"
    //  <<"selected: "<<selected<<",  ";
    //  if (edinfo.BlockType==BTYPE_STREAM) ss<<"BlockType==BTYPE_STREAM\n";
    //  else if (edinfo.BlockType==BTYPE_COLUMN) ss<<"BlockType==BTYPE_COLUMN\n";
    //  else ss<<"BlockType==BTYPE_NONE\n";
    //Warning(IDM_COMMENTING_CODE, ss.str());

    for (; i>=0; --i)
        {
         std::string line;
         if (!::Info.EditorCtrlGetString(i, line)) 
            break;
         if (selBlockAndHeaderTestString(selInfo.selBlockInfoVec, line, selInfo.case_sensitive))
            break;
        }
    if (i>=0)
       {
        //Warning(IDM_COMMENTING_CODE, "Found stop string");
        retSelStart = i+1;
       }
    else
       {
        //Warning(IDM_COMMENTING_CODE, "Stop string not found, select block only");
       }

    ::Info.EditorCtrlSelect( BTYPE_STREAM, retSelStart, 
                             0, -1, retSelEnd - retSelStart + 1);
   }
//-----------------------------------------------------------------------------
void SelectCommentedText(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
    if (!edinfo.FileName) return;

    TextTP::TTPConfig  cfgCopy = Config;
    readCustomConfig(edinfo.FileName, cfgCopy);
    std::string editorFileType;
    if (!lookupForFileType(cfgCopy, edinfo.FileName, editorFileType))
        {
         /*
         std::stringstream ss;
         ss<<"File type not found for file: "<<edinfo.FileName;
         Warning(IDM_COMMENTING_CODE, ss.str());
         */
         return;
        } 

    std::vector<CommentInfo> commentSequences;
    findInfo( editorFileType, 
              cfgCopy.comment_sequences.begin(), 
              cfgCopy.comment_sequences.end(), 
              commentSequences);
    
    /*
    std::stringstream ss;
    ss<<"Editor file type: "<<editorFileType<<"\n";

    std::vector<CommentInfo>::const_iterator csIt = commentSequences.begin();
    for(; csIt!=commentSequences.end(); ++csIt)
       {
        ss<<"Found comment sequence for file types: "<<csIt->file_type<<"\n";
       }
    Warning(IDM_COMMENTING_CODE, ss.str());
    */

    if (commentSequences.empty()) return;
    CommentInfo ci = commentSequences[0];



   }
//-----------------------------------------------------------------------------
void commentSingleLine( const EditorInfo &edinfo,
                        bool selected,
                        int startLine, int startPos,
                        int endLine, int endPos,
                        int leftMostPos,
                        int blockWidth,
                        bool sequenceOnly,
                        const CommentInfo &ci                        
                      )
   {
    EditorGetString egs = { startLine, 0, 0, 0, 0, 0};
    if (!::Info.EditorCtrlGetString(&egs))
       return; //failed to get info about string
    std::string line((char*)egs.StringText, (std::string::size_type)egs.StringLength);



   }
//-----------------------------------------------------------------------------
CommentTypeEnum findCommentInfoByType( const std::vector<CommentInfo> &ciVec, 
                                       CommentInfo &foundCi, 
                                       CommentTypeEnum requestedCommentType);
CommentTypeEnum findCommentInfoByType( const std::vector<CommentInfo> &ciVec, 
                                       CommentInfo &foundCi, 
                                       CommentTypeEnum requestedCommentType)
   {
    std::vector<CommentInfo>::const_iterator it = ciVec.begin();
    //for()
    return requestedCommentType;
   }
//-----------------------------------------------------------------------------
void commentText(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
    if (!edinfo.FileName) return;

    TextTP::TTPConfig  cfgCopy = Config;
    readCustomConfig(edinfo.FileName, cfgCopy);
    std::string editorFileType;
    if (!lookupForFileType(cfgCopy, edinfo.FileName, editorFileType))
        {
         return;
        } 

    std::vector<CommentInfo> commentSequences;
    findInfo(editorFileType, cfgCopy.comment_sequences.begin(), cfgCopy.comment_sequences.end(), commentSequences);
    
    if (commentSequences.empty()) return;
    CommentInfo ci = commentSequences[0];


    // Данный код отыскивает начало и конец текста по горизонтали 
    // - первый не пробельный символ и последний, и выделяет блок
    int leftMostPos = findLeftmostNonSpace(edinfo, startLine, endLine);
    int maxStrLen   = findMaxStringLen(edinfo, startLine, endLine);
    int blockWidth  = maxStrLen ? (int)(maxStrLen-leftMostPos) : (int)(0);
    //
    //::Info.EditorCtrlSelect( blockWidth ? BTYPE_COLUMN : BTYPE_NONE, 
    //                         startLine, leftMostPos,
    //                         blockWidth, endLine-startLine);

    

    CommentTypeEnum commentType = ctStream;

    // Если не выделено ничего, то коментируем одну строку как stream
    if (!selected)
       {
        startPos = 0; endPos = -1; // как будто выделена одна строка целиком
       }
    else if (edinfo.BlockType==BTYPE_STREAM && startPos==0 && endPos==-1)
       { // будем коментировать блок строк, а не произвольный фрагмент
        commentType = ctLines;
       }

//       if (edinfo.BlockType==BTYPE_STREAM || edinfo.BlockType==BTYPE_COLUMN)



/*

    if (!ci.begin_sequence.empty() && ci.end_sequence.empty())
       ci.end_sequence = ci.begin_sequence;
    if (ci.begin_sequence.empty() && !ci.end_sequence.empty())
       ci.begin_sequence = ci.end_sequence;

    bool sequenceOnly = ci.begin_sequence.empty() && ci.end_sequence.empty();
    if (sequenceOnly && ci.sequence.empty())
       {
        std::string warn = "Sequence not taken for comment (file type: ";
        warn += editorFileType;
        warn += ")";
        Warning(IDM_COMMENTING_CODE, warn);
        return;
       }

    std::string newLineEnd = "\r\n";
    if (ci.indent>64) ci.indent = 64;
    std::string indent = makeSpaceString(ci.indent);

    //::Info.EditorCtrlInsertNewString( startLine, "AAA" );
*/



/*
    if (startLine==endLine-1)
       {
        commentSingleLine( edinfo, selected,
                           startLine, startPos,
                           endLine, endPos,
                           leftMostPos, blockWidth,
                           sequenceOnly, ci);
        return;
       }
*/

/*
    for(int lineNo = startLine; lineNo!=endLine; ++lineNo)
       {
        EditorGetString egs = { lineNo, 0, 0, 0, 0, 0};
        if (!::Info.EditorCtrlGetString(&egs))
           return; //failed to get info about string

        std::string line((char*)egs.StringText, (std::string::size_type)egs.StringLength);
        if (lineNo==startLine)
           { // first line
            if (sequenceOnly)
               line.insert((std::string::size_type)leftMostPos, ci.sequence+indent);
            else
               line.insert((std::string::size_type)leftMostPos, ci.begin_sequence+indent);
           }

        if (lineNo==endLine-1)
           { // last line
            if (sequenceOnly)
               line.insert((std::string::size_type)leftMostPos, ci.sequence + indent);
            else
               {
                if (!ci.end_at_new_line)
                   { // do same as for middle line
                    if (ci.sequence.empty())
                       {
                        line.insert( (std::string::size_type)leftMostPos, 
                                     makeSpaceString((int)ci.begin_sequence.size()) + indent);
                        line += ci.end_sequence;
                       }
                    else if (lineNo==startLine)
                       line.insert( (std::string::size_type)leftMostPos, 
                                    makeSpaceString(ci.new_line_indent_before) + ci.sequence + indent);
                   }
               } // if (sequenceOnly)           
           }
        if (lineNo!=startLine && lineNo!=endLine-1)
        // line in the middle
           {
            if (ci.sequence.empty())
               line.insert( (std::string::size_type)leftMostPos, 
                            makeSpaceString((int)ci.begin_sequence.size()) + indent);
            else
               line.insert( (std::string::size_type)leftMostPos, 
                            makeSpaceString(ci.new_line_indent_before) + ci.sequence + indent);
           } 
        EditorSetString ess = { lineNo, (char*)line.data(), 
                                (char*)egs.StringEOL, 
                                (int)line.size()
                              };
        ::Info.EditorCtrlSetString(&ess);

        if (lineNo==endLine-1)
           newLineEnd = egs.StringEOL;
       } // for(int lineNo = startLine; lineNo!=endLine; ++lineNo)

    if (ci.end_at_new_line)
       {
        ::Info.EditorCtrlInsertNewString( endLine, 
                                          makeSpaceString(leftMostPos) 
                                        + makeSpaceString(ci.new_line_indent_before) 
                                        + ci.end_sequence
                                        );
       }
*/

   }
//-----------------------------------------------------------------------------
void uncommentText(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
   }
//-----------------------------------------------------------------------------
void configureCommenting()
   {
   }
//-----------------------------------------------------------------------------



}; // namespace TextTP
