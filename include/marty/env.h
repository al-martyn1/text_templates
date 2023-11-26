/*
   Copyright (c) 2007-2009 Alex Martynov, amart at mail.ru
   This code can be used without any limitations.
   This code provided as is, use it on your own risk.
*/

#ifndef MARTY_ENV_H
#define MARTY_ENV_H


#if !defined(_INC_STDLIB) && !defined(_STDLIB_H_) && !defined(_STDLIB_H)
    #include <stdlib.h>
#endif

#if !defined(_MAP_) && !defined(_STLP_MAP) && !defined(__STD_MAP__) && !defined(_CPP_MAP) && !defined(_GLIBCXX_MAP)
    #include <map>
#endif


#ifndef _MSC_VER
    #include <marty/utf.h>
    #include <marty/concvt.h>
#endif


//-----------------------------------------------------------------------------
namespace marty
{
namespace env
{

//-----------------------------------------------------------------------------
inline
bool getVar(const ::std::string &varName, ::std::string &val)
   {
    if (varName.empty()) return false;
    char * pval = ::getenv(varName.c_str());
    if (!pval) return false;
    val = pval;
    return true;
   }

//-----------------------------------------------------------------------------
#if defined(_MSC_VER) && _MSC_VER>=1300
    #pragma warning( push )
    #pragma warning( disable : 4996 ) // Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct.
#endif

inline
bool putVar(const ::std::string &varName, const ::std::string &val)
   {
    ::std::string tmp = varName + ::std::string("=") + val;
    // Могут быть утечки памяти, но зато везде безопасно.
    // Некоторые системы не делают копию передаваемого значения, 
    // а запоминают непосредственно переданное значение, поэтому передача
    // автоматических переменных вызовет крах.
    char *ptmp = (char*)malloc(tmp.size()+1);
    tmp.copy(ptmp, tmp.size());
    ptmp[tmp.size()] = 0;
    #ifdef _MSC_VER
    if (::_putenv(ptmp)<0) return false;
    #else
    if (::putenv(ptmp)<0) return false;
    #endif
    return true;
   }
#if defined(_MSC_VER) && _MSC_VER>=1300
    #pragma warning( pop )
#endif


//-----------------------------------------------------------------------------
#ifdef _MSC_VER

    #if defined(_MSC_VER) && _MSC_VER>=1300
        #pragma warning( push )
        #pragma warning( disable : 4996 ) // Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct.
    #endif

    //-----------------------------------------------------------------------------
    inline
    bool getVar(const ::std::wstring &varName, ::std::wstring &val)
       {
        if (varName.empty()) return false;
        wchar_t * pval = ::_wgetenv(varName.c_str());
        if (!pval) return false;
        val = pval;
        return true;
       }    

    //-----------------------------------------------------------------------------
    inline
    bool putVar(const ::std::wstring &varName, const ::std::wstring &val)
       {
        ::std::wstring tmp = varName + ::std::wstring(L"=") + val;
        wchar_t *ptmp = (wchar_t*)malloc((tmp.size()+1)*sizeof(wchar_t));
        tmp.copy(ptmp, tmp.size());
        ptmp[tmp.size()] = 0;
        if (::_wputenv(ptmp)<0) return false;
        return true;
       }

    #if defined(_MSC_VER) && _MSC_VER>=1300
        #pragma warning( pop )
    #endif    

#else // _MSC_VER not defined

    //-----------------------------------------------------------------------------
    inline
    bool getVar(const ::std::wstring &varName, ::std::wstring &val)
       {
        ::std::string strVal;
        //bool bRes = getVar(MARTY_CON_NS strToAnsi(varName), strVal);
        bool bRes = getVar(MARTY_CON_NS str2con(varName), strVal);
        if (!bRes) return bRes;
        MARTY_UTF_NS fromUtf8(strVal).swap(val);
        return true;
       }

    //-----------------------------------------------------------------------------
    inline
    bool putVar(const ::std::wstring &varName, const ::std::wstring &val)
       {
        //return putVar( MARTY_CON_NS strToAnsi(varName), MARTY_CON_NS strToAnsi(val) );
        return putVar( MARTY_CON_NS str2con(varName), MARTY_CON_NS str2con(val) );
       }

#endif

//-----------------------------------------------------------------------------
template < typename CharType
         , typename Traits
         , typename Allocator
         >
bool getVar( ::std::map< ::std::basic_string<CharType, Traits, Allocator>
                       , ::std::basic_string<CharType, Traits, Allocator> > &cache
           , const ::std::basic_string<CharType, Traits, Allocator> &varName
           , ::std::basic_string<CharType, Traits, Allocator>       &val
           )
   {
    typedef ::std::basic_string<CharType, Traits, Allocator> string;
    typename ::std::map< string, string >::const_iterator it = cache.find(varName);
    if (it!=cache.end())
       {
        val = it->second;
        return true;
       }

    if (getVar(varName, val))
       {
        cache[varName] = val;
        return true;
       }
    return false;
   }

//-----------------------------------------------------------------------------
template < typename CharType
         , typename Traits
         , typename Allocator
         >
bool putVar( ::std::map< ::std::basic_string<CharType, Traits, Allocator>
                       , ::std::basic_string<CharType, Traits, Allocator> > &cache
           , const ::std::basic_string<CharType, Traits, Allocator> &varName
           , const ::std::basic_string<CharType, Traits, Allocator> &val
           , bool bUpdateEnv = false
           )
   {
    typedef ::std::basic_string<CharType, Traits, Allocator> string;
    bool bNeedUpdateCache = false;

    typename ::std::map< string, string >::const_iterator it = cache.find(varName);
    if (it!=cache.end())
       { // found in cache
        if (it->second!=val) // val in cache is different with new, need to update cache
           bNeedUpdateCache = true;
       }
    else
       { // not found in cache, need to update
        bNeedUpdateCache = true;
       }

    if (bNeedUpdateCache)
       cache[varName] = val;

    if (!bUpdateEnv) return true; // job completed, don't need update environment vars

    // foooh, more job, need to update env var
    bool bNeedUpdateEnv = false;
    string tmp;
    if (!getVar(varName, tmp))
       { // failed to get var or var not exist
        return putVar(varName, val); // set var
       }

    // envVar value in tmp
    if (tmp!=val)
       return putVar(varName, val); // set var

    return true;
   }

template < typename CharType
         , typename Traits
         , typename Allocator
         >
void flushVars( ::std::map< ::std::basic_string<CharType, Traits, Allocator>
                          , ::std::basic_string<CharType, Traits, Allocator> > &cache
              )
   {
    typedef ::std::basic_string<CharType, Traits, Allocator> string;
    typename ::std::map< string, string >::const_iterator it = cache.begin();
    for(; it!=cache.end(); ++it)
       {
        string tmp;
        if (!getVar(it->first, tmp))
           { // failed to get var or var not exist
            putVar(it->first, it->second); // set var
            continue;
           }

        if (it->second!=tmp) // different values 
           { 
            putVar(it->first, it->second); // set var
            continue;
           }
        // value is the same       
       }
   }



}; // namespace env
}; // namespace marty


#endif /* MARTY_ENV_H */




