/* Misc Far warapper classes */
#ifndef CFARCLS_H
#define CFARCLS_H


#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif


#if !defined(_VECTOR_) && !defined(_STLP_VECTOR) && !defined(__STD_VECTOR__) && !defined(_CPP_VECTOR) && !defined(_GLIBCXX_VECTOR)
    #include <vector>
#endif


//#if !defined(_SSTREAM_) && !defined(_STLP_SSTREAM) && !defined(__STD_SSTREAM__) && !defined(_CPP_SSTREAM) && !defined(_GLIBCXX_SSTREAM)
//    #include <sstream>
//#endif


#include <assert.h>


#define ASSERT(expr) assert(expr)


#include "far/plugin.hpp"
#include "utils.h"




#ifndef SIZEOF_ARRAY
    #define SIZEOF_ARRAY(a)   (sizeof(a)/sizeof(a[0]))
#endif

namespace Far
{



class CPluginStartupInfo
{
        PluginStartupInfo      startup_info;
        FarStandardFunctions   fsf;


        inline
        CPluginStartupInfo(const CPluginStartupInfo &si)
            : startup_info(si.startup_info)
            {} 

        inline
        CPluginStartupInfo& operator=(const CPluginStartupInfo &si)
            {
             startup_info = si.startup_info;
             return *this;
            } 

    public:

        inline
        CPluginStartupInfo()
            {
             startup_info.StructSize      = sizeof(startup_info);
             startup_info.ModuleName[0]   = 0;
             startup_info.ModuleNumber    = 0;

             startup_info.RootKey         = 0;
             startup_info.Menu            = 0;
             startup_info.Dialog          = 0;
             startup_info.Message         = 0;
             startup_info.GetMsg          = 0;
             startup_info.Control         = 0;
             startup_info.SaveScreen      = 0;
             startup_info.RestoreScreen   = 0;
             startup_info.GetDirList      = 0;
             startup_info.GetPluginDirList= 0;
             startup_info.FreeDirList     = 0;
             startup_info.Viewer          = 0;
             startup_info.Editor          = 0;
             startup_info.CmpName         = 0;
             startup_info.CharTable       = 0;
             startup_info.Text            = 0;
             startup_info.EditorControl   = 0;

             startup_info.FSF             = 0;
             
             startup_info.ShowHelp        = 0;
             startup_info.AdvControl      = 0;
             startup_info.InputBox        = 0;
             startup_info.DialogEx        = 0;
             startup_info.SendDlgMessage  = 0;
             startup_info.DefDlgProc      = 0;
             //startup_info.Reserved[2];

             fsf.StructSize = 0;

            }

        CPluginStartupInfo(const PluginStartupInfo *info)
            {
             startup_info = *info;
             fsf          = *(info->FSF);
            }

        CPluginStartupInfo& operator=(const PluginStartupInfo &info)
            {
             startup_info = info;
             fsf          = *(info.FSF);
             return *this;
            } 


        inline 
        int Menu(int X, int Y, int MaxHeight, DWORD Flags,
                 const char *Title, const char* Bottom,
                 const char* HelpTopic, const int* BreakKeys,
                 int * BreakCode, 
                 const FarMenuItem* Items,
                 int ItemsNumber) const
            {
             ASSERT(startup_info.StructSize);
             ASSERT(startup_info.Menu);
             return startup_info.Menu( startup_info.ModuleNumber, 
                                       X, Y, MaxHeight, Flags,
                                       Title, Bottom, HelpTopic, BreakKeys,
                                       BreakCode, Items, ItemsNumber
                                     );        
            }

        inline
        int Dialog(int X1, int Y1, int X2, int Y2,
                   const char* HelpTopic,
                   FarDialogItem *Items,
                   int ItemsNumber) const
            {
             ASSERT(startup_info.StructSize);
             ASSERT(startup_info.Dialog);
             return startup_info.Dialog( startup_info.ModuleNumber, 
                                         X1, Y1, X2, Y2, HelpTopic, 
                                         Items, ItemsNumber);            
            }

