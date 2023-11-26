
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

#ifndef STRICT
    #define STRICT
#endif

#if !defined(_WINDOWS_)
    #include <windows.h>
#endif

#if !defined(_OBJBASE_H_)
    #include <objbase.h>
#endif

#if !defined(_INC_STRING) && !defined(__STRING_H_) && !defined(_STRING_H)
    #include <string.h>
#endif

#if !defined(_INC_WCHAR) && !defined(_WCHAR_H_) && !defined(_WCHAR_H)
    #include <wchar.h>
#endif

#if !defined(_INC_FLOAT) && !defined(_FLOAT_H_) && !defined(_FLOAT_H)
    #include <float.h>
#endif

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


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit

#ifndef __ATLBASE_H__
    #include <atlbase.h>
#endif

#ifndef __ATLDEF_H__
    #include <atldef.h>
#endif


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


#if !defined(_SSTREAM_) && !defined(_STLP_SSTREAM) && !defined(__STD_SSTREAM__) && !defined(_CPP_SSTREAM) && !defined(_GLIBCXX_SSTREAM)
    #include <sstream>
#endif

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

void moveToSelStart(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos);
void moveToSelEnd(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos);
void clearSelection(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos);
void incrementalSelection(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos);


void indentTextAux(int useTabs, int tabSize,
                   int startLine, int startPos,
                   int endLine, int endPos);
void unindentTextAux(int useTabs, int tabSize,
                   int startLine, int startPos,
                   int endLine, int endPos);


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
    struct FarMenuItem menuItems[18] = { 0 };
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
    INIT_MENU_STRING(menuItems[13].Text, IDM_INCREMENTAL_SELECTION, "&N Incremental selection");

    menuItems[14].Separator = 1;
    INIT_MENU_STRING(menuItems[15].Text, IDM_MOVE_TO_SELECTION_START, "&M Move to selection start");
    INIT_MENU_STRING(menuItems[16].Text, IDM_MOVE_TO_SELECTION_END  , "&E Move to selection end"  );
    INIT_MENU_STRING(menuItems[17].Text, IDM_CLEAR_SELECTION        , "&T Clear selection"        );
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


    // SS;
    // ss//<<"FileName  : "<<edinfo.FileName<<"\n"
    //   //<<"TotalLines: "<<edinfo.TotalLines<<"\n"
    //   <<"CurLine   : "<<edinfo.CurLine<<", CurPos: "<<edinfo.CurPos<<"\n";
    // ss<<"BlockType : ";
    
    // switch(edinfo.BlockType)
    //     {
    //      case BTYPE_NONE:    ss<<"BTYPE_NONE\n"; break;
    //      case BTYPE_STREAM:  ss<<"BTYPE_STREAM\n"; break;
    //      case BTYPE_COLUMN:  ss<<"BTYPE_COLUMN\n"; break;
    //      default:            ss<<"UNKNOWN\n";
    //     };
    
    // ss<<"BlockStartLine: "<<edinfo.BlockStartLine<<"\n";
    //  <<"TabSize: "<<edinfo.TabSize<<", ExpandTabs: ";
    //if (edinfo.Options&EOPT_EXPANDTABS) ss<<"true\n";
    //else                                ss<<"false\n";    



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
            //ss<<"---------\ni: "<<endLine<<"\n";

            EditorGetString egs = { endLine, 0, 0, 0, 0, 0};
            if (!::Info.EditorCtrlGetString(&egs))
               {
                return; //failed to get info about string
               }

            if (egs.SelStart<0) // Line does not contain selection
               {
                //ss<<"selStart<0, break\n";
                break;
               }

            if (egs.SelEnd>=0)
               {
                //ss<<"egs.SelEnd>=0, endPos = egs.SelEnd, endPos: "<<endPos<<"\n";
                endPos = egs.SelEnd;
                //ss<<"endPos: "<<endPos<<"\n";
               }

            if (endLine==startLine)
               {
                //ss<<"endLine==startLine, do startPos = egs.SelStart, startPos: "<<startPos<<"\n";
                startPos = egs.SelStart;
                //ss<<"startPos: "<<startPos<<"\n";
               }

            //SS;
            //ss<<"Main, selected, startPos: "<<startPos<<",  endPos: "<<endPos;
            //Warning(IDM_COMMENTING_CODE, ss.str());
           }
        //Warning(IDM_COMMENTING_CODE, ss.str());
        //ss<<"/**************/\n";
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
        if (startPos<0)
           {
            startPos = 0;
            endPos = -1;
           }
        if (startLine==endLine) ++endLine;
        // ss<<"startLine: "<<startLine<<",  endLine: "<<endLine<<"\n";
        // ss<<"startPos: "<<startPos<<",  endPos: "<<endPos<<"\n";
        //Warning(IDM_COMMENTING_CODE, ss.str());
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
        case 13:
               incrementalSelection(edinfo, selected, startLine, startPos, endLine, endPos);
               break;

        case 15:
               moveToSelStart(edinfo, selected, startLine, startPos, endLine, endPos);
               break;
        case 16:
               moveToSelEnd(edinfo, selected, startLine, startPos, endLine, endPos);
               break;
        case 17:
               clearSelection(edinfo, selected, startLine, startPos, endLine, endPos);
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
inline
bool isSpaceOnlyString(int lineNo, bool spaceOnly)
   {
    std::string line;
    if (!::Info.EditorCtrlGetString(lineNo, line)) 
       return false;
    return Utils::isSpaceOnlyString(line)==spaceOnly;
   }

//-----------------------------------------------------------------------------
// return -1 if line not found
int findSpaceOnlyString(int startLine, int endLine, int direction, bool spaceOnly);
int findSpaceOnlyString(int startLine, int endLine, int direction, bool spaceOnly)
   {
    if (!direction) direction = 1;
    else if (direction<0) direction = -1;
    else direction = 1;

    for(; startLine!=endLine; startLine += direction)
       {
        if (isSpaceOnlyString(startLine, spaceOnly)) break;
       }
    if (startLine==endLine) return -1;
    return startLine;
   }

