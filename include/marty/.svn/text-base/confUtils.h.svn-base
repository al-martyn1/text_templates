/*
   Copyright (c) 2007-2009 Alex Martynov, amart at mail.ru
   This code can be used without any limitations.
   This code provided as is, use it on your own risk.
*/

#ifndef MARTY_CONFUTILS_H
#define MARTY_CONFUTILS_H


#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

#if !defined(_MAP_) && !defined(_STLP_MAP) && !defined(__STD_MAP__) && !defined(_CPP_MAP) && !defined(_GLIBCXX_MAP)
    #include <map>
#endif

// aux macro for short string definition in template params
#define MARTY_CONFUTILS_STRING(tch)  ::std::basic_string< tch, ::std::char_traits< tch >, ::std::allocator< tch > >


namespace marty
{

namespace confUtils
{

//-----------------------------------------------------------------------------
struct IsSpacePred
{
    bool operator()(char ch) const
       {
        if (ch==' ' || ch=='\n' || ch=='\r' || ch=='\t') return true;
        return false;
       }
    bool operator()(wchar_t ch) const
       {
        if (ch==L' ' || ch==L'\n' || ch==L'\r' || ch==L'\t') return true;
        return false;
       }
};

//-----------------------------------------------------------------------------
// parses options string like: opt1=val;opt2="val"
// where [=], ["], [;] are customizable
template <typename TC, typename spacePred>
void
splitOptionsString( ::std::map< MARTY_CONFUTILS_STRING(TC) , MARTY_CONFUTILS_STRING(TC) > &optMap
                  , const MARTY_CONFUTILS_STRING(TC) &str
                  , TC sep
                  , TC eq
                  , TC quot
                  , const spacePred &pred
                  )
   {
    MARTY_CONFUTILS_STRING(TC) buf, name;
    bool inQuot = false;
    bool readName = true;
    //bool valQuoted = false;
    typename MARTY_CONFUTILS_STRING(TC) ::size_type i=0, size=str.size();

    for(; i<size; ++i)
       {
        if (!inQuot && pred(str[i]))
           continue; // skip space
        // если кавычка и (предыдущий символ eq или начало строки)
        if (str[i]==quot && (i==0 || str[i-1]==eq))
           {
            inQuot = true;
            // skip
           }
        // если кавычка и (следующий символ sep или конец строки)
        else if (str[i]==quot && ((i+1)>=size || str[i+1]==sep))
           {
            inQuot    = false;
            //valQuoted = true;
            // skip
           }
        else if (str[i]==sep && !inQuot)
           {
            if (readName) // opt name without value
               optMap[buf] = MARTY_CONFUTILS_STRING(TC) ();
            else
               {
                //if (buf.size()<2 || !valQuoted)
                   optMap[name] = buf;
                /* else
                 *    optMap[name] = MARTY_CONFUTILS_STRING(TC)(buf, 1, buf.size()-2);
                 */
               }
            buf.erase(buf.begin(), buf.end());
            name.erase(name.begin(), name.end());
            readName  = true;
            //valQuoted = false;
            inQuot    = false;
           }
        else if (str[i]==eq && !inQuot)
           {
            readName  = false;
            //valQuoted = false;
            inQuot    = false;
            name = buf;
            buf.erase(buf.begin(), buf.end());
           }
        else
           {
            buf.append(1, str[i]);
           }
       }

    if (!buf.empty())
       {
        if (readName) // opt name without value
           optMap[buf] = MARTY_CONFUTILS_STRING(TC) ();
        else
           optMap[name] = buf;
       }
   }

//-----------------------------------------------------------------------------
//template<>
inline
void splitOptionsString( ::std::map< ::std::string , ::std::string > &optMap
                  , const ::std::string &str
                  , char sep  = ';'
                  , char eq   = '='
                  , char quot = '\"'
                  , const IsSpacePred &pred = IsSpacePred()
                  )
   {
    splitOptionsString<char, IsSpacePred>(optMap, str, sep, eq, quot, pred);
   }

//-----------------------------------------------------------------------------
//template<>
inline
void splitOptionsString( ::std::map< ::std::wstring , ::std::wstring > &optMap
                  , const ::std::wstring &str
                  , wchar_t sep  = ';'
                  , wchar_t eq   = '='
                  , wchar_t quot = '\"'
                  , const IsSpacePred &pred = IsSpacePred()
                  )
   {
    splitOptionsString<wchar_t, IsSpacePred>(optMap, str, sep, eq, quot, pred);
   }

//-----------------------------------------------------------------------------
template <typename TC>
typename MARTY_CONFUTILS_STRING(TC) :: size_type 
findBracketPair( const MARTY_CONFUTILS_STRING(TC) srcStr, TC startCh, TC endCh)
   {
    int pairCount = 0;
    typename MARTY_CONFUTILS_STRING(TC) :: size_type pos = 0, size = srcStr.size();
    if (srcStr[0]!=startCh) return MARTY_CONFUTILS_STRING(TC) :: npos;
    for(; pos!=size; ++pos)
       {
        if (srcStr[pos]==startCh)
           {
            ++pairCount;
            continue;
           }
        if (srcStr[pos]==endCh)
           {
            --pairCount;
            if (!pairCount)
               {
                return pos+1;
               }
           }
       }

    return MARTY_CONFUTILS_STRING(TC) :: npos;   
   }

//-----------------------------------------------------------------------------
//template <>
inline
::std::string :: size_type 
findBracketPair( const ::std::string srcStr, char startCh, char endCh)
   {
    return findBracketPair<char>( srcStr, startCh, endCh);
   }

//-----------------------------------------------------------------------------
//template <>
inline
::std::wstring :: size_type 
findBracketPair( const ::std::wstring srcStr, wchar_t startCh, wchar_t endCh)
   {
    return findBracketPair<wchar_t>( srcStr, startCh, endCh);
   }



}; // namespace confUtils

}; // namespace marty


#endif /* MARTY_CONFUTILS_H */

