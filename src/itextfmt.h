/* (c)2005-2007 Alex Martynov, amart@mail.ru
 * This source file is a part of "Text Templates" - a plugin for file manager FAR.
 * You can use, modify, redistribute this source file without any limitations.
 * This source file is distributed "as is". No waranty of any kind is expressed or 
 * implied. Use it at your own risk.
 */

#ifndef ITEXTFMT_H
#define ITEXTFMT_H

#if !defined(_WINDOWS_)
    #include <windows.h>
#endif

#include "far/plugin.hpp"



typedef LPVOID (WINAPI *textFormatApiAllocateProc)(SIZE_T nBytes);
typedef BOOL (WINAPI *textFormatApiFreeProc)(LPVOID pMem);



#include <pshpack4.h>

typedef struct tagITextFormatApiAllocator
{
    textFormatApiAllocateProc       pfnAllocate;
    textFormatApiFreeProc           pfnFree;
} ITextFormatApiAllocator, *PITextFormatApiAllocator;

#include <poppack.h>


typedef LPSTR (WINAPI *textFormatApiGetPluginInternalNameProc)(PITextFormatApiAllocator allocator, const FARSTANDARDFUNCTIONS* pStdFns /* Can be NULL */);

typedef LPSTR (WINAPI *textFormatApiFormatTextProc)(LPCSTR text, SIZE_T nBytes,
                                      LPCSTR options, PITextFormatApiAllocator allocator,
                                      const FARSTANDARDFUNCTIONS* pStdFns, /* Can be NULL */
                                      LPSTR *resText, SIZE_T *nResBytes);


/* For internal usage in TextTP */
#include <pshpack4.h>

typedef struct tagTextFormatPluginInfo
{
    HMODULE hPluginModule;
    LPSTR   pluginFileName;
    textFormatApiGetPluginInternalNameProc    getPluginInternalName;
    textFormatApiFormatTextProc               formatText;

} TextFormatPluginInfo, *PTextFormatPluginInfo;

#include <poppack.h>


/* Plugin must export this names */

#define TEXTFORMATAPIGETPLUGININTERNALNAMEPROCNAME "textFormatApiGetPluginInternalName"
#define TEXTFORMATAPIFORMATTEXTPROCNAME            "textFormatApiFormatText"


#endif /* ITEXTFMT_H */