//-----------------------------------------------------------------------------
void selectLinesBlockUp(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
    int selStartLine = startLine;
    int selEndLine = endLine;
    if (!selected)
       { 
        if (isSpaceOnlyString(edinfo.CurLine, true /*spaceOnly*/))
           { // курсор стоит на пустой строке
             // выделяем блок, который расположен выше по тексту,
             // но не далее трех строк от текущей
            selEndLine = findSpaceOnlyString( edinfo.CurLine, 
                                         -1 /* up range */, 
                                         -1 /* dir - up*/, 
                                         false /*!spaceOnly*/);
            if (selEndLine<0)
              return; // block begin not found at all

            if ((edinfo.CurLine-selEndLine)>3)
               return; // found block is too far from cur line

            ++selEndLine; // selEndLine is next line after needed
           }
        else
           { // выделяем блок, конец которого расположен ниже по тексту
            selEndLine = findSpaceOnlyString( edinfo.CurLine, 
                                              edinfo.TotalLines /* down range */, 
                                              1 /* dir - down*/, 
                                              true /*spaceOnly*/);
            if (selEndLine<0)
               selEndLine = edinfo.TotalLines;
           }

        selStartLine = findSpaceOnlyString( selEndLine-1, 
                                          -1 /* up range */, 
                                          -1 /* dir - up*/, 
                                          true /*spaceOnly*/);
        if (selStartLine<0)
           selStartLine = 0;
        else
           ++selStartLine;
        // ::Info.EditorCtrlSelect( BTYPE_STREAM, selStartLine, 
        //                          0, -1, 
        //                          selEndLine - selStartLine);
        ::Info.EditorCtrlSelect( BTYPE_STREAM, selStartLine, 
                                 0, 0, 
                                 selEndLine - selStartLine + 1);
        return;
       }

    selStartLine = findSpaceOnlyString( startLine-1, 
                                        -1 /* up range */, 
                                        -1 /* dir - down*/, 
                                        false /*!spaceOnly*/);
    if (selStartLine<0)
        return; // no next block found, BOF reached

    selStartLine = findSpaceOnlyString( selStartLine-1, 
                                        -1 /* up range */, 
                                        -1 /* dir - down*/, 
                                        true /*spaceOnly*/);
    if (selStartLine<0)
       selStartLine = 0;
    else
       ++selStartLine;

    // ::Info.EditorCtrlSelect( BTYPE_STREAM, selStartLine, 
    //                          0, -1, 
    //                          selEndLine - selStartLine + 1);
    ::Info.EditorCtrlSelect( BTYPE_STREAM, selStartLine, 
                             0, 0, 
                             selEndLine - selStartLine + 1);

   }

//-----------------------------------------------------------------------------
void selectLinesBlockDown(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
    int selStartLine = startLine;
    int selEndLine = endLine;
    if (!selected)
       { 
        if (isSpaceOnlyString(edinfo.CurLine, true /*spaceOnly*/))
           { // курсор стоит на пустой строке
             // выделяем блок, который расположен ниже по тексту,
             // но не далее трех строк от текущей
            selStartLine = findSpaceOnlyString( edinfo.CurLine+1, 
                                                edinfo.TotalLines /* down range */, 
                                                1 /* dir - down*/, 
                                                false /*!spaceOnly*/);
            if (selStartLine<0)
               return; // block begin not found at all

            if ((selStartLine-edinfo.CurLine)>3)
               return; // found block is too far from cur line
           }
        else
           { // выделяем блок, начало которого расположено выше по тексту
            selStartLine = findSpaceOnlyString( edinfo.CurLine-1, 
                                                -1 /* up range */, 
                                                -1 /* dir - up*/, 
                                                true /*spaceOnly*/);
            if (selStartLine<0)
               selStartLine = 0;
            else
               selStartLine++;
           }
       
        selEndLine = findSpaceOnlyString( selStartLine, 
                                          edinfo.TotalLines /* down range */, 
                                          1 /* dir - down*/, 
                                          true /*spaceOnly*/);
        if (selEndLine<0)
           {
            selEndLine = edinfo.TotalLines;
           }

        // ::Info.EditorCtrlSelect( BTYPE_STREAM, selStartLine, 
        //                          0, -1, 
        //                          selEndLine - selStartLine);
        ::Info.EditorCtrlSelect( BTYPE_STREAM, selStartLine, 
                                 0, 0, 
                                 selEndLine - selStartLine + 1);
        return;
       }

    selEndLine = findSpaceOnlyString( endLine+1, 
                                      edinfo.TotalLines /* down range */, 
                                      1 /* dir - down*/, 
                                      false /*!spaceOnly*/);
    if (selEndLine<0)
        return; // no next block found, EOF reached

    selEndLine = findSpaceOnlyString( selEndLine+1, 
                                      edinfo.TotalLines /* down range */, 
                                      1 /* dir - down*/, 
                                      true /*spaceOnly*/);
    if (selEndLine<0)
       {
        selEndLine = edinfo.TotalLines;
       }

    // ::Info.EditorCtrlSelect( BTYPE_STREAM, selStartLine, 
    //                          0, -1, 
    //                          selEndLine - selStartLine);
    ::Info.EditorCtrlSelect( BTYPE_STREAM, selStartLine, 
                             0, 0, 
                             selEndLine - selStartLine + 1);
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

    int selStartLine = findSpaceOnlyString( edinfo.CurLine, 
                                            -1 /* up range */, 
                                            -1 /* dir - up*/, 
                                            true /*spaceOnly*/);
    if (selStartLine<0) // строка с пробелами не найдена,
       { // абзац идет с начала текста
        selStartLine = 0;
       }
    else
       {
        ++selStartLine;
       }

    int selEndLine = findSpaceOnlyString( edinfo.CurLine, 
                                          edinfo.TotalLines /* down range */, 
                                          1 /* dir - down*/, 
                                          true /*spaceOnly*/);
    if (selEndLine<0)
       {
        selEndLine = edinfo.TotalLines;
       }

    ::Info.EditorCtrlSelect( BTYPE_STREAM, selStartLine, 
                             0, 0, 
                             selEndLine - selStartLine + 1);
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
                             0, 0, 
                             retSelEnd - retSelStart + 1);
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
                             0, 0, 
                             retSelEnd - retSelStart + 1);
   }

//-----------------------------------------------------------------------------
CommentTypeEnum findCommentInfoByType( const std::vector<CommentInfo> &ciVec, 
                                       CommentInfo &foundCi, 
                                       CommentTypeEnum requestedCommentType);
CommentTypeEnum findCommentInfoByType( const std::vector<CommentInfo> &ciVec, 
                                       CommentInfo &foundCi, 
                                       CommentTypeEnum requestedCommentType)
   {
    //SS;
    std::vector<CommentInfo>::const_iterator it = ciVec.begin();
    for(; it!=ciVec.end(); ++it)
       {
        //ss<<"it->comment_type: "<<it->comment_type<<",   requestedCommentTypeЖ"<<requestedCommentType<<"\n";
        
        if (it->comment_type==requestedCommentType)
           {
            //ss<<"FoundType:  "<<it->comment_type<<"\n";
            //::Info.EditorCtrlInsertStrings( 150, ss.str() );
            foundCi = *it;
            return it->comment_type; // requestedCommentType;
           }
        //ss<<"\n";
       }

    foundCi = ciVec[0];

    //ss<<"Not found, return :  "<<foundCi.comment_type<<"\n";
    //::Info.EditorCtrlInsertStrings( 150, ss.str() );

    return foundCi.comment_type;
   }

//-----------------------------------------------------------------------------
inline
std::string appendWhiteSpaces(const std::string &s, int lenNeeded)
   {
    if (s.size() >= (std::string::size_type)lenNeeded )
       return s;

    return s + std::string( 
                           (std::string::size_type)lenNeeded - s.size(),
                           ' '
                          );
   }

