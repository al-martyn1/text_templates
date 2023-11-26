
#define GUIDGEN_CPP__63B0288C_671F_4B64_88AC_2EED94172A52__

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

#if !defined(_MEMORY_) && !defined(_STLP_MEMORY) && !defined(__STD_MEMORY__) && !defined(_CPP_MEMORY) && !defined(_GLIBCXX_MEMORY)
    #include <memory>
#endif

#if !defined(_INC_WCHAR) && !defined(_WCHAR_H_) && !defined(_WCHAR_H)
    #include <wchar.h>
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

#if !defined(_ALGORITHM_) && !defined(_STLP_ALGORITHM) && !defined(__STD_ALGORITHM) && !defined(_CPP_ALGORITHM) && !defined(_GLIBCXX_ALGORITHM)
    #include <algorithm>
#endif

#if !defined(_UTILITY_) && !defined(_STLP_UTILITY) && !defined(__STD_UTILITY__) && !defined(_CPP_UTILITY) && !defined(_GLIBCXX_UTILITY)
    #include <utility>
#endif

#if !defined(_IOSTREAM_) && !defined(_STLP_IOSTREAM) && !defined(__STD_IOSTREAM__) && !defined(_CPP_IOSTREAM) && !defined(_GLIBCXX_IOSTREAM)
    #include <iostream>
#endif

#if !defined(_FSTREAM_) && !defined(_STLP_FSTREAM) && !defined(__STD_FSTREAM__) && !defined(_CPP_FSTREAM) && !defined(_GLIBCXX_FSTREAM)
    #include <fstream>
#endif

#if !defined(_SSTREAM_) && !defined(_STLP_SSTREAM) && !defined(__STD_SSTREAM__) && !defined(_CPP_SSTREAM) && !defined(_GLIBCXX_SSTREAM)
    #include <sstream>
#endif

#if !defined(_INC_FLOAT) && !defined(_FLOAT_H_) && !defined(_FLOAT_H)
    #include <float.h>
#endif

#if !defined(_INC_MALLOC) && !defined(_MALLOC_H_) && !defined(_MALLOC_H)
    #include <malloc.h>
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
#include "ccomment.h"
#include "fileOp.h"

#include "far/farkeys.hpp"

#if !defined(TEXTFMT_H)
    #include "textfmt.h"
#endif


using TextTP::Warning;

#ifndef EXTERN_C
    #ifdef __cplusplus
        #define EXTERN_C extern "C"
    #endif /* __cplusplus */
#endif /* EXTERN_C */

#ifndef DLLEXPORT 
    #define DLLEXPORT __declspec(dllexport)
#endif /* DLLEXPORT */

#define SS std::stringstream ss
#define SSO() ::OutputDebugString(ss.str().c_str())
#define SSOC(str) ::OutputDebugString(str)

/*
#ifndef _stdcall
#define _stdcall __attribute__((stdcall))
#endif
#ifndef __stdcall
#define __stdcall __attribute__((stdcall))
#endif
*/



//#ifndef MAX_EXT_SIZE
//    #define MAX_EXT_SIZE     15
//#endif




Far::CPluginStartupInfo         Info;
char                            InputTplNameBuf[1024];
TextTP::TTPConfig               Config;
HMODULE                         hModule;
std::string                     plugin_file_name;
std::string                     plugin_conf_name;
//std::string                     plugin_tpl_dir_name;
std::string                     plugin_root_key;




//-----------------------------------------------------------------------------
EXTERN_C
BOOL WINAPI
DllMain (HANDLE hDll, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            // Code to run when the DLL is loaded
            hModule = (HMODULE)hDll;
            //ATLTRACE("DllMain(DLL_PROCESS_ATTACH)\n");
            break;

        case DLL_PROCESS_DETACH:
            // Code to run when the DLL is freed
            break;

        case DLL_THREAD_ATTACH:
            // Code to run when a thread is created during the DLL's lifetime
            //CoInitialize( 0 );
            break;

        case DLL_THREAD_DETACH:
            //CoUninitialize();
            // Code to run when a thread ends normally.
            break;
    }
    return TRUE;
}

