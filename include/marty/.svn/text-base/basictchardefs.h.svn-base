/*
   Copyright (c) 2007-2009 Alex Martynov, amart at mail.ru
   This code can be used without any limitations.
   This code provided as is, use it on your own risk.
*/

#ifndef MARTY_BASICTCHARDEFS_H
#define MARTY_BASICTCHARDEFS_H

//-----------------------------------------------------------------------------
#ifdef _UNICODE
    #ifndef UNICODE
        #define UNICODE
    #endif /* UNICODE */
#endif /* _UNICODE */

#ifdef UNICODE
    #ifndef _UNICODE
        #define _UNICODE
    #endif /* _UNICODE */
#endif /* UNICODE */

#ifdef WIN32
    #ifdef _UNICODE
        #ifndef _WIN32_UNICODE
            #define _WIN32_UNICODE
        #endif
    #else
        #ifndef _WIN32_CHAR
            #define _WIN32_CHAR
        #endif
    #endif
#else
    #ifdef _UNICODE
        #ifndef _POSIX_UNICODE
            #define _POSIX_UNICODE
        #endif
    #else
        #ifdef _POSIX_CHAR
            #define _POSIX_CHAR
        #endif
    #endif
#endif

#ifdef WIN32    
    #if !defined(_INC_TCHAR) && !defined(__TCHAR_H)    
        #include <tchar.h>
    #endif
#else
    // 
#endif


#ifndef _TCHAR_DEFINED
    #define _TCHAR_DEFINED
    #ifdef _UNICODE
        typedef wchar_t TCHAR;
    #else
        typedef char    TCHAR;
    #endif /* _UNICODE*/
#endif /* _TCHAR_DEFINED*/


#ifndef _WCHAR_DEFINED
    #define _WCHAR_DEFINED
    typedef wchar_t WCHAR;
#endif /* _WCHAR_DEFINED*/


#ifndef _PWSTR_DEFINED
    #define _PWSTR_DEFINED
    typedef wchar_t* PWSTR;
#endif /* _PWSTR_DEFINED*/


#ifndef _PCWSTR_DEFINED
    #define _PCWSTR_DEFINED
    typedef const wchar_t* PCWSTR;
#endif /* _PCWSTR_DEFINED*/



#ifndef _CHAR_DEFINED
    #define _CHAR_DEFINED
    typedef char   CHAR;
#endif /* _CHAR_DEFINED*/


#ifndef _PSTR_DEFINED
    #define _PSTR_DEFINED
    typedef char *  PSTR;
#endif /* _PSTR_DEFINED*/


#ifndef _PCSTR_DEFINED
    #define _PCSTR_DEFINED
    typedef const char*  PCSTR;
#endif /* _PCSTR_DEFINED*/


#if !defined(_TEXT)
   #ifdef _UNICODE
        #define _TEXT(t) L##t
    #else
        #define _TEXT(t) t
    #endif
#endif /* _TEXT */

#if !defined(_T)
    #ifdef _UNICODE
        #define _T(t) L##t
    #else
        #define _T(t) t
    #endif
#endif /* _T */


#ifdef _UNICODE
    typedef PWSTR PTSTR;
    typedef PCWSTR PCTSTR;
#else
    typedef PSTR PTSTR;
    typedef PCSTR PCTSTR;
#endif




#endif /* MARTY_BASICTCHARDEFS_H */

