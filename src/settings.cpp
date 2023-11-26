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
#include "ttpreg.h"

//-----------------------------------------------------------------------------

#ifndef SIZEOF_ARRAY
    #define SIZEOF_ARRAY(a)   (sizeof(a)/sizeof(a[0]))
#endif



extern Far::CPluginStartupInfo  Info;
extern TextTP::TTPConfig        Config;
extern std::string              plugin_tpl_dir_name;
extern std::string              plugin_root_key;


//-----------------------------------------------------------------------------
void InitDialogItems(const struct InitDialogItem *Init,struct FarDialogItem *Item,
                    int ItemsNumber)
{
  int I;
  struct FarDialogItem *PItem=Item;
  const struct InitDialogItem *PInit=Init;
  for (I=0;I<ItemsNumber;I++,PItem++,PInit++)
  {
    PItem->Type=PInit->Type;
    PItem->X1=PInit->X1;
    PItem->Y1=PInit->Y1;
    PItem->X2=PInit->X2;
    PItem->Y2=PInit->Y2;
    PItem->Focus=PInit->Focus;
    PItem->Selected=PInit->Selected;
    PItem->Flags=PInit->Flags;
    PItem->DefaultButton=PInit->DefaultButton;
    strcpy(PItem->Data,((DWORD)PInit->Data<2000)?::Info.GetMsg((unsigned int)PInit->Data):PInit->Data);
  }
}



//-----------------------------------------------------------------------------
  
namespace TextTP
{




//-----------------------------------------------------------------------------
int getConfigInputMethod()
{
    int val = 1;
    GetRegKey(HKEY_CURRENT_USER, 0 /*plugin_root_key.c_str()*/, "UseInputBox", val, 1);
    return val ? TTPINP_DIALOG : TTPINP_TEXT;
    //return TTPINP_TEXT;
}

//-----------------------------------------------------------------------------
BOOL getConfigAutoComplete()
{
    int val = 0;
    GetRegKey(HKEY_CURRENT_USER, 0 /*plugin_root_key.c_str()*/, "UseAutoCompletion", val, 0);
    return val;
    //return FALSE;
}

//-----------------------------------------------------------------------------
BOOL getConfigShowErrors()
{
    int val = 0;
    GetRegKey(HKEY_CURRENT_USER, 0 /*plugin_root_key.c_str()*/, "ShowErrorMessages", val, 0);
    return val;
    //return FALSE;
}

BOOL getConfigAllowEditorSettingsChangeFeature()
{
    int val = 0;
    GetRegKey(HKEY_CURRENT_USER, 0 /*plugin_root_key.c_str()*/, "AllowEditorSettingsChangeFeature", val, 0);
    return val;
    //return FALSE;
}

//-----------------------------------------------------------------------------
BOOL getConfigDisablePreloading()
{
    int val = 0;
    GetRegKey(HKEY_CURRENT_USER, 0 /*plugin_root_key.c_str()*/, "DisablePreloading", val, 0);
    return val;
    //return FALSE;
}

//-----------------------------------------------------------------------------
BOOL getConfigAllowReloadEditorFiles()
{
    int val = 0;
    GetRegKey(HKEY_CURRENT_USER, 0 /*plugin_root_key.c_str()*/, "AllowReloadEditorFiles", val, 0);
    return val;
    //return FALSE;
}

//-----------------------------------------------------------------------------
BOOL getConfigReloadEditorFilesFeatureDontAskUser()
{
    int val = 0;
    GetRegKey(HKEY_CURRENT_USER, 0 /*plugin_root_key.c_str()*/, "ReloadEditorFilesFeatureDontAskUser", val, 0);
    return val;
    //return FALSE;
}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
void settingsDialog()
{
  InitDialogItem InitItems[]={
  /* 0 */{DI_DOUBLEBOX,2,1,70,7,0,0,0,0,(char *)IDM_TEXTTP_TITLE},
  /* 1 */{DI_CHECKBOX,4,3,0,0,0,getConfigInputMethod()==TTPINP_DIALOG?TRUE:FALSE,0,0,(char *)IDM_USE_INPUT_BOX},
  /* 2 */{DI_CHECKBOX,4,4,0,0,0,getConfigAutoComplete(),0,0,(char *)IDM_USE_AUTOCOMPLETION},
  /* 3 */{DI_CHECKBOX,4,5,0,0,0,getConfigShowErrors(),0,0,(char *)IDM_SHOWERRORS},

  /* 4 */{DI_DOUBLEBOX,2, 8,70,15,0,0,0,0,(char *)IDM_TTP_EXT_FEATURES},
  /* 5 */{DI_CHECKBOX, 4,10,0,0,0,getConfigDisablePreloading(),0,0,(char *)IDM_DISABLE_PRELOADING_PLUGIN_FEATURE},
  /* 6 */{DI_CHECKBOX, 4,11,0,0,0,getConfigAllowEditorSettingsChangeFeature(),0,0,(char *)IDM_ALLOW_EDITOR_SETTING_CHANGE_FEATURE},
  /* 7 */{DI_CHECKBOX, 4,12,0,0,0,getConfigAllowReloadEditorFiles(),0,0,(char *)IDM_ALLOW_RELOAD_EDITOR_FILES},
  /* 8 */{DI_CHECKBOX, 8,13,0,0,0,getConfigReloadEditorFilesFeatureDontAskUser(),0,0,(char *)IDM_RELOAD_EDITOR_FILES_FEATURE_DONT_ASK},

  /* 9 */{DI_BUTTON,28,17,0,0,0,0,0,1,(char *)"Ok"},
  /* 10*/{DI_BUTTON,35,17,0,0,0,0,0,0,(char *)IDM_CANCEL},
  };

  FarDialogItem DialogItems[SIZEOF_ARRAY(InitItems)];
  InitDialogItems(InitItems,DialogItems, SIZEOF_ARRAY(InitItems));


  int res = ::Info.Dialog(-1, -1, 72, 19, 0, /* HelpTopic */
                          DialogItems, SIZEOF_ARRAY(DialogItems));
  if (res>0)
     {
      SetRegKey(HKEY_CURRENT_USER, 0 /*plugin_root_key.c_str()*/, "UseInputBox", (DWORD)DialogItems[1].Selected);
      SetRegKey(HKEY_CURRENT_USER, 0 /*plugin_root_key.c_str()*/, "UseAutoCompletion", (DWORD)DialogItems[2].Selected);
      SetRegKey(HKEY_CURRENT_USER, 0 /*plugin_root_key.c_str()*/, "ShowErrorMessages", (DWORD)DialogItems[3].Selected);

      SetRegKey(HKEY_CURRENT_USER, 0 /*plugin_root_key.c_str()*/, "DisablePreloading"                  , (DWORD)DialogItems[5].Selected);
      SetRegKey(HKEY_CURRENT_USER, 0 /*plugin_root_key.c_str()*/, "AllowEditorSettingsChangeFeature"   , (DWORD)DialogItems[6].Selected);
      SetRegKey(HKEY_CURRENT_USER, 0 /*plugin_root_key.c_str()*/, "AllowReloadEditorFiles"             , (DWORD)DialogItems[7].Selected);
      SetRegKey(HKEY_CURRENT_USER, 0 /*plugin_root_key.c_str()*/, "ReloadEditorFilesFeatureDontAskUser", (DWORD)DialogItems[8].Selected);
      // save
     }


}


}; // namespace TextTP