//-----------------------------------------------------------------------------
using namespace Rsdn;
using namespace Rsdn::Serializer;
using namespace Rsdn::Serializer::FieldAttrubutes;
using namespace Rsdn::Serializer::Tree;
using namespace Rsdn::Serializer::Tree::Xml;
using namespace Rsdn::Serializer::Primitives;



//-----------------------------------------------------------------------------
//DLLEXPORT
void WINAPI _export SetStartupInfo(const struct PluginStartupInfo *Info)
{
//     CoCreateGuid(&guid);
//     format_guid_registry(help_guid_buf, &guid);
    //ATLTRACE("SetStartupInfo\n");
    ::Info=*Info;

    plugin_root_key = Info->RootKey;
    plugin_root_key += "\\TextTP";

    char buf[1024];

    DWORD len = GetModuleFileName( hModule, buf, SIZEOF_ARRAY(buf)-MAX_EXT_SIZE-1);
    buf[len] = 0;
    if (!len) return;

    plugin_file_name = buf;
    Config.setPluginPath(plugin_file_name);
    Config.loadFormatterPlugins();


    char * pdot = strrchr( buf, '.');
    if (pdot && strlen(pdot)<=MAX_EXT_SIZE)
        *pdot = 0;
    strcat(buf, ".xml");
    plugin_conf_name = buf;

    char *tmp = buf;
    while(*tmp) { if (*tmp=='\\') *tmp = '/'; tmp++; }

    char * pslash = strrchr( buf, '/');
    if (pslash) *pslash = 0;
    std::string plugin_tpl_dir_name = buf;
    plugin_tpl_dir_name += "/templates";

    Config.template_dirs.push_back(plugin_tpl_dir_name);

    Config.readFromFile(plugin_conf_name);
    //read_config_file(Config, plugin_conf_name);

}

//-----------------------------------------------------------------------------
int WINAPI _export Configure( int ItemNumber)
{
    //std::stringstream ss;
    //ss<<"Configure, ItemNumber: "<<ItemNumber;
    //::Info.Message( FMSG_WARNING|FMSG_ALLINONE, 0, /*HelpTopic*/
    //                (const char * const *)ss.str().c_str(), 1, 1);

     switch(ItemNumber)
        {
         case 0: 
            TextTP::settingsDialog();
            break;
         case 1:
            TextTP::configureCommenting();
            break;        
        };

    //TextTP::settingsDialog();
    return TRUE;
}

//-----------------------------------------------------------------------------
//DLLEXPORT
HANDLE WINAPI _export OpenPlugin( int OpenFrom, int Item)
    {
     
     //std::stringstream ss;
     //ss<<"Configure, OpenFrom: "<<OpenFrom<<", ItemNumber: "<<Item;
     //::Info.Message( FMSG_WARNING|FMSG_ALLINONE, 0, /*HelpTopic*/
     //                (const char * const *)ss.str().c_str(), 1, 1);
     if (OpenFrom!=OPEN_EDITOR)
        return (INVALID_HANDLE_VALUE);

     switch(Item)
        {
         case 0: 
            TextTP::doTextTPJob();     
            break;
         case 1:
            TextTP::doCommentingJob();
            break;        
         case 2:
            TextTP::formattingText();
            break;        
        };
/*
     switch(OpenFrom)
         {
          case OPEN_EDITOR:       
               TextTP::doTextTPJob();     break;
          //case OPEN_PLUGINSMENU:  TextTP::settingsDialog();  break;
         };
*/     
     return (INVALID_HANDLE_VALUE);
    }

//-----------------------------------------------------------------------------
//DLLEXPORT
void WINAPI _export ClosePlugin( HANDLE hPlugin)
    {
     Config.unloadFormatterPlugins();
    }