//-----------------------------------------------------------------------------
void commentText(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
    if (!edinfo.FileName) return;

    if (startPos > endPos && startPos>=0 && endPos>=0)
       std::swap(startPos, endPos);

    TextTP::TTPConfig  cfgCopy = Config;
    readCustomConfig(edinfo.FileName, cfgCopy);
    std::string editorFileType;
    if (!lookupForFileType(cfgCopy, edinfo.FileName, editorFileType))
        {
         return;
        } 

    std::vector<CommentInfo> commentSequences;
    findInfo( editorFileType, 
              cfgCopy.comment_sequences.begin(), 
              cfgCopy.comment_sequences.end(), 
              commentSequences);
    
    if (commentSequences.empty()) return;

    CommentTypeEnum commentType = ctStream;
    
    if (!selected)
       { // Если не выделено ничего, то коментируем одну строку как lines
        startPos = 0; endPos = -1; // как будто выделена одна строка целиком
        commentType = ctLines;
       }
    else if (edinfo.BlockType==BTYPE_STREAM && startPos==0 && endPos==-1)
       { // будем коментировать блок строк, а не произвольный фрагмент
        commentType = ctLines;
       }

    CommentInfo ci;

    CommentTypeEnum foundCommentType = findCommentInfoByType( commentSequences, ci, commentType);
    //commentType = findCommentInfoByType( commentSequences, ci, commentType);

    if (foundCommentType!=commentType)
       { // комент нужного типа не найден
        if (commentType==ctLines)
           { // был заказан stream, найден lines
            // lines можно коментить stream'ом
            //commentType = ctLines;
           }
        else
           { // был заказан lines, найден stream
            // lines можно коментить stream'ом
            commentType = ctLines;
           }       
       }

    std::string beginSequence = ci.begin_sequence;
    std::string endSequence   = ci.end_sequence;

    if (!beginSequence.empty() && endSequence.empty())
       endSequence = beginSequence;
    if (beginSequence.empty() && !endSequence.empty())
       beginSequence = endSequence;

    std::string sequence = ci.sequence;

    if (beginSequence.empty())
       beginSequence = sequence;
    if (endSequence.empty())
       endSequence = sequence;

    if (commentType==ctStream)
       {
        std::string commentBegin = makeSpaceString(ci.new_line_indent_before)
                                 + beginSequence 
                                 + makeSpaceString(ci.indent);
        
        std::string commentEnd   = makeSpaceString(ci.indent) 
                                 + endSequence
                                 + makeSpaceString(ci.new_line_indent_before);

        if (edinfo.BlockType==BTYPE_COLUMN)
           {                                 // NOTE: less was
            for(int lineNo = startLine; lineNo!=endLine; ++lineNo)
               {
                std::string str, eol;
                if (!::Info.EditorCtrlGetString(lineNo, str, eol))
                   continue;
                str = appendWhiteSpaces(str, endPos+1);
                str.insert((std::string::size_type)endPos, commentEnd);
                str.insert((std::string::size_type)startPos, commentBegin);
                ::Info.EditorCtrlSetString(lineNo, str, eol);
               }

            endPos += (int)commentBegin.size();
            endPos += (int)commentEnd.size();
            
            if (selected)
               {
                ::Info.EditorCtrlSelect( BTYPE_COLUMN, startLine, 
                                         startPos, endPos-startPos, 
                                         endLine - startLine);
               }
           }
        else // if (edinfo.BlockType==BTYPE_COLUMN)
           {
            std::string str, eol;
            if (::Info.EditorCtrlGetString(startLine, str, eol))
               {
                str.insert((std::string::size_type)startPos, commentBegin);
                ::Info.EditorCtrlSetString(startLine, str, eol);
               }

            if (::Info.EditorCtrlGetString(endLine-1, str, eol))
               {
                if (endPos==-1)
                   {                    
                    str += commentEnd;
                   }
                else
                   {
                    int pos = endPos;
                    if (startLine==(endLine-1))
                       pos += (int)commentBegin.size();
                    str = appendWhiteSpaces(str, pos+1);
                    str.insert((std::string::size_type)pos, commentEnd);
                    endPos += (int)commentEnd.size();
                   }
                ::Info.EditorCtrlSetString(endLine-1, str, eol);
               }
            if (startLine==(endLine-1) && endPos!=-1)
               {
                endPos += (int)commentBegin.size();
               }

            if (selected)
               {
                ::Info.EditorCtrlSelect( BTYPE_STREAM, startLine, 
                                         startPos, endPos-startPos, 
                                         endLine - startLine);
               }
           } // else if (edinfo.BlockType==BTYPE_COLUMN)
       } // if (commentType==ctStream)
    else
       {
        int leftmostWSPos = findLeftmostNonSpace(edinfo, startLine, endLine);
        //if (startPos<leftmostWSPos)
        startPos = leftmostWSPos;

        std::string commentBegin = //makeSpaceString(ci.new_line_indent_before) + 
                                   beginSequence + 
                                   makeSpaceString(ci.indent);

        std::string commentMid   = makeSpaceString(ci.new_line_indent_before);// +
        if (ci.sequence.empty())
           {
            commentMid += makeSpaceString((int)beginSequence.size());
           }
        else
           {
            commentMid += sequence;
           }
        commentMid += makeSpaceString(ci.indent);
                                         // NOTE: less was
        for(int lineNo = startLine; lineNo!=endLine; ++lineNo)
           {
            std::string str, eol;
            if (!::Info.EditorCtrlGetString(lineNo, str, eol))
               continue;

            str = appendWhiteSpaces(str, startPos+1);

            if (lineNo==startLine)
               { // first line
                str.insert((std::string::size_type)startPos, commentBegin);
                if (lineNo==endLine && !ci.end_at_new_line && !ci.end_sequence.empty())
                   str += makeSpaceString(ci.indent) + endSequence;
               }
            else
               {
                str.insert((std::string::size_type)startPos, commentMid);
                if (lineNo==(endLine-1) && !ci.end_at_new_line && !ci.end_sequence.empty())
                   str += makeSpaceString(ci.indent) + endSequence;
               }

            ::Info.EditorCtrlSetString(lineNo, str, eol);
           }

        if (ci.end_at_new_line)
           {
            ::Info.EditorCtrlInsertNewString( endLine-1, 
                                              makeSpaceString(startPos) + 
                                              makeSpaceString(ci.new_line_indent_before) + 
                                              endSequence 
                                            );
            endLine++;
           }
        /*
        else
           {
            std::string str, eol;
            if (::Info.EditorCtrlGetString(lineNo, str, eol))
               {
                str += makeSpaceString(ci.indent) + endSequence;
                ::Info.EditorCtrlSetString(lineNo, str, eol);
               }
           }
        */

        if (selected)
           {
            ::Info.EditorCtrlSelect( BTYPE_STREAM, startLine, 
                                     0, 0, 
                                     endLine - startLine + 1);
           }
       } // else if (commentType==ctStream)

   }

//-----------------------------------------------------------------------------
int findCommentStart(const std::string &str, const std::string &commentBeginStr, int fromPos);
int findCommentStart(const std::string &str, const std::string &commentBeginStr, int fromPos)
   {
    if (str.size() < commentBeginStr.size()) return -1;
    fromPos += (int)commentBeginStr.size();
    if (fromPos>=(int)str.size())
       fromPos = (int)str.size();

    std::string::size_type fndPos = str.rfind(commentBeginStr, (std::string::size_type)fromPos);
    if (fndPos==std::string::npos) return -1;
    return (int)fndPos;
   }