        inline
        int DialogEx(int X1, int Y1, int X2, int Y2,
                   const char* HelpTopic,
                   FarDialogItem *Items,
                   int ItemsNumber,
                   DWORD Reserved, DWORD Flags, 
                   FARWINDOWPROC DlgProc,
                   long Param) const
            {
             ASSERT(startup_info.StructSize);
             ASSERT(startup_info.DialogEx);
             return startup_info.DialogEx( startup_info.ModuleNumber, 
                                           X1, Y1, X2, Y2, HelpTopic, 
                                           Items, ItemsNumber,
                                           Reserved, Flags, 
                                           DlgProc, Param);
            }

        inline
        FARAPIDEFDLGPROC GetDefDlgProc()
            {
             ASSERT(startup_info.StructSize);
             ASSERT(startup_info.DefDlgProc);
             return startup_info.DefDlgProc;
            }

        inline
        long DefDlgProc(HANDLE hDlg, int Msg, int Param1, long Param2) const
            {
             ASSERT(startup_info.StructSize);
             ASSERT(startup_info.DefDlgProc);
             return startup_info.DefDlgProc(hDlg, Msg, Param1, Param2);
            }

        inline
        long SendDlgMessage(HANDLE hDlg, int Msg,
                            int Param1, long Param2) const
            {
             ASSERT(startup_info.StructSize);
             ASSERT(startup_info.SendDlgMessage);
             return startup_info.SendDlgMessage( hDlg, Msg, Param1, Param2 );
            }

        inline
        int Message(DWORD Flags, const char *HelpTopic,
                    const char * const *Items, int ItemsNumber, int ButtonsNumber) const
            {
             ASSERT(startup_info.StructSize);
             ASSERT(startup_info.Message);
             return startup_info.Message( startup_info.ModuleNumber, 
                                          Flags, HelpTopic,
                                          Items, ItemsNumber, ButtonsNumber);
            }

        inline 
        int Message( const std::string &title, 
                     const std::string &_msg,
                     DWORD Flags = 0)
            {
             //std::string title = _title;
             //if (title.empty()) title = "Message";
             std::string msg = title.empty() ? std::string("Message") : title;
             msg += "\n";
             msg += _msg;
             msg += "\nOk";
             return Message( Flags|FMSG_ALLINONE, 0, /*HelpTopic*/
                             (const char * const *)msg.c_str(), 1, 1);            
            }

        inline 
        int Warning( const std::string &_msg,
                     const std::string &title = "" )
            {
             return Message( title.empty() ? std::string("Warning!") : title, 
                            _msg, FMSG_WARNING);
            }

        inline    
        const char* GetMsg(int id) const
            {
             ASSERT(startup_info.StructSize);
             ASSERT(startup_info.GetMsg);
             return startup_info.GetMsg( startup_info.ModuleNumber, id );
            }

        inline
        int InputBox(
                     const char *Title, const char *SubTitle,
                     const char *HistoryName,  const char *SrcText,
                     char *DestText,  int DestLength,
                     const char *HelpTopic,  DWORD Flags
                    ) const
            {
             ASSERT(startup_info.StructSize);
             ASSERT(startup_info.InputBox);
             return startup_info.InputBox( Title, SubTitle,
                                           HistoryName, SrcText,
                                           DestText, DestLength,
                                           HelpTopic, Flags
                                         );
            }

        inline 
        const FARSTANDARDFUNCTIONS* StdFn() const
            {
             //ASSERT(startup_info.StructSize);
             //return startup_info.FSF;//&FARSTANDARDFUNCTIONS;
             ASSERT(fsf.StructSize);
             return &fsf;             
            }

        int Editor( const char *FileName
                  , const char *Title
                  , int X1
                  , int Y1
                  , int X2
                  , int Y2
                  , DWORD Flags
                  , int StartLine
                  , int StartChar
                  )
            {
             ASSERT(startup_info.StructSize);
             ASSERT(startup_info.Editor);
             return startup_info.Editor( FileName, Title, X1, Y1, X2, Y2, Flags, StartLine, StartChar);
            }
            
        inline /* Far fn original named */
        int EditorControl(int cmd, void* param) const
            {
             ASSERT(startup_info.StructSize);
             ASSERT(startup_info.EditorControl);
             return startup_info.EditorControl(cmd, param);            
            }

        inline /* Short alias for far fn */
        int EditorCtrl(int cmd, void* param) const
            { return EditorControl(cmd, param); }