//-----------------------------------------------------------------------------
//DLLEXPORT
#define INIT_MENU_STRING(var, rcId, str)\
        if (!var)                       \
           {                            \
            var = ::Info.GetMsg(rcId);  \
            if (!var)                   \
               var = str;               \
           }

//-----------------------------------------------------------------------------
void WINAPI _export GetPluginInfo(struct PluginInfo *Info)
{
  Info->StructSize = sizeof(*Info);
//  Info->Flags = PF_EDITOR | PF_PRELOAD | PF_DISABLEPANELS;
  Info->Flags = PF_EDITOR | PF_DISABLEPANELS;
  if ((TextTP::getConfigAllowEditorSettingsChangeFeature() || 
       TextTP::getConfigAllowReloadEditorFiles())
     && !TextTP::getConfigDisablePreloading())
      Info->Flags |= PF_PRELOAD;

  //PF_EDITOR|PF_DISABLEPANELS;
  Info->DiskMenuStringsNumber=0;

  static const char *PluginMenuStrings[3] = { 0, 0, 0 };
  INIT_MENU_STRING(PluginMenuStrings[0], IDM_TEXTTP_TITLE, "Text Templates");
  INIT_MENU_STRING(PluginMenuStrings[1], IDM_COMMENTING_CODE, "Commenting code");
  INIT_MENU_STRING(PluginMenuStrings[2], IDM_FORMATTING_TEXT, "Formatting text");
  Info->PluginMenuStrings=PluginMenuStrings;
  Info->PluginMenuStringsNumber=SIZEOF_ARRAY(PluginMenuStrings);

  static const char *PluginConfigMenuStrings[1] = { 0 };
  INIT_MENU_STRING(PluginConfigMenuStrings[0], IDM_TEXTTP_TITLE, "Text Templates");
  Info->PluginConfigStrings=PluginConfigMenuStrings;
  Info->PluginConfigStringsNumber=SIZEOF_ARRAY(PluginConfigMenuStrings);
}

//-----------------------------------------------------------------------------
int WINAPI _export GetMinFarVersion(void)
{
 // required Far 1.70 beta 5 (build 1634)
 return MAKEFARVERSION(1,70,1634);
}

void processEditorEvent(int Event, int EditorId, const char *fileName);
void editorDeactivate(int Event, int EditorId);
void editorActivate(int Event, int EditorId, const char *fileName, bool bAgain);

//-----------------------------------------------------------------------------
int WINAPI _export ProcessEditorEvent(int Event, void *Param)
{
 //std::ofstream ofs("PEE.txt", std::ios_base::out|std::ios_base::app);
 switch(Event)
    {
     case EE_SAVE:   
     case EE_REDRAW: 
     case EE_READ:   {
                      EditorInfo ei;
                      if (Info.EditorCtrlGetInfo( &ei ))
                         {
                          processEditorEvent(Event, ei.EditorID, ei.FileName);
                         }
                      else
                         {                          
                         }
                      break;
                     }

     case EE_CLOSE:  {
                      int *piParam = (int*)Param;
                      processEditorEvent(Event, *piParam, 0);
                     }
                     break;
    };

// editor can be obtained earlier using the ECTL_GETINFO EditorControl command. But the plugin should not call
// 
// [+] ECTL_SETPARAM - установка параметров:
//     - "Размер табуляции" (ESPT_TABSIZE)
//     - "Пробелы вместо табуляции" (ESPT_EXPANDTABS)
//     - "Автоотступ" (ESPT_AUTOINDENT)
//     - "Курсор за пределами строки" (ESPT_CURSORBEYONDEOL)


 return 0;
}