//-----------------------------------------------------------------------------
int findCommentEnd(const std::string &str, const std::string &commentEndStr, int fromPos);
int findCommentEnd(const std::string &str, const std::string &commentEndStr, int fromPos)
   {
    if (str.size() < commentEndStr.size()) return -1;
    fromPos -= (int)commentEndStr.size();
    if (fromPos<0)
       fromPos = 0;

    std::string::size_type fndPos = str.find(commentEndStr, (std::string::size_type)fromPos);
    if (fndPos==std::string::npos) return -1;
    return (int)fndPos;
   }

//-----------------------------------------------------------------------------
bool findCommentInLine(const std::string &str, 
                       const std::string &commentBeginStr,
                       const std::string &commentEndStr, 
                       int fromPos, int &foundBegin, int &foundEnd);
bool findCommentInLine(const std::string &str, 
                       const std::string &commentBeginStr,
                       const std::string &commentEndStr, 
                       int fromPos, int &foundBegin, int &foundEnd)
   {
    foundBegin = findCommentStart(str, commentBeginStr, fromPos);
    if (foundBegin<0) return false;

    foundEnd = findCommentEnd(str, commentEndStr, foundBegin+(int)commentBeginStr.size()+1);
    //foundEnd = findCommentEnd(str, commentEndStr, foundBegin+(int)commentBeginStr.size());
    if (foundEnd<0) return false;

    foundEnd += (int)commentEndStr.size();

    return true;
   }

//-----------------------------------------------------------------------------
bool findIndentAtStart(const std::string &str, int fromPos, int indentSize);
bool findIndentAtStart(const std::string &str, int fromPos, int indentSize)
   {
    int base = fromPos - indentSize - 1;
    for (int i=indentSize; i; --i)
       {
        int pos = base + i;
        if (pos<0) return true;
        if (pos>=(int)str.size()) continue;
        if (str[pos]!=' ') return false;
       }
    return true;
   }

//-----------------------------------------------------------------------------
bool findIndentAtEnd(const std::string &str, int fromPos, int indentSize);
bool findIndentAtEnd(const std::string &str, int fromPos, int indentSize)
   {
    int size = (int)str.size();
    for(int i=0; i<indentSize; ++i)
       {
        int pos = fromPos + i;
        if (pos>=size) return true;
        if (str[pos]!=' ') return false;
       }
    return true;
   }

//-----------------------------------------------------------------------------
bool findSelectColumnComment( const EditorInfo &edinfo, 
                              const std::vector<CommentInfo> &commentSequences,
                              bool bEraseDontSelect);
bool findSelectColumnComment( const EditorInfo &edinfo, 
                              const std::vector<CommentInfo> &commentSequences,
                              bool bEraseDontSelect)
   {
    CommentTypeEnum commentType = ctStream;
    CommentInfo ci;
    CommentTypeEnum foundCommentType = findCommentInfoByType( commentSequences, ci, commentType);
    if (foundCommentType!=commentType)
       return false; // Подходящий тип коментария не найден

    // пробуем отыскать закоментированный вертикальный блок
    std::string beginSequence = ci.begin_sequence;
    std::string endSequence   = ci.end_sequence;

    if (!beginSequence.empty() && endSequence.empty())
       endSequence = beginSequence;
    if (beginSequence.empty() && !endSequence.empty())
       beginSequence = endSequence;

    std::string line;
    if (!::Info.EditorCtrlGetString(edinfo.CurLine, line))
       return false;

    int selStart = 0, selEnd = 0;
    if (!findCommentInLine( line, beginSequence, endSequence, 
                            edinfo.CurPos, selStart, selEnd ))
       return false; // коментарий не найден
       
    bool indentAtStart = findIndentAtStart(line, selStart, ci.new_line_indent_before);
    bool indentAtEnd   = findIndentAtEnd(line, selEnd, ci.new_line_indent_before);

    bool indentAfterStart = findIndentAtEnd(line, selStart+(int)beginSequence.size(), ci.indent);
    bool indentBeforeEnd  = findIndentAtStart(line, selEnd-(int)endSequence.size(), ci.indent);

    int selStartLine = edinfo.CurLine;
    int selEndLine = edinfo.CurLine + 1;

    for(--selStartLine; selStartLine>=0; --selStartLine)
       {
        if (!::Info.EditorCtrlGetString(selStartLine, line))
           return false;

        int ss = 0, se = 0;
        if (!findCommentInLine( line, beginSequence, endSequence, 
                                edinfo.CurPos, ss, se ))
           break; // коментарий не найден
       
        if (ss!=selStart || se!=selEnd)
           break; // коментарий найден не в той же позиции

        indentAtStart &= findIndentAtStart(line, selStart, ci.new_line_indent_before);
        indentAtEnd   &= findIndentAtEnd(line, selEnd, ci.new_line_indent_before);

        indentAfterStart &= findIndentAtEnd(line, selStart+(int)beginSequence.size(), ci.indent);
        indentBeforeEnd  &= findIndentAtStart(line, selEnd-(int)endSequence.size(), ci.indent);
       }

    if (selStartLine<0) selStartLine = 0;
    else ++selStartLine;


    for(; selEndLine<edinfo.TotalLines; ++selEndLine)
       {
        if (!::Info.EditorCtrlGetString(selEndLine, line))
           return false;
       
        int ss = 0, se = 0;
        if (!findCommentInLine( line, beginSequence, endSequence, 
                                edinfo.CurPos, ss, se ))
           break; // коментарий не найден

        if (ss!=selStart || se!=selEnd)
           break; // коментарий найден не в той же позиции

        indentAtStart &= findIndentAtStart(line, selStart, ci.new_line_indent_before);
        indentAtEnd   &= findIndentAtEnd(line, selEnd, ci.new_line_indent_before);

        indentAfterStart &= findIndentAtEnd(line, selStart+(int)beginSequence.size(), ci.indent);
        indentBeforeEnd  &= findIndentAtStart(line, selEnd-(int)endSequence.size(), ci.indent);
       }

    // end
    int selStartIndentAfterPos = selStart + (int)beginSequence.size();
    if (indentAfterStart)
       selStartIndentAfterPos += ci.indent;

    int selEndIndentBeforePos = selEnd - (int)endSequence.size();
    if (indentBeforeEnd)
       selEndIndentBeforePos -= ci.indent;

    if (indentAtStart)
       selStart -= ci.new_line_indent_before;
    if (selStart<0)
       selStart = 0;
    if (indentAtEnd)
       selEnd += ci.new_line_indent_before;

    
    if (bEraseDontSelect)
       { // erase found block
        std::string eol;
        for(int lineNo=selStartLine; lineNo<selEndLine; ++lineNo)
           {
            if (!::Info.EditorCtrlGetString(lineNo, line, eol))
               continue;
            line.erase( (std::string::size_type)selEndIndentBeforePos, 
                        (std::string::size_type)(selEnd-selEndIndentBeforePos)
                      );
            line.erase( (std::string::size_type)selStart, 
                        (std::string::size_type)(selStartIndentAfterPos-selStart)
                      );
            ::Info.EditorCtrlSetString(lineNo, line, eol);
           }

        selEnd -= (selStartIndentAfterPos-selStart) 
                + (selEnd-selEndIndentBeforePos);
        ::Info.EditorCtrlSelect( BTYPE_COLUMN, selStartLine,
                                 selStart, selEnd-selStart, 
                                 selEndLine - selStartLine);
        // ::Info.EditorCtrlSelect( BTYPE_COLUMN, selStartLine,
        //                          selStart, selStartIndentAfterPos-selStart, 
        //                          selEndLine - selStartLine);

        // ::Info.EditorCtrlSelect( BTYPE_COLUMN, selStartLine,
        //                          selEndIndentBeforePos, selEnd-selEndIndentBeforePos, 
        //                          selEndLine - selStartLine);
       }
    else
       {
        ::Info.EditorCtrlSelect( BTYPE_COLUMN, selStartLine,
                                 selStart, selEnd-selStart, 
                                 selEndLine - selStartLine);
       }
    return true;
   }

