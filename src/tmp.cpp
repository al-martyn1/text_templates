
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

#ifndef STRICT
    #define STRICT
#endif

#if !defined(_WINDOWS_)
    #include <windows.h>
#endif


#if !defined(_INC_MALLOC) && !defined(_MALLOC_H_) && !defined(_MALLOC_H)
    #include <malloc.h>
#endif


#include "fileOp.h"
#include "cfarcls.h"



extern Far::CPluginStartupInfo         Info;

// #include <fstream>



//-----------------------------------------------------------------------------
std::string getFileExt(const std::string &fn)
    {
     std::string::size_type pos = fn.rfind('.');
     if (pos==fn.npos || pos+MAX_EXT_SIZE <= fn.size()) 
        return std::string(".");

     std::string::size_type len = fn.size() - pos;
     char *buf = (char*)_alloca(len+1);
     fn.copy(buf, len, pos);
     buf[len] = 0;
     ::Info.StdFn()->LStrlwr(buf);
     return std::string(buf);
    }

//-----------------------------------------------------------------------------
std::string getFilePath(const std::string &fn)
    {
     std::string::size_type pos1 = fn.rfind('/');
     std::string::size_type pos2 = fn.rfind('\\');
     std::string::size_type pos = fn.npos;

     if (pos1!=fn.npos && pos2!=fn.npos)
        {
         pos = pos1>pos2 ? pos1 : pos2;
        }
     else if (pos1!=fn.npos)
        pos = pos1;
     else 
        pos = pos2;

     if (pos==fn.npos) return std::string();
     return std::string(fn, 0, pos + 1);    
    }

//-----------------------------------------------------------------------------
/*
bool FileExists(const char *fname)
{
  return std::ifstream(fname) != NULL;
}
*/
bool FileExists(const char *fname)
   {
    if (!fname) return false;

    HANDLE hFile = CreateFile( fname,
                               GENERIC_READ,
                               0, // share mode
                               0, // lpSecurityAttributes
                               OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL,
                               0);
    if (hFile==INVALID_HANDLE_VALUE)
       return false;

    CloseHandle(hFile);
    return true;
   }

//-----------------------------------------------------------------------------
std::string makeCanonicalFileName(const std::string &name)
    {
     std::string res = name;
     std::string::size_type pos = 0, size = res.size();
     for(; pos<size; ++pos)
        {
         if (res[pos]=='\\') res[pos] = '/';
        }
     return res;
    }

//-----------------------------------------------------------------------------
std::string makeSystemFileName(const std::string &name)
    {
     std::string res = name;
     std::string::size_type pos = 0, size = res.size();
     for(; pos<size; ++pos)
        {
         if (res[pos]=='/') res[pos] = '\\';
        }
     return res;
    }
//-----------------------------------------------------------------------------