//-----------------------------------------------------------------------------
void processEditorEvent(int Event, int EditorId, const char *fileName)
   {
    static int prevActiveEditorId = -1;
    switch(Event)
       {
        case EE_READ:  
        //case EE_SAVE:   
        case EE_REDRAW: 
                        if (prevActiveEditorId>=0 && prevActiveEditorId!=EditorId)
                           editorDeactivate(Event, prevActiveEditorId);
                        editorActivate(Event, EditorId, fileName, prevActiveEditorId==EditorId);
                        prevActiveEditorId = EditorId;
                        break;

        case EE_CLOSE:  
                        editorDeactivate(Event, EditorId);
                        prevActiveEditorId = -1;
       };


    /*
    std::ofstream ofs("PEE.txt", std::ios_base::out|std::ios_base::app);
    switch(Event)
       {
        case EE_READ:   ofs<<"EE_READ"; break;
        case EE_SAVE:   ofs<<"EE_SAVE"; break;
        case EE_REDRAW: ofs<<"EE_REDRAW"; break;
        case EE_CLOSE:  ofs<<"EE_CLOSE"; break;
        default:        ofs<<"EE_UNKNOWN_"<<Event<<""; break;
       };
    ofs<<" - "<<EditorId<<"\n";
    */
    
    // editor can be obtained earlier using the ECTL_GETINFO EditorControl command. But the plugin should not call
    // 
    // [+] ECTL_SETPARAM - установка параметров:
    //     - "Размер табуляции" (ESPT_TABSIZE)
    //     - "Пробелы вместо табуляции" (ESPT_EXPANDTABS)
    //     - "Автоотступ" (ESPT_AUTOINDENT)
    //     - "Курсор за пределами строки" (ESPT_CURSORBEYONDEOL)
   }

//-----------------------------------------------------------------------------
struct EditorFileTimes
{
    std::string                          fileName;
    bool                                 timesAreInvalid;
    FILETIME                             creationTime;
    FILETIME                             lastAccessTime;
    FILETIME                             lastWriteTime;
};

//-----------------------------------------------------------------------------
BOOL getFileTimes(EditorFileTimes &times);
BOOL getFileTimes(EditorFileTimes &times)
   {
    HANDLE hFile = CreateFile( times.fileName.c_str(),
                               0, // dwDesiredAccess - we only get the times
                               0, // dwShareMode
                               0, 
                               OPEN_EXISTING,
                               0, // dwFlagsAndAttributes
                               0);
    if (hFile==INVALID_HANDLE_VALUE)
       {
        times.timesAreInvalid = true;
        return FALSE;
       }

    BOOL res = GetFileTime( hFile, &times.creationTime, &times.lastAccessTime, &times.lastWriteTime);
    CloseHandle(hFile);
    times.timesAreInvalid = (res==FALSE);
    return res;
   }

//-----------------------------------------------------------------------------
struct EditorStateInfo
{
    std::string                          fileName;
    std::string                          fileType;
    TextTP::EditorSettingChangeConfig    originalConfig;
    TextTP::EditorSettingChangeConfig    config;
};

const char* getEventName(int EventId)
   {
    switch(EventId)
       {
        case EE_CLOSE:   return "EE_CLOSE";
        case EE_REDRAW:  return "EE_REDRAW";
        case EE_READ:    return "EE_READ";
        case EE_SAVE:    return "EE_SAVE";
        default: return "EE_UNKNOWN";
       };
   }

const char *getEditorErrorName(int ee)
   {
    switch(ee)
       {
        case EEC_OPEN_ERROR:            return "EEC_OPEN_ERROR";
        case EEC_MODIFIED:              return "EEC_MODIFIED";
        case EEC_NOT_MODIFIED:          return "EEC_NOT_MODIFIED";
        case EEC_LOADING_INTERRUPTED:   return "EEC_LOADING_INTERRUPTED";
        default: return "EEC_UNKNOWN";
       };
   }

//-----------------------------------------------------------------------------
typedef std::map<int, EditorStateInfo>::iterator        editor_state_iterator;
typedef std::map<int, EditorStateInfo>::const_iterator  editor_state_const_iterator;