//-----------------------------------------------------------------------------
// return -1 if not found
//         0 if found middle
//         1 if begin found
//         2 if end found
int testLineStartsWithComment(const std::string &str, 
                              const std::string &beginSequence,
                              const std::string &endSequence,
                              const std::string &sequence,
                              int &foundPos);
int testLineStartsWithComment(const std::string &str, 
                              const std::string &beginSequence,
                              const std::string &endSequence,
                              const std::string &sequence,
                              int &foundPos)
   {
    int size = (int)str.size();
    for(foundPos = 0; foundPos<size; ++foundPos)
       {
        if (str[foundPos]==' ' || str[foundPos]=='\t')
           continue;
        break;
       }
    if (foundPos>=size) return -1;

    std::string tmp(str, (std::string::size_type)foundPos);
    bool testEndSeqBeforeMid = false;
    if (endSequence!=sequence && !sequence.empty() && !endSequence.empty() && Utils::strStartsWith(endSequence, sequence))
       {
        testEndSeqBeforeMid = true;
       }

    if (!testEndSeqBeforeMid)
       {
        if (!sequence.empty()      && Utils::strStartsWith(tmp, sequence)) return 0;
       }

    if (!beginSequence.empty() && Utils::strStartsWith(tmp, beginSequence)) return 1;
    if (!endSequence.empty()   && Utils::strStartsWith(tmp, endSequence)) return 2;

    if (testEndSeqBeforeMid)
       {
        if (!sequence.empty()      && Utils::strStartsWith(tmp, sequence)) return 0;
       }

    return -1;
   }

//-----------------------------------------------------------------------------
bool findSelectLinesBlockComment( const EditorInfo &edinfo, 
                              const std::vector<CommentInfo> &commentSequences,
                              bool bEraseDontSelect);
bool findSelectLinesBlockComment( const EditorInfo &edinfo, 
                              const std::vector<CommentInfo> &commentSequences,
                              bool bEraseDontSelect)
   {
    CommentTypeEnum commentType = ctLines;
    CommentInfo ci;
    CommentTypeEnum foundCommentType = findCommentInfoByType( commentSequences, ci, commentType);
    //if (foundCommentType!=commentType)
    //   return false; // Подходящий тип коментария не найден

    std::string beginSequence = ci.begin_sequence;
    std::string endSequence   = ci.end_sequence;

    if (!beginSequence.empty() && endSequence.empty())
       endSequence = beginSequence;
    if (beginSequence.empty() && !endSequence.empty())
       beginSequence = endSequence;

    std::string sequence = ci.sequence;

    //if (beginSequence.empty())
    //   beginSequence = sequence; beginSequence endSequence
    //if (endSequence.empty())
    //   endSequence = sequence;

    int firstLine = edinfo.CurLine;
    std::string line;
    int firstLineFoundPos = 0;
    int testRes = -1;

    for(; firstLine>=0; --firstLine)
       {
        if (!::Info.EditorCtrlGetString(firstLine, line))
           {
            return false;
           }
        testRes = testLineStartsWithComment(line, 
                                            beginSequence,
                                            endSequence,
                                            sequence,
                                            firstLineFoundPos);

        //SS;
        //ss<<"testRes: "<<testRes<<"\n";
        //Warning(IDM_COMMENTING_CODE, ss.str());

        if (testRes==1) 
           {
            //Warning(IDM_COMMENTING_CODE, "01");
            break; // begin found
           }
        if (testRes==-1) 
           {
            //Warning(IDM_COMMENTING_CODE, "02");
            break; // nothing found
           }
        if (testRes==2 && firstLine!=edinfo.CurLine) 
           {
            //Warning(IDM_COMMENTING_CODE, "03");
            break; // end found
           }
       }

    if (firstLine<0)
       {
        //Warning(IDM_COMMENTING_CODE, "04");
        firstLine++;
       }

    if (testRes==-1 && firstLine==edinfo.CurLine)
       {
        //Warning(IDM_COMMENTING_CODE, "1");
        return false; // not found at all
       }

    if (testRes==-1) // на предыдущей строке что-то было
       {
        //Warning(IDM_COMMENTING_CODE, "2");
        firstLine++;
       }

    if (testRes==2 && beginSequence==endSequence)
       {
        //Warning(IDM_COMMENTING_CODE, "3");
        testRes = 1; // begin found, not end, they there are equals
       }

    if (testRes==2)
       {
        //Warning(IDM_COMMENTING_CODE, "4");
        return false;
       }
    


    int endLine = edinfo.CurLine+1;
    int lastLineFoundPos = 0;
    testRes = -1;

    for(; endLine<edinfo.TotalLines; ++endLine)
       {
        if (!::Info.EditorCtrlGetString(endLine, line))
           {
            //Warning(IDM_COMMENTING_CODE, "findSelectLinesBlockComment - 1");
            return false;
           }
        testRes = testLineStartsWithComment(line, 
                                            beginSequence,
                                            endSequence,
                                            sequence,
                                            lastLineFoundPos);
        //if (testRes<0) return false; // no found at all
        //if (testRes) continue;
        //break;
        if (testRes==1) 
           {
            //Warning(IDM_COMMENTING_CODE, "5");
            break; // begin found
           }
        if (testRes==-1) 
           {
            //Warning(IDM_COMMENTING_CODE, "6");
            break; // nothing found
           }
        if (testRes==2) 
           {
            //Warning(IDM_COMMENTING_CODE, "7");
            break; // end found
           }
       }

    //if (testRes==-1 && firstLine==edinfo.CurLine)
    //   {
    //   }

    //Warning(IDM_COMMENTING_CODE, "8");

    if (testRes==1 && beginSequence==endSequence)
       {
        Warning(IDM_COMMENTING_CODE, "9");
        testRes = 2; // end found, not begin, they there are equals
       }

    if (testRes==1)
       {
        //Warning(IDM_COMMENTING_CODE, "10");
        return false; // begin found - error
       }

    if (testRes==2)
       {
        //Warning(IDM_COMMENTING_CODE, "11");
        ++endLine;
       }

    if (!testRes && endSequence.size() && !Utils::strEndsWith(line, endSequence, ::Info.StdFn()))
       { // последним найден ограничитель средней строки
         // и строка не заканчивается подстрокой "конец коментария"
        //Warning(IDM_COMMENTING_CODE, "12");
        if (sequence!=endSequence)
           { // середина и конец комента различаются
            // Warning(IDM_COMMENTING_CODE, "8");
            return false;           
           }
       }

    //bool strEndsWith(line, endSequence, ::Info.StdFn())

    if (!bEraseDontSelect)
       {
        ::Info.EditorCtrlSelect( BTYPE_STREAM, firstLine,
                                 0, 0, 
                                 endLine - firstLine + 1);
        return true;
       }

    std::string eol;
    for(int lineNo=firstLine; lineNo<endLine; ++lineNo)
       {
        if (!::Info.EditorCtrlGetString(lineNo, line, eol))
           continue;
        int atPos = 0;
        testRes = testLineStartsWithComment(line, 
                                            beginSequence,
                                            endSequence,
                                            sequence,
                                            atPos);
        if (testRes<0) continue;
        int rmLen = 0;
        switch(testRes)
           {

            case 0: rmLen = (int)sequence.size(); break;
           
            case 1: rmLen = (int)beginSequence.size(); break;
           
            case 2: rmLen = (int)endSequence.size(); break;
           
           };

        // SS;
        // ss<<"atPos: "<<atPos<<",  testRes: "<<testRes<<"\n";
        // Warning(IDM_COMMENTING_CODE, ss.str());


        if (rmLen)
           {
            line.erase((std::string::size_type)atPos, (std::string::size_type)rmLen);
           }

        if (lineNo==firstLine)
           {
            if (findIndentAtEnd(line, atPos, ci.indent))
               {
                line.erase((std::string::size_type)atPos, (std::string::size_type)ci.indent);
               }
           }
        else
           {
            if (findIndentAtEnd(line, atPos, ci.indent))
               {
                line.erase((std::string::size_type)atPos, (std::string::size_type)ci.indent);
               }
            if (findIndentAtStart(line, atPos, ci.new_line_indent_before))
               {
                int rmPos = atPos-ci.new_line_indent_before;
                if (rmPos<0) rmPos = 0;
                line.erase((std::string::size_type)rmPos, (std::string::size_type)ci.new_line_indent_before);
               }           
           }

        if (lineNo==(endLine-1) && ci.end_at_new_line && Utils::isSpaceOnlyString(line))
           {
            // delete string
            ::Info.EditorCtrlDeleteLine(lineNo);
            --endLine;           
           }
        else
           {
            // если строка последняя, и заканчивается коментом, то удаляем камент и отступ indent
            if (lineNo==(endLine-1))
               {
                std::string lineCopy = Utils::rtrim(line, ::Info.StdFn());
                if (Utils::strEndsWith(lineCopy, endSequence))
                   {
                    lineCopy.erase(lineCopy.size()-endSequence.size());
                    int pos = (int)lineCopy.size() - ci.indent;
                    if (pos<0) pos = 0;
                    if (findIndentAtEnd(lineCopy, (int)pos, ci.indent))
                       lineCopy.erase((std::string::size_type)pos, (std::string::size_type)ci.indent);
                    line = lineCopy;
                   }
               }
            ::Info.EditorCtrlSetString(lineNo, line, eol);
           }
       }

    ::Info.EditorCtrlSelect( BTYPE_STREAM, firstLine,
                             0, 0, 
                             endLine - firstLine + 1);

    return true;
   }

