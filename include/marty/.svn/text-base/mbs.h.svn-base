/*
   Copyright (c) 2007-2009 Alex Martynov, amart at mail.ru
   This code can be used without any limitations.
   This code provided as is, use it on your own risk.
*/

#ifndef MARTY_MBS_H
#define MARTY_MBS_H

/* M build system definitions */

#ifdef WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef STRICT
        #define STRICT
    #endif

    #if !defined(WINVER) || !defined(_WIN32_WINNT)
        #ifndef _WINDOWS_
            #include <windows.h>
        #endif
    #endif
#endif
 


#if !defined(WIN32) && !defined(LINUX) && !defined(FREEBSD)
    #define WIN32 0x0500
#endif

#ifndef __MBS_STRINGISE
    #define __MBS_STRINGISE(name) #name
#endif

#ifndef MBS_STRINGISE
    #define MBS_STRINGISE(name) __MBS_STRINGISE(name)
#endif

#ifndef MBS_STRINGISE_W
    #define MBS_STRINGISE_W(name) L##__MBS_STRINGISE(name)
#endif


#ifndef MBS_PROJECT_NAME
    #ifndef PROJECT_NAME
        #ifdef _MSC_VER
            #pragma message("PROJECT_NAME macro not defined, set up PROJECT_NAME=$(ProjectName) in your project's C++ preporcessor definitions")
            #pragma message("PROJECT_NAME macro required for some automatizations")
        #endif
        #define PROJECT_NAME Project_name_not_taken__Please_set_macro_PROJECT_NAME
    #endif

    #define MBS_PROJECT_NAME  MBS_STRINGISE(PROJECT_NAME)
#endif

/*
#ifndef __MBS_PROJECT_NAME_W
    #define __MBS_PROJECT_NAME_W(name)  L##name
#endif

#ifndef MBS_PROJECT_NAME_W
    #define MBS_PROJECT_NAME_W  __MBS_PROJECT_NAME_W(MBS_PROJECT_NAME)
#endif
*/

#ifdef __GNUC__
    #ifdef __GNUC_MINOR__
        #ifdef __GNUC_PATCHLEVEL__
            #define _GNUC_VER  (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
        #else
            #define _GNUC_VER  (__GNUC__ * 10000 + __GNUC_MINOR__ * 100)
        #endif
    #else
        #define _GNUC_VER  (__GNUC__ * 10000)
    #endif
#endif


#ifdef _GNUC_VER

    #if _GNUC_VER<29500
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(gcc2X)
    #elif _GNUC_VER<30000
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(gcc29X)
    #elif _GNUC_VER<30100
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(gcc30)
    #elif _GNUC_VER<30200
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(gcc31)
    #elif _GNUC_VER<30300
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(gcc32)
    #elif _GNUC_VER<30400
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(gcc33)
    #elif _GNUC_VER<30500
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(gcc34)
    #elif _GNUC_VER<30600
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(gcc35)
    #elif _GNUC_VER<30700
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(gcc36)
    #elif _GNUC_VER<30800
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(gcc37)
    #elif _GNUC_VER<30900
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(gcc38)
    #elif _GNUC_VER<40000
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(gcc39)
    #elif _GNUC_VER<40100
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(gcc40)
    #elif _GNUC_VER<40200
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(gcc41)
    #elif _GNUC_VER<40300
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(gcc42)
    #elif _GNUC_VER<40400
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(gcc43)
    #elif _GNUC_VER<40500
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(gcc44)
    #endif

    #define _MBS_CC_VER  _GNUC_VER

#elif defined(_MSC_VER)

    #if _MSC_VER<1200
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(msvc5ol) // v5 or less
    #elif _MSC_VER<1300
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(msvc6)
    #elif _MSC_VER<1310
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(msvc70)
    #elif _MSC_VER<1400
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(msvc71)
    #else
        #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(msvc80)
    #endif

    #define _MBS_CC_VER  _MSC_VER

#else
    #define MBS_TOOLSET_EXACT_NAME     MBS_STRINGISE(unkncc)
    #define _MBS_CC_VER                0
#endif





#ifndef MBS_TOOLSET_NAME
    #define MBS_TOOLSET_NAME MBS_TOOLSET_EXACT_NAME