typedef std::map<int, EditorFileTimes>::iterator        editor_times_iterator;
typedef std::map<int, EditorFileTimes>::const_iterator  editor_times_const_iterator;

//-----------------------------------------------------------------------------
std::map<int, EditorStateInfo>& getEditorStateMap()
   {
    static std::map<int, EditorStateInfo> m;
    return m;
   }

#define DECLARE_STATE_MAP_EX(varName)   std::map<int, EditorStateInfo> &varName = getEditorStateMap()
#define DECLARE_STATE_MAP()             DECLARE_STATE_MAP_EX(m)


//-----------------------------------------------------------------------------
std::map<int, EditorFileTimes>& getEditorTimesMap()
   {
    static std::map<int, EditorFileTimes> m;
    return m;
   }

#define DECLARE_TIMES_MAP_EX(varName)   std::map<int, EditorFileTimes> &varName = getEditorTimesMap()
#define DECLARE_TIMES_MAP()             DECLARE_TIMES_MAP_EX(t)

//-----------------------------------------------------------------------------
void setEditorParameters(const TextTP::EditorSettingChangeConfig &config);
void setEditorParameters(const TextTP::EditorSettingChangeConfig &config)
   {
    EditorSetParameter esp;

    if (config.tabSize>=0)
       {
        esp.Type     = ESPT_TABSIZE;
        esp.Param.iParam   = config.tabSize;
        esp.Flags    = 0;
        esp.Reserved2= 0;
        Info.EditorCtrl(ECTL_SETPARAM, (void*)&esp);
       }

    if (config.expandTabsToSpaces>=0)
       {
        esp.Type     = ESPT_EXPANDTABS;
        esp.Param.iParam   = config.expandTabsToSpaces ? TRUE : FALSE;
        esp.Flags    = 0;
        esp.Reserved2= 0;
        Info.EditorCtrl(ECTL_SETPARAM, (void*)&esp);
       }

    if (config.autoIndent>=0)
       {
        esp.Type     = ESPT_AUTOINDENT;
        esp.Param.iParam   = config.autoIndent ? TRUE : FALSE;
        esp.Flags    = 0;
        esp.Reserved2= 0;
        Info.EditorCtrl(ECTL_SETPARAM, (void*)&esp);
       }

    if (config.cursorBeyondEol>=0)
       {
        esp.Type     = ESPT_CURSORBEYONDEOL;
        esp.Param.iParam   = config.cursorBeyondEol ? TRUE : FALSE;
        esp.Flags    = 0;
        esp.Reserved2= 0;
        Info.EditorCtrl(ECTL_SETPARAM, (void*)&esp);
       }
   }

//-----------------------------------------------------------------------------
bool checkEditorFileIsModified(int EditorId, const char * fileName, bool bClearTimes);
bool checkEditorFileIsModified(int EditorId, const char * fileName, bool bClearTimes)
   {
    DECLARE_TIMES_MAP();
    editor_times_iterator tit = t.find(EditorId);
    if (tit==t.end())
       { // file with id not found, first time file touched
        EditorFileTimes ftimes;
        ftimes.fileName = fileName;
        getFileTimes(ftimes);
        //ftimes.timesAreInvalid = true;
        t[EditorId] = ftimes;
       }
    else
       { // next time event
        if (bClearTimes)
           {
            tit->second.timesAreInvalid = true;
           }
        else
           {
            EditorFileTimes curTimes;
            curTimes.fileName = tit->second.fileName;
            getFileTimes(curTimes);
            if (!curTimes.timesAreInvalid && !tit->second.timesAreInvalid && 
                (curTimes.lastWriteTime.dwLowDateTime != tit->second.lastWriteTime.dwLowDateTime ||
                curTimes.lastWriteTime.dwHighDateTime != tit->second.lastWriteTime.dwHighDateTime)            
               )
               {
                return true;
               }
           }
       }
    return false;
   }