        inline 
        int EditorCtrlGetString(/* struct */ EditorGetString* EdtStrStruct) const
            {
             return EditorControl(ECTL_GETSTRING, (void*)EdtStrStruct);
            }

        inline 
        int EditorCtrlSetString(/* struct */ EditorSetString* EdtStrStruct) const
            {
             return EditorControl(ECTL_SETSTRING, (void*)EdtStrStruct);
            }

        inline 
        int EditorCtrlGetString(int lineNo, std::string &str, std::string &eol)
            {
             EditorGetString egs = { lineNo, 0, 0, 0, 0, 0};
             int res = EditorCtrlGetString(&egs);
             if (!res) return res; //failed to get info about string
             str = std::string(egs.StringText, (std::string::size_type)egs.StringLength);
             eol = std::string(egs.StringEOL);
             return res;
            }

        inline 
        int EditorCtrlGetString(int lineNo, std::string &str)
            {
             EditorGetString egs = { lineNo, 0, 0, 0, 0, 0};
             int res = EditorCtrlGetString(&egs);
             if (!res) return res; //failed to get info about string
             str = std::string(egs.StringText, (std::string::size_type)egs.StringLength);
             return res;
            }

        inline 
        int EditorCtrlSetString(int lineNo, const std::string &str)
            {
             EditorSetString ess = { lineNo, 
                                     str.empty() ? (char*)"" : (char*)str.data(), 
                                     "\r\n", (int)str.size() };
             return EditorCtrlSetString(&ess);
            }

        inline 
        int EditorCtrlSetString(int lineNo, const std::string &str, const std::string &eol)
            {
             EditorSetString ess = { lineNo, 
                                     str.empty() ? (char*)"" : (char*)str.data(), 
                                     eol.empty() ? (char*)"\r\n" : (char*)eol.c_str(), 
                                     (int)str.size() };
             return EditorCtrlSetString(&ess);
            }

        inline
        int EditorCtrlInsertLine(BOOL bUseIndent) const
            {
             int use_indent = bUseIndent ? 1 : 0;
             return EditorControl(ECTL_INSERTSTRING, (void*)&use_indent);
            }

        inline
        int EditorCtrlDeleteLine() const
            {
             return EditorControl(ECTL_DELETESTRING, 0);
            }

        inline
        int EditorCtrlInsertText(const char* text) const
            {
             return EditorControl(ECTL_INSERTTEXT, (void*)text);
            }

        inline
        int EditorCtrlGetInfo( EditorInfo *pinfo) const
            {
             return EditorControl(ECTL_GETINFO, (void*)pinfo);
            }

        inline 
        int EditorCtrlSetPos( EditorSetPosition *pos_info ) const
            {
             return EditorControl(ECTL_SETPOSITION, (void*)pos_info);
            }

        inline 
        int EditorCtrlSetPos( int pos, int line ) const
            {
             EditorSetPosition sp = { line, pos, -1, -1, -1 };
             return EditorCtrlSetPos( &sp );
            }

        inline 
        int EditorCtrlSelect( int BlockType,
                              int BlockStartLine, int BlockStartPos,
                              int BlockWidth, int BlockHeight)

            {
             EditorSelect sel = { BlockType, 
                                  BlockStartLine, BlockStartPos, 
                                  BlockWidth, BlockHeight };
             return EditorControl(ECTL_SELECT, (void*)&sel);
            }

        /*
        inline
        std::string makeStringFromEgs(const EditorGetString &egs)
            {
             if (!egs1.StringText || !egs1.StringLength)
                return std::string();
             return std::string(egs1.StringText, (std::string::size_type)egs1.StringLength);
            }

        inline
        std::string makeEolStringFromEgs(const EditorGetString &egs)
            {
             if (!egs1.StringEOL)
                return std::string("\r\n");
             return std::string(egs1.StringEOL);
            }
        */