#endif



#ifndef MBS_HOST_PLATFORM_NAME
    #ifdef WIN32
        #define MBS_HOST_PLATFORM_NAME      MBS_STRINGISE(win32)
    #elif defined(LINUX)
        #define MBS_HOST_PLATFORM_NAME      MBS_STRINGISE(generic_linux)
    #elif defined(FREEBSD)
        #define MBS_HOST_PLATFORM_NAME      MBS_STRINGISE(generic_freebsd)
    #else
        #define MBS_HOST_PLATFORM_NAME      MBS_STRINGISE(unknown_host_platform)
    #endif
#endif

#ifndef MBS_TARGET_PLATFORM_NAME
    #ifdef WIN32
        #define MBS_TARGET_PLATFORM_NAME    MBS_STRINGISE(win32)
    #elif defined(LINUX)
        #define MBS_TARGET_PLATFORM_NAME    MBS_STRINGISE(generic_linux)
    #elif defined(FREEBSD)
        #define MBS_TARGET_PLATFORM_NAME    MBS_STRINGISE(generic_freebsd)
    #else
        #define MBS_TARGET_PLATFORM_NAME    MBS_STRINGISE(unknown_target_platform)
    #endif
#endif


#ifndef PLATFORMOSVER
    #ifdef WIN32
        #if defined(_WIN32_WINNT)
            #define PLATFORMOSVER   _WIN32_WINNT
        #elif defined(WINVER)
            #define PLATFORMOSVER   WINVER
        #endif
    #else
        //#define PLATFORMOSVER 0
    #endif
#endif



#ifdef WIN32

    #ifdef LINUX
        #error "WIN32 defined macro conflicts with LINUX defined macro"
    #endif

    #ifdef FREEBSD
        #error "WIN32 defined macro conflicts with FREEBSD defined macro"
    #endif

    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT  WIN32
    #endif

#endif

#ifdef LINUX
    
    #ifdef FREEBSD
        #error "LINUX defined macro conflicts with FREEBSD defined macro"
    #endif

#endif


#ifndef DISABLE_CONST_CHAR_MBS_STRINGS

#include <stdlib.h>
#include <stdio.h>

/*
#ifndef MBS_TARGET_PLATFORM_NAME
    static const char * _mbsTargetPlatformName = "unknown_target_platform";
#else
    static const char * _mbsTargetPlatformName = MBS_TARGET_PLATFORM_NAME;
#endif

#ifndef MBS_HOST_PLATFORM_NAME
    static const char * _mbsHostPlatformName = "unknown_host_platform";
#else
    static const char * _mbsHostPlatformName = MBS_HOST_PLATFORM_NAME;
#endif

#ifndef MBS_TOOLSET_NAME
    static const char * _mbsToolsetName = "unkncc";
#else
    static const char * _mbsToolsetName = MBS_TOOLSET_NAME;
#endif

#ifndef MBS_TOOLSET_EXACT_NAME
    static const char * _mbsToolsetExactName = "unkncc";
#else
    static const char * _mbsToolsetExactName = MBS_TOOLSET_EXACT_NAME;
#endif

#ifndef MBS_CONFIGURATION_NAME
    #ifdef _DEBUG
        #ifdef UNICODE
            static const char * _mbsConfigurationName = "unicode_debug";
        #else
            static const char * _mbsConfigurationName = "debug";
        #endif
    #else
        #ifdef UNICODE
            static const char * _mbsConfigurationName = "unicode_release";
        #else
            static const char * _mbsConfigurationName = "release";
        #endif
    #endif

#else
    static const char * _mbsConfigurationName = MBS_CONFIGURATION_NAME;
#endif

*/

#if !defined(_MSC_VER) || _MSC_VER<1400
    #ifndef _sprintf
        #define _sprintf sprintf
        #define _snprintf snprintf
        #define __mbs_undef_sprintf
    #endif
#endif