//-----------------------------------------------------------------------------
static int currentEditorId = -1;

//-----------------------------------------------------------------------------
void editorDeactivate(int Event, int EditorId)
   {
    currentEditorId = -1;

    if (Event==EE_CLOSE)
       {
        DECLARE_TIMES_MAP();
        editor_times_iterator tit = t.find(EditorId);
        if (tit!=t.end())
           t.erase(tit);
       }

    if (!TextTP::getConfigAllowEditorSettingsChangeFeature()) return;

    DECLARE_STATE_MAP();
    editor_state_iterator esit = m.find(EditorId);
    if (esit==m.end()) return;
    if (Event == EE_CLOSE)
       m.erase(esit);
    else
       setEditorParameters(esit->second.originalConfig);
   }

//-----------------------------------------------------------------------------
typedef struct tagCSendKeyEvent
{
    BYTE  vkCode;
    DWORD dwFlags;
} CSendKeyEvent;

//-----------------------------------------------------------------------------
void sendKeystroke(int numOfKeys, const CSendKeyEvent *pkeys);
void sendKeystroke(int numOfKeys, const CSendKeyEvent *pkeys)
   {
    for(int i=0; i<numOfKeys; ++i, ++pkeys)
       {
        { SS; ss<<"keybd_event,i: "<<i; SSO(); }
        keybd_event( pkeys->vkCode,
                     0,  /* BYTE bScan This parameter is not used */ 
                     pkeys->dwFlags,
                     0 );
       }
   }



