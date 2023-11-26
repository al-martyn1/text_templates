/*
   Copyright (c) 2007-2009 Alex Martynov, amart at mail.ru
   This code can be used without any limitations.
   This code provided as is, use it on your own risk.
*/

#ifndef MARTY_MHDLDEF_H
#define MARTY_MHDLDEF_H

#ifdef WIN32
    #if !defined(_WINDOWS_)
        #include <windows.h>
    #endif
#else
    //#include <dlfcn.h>
#endif


#ifndef ABSTRACT_MODULE_HANDLE_DEFINED

    #define ABSTRACT_MODULE_HANDLE_DEFINED

    #ifdef WIN32
        typedef HMODULE  ABSTRACT_MODULE_HANDLE;
    #else
        typedef void*    ABSTRACT_MODULE_HANDLE;
    #endif

#endif /* ABSTRACT_MODULE_HANDLE_DEFINED */


#ifndef MODULE_EXPORT
    #if defined(_MSC_VER) || defined(__BORLANDC__) || (defined(__GNUC__) && defined(_WIN32))
        #define MODULE_EXPORT __declspec(dllexport)
        #define MODULE_IMPORT __declspec(dllimport)
        #define MODULE_LOCAL
    #elif defined(__GNUC__)
        #ifdef HAVE_GCCVISIBILITYPATCH
          #define MODULE_EXPORT __attribute__ ((visibility("default")))
          #define MODULE_IMPORT __attribute__ ((visibility("default")))
          //#define MODULE_LOCAL __attribute__ ((visibility("hidden")))
          #define MODULE_LOCAL __attribute__ ((visibility("internal")))
        #else
          #define MODULE_EXPORT
          #define MODULE_IMPORT
          #define MODULE_LOCAL
        #endif
    #endif
#endif


#ifndef MODULE_EXPORT_FUNC
    #define MODULE_EXPORT_FUNC MODULE_EXPORT
#endif

#ifndef MODULE_LOCAL_FUNC
    #define MODULE_LOCAL_FUNC MODULE_LOCAL
#endif

#ifndef EXTERN_C
    #ifdef __cplusplus
        #define EXTERN_C   extern "C"
    #else
        #define EXTERN_C
    #endif
#endif

#ifndef STDCALL
    #ifdef WIN32
        #if defined(_MSC_VER)
            #define STDCALL __stdcall
        #elif defined(__GNUC__)
            #define STDCALL __attribute__((stdcall))
        #else
            #error "Unsupported compiler"
        #endif
    #else /* WIN32 */
        #if defined(__GNUC__)
            #define STDCALL __attribute__((stdcall))
        #else
            #error "Unsupported compiler"
        #endif
    #endif
#endif /* STDCALL */

#ifndef CDECL
    #ifdef WIN32
        #if defined(_MSC_VER)
            #define CDECL __cdecl
        #elif defined(__GNUC__)
            #define CDECL __attribute__((cdecl))
        #else
            #error "Unsupported compiler"
        #endif
    #else /* ! WIN32 */
        #if defined(__GNUC__)
            #define CDECL __attribute__((cdecl))
        #else
            #error "Unsupported compiler"
        #endif
    #endif
#endif /* CDECL */



#endif /* MARTY_MHDLDEF_H */