#define MBS_MAKEAPPVERSIONSTRING_FLAG_NO_PLATFORM_NAME          0x01
#define MBS_MAKEAPPVERSIONSTRING_FLAG_NO_PLATFORMOSVER          0x02
#define MBS_MAKEAPPVERSIONSTRING_FLAG_NO_TOOLSET_NAME           0x04
#define MBS_MAKEAPPVERSIONSTRING_FLAG_NO_TOOLSET_EXACT_NAME     0x08
#define MBS_MAKEAPPVERSIONSTRING_FLAG_NO_HOST_PLATFORM_NAME     0x10
#define MBS_MAKEAPPVERSIONSTRING_FLAG_NO_CONFIGURATION_NAME     0x20

inline
char* makeAppVersionString(char *buf, unsigned bufSize, const char *appName, int flags)
   {
    char *savedPtr = buf;
    unsigned nCopied = 0;

    const char *projectName = appName;
    #ifdef MBS_PROJECT_NAME
    if (!projectName) projectName = MBS_PROJECT_NAME;
    #endif

    {
        char tmpBuf[128];
        char *tmpBufPtr = tmpBuf;
        // 
        _snprintf(tmpBuf, sizeof(tmpBuf)/sizeof(tmpBuf[0]), "'%s'", projectName);
        while(nCopied<(bufSize-1) && *tmpBufPtr) { *buf++ = *tmpBufPtr++; nCopied++; }
    }

    //while(nCopied<(bufSize-1) && *appName) { *buf++ = *appName++; nCopied++; }

    #if defined(MBS_TARGET_PLATFORM_REAL_NAME) || defined(MBS_TARGET_PLATFORM_NAME)
    if (!(flags&MBS_MAKEAPPVERSIONSTRING_FLAG_NO_PLATFORM_NAME))
       {
        #ifdef MBS_TARGET_PLATFORM_REAL_NAME
        static const char * _mbsTargetPlatformName = MBS_TARGET_PLATFORM_REAL_NAME;
        #else
        static const char * _mbsTargetPlatformName = MBS_TARGET_PLATFORM_NAME;
        #endif
        char tmpBuf[128];
        char *tmpBufPtr = tmpBuf;
        _snprintf(tmpBuf, sizeof(tmpBuf)/sizeof(tmpBuf[0]), " for '%s'", _mbsTargetPlatformName);
        while(nCopied<(bufSize-1) && *tmpBufPtr) { *buf++ = *tmpBufPtr++; nCopied++; }
       }
    #endif

    #ifdef PLATFORMOSVER
    if (!(flags&MBS_MAKEAPPVERSIONSTRING_FLAG_NO_PLATFORMOSVER))
       {
        char tmpBuf[128];
        char *tmpBufPtr = tmpBuf;
        _snprintf(tmpBuf, sizeof(tmpBuf)/sizeof(tmpBuf[0]), " kernel '%04X'", (unsigned)PLATFORMOSVER);
        while(nCopied<(bufSize-1) && *tmpBufPtr) { *buf++ = *tmpBufPtr++; nCopied++; }
       }
    #endif

    #ifdef PLATFORMOSVERPATCH
    if (!(flags&MBS_MAKEAPPVERSIONSTRING_FLAG_NO_PLATFORMOSVER))
       {
        char tmpBuf[128];
        char *tmpBufPtr = tmpBuf;
        _snprintf(tmpBuf, sizeof(tmpBuf)/sizeof(tmpBuf[0]), " patchlevel '%04X'", (unsigned)PLATFORMOSVERPATCH);
        while(nCopied<(bufSize-1) && *tmpBufPtr) { *buf++ = *tmpBufPtr++; nCopied++; }
       }
    #endif

    

    #ifdef MBS_TOOLSET_NAME
    if (!(flags&MBS_MAKEAPPVERSIONSTRING_FLAG_NO_TOOLSET_NAME))
       {
        static const char * _mbsToolsetName = MBS_TOOLSET_NAME;
        char tmpBuf[128];
        char *tmpBufPtr = tmpBuf;
        _snprintf(tmpBuf, sizeof(tmpBuf)/sizeof(tmpBuf[0]), ", built with toolset '%s'", _mbsToolsetName);
        while(nCopied<(bufSize-1) && *tmpBufPtr) { *buf++ = *tmpBufPtr++; nCopied++; }
       }
    #endif
    
    #ifdef MBS_TOOLSET_EXACT_NAME
    if (!(flags&(MBS_MAKEAPPVERSIONSTRING_FLAG_NO_TOOLSET_NAME|MBS_MAKEAPPVERSIONSTRING_FLAG_NO_TOOLSET_EXACT_NAME)))
       {
        static const char * _mbsToolsetExactName = MBS_TOOLSET_EXACT_NAME;
        char tmpBuf[128];
        char *tmpBufPtr = tmpBuf;
        _snprintf(tmpBuf, sizeof(tmpBuf)/sizeof(tmpBuf[0]), " ('%s')", _mbsToolsetExactName);
        while(nCopied<(bufSize-1) && *tmpBufPtr) { *buf++ = *tmpBufPtr++; nCopied++; }
       }
    #endif

    #ifdef MBS_HOST_PLATFORM_NAME
    if (!(flags&MBS_MAKEAPPVERSIONSTRING_FLAG_NO_HOST_PLATFORM_NAME))
       {
        static const char * _mbsHostPlatformName = MBS_HOST_PLATFORM_NAME;
        char tmpBuf[128];
        char *tmpBufPtr = tmpBuf;
        _snprintf(tmpBuf, sizeof(tmpBuf)/sizeof(tmpBuf[0]), " on '%s' platform", _mbsHostPlatformName);
        while(nCopied<(bufSize-1) && *tmpBufPtr) { *buf++ = *tmpBufPtr++; nCopied++; }
       }
    #endif

    #ifdef MBS_CONFIGURATION_NAME
    if (!(flags&MBS_MAKEAPPVERSIONSTRING_FLAG_NO_CONFIGURATION_NAME))
       {
        static const char * _mbsConfigurationName = MBS_CONFIGURATION_NAME;
        char tmpBuf[128];
        char *tmpBufPtr = tmpBuf;
        _snprintf(tmpBuf, sizeof(tmpBuf)/sizeof(tmpBuf[0]), ", configuration - '%s'", _mbsConfigurationName);
        while(nCopied<(bufSize-1) && *tmpBufPtr) { *buf++ = *tmpBufPtr++; nCopied++; }
       }
    #endif

    //savedPtr[nCopied] = 0;
    *buf = 0;
    return savedPtr;
   }