//-----------------------------------------------------------------------------
bool findSelectStreamComment( const EditorInfo &edinfo, 
                              const std::vector<CommentInfo> &commentSequences,
                              bool bEraseDontSelect);
bool findSelectStreamComment( const EditorInfo &edinfo, 
                              const std::vector<CommentInfo> &commentSequences,
                              bool bEraseDontSelect)
   {
    CommentTypeEnum commentType = ctStream;
    CommentInfo ci;
    CommentTypeEnum foundCommentType = findCommentInfoByType( commentSequences, ci, commentType);
    if (foundCommentType!=commentType)
       return false; // Подходящий тип коментария не найден

    // пробуем отыскать закоментированный вертикальный блок
    std::string beginSequence = ci.begin_sequence;
    std::string endSequence   = ci.end_sequence;

    if (!beginSequence.empty() && endSequence.empty())
       endSequence = beginSequence;
    
    if (beginSequence.empty() && !endSequence.empty())
       beginSequence = endSequence;

    if (beginSequence.empty())
       return false;
       
    std::string line;
    int startPos = edinfo.CurPos;
    //int endPos = edinfo.CurPos + 1;
    if (beginSequence!=endSequence)
       {
        startPos = edinfo.CurPos + (int)beginSequence.size();
        //endPos   = edinfo.CurPos - (int)endSequence.size();
       }

    int startLine = edinfo.CurLine;
    for(; startLine>=0; --startLine)
       {
        if (!::Info.EditorCtrlGetString(startLine, line))
           return false;
        
        if (startLine==edinfo.CurLine)
           {
            if (startPos>=(int)line.size())
               startPos = (int)line.size()-1;
           }
        else
           {
            startPos = (int)line.size()-1;
           }
        if (startPos<0) continue;

        std::string::size_type foundPos = line.rfind(beginSequence, (std::string::size_type)startPos);
        if (foundPos==std::string::npos)
           continue;
        startPos = (int)foundPos;
        break;       
       }

    if (startLine<0)
       return false;

    int endLine = startLine;
    int endPos = startPos + (int)beginSequence.size();
    for(; endLine<edinfo.TotalLines; ++endLine)
       {
        if (!::Info.EditorCtrlGetString(endLine, line))
           return false;
       
        if (endLine==startLine)
           {
            if (endPos>=(int)line.size())
               continue;
           }
        else
           {
            endPos = 0;
           }
        std::string::size_type foundPos = line.find(endSequence, (std::string::size_type)endPos);
        if (foundPos==std::string::npos)
           continue;
        endPos = (int)(foundPos + endSequence.size());
        break;
       }

    if (endLine >= edinfo.TotalLines)
       return false;

    if (edinfo.CurLine>endLine && (edinfo.CurLine-endLine)>3)
       return false;

    ++endLine;


    if (!bEraseDontSelect)
       {
        if (::Info.EditorCtrlGetString(startLine, line))
           {
            if (findIndentAtStart(line, (int)startPos, ci.indent))
               startPos -= ci.indent;
            if (startPos<0) startPos = 0;
           }
        if (::Info.EditorCtrlGetString(endLine, line))
           {
            if (findIndentAtEnd(line, endPos + (int)endSequence.size(), ci.indent))
               endPos += ci.indent;
           }

        ::Info.EditorCtrlSelect( BTYPE_STREAM, startLine, 
                                 startPos, endPos - startPos, 
                                 endLine - startLine);
        return true;
       }


    // SS;
    // ss<<

    std::string eol;
    if (::Info.EditorCtrlGetString(endLine-1, line, eol))
       {
        int rmLen = (int)endSequence.size();
        // ss<<"rmLen(1): "<<rmLen<<"\n";

        if (findIndentAtEnd(line, endPos, ci.indent))
           {
            rmLen += ci.indent;
            // ss<<"Found indent at end\n";
           }
        // else
        //    {
        //     ss<<"Indent at end not found\n";
        //    }
        // ss<<"rmLen(2): "<<rmLen<<"\n";
       
        int pos = endPos - (int)endSequence.size();
        if (pos>=(int)ci.new_line_indent_before && findIndentAtStart(line, pos, ci.new_line_indent_before))
           {
            // ss<<"Found indent at start\n";
            endPos -= ci.new_line_indent_before;
            rmLen  += ci.new_line_indent_before;
           }
        // else
        //    {
        //     ss<<"Indent at start not found\n";
        //    }
        // ss<<"rmLen(3): "<<rmLen<<"\n";
        // ss<<"endPos: "<<endPos<<"\n";

        endPos -= (int)endSequence.size();
        if (endPos<0)
           {
            rmLen += endPos;
            endPos = 0;
           }
        // ss<<"rmLen(3): "<<rmLen<<"\n";
        // ss<<"endPos: "<<endPos<<"\n";
        line.erase((std::string::size_type)endPos, (std::string::size_type)rmLen );
        ::Info.EditorCtrlSetString(endLine-1, line, eol);
       }

    //Warning(IDM_COMMENTING_CODE, ss.str());

    if (::Info.EditorCtrlGetString(startLine, line, eol))
       {
        int rmLen = (int)beginSequence.size();

        if (findIndentAtEnd(line, startPos + (int)beginSequence.size(), ci.new_line_indent_before))
           rmLen += ci.new_line_indent_before;

        if (findIndentAtStart(line, (int)startPos, ci.indent))
           { 
            rmLen += ci.indent;
            startPos -= ci.indent;
           }

        if (startPos<0) 
           {
            rmLen += startPos;
            startPos = 0;
           }

        line.erase((std::string::size_type)startPos, (std::string::size_type)rmLen );
        ::Info.EditorCtrlSetString(startLine, line, eol);

        if (startLine==endLine-1)
           {
            endPos -= rmLen;
           }
       }

    ::Info.EditorCtrlSelect( BTYPE_STREAM, startLine, 
                             startPos, endPos - startPos, 
                             endLine - startLine);
    return true;
   }