        inline
        void EditorCtrlSwapLines(int line1No, int line2No)
            {
             /*
             EditorInfo edinfo;
             EditorCtrlGetInfo( &edinfo );

             std::stringstream ss;
             ss<<"TotalLines: "<<edinfo.TotalLines<<"\n";
             ss<<"line1No: "<<line1No<<"\nline2No: "<<line2No;
             Warning(ss.str());
             */

             /*/      
             EditorGetString egs1 = { line1No, 0, 0, 0, 0, 0};
             EditorCtrlGetString(&egs1);
             std::string str1(egs1.StringText, (std::string::size_type)egs1.StringLength);
             std::string eol1(egs1.StringEOL);
             

             EditorGetString egs2 = { line2No, 0, 0, 0, 0, 0};
             EditorCtrlGetString(&egs2);
             std::string str2(egs2.StringText, (std::string::size_type)egs2.StringLength);
             std::string eol2(egs2.StringEOL);
             */

             std::string str1, eol1;
             if (!EditorCtrlGetString(line1No, str1, eol1)) return;

             std::string str2, eol2;
             if (!EditorCtrlGetString(line2No, str2, eol2)) return;
             
             EditorSetString ess1 = { line1No, (char*)str2.data(), 
                                     (char*)eol2.c_str(), (int)str2.size() };
             EditorSetString ess2 = { line2No, (char*)str1.data(), 
                                     (char*)eol1.c_str(), (int)str1.size() };

             EditorCtrlSetString(&ess1);
             EditorCtrlSetString(&ess2);
            }

        inline
        void EditorCtrlInsertNewString(int lineNo, const std::string &str)
            {
             EditorInfo edinfo;
             EditorCtrlGetInfo( &edinfo );
             // edinfo.CurLine; edinfo.CurPos;

             /*
             std::stringstream ss;
             ss<<"TotalLines: "<<edinfo.TotalLines<<"\nlineNo: "<<lineNo;
             Warning(ss.str());
             */

             EditorSetPosition spos = { lineNo+1, 0, -1, -1, -1, -1 };
             if ((lineNo+1) < edinfo.TotalLines)
                {
                 EditorCtrlSetPos( &spos );
                 EditorCtrlInsertLine(FALSE);
                }
             else
                {
                 spos.CurLine = lineNo;
                 EditorCtrlSetPos( &spos );
                 EditorCtrlInsertLine(FALSE);
                 EditorCtrlSwapLines(lineNo, lineNo+1);
                }

             
             EditorGetString egs = { lineNo+1, 0, 0, 0, 0, 0};
             if (!EditorCtrlGetString(&egs))
                {
                 spos.CurLine = edinfo.CurLine; 
                 spos.CurPos  = edinfo.CurPos;
                 EditorCtrlSetPos( &spos );            
                 return; 
                }

             EditorSetString ess = { lineNo+1, (char*)str.data(), 
                                     (char*)egs.StringEOL, 
                                     (int)str.size()
                                   };
             EditorCtrlSetString(&ess);

             spos.CurLine = edinfo.CurLine; 
             spos.CurPos  = edinfo.CurPos;
             EditorCtrlSetPos( &spos );            
            }

        inline
        int EditorCtrlInsertStrings(int lineNo, const std::string &text, const std::string &indentText = "")
            {
             std::vector<std::string> lines;
             TextTP::Utils::split_string(lines, text, '\n');
             TextTP::Utils::removeCR(lines);
             std::vector<std::string>::const_iterator it = lines.begin();
             int lineCount = 0;
             for(; it!=lines.end(); ++it, ++lineCount)
                EditorCtrlInsertNewString(lineNo++, indentText + *it);
             return lineCount;
            }


        inline
        void EditorCtrlDeleteLine(int lineNo)
            {
             EditorInfo edinfo;
             EditorCtrlGetInfo( &edinfo );
             // edinfo.CurLine; edinfo.CurPos;
             if (lineNo>=edinfo.TotalLines)
                return;

             EditorSetPosition spos = { lineNo, 0, -1, -1, -1, -1 };
             EditorCtrlSetPos( &spos );
             EditorCtrlDeleteLine();

             spos.CurLine = edinfo.CurLine; 
             spos.CurPos  = edinfo.CurPos;
             EditorCtrlSetPos( &spos );            
            }


}; // class CPluginStartupInfo



}; // namespace Far


#endif /* CFARCLS_H */