#ifdef __mbs_undef_sprintf
    #undef __mbs_undef_sprintf
    #undef _sprintf
    #undef __mbs_undef_sprintf
#endif

#endif /* DISABLE_CONST_CHAR_MBS_STRINGS */


#if !defined(TARGET_TYPE_EXE) && !defined(TARGET_TYPE_DLL) && !defined(TARGET_TYPE_LIB)
    #ifdef _MSC_VER
        #if defined(_LIB)
            #define TARGET_TYPE_LIB
        #elif defined(_USRDLL) || defined(_WINDLL)
            #define TARGET_TYPE_DLL
        #else
            #define TARGET_TYPE_EXE
        #endif
    #else
        #define TARGET_TYPE_EXE /* default - exe */
    #endif
#endif /* TARGET_TYPE_* */

#ifdef WIN32 
    #if defined(TARGET_TYPE_LIB) && !defined(_LIB)
        #define _LIB
    #endif
    #if defined(TARGET_TYPE_DLL) && !defined(_USRDLL)
        #define _USRDLL
    #endif
#endif /* WIN32  */


// PLATFORMOSVER
// _MBS_CC_VER

#ifdef _MSC_VER
    #define _MBS_DEPRECATED_PREFIX         __declspec(deprecated)
    #define _MBS_DEPRECATED_PREFIXT(txt)   __declspec(deprecated(txt))
    #define _MBS_DEPRECATED_SUFFIX 
#elif defined(__GNUC__)
    #define _MBS_DEPRECATED_PREFIX 
    #define _MBS_DEPRECATED_PREFIXT(txt)
    #define _MBS_DEPRECATED_SUFFIX         __attribute__ ((deprecated))
#else
    #define _MBS_DEPRECATED_PREFIX
    #define _MBS_DEPRECATED_PREFIXT(txt)
    #define _MBS_DEPRECATED_SUFFIX
#endif

/*
   Usage:
   _MBS_DEPRECATED_PREFIXT(Function 'deprecated_fn' was deprecated by security problems)
   void deprecated_fn() _MBS_DEPRECATED_SUFFIX;
*/




#endif /* MARTY_MBS_H */