//-----------------------------------------------------------------------------
void SelectOrRemoveComment(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos, bool bEraseDontSelect);
void SelectOrRemoveComment(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos, bool bEraseDontSelect)
   {
    if (!edinfo.FileName) return;

    if (startPos > endPos && startPos>=0 && endPos>=0)
       std::swap(startPos, endPos);

    TextTP::TTPConfig  cfgCopy = Config;
    readCustomConfig(edinfo.FileName, cfgCopy);
    std::string editorFileType;
    if (!lookupForFileType(cfgCopy, edinfo.FileName, editorFileType))
        {
         return;
        } 

    std::vector<CommentInfo> commentSequences;
    findInfo( editorFileType, 
              cfgCopy.comment_sequences.begin(), 
              cfgCopy.comment_sequences.end(), 
              commentSequences);
    
    if (commentSequences.empty()) return;


    if (findSelectLinesBlockComment( edinfo, commentSequences, bEraseDontSelect))
       return; // found and selected

    if (findSelectColumnComment( edinfo, commentSequences, bEraseDontSelect))
       return; // found and selected

    if (findSelectStreamComment( edinfo, commentSequences, bEraseDontSelect))
       return; // found and selected

   }

//-----------------------------------------------------------------------------
void SelectCommentedText(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
    SelectOrRemoveComment(edinfo, selected, 
                          startLine, startPos,
                          endLine, endPos, 
                          false /* bEraseDontSelect */);
   }

//-----------------------------------------------------------------------------
void uncommentText(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
    if (!selected) return;
    SelectOrRemoveComment(edinfo, selected, 
                          startLine, startPos,
                          endLine, endPos, 
                          true /* bEraseDontSelect */);
   }

//-----------------------------------------------------------------------------
void moveToSelStart(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
    if (!selected) return;
    ::Info.EditorCtrlSetPos( startPos, startLine );
   }
//-----------------------------------------------------------------------------

void moveToSelEnd(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
    if (!selected) return;

    if (edinfo.BlockType==BTYPE_COLUMN)
       {
        ::Info.EditorCtrlSetPos( endPos-1, endLine-1 );
       }
    else
       {
        if (endPos<1)
           {
           std::string str;
            if (::Info.EditorCtrlGetString(endLine-1, str))
               {
                endPos = (int)Utils::rtrim(str, ::Info.StdFn()).size(); // - 1;
                //if (endPos<0) endPos = 0;
               }
           }
        ::Info.EditorCtrlSetPos( endPos-1, endLine-1 );
       }
   }
//-----------------------------------------------------------------------------

void clearSelection(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
    ::Info.EditorCtrlSelect( BTYPE_NONE, 0, 0, 0, 0);
   }

//-----------------------------------------------------------------------------
void configureCommenting()
   {
   }

//-----------------------------------------------------------------------------
void expandTabs(std::string &str, const EditorInfo &edinfo)
   {
    std::string res;
    res.reserve(str.size());
    std::string::size_type pos = 0, size = str.size();
    for(; pos<size; ++pos)
       {
        if (str[pos]=='\t')
           res.append((std::string::size_type)edinfo.TabSize, ' ');
        else
           res.append(1, ' ');
       }
    res.swap(str);
   }

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
    std::vector<std::string>::const_iterator sIt = bs.singles.begin();
    for(; sIt!=bs.singles.end(); ++sIt)
       {
        if (Utils::comparePartOfString(str, fromPos, *sIt, bForward))
           {
            bSingle = true;
            begin = *sIt;
            return true;
           }
       }

    std::vector<CBreakSymbolPair>::const_iterator pIt = bs.pairs.begin();
    for(; pIt!=bs.pairs.end(); ++pIt)
       {

        if (Utils::comparePartOfString(str, fromPos, pIt->begin, bForward))
           {
            bSingle = false;
            begin = pIt->begin;
            end   = pIt->end;
            bBegin = true;
            return true;
           }

        if (Utils::comparePartOfString(str, fromPos, pIt->end, bForward))
           {
            bSingle = false;
            begin = pIt->begin;
            end   = pIt->end;
            bBegin = false;
            return true;
           }

       }

    return false;
   }

