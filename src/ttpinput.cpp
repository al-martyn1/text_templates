
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

#if !defined(_SSTREAM_) && !defined(_STLP_SSTREAM) && !defined(__STD_SSTREAM__) && !defined(_CPP_SSTREAM) && !defined(_GLIBCXX_SSTREAM)
    #include <sstream>
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

#ifndef FILENAME_H
    #include <filename.h>
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

#include "far/farkeys.hpp"

#include "compareWC.h"

#include "fileOp.h"

/*
#include <fstream>
bool FileExists(const char *fname)
{
  return std::ifstream(fname) != NULL;
}
*/

//-----------------------------------------------------------------------------

#ifndef SIZEOF_ARRAY
    #define SIZEOF_ARRAY(a)   (sizeof(a)/sizeof(a[0]))
#endif

//#ifndef MAX_EXT_SIZE
//    #define MAX_EXT_SIZE     15
//#endif

#define SS std::stringstream ss


//-----------------------------------------------------------------------------

extern Far::CPluginStartupInfo  Info;
extern TextTP::TTPConfig        Config;
extern std::string              plugin_tpl_dir_name;

//-----------------------------------------------------------------------------
  
namespace TextTP
{


using namespace Rsdn;
using namespace Rsdn::Serializer;
using namespace Rsdn::Serializer::FieldAttrubutes;
using namespace Rsdn::Serializer::Tree;
using namespace Rsdn::Serializer::Tree::Xml;
using namespace Rsdn::Serializer::Primitives;

//-----------------------------------------------------------------------------
 // -1 - error, 0 - no sush file, 1 - ok
#define CATCH_READ_CUSTOM_INFO_ERR(exept_type, str)                         \
catch(const exept_type &e)                                                  \
    {                                                                       \
     why = std::string("\nError: ") + std::string(str) + std::string("\n"); \
     why += e.what();                                                       \
     return -1;                                                            \
    }

//-----------------------------------------------------------------------------
int readCustomInfo(const std::string &file, CCustomInfo &ci, std::string &why)
    {
     if (!FileExists(file.c_str())) return 0;

     std::string msg("File: ");
     std::string read_broken_at_file = file;

     try {
          INIT_COM();                               //texttp-custom-info
          Rsdn::Serializer::Tree::Xml::Load(file, _T("texttp-custom-info"), &ci);
          return 1;
         }
     CATCH_READ_CUSTOM_INFO_ERR( MondatoryFieldException, "MondatoryFieldException")
     CATCH_READ_CUSTOM_INFO_ERR( XmlStorageException, "XmlStorageException")
     CATCH_READ_CUSTOM_INFO_ERR( StorageException, "StorageException")
     CATCH_READ_CUSTOM_INFO_ERR( ParseValueException, "ParseValueException")
     CATCH_READ_CUSTOM_INFO_ERR( Exception, "Exception")
     CATCH_READ_CUSTOM_INFO_ERR( std::exception, "std::exception")
     //CATCH_READ_CUSTOM_INFO_ERR( , "")

    catch(...)
        {
         why = std::string("\nError: Unknown error (...)\n");
         return -1;
        }
     return -1;
    }

//-----------------------------------------------------------------------------
bool lookupForCustomFile(const std::string &editorFileName, std::string &foundFileName)
   {
    std::string editorCanonicalFileName = makeCanonicalFileName(editorFileName);
    std::string filePath = getFilePath(editorCanonicalFileName);
    //std::string foundFileName;
    do {
        std::string tmp = makeSystemFileName(filePath + std::string("texttp-custom-info.xml"));
        if (FileExists(tmp.c_str())) 
           { 
            foundFileName = tmp;
            break;
           }
        // up to one level directory
        filePath.erase(filePath.size()-1); 
        filePath = getFilePath(filePath);
       } while(filePath.size()>2);

    if (foundFileName.empty()) return false;
    return true;
   }

//-----------------------------------------------------------------------------
void readCustomConfig(const std::string &ed_fname, TextTP::TTPConfig &cfg)
    {
/*
        std::string editorCanonicalFileName = makeCanonicalFileName(ed_fname);
        std::string filePath = getFilePath(editorCanonicalFileName);
        std::string foundFileName;
        
        do {
            std::string tmp = makeSystemFileName(filePath + std::string("texttp-custom-info.xml"));
            if (FileExists(tmp.c_str())) 
               { 
                foundFileName = tmp;
                break;
               }
            // up to one level directory
            filePath.erase(filePath.size()-1); 
            filePath = getFilePath(filePath);
           } while(filePath.size()>2);

        if (foundFileName.empty()) return;
*/

        //std::string foundFileName;
        std::string cust_conf_file; // = foundFileName;
        //if (!lookupForCustomFile(ed_fname, foundFileName)
        if (!lookupForCustomFile(ed_fname, cust_conf_file))
            return;

        CCustomInfo ci;

        std::string err_descr("no error");
        int rci_res = readCustomInfo(cust_conf_file, ci, err_descr);
        if (rci_res<1)
           {
            if (!rci_res) 
               {
                return; // file not found
               }

            if (getConfigShowErrors())
               {
                std::string msg("Failed to read custom config file\nFile: ");
                msg += cust_conf_file;
                //msg += "\n";
                msg += err_descr;
                Warning(IDM_TEXTTP_CFGERR_TITLE, msg);
               }
            return;
           }
        else
           {
            //Warning(IDM_TEXTTP_CFGERR_TITLE, "read custom config: res>0");
           }

        cfg.template_dirs.push_front(ci.template_dir);
        if (ci.config_file.empty()) 
           {
            //Warning(IDM_TEXTTP_CFGERR_TITLE, "DialogData::readCustomConfig 5");
            if (getConfigShowErrors())
               Warning(IDM_TEXTTP_CFGERR_TITLE, "Custom config: config file name not defined (use config-name tag)");
           }
        else
           {
            //Warning(IDM_TEXTTP_CFGERR_TITLE, "DialogData::readCustomConfig 4");
            cfg.readFromFile(ci.config_file);
           }
    }

//-----------------------------------------------------------------------------


bool lookupForFileType(const TextTP::TTPConfig &cfg, const std::string &fileName, std::string &foundFileType, const FARSTANDARDFUNCTIONS* fns)
   {
    std::string onlyName = filename::getFile(fileName);
    onlyName = TextTP::Utils::strlwr(onlyName, fns);

    std::string nameWithPath = fileName;
    nameWithPath = TextTP::Utils::strlwr(nameWithPath, fns);
    //std::string ext = getFileExt(fileName);

    // SS;
    // ss<<"onlyName: "<<onlyName<<", pathName: "<<nameWithPath<<"\n---------------\n";

            //SS;
            //ss<<"Main, selected, startPos: "<<startPos<<",  endPos: "<<endPos;
            //Warning(IDM_COMMENTING_CODE, ss.str());

    std::vector<FiletypeExtentionsInfo2>::const_iterator it = cfg.file_types.begin();
    for(; it!=cfg.file_types.end(); ++it)
       {
        std::string &cmpVal = it->use_path ? nameWithPath : onlyName;
        // ss<<"cmpVal: "<<cmpVal<<"; ";

        std::vector<std::string>::const_iterator maskIt = it->mask_list.begin();
        for(; maskIt!=it->mask_list.end(); ++maskIt)
           {
            std::string lwcMask = Utils::strlwr(*maskIt, fns);
            // ss<<*maskIt<<"("<<lwcMask<<"), ";

            if (filename::matchMaskE(cmpVal, lwcMask))
               {
                // ss<<"\nFound!\n";
                // Warning(IDM_COMMENTING_CODE, ss.str());
                foundFileType = it->file_type;
                return true;
               }
           }
        // ss<<"\n";
        // Warning(IDM_COMMENTING_CODE, ss.str());
       }

    // std::map<std::string, std::string>::const_iterator it = cfg.file_types.find(ext);
    // if (it!=cfg.file_types.end())
    //    {
    //     foundFileType = it->second;
    //     return true;
    //    }
    // else
   //{ // поиск 'в лоб' результатов не дал
     // пробуем поискать по маске
     //Warning(IDM_TEXTTP_CFGERR_TITLE, "File ext not found by force search, try lookup with wildcards");
    //std::map<std::string, std::string>::const_iterator it = cfg.file_types.begin();

    // for(; it!=cfg.file_types.end(); ++it)
    //    {
    //     //if (compareWildcardStrings(ext, it->first))
    //     if (compareWildcardStrings(ext, it->first))
    //        { // found by mask
    //         foundFileType = it->second;
    //         //Warning(IDM_TEXTTP_CFGERR_TITLE, "File ext not found by force search, try lookup with wildcards");
    //         return true;
    //        }
    //    }
     // not found by mask
     //return;
   //}
       //return;
    return false;
   }

//-----------------------------------------------------------------------------
DialogData::DialogData( const TextTP::TTPConfig &cfg, const std::string &ed_fname ) 
        : Config(cfg),
          editor_file_name(ed_fname), 
          editor_file_type(), 
          shortcuts(), 
          found_shortcut(), 
          bfound(false)
    {
     readCustomConfig(editor_file_name, Config);

     editor_file_type.erase();

/*
     std::string ext = getFileExt(editor_file_name);

        { //Utils::strlwr(ext_list, stdfns);
         std::map<std::string, std::string>::const_iterator it = Config.file_types.find(ext);
         if (it!=Config.file_types.end())
            {
             editor_file_type = it->second;
            }
         else
            { // поиск 'в лоб' результатов не дал
              // пробуем поискать по маске
              //Warning(IDM_TEXTTP_CFGERR_TITLE, "File ext not found by force search, try lookup with wildcards");
              it = Config.file_types.begin();
              for(; it!=Config.file_types.end(); ++it)
                 {
                  if (compareWildcardStrings(ext, it->first))
                     { // found by mask
                      editor_file_type = it->second;
                      //Warning(IDM_TEXTTP_CFGERR_TITLE, "File ext not found by force search, try lookup with wildcards");
                      break;
                     }
                 }
              // not found by mask
              //return;
            }
            //return;
         if (editor_file_type.empty()) return;
        }
*/
     if (!lookupForFileType(Config, editor_file_name, editor_file_type, ::Info.StdFn()))
        return;

/*
     std::vector<TextTP::ShortcutInfo>::const_iterator it = Config.shortcuts.begin();
     for (; it!=Config.shortcuts.end(); it++)
         {     
          //if (it->file_type=="*" || it->file_type==editor_file_type)
          //   shortcuts.push_back(*it);
          if (it->file_type=="*")
             {
              shortcuts.push_back(*it);
              continue;
             }

          std::vector<std::string> fileTypesVec;
          TextTP::Utils::split_string(fileTypesVec, it->file_type, ',');
          std::vector<std::string>::const_iterator ftIt = fileTypesVec.begin();
          for(; ftIt!=fileTypesVec.end(); ++ftIt)
             {
              if (*ftIt==editor_file_type)
                 {
                  shortcuts.push_back(*it);
                  continue;
                 }
             }
         }
*/
     findInfo(editor_file_type, Config.shortcuts.begin(), Config.shortcuts.end(), shortcuts);

     std::sort(shortcuts.begin(), shortcuts.end());
    }
//-----------------------------------------------------------------------------









//-----------------------------------------------------------------------------
inline
bool isValidShortcutChar(char ch)
{
    static const char *valid_chars = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789"
"_-:./*";
    const char* tmp = valid_chars;
    while(*tmp && *tmp!=ch) tmp++;
    if (*tmp) return true;
    return false;
}




//-----------------------------------------------------------------------------
int ShowInputDlg(const EditorInfo &ei, DialogData *pdata);
long WINAPI InputTplNameDialogProc( HANDLE hDlg, int Msg, int Param1, long Param2);
std::vector<TextTP::ShortcutInfo>::const_iterator 
findShortcut( const std::vector<TextTP::ShortcutInfo> &v, 
              int &count,
              const std::string &name);

//-----------------------------------------------------------------------------
std::vector<TextTP::ShortcutInfo>::const_iterator 
findShortcut(const std::vector<TextTP::ShortcutInfo> &v, 
             int &count,
             const std::string &name)
{
    count = 0;
    std::vector<TextTP::ShortcutInfo>::const_iterator 
                                          its = v.end();
    std::vector<TextTP::ShortcutInfo>::const_iterator it = v.begin();
    for (; it!=v.end(); it++)
        {
         if (it->partial_cmp_name(name))
            {
             its = it; 
             break;
            }                  
        }

    for (; it!=v.end(); it++, count++)
         if (!it->partial_cmp_name(name))
             break;

    return its;
}

//-----------------------------------------------------------------------------
/* return TTPINP_* */
int  getShortcutInput( DialogData &dd, TextTP::ShortcutInfo &shi, const std::string &editor_file_name,
                       int &Line, int &Pos, int &len)
{
    EditorInfo edinfo;
    ::Info.EditorCtrlGetInfo( &edinfo );
        
    if (dd.shortcuts.empty())
       return TTPINP_CANCEL;

    if (getConfigInputMethod()==TTPINP_DIALOG)
       {
        int dlg_res = ShowInputDlg(edinfo, &dd);
        //ATLTRACE("dlg_res: %d\n", dlg_res);
        if (dlg_res<0) return TTPINP_CANCEL;
        if (!dd.bfound) return TTPINP_CANCEL;
        shi = dd.found_shortcut;
        return TTPINP_DIALOG;
       }

    EditorGetString str_info = { -1 }; // cur line
    if (!::Info.EditorCtrlGetString( &str_info))
       return TTPINP_CANCEL;

    if (!str_info.StringText)
       return TTPINP_CANCEL;

    int pos_end = edinfo.CurPos;
    if (!isValidShortcutChar(str_info.StringText[pos_end]))
       { // ищем конец слова назад (так как находимся на пробельном символе)
        if (!pos_end || !isValidShortcutChar(str_info.StringText[pos_end-1]))
           return TTPINP_CANCEL; // предыдущий символ также пробельный
        // всю строку не сканируем
        // pos_end++; // pos after end
       }
    else
       { // ищем конец слова вперед, так как находимся в середине слова
        while(pos_end<str_info.StringLength)
            {
             if (!isValidShortcutChar(str_info.StringText[pos_end]))
                break;
             else
                pos_end++;
            }
       }

    int pos_start = pos_end-1;
    while(pos_start>=0)
        {
         if (!isValidShortcutChar(str_info.StringText[pos_start]))
            break;
         else pos_start--;
        }
    pos_start++;
    if (pos_start==pos_end)
       return TTPINP_CANCEL;

    len = pos_end - pos_start;
    std::string name(str_info.StringText + pos_start, len);

    int cnt = 0;
    std::vector<TextTP::ShortcutInfo>::const_iterator 
    it = findShortcut( dd.shortcuts, cnt, name);

    if (cnt!=1)
       return TTPINP_CANCEL;

    shi = *it;
    Pos = pos_start;
    Line = edinfo.CurLine;
    return TTPINP_TEXT;
}

//-----------------------------------------------------------------------------
int ShowInputDlg(const EditorInfo &ei, DialogData *pdata)
{
    FarDialogItem Items[1] = { { 0 } };

    int xsize = 33;
    int ysize = 1;
    //InputTplNameBuf[0] = 0;

    Items[0].Type = DI_EDIT;
    Items[0].X1 = 0; 
    Items[0].Y1 = 0; 
    Items[0].X2 = xsize-1; 
    Items[0].Y2 = ysize-1;
    Items[0].Focus = TRUE;
    Items[0].Flags = 0 ;//DIF_VAREDIT;
    Items[0].DefaultButton = 1;
    
    //Items[0].Ptr.PtrData   = InputTplNameBuf;
    //Items[0].Ptr.PtrLength = xsize-1; //SIZEOF_ARRAY(InputTplNameBuf)-1;

    int ypos = ei.CurLine - ei.TopScreenLine + 1;
    int xpos = ei.CurPos  - ei.LeftPos;

    if (xpos+xsize-1 >= ei.WindowSizeX )
       xpos = ei.WindowSizeX + 1 - xsize;
    if (ypos+ysize-1 >= ei.WindowSizeY )
       ypos = ei.WindowSizeY + 1 - ysize;

    return Info.DialogEx( xpos, ypos, xpos+xsize-1, ypos+ysize-1, 0, /* HelpTopic*/
                          Items, SIZEOF_ARRAY(Items), 0, /* Reserved */
                          FDLG_SMALLDIALOG | FDLG_NODRAWSHADOW/* FDLG_NODRAWPANEL */,
                          InputTplNameDialogProc, (long)pdata
                        );

}

//-----------------------------------------------------------------------------
long WINAPI InputTplNameDialogProc( HANDLE hDlg, int Msg, int Param1, long Param2)
{
    switch(Msg)
       {
        case DN_INITDIALOG:
             Info.SendDlgMessage(hDlg, DM_SETDLGDATA, 0, Param2);
             return Info.DefDlgProc(hDlg, Msg, Param1, Param2);

        case DN_KEY:
             if (Param1==0)
                {
                 DialogData *pdata = (DialogData*)Info.SendDlgMessage(hDlg, DM_GETDLGDATA, 0, 0);
                 if (Param2==KEY_BS)
                    pdata->bs_pressed = true;
                 else
                    pdata->bs_pressed = false;
                }
             //ATLTRACE("DN_KEY, code: %d\n", Param2);
             return Info.DefDlgProc(hDlg, Msg, Param1, Param2);

        case DN_CLOSE:
             {
              //ATLTRACE("DN_CLOSE, Param1: %d\n", Param1);
              if (!getConfigAutoComplete()) return TRUE;

              DialogData *pdata = (DialogData*)Info.SendDlgMessage(hDlg, DM_GETDLGDATA, 0, 0);
              pdata->bfound = false;

              if (Param1<0) 
                 {                  
                  return TRUE;
                 }

              FarDialogItem item;
              Info.SendDlgMessage(hDlg, DM_GETDLGITEM, 0, (long)&item);

              int cnt = 0;
              std::vector<TextTP::ShortcutInfo>::const_iterator 
                  it = findShortcut(pdata->shortcuts,  cnt, std::string(item.Data));
              if (cnt==1)
                 {
                  pdata->bfound = true;
                  pdata->found_shortcut = *it;
                 }
              return TRUE;
             }
             

        case DN_EDITCHANGE:
             {
              if (Param1!=0)
                 return Info.DefDlgProc(hDlg, Msg, Param1, Param2);
              DialogData *pdata = (DialogData*)Info.SendDlgMessage(hDlg, DM_GETDLGDATA, 0, 0);
              if (!pdata)
                 return Info.DefDlgProc(hDlg, Msg, Param1, Param2);

              FarDialogItem *pitem = (FarDialogItem*)Param2;


              int cnt = 0;
              std::vector<TextTP::ShortcutInfo>::const_iterator 
                  it = findShortcut(pdata->shortcuts,  cnt, std::string(pitem->Data));

              if (cnt>1) 
                 {
                  pdata->bfound = false;
                  break;
                 }
              if (!cnt)  
                 {
                  pdata->bfound = false;
                  ::Info.SendDlgMessage(hDlg, DM_CLOSE, -1, 0);
                  return TRUE;
                 }

              // cnt==1
              pdata->found_shortcut = *it;
              pdata->bfound = true;

              if (getConfigAutoComplete())
                 {
                  if (it->shortcut == std::string(pitem->Data))
                     break; // already autocompleted
                  if (!pdata->bs_pressed)
                     { 
                      FarDialogItemData item_data = { 0, (char*)it->shortcut.c_str() };
                      ::Info.SendDlgMessage(hDlg, DM_SETTEXT, 0, (long)&item_data);
                     }
                 }
              else
                 {
                  Info.SendDlgMessage(hDlg, DM_CLOSE, 0, 0);
                 }
              break;
             }

        //default: 
       };
    return Info.DefDlgProc(hDlg, Msg, Param1, Param2);
}

//-----------------------------------------------------------------------------
void InsertTemplate  ( DialogData &dd, const TextTP::ShortcutInfo &shi, const std::string &editor_file_name,
                       int input_type, int &Line, int Pos, int len)
{
   EditorInfo edinfo;
   ::Info.EditorCtrlGetInfo( &edinfo );
   int saved_pos  = edinfo.CurPos;
   int saved_line = edinfo.CurLine;

   if (input_type==TTPINP_TEXT)
      {
       saved_pos -= len;
       edinfo.CurPos -= len;
       ::Info.EditorCtrlSetPos( saved_pos, -1 );
      }

   std::string indent;
   if (shi.keep_indent)
      {
       for (int i=0; i<saved_pos; i++) indent += " ";
      }

   std::string text, tpl_file;
   if (!dd.Config.getTemplateText(shi.tpl_name, tpl_file, text))
      {
       // show error message
       return;
      }

   //std::string text = TextTP::templateGetText(shi.tpl_name);
   //if (text.empty()) return;

   text = TextTP::templateParse(text, editor_file_name, shi.tpl_name);
   if (text.empty()) return;

   if (input_type==TTPINP_TEXT)
      {
       EditorGetString str_info = { Line };
       ::Info.EditorCtrlGetString( &str_info);
       if (str_info.StringText)
          {
           char *buf = (char*)_alloca(str_info.StringLength+1);
           strcpy(buf, str_info.StringText);
           strcpy(buf+Pos, str_info.StringText+Pos+len);
          
           EditorSetString set_str = { Line, buf, (char*)str_info.StringEOL, (int)strlen(buf) };
           ::Info.EditorCtrlSetString( &set_str);
          }
      }

   //shi.line_before
   //shi.line_after
   if (shi.line_before)
      {
       EditorGetString str_info = { edinfo.CurLine };
       ::Info.EditorCtrlGetString( &str_info);

       //int saved_pos = edinfo.CurPos;
       ::Info.EditorCtrlSetPos( str_info.StringLength, edinfo.CurLine );
       ::Info.EditorCtrlInsertLine(FALSE);

       ::Info.EditorCtrlGetInfo( &edinfo );
       ::Info.EditorCtrlSetPos( saved_pos, edinfo.CurLine );
      }

   std::vector<std::string> lines;
   TextTP::Utils::split_string(lines, text, '\n');

   int cur_pos = 0, cur_line = 0;
   bool set_cur_pos = TextTP::templateFindCursor(lines, cur_pos, cur_line);

   std::vector<std::string>::const_iterator it = lines.begin();
   for (; it!=lines.end(); it++)
       {
        if (it != lines.begin())
           {
            //::Info.EditorCtrlInsertLine(shi.keep_indent ? TRUE : FALSE);
            ::Info.EditorCtrlInsertLine(FALSE);
            Info.EditorCtrlInsertText(indent.empty() ? "" : indent.c_str());            
           }
        Info.EditorCtrlInsertText(it->empty() ? "" : it->c_str());
       }   

   ::Info.EditorCtrlGetInfo( &edinfo );
   if (shi.line_after)
      {
       EditorGetString str_info = { edinfo.CurLine };
       ::Info.EditorCtrlGetString( &str_info);

       //int saved_pos = edinfo.CurPos;
       ::Info.EditorCtrlGetInfo( &edinfo );
       ::Info.EditorCtrlSetPos( str_info.StringLength, edinfo.CurLine );
       ::Info.EditorCtrlInsertLine(FALSE);

       ::Info.EditorCtrlGetInfo( &edinfo );
       ::Info.EditorCtrlSetPos( saved_pos, edinfo.CurLine );
      }

   if (set_cur_pos)
      {
       if (shi.line_before)
          saved_line ++;
       saved_line += cur_line;
       saved_pos += cur_pos;
       ::Info.EditorCtrlSetPos( saved_pos, saved_line );
      }
}

//-----------------------------------------------------------------------------
void doTextTPJob()
{
    TextTP::ShortcutInfo shi;
    int Line, Pos, len;

    EditorInfo edinfo;
    ::Info.EditorCtrlGetInfo( &edinfo);

    DialogData data(Config, edinfo.FileName);

    int input_type = getShortcutInput( data, shi, edinfo.FileName, Line, Pos, len);
    if (input_type==TTPINP_CANCEL) return;

    InsertTemplate ( data, shi, edinfo.FileName, input_type, Line, Pos, len);
}
//-----------------------------------------------------------------------------





}; // namespace TextTP





