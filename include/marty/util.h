/*
   Copyright (c) 2007-2009 Alex Martynov, amart at mail.ru
   This code can be used without any limitations.
   This code provided as is, use it on your own risk.
*/

#ifndef MARTY_UTIL_H
#define MARTY_UTIL_H


namespace marty{

namespace util
{


//-----------------------------------------------------------------------------

const int token_compress_on  = 0x0001;
const int token_compress_off = 0x0000;

template< typename CharType 
        , typename Traits   
        , typename Allocator
        , typename PredSepType
        >
void split( ::std::vector< ::std::basic_string<CharType, Traits, Allocator> > &vec
          , const ::std::basic_string<CharType, Traits, Allocator> &str
          , PredSepType predSep
          , int flags
          )
   {
    typedef ::std::basic_string<CharType, Traits, Allocator>   string;
    typedef typename string::const_iterator const_iterator;
    const_iterator it = str.begin();
    const_iterator prev = it;

    while(it!=str.end())
       {
        if (predSep(*it)) // found sep
           {
            string tmp(prev, it);
            if (!tmp.empty() || !(flags&token_compress_on))
               vec.push_back(tmp);
            prev = ++it;
            continue;
           }
        ++it;
       }

    if (it!=prev)
       {
        string tmp(prev, it);
        if (!tmp.empty() || !(flags&token_compress_on))
           vec.push_back(tmp);
       }
   }

//-----------------------------------------------------------------------------
template<char C>
struct CIsExactChar
{
    bool operator()(char c) const
       { return c==C; }
    bool operator()(wchar_t c) const
       { return c==(wchar_t)C; }
};

//-----------------------------------------------------------------------------
template< typename CharType >
struct CIsSpace
{
    bool operator()(const CharType ch) const
       {
        if ( ch==(CharType)' '
           ||ch==(CharType)'\r'
           ||ch==(CharType)'\n'
           ||ch==(CharType)'\t'
           ) return true;
        return false;
       }
};

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        , typename IsSpacePred
        >
void ltrim(::std::basic_string<CharType, Traits, Allocator> &str, const IsSpacePred &pred)
   {
    typedef ::std::basic_string<CharType, Traits, Allocator> string;
    typedef typename string::size_type                       size_type;
    size_type pos = 0, size = str.size();
    for(; pos!=size && pred(str[pos]); ++pos) {}
    str.erase(0, pos);
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        , typename IsSpacePred
        >
void rtrim(::std::basic_string<CharType, Traits, Allocator> &str, const IsSpacePred &pred)
   {
    typedef ::std::basic_string<CharType, Traits, Allocator> string;
    typedef typename string::size_type                       size_type;
    size_type pos = str.size();
    for(; pos && pred(str[pos-1]); --pos) {}
    str.erase(pos, str.size()-pos);
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        , typename IsSpacePred
        >
void trim(::std::basic_string<CharType, Traits, Allocator> &str, const IsSpacePred &pred)
   {
    ltrim(str, pred);
    rtrim(str, pred);
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        , typename IsSpacePred
        >
::std::basic_string<CharType, Traits, Allocator>
ltrim_copy(const ::std::basic_string<CharType, Traits, Allocator> &str, const IsSpacePred &pred)
   {
    ::std::basic_string<CharType, Traits, Allocator> res = str;
    ltrim(res, pred);
    return res;
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        , typename IsSpacePred
        >
::std::basic_string<CharType, Traits, Allocator>
rtrim_copy(const ::std::basic_string<CharType, Traits, Allocator> &str, const IsSpacePred &pred)
   {
    ::std::basic_string<CharType, Traits, Allocator> res = str;
    rtrim(res, pred);
    return res;
   }

//-----------------------------------------------------------------------------
template< typename CharType 
        , typename Traits   
        , typename Allocator
        , typename IsSpacePred
        >
::std::basic_string<CharType, Traits, Allocator>
trim_copy(const ::std::basic_string<CharType, Traits, Allocator> &str, const IsSpacePred &pred)
   {
    ::std::basic_string<CharType, Traits, Allocator> res = str;
    ltrim(res, pred);
    rtrim(res, pred);
    return res;
   }
//-----------------------------------------------------------------------------




}; // namespace util

}; // namespace marty


#endif /* MARTY_UTIL_H */