//-----------------------------------------------------------------------------
/*
bool lookupBreakSymbols( const std::string &str, 
                         const CBreakSymbols &bs,
                         std::string::size_type &fromPos, 
                         bool &bSingle, bool bBegin,
                         std::string &begin, std::string &end,
                         bool bForward
                       );

//-----------------------------------------------------------------------------
bool lookupBreakSymbols( const std::string &str, 
                         const CBreakSymbols &bs,
                         std::string::size_type &fromPos, 
                         bool &bSingle, bool bBegin,
                         std::string &begin, std::string &end
                         bool bForward
                       )
   {
    return false;
   }
*/
//-----------------------------------------------------------------------------
void incrementalSelection(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
    if (!edinfo.FileName) return;

    TextTP::TTPConfig  cfgCopy = Config;
    readCustomConfig(edinfo.FileName, cfgCopy);
    std::string editorFileType;
    if (!lookupForFileType(cfgCopy, edinfo.FileName, editorFileType))
        {
         editorFileType = "default";
        } 

    std::vector<CBreakSymbols> breakSymbolsVec;
    findInfo(editorFileType, cfgCopy.break_symbols.begin(), cfgCopy.break_symbols.end(), breakSymbolsVec);
    if (breakSymbolsVec.empty() && editorFileType!="default")
       {
        editorFileType = "default";
        findInfo(editorFileType, cfgCopy.break_symbols.begin(), cfgCopy.break_symbols.end(), breakSymbolsVec);
       } 

    if (breakSymbolsVec.empty())
       {
        //SS;
        //ss<<"break-symbols not found for this file"<<startPos<<",  endPos: "<<endPos;
        //Warning(IDM_COMMENTING_CODE, "break-symbols not found for this file");
        return;
       }

    const CBreakSymbols &breakSymbols = breakSymbolsVec[0];

/*    
    SS;
    ss<<"Found break-symbols for file-type: "<<editorFileType<<"\n";
    std::vector<std::string>::const_iterator sIt = breakSymbols.singles.begin();
    for(; sIt!=breakSymbols.singles.end(); ++sIt)
       {
        ss<<*sIt<<", ";
       }
    ss<<"\n";
    std::vector<CBreakSymbolPair>::const_iterator pIt = breakSymbols.pairs.begin();
    for(; pIt!=breakSymbols.pairs.end(); ++pIt)
       {
        ss<<"pair("<<pIt->begin<<","<<pIt->end<<"); ";
       }

    if (selected)
       {
        ss<<"startLine: "<<startLine<<", startPos: "<<startPos<<", endLine: "<<endLine<<", endPos: "<<endPos<<"\n";
       }
*/

    //Warning(IDM_COMMENTING_CODE, ss.str());
    if (selected && startLine==(endLine-1) && startPos==0 && endPos==-1)
       { // deselect previously selected line
        ::Info.EditorCtrlSelect( BTYPE_NONE, 0, 0, 0, 0);
        Warning(IDM_COMMENTING_CODE, "1");
        return;
       }
    
    if (selected && startLine!=(endLine-1))
       selected = false;

    if (!selected)
       {
        startLine = edinfo.CurLine;
        startPos  = edinfo.CurPos;
       }

    std::string curLine;
    if (!::Info.EditorCtrlGetString(startLine, curLine)) 
       {
        Warning(IDM_COMMENTING_CODE, "2");
        return;
       }

    expandTabs(curLine, edinfo);

    if (selected)
       { // check is needed full line select 
        std::string::size_type pos1 = curLine.find_first_not_of("\t ");
        std::string::size_type pos2 = curLine.find_last_not_of("\t ");
        // SS;
        // ss<<"startLine: "<<startLine<<", startPos: "<<startPos<<", endLine: "<<endLine<<", endPos: "<<endPos<<"\n";
        // ss<<"pos1: "<<(unsigned)pos1<<", ";
        // ss<<"pos2: "<<(unsigned)pos2<<"\n";
        // Warning(IDM_COMMENTING_CODE, ss.str());
        if ( pos1!=std::string::npos && 
             pos2!=std::string::npos && 
             (unsigned)pos1==(unsigned)startPos &&
             (unsigned)pos2+1==(unsigned)endPos
           )
           { // select full line
            ::Info.EditorCtrlSelect( BTYPE_STREAM, startLine, 
                                     0, -1, 
                                     1);
            Warning(IDM_COMMENTING_CODE, "3");
            return;
           }

       }
    else // no text selected, initial select
       {

        bool bEndIsSingle = false, bEndIsBegin = false;
        std::string beginBreakAtEnd, endBreakAtEnd;
        int posEndBreak = startPos, size = (int)curLine.size();
        bool bFoundEndBreak = false;
        // skip ws
        for(; posEndBreak<size; ++posEndBreak)
           if (curLine[posEndBreak]!=' ') break;
        for(; posEndBreak<size; ++posEndBreak)
           {
            if (isBreakSymbol(curLine, breakSymbols, true, /* fwd */
                              (std::string::size_type)posEndBreak,
                              bEndIsSingle, bEndIsBegin,
                              beginBreakAtEnd, endBreakAtEnd
                             ))
               {
                if (beginBreakAtEnd==endBreakAtEnd)
                   bEndIsBegin = false;
                bFoundEndBreak = true;
                Warning(IDM_COMMENTING_CODE, "4");
                break;
               }
            if (curLine[posEndBreak]==' ')
               {
                bFoundEndBreak = true;
                bEndIsSingle = true;
                beginBreakAtEnd = " ";
                Warning(IDM_COMMENTING_CODE, "5");
                break;
               }

           }

        int brCnt = 0;
        bool bBeginIsSingle = false, bBeginIsBegin = false;
        std::string beginBreakAtBegin, endBreakAtBegin;
        bool bFoundBeginBreak = false;
        int posBeginBreak = startPos;
        for(; posBeginBreak>=0; --posBeginBreak)
          if (curLine[posEndBreak]!=' ') break;
        for(; posBeginBreak>=0; --posBeginBreak)
           {
            if (!isBreakSymbol(curLine, breakSymbols, false, /* rew */
                              (std::string::size_type)posBeginBreak,
                              bBeginIsSingle, bBeginIsBegin,
                              beginBreakAtBegin, endBreakAtBegin
                             ) && curLine[posBeginBreak]!=' ')
               continue;

            if (!bFoundEndBreak || bEndIsSingle || bEndIsBegin)
               {
                bFoundBeginBreak = true;
                if (curLine[posBeginBreak]==' ')
                   {
                    bBeginIsSingle = true;
                    beginBreakAtBegin = " ";
                   }
                Warning(IDM_COMMENTING_CODE, "6");
                break;
               }

            if (bBeginIsSingle) continue;
            if (beginBreakAtBegin==endBreakAtBegin)
               {
                bBeginIsBegin = true;
                bFoundBeginBreak = true;
                Warning(IDM_COMMENTING_CODE, "7");
                break;
               }

            if (bBeginIsBegin && beginBreakAtBegin!=beginBreakAtEnd)
               continue;

            if (!bBeginIsBegin)
               --brCnt;
            else
               if (!brCnt)
                  {
                   bFoundBeginBreak = true;
                   Warning(IDM_COMMENTING_CODE, "8");
                   break;
                  }
               else
                  {
                   ++brCnt;
                  }
           }


        if (!bFoundBeginBreak && !bFoundEndBreak)
           { // select full line
            ::Info.EditorCtrlSelect( BTYPE_STREAM, startLine, 
                                     0, -1, 
                                     1);
            Warning(IDM_COMMENTING_CODE, "9");
            return;
           }

        if (!bFoundBeginBreak)
           { 
            ::Info.EditorCtrlSelect( BTYPE_STREAM, startLine, 
                                     0, posEndBreak, 
                                     0);
            Warning(IDM_COMMENTING_CODE, "10");
            return;
           }

        if (!bFoundEndBreak)
           { 
            ::Info.EditorCtrlSelect( BTYPE_STREAM, startLine, 
                                     posBeginBreak, -1, 
                                     1);
            //Warning(IDM_COMMENTING_CODE, "11");
            SS;
            ss
            return;
           }
        ::Info.EditorCtrlSelect( BTYPE_STREAM, startLine, 
                                 posBeginBreak, posEndBreak, 
                                 0);


       
       }



   }

//-----------------------------------------------------------------------------
}; // namespace TextTP