void editorActivate(int Event, int EditorId, const char *fileName, bool bAgain)
   {
    currentEditorId = EditorId;

    if (Event==EE_REDRAW && TextTP::getConfigAllowReloadEditorFiles())
       {
        KeyBarTitles titles;
        for(int i=0; i<12; ++i)
           {
            titles.Titles[i] = titles.CtrlTitles[i] = titles.AltTitles[i] = 
            titles.ShiftTitles[i] = titles.CtrlShiftTitles[i] = 
            titles.AltShiftTitles[i] = titles.CtrlAltTitles[i] = 0;
           }
        titles.AltTitles[2-1] = (char*)::Info.GetMsg(IDM_REOPEN_STR);
        if (!titles.AltTitles[2-1]) titles.AltTitles[2-1] = "Reopen";

        ::Info.EditorControl(ECTL_SETKEYBAR, (void*)&titles);
       }


    //static bool inReload = false;
    //{ SS; ss<<"-----------------------------------\n>>>Event: "<<getEventName(Event)<<", EditorId: "<<EditorId<<", bAgain: "<<bAgain<<", inReload: "<<inReload<<", file: "<<fileName; SSO(); }
    { SS; ss<<"Reload, 1"; SSO(); }
    if (TextTP::getConfigAllowReloadEditorFiles()   /* && !inReload */  && Event==EE_REDRAW)
       { // reload editor file
        { SS; ss<<"Reload, 2"; SSO(); }
        if (checkEditorFileIsModified(EditorId, fileName, Event==EE_SAVE))
           {
            { SS; ss<<"Reload, 3"; SSO(); }
            checkEditorFileIsModified(EditorId, fileName, true);

            CSendKeyEvent keyStroke[] = {
                                         { VK_MENU, 0 },
                                         { VK_F2, KEYEVENTF_EXTENDEDKEY|0 },
                                         { VK_F2, KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP },
                                         { VK_MENU, KEYEVENTF_KEYUP },

                                         { VK_LEFT, KEYEVENTF_EXTENDEDKEY|0 },
                                         { VK_LEFT, KEYEVENTF_EXTENDEDKEY|KEYEVENTF_KEYUP },
                                         { VK_RETURN, 0 },
                                         { VK_RETURN, KEYEVENTF_KEYUP },
                                        };

            int numOfKeys = 4;
            if (TextTP::getConfigReloadEditorFilesFeatureDontAskUser())
               {
                { SS; ss<<"Reload, 4"; SSO(); }
                numOfKeys += 4;
               }

            sendKeystroke(numOfKeys, keyStroke);


            // keybd_event( VK_F2,  /* BYTE bVk */
            //              0,  /* BYTE bScan This parameter is not used */ 
            //              0, 0 );
            // keybd_event( VK_F2,  /* BYTE bVk */
            //              0,  /* BYTE bScan This parameter is not used */ 
            //              KEYEVENTF_KEYUP, 0 );


            // KEYBDINPUT ki = { VK_F2 };
            // INPUT inputs[] = {
            //                   { INPUT_KEYBOARD}
            //                  };
            // inputs[0type
            // UINT uiRes = SendInput( 1,  /* UINT nInputs */
            //                         inputs,  /* LPINPUT pInputs */ 
            //                         sizeof(INPUT)  /* cbSize */ 
            //                       );

            /*
            { SS; ss<<"Reload, calling 'Editor'"; SSO(); }
            inReload = true;
            //ECTL_QUIT
            //Info.EditorControl(ECTL_QUIT, 0);
            int edRes = Info.Editor( fileName, 0, 
                         0, 0, -1, -1, 
                         EF_IMMEDIATERETURN|EF_NONMODAL,
                         ei.CurLine,
                         ei.CurPos);
            inReload = false;
            
            { SS; ss<<"* Reload, Editor returns: "<<getEditorErrorName(edRes)<<"\n<<<Exit by reload"; SSO(); }
            */

            return;
           }
        //inReload = false;
       }

    // if (inReload)
    //    {
    //     return;
    //    }

    if (!TextTP::getConfigAllowEditorSettingsChangeFeature() || bAgain)
       {
        //Warning(IDM_COMMENTING_CODE, "editor setting change not allowed");
        return;
       }

    DECLARE_STATE_MAP();
    editor_state_iterator esit = m.find(EditorId);
    if (esit==m.end())
       { // first time event for current editor
        if (!fileName) 
           {
            //Warning(IDM_COMMENTING_CODE, "no file name given");
            return; // no file name given
           }

        TextTP::TTPConfig  cfgCopy = Config;
        readCustomConfig(fileName, cfgCopy);

        std::string editorFileType;
        if (!lookupForFileType(cfgCopy, fileName, editorFileType, Info.StdFn()))
           {
            //SS; ss<<"FileType not found for file "<<fileName;
            //Warning(IDM_COMMENTING_CODE, ss.str());
            return;
           }

        std::map<std::string, TextTP::EditorSettingChangeConfig>::const_iterator escIt = cfgCopy.editorSettingConfig.find(editorFileType);
        if (escIt==cfgCopy.editorSettingConfig.end())
           {
            //SS; ss<<"Editor settings not found for file-type "<<editorFileType<<"\nCount of esConfig items: "<<cfgCopy.editorSettingConfig.size();
            //Warning(IDM_COMMENTING_CODE, ss.str());
            return;
           }

        EditorStateInfo stateInfo;
        stateInfo.fileName = fileName;
        stateInfo.fileType = editorFileType;
        stateInfo.config   = escIt->second;
        //stateInfo.originalConfig = { -1, -1, -1, -1 };
        stateInfo.originalConfig.tabSize = -1;           
        stateInfo.originalConfig.expandTabsToSpaces = -1;
        stateInfo.originalConfig.autoIndent = -1;        
        stateInfo.originalConfig.cursorBeyondEol = -1;   

        //TextTP::EditorSettingChangeConfig eCfg = { -1, -1, -1, -1 };

        EditorInfo ei;
        if (Info.EditorCtrlGetInfo( &ei ))
           {
            // Options EOPT_EXPANDTABS EOPT_AUTOINDENT EOPT_CURSORBEYONDEOL
            // TabSize
            if (ei.Options & EOPT_EXPANDTABS)
               stateInfo.originalConfig.expandTabsToSpaces = 1;
            else
               stateInfo.originalConfig.expandTabsToSpaces = 0;

            if (ei.Options & EOPT_AUTOINDENT)
               stateInfo.originalConfig.autoIndent = 1;
            else
               stateInfo.originalConfig.autoIndent = 0;

            if (ei.Options & EOPT_CURSORBEYONDEOL)
               stateInfo.originalConfig.cursorBeyondEol = 1;
            else
               stateInfo.originalConfig.cursorBeyondEol = 0;

            stateInfo.originalConfig.tabSize = ei.TabSize;
           }

        esit = m.insert(std::make_pair(EditorId, stateInfo)).first;
       }

    setEditorParameters(esit->second.config);
   }

