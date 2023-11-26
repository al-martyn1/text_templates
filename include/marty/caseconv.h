/*
   Copyright (c) 2007-2009 Alex Martynov, amart at mail.ru
   This code can be used without any limitations.
   This code provided as is, use it on your own risk.
*/

#ifndef MARTY_CASECONV_H
#define MARTY_CASECONV_H


#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

#if !defined(_LOCALE_) && !defined(_STLP_LOCALE) && !defined(__STD_LOCALE__) && !defined(_CPP_LOCALE) && !defined(_GLIBCXX_LOCALE)
    #include <locale>
#endif

#ifdef WIN32
    #if !defined(_WINDOWS_)
        #include <windows.h>
    #endif

    #ifndef _WINNLS_
        #include <winnls.h>
    #endif
#endif


#if !defined(_ALGORITHM_) && !defined(_STLP_ALGORITHM) && !defined(__STD_ALGORITHM) && !defined(_CPP_ALGORITHM) && !defined(_GLIBCXX_ALGORITHM)
    #include <algorithm>
#endif

//#ifndef MARTY_FILENAME_H
//    #include <marty/filename.h>
//#endif


namespace marty{
namespace util{

// 1) in BCC 5.6 (BCB 6) we need to undef folowing macroses 
#ifdef __BORLANDC__
    #ifdef toupper
        #undef toupper
    #endif
    
    #ifdef tolower
        #undef tolower
    #endif
#endif

// 2) There is a bug in BCC 5.5 runtime - 
// lowercase char 'ya' (255) case not changed to 'YA' (159) in ANSI codepage
// workaround is to always use lowercase strings in path names comparison

#ifdef _WIN32

#ifdef _MSC_VER
    #pragma warning( push )
    #pragma warning( disable:4311 )
    #pragma warning( disable:4312 )
#endif

/*
template <typename CharType>
CharType win32CharUpper(CharType ch)
   { throw ::std::exception("win32CharUpper must be specialized"); return ch; }

template <>
inline
char win32CharUpper<char>(char ch)
   { return (char)(unsigned char)(unsigned)CharUpperA((char*)(unsigned)(unsigned char)ch); }
   
template <>
inline
wchar_t win32CharUpper<wchar_t>(wchar_t ch)
   { return (wchar_t)(unsigned)CharUpperW((wchar_t*)(unsigned)ch); }
   
template <typename CharType>
CharType win32CharLower(CharType ch)
   { throw ::std::exception("win32CharLower must be specialized"); return ch; }

template <>
inline
char win32CharLower<char>(char ch)
   { return (char)(unsigned char)(unsigned)CharLowerA((char*)(unsigned)(unsigned char)ch); }
   
template <>
inline
wchar_t win32CharLower<wchar_t>(wchar_t ch)
   { return (wchar_t)(unsigned)CharLowerW((wchar_t*)(unsigned)ch); }
*/

struct win32CharUpper
{
     char operator()(char ch)
        {
         return (char)(unsigned char)(unsigned)CharUpperA((char*)(unsigned)(unsigned char)ch);
        }

     wchar_t operator()(wchar_t ch)
        { 
         return (wchar_t)(unsigned)CharUpperW((wchar_t*)(unsigned)ch);
        }
};

struct win32CharLower
{
     char operator()(char ch)
        {
         return (char)(unsigned char)(unsigned)CharLowerA((char*)(unsigned)(unsigned char)ch);
        }

