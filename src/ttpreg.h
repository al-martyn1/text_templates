#ifndef TTPREG_H
#define TTPREG_H


#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

#ifndef STRICT
    #define STRICT
#endif

#if !defined(_WINDOWS_)
    #include <windows.h>
#endif


#include "cfarcls.h"

namespace TextTP
{

void SetRegKey(HKEY hRoot,const char *Key,const char *ValueName,char *ValueData);
void SetRegKey(HKEY hRoot,const char *Key,const char *ValueName,DWORD ValueData);
int GetRegKey(const char *Key,const char *ValueName,char *ValueData,char *Default,DWORD DataSize);
int GetRegKey(HKEY hRoot,const char *Key,const char *ValueName,char *ValueData,char *Default,DWORD DataSize);
int GetRegKey(HKEY hRoot,const char *Key,const char *ValueName,int &ValueData,DWORD Default);
int GetRegKey(HKEY hRoot,const char *Key,const char *ValueName,DWORD Default);
void DeleteRegKey(HKEY hRoot,const char *Key);
void DeleteRegValue(HKEY hRoot,const char *Key,const char *ValueName);

}; // namespace TextTP



#endif