//-----------------------------------------------------------------------------
//int WINAPI _export ProcessEvent( HANDLE hPlugin, int Event, void *Param );
int WINAPI _export ProcessEvent( HANDLE hPlugin, int Event, void *Param )
   {
    // Warning(IDM_COMMENTING_CODE, "ProcessEvent called");
    // { SS; ss<<"ProcessEvent"; SSO(); }

    if (Event==FE_IDLE) {  /* SS; ss<<"* Event==FE_IDLE"; SSO(); */  }
    return FALSE;
   }

//-----------------------------------------------------------------------------
int WINAPI ProcessEditorInput( const INPUT_RECORD *pRec )
  {
   if (!pRec) return 0;
   if (!TextTP::getConfigAllowReloadEditorFiles()) return 0;
   if (pRec->EventType!=KEY_EVENT) return 0;
   //pRec->Event.KeyEvent.


   { 
    SS; 
    char buf[1024];
    wsprintf(buf, "PEI. bDown: %s, vkCode: 0x%02x, scanCode: 0x%02x (%d), flags: 0x%08x",
             (pRec->Event.KeyEvent.bKeyDown ? "TRUE" : "FALSE"),
             (unsigned)pRec->Event.KeyEvent.wVirtualKeyCode,
             (unsigned)pRec->Event.KeyEvent.wVirtualScanCode,
             (unsigned)pRec->Event.KeyEvent.wVirtualScanCode,
             (unsigned)pRec->Event.KeyEvent.dwControlKeyState
             );
    ss<<buf;
    SSO(); 
   }
   
   if (pRec->Event.KeyEvent.bKeyDown!=FALSE &&
       pRec->Event.KeyEvent.wVirtualKeyCode == VK_F2 &&
       (pRec->Event.KeyEvent.dwControlKeyState&(RIGHT_ALT_PRESSED|LEFT_ALT_PRESSED)))
      {
       { SS; ss<<"ProcessEditorInput 1"; SSO(); }
       if (currentEditorId<0) return 0; // 
       { SS; ss<<"ProcessEditorInput 2"; SSO(); }

       DECLARE_TIMES_MAP();
       editor_times_iterator tit = t.find(currentEditorId);
       if (tit!=t.end())
          {
           { SS; ss<<"ProcessEditorInput 3"; SSO(); }
           EditorInfo ei;
           if (!Info.EditorCtrlGetInfo( &ei ))
              {
               ei.WindowSizeX = 80;
               ei.WindowSizeY = 25;
               ei.CurLine = -1;
               ei.CurPos  = -1;
               ei.TopScreenLine = 0;
               ei.LeftPos = 0;
               ei.BlockType = BTYPE_NONE;
               ei.BlockStartLine = 0;
              }
           int edRes = Info.Editor( tit->second.fileName.c_str(), 0, 
                        0, 0, -1, -1, 
                        EF_IMMEDIATERETURN|EF_NONMODAL,
                        ei.CurLine+1,
                        ei.CurPos+1);
           return 1;
          }
       else
          {
           { SS; ss<<"ProcessEditorInput 4"; SSO(); }
          }
      }
   return 0;
  }


