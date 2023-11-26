/*
   Copyright (c) 2007-2009 Alex Martynov, amart at mail.ru
   This code can be used without any limitations.
   This code provided as is, use it on your own risk.
*/

#ifndef MARTY_CONCVT_H
#define MARTY_CONCVT_H

#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif


#ifdef _WIN32
    #if !defined(_INC_MALLOC) && !defined(_MALLOC_H_) && !defined(_MALLOC_H)
        #include <malloc.h>
    #endif
#else
    #include <alloca.h>
    #ifndef _alloca
        #define _alloca  alloca
    #endif
#endif


#ifdef CON_IN_MARTY_NAMESPACE
    #ifndef MARTY_NS
        #define BEGIN_MARTY_NS          namespace marty {
        #define END_MARTY_NS            };
        #define MARTY_NS                ::marty::
    #endif
    #define MARTY_CON_NS                MARTY_NS con::
    #define MARTY_CON                   MARTY_NS con
#else
    #ifndef MARTY_NS
        #define BEGIN_MARTY_NS 
        #define END_MARTY_NS 
        #define MARTY_NS                ::
    #endif
    #define MARTY_CON_NS                MARTY_NS con::
    #define MARTY_CON                   MARTY_NS con
#endif

#ifdef _WIN32
    #include <windows.h>
#else
    #include <marty/utf.h>
#endif


BEGIN_MARTY_NS
namespace con
{


//-----------------------------------------------------------------------------
//Use the AreFileApisANSI function to determine which code page the set of file I/O 
//functions is currently using. Use the SetConsoleCP and SetConsoleOutputCP functions 
//to set the code page for the 8-bit console functions.


#ifdef _WIN32


    inline 
    bool isConsoleAnsi()
       {
        return AreFileApisANSI() > 0;
       }

    //-----------------------------------------------------------------------------
    inline ::std::string strToAnsi(const ::std::string &s) { return s; }
    
    //-----------------------------------------------------------------------------
    inline
    ::std::string strToAnsi(const ::std::wstring &str, int codePage = CP_ACP)
       {
        if (codePage==-1)
           {
            if (isConsoleAnsi()) codePage = CP_ACP;
            else                 codePage = CP_OEMCP;
           }

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
            return ::std::string(pRes); 
           }
        //pRes[res] = 0;
        return ::std::string();
       }
    
    //-----------------------------------------------------------------------------
    inline ::std::wstring strToWide(const ::std::wstring &s) { return s; }
    
    //-----------------------------------------------------------------------------
    inline
    std::wstring strToWide(const std::string &str, int codePage = CP_ACP )
       {
        if (codePage==-1)
           {
            if (isConsoleAnsi()) codePage = CP_ACP;
            else                 codePage = CP_OEMCP;
           }
        
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
    
        inline ::std::string  str2con(const ::std::string  &str) { return str; }        
        inline ::std::string  str2con(const ::std::wstring &str) { return strToAnsi(str, CP_OEMCP); }

        inline ::std::wstring a2wide(const ::std::string  &str)  { return strToWide(str, CP_ACP); }
        inline ::std::wstring a2wide(const ::std::wstring &str)  { return str; }

        inline ::std::string w2ansi(const ::std::string  &str)  { return str; }
        inline ::std::string w2ansi(const ::std::wstring &str)  { return strToAnsi(str, CP_ACP); }

    
    #ifdef _UNICODE
        
        inline ::std::wstring con2str(const ::std::wstring &str) { return str; }
        inline ::std::wstring con2str(const ::std::string  &str) { return strToWide(str, -1); } // , CP_OEMCP

        inline ::std::wstring arg2str(const ::std::string  &str) { return strToWide(str, CP_ACP); } // , CP_ACP
        inline ::std::wstring arg2str(const ::std::wstring &str) { return str; }

    #else
    
        inline ::std::string  con2str(const ::std::string  &str) { return str; }
        inline ::std::string  con2str(const ::std::wstring &str) { return strToAnsi(str, -1); } // , CP_ACP

        inline ::std::string  arg2str(const ::std::string  &str) { return str; }
        inline ::std::string  arg2str(const ::std::wstring &str) { return strToAnsi(str, CP_ACP); } // , CP_ACP
    #endif

#else // !_WIN32
    
        inline ::std::string  str2con(const ::std::string  &str) { return str; }
        inline ::std::string  str2con(const ::std::wstring &str) { return MARTY_UTF_NS toUtf8(str); }

        inline ::std::wstring a2wide(const ::std::string  &str)  { return MARTY_UTF_NS fromUtf8(str); }
        inline ::std::wstring a2wide(const ::std::wstring &str)  { return str; }

        inline ::std::string w2ansi(const ::std::string  &str)  { return str; }
        inline ::std::string w2ansi(const ::std::wstring &str)  { return MARTY_UTF_NS toUtf8(str); }


        inline ::std::string  strToAnsi(const ::std::string   &str) { return str; }
        inline ::std::string  strToAnsi(const ::std::wstring  &str) { return MARTY_UTF_NS toUtf8(str); }
        inline ::std::wstring strToWide(const ::std::string   &str) { return MARTY_UTF_NS fromUtf8(str); }
        inline ::std::wstring strToWide(const ::std::wstring  &str) { return str; }

    #ifdef _UNICODE
        
        inline ::std::wstring con2str(const ::std::string  &str) { return MARTY_UTF_NS fromUtf8(str); }
        inline ::std::wstring con2str(const ::std::wstring &str) { return str; }

        inline ::std::wstring arg2str(const ::std::string  &str) { return MARTY_UTF_NS fromUtf8(str); }
        inline ::std::wstring arg2str(const ::std::wstring &str) { return str; }

    #else

        inline ::std::string con2str(const ::std::string   &str) { return str;         }
        inline ::std::string con2str(const ::std::wstring  &str) { return MARTY_UTF_NS toUtf8(str); }

        inline ::std::string arg2str(const ::std::string   &str) { return str;         }
        inline ::std::string arg2str(const ::std::wstring  &str) { return MARTY_UTF_NS toUtf8(str); }

    #endif
    

#endif


#if defined(_UNICODE) || defined(UNICODE)
    inline ::std::wstring toTstr(const ::std::string  &str)  { return a2wide(str); }
    inline ::std::wstring toTstr(const ::std::wstring  &str) { return str; }
#else
    inline ::std::string toTstr(const ::std::string  &str)  { return str; }
    inline ::std::string toTstr(const ::std::wstring  &str) { return w2ansi(str); }
#endif


}; // namespace con

END_MARTY_NS


#endif /* MARTY_CONCVT_H */

