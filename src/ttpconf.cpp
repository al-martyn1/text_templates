
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

#if !defined(_IOSTREAM_) && !defined(_STLP_IOSTREAM) && !defined(__STD_IOSTREAM__) && !defined(_CPP_IOSTREAM) && !defined(_GLIBCXX_IOSTREAM)
    #include <iostream>
#endif

#if !defined(_FSTREAM_) && !defined(_STLP_FSTREAM) && !defined(__STD_FSTREAM__) && !defined(_CPP_FSTREAM) && !defined(_GLIBCXX_FSTREAM)
    #include <fstream>
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


#include "tpl.h"
#include "cfarcls.h"
#include "ttprc.h"
#include "ttpconf.h"
#include "autocom.h"


extern Far::CPluginStartupInfo  Info;


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
void TTPConfig::ReadConfigAux(const char *from_file, std::string &readed_file_name, std::map<std::string, bool> &readed, const FARSTANDARDFUNCTIONS* stdfns)
{
     if (!from_file) return;
     TTPReadConfig rc;

     INIT_COM();

     //HRESULT hres = CoInitialize( 0 );

     readed_file_name = from_file;
     Rsdn::Serializer::Tree::Xml::Load(from_file, _T("texttp"), &rc);

     //if (hres==S_OK || hres==S_FALSE )
     //   CoUninitialize();

     AssignConfig(rc, stdfns);

     std::string path = from_file;
     std::string::size_type pos1 = path.rfind('/');
     if (pos1==path.npos) pos1 = 0;
     std::string::size_type pos2 = path.rfind('\\');
     if (pos2==path.npos) pos2 = 0;
     std::string::size_type pos = pos1>pos2 ? pos1 : pos2;
     if (pos!=0)
        pos++;
     path.erase(pos, path.npos);

     std::vector<FileInclude>::const_iterator it = rc.includes.begin();
     for (; it!=rc.includes.end(); it++)
         {              
          std::string file = path + it->file;
          std::map<std::string, bool>::iterator rit = readed.find(file);
          if (rit!=readed.end()) continue;
          readed[file] = true;
          ReadConfigAux(file.c_str(), readed_file_name, readed, stdfns);
         }
}

//-----------------------------------------------------------------------------
bool TTPConfig::templateIsFile(const std::string &tpl_name, std::string &file_name) const
{
    std::string prefix_file = "file:";
    if (tpl_name.size() < prefix_file.size()) return false;
    bool res = tpl_name.compare(0, prefix_file.size(), prefix_file)==0;
    if (res)
       {
        file_name = std::string(tpl_name, prefix_file.size());
       }
    return res;
}

//-----------------------------------------------------------------------------
bool TTPConfig::templateIsInline(const std::string &tpl_name, std::string &tplText) const
{
    std::string prefix_file = "inline:";
    if (tpl_name.size() < prefix_file.size()) return false;
    bool res = tpl_name.compare(0, prefix_file.size(), prefix_file)==0;
    if (res)
       {
        tplText = std::string(tpl_name, prefix_file.size());
       }
    return res;
}

//-----------------------------------------------------------------------------
bool TTPConfig::templateReadFile(const std::string &dir, const std::string& file, std::string &tpl_text) const
{
    std::string full_name = dir;
    if (!full_name.empty() && full_name[full_name.size()-1]!='/' && full_name[full_name.size()-1]!='\\')
       full_name += "\\";
    full_name += file;
    if (full_name.empty()) return false;

    std::ifstream in(full_name.c_str(), std::ios::in | std::ios::binary); 
    if (!in) return false;

    std::istream_iterator<char> inBegin(in), end;
    in.unsetf(std::ios::skipws);
    copy(inBegin, end, std::inserter(tpl_text, tpl_text.end()));
    return true;
}

//-----------------------------------------------------------------------------
bool TTPConfig::templateReadFile(const std::deque<std::string> &dirs, const std::string& file, std::string &tpl_text) const
{
    std::deque<std::string>::const_iterator it = dirs.begin();
    for (; it!= dirs.end(); it++)
        {
         if (templateReadFile(*it, file, tpl_text)) return true;
        }
    return false;
}

//-----------------------------------------------------------------------------
bool TTPConfig::getTemplateText(const std::deque<std::string> &dirs, const std::string& tpl_name, std::string &tpl_file, std::string &tpl_text) const
{    
    if (templateIsFile(tpl_name, tpl_file))
       {
        return templateReadFile(dirs, tpl_file, tpl_text);
       }
    else if (templateIsInline(tpl_name, tpl_text))
       {
        return true;
       }
    else
       {
        std::map<std::string, std::string>::const_iterator it = templates.find(tpl_name);
        if (it==templates.end()) return false;
        tpl_text = it->second;
        return true;       
       }
    //return templateReadFile(dirs, tpl_file, tpl_text);

    // if (!templateIsFile(tpl_name, tpl_file))
    //    {
    //     std::map<std::string, std::string>::const_iterator it = templates.find(tpl_name);
    //     if (it==templates.end()) return false;
    //     tpl_text = it->second;
    //     return true;       
    //    }
    // return templateReadFile(dirs, tpl_file, tpl_text);
}

//-----------------------------------------------------------------------------
void TTPConfig::readFromFile(const std::string &file)
{
    std::string msg("File: ");
    std::string read_broken_at_file = file;

    try {
         ReadConfig(file.c_str(), read_broken_at_file, ::Info.StdFn());
        }
    catch(const MondatoryFieldException &e)
        {
         msg += read_broken_at_file + std::string("\nError: MondatoryFieldException\n");
         msg += e.what();// ? e.what() : "Unknown error (what==0)";
         Warning(IDM_TEXTTP_CFGERR_TITLE, msg);
        }
    catch(const XmlStorageException &e)
        {
         msg += read_broken_at_file + std::string("\nError: XmlStorageException\n");
         msg += e.what();// ? e.what() : "Unknown error (what==0)";
         Warning(IDM_TEXTTP_CFGERR_TITLE, msg);
        }
    catch(const StorageException &e)
        {
         msg += read_broken_at_file + std::string("\nError: StorageException\n");
         msg += "StorageException - ";
         msg += e.what();// ? e.what() : "Unknown error (what==0)";
         Warning(IDM_TEXTTP_CFGERR_TITLE, msg);
        }
    catch(const ParseValueException &e)
        {
         msg += read_broken_at_file + std::string("\nError: ParseValueException\n");
         msg += e.what();// ? e.what() : "Unknown error (what==0)";
         Warning(IDM_TEXTTP_CFGERR_TITLE, msg);
        }
    catch(const Exception &e)
        {
         msg += read_broken_at_file + std::string("\nError: Exception\n");
         msg += e.what();// ? e.what() : "Unknown error (what==0)";
         Warning(IDM_TEXTTP_CFGERR_TITLE, msg);
        }
    catch(const std::exception &e)
        {
         msg += read_broken_at_file + std::string("\n");
         msg += e.what();// ? e.what() : "Unknown error (what==0)";
         Warning(IDM_TEXTTP_CFGERR_TITLE, msg);
        }
    catch(...)
        {
         msg += read_broken_at_file + std::string("\n");
         msg += "Unknown error (...)";
         Warning(IDM_TEXTTP_CFGERR_TITLE, msg);
        }
}






}; // namespace TextTP