     wchar_t operator()(wchar_t ch)
        { 
         return (wchar_t)(unsigned)CharLowerW((wchar_t*)(unsigned)ch);
        }
};

template <typename TCH>
TCH _win32CharLower(TCH ch)
   {
    return win32CharLower()(ch);
   }

template <typename TCH>
TCH _win32CharUpper(TCH ch)
   {
    return win32CharUpper()(ch);
   }


#ifdef _MSC_VER
    #pragma warning( pop )
#endif
   
#endif /* _WIN32 */



struct uppercaseChar
{
    private:
        const ::std::locale &loc;
        uppercaseChar& operator=(const uppercaseChar&) { return *this; }
    public:
        uppercaseChar(const ::std::locale &loc) : loc(loc) {}
        template<typename TC>
        TC operator()(TC c) const
           {
            #ifdef _WIN32
            // GCC does not support locales except C and POSIX, under windows we workaround this problem
            return _win32CharUpper(c);
            #else
                #ifdef __BORLANDC__
                    return ::std::toupper<TC>(c);
                #else
                    return ::std::toupper<TC>(c, loc);
                #endif
            #endif
           }
};


struct asciiUppercaseChar
{
    private:
        asciiUppercaseChar& operator=(const asciiUppercaseChar&) { return *this; }
    public:
        asciiUppercaseChar(){}
        char operator()(char ch)
           {
            if (ch>='a' && ch<='z') return ch - 'a' + 'A';
            return ch;
           }
        wchar_t operator()(wchar_t ch)
           {
            if (ch>=L'a' && ch<=L'z') return ch - L'a' + L'A';
            return ch;
           }
};



struct lowercaseChar
{
    private:
        const ::std::locale &loc;
        lowercaseChar& operator=(const lowercaseChar&) { return *this; }
    public:
        lowercaseChar(const ::std::locale &loc) : loc(loc) {}
        template<typename TC>
        TC operator()(TC c) const
           {
            #ifdef _WIN32
            // GCC does not support locales except C and POSIX, under windows we workaround this problem
            return _win32CharLower(c);
            #else
                #ifdef __BORLANDC__
                    return ::std::tolower<TC>(c);
                #else
                    return ::std::tolower<TC>(c, loc);
                #endif
            #endif
           }
};

struct asciiLowercaseChar
{
    private:
        asciiLowercaseChar& operator=(const asciiLowercaseChar&) { return *this; }
    public:
        asciiLowercaseChar(){}
        char operator()(char ch)
           {
            if (ch>='A' && ch<='Z') return ch - 'A' + 'a';
            return ch;
           }
        wchar_t operator()(wchar_t ch)
           {
            if (ch>=L'A' && ch<=L'Z') return ch - L'A' + L'a';
            return ch;
           }
};


struct keepcaseChar
{
    private:
        keepcaseChar& operator=(const keepcaseChar&) { return *this; }
    public:
        keepcaseChar() {}
        template<typename TC>
        TC operator()(TC c) const { return c; }
};


/*
MARTY_STRING3
typedef ::std::basic_string< TCHAR, 
                             ::std::char_traits<TCHAR>, 
                             ::std::allocator<TCHAR> >  tstring;
*/

/*
upperCaseCopy is same as upperCase
lowerCaseCopy is same as lowerCase

*/

template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator>
upperCase( const ::std::basic_string<CharType, Traits, Allocator> &str
         , const ::std::locale &loc /*  = MARTY_NS filename::utils::makeCurrentLocale() */ )
   {
    ::std::basic_string<CharType, Traits, Allocator> res(str);
    ::std::transform(res.begin(), res.end(), res.begin(), uppercaseChar(loc));
    return res;
   }

/*
template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator>
upperCase( const ::std::basic_string<CharType, Traits, Allocator> &str )
   {
    return upperCase(str, MARTY_FILENAME::utils::makeCurrentLocale() );
   }

template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator>
upperCaseCopy( const ::std::basic_string<CharType, Traits, Allocator> &str
         , const ::std::locale &loc )
   {
    ::std::basic_string<CharType, Traits, Allocator> res(str);
    ::std::transform(res.begin(), res.end(), res.begin(), uppercaseChar(loc));
    return res;
   }

template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
::std::basic_string<CharType, Traits, Allocator>
upperCaseCopy( const ::std::basic_string<CharType, Traits, Allocator> &str )
   {
    return upperCase(str, MARTY_FILENAME::utils::makeCurrentLocale() );
   }


template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
void
upperCaseInplace( ::std::basic_string<CharType, Traits, Allocator> &str
         , const ::std::locale &loc )
   {
    str = upperCaseCopy(str, loc);
   }

template< typename CharType 
        , typename Traits   
        , typename Allocator
        >
void
upperCaseInplace( ::std::basic_string<CharType, Traits, Allocator> &str )
   {
    str = upperCaseCopy(str, MARTY_FILENAME::utils::makeCurrentLocale());
   }
*/





template< typename CharType 
        , typename Traits   
        , typename Allocator
        > 
::std::basic_string<CharType, Traits, Allocator>
lowerCase( const ::std::basic_string<CharType, Traits, Allocator> &str
         , const std::locale &loc /*  = MARTY_NS filename::utils::makeCurrentLocale() */ )
   {
    ::std::basic_string<CharType, Traits, Allocator> res(str);
    ::std::transform(res.begin(), res.end(), res.begin(), lowercaseChar(loc));
    return res;
   }

/*
template< typename CharType 
        , typename Traits   
        , typename Allocator
        > 
::std::basic_string<CharType, Traits, Allocator>
lowerCase( const ::std::basic_string<CharType, Traits, Allocator> &str )
   {
    return lowerCase(str, MARTY_FILENAME::utils::makeCurrentLocale() );
   }

template< typename CharType 
        , typename Traits   
        , typename Allocator
        > 
::std::basic_string<CharType, Traits, Allocator>
lowerCaseCopy( const ::std::basic_string<CharType, Traits, Allocator> &str
         , const std::locale &loc )
   {
    ::std::basic_string<CharType, Traits, Allocator> res(str);
    ::std::transform(res.begin(), res.end(), res.begin(), lowercaseChar(loc));
    return res;
   }

template< typename CharType 
        , typename Traits   
        , typename Allocator
        > 
::std::basic_string<CharType, Traits, Allocator>
lowerCaseCopy( const ::std::basic_string<CharType, Traits, Allocator> &str )
   {
    return lowerCase(str, MARTY_FILENAME::utils::makeCurrentLocale() );
   }

template< typename CharType 
        , typename Traits   
        , typename Allocator
        > 
void
lowerCaseInplace( ::std::basic_string<CharType, Traits, Allocator> &str
         , const std::locale &loc )
   {
    str = lowerCaseCopy(str, MARTY_FILENAME::utils::makeCurrentLocale() );
   }

template< typename CharType 
        , typename Traits   
        , typename Allocator
        > 
void
lowerCaseInplace( ::std::basic_string<CharType, Traits, Allocator> &str )
   {
    return lowerCaseCopy(str, MARTY_FILENAME::utils::makeCurrentLocale() );
   }
*/







}; // namespace util
}; // namespace marty



#endif /* MARTY_CASECONV_H */

