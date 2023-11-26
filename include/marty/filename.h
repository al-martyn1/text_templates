/*
   Copyright (c) 2007-2009 Alex Martynov, amart at mail.ru
   This code can be used without any limitations.
   This code provided as is, use it on your own risk.
*/

#ifndef MARTY_FILENAME_H
#define MARTY_FILENAME_H

// about locale names
// Language Strings ms-help://MS.VSCC.v80/MS.MSDN.v80/MS.VisualStudio.v80.en/dv_vccrt/html/bbee63b1-af0b-4e44-9eaf-dd3e265c05fd.htm
// locale  "lang[_country_region[.code_page]]"
//            | ".code_page"
//            | ""
//            | NULL
// country/region ms-help://MS.VSCC.v80/MS.MSDN.v80/MS.VisualStudio.v80.en/dv_vccrt/html/5baf0ccf-0d9b-40dc-83bd-323705287930.htm


// Copyright (c) 2004-2007 Alex Martynov, amart at mail dot ru

// define FILENAME_IN_MARTY_NAMESPACE for use ::marty::filename::* insted of ::filename::*
// also you can define MARTY_NAMESPACE macro, which automaticaly define FILENAME_IN_MARTY_NAMESPACE
// define FILENAME_OLD_NAMES for use old short names
// define FILENAME_USE_TEST for use included test cases 
// define FILENAME_USING_TSTRING for use filename::tstring in global namespace
// define FILENAME_PRINT_ALL_TESTS for output all tests results, not only failed
// define FILENAME_COMPARISON_ALWAYS_USE_LOCALE if you want unified unterface for 
   // equal, less, lessEqual, greater, greaterEqual, compareTo functions


/*
#ifdef USE_MARTY_NAMESPACE
    #ifndef MARTY_NAMESPACE
        #define MARTY_NAMESPACE
    #endif
#endif
*/

#ifndef MARTY_BASICTCHARDEFS_H
    #include <marty/basictchardefs.h>
#endif


#ifdef USE_MARTY_NAMESPACE
    #ifndef FILENAME_IN_MARTY_NAMESPACE
        #define FILENAME_IN_MARTY_NAMESPACE
    #endif
#endif

#ifdef FILENAME_IN_MARTY_NAMESPACE
    #define USING_MARTY_FILENAME using namespace ::marty::filename
#else
    #define USING_MARTY_FILENAME using namespace ::filename
#endif



//-----------------------------------------------------------------------------
#ifdef __BORLANDC__
    #define __USELOCALES__
#endif

//-----------------------------------------------------------------------------
/* std/stl headers */

#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

#if !defined(_MEMORY_) && !defined(_STLP_MEMORY) && !defined(__STD_MEMORY__) && !defined(_CPP_MEMORY) && !defined(_GLIBCXX_MEMORY)
    #include <memory>
#endif

#if !defined(_VECTOR_) && !defined(_STLP_VECTOR) && !defined(__STD_VECTOR__) && !defined(_CPP_VECTOR) && !defined(_GLIBCXX_VECTOR)
    #include <vector>
#endif

#if !defined(_ALGORITHM_) && !defined(_STLP_ALGORITHM) && !defined(__STD_ALGORITHM) && !defined(_CPP_ALGORITHM) && !defined(_GLIBCXX_ALGORITHM)
    #include <algorithm>
#endif

#if !defined(_UTILITY_) && !defined(_STLP_UTILITY) && !defined(__STD_UTILITY__) && !defined(_CPP_UTILITY) && !defined(_GLIBCXX_UTILITY)
    #include <utility>
#endif

#if !defined(_LOCALE_) && !defined(_STLP_LOCALE) && !defined(__STD_LOCALE__) && !defined(_CPP_LOCALE) && !defined(_GLIBCXX_LOCALE)
    #include <locale>
#endif

#ifdef FILENAME_USE_TEST
    #if !defined(_IOSTREAM_) && !defined(_STLP_IOSTREAM) && !defined(__STD_IOSTREAM__) && !defined(_CPP_IOSTREAM) && !defined(_GLIBCXX_IOSTREAM)
        #include <iostream>
    #endif

    #ifdef _UNICODE
        #define tcout ::std::wcout
    #else
        #define tcout ::std::cout
    #endif /* _UNICODE */
#endif /* FILENAME_USE_TEST */

#ifdef _DEBUG
    #if !defined(_IOSTREAM_) && !defined(_STLP_IOSTREAM) && !defined(__STD_IOSTREAM__) && !defined(_CPP_IOSTREAM) && !defined(_GLIBCXX_IOSTREAM)
        #include <iostream>
    #endif
#endif

//-----------------------------------------------------------------------------
/* platform headers */

#ifdef WIN32
    #if !defined(_WINDOWS_)
        #include <windows.h>
    #endif

    #ifndef _WINNLS_
        #include <winnls.h>
    #endif
#endif


#ifdef WIN32
    #if !defined(_INC_MALLOC) && !defined(_MALLOC_H_) && !defined(_MALLOC_H)
        #include <malloc.h>
    #endif
#else /* __GNUC__ */
    /* #include <alloca.h>
     * #ifndef _alloca
     *     #define _alloca  alloca
     * #endif
     */
#endif


#include <marty/caseconv.h>



#ifndef MARTY_STRING
    #define MARTY_STRING(TC) ::std::basic_string<TC>
#endif

#ifndef MARTY_STRING3
    #define MARTY_STRING3(CharType, Traits, Allocator) ::std::basic_string<CharType, Traits, Allocator>
#endif
/*
#ifndef MARTY_STRING3
    #define 
#endif

MARTY_STRING3(CharType, Traits, Allocator)
*/

#ifndef MARTY_TCSTRING
    #define MARTY_TCSTRING   MARTY_STRING(TC)
#endif

#ifdef WIN32
    #define FILENAMES_ARE_CASE_INSENSITIVE
#endif

//-----------------------------------------------------------------------------
#ifdef USE_MARTY_NAMESPACE
    #ifndef FILENAME_IN_MARTY_NAMESPACE
        #define FILENAME_IN_MARTY_NAMESPACE
    #endif
#endif


#ifdef FILENAME_IN_MARTY_NAMESPACE
namespace marty {
#endif
namespace filename
{
#ifndef __FILENAME_TSTRING_DEFINED__
#define __FILENAME_TSTRING_DEFINED__
typedef ::std::basic_string< TCHAR, 
                             ::std::char_traits<TCHAR>, 
                             ::std::allocator<TCHAR> >  tstring;
#endif 
}; // namespace filename
#ifdef FILENAME_IN_MARTY_NAMESPACE
}; // namespace marty
#endif


#ifdef FILENAME_IN_MARTY_NAMESPACE
    #ifndef MARTY_NS
        #define BEGIN_MARTY_NS          namespace marty {
        #define END_MARTY_NS            };
        #define MARTY_NS                ::marty::
     #endif
    #define MARTY_FILENAME_NS       MARTY_NS filename::
    #define MARTY_FILENAME          ::marty::filename
#else
    #ifndef MARTY_NS
        #define BEGIN_MARTY_NS 
        #define END_MARTY_NS 
        #define MARTY_NS                ::
     #endif
    #define MARTY_FILENAME_NS       MARTY_NS filename::
    #define MARTY_FILENAME          ::filename
#endif

#define BEGIN_MARTY_FILENAME_NS    BEGIN_MARTY_NS namespace filename {
#define END_MARTY_FILENAME_NS      END_MARTY_NS   };



#ifdef FILENAME_USING_TSTRING
    #ifdef FILENAME_IN_MARTY_NAMESPACE
    using ::marty::filename::tstring;
    #else
    using ::filename::tstring;
    #endif
#endif



//-----------------------------------------------------------------------------
#if defined(WIN32)
    #define PATH_SEPARATORS                _T("\\/")
    #define PATH_PREFFERED_SEPARATOR_STR   _T("\\")
    #define PATH_PREFFERED_SEPARATOR       _T('\\')
    #define PATH_EXT_SEPARATOR_STR         _T(".")
    #define PATH_EXT_SEPARATOR             _T('.')

    #define C_PATH_SEPARATORS                "\\/"
    #define C_PATH_PREFFERED_SEPARATOR_STR   "\\"
    #define C_PATH_PREFFERED_SEPARATOR       '\\'
    #define C_PATH_EXT_SEPARATOR_STR         "."
    #define C_PATH_EXT_SEPARATOR             '.'

    #define W_PATH_SEPARATORS                L"\\/"
    #define W_PATH_PREFFERED_SEPARATOR_STR   L"\\"
    #define W_PATH_PREFFERED_SEPARATOR       L'\\'
    #define W_PATH_EXT_SEPARATOR_STR         L"."
    #define W_PATH_EXT_SEPARATOR             L'.'

#else
    #define PATH_SEPARATORS                _T("/")
    #define PATH_PREFFERED_SEPARATOR_STR   _T("/")
    #define PATH_PREFFERED_SEPARATOR       _T('/')
    #define PATH_EXT_SEPARATOR_STR         _T(".")
    #define PATH_EXT_SEPARATOR             _T('.')

    #define C_PATH_SEPARATORS                "/"
    #define C_PATH_PREFFERED_SEPARATOR_STR   "/"
    #define C_PATH_PREFFERED_SEPARATOR       '/'
    #define C_PATH_EXT_SEPARATOR_STR         "."
    #define C_PATH_EXT_SEPARATOR             '.'
              
    #define W_PATH_SEPARATORS                L"/"
    #define W_PATH_PREFFERED_SEPARATOR_STR   L"/"
    #define W_PATH_PREFFERED_SEPARATOR       L'/'
    #define W_PATH_EXT_SEPARATOR_STR         L"."
    #define W_PATH_EXT_SEPARATOR             L'.'
#endif

#define PATH_SEPARATORS_STRICT             _T("\\/")
#define PATH_PREFFERED_SEPARATOR_STRICT    _T('/')

#define C_PATH_SEPARATORS_STRICT           "\\/"
#define C_PATH_PREFFERED_SEPARATOR_STRICT  '/'

#define W_PATH_SEPARATORS_STRICT           L"\\/"
#define W_PATH_PREFFERED_SEPARATOR_STRICT  L'/'

//-----------------------------------------------------------------------------
/*
#ifndef ISNPOS
    #define ISNPOS(pos)                          (pos==tstring::npos)
#endif
*/
/*
#define NSFN_ISALPHAAUX(ch, alphaMin, alphaMax)   ((ch)>=(alphaMin) && (ch)<=(alphaMax))
#define NSFN_ISUPPERALPHA(ch)                     NSFN_ISALPHAAUX(ch, _T('A'), _T('Z'))
#define NSFN_ISLOWERALPHA(ch)                     NSFN_ISALPHAAUX(ch, _T('a'), _T('z'))
#define NSFN_ISALPHA(ch)                          (NSFN_ISUPPERALPHA(ch) || NSFN_ISLOWERALPHA(ch))
*/

template <typename CharType>
bool nsfnIsAlphaAux(CharType ch, CharType chMin, CharType chMax)
   {
    //throw ::std::exception("nsfnIsAlphaAux must be specialized");
    //return false;
    return (ch>=chMin && ch<=chMax);
   }

template <typename CharType>
bool nsfnIsUpperAlpha(CharType ch)
   { throw ::std::exception("nsfnIsUpperAlpha must be specialized"); return false; }

template <typename CharType>
bool nsfnIsLowerAlpha(CharType ch)
   { throw ::std::exception("nsfnIsLowerAlpha must be specialized"); return false; }


template <>
inline
bool nsfnIsUpperAlpha<char>(char ch)
   { return nsfnIsAlphaAux(ch, 'A', 'Z'); }

template <>
inline
bool nsfnIsUpperAlpha<wchar_t>(wchar_t ch)
   { return nsfnIsAlphaAux(ch, L'A', L'Z'); }

template <>
inline
bool nsfnIsLowerAlpha<char>(char ch)
   { return nsfnIsAlphaAux(ch, 'a', 'z'); }

template <>
inline
bool nsfnIsLowerAlpha<wchar_t>(wchar_t ch)
   { return nsfnIsAlphaAux(ch, L'a', L'z'); }

template <typename CharType>
bool nsfnIsAlpha(CharType ch)
   { return nsfnIsLowerAlpha(ch) || nsfnIsUpperAlpha(ch); }


#define NSFN_ISALPHA(ch)   nsfnIsAlpha(ch)


//-----------------------------------------------------------------------------

