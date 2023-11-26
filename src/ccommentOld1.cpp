#include <windows.h>
#include <objbase.h>

#include <string.h>
#include <memory>
#include <wchar.h>
#include <exception>
#include <stdexcept>
#include <vector>
#include <map>
#include <algorithm>
#include <float.h>
#include <malloc.h>



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
    struct FarMenuItem menuItems[5] = { 0 };
    INIT_MENU_STRING(menuItems[0].Text, IDM_COMMENT_SELECTION, "&C Comment selection");
    INIT_MENU_STRING(menuItems[1].Text, IDM_UNCOMMENT_SELECTION, "&U Uncomment selection");
    INIT_MENU_STRING(menuItems[2].Text, IDM_SELECT_COMMENTED, "&S Select commented");
    menuItems[3].Separator = 1;
    INIT_MENU_STRING(menuItems[4].Text, IDM_INC_INDENT, "&I Increase indent");
    INIT_MENU_STRING(menuItems[5].Text, IDM_DEC_INDENT, "&D Decrease indent");
    INIT_MENU_STRING(menuItems[6].Text, IDM_INC_INDENT1, "&+ Increase indent (1 ws)");
    INIT_MENU_STRING(menuItems[7].Text, IDM_DEC_INDENT1, "&- Decrease indent (1 ws)");
    //INIT_MENU_STRING(menuItems[], , );

    char title[128];
    INIT_MENU_STRING(title, IDM_COMMENTING_CODE, "Commenting code");

    int menuRes = ::Info.Menu( -1, -1, 0,
                               FMENU_WRAPMODE, title, 0,
                               0, /* Help topic */
                               0, 0, 
                               menuItems,
                               sizeof(menuItems)/sizeof(menuItems[0])
                             );
    if (menuRes<0 || menuRes>7) 
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
           }
        if (endPos==0)
           {
            endPos = -1; // последняя строка выделена до конца
            --endLine;
           }
       }

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
       }

/*
    ss<<"SelStart: "<<startLine<<", startPos: "<<startPos<<", SelEnd: "<<endLine<<", endPos: "<<endPos<<" \n";
    Warning(IDM_COMMENTING_CODE, ss.str());
*/

/*
struct EditorGetString
{
  int StringNumber;
  const char *StringText;
  const char *StringEOL;
  int StringLength;
  int SelStart;
  int SelEnd;
};
*/

/*
    EditorGetString str_info = { -1 }; // cur line
    if (!::Info.EditorCtrlGetString( &str_info))
       return;

    if (!str_info.StringText)
       return;

    int pos_end = edinfo.CurPos;
*/
   
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

        int tmpRes = findFirstNonSpace(edinfo, egs.StringText, egs.StringLength);
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
        int tmpRes = findLastNonSpace(edinfo, egs.StringText, egs.StringLength);
        if (res<tmpRes) res = tmpRes;
        //if (res<egs.StringLength) res = egs.StringLength;
       }
    return res;
   }
//-----------------------------------------------------------------------------
void indentText(const EditorInfo &edinfo, bool selected, 
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

        char *buf = (char*)_alloca(egs.StringLength+edinfo.TabSize+1);
        char *buf2 = buf;
        if (!(edinfo.Options&EOPT_EXPANDTABS))
           *buf2++ = '\t';
        else
           {
            for(int i=0; i<edinfo.TabSize; ++i)
               *buf2++ = ' ';
           }
        memcpy((void*)buf2, (void*)egs.StringText, (size_t)egs.StringLength);
        buf2[egs.StringLength] = 0;
   
        EditorSetString ess = { startLine, buf, 
                                (char*)egs.StringEOL, 
                                egs.StringLength + 
                                    ((edinfo.Options&EOPT_EXPANDTABS) ? (int)(edinfo.TabSize) : (int)1) 
                              };
        ::Info.EditorCtrlSetString(&ess);
       }
   }
//-----------------------------------------------------------------------------
void unindentText(const EditorInfo &edinfo, bool selected, 
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
   
        const char *pTxt = egs.StringText;
        int i = 0;
        for( ; i<edinfo.TabSize; ++i, ++pTxt)
           {
            if (*pTxt==' ')
               continue;
            if (*pTxt=='\t')
               {
                ++pTxt;
                ++i;
               }
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
void indentText1(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
   }
//-----------------------------------------------------------------------------
void unindentText1(const EditorInfo &edinfo, bool selected, 
                 int startLine, int startPos,
                 int endLine, int endPos)
   {
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
    findInfo(editorFileType, cfgCopy.comment_sequences.begin(), cfgCopy.comment_sequences.end(), commentSequences);
    
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

    ::Info.EditorCtrlInsertNewString( startLine, "AAA" );


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