/*
struct PluginStartupInfo
{
  int StructSize;
  char ModuleName[NM];
  int ModuleNumber;
  const char *RootKey;
  FARAPIMENU             Menu;
  FARAPIDIALOG           Dialog;
  FARAPIMESSAGE          Message;
  FARAPIGETMSG           GetMsg;
  FARAPICONTROL          Control;
  FARAPISAVESCREEN       SaveScreen;
  FARAPIRESTORESCREEN    RestoreScreen;
  FARAPIGETDIRLIST       GetDirList;
  FARAPIGETPLUGINDIRLIST GetPluginDirList;
  FARAPIFREEDIRLIST      FreeDirList;
  FARAPIVIEWER           Viewer;
  FARAPIEDITOR           Editor;
  FARAPICMPNAME          CmpName;
  FARAPICHARTABLE        CharTable;
  FARAPITEXT             Text;
  FARAPIEDITORCONTROL    EditorControl;

  FARSTANDARDFUNCTIONS  *FSF;

  FARAPISHOWHELP         ShowHelp;
  FARAPIADVCONTROL       AdvControl;
  FARAPIINPUTBOX         InputBox;
  FARAPIDIALOGEX         DialogEx;
  FARAPISENDDLGMESSAGE   SendDlgMessage;
  FARAPIDEFDLGPROC       DefDlgProc;
  DWORD                  Reserved[2];
};





typedef struct FarStandardFunctions
{
  int StructSize;

  FARSTDATOI                 atoi;
  FARSTDATOI64               atoi64;
  FARSTDITOA                 itoa;
  FARSTDITOA64               itoa64;
  // <C&C++>
  FARSTDSPRINTF              sprintf;
  FARSTDSSCANF               sscanf;
  // </C&C++>
  FARSTDQSORT                qsort;
  FARSTDBSEARCH              bsearch;
  FARSTDQSORTEX              qsortex;

  DWORD                      Reserved[9];

  FARSTDLOCALISLOWER         LIsLower;
  FARSTDLOCALISUPPER         LIsUpper;
  FARSTDLOCALISALPHA         LIsAlpha;
  FARSTDLOCALISALPHANUM      LIsAlphanum;
  FARSTDLOCALUPPER           LUpper;
  FARSTDLOCALLOWER           LLower;
  FARSTDLOCALUPPERBUF        LUpperBuf;
  FARSTDLOCALLOWERBUF        LLowerBuf;
  FARSTDLOCALSTRUPR          LStrupr;
  FARSTDLOCALSTRLWR          LStrlwr;
  FARSTDLOCALSTRICMP         LStricmp;
  FARSTDLOCALSTRNICMP        LStrnicmp;

  FARSTDUNQUOTE              Unquote;
  FARSTDEXPANDENVIRONMENTSTR ExpandEnvironmentStr;
  FARSTDLTRIM                LTrim;
  FARSTDRTRIM                RTrim;
  FARSTDTRIM                 Trim;
  FARSTDTRUNCSTR             TruncStr;
  FARSTDTRUNCPATHSTR         TruncPathStr;
  FARSTDQUOTESPACEONLY       QuoteSpaceOnly;
  FARSTDPOINTTONAME          PointToName;
  FARSTDGETPATHROOT          GetPathRoot;
  FARSTDADDENDSLASH          AddEndSlash;
  FARSTDCOPYTOCLIPBOARD      CopyToClipboard;
  FARSTDPASTEFROMCLIPBOARD   PasteFromClipboard;
  FARSTDKEYTOKEYNAME         FarKeyToName;
  FARSTDKEYNAMETOKEY         FarNameToKey;
  FARSTDINPUTRECORDTOKEY     FarInputRecordToKey;
  FARSTDXLAT                 XLat;
  FARSTDGETFILEOWNER         GetFileOwner;
  FARSTDGETNUMBEROFLINKS     GetNumberOfLinks;
  FARSTDRECURSIVESEARCH      FarRecursiveSearch;
  FARSTDMKTEMP               MkTemp;
  FARSTDDELETEBUFFER         DeleteBuffer;
  FARSTDPROCESSNAME          ProcessName;
  FARSTDMKLINK               MkLink;
  FARCONVERTNAMETOREAL       ConvertNameToReal;
  FARGETREPARSEPOINTINFO     GetReparsePointInfo;
} FARSTANDARDFUNCTIONS;


*/