#ifdef FILENAME_IN_MARTY_NAMESPACE
namespace marty {
#endif

namespace filename
{
namespace utils
{

// sync C++ locale with system
#ifdef WIN32
namespace localeAux
{

inline
std::wstring strToWide(const std::wstring &s)
   {
    return s;
   }

//-----------------------------------------------------------------------------
inline
std::wstring strToWide(const std::string &str, int codePage = CP_ACP)
   {
    DWORD flags = MB_PRECOMPOSED;
    switch(codePage)
       {
        case 50220: case 50221: case 50222: case 50225: case 50227: case 50229: case 52936: case 54936:
        case 57002: case 57003: case 57004: case 57005: case 57006: case 57007: case 57008: case 57009:
        case 57010: case 57011: case 65000: case 65001: case 42   :
            flags = 0; break;
       };

    int res = ::MultiByteToWideChar( codePage,
                                     flags,
                                     str.c_str(),
                                     //str.size(),
                                     -1,
                                     0, 0);
    wchar_t *pRes = (wchar_t*)_alloca((size_t)(res+1)*sizeof(wchar_t));
    if (::MultiByteToWideChar( codePage,
                         flags,
                         str.c_str(),
                         //str.size(),
                         -1,
                         pRes, res))
       {
        pRes[res] = 0;
        return std::wstring(pRes); 
       }
    return std::wstring();
    //pRes[res] = 0;
   }

//-----------------------------------------------------------------------------
inline
std::string strToAnsi(const std::string &s)
   {
    return s;
   }

//-----------------------------------------------------------------------------
inline
std::string strToAnsi(const std::wstring &str, int codePage = CP_ACP)
   {
    int res = ::WideCharToMultiByte( codePage,
                                     WC_COMPOSITECHECK,
                                     str.c_str(),
                                     //str.size(),
                                     -1,
                                     0, 0,
                                     0, 0
                                     );
    char *pRes = (char*)_alloca((size_t)(res+1)*sizeof(char));
    if (::WideCharToMultiByte( codePage,
                           WC_COMPOSITECHECK,
                           str.c_str(),
                           //str.size(),
                           -1,
                           pRes, res,
                           0, 0
                           ))
       {
        pRes[res] = 0;
        return std::string(pRes); 
       }
    //pRes[res] = 0;
    return std::string();
   }


//-----------------------------------------------------------------------------
inline
tstring getLocaleInfo(LCID lcid, LCTYPE lctype)
   {
    TCHAR buf[128];
    buf[ ::GetLocaleInfo(lcid, lctype, buf, sizeof(buf)/sizeof(buf[0])-1) ] = 0;
    return tstring(buf);
   }

//-----------------------------------------------------------------------------
inline 
tstring getLocaleCrtNameT(LCID lcid, bool getOemLocale)
   {
    tstring res = getLocaleInfo(lcid, LOCALE_SENGLANGUAGE);
    
    if (res.empty()) return tstring(_T("C"));

    tstring country = getLocaleInfo(lcid, LOCALE_SENGCOUNTRY);
    if (!country.empty())
       {
        res.append(_T("_"));
        res.append(country);
       }

    tstring cpName = getLocaleInfo(lcid, getOemLocale ? LOCALE_IDEFAULTCODEPAGE : LOCALE_IDEFAULTANSICODEPAGE);
    if (!cpName.empty())
       {
        res.append(_T("."));
        res.append(cpName);
       }

    return res;
   }

//-----------------------------------------------------------------------------
inline std::string getLocaleCrtNameA(LCID lcid, bool getOemLocale)
   { return strToAnsi(getLocaleCrtNameT(lcid, getOemLocale)); }

//-----------------------------------------------------------------------------
inline std::wstring getLocaleCrtNameW(LCID lcid, bool getOemLocale)
   { return strToWide(getLocaleCrtNameT(lcid, getOemLocale)); }


#if defined(UNICODE) || defined(_UNICODE)
#else
#endif


}; // namespace localeAux


#endif
//-----------------------------------------------------------------------------
inline
::std::locale makeCurrentLocale(bool getOemLocale = false)
   {
    #ifdef WIN32
    //return ::std::locale(localeAux::getLocaleCrtName(GetThreadLocale(), getOemLocale).c_str());
    //return ::std::locale(localeAux::getLocaleCrtName(LOCALE_USER_DEFAULT, getOemLocale).c_str());
        // GCC + libstdc++? Так и должно быть - там ничего на эту тему не реализвано в дефолтовых сборках. 
        // Сам на это наступил. Лучший вариант - STLPort или setlocale + toupper/strupr

        ::std::string localeName = localeAux::getLocaleCrtNameA(LOCALE_SYSTEM_DEFAULT, getOemLocale);
        //std::cout<<"localeName: "<<localeName<<"\n";
        try{
            return ::std::locale(localeName.c_str());
           }
        catch(const std::exception  /* &e */ )
           {
            //std::cout<<"Error: "<<e.what()<<", locale name: "<<localeName<<"\n";
           }
        catch(...)
           {
            //std::cout<<"Error: unknown error, locale name: "<<localeName<<"\n";
           }
        return ::std::locale("C");
        //return ::std::locale(localeAux::getLocaleCrtNameA(LOCALE_SYSTEM_DEFAULT, getOemLocale).c_str());
    #else
    //#error "Keeping in sync system and C++ locales are not supported on this target or not needed"
        #ifdef __GNUC__
        return ::std::locale();
        #else
        return ::std::locale::global();
        #endif
    #endif
   }

//-----------------------------------------------------------------------------
inline
::std::locale makeSystemLocale(bool getOemLocale = false)
   {
    #ifdef WIN32
    //return ::std::locale(localeAux::getLocaleCrtName(GetThreadLocale(), getOemLocale).c_str());
    //return ::std::locale(localeAux::getLocaleCrtName(LOCALE_USER_DEFAULT, getOemLocale).c_str());

        ::std::string localeName = localeAux::getLocaleCrtNameA(LOCALE_SYSTEM_DEFAULT, getOemLocale);
        try{
            return ::std::locale(localeName.c_str());
           }
        catch(const std::exception  /* &e */ )
           {
            //std::cout<<"Error: "<<e.what()<<", locale name: "<<localeName<<"\n";
           }
        catch(...)
           {
            //std::cout<<"Error: unknown error, locale name: "<<localeName<<"\n";
           }
        return ::std::locale("C");
        //return ::std::locale(localeAux::getLocaleCrtNameA(LOCALE_SYSTEM_DEFAULT, getOemLocale).c_str());
    //return ::std::locale(localeAux::getLocaleCrtNameA(LOCALE_SYSTEM_DEFAULT, getOemLocale).c_str());
    #else
    //#error "Keeping in sync system and C++ locales are not supported on this target or not needed"
        #ifdef __GNUC__
        return ::std::locale();
        #else
        return ::std::locale::global();
        #endif
    #endif
   }

//-----------------------------------------------------------------------------
inline
::std::locale makeThreadLocale(bool getOemLocale = false)
   {
    #ifdef WIN32
        ::std::string localeName = localeAux::getLocaleCrtNameA(GetThreadLocale(), getOemLocale);
        try{
            return ::std::locale(localeName.c_str());
           }
        catch(const std::exception  /* &e */ )
           {
            //std::cout<<"Error: "<<e.what()<<", locale name: "<<localeName<<"\n";
           }
        catch(...)
           {
            //std::cout<<"Error: unknown error, locale name: "<<localeName<<"\n";
           }
        return ::std::locale("C");
        //return ::std::locale(localeAux::getLocaleCrtNameA(GetThreadLocale(), getOemLocale).c_str());
    //return ::std::locale(localeAux::getLocaleCrtNameA(GetThreadLocale(), getOemLocale).c_str());
    #else
        #ifdef __GNUC__
        return ::std::locale();
        #else
        return ::std::locale::global();
        #endif
    #endif
   }

//-----------------------------------------------------------------------------
inline
::std::locale makeUserLocale(bool getOemLocale = false)
   {
    #ifdef WIN32
        ::std::string localeName = localeAux::getLocaleCrtNameA(LOCALE_USER_DEFAULT, getOemLocale);
        try{
            return ::std::locale(localeName.c_str());
           }
        catch(const std::exception  /* &e */ )
           {
            //std::cout<<"Error: "<<e.what()<<", locale name: "<<localeName<<"\n";
           }
        catch(...)
           {
            //std::cout<<"Error: unknown error, locale name: "<<localeName<<"\n";
           }
        return ::std::locale("C");
        //return ::std::locale(localeAux::getLocaleCrtNameA(LOCALE_USER_DEFAULT, getOemLocale).c_str());
    //return ::std::locale(localeAux::getLocaleCrtNameA(LOCALE_USER_DEFAULT, getOemLocale).c_str());
    #else
        #ifdef __GNUC__
        return ::std::locale();
        #else
        return ::std::locale::global();
        #endif
    #endif
   }

//-----------------------------------------------------------------------------


typedef ::marty::util::uppercaseChar uppercaseChar;
typedef ::marty::util::lowercaseChar lowercaseChar;
typedef ::marty::util::keepcaseChar  keepcaseChar;

//using ::marty::util::upperCase;
//using ::marty::util::lowerCase;




template< typename CharType, typename Traits, typename Allocator >
::std::basic_string<CharType, Traits, Allocator>
upperCase( const ::std::basic_string<CharType, Traits, Allocator> &str
         , const ::std::locale &loc )
   {
    return ::marty::util::upperCase( str, loc );
   }

template< typename CharType, typename Traits, typename Allocator >
::std::basic_string<CharType, Traits, Allocator>
upperCase( const ::std::basic_string<CharType, Traits, Allocator> &str )
   {
    return ::marty::util::upperCase( str, MARTY_FILENAME::utils::makeCurrentLocale() );
   }

template< typename CharType, typename Traits, typename Allocator >
::std::basic_string<CharType, Traits, Allocator>
upperCaseCopy( const ::std::basic_string<CharType, Traits, Allocator> &str
         , const ::std::locale &loc )
   {
    return ::marty::util::upperCase( str, loc );
   }

template< typename CharType, typename Traits, typename Allocator >
::std::basic_string<CharType, Traits, Allocator>
upperCaseCopy( const ::std::basic_string<CharType, Traits, Allocator> &str )
   {
    return ::marty::util::upperCase( str, MARTY_FILENAME::utils::makeCurrentLocale() );
   }

template< typename CharType, typename Traits, typename Allocator >
void
upperCaseInplace( ::std::basic_string<CharType, Traits, Allocator> &str
         , const ::std::locale &loc )
   {
    str = ::marty::util::upperCase( str, loc );
   }

template< typename CharType, typename Traits, typename Allocator >
void
upperCaseInplace( ::std::basic_string<CharType, Traits, Allocator> &str )
   {
    str = ::marty::util::upperCase( str, MARTY_FILENAME::utils::makeCurrentLocale() );
   }




template< typename CharType, typename Traits, typename Allocator > 
::std::basic_string<CharType, Traits, Allocator>
lowerCase( const ::std::basic_string<CharType, Traits, Allocator> &str
         , const std::locale &loc )
   {
    return ::marty::util::lowerCase( str, loc );
   }

template< typename CharType, typename Traits, typename Allocator > 
::std::basic_string<CharType, Traits, Allocator>
lowerCase( const ::std::basic_string<CharType, Traits, Allocator> &str )
   {
    return ::marty::util::lowerCase( str, MARTY_FILENAME::utils::makeCurrentLocale() );
   }

template< typename CharType, typename Traits, typename Allocator > 
::std::basic_string<CharType, Traits, Allocator>
lowerCaseCopy( const ::std::basic_string<CharType, Traits, Allocator> &str
         , const std::locale &loc )
   {
    return ::marty::util::lowerCase( str, loc );
   }

template< typename CharType, typename Traits, typename Allocator > 
::std::basic_string<CharType, Traits, Allocator>
lowerCaseCopy( const ::std::basic_string<CharType, Traits, Allocator> &str )
   {
    return ::marty::util::lowerCase( str, MARTY_FILENAME::utils::makeCurrentLocale() );
   }

template< typename CharType, typename Traits, typename Allocator > 
void
lowerCaseCopy( ::std::basic_string<CharType, Traits, Allocator> &str
         , const std::locale &loc )
   {
    str = ::marty::util::lowerCase( str, loc );
   }

template< typename CharType, typename Traits, typename Allocator > 
void
lowerCaseCopy( ::std::basic_string<CharType, Traits, Allocator> &str )
   {
    str = ::marty::util::lowerCase( str, MARTY_FILENAME::utils::makeCurrentLocale() );
   }









}; // namespace utils

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifdef FILENAME_USE_TEST
namespace test
{

//-----------------------------------------------------------------------------
// used for testing false and true results
struct genericTestCaseResStruct
{
    TCHAR *inputVal;
    TCHAR *checkVal;
    bool   res;
};

//-----------------------------------------------------------------------------
// used for truth cases
struct genericTestCaseStruct
{
    TCHAR *inputVal;
    TCHAR *checkVal;
};

typedef genericTestCaseStruct genericTestCaseStruct1;
typedef tstring (*strfn1_t)( const tstring & );

struct genericTestCaseStruct2
{
    TCHAR *inputVal1;
    TCHAR *inputVal2;
    TCHAR *checkVal;
};

typedef tstring (*strfn2_t)( const tstring &, const tstring & );

struct genericTestCaseStruct3
{
    TCHAR *inputVal1;
    TCHAR *inputVal2;
    TCHAR *inputVal3;
    TCHAR *checkVal;
};

typedef tstring (*strfn3_t)( const tstring &, const tstring &, const tstring & );


//-----------------------------------------------------------------------------
template<typename TestFn>
int genericTestSuite(
                     const genericTestCaseResStruct *pTestData,
                     int *pTotalTests, /* pointer to var that incremented with total num of tests */
                     const TCHAR* testName,
                     TestFn fn
                    )
   {
    int failedTests = 0;
    int i = 0;
    for(; pTestData && pTestData->inputVal; ++i, ++pTestData)
       {
        if (!fn(tstring(pTestData->inputVal), tstring(pTestData->checkVal), pTestData->res))
           {
            tcout<<_T("Test ")<<testName<<_T(":")<<i+1<<_T(" failed\n");
            ++failedTests;
           }
        #ifdef FILENAME_PRINT_ALL_TESTS
        else
           {
            tcout<<_T("Test ")<<testName<<_T(":")<<i+1<<_T(" passed\n");
           }
        #endif
       }

    if (pTotalTests)
       *pTotalTests += i;

    tcout<<testName<<_T(": total tests - ")<<i<<_T(", failed tests - ")<<failedTests<<_T("\n---\n");

    return failedTests;
   }

//-----------------------------------------------------------------------------
//template<typename TestFn>
int genericTestSuite(
                     const genericTestCaseStruct *pTestData,
                     int *pTotalTests, /* pointer to var that incremented with total num of tests */
                     const TCHAR* testName,
                     strfn1_t fn
                    )
   {
    int failedTests = 0;
    int i = 0;
    for(; pTestData && pTestData->inputVal; ++i, ++pTestData)
       {
        tstring strRes = fn(tstring(pTestData->inputVal));
        if (strRes != tstring(pTestData->checkVal))
           {
            tcout<<_T("Test ")<<testName<<_T(":")<<i+1<<_T(" failed, result: ")<<strRes<<_T(", awaited res: ")<<pTestData->checkVal<<_T("\n");
            ++failedTests;
           }
        #ifdef FILENAME_PRINT_ALL_TESTS
        else
           {
            tcout<<_T("Test ")<<testName<<_T(":")<<i+1<<_T(" passed\n");
           }
        #endif
       }

    if (pTotalTests)
       *pTotalTests += i;

    tcout<<testName<<_T(": total tests - ")<<i<<_T(", failed tests - ")<<failedTests<<_T("\n---\n");

    return failedTests;
   }

//-----------------------------------------------------------------------------
//template<typename TestFn>
int genericTestSuite(
                     const genericTestCaseStruct2 *pTestData,
                     int *pTotalTests, /* pointer to var that incremented with total num of tests */
                     const TCHAR* testName,
                     strfn2_t fn
                    )
   {
    int failedTests = 0;
    int i = 0;
    for(; pTestData && pTestData->inputVal1; ++i, ++pTestData)
       {
        tstring strRes = fn(tstring(pTestData->inputVal1), tstring(pTestData->inputVal2));
        if (strRes != tstring(pTestData->checkVal))
           {
            tcout<<_T("Test ")<<testName<<_T(":")<<i+1<<_T(" failed, result: ")<<strRes<<_T(", awaited res: ")<<pTestData->checkVal<<_T("\n");
            ++failedTests;
           }
        #ifdef FILENAME_PRINT_ALL_TESTS
        else
           {
            tcout<<_T("Test ")<<testName<<_T(":")<<i+1<<_T(" passed\n");
           }
        #endif
       }

    if (pTotalTests)
       *pTotalTests += i;

    tcout<<testName<<_T(": total tests - ")<<i<<_T(", failed tests - ")<<failedTests<<_T("\n---\n");

    return failedTests;
   }

//-----------------------------------------------------------------------------
//template<typename TestFn>
int genericTestSuite(
                     const genericTestCaseStruct3 *pTestData,
                     int *pTotalTests, /* pointer to var that incremented with total num of tests */
                     const TCHAR* testName,
                     strfn3_t fn
                    )
   {
    int failedTests = 0;
    int i = 0;
    for(; pTestData && pTestData->inputVal1; ++i, ++pTestData)
       {
        tstring strRes = fn(tstring(pTestData->inputVal1), tstring(pTestData->inputVal2), tstring(pTestData->inputVal3));
        if (strRes != tstring(pTestData->checkVal))
           {
            tcout<<_T("Test ")<<testName<<_T(":")<<i+1<<_T(" failed, result: ")<<strRes<<_T(", awaited res: ")<<pTestData->checkVal<<_T("\n");
            ++failedTests;
           }
        #ifdef FILENAME_PRINT_ALL_TESTS
        else
           {
            tcout<<_T("Test ")<<testName<<_T(":")<<i+1<<_T(" passed\n");
           }
        #endif
       }

    if (pTotalTests)
       *pTotalTests += i;

    tcout<<testName<<_T(": total tests - ")<<i<<_T(", failed tests - ")<<failedTests<<_T("\n---\n");

    return failedTests;
   }

//-----------------------------------------------------------------------------
}; // namespace test


#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/*
template< typename CharType
        , typename Traits    = ::std::char_traits<CharType>
        , typename Allocator = ::std::allocator<CharType> >
*/


typedef tstring::size_type pos_type_t;
typedef ::std::pair<pos_type_t, pos_type_t> pos_pair_t;

typedef tstring::iterator               iter_t;
typedef tstring::const_iterator         const_iter_t;

typedef ::std::pair<iter_t, iter_t>               iter_pair_t;
typedef ::std::pair<const_iter_t, const_iter_t>   const_iter_pair_t;

//-----------------------------------------------------------------------------
template<typename TC>
struct isPathSeparator
{
    private:
        const TC *sepList;
    public:
        isPathSeparator(const TC *s) : sepList(s) {}
        bool operator()(TC c)
           {
            for(const TC *p = sepList; *p; ++p) if (*p==c) return true;
            return false;
           }
};

//-----------------------------------------------------------------------------
template<typename TC>
struct isExtSeparator
{
    private:
        const TC *sepList;
    public:
        isExtSeparator(const TC *s) : sepList(s) {}
        bool operator()(TC c)
           {
            for(const TC *p = sepList; *p; ++p) if (*p==c) return true;
            return false;
           }
};

//-----------------------------------------------------------------------------
/*
struct defaultIsPathSeparator : public isPathSeparator
{
        defaultIsPathSeparator() : isPathSeparator(PATH_SEPARATORS) {} 
        // bug here - tsring is temporary object, and we store reference to it
        defaultIsPathSeparator(const defaultIsPathSeparator &s) : isPathSeparator(s) {}

};

//-----------------------------------------------------------------------------
struct defaultIsExtSeparator : public isExtSeparator
{
        defaultIsExtSeparator() : isExtSeparator(PATH_EXT_SEPARATOR_STR) {}
        defaultIsExtSeparator(const defaultIsExtSeparator &s) : isExtSeparator(s) {}
};
*/

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template <typename Iter, typename pathPred, typename extPred>
void splitFilename(::std::pair<Iter, Iter> pair, // fullName
                   ::std::pair<Iter, Iter> &pathPair,
                   ::std::pair<Iter, Iter> &namePair,
                   ::std::pair<Iter, Iter> &extPair,
                   pathPred pPath, extPred pExt)
   {
    Iter it               = pair.first;
    Iter lastFoundPathSep = pair.first;
    Iter foundExtSep      = pair.first;

    for(; it!=pair.second; ++it)
       {
        if (pPath(*it))
           { // found path separator
            lastFoundPathSep = it; ++lastFoundPathSep;
            foundExtSep      = pair.first; // бЎа®бЁ«Ё гЄ § вҐ«м ­  ­ з «® а биЁаҐ­Ёп
           }
        else if (pExt(*it) && foundExtSep==pair.first)
           {
            foundExtSep = it;
           }
       }

    pathPair.first  = pair.first;
    pathPair.second = lastFoundPathSep;

    namePair.first = pathPair.second;
    //if (namePair.first!=pair.first) ++namePair.first; // skip path separator

    if (foundExtSep == pair.first)
       { // ext not found
        extPair.first = extPair.second = pair.second; //pair.first;
        namePair.second = pair.second;
       }
    else
       {
        extPair.first = foundExtSep;
        extPair.second = pair.second;
        namePair.second = foundExtSep;
       }
   }

//-----------------------------------------------------------------------------

template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
void splitFilename(const ::std::basic_string<CharType, Traits, Allocator> &fullName,
                   ::std::basic_string<CharType, Traits, Allocator> &path,
                   ::std::basic_string<CharType, Traits, Allocator> &name,
                   ::std::basic_string<CharType, Traits, Allocator> &ext,
                   const CharType* pathSeparators/* = PATH_SEPARATORS*/,
                   const CharType* extSeparators/* = PATH_EXT_SEPARATOR_STR*/)
   {
    typedef typename ::std::basic_string<CharType, Traits, Allocator>::const_iterator const_iterator;
    ::std::pair<const_iterator, const_iterator> pathPair, namePair, extPair;
    splitFilename(::std::make_pair(fullName.begin(), fullName.end()),
                  pathPair, namePair, extPair,
                  isPathSeparator<CharType>(pathSeparators),
                  isExtSeparator<CharType>(extSeparators));

    #ifdef __BORLANDC__
    // call assign causes 
    // Unresolved external basic_string::::replace<const char *>
    //                     (char *, char *, const char *, const char *)
    ::std::basic_string<CharType, Traits, Allocator>(pathPair.first, pathPair.second).swap(path);
    ::std::basic_string<CharType, Traits, Allocator>(namePair.first, namePair.second).swap(name);
    ::std::basic_string<CharType, Traits, Allocator>(extPair.first, extPair.second).swap(ext);
    #else
    path.assign(pathPair.first, pathPair.second);
    name.assign(namePair.first, namePair.second);
    ext .assign(extPair.first, extPair.second);
    #endif
   }
/*
template<typename TC>
void splitFilename(const MARTY_TCSTRING &fullName,
                   MARTY_TCSTRING &path,
                   MARTY_TCSTRING &name,
                   MARTY_TCSTRING &ext,
                   const TC* pathSeparators, // = PATH_SEPARATORS
                   const TC* extSeparators // = PATH_EXT_SEPARATOR_STR
                  )
   {
    ::std::pair<typename MARTY_TCSTRING::const_iterator, typename MARTY_TCSTRING::const_iterator> pathPair, namePair, extPair;
    MARTY_NS filename::splitFilename<TC>(::std::make_pair(fullName.begin(), fullName.end()),
                              pathPair, namePair, extPair,
                              isPathSeparator<TC>(pathSeparators),
                              isExtSeparator<TC>(extSeparators));

    #ifdef __BORLANDC__
    // call assign causes 
    // Unresolved external basic_string::::replace<const char *>
    //                     (char *, char *, const char *, const char *)
    MARTY_TCSTRING(pathPair.first, pathPair.second).swap(path);
    MARTY_TCSTRING(namePair.first, namePair.second).swap(name);
    MARTY_TCSTRING(extPair.first, extPair.second).swap(ext);
    #else
    path.assign(pathPair.first, pathPair.second);
    name.assign(namePair.first, namePair.second);
    ext .assign(extPair.first, extPair.second);
    #endif
   }
*/
//-----------------------------------------------------------------------------
template <typename Iter, typename pathPred, typename TC>
void changePathChars(::std::pair<Iter, Iter> name, pathPred pred, TC pathSep)
   {
    for(; name.first!=name.second; ++name.first)
       {
        if (pred(*name.first)) *name.first = pathSep;
       }
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
void changePathChars( ::std::basic_string<CharType, Traits, Allocator> &name
                    , const CharType* sepList, CharType pathSep)
   {
    changePathChars(::std::make_pair(name.begin(), name.end()), isPathSeparator<CharType>(sepList), pathSep);
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator> 
changePathCharsCopy( const ::std::basic_string<CharType, Traits, Allocator> &name
                   , const CharType* sepList, CharType pathSep)
   {
    ::std::basic_string<CharType, Traits, Allocator> tmp = name;
    changePathChars(::std::make_pair(tmp.begin(), tmp.end()), isPathSeparator<CharType>(sepList), pathSep);
    return tmp;
   }

//-----------------------------------------------------------------------------
inline
void changePathChars(::std::string &name)
   {
    changePathChars(::std::make_pair(name.begin(), name.end()), isPathSeparator<char>(C_PATH_SEPARATORS), C_PATH_PREFFERED_SEPARATOR);
   }

//-----------------------------------------------------------------------------
inline
::std::string changePathCharsCopy(::std::string &aname)
   {
    ::std::string name = aname;
    changePathChars(::std::make_pair(name.begin(), name.end()), isPathSeparator<char>(C_PATH_SEPARATORS), C_PATH_PREFFERED_SEPARATOR);
    return name;
   }

//-----------------------------------------------------------------------------
inline
void changePathChars(::std::wstring &name)
   {
    changePathChars(::std::make_pair(name.begin(), name.end()), isPathSeparator<wchar_t>(W_PATH_SEPARATORS), W_PATH_PREFFERED_SEPARATOR);
   }

//-----------------------------------------------------------------------------
inline
::std::wstring changePathCharsCopy(::std::wstring &aname)
   {
    ::std::wstring name = aname;
    changePathChars(::std::make_pair(name.begin(), name.end()), isPathSeparator<wchar_t>(W_PATH_SEPARATORS), W_PATH_PREFFERED_SEPARATOR);
    return name;
   }

//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
template <typename Iter, typename pathPred>
void splitPath(::std::pair<Iter, Iter> path, 
               //::std::vector< ::std::pair<Iter, Iter>, ::std::allocator< ::std::pair<Iter, Iter> > > &parts,
               ::std::vector< ::std::pair<Iter, Iter> > &parts,
               pathPred pred)
   {
    Iter beginIt = path.first;
    Iter endIt   = beginIt;
    for(; endIt!=path.second && pred(*endIt); ++endIt) {};
    for(; endIt!=path.second; endIt++)
       {
        if (pred(*endIt))
           { // found separator
            if (beginIt!=endIt) // skip multiple path separator occurences
               parts.push_back(::std::make_pair(beginIt, endIt));            
            beginIt = endIt;
            beginIt++;
           }
       }
    if (beginIt!=endIt) // skip multiple path separator occurences
       parts.push_back(::std::make_pair(beginIt, endIt));
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
void splitPath( const ::std::basic_string<CharType, Traits, Allocator> &path
              , ::std::vector< ::std::basic_string<CharType, Traits, Allocator> > &pathParts,
               const CharType* pathSeparators /* = PATH_SEPARATORS */)
   {
    typedef typename ::std::basic_string<CharType, Traits, Allocator>::const_iterator const_iterator;
    ::std::vector< ::std::pair<const_iterator, const_iterator> > parts;

    splitPath( ::std::make_pair(path.begin(), path.end()), 
               parts, isPathSeparator<CharType>(pathSeparators) );
    for(typename ::std::vector< ::std::pair<const_iterator, const_iterator> >::const_iterator it = parts.begin();
        it!=parts.end();
        ++it)
       {
        pathParts.push_back(::std::basic_string<CharType, Traits, Allocator>(it->first, it->second));
       }
   }

//-----------------------------------------------------------------------------
inline
void splitPath(const ::std::string &path, ::std::vector< ::std::string > & pathParts )
   { splitPath(path, pathParts, C_PATH_SEPARATORS); }

//-----------------------------------------------------------------------------
inline
void splitPath(const ::std::wstring &path, ::std::vector< ::std::wstring > & pathParts )
   { splitPath(path, pathParts, W_PATH_SEPARATORS); }

//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
/*
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator> 
mergePath( typename ::std::vector< ::std::basic_string<CharType, Traits, Allocator> >::const_iterator b
         , typename ::std::vector< ::std::basic_string<CharType, Traits, Allocator> >::const_iterator e
         , CharType pathSep )
   {
    ::std::basic_string<CharType, Traits, Allocator> res;
    const typename ::std::vector< ::std::basic_string<CharType, Traits, Allocator> >::const_iterator savedB = b;
    for(; b!=e; ++b)
       {
        if (b!=savedB)
           res.append(1, pathSep);
        res.append(*b);
       }
    return res;
   }
*/

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator> 
mergePath( typename ::std::vector< ::std::basic_string<CharType, Traits, Allocator> >::const_iterator b
         , typename ::std::vector< ::std::basic_string<CharType, Traits, Allocator> >::const_iterator e
         , CharType pathSep
         )
   {
    ::std::basic_string<CharType, Traits, Allocator> res;
    const typename ::std::vector< ::std::basic_string<CharType, Traits, Allocator> >::const_iterator savedB = b;
    for(; b!=e; ++b)
       {
        if (b!=savedB)
           res.append(1, pathSep);
        res.append(*b);
       }
    return res;
   }

//-----------------------------------------------------------------------------
template< typename IterType
        , typename CharType 
        >
::std::basic_string<CharType, ::std::char_traits<CharType>, ::std::allocator<CharType> > 
mergePath( IterType b
         , IterType e
         , CharType pathSep
         )
   {
    ::std::basic_string<CharType, ::std::char_traits<CharType>, ::std::allocator<CharType> >  res;
    const IterType savedB = b;
    for(; b!=e; ++b)
       {
        if (b!=savedB)
           res.append(1, pathSep);
        res.append(*b);
       }
    return res;
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator> 
mergePath( const ::std::vector< ::std::basic_string<CharType, Traits, Allocator> > &pathParts
         , CharType pathSep /* = PATH_PREFFERED_SEPARATOR */)
   {
    return mergePath<CharType, Traits, Allocator>(pathParts.begin(), pathParts.end(), pathSep);
   }

//-----------------------------------------------------------------------------
inline
::std::string 
mergePath( const ::std::vector< ::std::string > &pathParts )
   {
    return mergePath<char, ::std::char_traits<char>, ::std::allocator<char> >(pathParts.begin(), pathParts.end(), C_PATH_PREFFERED_SEPARATOR);
   }

//-----------------------------------------------------------------------------
inline
::std::wstring 
mergePath( const ::std::vector< ::std::wstring > &pathParts )
   {
    return mergePath<wchar_t, ::std::char_traits<wchar_t>, ::std::allocator<wchar_t> >(pathParts.begin(), pathParts.end(), W_PATH_PREFFERED_SEPARATOR);
   }

//-----------------------------------------------------------------------------
#ifdef FILENAME_USE_TEST
#ifndef FILENAME_DISABLE_SPLITFILENAMETEST
namespace test
{

inline
::std::ostream& printStringVector(::std::ostream &os, const ::std::vector<tstring> &vec, const tstring &sep)
   {
    ::std::vector<tstring>::const_iterator it = vec.begin();
    for(; it!=vec.end(); ++it)
       {
        if (it!=vec.begin()) os<<sep;
        os<<*it;
       }
    return os;
   }

inline
::std::ostream& printSplitPath(::std::ostream &os, const tstring &path)
   {
    ::std::vector<tstring> vec;
    MARTY_NS filename::splitPath(path, vec);
    MARTY_NS filename::test::printStringVector(os, vec, _T(","));
    return os;
   }

//-----------------------------------------------------------------------------
inline
bool splitFilenameTest(const tstring &fullName, const tstring &chkPath, const tstring &chkName, const tstring &chkExt )
   {
    tstring path, name, ext;
    MARTY_NS filename::splitFilename(fullName, path, name, ext);

    bool bRes = true;

    if (path!=chkPath || name!=chkName || ext!=chkExt)
       {
        bRes = false;

        tcout<<_T("splitFilename test failed, wrong");
        
        if (path!=chkPath) tcout<<_T(" path");
    
        if (name!=chkName) tcout<<_T(" name");       

        if (ext!=chkExt)   tcout<<_T(" ext");       
       }

    if (!bRes)
       {
        std::cout<<_T(" - path: [")<<path<<_T("], name: [")<<name<<_T("], ext: [")<<ext<<_T("]\n");
       }
    #ifdef FILENAME_PRINT_ALL_TESTS
    else
       {
        std::cout<<_T("splitFilename test passed. Path: [")<<path<<_T("], name: [")<<name<<_T("], ext: [")<<ext<<_T("]\n");
       }
    #endif
    return bRes;
   }

//-----------------------------------------------------------------------------
struct CFilenameTestStruct
{
    TCHAR *name;
    TCHAR *res1;
    TCHAR *res2;
    TCHAR *res3;
};

// return num of failed tests
inline
int splitFilenameTestSuite(int *pTotalTests /* pointer to var that incremented with total num of tests */)
   {
    CFilenameTestStruct tests[] = {
                                   { _T("c:\\winnt/system32/file.ext"), _T("c:\\winnt/system32/"), _T("file"), _T(".ext") },
                                   { _T("c:\\winnt/system32/file.ext1.ext2"), _T("c:\\winnt/system32/"), _T("file"), _T(".ext1.ext2") },
                                   { _T("file.ext"), _T(""), _T("file"), _T(".ext") },
                                   { _T("file.ext1.ext2"), _T(""), _T("file"), _T(".ext1.ext2") },
                                   //{ _T(""), _T(""), _T(""), _T("") },
                                   { 0, 0, 0, 0 }
                                  };
    int failedTests = 0;
    int i = 0;
    CFilenameTestStruct *pTests = tests;
    for(; pTests->name; ++i, ++pTests)
       {
        if (!MARTY_NS filename::test::splitFilenameTest(pTests->name, pTests->res1, pTests->res2, pTests->res3))
           {
            ++failedTests;
           }
       }

    if (pTotalTests)
       *pTotalTests += i;

    tcout<<_T("splitFilenameTestSuite: total tests - ")<<i<<_T(", failed tests - ")<<failedTests<<_T("\n---\n");

    return failedTests;
   }

}; // namespace test
#endif // FILENAME_DISABLE_SPLITFILENAMETEST
#endif // FILENAME_USE_TEST

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template <typename Iter, typename Pred>
::std::pair<Iter, Iter> getPath(::std::pair<Iter, Iter> pair, Pred pred)
   {
    Iter it = pair.first;
    Iter lastFoundIt = pair.first;
    for(; it!=pair.second; )
       if (pred(*it++)) { lastFoundIt = it; }
    return ::std::make_pair( pair.first, lastFoundIt );
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator> 
getPath( const ::std::basic_string<CharType, Traits, Allocator> &fullName
       , const CharType* pathSeparators /* = PATH_SEPARATORS */)
   {
    typedef typename ::std::basic_string<CharType, Traits, Allocator>::const_iterator const_iterator;
    std::pair<const_iterator, const_iterator> res = getPath(
                                                            ::std::make_pair(fullName.begin(), fullName.end()), 
                                                            isPathSeparator<CharType>(pathSeparators)
                                                           );
    return ::std::basic_string<CharType, Traits, Allocator>(res.first, res.second); 
   }

//-----------------------------------------------------------------------------
inline
::std::string getPath(const ::std::string &fullName)
   {
    return getPath(fullName, C_PATH_SEPARATORS); 
   }

//-----------------------------------------------------------------------------
inline
::std::wstring getPath(const ::std::wstring &fullName)
   {
    return getPath(fullName, W_PATH_SEPARATORS);
   }

//-----------------------------------------------------------------------------
#ifdef FILENAME_OLD_NAMES
inline
tstring path(const tstring &fullName, const TCHAR* pathSeparators = PATH_SEPARATORS)
   {
    // typename removed - MINGW reports error
    std::pair<tstring::const_iterator, tstring::const_iterator> res = MARTY_NS filename::getPath(
                                   ::std::make_pair(fullName.begin(), fullName.end()), 
                                   isPathSeparator<TCHAR>(pathSeparators)
                                                                   );
    if (res.first!=res.second)
       return tstring(res.first, --res.second);
    else
       return tstring(res.first, res.second);
   }
#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#ifdef FILENAME_USE_TEST
namespace test
{

//-----------------------------------------------------------------------------
inline
bool getPathTest(const tstring &fullName, const tstring &resPath, bool rightRes)
   {
    bool res = (MARTY_NS filename::getPath(fullName)==resPath);
    return (res==rightRes);
   }

//-----------------------------------------------------------------------------
// return num of failed tests
inline
int getPathTestSuite(int *pTotalTests /* pointer to var that incremented with total num of tests */)
   {
    genericTestCaseResStruct tests[] = {
                                     { _T("c:\\winnt\\system32"), _T("c:\\winnt\\"), true },
                                     { _T("c:\\winnt\\system32"), _T("c:\\winnt"), false },
                                     { _T("c:/winnt\\system32"), _T("c:/winnt\\"), true },
                                     { _T("file.txt"), _T(""), true },
                                     //{ _T(""), _T(""), false },
                                     { 0, 0, false }
                                    };
    return MARTY_NS filename::test::genericTestSuite(tests, pTotalTests, _T("getPath"), MARTY_NS filename::test::getPathTest);
   }

}; // namespace test
#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



// return file name (file+ext)
//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator> 
getFile(const ::std::basic_string<CharType, Traits, Allocator>  &fullName, const CharType* pathSeparators, const CharType* extSeparators)
   {
    typedef typename ::std::basic_string<CharType, Traits, Allocator>::const_iterator const_iterator;
    ::std::pair<const_iterator, const_iterator> pathPair, namePair, extPair;
    splitFilename(::std::make_pair(fullName.begin(), fullName.end()),
                  pathPair, namePair, extPair,
                  isPathSeparator<CharType>(pathSeparators),
                  isExtSeparator<CharType>(extSeparators));
    if (namePair.first==namePair.second)
        return ::std::basic_string<CharType, Traits, Allocator> (namePair.first, namePair.second);
    else // name and ext
        return ::std::basic_string<CharType, Traits, Allocator> (namePair.first, fullName.end());
   }

//-----------------------------------------------------------------------------
inline
::std::string getFile(const ::std::string &fullName)
   { 
    return MARTY_NS filename::getFile(fullName, C_PATH_SEPARATORS, C_PATH_EXT_SEPARATOR_STR);
   }

//-----------------------------------------------------------------------------
inline
::std::wstring getFile(const ::std::wstring &fullName)
   { 
    return MARTY_NS filename::getFile(fullName, W_PATH_SEPARATORS, W_PATH_EXT_SEPARATOR_STR);
   }

//-----------------------------------------------------------------------------
#ifdef FILENAME_OLD_NAMES
inline
tstring file(const tstring &fullName, const TCHAR* pathSeparators = PATH_SEPARATORS)
   { return getFile<TCHAR>(fullName, pathSeparators, PATH_EXT_SEPARATOR_STR); }
#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#ifdef FILENAME_USE_TEST
namespace test
{

//-----------------------------------------------------------------------------
// return num of failed tests
inline
int getFileTestSuite(int *pTotalTests /* pointer to var that incremented with total num of tests */)
   {
    genericTestCaseStruct tests[] = {
                                     { _T("c:\\winnt\\system32/file.txt"), _T("file.txt") },
                                     { _T("c:\\winnt\\system32/file"), _T("file") },
                                     { _T("file.txt"), _T("file.txt") },
                                     { _T("file"), _T("file") },
                                     //{ _T(""), _T("") },
                                     { 0, 0 }
                                    };
    
    return MARTY_NS filename::test::genericTestSuite(tests, pTotalTests, _T("getFile"), MARTY_NS filename::getFile);
   }

}; // namespace test
#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator>
getExtention( const ::std::basic_string<CharType, Traits, Allocator> &fullName, 
              const CharType*  pathSeparators, 
              const CharType*  extSeparators)
   {
    typedef typename ::std::basic_string<CharType, Traits, Allocator>::const_iterator const_iterator;
    ::std::pair<const_iterator, const_iterator> pathPair, namePair, extPair;
    splitFilename(::std::make_pair(fullName.begin(), fullName.end()),
                  pathPair, namePair, extPair,
                  isPathSeparator<CharType>(pathSeparators),
                  isExtSeparator<CharType>(extSeparators));
    return ::std::basic_string<CharType, Traits, Allocator>(extPair.first, extPair.second);
   }

//-----------------------------------------------------------------------------
inline 
::std::string getExtention( const ::std::string &fullName )
   { 
    return getExtention(fullName, C_PATH_SEPARATORS, C_PATH_EXT_SEPARATOR_STR); 
   }

//-----------------------------------------------------------------------------
inline 
::std::wstring getExtention( const ::std::wstring &fullName )
   { 
    return getExtention(fullName, W_PATH_SEPARATORS, W_PATH_EXT_SEPARATOR_STR); 
   }

//-----------------------------------------------------------------------------
#ifdef FILENAME_OLD_NAMES
inline
tstring extention( const tstring &fullName, 
                   const TCHAR* pathSeparators = PATH_SEPARATORS, 
                   const TCHAR* extSeparators = PATH_EXT_SEPARATOR_STR)
   { return getExtention<TCHAR>(fullName, pathSeparators, extSeparators); }
#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#ifdef FILENAME_USE_TEST
namespace test
{

//-----------------------------------------------------------------------------
// return num of failed tests
inline
int getExtentionTestSuite(int *pTotalTests)
   {
    genericTestCaseStruct tests[] = {
                                     { _T("c:\\winnt\\system32/file.txt"), _T(".txt") },
                                     { _T("c:\\winnt\\system32/file"), _T("") },
                                     { _T("file.txt"), _T(".txt") },
                                     { _T("file"), _T("") },
                                     //{ _T(""), _T("") },
                                     { 0, 0 }
                                    };
    return MARTY_NS filename::test::genericTestSuite(tests, pTotalTests, _T("getExtention"), MARTY_NS filename::getExtention);
   }

}; // namespace test
#endif
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator>
getName( const ::std::basic_string<CharType, Traits, Allocator> &fullName, 
         const CharType* pathSeparators, 
         const CharType* extSeparators)
   {
    #ifndef __GNUC__
    extSeparators;
    #endif
    typedef typename ::std::basic_string<CharType, Traits, Allocator>::const_iterator const_iterator;
    ::std::pair<const_iterator, const_iterator> pathPair, namePair, extPair;
    splitFilename(::std::make_pair(fullName.begin(), fullName.end()),
                  pathPair, namePair, extPair,
                  isPathSeparator<CharType>(pathSeparators),
                  isExtSeparator<CharType>(extSeparators));
    return ::std::basic_string<CharType, Traits, Allocator>(namePair.first, namePair.second);
   }

//-----------------------------------------------------------------------------
inline 
::std::string getName( const ::std::string &fullName )
   { 
    return getName( fullName, C_PATH_SEPARATORS, C_PATH_EXT_SEPARATOR_STR); 
   }

//-----------------------------------------------------------------------------
inline 
::std::wstring getName( const ::std::wstring &fullName )
   { 
    return getName( fullName, W_PATH_SEPARATORS, W_PATH_EXT_SEPARATOR_STR); 
   }

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#ifdef FILENAME_USE_TEST
namespace test
{

//-----------------------------------------------------------------------------
// return num of failed tests
inline
int getNameTestSuite(int *pTotalTests)
   {
    genericTestCaseStruct tests[] = {
                                     { _T("c:\\winnt\\system32/file.txt"), _T("file") },
                                     { _T("c:\\winnt\\system32\\file"), _T("file") },
                                     { _T("file.txt"), _T("file") },
                                     { _T("file"), _T("file") },
                                     //{ _T(""), _T("") },
                                     { 0, 0 }
                                    };
    return MARTY_NS filename::test::genericTestSuite(tests, pTotalTests, _T("getName"), MARTY_NS filename::getName);
   }

}; // namespace test
#endif
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator>
getPathName( const ::std::basic_string<CharType, Traits, Allocator> &fullName, 
             const CharType* pathSeparators, 
             const CharType* extSeparators)
   {
    #ifndef __GNUC__
    extSeparators;
    #endif
    typedef typename ::std::basic_string<CharType, Traits, Allocator>::const_iterator const_iterator;
    ::std::pair<const_iterator, const_iterator > pathPair, namePair, extPair;
    splitFilename(::std::make_pair(fullName.begin(), fullName.end()),
                  pathPair, namePair, extPair,
                  isPathSeparator<CharType>(pathSeparators),
                  isExtSeparator<CharType>(extSeparators));
    return ::std::basic_string<CharType, Traits, Allocator>(pathPair.first, namePair.second);
   }

//-----------------------------------------------------------------------------
inline
::std::string getPathName( const ::std::string &fullName)
   { 
    return getPathName( fullName, C_PATH_SEPARATORS, C_PATH_EXT_SEPARATOR_STR); 
   }

//-----------------------------------------------------------------------------
inline
::std::wstring getPathName( const ::std::wstring &fullName)
   { 
    return getPathName( fullName, W_PATH_SEPARATORS, W_PATH_EXT_SEPARATOR_STR); 
   }

//-----------------------------------------------------------------------------

#ifdef FILENAME_OLD_NAMES
inline
tstring name( const tstring &fullName, 
              const TCHAR*  pathSeparators = PATH_SEPARATORS, 
              const TCHAR*  extSeparators = PATH_EXT_SEPARATOR_STR)
   { return MARTY_NS filename::getPathName<TCHAR>(fullName, pathSeparators, extSeparators); }
#endif


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#ifdef FILENAME_USE_TEST
namespace test
{

//-----------------------------------------------------------------------------
// return num of failed tests
inline
int getPathNameTestSuite(int *pTotalTests)
   {
    genericTestCaseStruct tests[] = {
                                     { _T("c:\\winnt\\system32/file.txt"), _T("c:\\winnt\\system32/file") },
                                     { _T("c:\\winnt\\system32\\file"), _T("c:\\winnt\\system32\\file") },
                                     { _T("file.txt"), _T("file") },
                                     { _T("file"), _T("file") },
                                     //{ _T(""), _T("") },
                                     { 0, 0 }
                                    };
    return MARTY_NS filename::test::genericTestSuite(tests, pTotalTests, _T("getPathName"), MARTY_NS filename::getPathName);
   }

}; // namespace test
#endif
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator>
appendPathSep(const ::std::basic_string<CharType, Traits, Allocator> &path, 
              const CharType* pathSeparators   /* = PATH_SEPARATORS */ ,
              CharType pathPrefferedSeparator  /* = PATH_PREFFERED_SEPARATOR */ )
   {
    if (!path.empty() && !isPathSeparator<CharType>(pathSeparators)(path[path.size()-1]))
       return path + ::std::basic_string<CharType, Traits, Allocator>(1, pathPrefferedSeparator);
    return path;
    //if (!path.empty() )
   }

//-----------------------------------------------------------------------------
inline
::std::string appendPathSep(const ::std::string &path)
   {
    return appendPathSep(path, C_PATH_SEPARATORS, C_PATH_PREFFERED_SEPARATOR);
   }

//-----------------------------------------------------------------------------
inline
::std::wstring appendPathSep(const ::std::wstring &path)
   {
    return appendPathSep(path, W_PATH_SEPARATORS, W_PATH_PREFFERED_SEPARATOR);
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
void
appendPathSepInplace(::std::basic_string<CharType, Traits, Allocator> &path, 
              const CharType* pathSeparators   /* = PATH_SEPARATORS */ ,
              CharType pathPrefferedSeparator  /* = PATH_PREFFERED_SEPARATOR */ )
   {
    if (!path.empty() && !isPathSeparator<CharType>(pathSeparators)(path[path.size()-1]))
       path.append(1, pathPrefferedSeparator);
   }

//-----------------------------------------------------------------------------
inline
void appendPathSepInplace(::std::string &path)
   {
    return appendPathSepInplace(path, C_PATH_SEPARATORS, C_PATH_PREFFERED_SEPARATOR);
   }

//-----------------------------------------------------------------------------
inline
void appendPathSepInplace(::std::wstring &path)
   {
    return appendPathSepInplace(path, W_PATH_SEPARATORS, W_PATH_PREFFERED_SEPARATOR);
   }

//-----------------------------------------------------------------------------
inline
::std::string appendPath(const ::std::string &path, const ::std::string &pathAppend,
                   const char*  pathSeparators = C_PATH_SEPARATORS,
                   char pathPrefferedSeparator = C_PATH_PREFFERED_SEPARATOR)
   {
    return appendPathSep(path, pathSeparators, pathPrefferedSeparator) + pathAppend;
   }

//-----------------------------------------------------------------------------
inline
::std::wstring appendPath(const ::std::wstring &path, const ::std::wstring &pathAppend,
                   const wchar_t*  pathSeparators = W_PATH_SEPARATORS,
                   wchar_t pathPrefferedSeparator = W_PATH_PREFFERED_SEPARATOR)
   {
    return appendPathSep(path, pathSeparators, pathPrefferedSeparator) + pathAppend;
   }

//-----------------------------------------------------------------------------
inline
void appendPathInplace(::std::string &path, const ::std::string &pathAppend,
                       const char*  pathSeparators = C_PATH_SEPARATORS,
                       char pathPrefferedSeparator = C_PATH_PREFFERED_SEPARATOR)
   {
    appendPathSepInplace(path, pathSeparators, pathPrefferedSeparator);
    path.append(pathAppend);
   }

//-----------------------------------------------------------------------------
inline
void appendPathInplace(::std::wstring &path, const ::std::wstring &pathAppend,
                       const wchar_t*  pathSeparators = W_PATH_SEPARATORS,
                       wchar_t pathPrefferedSeparator = W_PATH_PREFFERED_SEPARATOR)
   {
    appendPathSepInplace(path, pathSeparators, pathPrefferedSeparator);
    path.append(pathAppend);
   }


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#ifdef FILENAME_USE_TEST
#ifndef FILENAME_DISABLE_PATHTEST
namespace test
{
inline int appendPathTestSuite(int *pTotalTests)
   {
    genericTestCaseStruct2 tests[] = {
                                     { _T("c:\\winnt\\system32"), _T("file"), _T("c:\\winnt\\system32\\file")},
                                     { _T("c:\\winnt\\system32\\"), _T("file"), _T("c:\\winnt\\system32\\file")},
                                     { _T("c:\\winnt\\system32/"), _T("file"), _T("c:\\winnt\\system32/file")},
                                     { _T("c:\\winnt\\system32"), _T("file.ext"), _T("c:\\winnt\\system32\\file.ext")},
                                     { _T("c:\\winnt\\system32\\"), _T("file.ext"), _T("c:\\winnt\\system32\\file.ext")},
                                     { _T("c:\\winnt\\system32/"), _T("file.ext"), _T("c:\\winnt\\system32/file.ext")},

                                     { _T("c:\\winnt"), _T("system32\\file"), _T("c:\\winnt\\system32\\file")},
                                     { _T("c:\\winnt\\"), _T("system32\\file"), _T("c:\\winnt\\system32\\file")},
                                     { _T("c:\\winnt\\"), _T("system32/file"), _T("c:\\winnt\\system32/file")},
                                     { _T("c:\\winnt\\"), _T("system32\\file.ext"), _T("c:\\winnt\\system32\\file.ext")},
                                     { _T("c:\\winnt\\"), _T("system32\\file.ext"), _T("c:\\winnt\\system32\\file.ext")},
                                     { _T("c:\\winnt"), _T("system32/file.ext"), _T("c:\\winnt\\system32/file.ext")},
                                     { _T(""), _T("file.ext"), _T("file.ext")},
                                     { 0, 0, 0 }
                                    };
    return MARTY_NS filename::test::genericTestSuite(tests, pTotalTests, _T("appendPathName"), MARTY_NS filename::appendPath);
   }
}; // namespace test
#endif // FILENAME_DISABLE_PATHTEST
#endif // FILENAME_USE_TEST
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator>
appendExtention( const ::std::basic_string<CharType, Traits, Allocator> &name
               , const ::std::basic_string<CharType, Traits, Allocator> &ext
               , const CharType* extSeparators /*= PATH_EXT_SEPARATOR_STR*/
               , CharType extSeparator /*= PATH_EXT_SEPARATOR*/)
   {
    ::std::basic_string<CharType, Traits, Allocator> tmp = name;
    if (!ext.empty() && !isExtSeparator<CharType>(extSeparators)(ext[0]))
        tmp.append(1, extSeparator);
    return tmp + ext;
   }

//-----------------------------------------------------------------------------
inline 
::std::string appendExtention(const ::std::string &name, const ::std::string &ext)
   { 
    return appendExtention(name, ext, C_PATH_EXT_SEPARATOR_STR, C_PATH_EXT_SEPARATOR); 
   }

//-----------------------------------------------------------------------------
inline 
::std::wstring appendExtention(const ::std::wstring &name, const ::std::wstring &ext)
   { 
    return appendExtention(name, ext, W_PATH_EXT_SEPARATOR_STR, W_PATH_EXT_SEPARATOR);
   }


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#ifdef FILENAME_USE_TEST
namespace test
{
inline int appendExtentionTestSuite(int *pTotalTests)
   {
    genericTestCaseStruct2 tests[] = {
                                     { _T("c:\\winnt\\system32\\file"), _T("ext"), _T("c:\\winnt\\system32\\file.ext")},
                                     { _T("c:\\winnt\\system32\\file"), _T(".ext"), _T("c:\\winnt\\system32\\file.ext")},
                                     { _T("c:\\winnt\\system32\\file.ext1"), _T("ext2"), _T("c:\\winnt\\system32\\file.ext1.ext2")},
                                     { _T("c:\\winnt\\system32\\file.ext1"), _T(".ext2"), _T("c:\\winnt\\system32\\file.ext1.ext2")},
                                     { _T("file"), _T(".ext"), _T("file.ext")},
                                     { _T("file"), _T("ext"), _T("file.ext")},
                                     { _T("file.ext1"), _T(".ext2"), _T("file.ext1.ext2")},
                                     { _T("file.ext1"), _T("ext2"), _T("file.ext1.ext2")},
                                     { 0, 0, 0 }
                                    };
    return MARTY_NS filename::test::genericTestSuite(tests, pTotalTests, _T("appendExtentionName"), MARTY_NS filename::appendExtention);
   }
}; // namespace test
#endif
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
#define removeLastSlashInplace  pathRemoveTrailingSlashInplace 
#define removeLastSlash         pathRemoveTrailingSlash

template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
void pathRemoveTrailingSlashInplace(::std::basic_string<CharType, Traits, Allocator> &path, const CharType* pathSeparators /*  = PATH_SEPARATORS */ )
   {
    if (!path.empty() && isPathSeparator<CharType>(pathSeparators)(path[path.size()-1]))
       path.erase(path.size()-1); 
   }

//-----------------------------------------------------------------------------
inline
void pathRemoveTrailingSlashInplace(::std::string &path)
   {
    if (!path.empty() && isPathSeparator<char>(C_PATH_SEPARATORS)(path[path.size()-1]))
       path.erase(path.size()-1); 
   }

//-----------------------------------------------------------------------------
inline
void pathRemoveTrailingSlashInplace(::std::wstring &path)
   {
    if (!path.empty() && isPathSeparator<wchar_t>(W_PATH_SEPARATORS)(path[path.size()-1]))
       path.erase(path.size()-1); 
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator>
pathRemoveTrailingSlash(const ::std::basic_string<CharType, Traits, Allocator> &path, const CharType* pathSeparators /*  = PATH_SEPARATORS */ )
   {
    ::std::basic_string<CharType, Traits, Allocator> copy = path;
    pathRemoveTrailingSlashInplace(copy, pathSeparators);
    return copy;
   }

//-----------------------------------------------------------------------------
inline
::std::string pathRemoveTrailingSlash(const ::std::string &path)
   {
    ::std::string copy = path;
    pathRemoveTrailingSlashInplace(copy, C_PATH_SEPARATORS);
    return copy;
   }

//-----------------------------------------------------------------------------
inline
::std::wstring pathRemoveTrailingSlash(const ::std::wstring &path)
   {
    ::std::wstring copy = path;
    pathRemoveTrailingSlashInplace(copy, W_PATH_SEPARATORS);
    return copy;
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        , typename Iter
        >
::std::basic_string<CharType, Traits, Allocator>
mergeFilename(::std::pair<Iter, Iter> pathPair,
              ::std::pair<Iter, Iter> namePair,
              ::std::pair<Iter, Iter> extPair,
              const CharType* pathSeparators /*  = PATH_SEPARATORS */ ,
              CharType pathPrefferedSeparator  /* = PATH_PREFFERED_SEPARATOR */ ,
              const CharType* extSeparators  /* = PATH_EXT_SEPARATOR_STR */ ,
              CharType extSeparator  /* = PATH_EXT_SEPARATOR */ )
   {
    return appendPath( ::std::basic_string<CharType, Traits, Allocator>(pathPair.first, pathPair.second), 
                       appendExtention( ::std::basic_string<CharType, Traits, Allocator>(namePair.first, namePair.second)
                                      , ::std::basic_string<CharType, Traits, Allocator>(extPair.first, extPair.second)
                                      , extSeparators, extSeparator), 
                       pathSeparators, pathPrefferedSeparator 
                     );
   }

//-----------------------------------------------------------------------------
/*
template <typename Iter>
tstring mergeFilename(::std::pair<Iter, Iter> pathPair,
                      ::std::pair<Iter, Iter> namePair,
                      ::std::pair<Iter, Iter> extPair,
                      const TCHAR* pathSeparators = PATH_SEPARATORS,
                      TCHAR pathPrefferedSeparator = PATH_PREFFERED_SEPARATOR,
                      const TCHAR* extSeparators = PATH_EXT_SEPARATOR_STR,
                      TCHAR extSeparator = PATH_EXT_SEPARATOR)
   {
    return MARTY_NS filename::appendPath(tstring(pathPair.first, pathPair.second), 
                                  MARTY_NS filename::appendExtention( tstring(namePair.first, namePair.second), 
                                                               ext, extSeparators, extSeparator), 
                                  pathSeparators, pathPrefferedSeparator 
                                 );
   }
*/

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator>
mergeFilename( const ::std::basic_string<CharType, Traits, Allocator> &path,
               const ::std::basic_string<CharType, Traits, Allocator> &name,
               const ::std::basic_string<CharType, Traits, Allocator> &ext,
               const CharType* pathSeparators,
               CharType pathPrefferedSeparator,
               const CharType* extSeparators,
               CharType extSeparator)
                   
   {
    return filename::appendPath(path, 
                                appendExtention(name, ext, extSeparators, extSeparator), 
                                pathSeparators, pathPrefferedSeparator 
                               );
   }

//-----------------------------------------------------------------------------
inline
::std::string mergeFilename( const ::std::string &path,
                       const ::std::string &name,
                       const ::std::string &ext)
                   
   {
    return appendPath( path
                     , appendExtention(name, ext, C_PATH_EXT_SEPARATOR_STR, C_PATH_EXT_SEPARATOR)
                     , C_PATH_SEPARATORS, C_PATH_PREFFERED_SEPARATOR 
                     );
   }

//-----------------------------------------------------------------------------
inline
::std::wstring mergeFilename( const ::std::wstring &path,
                       const ::std::wstring &name,
                       const ::std::wstring &ext)
                   
   {
    return appendPath( path
                     , appendExtention(name, ext, W_PATH_EXT_SEPARATOR_STR, W_PATH_EXT_SEPARATOR)
                     , W_PATH_SEPARATORS, W_PATH_PREFFERED_SEPARATOR
                     );
   }

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#ifdef FILENAME_USE_TEST
namespace test
{
inline int mergeFilenameTestSuite(int *pTotalTests)
   {
    genericTestCaseStruct3 tests[] = {
                                     { _T("c:\\winnt\\"), _T("file"), _T("ext"), _T("c:\\winnt\\file.ext") },
                                     { _T("c:\\winnt"), _T("file"), _T("ext"), _T("c:\\winnt\\file.ext") },
                                     { _T("c:\\winnt/"), _T("file"), _T("ext"), _T("c:\\winnt/file.ext") },

                                     { _T("c:\\winnt\\"), _T("file"), _T(".ext"), _T("c:\\winnt\\file.ext") },
                                     { _T("c:\\winnt"), _T("file"), _T(".ext"), _T("c:\\winnt\\file.ext") },
                                     { _T("c:\\winnt/"), _T("file"), _T(".ext"), _T("c:\\winnt/file.ext") },

                                     { _T("c:\\winnt\\"), _T("file"), _T(""), _T("c:\\winnt\\file") },
                                     { _T("c:\\winnt"), _T("file"), _T(""), _T("c:\\winnt\\file") },
                                     { _T("c:\\winnt/"), _T("file"), _T(""), _T("c:\\winnt/file") },

                                     //{ _T(""), _T(""), _T(""), _T("") },
                                     { 0, 0, 0, 0 }
                                    };
    return MARTY_NS filename::test::genericTestSuite(tests, pTotalTests, _T("mergeFilename"), MARTY_NS filename::mergeFilename);
   }
}; // namespace test
#endif
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator>
changeExtention( const ::std::basic_string<CharType, Traits, Allocator> &fullName, 
                 const ::std::basic_string<CharType, Traits, Allocator> &newExt,
                 const CharType*  pathSeparators  /* = PATH_SEPARATORS */ ,
                 const CharType*  extSeparators  /* = PATH_EXT_SEPARATOR_STR */ ,
                 CharType extSeparator  /* = PATH_EXT_SEPARATOR */ )
   {
    return appendExtention( getPathName(fullName, pathSeparators, extSeparators)
                          , newExt, extSeparators, extSeparator);
   }

//-----------------------------------------------------------------------------
inline
::std::string changeExtention( const ::std::string &fullName, 
                               const ::std::string &newExt )
   {
    return changeExtention(fullName, newExt, C_PATH_SEPARATORS, C_PATH_EXT_SEPARATOR_STR, C_PATH_EXT_SEPARATOR);
   }

//-----------------------------------------------------------------------------
inline
::std::wstring changeExtention( const ::std::wstring &fullName, 
                                const ::std::wstring &newExt )
   {
    return changeExtention(fullName, newExt, W_PATH_SEPARATORS, W_PATH_EXT_SEPARATOR_STR, W_PATH_EXT_SEPARATOR);
   }

//-----------------------------------------------------------------------------
template <typename Iter, typename Pred>
bool isUncAbsolutePath(::std::pair<Iter, Iter> name, Pred pred)
   {
    #ifdef WIN32
    unsigned i = 0;
    for(; name.first!=name.second && i<2; ++name.first, ++i)
       {
        if (!pred(*name.first)) break;
       }
    return i>=2;
    #endif
    return false;
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
bool isUncAbsolutePath(const ::std::basic_string<CharType, Traits, Allocator> &fullName, 
                       const CharType* pathSeparators  /* = PATH_SEPARATORS */ )
   {
    return isUncAbsolutePath( ::std::make_pair(fullName.begin(), fullName.end())
                            , isPathSeparator<CharType>(pathSeparators)
                            );
   }

//-----------------------------------------------------------------------------
inline
bool isUncAbsolutePath(const ::std::string &fullName )
   {
    return isUncAbsolutePath(fullName, C_PATH_SEPARATORS);
   }

//-----------------------------------------------------------------------------
inline
bool isUncAbsolutePath(const ::std::wstring &fullName )
   {
    return isUncAbsolutePath(fullName, W_PATH_SEPARATORS);
   }


//-----------------------------------------------------------------------------
template <typename Iter> 
bool isDriveAbsolutePath(::std::pair<Iter, Iter> name)
   {
    #ifdef WIN32
    if (name.first==name.second)    return false;
    if (!NSFN_ISALPHA(*name.first)) return false;
    if (++name.first==name.second)  return false;
    if (*name.first!=_T(':'))       return false;
    return true;
    #else
    return false;   
    #endif
   }

//-----------------------------------------------------------------------------
// pred not used, for same fn signature as other fns
template <typename Iter, typename Pred> 
bool isDriveAbsolutePath(::std::pair<Iter, Iter> name, Pred pred)
   {
    #ifdef WIN32
    if (name.first==name.second)    return false;
    if (!NSFN_ISALPHA(*name.first)) return false;
    if (++name.first==name.second)  return false;
    if (*name.first!=_T(':'))       return false;
    return true;
    #else
    return false;   
    #endif
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
bool isDriveAbsolutePath(const ::std::basic_string<CharType, Traits, Allocator> &fullName, 
                         const CharType*  pathSeparators /*  = PATH_SEPARATORS */ )
   {
    #ifdef WIN32
        #ifdef _MSC_VER
        pathSeparators;
        #endif
    return isDriveAbsolutePath( ::std::make_pair(fullName.begin(), fullName.end()) );
    #else
    //fullName;
    return false;   
    #endif
   }

//-----------------------------------------------------------------------------
inline
bool isDriveAbsolutePath(const ::std::string &fullName)
   {
    return isDriveAbsolutePath(fullName, C_PATH_SEPARATORS);
   }

//-----------------------------------------------------------------------------
inline
bool isDriveAbsolutePath(const ::std::wstring &fullName)
   {
    return isDriveAbsolutePath(fullName, W_PATH_SEPARATORS);
   }

//-----------------------------------------------------------------------------
template <typename Iter, typename Pred> 
bool isUnixAbsolutePath(::std::pair<Iter, Iter> name, Pred pred)
   {
    // Unix-style absolute path also used in Win32
    // as absolute path from current drive root
    if (name.first==name.second) return false;
    if (!pred(*name.first))      return false;
    #ifdef WIN32
    name.first++; // check the \\server\share\path path form
    if (name.first==name.second) return true;
    if (pred(*name.first))      return false;
    #endif
    return true;
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
bool isUnixAbsolutePath(const ::std::basic_string<CharType, Traits, Allocator> &fullName, 
                        const CharType*  pathSeparators /*  = PATH_SEPARATORS */ )
   {
    return isUnixAbsolutePath(
                     ::std::make_pair(fullName.begin(), fullName.end()),
                     isPathSeparator<CharType>(pathSeparators)
                             );
   }

//-----------------------------------------------------------------------------
inline
bool isUnixAbsolutePath(const ::std::string &fullName )
   {
    return isUnixAbsolutePath( fullName, C_PATH_SEPARATORS );
   }

//-----------------------------------------------------------------------------
inline
bool isUnixAbsolutePath(const ::std::wstring &fullName )
   {
    return isUnixAbsolutePath( fullName, W_PATH_SEPARATORS );
   }


//-----------------------------------------------------------------------------
template <typename Iter, typename Pred> 
bool isAbsolutePath(::std::pair<Iter, Iter> name, Pred pred)
   {
    return isUncAbsolutePath(name, pred) ||
           //isDriveAbsolutePath(name, pred) ||
           isDriveAbsolutePath(name) ||
           isUnixAbsolutePath(name, pred);
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
bool isAbsolutePath(const ::std::basic_string<CharType, Traits, Allocator> &fullName, 
                    const CharType*  pathSeparators  /* = PATH_SEPARATORS */ )
   {
    return isAbsolutePath(
                     ::std::make_pair(fullName.begin(), fullName.end()),
                     isPathSeparator<CharType>(pathSeparators)
                                     );
   }

//-----------------------------------------------------------------------------
inline
bool isAbsolutePath(const ::std::string &fullName )
   {
    return isAbsolutePath(
                     ::std::make_pair(fullName.begin(), fullName.end()),
                     isPathSeparator<char>(C_PATH_SEPARATORS)
                         );
   }

//-----------------------------------------------------------------------------
inline
bool isAbsolutePath(const ::std::wstring &fullName )
   {
    return isAbsolutePath(
                     ::std::make_pair(fullName.begin(), fullName.end()),
                     isPathSeparator<wchar_t>(W_PATH_SEPARATORS)
                         );
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator> 
makeFullPath( const ::std::basic_string<CharType, Traits, Allocator> &basePath
            , const ::std::basic_string<CharType, Traits, Allocator> &relPath
            , const CharType*  pathSeparators  /* = PATH_SEPARATORS */ 
            , CharType pathPrefferedSeparator  /* = PATH_PREFFERED_SEPARATOR */  )
   {
    if (isAbsolutePath(relPath, pathSeparators)) return relPath;
    return appendPath(basePath, relPath, pathSeparators, pathPrefferedSeparator );
   }

//-----------------------------------------------------------------------------
inline
::std::string
makeFullPath( const ::std::string &basePath
            , const ::std::string &relPath )
   {
    return makeFullPath(basePath, relPath, C_PATH_SEPARATORS, C_PATH_PREFFERED_SEPARATOR);
   }

//-----------------------------------------------------------------------------
inline
::std::wstring
makeFullPath( const ::std::wstring &basePath
            , const ::std::wstring &relPath )
   {
    return makeFullPath(basePath, relPath, W_PATH_SEPARATORS, W_PATH_PREFFERED_SEPARATOR);
   }


//-----------------------------------------------------------------------------
namespace utils
{
//-----------------------------------------------------------------------------
template <typename Iter1, typename Iter2>
bool equalPairs( ::std::pair<Iter1, Iter1> p1, ::std::pair<Iter2, Iter2> p2)
   {
    for(; p1.first!=p1.second && p2.first!=p2.second; ++p1.first, ++p2.first)
       {
        if (*p1.first != *p2.first) return false;
       }
    return p1.first==p1.second && p2.first==p2.second;
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        , typename Iter
        >
bool equalToPair( const ::std::basic_string<CharType, Traits, Allocator> &str, ::std::pair<Iter, Iter> p)
   {
    return equalPairs( ::std::make_pair(str.begin(), str.end()), p);
   }
}; /* namespace utils */
//-----------------------------------------------------------------------------






template <typename Iter, typename CharType >
bool isDirectorySpecialNameAux( ::std::pair<Iter, Iter> dirNamePair 
                              , CharType ch, unsigned n)
   {
    return utils::equalToPair( ::std::basic_string<CharType, ::std::char_traits<CharType>, std::allocator<CharType> >(n, ch)
                             , dirNamePair );
   }

/*
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
bool isCurrentDirectorySpecialName( ::std::pair< typename ::std::basic_string<CharType, Traits, Allocator>::const_iterator
                                               , typename ::std::basic_string<CharType, Traits, Allocator>::const_iterator> dirNamePair)
   {
    return isDirectorySpecialNameAux(dirNamePair, reinterpret_cast<CharType>('.'), 1);
   }
*/

//-----------------------------------------------------------------------------
inline
bool isCurrentDirectorySpecialName( ::std::pair< ::std::string::const_iterator
                                               , ::std::string::const_iterator> dirNamePair)
   {
    return isDirectorySpecialNameAux(dirNamePair, '.', 1);
   }

//-----------------------------------------------------------------------------
inline
bool isCurrentDirectorySpecialName( ::std::pair< ::std::wstring::const_iterator
                                               , ::std::wstring::const_iterator> dirNamePair)
   {
    return isDirectorySpecialNameAux(dirNamePair, L'.', 1);
   }

//-----------------------------------------------------------------------------
inline
bool isCurrentDirectorySpecialName(const ::std::string& dirName )
   { 
    return isCurrentDirectorySpecialName(::std::make_pair(dirName.begin(), dirName.end()) ); 
   }

//-----------------------------------------------------------------------------
inline
bool isCurrentDirectorySpecialName(const ::std::wstring& dirName )
   { 
    return isCurrentDirectorySpecialName(::std::make_pair(dirName.begin(), dirName.end()) ); 
   }

//-----------------------------------------------------------------------------
inline
bool isUpperDirectorySpecialName( ::std::pair< ::std::string::const_iterator
                                             , ::std::string::const_iterator> dirNamePair)
   {
    return isDirectorySpecialNameAux(dirNamePair, '.', 2);
   }

//-----------------------------------------------------------------------------
inline
bool isUpperDirectorySpecialName( ::std::pair< ::std::wstring::const_iterator
                                             , ::std::wstring::const_iterator> dirNamePair)
   {
    return isDirectorySpecialNameAux(dirNamePair, L'.', 2);
   }

//-----------------------------------------------------------------------------
inline
bool isUpperDirectorySpecialName(const ::std::string& dirName )
   { 
    return isUpperDirectorySpecialName(::std::make_pair(dirName.begin(), dirName.end()) ); 
   }

//-----------------------------------------------------------------------------
inline
bool isUpperDirectorySpecialName(const ::std::wstring& dirName )
   { 
    return isUpperDirectorySpecialName(::std::make_pair(dirName.begin(), dirName.end()) ); 
   }



//-----------------------------------------------------------------------------
template <typename Iter, typename Pred>
void makeCanonical(const ::std::vector< ::std::pair<Iter, Iter> > &srcPathParts,
                   //const ::std::vector< ::std::pair<Iter, Iter>, ::std::allocator< ::std::pair<Iter, Iter> > > &srcPathParts,
                   Pred pathPred,
                   //::std::vector< ::std::pair<Iter, Iter>, ::std::allocator< ::std::pair<Iter, Iter> > > &resVec
                   ::std::vector< ::std::pair<Iter, Iter> > &resVec
                  )
   {
    resVec.reserve(srcPathParts.size());
    typename ::std::vector< ::std::pair<Iter, Iter> >::const_iterator it = srcPathParts.begin();

    for(;it!=srcPathParts.end(); ++it)
       {
        if (MARTY_NS filename::isCurrentDirectorySpecialName(*it))
           { // skip current directory
            if (resVec.empty()) // don't skip first element in path
               resVec.push_back(*it);
           }
        else if (MARTY_NS filename::isUpperDirectorySpecialName(*it))
           { // go one level up
            if (resVec.empty()) 
               resVec.push_back(*it); // possible error, but we ignoring it
            else if (resVec.size()>1) 
               {
                if (MARTY_NS filename::isUpperDirectorySpecialName(resVec.back()))
                   resVec.push_back(*it); // possible error, but we ignoring it
                else
                   resVec.pop_back(); // Ok
               }
            else if (MARTY_NS filename::isAbsolutePath(*(resVec.begin()), pathPred))
               resVec.push_back(*it); // possible error, but we ignoring it
            else
               {
                if (MARTY_NS filename::isUpperDirectorySpecialName(resVec.back()))
                   resVec.push_back(*it); // possible error, but we ignoring it
                else
                   resVec.pop_back(); // Ok
               }
           }
        else
           resVec.push_back(*it);
       }
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator>
makeCanonical(const ::std::basic_string<CharType, Traits, Allocator> &path, 
              const CharType* pathSeparators,
              CharType pathPrefferedSeparator)
   {
    typedef typename ::std::basic_string<CharType, Traits, Allocator>::const_iterator const_iterator;
    ::std::vector< ::std::pair<const_iterator, const_iterator > > srcParts;
    splitPath(::std::make_pair(path.begin(), path.end()), srcParts, isPathSeparator<CharType>(pathSeparators));

    ::std::vector< ::std::pair<const_iterator, const_iterator> > resParts;
    makeCanonical(srcParts, isPathSeparator<CharType>(pathSeparators), resParts);

    ::std::basic_string<CharType, Traits, Allocator> res; res.reserve(path.size());
    
    typename ::std::vector< ::std::pair<const_iterator, const_iterator> >::const_iterator it = resParts.begin();
    for(;it!=resParts.end(); ++it)
       {
        /*
        if (it==resParts.begin())
           changePathChars(*it, isPathSeparator<TC>(pathSeparators), pathPrefferedSeparator);
        */
        if (!res.empty()) res.append(1, pathPrefferedSeparator);
        
        // call append directly causes 
        // Unresolved external basic_string::::replace<const char *>
        //                     (char *, char *, const char *, const char *)
        if (it==resParts.begin())
           {
            //res.append(changePathCharsCopy(MARTY_TCSTRING(it->first, it->second), isPathSeparator<TC>(pathSeparators), pathPrefferedSeparator));
            res.append(
                       changePathCharsCopy( ::std::basic_string<CharType, Traits, Allocator>(it->first, it->second)
                                          , pathSeparators  /* isPathSeparator<TC>(pathSeparators) */ 
                                          , pathPrefferedSeparator
                                          )
                      );
           }
        else
           #ifdef __BORLANDC__
           res.append(::std::basic_string<CharType, Traits, Allocator>(it->first, it->second));
           #else
           res.append(it->first, it->second);
           #endif
        /*
        if (it==resParts.begin())
           res.append(changePathCharsCopy(MARTY_TCSTRING(it->first, it->second), isPathSeparator<TC>(pathSeparators), pathPrefferedSeparator))
        res.append(it->first, it->second);
        */
       }
    
    return res;   
   }

//-----------------------------------------------------------------------------
inline
::std::string makeCanonical(const ::std::string &path)
   { 
    return makeCanonical(path, C_PATH_SEPARATORS, C_PATH_PREFFERED_SEPARATOR);
   }

//-----------------------------------------------------------------------------
inline
::std::wstring makeCanonical(const ::std::wstring &path)
   { 
    return makeCanonical(path, W_PATH_SEPARATORS, W_PATH_PREFFERED_SEPARATOR);
   }

//-----------------------------------------------------------------------------
inline
::std::string makeCanonicalStrict(const ::std::string &path)
   { 
    return makeCanonical(path, C_PATH_SEPARATORS_STRICT, C_PATH_PREFFERED_SEPARATOR_STRICT);
   }

//-----------------------------------------------------------------------------
inline
::std::wstring makeCanonicalStrict(const ::std::wstring &path)
   { 
    return makeCanonical(path, W_PATH_SEPARATORS_STRICT, W_PATH_PREFFERED_SEPARATOR_STRICT);
   }


// TODO: make functions below as templates <typename TC> as above

//-----------------------------------------------------------------------------
template <typename Iter, typename Pred>
void makeValidDirectoryName(const ::std::pair<Iter, Iter> &dir,
                        ::std::pair<Iter, Iter> &dstName,
                        Pred pathPred)
   {
    dstName = dir;
    if (dstName.first==dstName.second) return;
    --dstName.second;

    if (!pathPred(*dstName.second))
       dstName.second = dir.second;
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator>
makeValidDirectoryName(const ::std::basic_string<CharType, Traits, Allocator> &dir,
                       const CharType* pathSeparators)
   {
    typedef typename ::std::basic_string<CharType, Traits, Allocator>::const_iterator const_iterator;
    ::std::pair<const_iterator, const_iterator> dirPair;
    makeValidDirectoryName(::std::make_pair(dir.begin(), dir.end()), dirPair, isPathSeparator<CharType>(pathSeparators));
    return ::std::basic_string<CharType, Traits, Allocator>(dirPair.first, dirPair.second);
   }

//-----------------------------------------------------------------------------
inline
::std::string makeValidDirectoryName(const ::std::string &dir)
   {
    return makeValidDirectoryName(dir, C_PATH_SEPARATORS);
   }

//-----------------------------------------------------------------------------
inline
::std::wstring makeValidDirectoryName(const ::std::wstring &dir)
   {
    return makeValidDirectoryName(dir, W_PATH_SEPARATORS);
   }


//-----------------------------------------------------------------------------
inline
void makeValidDirectoryNameInplace(::std::string &dir)
   {
    makeValidDirectoryName(dir).swap(dir);
   }

//-----------------------------------------------------------------------------
inline
void makeValidDirectoryNameInplace(::std::wstring &dir)
   {
    makeValidDirectoryName(dir).swap(dir);
   }

#define stripSlashInplace  pathRemoveTrailingSlashInplace 
//#define stripSlash         pathRemoveTrailingSlash
#define stripSlashCopy     pathRemoveTrailingSlash

/*
//-----------------------------------------------------------------------------
template <typename Iter, typename Pred>
void stripSlash(const ::std::pair<Iter, Iter> &dir,
                ::std::pair<Iter, Iter> &dstName,
                Pred pathPred)
   {
    dstName = dir;
    if (dstName.first==dstName.second) return;
    --dstName.second;

    if (!pathPred(*dstName.second))
       dstName.second = dir.second;
   }

//-----------------------------------------------------------------------------
inline
tstring stripSlashCopy(const tstring &dir, const TCHAR* pathSeparators = PATH_SEPARATORS)
   {
    ::std::pair<const_iter_t, const_iter_t> dirPair;
    stripSlash(::std::make_pair(dir.begin(), dir.end()), dirPair, isPathSeparator<TCHAR>(pathSeparators));
    return tstring(dirPair.first, dirPair.second);
   }

//-----------------------------------------------------------------------------
inline
void stripSlashInplace(tstring &dir, const TCHAR* pathSeparators = PATH_SEPARATORS)
   {
    ::std::pair<iter_t, iter_t> dirPair;
    stripSlash(::std::make_pair(dir.begin(), dir.end()), dirPair, isPathSeparator<TCHAR>(pathSeparators));
    dir.erase(dirPair.second, dir.end());
   }


                   //const ::std::vector< ::std::pair<Iter, Iter>, ::std::allocator< ::std::pair<Iter, Iter> > > &srcPathParts,

*/


//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
bool subPath( ::std::basic_string<CharType, Traits, Allocator> &resSubPath 
            , const ::std::basic_string<CharType, Traits, Allocator> &pathRoot 
            , const ::std::basic_string<CharType, Traits, Allocator> &fullPath
            , const CharType* pathSeparators  /*  = PATH_SEPARATORS */
            , CharType pathPrefferedSeparator  /* = PATH_PREFFERED_SEPARATOR */ 
             #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE) /* FILENAMES_ARE_CASE_INSENSITIVE */
            , const ::std::locale &loc = MARTY_NS filename::utils::makeCurrentLocale()
             #endif
           )
   {
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
    ::std::basic_string<CharType, Traits, Allocator> canonPathRoot = utils::lowerCase(MARTY_NS filename::makeCanonical(pathRoot, pathSeparators, pathPrefferedSeparator), loc);
    ::std::basic_string<CharType, Traits, Allocator> canonFullPath = utils::lowerCase(MARTY_NS filename::makeCanonical(fullPath, pathSeparators, pathPrefferedSeparator), loc);
    #else
    ::std::basic_string<CharType, Traits, Allocator> canonPathRoot = MARTY_NS filename::makeCanonical(pathRoot, pathSeparators, pathPrefferedSeparator);
    ::std::basic_string<CharType, Traits, Allocator> canonFullPath = MARTY_NS filename::makeCanonical(fullPath, pathSeparators, pathPrefferedSeparator);
    #endif

    canonPathRoot = appendPathSep(canonPathRoot, pathSeparators, pathPrefferedSeparator);
    if (canonFullPath.compare(0, canonPathRoot.size(), canonPathRoot, 0, canonPathRoot.size()))
       { // not match
        //resSubPath
        return false;
       }

    //resSubPath = ::std::basic_string<CharType, Traits, Allocator>(canonFullPath, canonPathRoot.size());
    resSubPath = ::std::basic_string<CharType, Traits, Allocator>(fullPath, canonPathRoot.size());
    return true;
   }

//-----------------------------------------------------------------------------
inline
bool subPath( ::std::string &resSubPath
            , const ::std::string &pathRoot
            , const ::std::string &fullPath
            )
   {
    return subPath( resSubPath
                  , pathRoot
                  , fullPath
                  , C_PATH_SEPARATORS
                  , C_PATH_PREFFERED_SEPARATOR
                  #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE) /* FILENAMES_ARE_CASE_INSENSITIVE */
                  , utils::makeCurrentLocale()
                  #endif
                  );
   }

//-----------------------------------------------------------------------------
inline
bool subPath( ::std::wstring &resSubPath
            , const ::std::wstring &pathRoot
            , const ::std::wstring &fullPath
            )
   {
    return subPath( resSubPath
                  , pathRoot
                  , fullPath
                  , W_PATH_SEPARATORS
                  , W_PATH_PREFFERED_SEPARATOR
                  #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE) /* FILENAMES_ARE_CASE_INSENSITIVE */
                  , utils::makeCurrentLocale()
                  #endif
                  );
   }

//-----------------------------------------------------------------------------
inline
bool subPathStrict(::std::string &resSubPath, 
                   const ::std::string &pathRoot, 
                   const ::std::string &fullPath
                  )
   {
   return subPath( resSubPath
                 , pathRoot
                 , fullPath
                 , C_PATH_SEPARATORS_STRICT
                 , C_PATH_PREFFERED_SEPARATOR_STRICT
                 #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE) /* FILENAMES_ARE_CASE_INSENSITIVE */
                 , utils::makeCurrentLocale()
                 #endif
                 );
   }

//-----------------------------------------------------------------------------
inline
bool subPathStrict(::std::wstring &resSubPath, 
                   const ::std::wstring &pathRoot, 
                   const ::std::wstring &fullPath
                  )
   {
   return subPath( resSubPath
                 , pathRoot
                 , fullPath
                 , W_PATH_SEPARATORS_STRICT
                 , W_PATH_PREFFERED_SEPARATOR_STRICT
                 #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE) /* FILENAMES_ARE_CASE_INSENSITIVE */
                 , utils::makeCurrentLocale()
                 #endif
                 );
   }


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
   
#ifdef FILENAME_USE_TEST
namespace test
{
inline int makeCanonicalTestSuite(int *pTotalTests)
   {
    genericTestCaseStruct1 tests[] = {
                                     { _T("c:\\winnt\\system32/drivers\\etc/hosts"), _T("c:\\winnt\\system32\\drivers\\etc\\hosts") },
                                     { _T("c:\\winnt\\system32/.\\drivers\\etc/hosts"), _T("c:\\winnt\\system32\\drivers\\etc\\hosts") },
                                     { _T("c:\\winnt\\..\\winnt\\system32/..\\.\\system32/drivers\\etc/hosts"), _T("c:\\winnt\\system32\\drivers\\etc\\hosts") },
                                     { _T(".\\winnt\\system32/drivers\\etc/hosts"), _T(".\\winnt\\system32\\drivers\\etc\\hosts") },
                                     { _T("\\\\server\\winnt\\./system32/drivers\\etc/hosts"), _T("\\\\server\\winnt\\system32\\drivers\\etc\\hosts") },
                                     { _T("\\\\server\\../winnt\\./system32/drivers\\etc/hosts"), _T("\\\\server\\..\\winnt\\system32\\drivers\\etc\\hosts") },

                                     //{ _T(""), _T("") },
                                     { 0, 0 }
                                    };
    return MARTY_NS filename::test::genericTestSuite(tests, pTotalTests, _T("makeCanonical"), MARTY_NS filename::makeCanonical);
   }
}; // namespace test
#endif
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------




#ifdef FILENAME_OLD_NAMES
inline
bool isDotDirectory ( const tstring& dir )
   {
    return dir==_T(".");
   }

//-----------------------------------------------------------------------------
inline
tstring canonical(const tstring &filename,
                  const TCHAR*  pathSeparators = PATH_SEPARATORS,
                  TCHAR pathPrefferedSeparator = PATH_PREFFERED_SEPARATOR )
   {
    std::vector<tstring> pathParts;

    splitPath(filename, pathParts, pathSeparators);

    std::vector<tstring>::iterator end = std::remove_if ( 
                                              pathParts.begin(), 
                                              pathParts.end(), 
                                              isDotDirectory 
                                                        );
    std::vector<tstring> resVec;
    resVec.reserve(pathParts.size());

    std::vector<tstring>::iterator it = pathParts.begin();
    for(;it!=end; ++it)
       {
        if (*it==_T(".."))
           { // go one level up
            if (resVec.empty()) 
               resVec.push_back(*it); // possible error, but we ignoring it
            else if (resVec.size()>1) 
               {
                if (resVec.back()==_T(".."))
                   resVec.push_back(*it); // possible error, but we ignoring it
                else
                   resVec.pop_back(); // Ok
               }
            else if (isAbsolutePath(resVec[0], pathSeparators))
               resVec.push_back(*it); // possible error, but we ignoring it
            else
               {
                if (resVec.back()==_T(".."))
                   resVec.push_back(*it); // possible error, but we ignoring it
                else
                   resVec.pop_back(); // Ok
               }
           }
        else
           resVec.push_back(*it);       
       }

    tstring res;
    res.reserve(filename.size());

    it = resVec.begin();
    for(;it!=resVec.end(); ++it)
       {
        if (!res.empty()) res.append(1, pathPrefferedSeparator);
        res.append(*it);
       }

    return res;
   }
#endif /* FILENAME_OLD_NAMES*/
//-----------------------------------------------------------------------------



typedef bool (*compareProcType)( const tstring &n1, const tstring &n2,
                                 #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
                                 const ::std::locale &loc,
                                 #endif
                                 const TCHAR* pathSeparators,
                                 TCHAR pathPrefferedSeparator);

typedef bool (*compareProcTypeStrict)( const tstring &n1, const tstring &n2,
                                 const TCHAR* pathSeparators);


inline
bool equal( const tstring &name1, const tstring &name2,
            #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
            const ::std::locale &loc = MARTY_NS filename::utils::makeCurrentLocale(),
            #endif
            const TCHAR* pathSeparators = PATH_SEPARATORS,
            TCHAR pathPrefferedSeparator = PATH_PREFFERED_SEPARATOR )
   {
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
     return utils::lowerCase(MARTY_NS filename::makeCanonical(name1, pathSeparators, pathPrefferedSeparator), loc)
         == utils::lowerCase(MARTY_NS filename::makeCanonical(name2, pathSeparators, pathPrefferedSeparator), loc);
    #else
     return MARTY_NS filename::makeCanonical(name1, pathSeparators, pathPrefferedSeparator)
         == MARTY_NS filename::makeCanonical(name2, pathSeparators, pathPrefferedSeparator);
    #endif
   }

inline
bool equalStrict( const tstring &name1, const tstring &name2 )
   {
     return MARTY_NS filename::makeCanonical(name1, PATH_SEPARATORS_STRICT, PATH_PREFFERED_SEPARATOR_STRICT)
         == MARTY_NS filename::makeCanonical(name2, PATH_SEPARATORS_STRICT, PATH_PREFFERED_SEPARATOR_STRICT);
   }

struct predEqual{

    const TCHAR* pathSeparators;
    TCHAR pathPrefferedSeparator;
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
    const ::std::locale loc;
    #endif

    predEqual(
              #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
              const ::std::locale l = MARTY_NS filename::utils::makeCurrentLocale(),
              #endif
              const TCHAR* ps = PATH_SEPARATORS,
              TCHAR pps = PATH_PREFFERED_SEPARATOR
             )
    : pathSeparators(ps)
    , pathPrefferedSeparator(pps)
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
    , loc(l)
    #endif
    {}

    predEqual( const predEqual &p)
    : pathSeparators(p.pathSeparators)
    , pathPrefferedSeparator(p.pathPrefferedSeparator)
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
    , loc(p.loc)
    #endif
    {}

    bool operator()(const tstring &name1, const tstring &name2) const
       {
        #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
        return MARTY_NS filename::equal(name1, name2, loc, pathSeparators, pathPrefferedSeparator);
        #else
        return MARTY_NS filename::equal(name1, name2, pathSeparators, pathPrefferedSeparator);
        #endif
       }
};

struct predEqualStrict{

    bool operator()(const tstring &name1, const tstring &name2) const
       {
        return equalStrict(name1, name2);
       }
};

//-----------------------------------------------------------------------------
inline
bool less( const tstring &name1, const tstring &name2,
           #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
           const ::std::locale &loc = MARTY_NS filename::utils::makeCurrentLocale(),
           #endif
           const TCHAR*  pathSeparators = PATH_SEPARATORS,
           TCHAR pathPrefferedSeparator = PATH_PREFFERED_SEPARATOR )
   {
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
     return utils::lowerCase(MARTY_NS filename::makeCanonical(name1, pathSeparators, pathPrefferedSeparator), loc)
          < utils::lowerCase(MARTY_NS filename::makeCanonical(name2, pathSeparators, pathPrefferedSeparator), loc);
    #else
     return MARTY_NS filename::makeCanonical(name1, pathSeparators, pathPrefferedSeparator)
          < MARTY_NS filename::makeCanonical(name2, pathSeparators, pathPrefferedSeparator);
    #endif
   }

inline
bool lessStrict( const tstring &name1, const tstring &name2 )
   {
     return MARTY_NS filename::makeCanonical(name1, PATH_SEPARATORS_STRICT, PATH_PREFFERED_SEPARATOR_STRICT)
          < MARTY_NS filename::makeCanonical(name2, PATH_SEPARATORS_STRICT, PATH_PREFFERED_SEPARATOR_STRICT);
   }

struct predLess{

    const TCHAR* pathSeparators;
    TCHAR pathPrefferedSeparator;
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
    const ::std::locale loc;
    #endif

    predLess(
              #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
              const ::std::locale l = MARTY_NS filename::utils::makeCurrentLocale(),
              #endif
              const TCHAR* ps = PATH_SEPARATORS,
              TCHAR pps = PATH_PREFFERED_SEPARATOR
             )
    : pathSeparators(ps)
    , pathPrefferedSeparator(pps)
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
    , loc(l)
    #endif
    {}

    predLess( const predLess &p)
    : pathSeparators(p.pathSeparators)
    , pathPrefferedSeparator(p.pathPrefferedSeparator)
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
    , loc(p.loc)
    #endif
    {}

    bool operator()(const tstring &name1, const tstring &name2) const
       {
        #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
        return MARTY_NS filename::less(name1, name2, loc, pathSeparators, pathPrefferedSeparator);
        #else
        return MARTY_NS filename::less(name1, name2, pathSeparators, pathPrefferedSeparator);
        #endif
       }
};

struct predLessStrict{

    bool operator()(const tstring &name1, const tstring &name2) const
       {
        return lessStrict(name1, name2);
       }
};

//-----------------------------------------------------------------------------
inline
bool lessEqual( const tstring &name1, const tstring &name2,
                #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
                const ::std::locale &loc = MARTY_NS filename::utils::makeCurrentLocale(),
                #endif
                const TCHAR* pathSeparators = PATH_SEPARATORS,
                TCHAR pathPrefferedSeparator = PATH_PREFFERED_SEPARATOR )
   {
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
     return utils::lowerCase(MARTY_NS filename::makeCanonical(name1, pathSeparators, pathPrefferedSeparator), loc)
         <= utils::lowerCase(MARTY_NS filename::makeCanonical(name2, pathSeparators, pathPrefferedSeparator), loc);
    #else
     return MARTY_NS filename::makeCanonical(name1, pathSeparators, pathPrefferedSeparator)
         <= MARTY_NS filename::makeCanonical(name2, pathSeparators, pathPrefferedSeparator);
    #endif
   }

inline
bool lessEqualStrict( const tstring &name1, const tstring &name2 )
   {
     return MARTY_NS filename::makeCanonical(name1, PATH_SEPARATORS_STRICT, PATH_PREFFERED_SEPARATOR_STRICT)
         <= MARTY_NS filename::makeCanonical(name2, PATH_SEPARATORS_STRICT, PATH_PREFFERED_SEPARATOR_STRICT);
   }

struct predLessEqual{

    const TCHAR* pathSeparators;
    TCHAR pathPrefferedSeparator;
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
    const ::std::locale loc;
    #endif

    predLessEqual(
              #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
              const ::std::locale l = MARTY_NS filename::utils::makeCurrentLocale(),
              #endif
              const TCHAR* ps = PATH_SEPARATORS,
              TCHAR pps = PATH_PREFFERED_SEPARATOR
             )
    : pathSeparators(ps)
    , pathPrefferedSeparator(pps)
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
    , loc(l)
    #endif
    {}

    predLessEqual( const predLessEqual &p)
    : pathSeparators(p.pathSeparators)
    , pathPrefferedSeparator(p.pathPrefferedSeparator)
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
    , loc(p.loc)
    #endif
    {}

    bool operator()(const tstring &name1, const tstring &name2) const
       {
        #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
        return lessEqual(name1, name2, loc, pathSeparators, pathPrefferedSeparator);
        #else
        return lessEqual(name1, name2, pathSeparators, pathPrefferedSeparator);
        #endif
       }
};

struct predLessEqualStrict{

    bool operator()(const tstring &name1, const tstring &name2) const
       {
        return lessEqualStrict(name1, name2);
       }
};
//-----------------------------------------------------------------------------
inline
bool greater( const tstring &name1, const tstring &name2,
              #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
              const ::std::locale &loc = MARTY_NS filename::utils::makeCurrentLocale(),
              #endif
              const TCHAR* pathSeparators = PATH_SEPARATORS,
              TCHAR pathPrefferedSeparator = PATH_PREFFERED_SEPARATOR )
   {
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
     return utils::lowerCase(MARTY_NS filename::makeCanonical(name1, pathSeparators, pathPrefferedSeparator), loc)
          > utils::lowerCase(MARTY_NS filename::makeCanonical(name2, pathSeparators, pathPrefferedSeparator), loc);
    #else
     return MARTY_NS filename::makeCanonical(name1, pathSeparators, pathPrefferedSeparator)
          > MARTY_NS filename::makeCanonical(name2, pathSeparators, pathPrefferedSeparator);
    #endif
   }

inline
bool greaterStrict( const tstring &name1, const tstring &name2 )
   {
     return MARTY_NS filename::makeCanonical(name1, PATH_SEPARATORS_STRICT, PATH_PREFFERED_SEPARATOR_STRICT)
          > MARTY_NS filename::makeCanonical(name2, PATH_SEPARATORS_STRICT, PATH_PREFFERED_SEPARATOR_STRICT);
   }

struct predGreater{

    const TCHAR* pathSeparators;
    TCHAR pathPrefferedSeparator;
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
    const ::std::locale loc;
    #endif

    predGreater(
              #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
              const ::std::locale l = MARTY_NS filename::utils::makeCurrentLocale(),
              #endif
              const TCHAR* ps = PATH_SEPARATORS,
              TCHAR pps = PATH_PREFFERED_SEPARATOR
             )
    : pathSeparators(ps)
    , pathPrefferedSeparator(pps)
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
    , loc(l)
    #endif
    {}

    predGreater( const predGreater &p)
    : pathSeparators(p.pathSeparators)
    , pathPrefferedSeparator(p.pathPrefferedSeparator)
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
    , loc(p.loc)
    #endif
    {}

    bool operator()(const tstring &name1, const tstring &name2) const
       {
        #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
        return MARTY_NS filename::greater(name1, name2, loc, pathSeparators, pathPrefferedSeparator);
        #else
        return MARTY_NS filename::greater(name1, name2, pathSeparators, pathPrefferedSeparator);
        #endif
       }
};

struct predGreaterStrict{

    bool operator()(const tstring &name1, const tstring &name2) const
       {
        return greaterStrict(name1, name2);
       }
};

//-----------------------------------------------------------------------------
inline
bool greaterEqual( const tstring &name1, const tstring &name2,
                   #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
                   const ::std::locale &loc = MARTY_NS filename::utils::makeCurrentLocale(),
                   #endif
                   const TCHAR* pathSeparators = PATH_SEPARATORS,
                   TCHAR pathPrefferedSeparator = PATH_PREFFERED_SEPARATOR )
   {
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
     return utils::lowerCase(MARTY_NS filename::makeCanonical(name1, pathSeparators, pathPrefferedSeparator), loc)
         >= utils::lowerCase(MARTY_NS filename::makeCanonical(name2, pathSeparators, pathPrefferedSeparator), loc);
    #else
     return MARTY_NS filename::makeCanonical(name1, pathSeparators, pathPrefferedSeparator)
         >= MARTY_NS filename::makeCanonical(name2, pathSeparators, pathPrefferedSeparator);
    #endif
   }

inline
bool greaterEqualStrict( const tstring &name1, const tstring &name2 )
   {
     return MARTY_NS filename::makeCanonical(name1, PATH_SEPARATORS_STRICT, PATH_PREFFERED_SEPARATOR_STRICT)
         >= MARTY_NS filename::makeCanonical(name2, PATH_SEPARATORS_STRICT, PATH_PREFFERED_SEPARATOR_STRICT);
   }

struct predGreaterEqual{

    const TCHAR* pathSeparators;
    TCHAR pathPrefferedSeparator;
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
    const ::std::locale loc;
    #endif

    predGreaterEqual(
              #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
              const ::std::locale l = MARTY_NS filename::utils::makeCurrentLocale(),
              #endif
              const TCHAR* ps = PATH_SEPARATORS,
              TCHAR pps = PATH_PREFFERED_SEPARATOR
             )
    : pathSeparators(ps)
    , pathPrefferedSeparator(pps)
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
    , loc(l)
    #endif
    {}

    predGreaterEqual( const predGreaterEqual &p)
    : pathSeparators(p.pathSeparators)
    , pathPrefferedSeparator(p.pathPrefferedSeparator)
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
    , loc(p.loc)
    #endif
    {}

    bool operator()(const tstring &name1, const tstring &name2) const
       {
        #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
        return greaterEqual(name1, name2, loc, pathSeparators, pathPrefferedSeparator);
        #else
        return greaterEqual(name1, name2, pathSeparators, pathPrefferedSeparator);
        #endif
       }
};

struct predGreaterEqualStrict{

    bool operator()(const tstring &name1, const tstring &name2) const
       {
        return greaterEqualStrict(name1, name2);
       }
};



/* //-----------------------------------------------------------------------------
 * struct equalTo
 * {
 *     tstring name;
 *     tstring pathSeparators;
 *     TCHAR   pathPrefferedSeparator;
 *  
 *     equalTo(const tstring &name,
 *             const tstring &pathSeparators = PATH_SEPARATORS,
 *             TCHAR pathPrefferedSeparator = PATH_PREFFERED_SEPARATOR)
 *        : name(name)
 *        , pathSeparators(pathSeparators)
 *        , pathPrefferedSeparator(pathPrefferedSeparator)
 *        {}
 *  
 *     bool operator() (const tstring &cmpName)
 *        {
 *         return equal( name, cmpName, pathSeparators, pathPrefferedSeparator);
 *        }
 * };
 */

//-----------------------------------------------------------------------------

template <typename TCompare>
struct compareTo
{
    tstring  name;
    const TCHAR* pathSeparators;
    TCHAR    pathPrefferedSeparator;
    TCompare compare;
    #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
    ::std::locale loc;
    #endif
 
    compareTo(const tstring &name,
              TCompare       cmp,
              #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
              ::std::locale l = MARTY_NS filename::utils::makeCurrentLocale(),
              #endif
              const TCHAR* pathSeparators = PATH_SEPARATORS,
              TCHAR pathPrefferedSeparator = PATH_PREFFERED_SEPARATOR)
       : name(name)
       , pathSeparators(pathSeparators)
       , pathPrefferedSeparator(pathPrefferedSeparator)
       , compare(cmp)
       #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
       , loc(l)
       #endif
       {}
 
    bool operator() (const tstring &cmpName) const
       {
        return compare( name, 
                        cmpName, 
                        #if defined(FILENAMES_ARE_CASE_INSENSITIVE) || defined(FILENAME_COMPARISON_ALWAYS_USE_LOCALE)
                        loc,
                        #endif
                        pathSeparators, 
                        pathPrefferedSeparator
                      );
       }
};


template <typename TContainer>
struct null_insert_iterator
{
    null_insert_iterator() {}

    null_insert_iterator<TContainer>& operator=( typename TContainer::const_reference _Val )
       {
        return *this;
       }
    null_insert_iterator<TContainer>& operator*( )
       {
        return *this;
       }
    null_insert_iterator<TContainer>& operator++( )
       {
        return *this;
       }
    null_insert_iterator<TContainer>& operator++( int )
       {
        return *this;
       }

};

template <typename TContainer>
null_insert_iterator<TContainer> null_inserter( TContainer &container )
   {
    return null_insert_iterator<TContainer>();
   }

template <typename Iter>
struct CMatchPair
{
    bool isMask; // text or mask (text matched mask symbol) fragment
    bool fQuantity; // false - ?, true - *, no mean for plain text
    Iter begin;
    Iter end;

    CMatchPair(bool isM, Iter b, Iter e, bool fq = false) : isMask(isM), fQuantity(fq), begin(b), end(e) {}

    // mask: "aaa*bbb?ccc"
    // str : "aaa111bbb2ccc"
    //         ^  ^  ^ ^ ^
    //         1  2  3 4 5
    // 1 - source text
    // 2 - source text matched *
    // 3 - source text
    // 4 - source text matched ?
    // 5 - source text
};



//-----------------------------------------------------------------------------
template <typename Iter, typename Comparator, typename CharType>
bool matchMask(::std::pair<Iter, Iter> name, 
               ::std::pair<Iter, Iter> mask,
               //const ChangeCase &changeCase,
               const Comparator &compare,
               CharType mult,
               CharType quest
              )
   {
    while(name.first!=name.second && mask.first!=mask.second)
       {
        if (compare(*mask.first,mult))
           {
            //while(mask.first!=mask.second && *mask.first==mult)
            while(mask.first!=mask.second && compare(*mask.first,mult) )
               ++mask.first; // skip '*'
            if (mask.first==mask.second) return true;
            for(; name.first!=name.second; ++name.first)
               {
                if ( matchMask( ::std::make_pair(name.first, name.second)
                              , ::std::make_pair(mask.first, mask.second)
                              , compare /* changeCase */ , mult, quest
                              )
                   ) return true;
               }
            return false;           
           }
        else if (compare(*mask.first,quest))
           {
            // simple skip character
            ++name.first; ++mask.first;
           }
        else
           {
            if (!compare(*name.first, *mask.first))
               return false;
            ++name.first; ++mask.first;
           }
       }

    if (name.first==name.second && mask.first!=mask.second)
       {
        while(mask.first!=mask.second && compare(*mask.first,mult) )
           ++mask.first; // skip trailing '*'
       }

    if (name.first!=name.second || mask.first!=mask.second)
       return false;

    return true;
   }
//-----------------------------------------------------------------------------
template <typename Iter, typename Comparator>
::std::pair<Iter, Iter> findSubstring( ::std::pair<Iter, Iter> text, ::std::pair<Iter, Iter> substring, const Comparator &compare)
   { // can be optimized if Iter is random access iterator, read Knutt's 'Sort and search'
    Iter i = text.first;
    for(; i!=text.second; ++i)
       {
        bool noMatch = false;
        Iter iTmp = i;
        Iter s = substring.first;
        for(; iTmp!=text.second && s!=substring.second; ++s, ++iTmp)
           {
            if (!compare(*iTmp, *s)) 
               {
                noMatch = true;
                break; // not match
               }
           }
        if (!noMatch)
           return ::std::make_pair( i, iTmp );
       }
    return ::std::make_pair( i, i ); // not found
   }


// for testing
template < typename CharType, typename Traits, typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator>&
buildMatchesString( const ::std::vector< CMatchPair< typename ::std::basic_string<CharType, Traits, Allocator> :: const_iterator > > &matches
                  , ::std::basic_string<CharType, Traits, Allocator> &resStr
                  )
   {
    // GCC workaround
    typedef ::std::basic_string<CharType, Traits, Allocator> string_type;
    typedef typename string_type :: const_iterator           string_type_const_iterator;
    typedef CMatchPair< string_type_const_iterator >         match_pair_type;
    typedef ::std::vector< match_pair_type >                 match_pair_vector;
    typedef typename match_pair_vector :: const_iterator     match_pair_vector_const_iterator;
    //::std::basic_string<CharType, Traits, Allocator> resStr;
    match_pair_vector_const_iterator mit = matches.begin();
    for(; mit!=matches.end(); ++mit)
       {
        resStr.append( 1, (CharType)'(');
        if (mit->isMask)
            resStr.append( 1, (CharType)(mit->fQuantity ? '*' : '?'));
        resStr.append( mit->begin, mit->end );
        resStr.append( 1, (CharType)')');
       }
    return resStr;
   }


//-----------------------------------------------------------------------------
template < typename Iter
         , typename Comparator
         , typename CharType
         , typename Inserter
         >
void buildMaskTestString( ::std::pair<Iter, Iter> mask
                        , const Comparator &compare
                        , CharType mult
                        , CharType quest
                        , Inserter ins
                        )
   {
    size_t multNo = 0;
    static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234567890#=!";
    for(; mask.first!=mask.second; ++mask.first)
       {
        if (compare(*mask.first,mult))
           {
            CharType ch = (CharType)alphabet[multNo++];
            multNo &= 0x3F;
            for(int i=0; i!=3; ++i)
               *ins++ = ch;
           }
        else if (compare(*mask.first,quest))
           {
            *ins++ = (CharType)'N';
           }
        else
           {
            *ins++ = *mask.first;
           }
       }
   }

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
template < typename CharType, typename Traits, typename Allocator
         , typename Comparator
         , typename Inserter
        >
void buildMaskTestString( const ::std::basic_string<CharType, Traits, Allocator> &mask
                        , const Comparator &compare
                        , CharType mult
                        , CharType quest
                        , Inserter ins
                        )
   {
    buildMaskTestString( ::std::make_pair(mask.begin(), mask.end())
                       , compare, mult, quest, ins
                       );
   }

/*
template < typename CharType, typename Traits, typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator>
buildMaskTestString
*/

// ms-help://MS.MSDNQTR.v80.en/MS.MSDN.v80/MS.VisualStudio.v80.en/dv_vclang/html/cb13c1cc-186a-45bc-bee7-95a8de7381cc.htm
#if defined(FILENAME_MATCHMASK_FORCE_OPTIMIZATION)
    #include <marty/optimizeSpeed.h>
#endif

//-----------------------------------------------------------------------------
template <typename Iter, typename Comparator, typename CharType>
bool matchMaskExAux( ::std::pair<Iter, Iter> name
                   , ::std::pair<Iter, Iter> mask
                   , const Comparator &compare
                   , CharType mult
                   , CharType quest
                   , ::std::vector< CMatchPair<Iter> > &matches
                   )
   {
    //Iter 
    Iter textStartIt = name.first;
    while(name.first!=name.second && mask.first!=mask.second)
       {
        if (compare(*mask.first,mult))
           {
            //Iter 
            //while(mask.first!=mask.second && *mask.first==mult)
            while(mask.first!=mask.second && compare(*mask.first,mult) )
               ++mask.first; // skip '*'

            if (textStartIt!=name.first) 
               {
                matches.push_back( CMatchPair<Iter>(false, textStartIt, name.first) );
                textStartIt = name.first;
               }

            if (mask.first==mask.second) // trailing '*' found
               { // put all ellapsed text into matches
                matches.push_back( CMatchPair<Iter>(true, name.first, name.second, true) );
                return true; 
               }

            //Iter textStartIt = name.first;
            for(; name.first!=name.second; ++name.first)
               {
                ::std::vector< CMatchPair<Iter> > matchesTmp = matches;
                //matchesTmp.push_back( CMatchPair<Iter>(false, textStartIt, name.first) );
                //if (textStartIt!=name.first)
                   matchesTmp.push_back( CMatchPair<Iter>(true, textStartIt, name.first, true) );

                if ( matchMaskExAux( ::std::make_pair(name.first, name.second)
                                   , ::std::make_pair(mask.first, mask.second)
                                   , compare /* changeCase */ , mult, quest, matchesTmp
                                   )
                   )
                   {
                    matchesTmp.swap(matches);
                    return true;
                   }
               }
            return false;           
           }
        else if (compare(*mask.first,quest))
           {
            // simple skip character

            // add text before '?'
            if (textStartIt!=name.first) 
               matches.push_back( CMatchPair<Iter>(false, textStartIt, name.first) );
            textStartIt = name.first++;
            //++name.first; 
            ++mask.first;
            // add matched text
            matches.push_back( CMatchPair<Iter>(true, textStartIt, name.first) );
            textStartIt = name.first;
           }
        else
           {
            if (!compare(*name.first, *mask.first))
               return false;
            ++name.first; ++mask.first;
           }
       }

    if (mask.first==mask.second && name.first!=name.second)
       { // end of mask reached, but end of text not reached
        return false;
       }

    if (name.first==name.second)
       { // end of text reached
        if (textStartIt!=name.first)
           {
            matches.push_back( CMatchPair<Iter>(false, textStartIt, name.first) );
            textStartIt = name.first;
           }
       }

    bool hasTrailingMult = false;
    while(mask.first!=mask.second)
       {
        if (!compare(*mask.first,mult)) return false;
        ++mask.first;
        hasTrailingMult = true;
       }

    if ( hasTrailingMult)
       {
        matches.push_back( CMatchPair<Iter>(hasTrailingMult, textStartIt, name.first, hasTrailingMult) );
       }

    return true;
   }


//-----------------------------------------------------------------------------
template < typename Iter
         , typename Comparator
         , typename CharType
         , typename Inserter
         >
bool matchMaskEx(::std::pair<Iter, Iter> name 
                , ::std::pair<Iter, Iter> mask
                , const Comparator &compare
                , CharType mult
                , CharType quest
                , Inserter ins
                )
    {
     // GCC workaround
     typedef CMatchPair<Iter> match_pair_iter;
     typedef ::std::vector< match_pair_iter > match_pair_iter_vector;
     typedef typename match_pair_iter_vector::const_iterator match_pair_iter_vector_const_iterator;

     match_pair_iter_vector matches;
     bool res = matchMaskExAux( name, mask, compare, mult, quest, matches );
     if (!res) return res;
     match_pair_iter_vector_const_iterator mit = matches.begin();
     for(; mit!=matches.end(); ++mit)
        {
         *ins++ = *mit;
        }
     return true;
    }

//-----------------------------------------------------------------------------
template < typename Iter
         , typename Comparator
         , typename CharType
         , typename Inserter
         >
bool matchMaskEx2(::std::pair<Iter, Iter> name 
                , ::std::pair<Iter, Iter> mask
                , const Comparator &compare
                , CharType mult
                , CharType quest
                , Inserter insAll
                , Inserter insMask
                , Inserter insText
                )
    {
     // GCC workaround
     typedef CMatchPair<Iter> match_pair_iter;
     typedef ::std::vector< match_pair_iter > match_pair_iter_vector;
     typedef typename match_pair_iter_vector::const_iterator match_pair_iter_vector_const_iterator;

     match_pair_iter_vector matches;
     bool res = matchMaskExAux( name, mask, compare, mult, quest, matches );
     if (!res) return res;
     match_pair_iter_vector_const_iterator mit = matches.begin();
     for(; mit!=matches.end(); ++mit)
        {
         *insAll++ = *mit;
         if (mit->isMask)
            *insMask++ = *mit;
         else
            *insText++ = *mit;
        }
     return true;
    }


//-----------------------------------------------------------------------------
template < typename CharType, typename Traits, typename Allocator
         , typename Comparator
         , typename Inserter
        >
bool matchMaskEx( const ::std::basic_string<CharType, Traits, Allocator> &name
                , const ::std::basic_string<CharType, Traits, Allocator> &mask
                , const Comparator &compare
                , CharType mult
                , CharType quest
                , Inserter ins
                )
    {
     return matchMaskEx( ::std::make_pair(name.begin(), name.end())
                       , ::std::make_pair(mask.begin(), mask.end())
                       , compare, mult, quest, ins
                       );
    }

//-----------------------------------------------------------------------------
template < typename CharType, typename Traits, typename Allocator
         , typename Comparator
         , typename Inserter
        >
bool matchMaskEx2( const ::std::basic_string<CharType, Traits, Allocator> &name
                , const ::std::basic_string<CharType, Traits, Allocator> &mask
                , const Comparator &compare
                , CharType mult
                , CharType quest
                , Inserter insAll
                , Inserter insMask
                , Inserter insText
                )
    {
     return matchMaskEx2( ::std::make_pair(name.begin(), name.end())
                       , ::std::make_pair(mask.begin(), mask.end())
                       , compare, mult, quest, insAll, insMask, insText
                       );
    }


inline
unsigned char2dec(char ch)
   {
    if (ch>='0' && ch<='9') return ch-'0';
    return 10;
   }

inline
unsigned char2dec(wchar_t ch)
   {
    if (ch>=L'0' && ch<=L'9') return ch-L'0';
    return 10;
   }

template < typename Iter
         , typename CharType
         , typename Inserter
         >
void buildMatchSubstImpl( ::std::pair<Iter, Iter> substStr
                        , const std::vector< CMatchPair< Iter > > &matches
                        , const std::vector< CMatchPair< Iter > > &matchesMask
                        , const std::vector< CMatchPair< Iter > > &matchesText
                        , CharType metaRefChar // usually $, zero if no metarefs used
                        , CharType textRefChar // usually #, zero if no textrefs used
                        , CharType allRefChar  // usually %, zero if no textrefs used
                        , CharType lb // left bracket '('
                        , CharType rb // right bracket ')'
                        , Inserter ins // ::std::back_inserter( string )
                        )
   {
    Iter i   = substStr.first;
    Iter end = substStr.second;
    CharType refFound = 0;
    while(i!=end)
       {
        if (refFound)
           { // test next char is also same ref
            if (*i==refFound)
               { // dup ref char turns into single
                *ins++ = *i++;
                refFound = 0;
               }
            else // if (*i==refFound)
               { // found ref
                bool inBrackets = false;
                unsigned index = 0;
                if (*i==lb) // ref in brackets
                   { ++i; inBrackets = true; if (i==end) return; }

                if (inBrackets)
                   {
                    while(i!=end && *i!=rb)
                       {
                        if (char2dec(*i)<10)
                           { index *= 10; index += char2dec(*i++); }
                        else ++i;
                       }
                    if (i==end) return; // last bracket required
                    ++i;
                   }
                else
                   {
                    while(i!=end && char2dec(*i)<10)
                       {
                        index *= 10; index += char2dec(*i++);
                       }
                   }

                if (index)
                   {
                    --index;
                    const std::vector< CMatchPair< Iter > > &v 
                        = ( refFound==metaRefChar
                          ? matchesMask
                          : (refFound==textRefChar ? matchesText : matches)
                          );
                    if (index<v.size())
                       {
                        const CMatchPair< Iter > &mp = v[index];                        
                        for(Iter it = mp.begin; it!=mp.end; ++it )
                           {
                            *ins++ = *it;
                           }
                       } // if (index<v.size())
                   } // if (index)
               } // if (*i==refFound)
            refFound = 0;
           } 
        else // if (refFound)
           { // no prev ref found
            if (*i==metaRefChar)
               {
                if (metaRefChar)
                   refFound = *i++;
                else
                   *ins++ = *i++;
               }
            else if (*i==textRefChar)
               {
                if (textRefChar)
                   refFound = *i++;
                else
                   *ins++ = *i++;
               }
            else if (*i==allRefChar)
               {
                if (allRefChar)
                   refFound = *i++;
                else
                   *ins++ = *i++;
               }
            else
               {
                *ins++ = *i++;
               }
           } // // if (refFound)
       } // while(i!=end)
   }

inline
::std::string buildMatchSubst( const ::std::string &substStr
                             , const std::vector< CMatchPair< ::std::string::const_iterator > > &matches
                             , const std::vector< CMatchPair< ::std::string::const_iterator > > &matchesMask
                             , const std::vector< CMatchPair< ::std::string::const_iterator > > &matchesText
                             , char metaRefChar = '$' // usually $, zero if no metarefs used
                             , char textRefChar = '#' // usually #, zero if no textrefs used
                             , char allRefChar  = '%' // usually %, zero if no textrefs used
                             , char lb          = '(' // left bracket '('
                             , char rb          = ')' // right bracket ')'
                             )
   {
    ::std::string res;
    buildMatchSubstImpl( ::std::make_pair( substStr.begin(), substStr.end() )
                       , matches, matchesMask, matchesText
                       , metaRefChar, textRefChar, allRefChar, lb, rb
                       , ::std::back_inserter(res)
                       );
    return res;
   }

inline
::std::wstring buildMatchSubst( const ::std::wstring &substStr
                             , const std::vector< CMatchPair< ::std::wstring::const_iterator > > &matches
                             , const std::vector< CMatchPair< ::std::wstring::const_iterator > > &matchesMask
                             , const std::vector< CMatchPair< ::std::wstring::const_iterator > > &matchesText
                             , wchar_t metaRefChar = L'$' // usually $, zero if no metarefs used
                             , wchar_t textRefChar = L'#' // usually #, zero if no textrefs used
                             , wchar_t allRefChar  = L'%' // usually %, zero if no textrefs used
                             , wchar_t lb          = L'(' // left bracket '('
                             , wchar_t rb          = L')' // right bracket ')'
                             )
   {
    ::std::wstring res;
    buildMatchSubstImpl( ::std::make_pair( substStr.begin(), substStr.end() )
                       , matches, matchesMask, matchesText
                       , metaRefChar, textRefChar, allRefChar, lb, rb
                       , ::std::back_inserter(res)
                       );
    return res;
   }






//-----------------------------------------------------------------------------
template <typename CaseConverter>
struct CCaseCompare
{
    const CaseConverter &caseConverter;
    CCaseCompare( const CaseConverter &cc ) : caseConverter(cc) {}
    template <typename TC>
    bool operator()( TC ch1, TC ch2) const
       {
        return caseConverter(ch1)==caseConverter(ch2);
       }
};
//-----------------------------------------------------------------------------
inline
bool matchMaskE(const ::std::string &name, const ::std::string &mask)
   {
    // some optimiztions
    if (mask.empty() && name.empty()) return true;
    if (mask.size()==1 && mask[0]=='*') return true;
    return matchMask( ::std::make_pair(name.begin(), name.end())
                    , ::std::make_pair(mask.begin(), mask.end())
                    , CCaseCompare<utils::keepcaseChar>(utils::keepcaseChar())
                    , '*', '?'
                    );
   }

//-----------------------------------------------------------------------------
inline
bool matchMaskE(const ::std::wstring &name, const ::std::wstring &mask)
   {
    // some optimiztions
    if (mask.empty() && name.empty()) return true;
    if (mask.size()==1 && mask[0]==L'*') return true;
    return matchMask( ::std::make_pair(name.begin(), name.end())
                    , ::std::make_pair(mask.begin(), mask.end())
                    , CCaseCompare<utils::keepcaseChar>(utils::keepcaseChar())
                    , L'*', L'?'
                    );
   }

//-----------------------------------------------------------------------------
inline
bool matchMaskI(const ::std::string &name, const ::std::string &mask)
   {
    //std::cout<<"CP38\n";
    // some optimiztions
    if (mask.empty() && name.empty()) return true;
    if (mask.size()==1 && mask[0]=='*') return true;
    return matchMask( ::std::make_pair(name.begin(), name.end())
                    , ::std::make_pair(mask.begin(), mask.end())
                    , CCaseCompare<utils::lowercaseChar>(utils::lowercaseChar(utils::makeCurrentLocale()))
                    , '*', '?'
                    );
   }

//-----------------------------------------------------------------------------
inline
bool matchMaskI(const ::std::wstring &name, const ::std::wstring &mask)
   {
    // some optimiztions
    if (mask.empty() && name.empty()) return true;
    if (mask.size()==1 && mask[0]==L'*') return true;
    return matchMask( ::std::make_pair(name.begin(), name.end())
                    , ::std::make_pair(mask.begin(), mask.end())
                    , CCaseCompare<utils::lowercaseChar>(utils::lowercaseChar(utils::makeCurrentLocale()))
                    , L'*', L'?'
                    );
   }

//-----------------------------------------------------------------------------
inline
bool matchMask(const ::std::string &name, const ::std::string &mask)
   {
    return 
           #ifdef FILENAMES_ARE_CASE_INSENSITIVE
           matchMaskI
           #else
           matchMaskE
           #endif
                    ( name, mask );
   }

//-----------------------------------------------------------------------------
inline
bool matchMask(const ::std::wstring &name, const ::std::wstring &mask)
   {
    return 
           #ifdef FILENAMES_ARE_CASE_INSENSITIVE
           matchMaskI
           #else
           matchMaskE
           #endif
                    ( name, mask );
   }

#if defined(FILENAME_MATCHMASK_FORCE_OPTIMIZATION)
    #include <marty/optimizeRestore.h>
#endif


//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#ifdef FILENAME_USE_TEST
namespace test
{

//-----------------------------------------------------------------------------
inline
bool matchMaskTest(const tstring &name, const tstring &mask, bool rightRes)
   {
    bool res = MARTY_NS filename::matchMask(name, mask);
    return (res==rightRes);
   }

//-----------------------------------------------------------------------------
// return num of failed tests
inline
int matchMaskTestSuite(int *pTotalTests /* pointer to var that incremented with total num of tests */)
   {
    genericTestCaseResStruct tests[] = {
                                     { "ab"           , "a*?b"     , false },
                                     { ".cxxp"        , ".c*p"     , true  },
                                     { ".information" , ".in*mat*" , true  },
                                     { ".inmat"       , ".in*mat*" , true  },
                                     { ".infwat"      , ".in*mat*" , false },
                                     { ".infwat"      , ".in*?at*" , true  },
                                     { ".cpp"         , ".cxx"     , false },
                                     { ".hpp"         , ".c??"     , false },
                                     { ".cpp"         , ".c??"     , true  },
                                     { ".cxx"         , ".c??"     , true  },
                                     { ".hpp"         , ".c*"      , false },
                                     { ".cpp"         , ".c*"      , true  },
                                     { ".cxx"         , ".c*"      , true  },
                                     { ".c"           , ".c*"      , true  },
                                     { "bla-bla"      , "*"        , true  },
                                     { "bla-bla"      , "*?"       , true  },
                                     { "bla-bla"      , "*?a"      , true  },
                                     { "bla-bla"      , "*?c"      , false },
                                     { "bla-bla"      , "bla-*?"   , true },
                                     { "bla-bla"      , "*?bla"    , true },
                                     { "bla-bla"      , "?*bla"    , true },
                                     { 0, 0, false }
                                    };
    return MARTY_NS filename::test::genericTestSuite(&tests[0], pTotalTests, _T("matchMask"), MARTY_NS filename::test::matchMaskTest);
   }

//-----------------------------------------------------------------------------
inline
tstring matchMaskExTest(const tstring &name, const tstring &mask)
   {
    std::vector< CMatchPair< ::std::string::const_iterator > > matches;
    if (!matchMaskEx( name, mask     
                    , CCaseCompare<utils::keepcaseChar>(utils::keepcaseChar())
                    , '*'
                    , '?'
                    , ::std::back_inserter(matches)
                    ) 
       ) return tstring();

    ::std::string matchesStr;
    return buildMatchesString(matches, matchesStr);
   }

inline
int matchMaskExTestSuite(int *pTotalTests /* pointer to var that incremented with total num of tests */)
   {
    genericTestCaseStruct2 tests[] = {
                                      { "aaa"          , "*"        , "(*aaa)" },
                                      { "aaa"          , "*?"       , "(*aa)(?a)" },
                                      { "aaa"          , "*?*"      , "(*)(?a)(*aa)" },
                                      { "aaa"          , "*?*?"     , "(*)(?a)(*a)(?a)" },
                                      { "aaa"          , "*???"     , "(*)(?a)(?a)(?a)" },
                                      { "aaa"          , "*??*"     , "(*)(?a)(?a)(*a)" },
                                      { "aaa"          , "?*?"      , "(?a)(*a)(?a)" },
                                      { "aaa"          , "?*?*"     , "(?a)(*)(?a)(*a)" },
                                      { "aaa"          , "???*"     , "(?a)(?a)(?a)(*)" },
                                      { "bbbbbb"       , "*?*?*?*"  , "(*)(?b)(*)(?b)(*)(?b)(*bbb)" },
                                      { "bbbbbb"       , "?*?*?*?"  , "(?b)(*)(?b)(*)(?b)(*bb)(?b)" },
                                      { "aa11bb2cc33dd4ee","aa*bb?cc*dd?ee","(aa)(*11)(bb)(?2)(cc)(*33)(dd)(?4)(ee)" },
                                      //{ ""       , "" , "" },
                                      { ".inmat"       , ".in*mat*" , "(.in)(*)(mat)(*)" },
                                      { ".information" , ".in*mat*" , "(.in)(*for)(mat)(*ion)" },
                                      { "bla-bla"      , "*?a"      , "(*bla-b)(?l)(a)" },
                                      { "bla-bla"      , "*?"       , "(*bla-bl)(?a)" },
                                      { ".infwat"      , ".in*?at*" , "(.in)(*f)(?w)(at)(*)" },
                                      { "ab"           , "a*?b"     , "" },
                                      { ".cxxp"        , ".c*p"     , "(.c)(*xx)(p)" },
                                      { ".infwat"      , ".in*mat*" , "" },
                                      { ".cpp"         , ".cxx"     , "" },
                                      { ".hpp"         , ".c??"     , "" },
                                      { ".cpp"         , ".c??"     , "(.c)(?p)(?p)" },
                                      { ".cxx"         , ".c??"     , "(.c)(?x)(?x)" },
                                      { ".hpp"         , ".c*"      , "" },
                                      { ".cpp"         , ".c*"      , "(.c)(*pp)" },
                                      { ".cxx"         , ".c*"      , "(.c)(*xx)" },
                                      { ".c"           , ".c*"      , "(.c)(*)" },
                                      { "bla-bla"      , "*"        , "(*bla-bla)" },
                                      { "bla-bla"      , "*?c"      , "" },
                                      { "bla-bla"      , "bla-*?"   , "(bla-)(*bl)(?a)" },
                                      { "bla-bla"      , "*?bla"    , "(*bla)(?-)(bla)" },
                                      { "bla-bla"      , "?*bla"    , "(?b)(*la-)(bla)" },
                                      { 0, 0, 0 }
                                     };
    return MARTY_NS filename::test::genericTestSuite(&tests[0], pTotalTests, _T("matchMaskEx"), MARTY_NS filename::test::matchMaskExTest);
   }

}; // namespace test
#endif


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



}; // namespace filename

#ifdef FILENAME_IN_MARTY_NAMESPACE
}; // namespace marty
#endif


//#undef ISNPOS


#endif /* MARTY_FILENAME_H */

