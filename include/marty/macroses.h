/*
   Copyright (c) 2007-2009 Alex Martynov, amart at mail.ru
   This code can be used without any limitations.
   This code provided as is, use it on your own risk.
*/

#ifndef MARTY_MACROSES_H
#define MARTY_MACROSES_H



#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

#if !defined(_VECTOR_) && !defined(_STLP_VECTOR) && !defined(__STD_VECTOR__) && !defined(_CPP_VECTOR) && !defined(_GLIBCXX_VECTOR)
    #include <vector>
#endif

#if !defined(_MAP_) && !defined(_STLP_MAP) && !defined(__STD_MAP__) && !defined(_CPP_MAP) && !defined(_GLIBCXX_MAP)
    #include <map>
#endif

#if !defined(_SET_) && !defined(_STLP_SET) && !defined(__STD_SET__) && !defined(_CPP_SET) && !defined(_GLIBCXX_SET)
    #include <set>
#endif

#if !defined(_SSTREAM_) && !defined(_STLP_SSTREAM) && !defined(__STD_SSTREAM__) && !defined(_CPP_SSTREAM) && !defined(_GLIBCXX_SSTREAM)
    #include <sstream>
#endif

#if !defined(_STDEXCEPT_) && !defined(_STLP_STDEXCEPT) && !defined(__STD_STDEXCEPT) && !defined(_CPP_STDEXCEPT) && !defined(_GLIBCXX_STDEXCEPT)
    #include <stdexcept>
#endif


#include <marty/env.h>
#include <marty/concvt.h>
//#include <boost/algorithm/string/split.hpp>
//#include <boost/algorithm/string/trim.hpp>
#include <marty/util.h>
#include <marty/caseconv.h>



namespace marty
{
namespace macro
{

namespace util
{


#if !defined(_MSC_VER) || _MSC_VER<1400
    #ifndef _sprintf
        #define _sprintf sprintf
        #define _snprintf snprintf
        #define __macroses_undef_sprintf
    #endif
#endif

// CharType, ::std::char_traits<CharType>, ::std::allocator<CharType>
template < typename CharType
         , typename Traits
         , typename Allocator
         >
::std::basic_string<CharType, Traits, Allocator>
intToStr(int i)
   {
    char tmpBuf[128];
    char *tmpBufPtr = tmpBuf;
    _snprintf(tmpBuf, sizeof(tmpBuf)/sizeof(tmpBuf[0]), "%d", i);
    tmpBuf[sizeof(tmpBuf)/sizeof(tmpBuf[0])-1] = 0;

    ::std::basic_string<CharType, Traits, Allocator> res;
    for(; *tmpBufPtr; ++tmpBufPtr) res.append(1, (CharType)*tmpBufPtr);
    return res;

   }

#ifdef __macroses_undef_sprintf
    #undef __mbs_undef_sprintf
    #undef _sprintf
    #undef __macroses_undef_sprintf
#endif


}; // namespace util



// subst macros flags
const int smf_ArgsAllowed                     = 0x0001;
const int smf_ConditionAllowed                = 0x0002;
const int smf_AppendVarValueAllowed           = 0x0004;
const int smf_SetVarValueSubstitutionAllowed  = 0x0008;
const int smf_changeDot                       = 0x0010;
const int smf_changeSlash                     = 0x0020;
const int smf_uppercaseNames                  = 0x0040;
const int smf_lowercaseNames                  = 0x0080;




//-----------------------------------------------------------------------------
template < typename CharType
         , typename Traits
         , typename Allocator
         >
::std::basic_string<CharType, Traits, Allocator> changeDotsAndSlash(const ::std::basic_string<CharType, Traits, Allocator> &str, bool changeDot, bool changeSlash)
   {
    ::std::basic_string<CharType, Traits, Allocator> res = str;
    //res.reserve(str.size());
    for(typename ::std::basic_string<CharType, Traits, Allocator>::iterator it = res.begin(); it!=res.end(); ++it)
       {
        if ((*it==(CharType)'\\' || *it==(CharType)'/') && changeSlash)
           {
            *it = (CharType)'_';
            continue;
           }
        if (*it==(CharType)'.' && changeDot)
           {
            *it = (CharType)'_';
            continue;
           }
       }
    return res;
   }

//-----------------------------------------------------------------------------
template < typename CharType
         , typename Traits
         , typename Allocator
         >
::std::basic_string<CharType, Traits, Allocator> 
substMacroses( const std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > &macroses
             , const ::std::basic_string<CharType, Traits, Allocator> &str
             , bool changeDot=true
             , bool changeSlash=true
             , std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > *pEnvCache = 0
             , int flags = smf_ArgsAllowed|smf_ConditionAllowed
             );


template < typename CharType
         , typename Traits
         , typename Allocator
         >
void splitToMacrosPair( const ::std::basic_string<CharType, Traits, Allocator> &variableValuePair
                , ::std::basic_string<CharType, Traits, Allocator> &var
                , ::std::basic_string<CharType, Traits, Allocator> &val
                , bool &bAppend
                , bool &bSubst
                , CharType separatorChar
                )
   {
    typedef typename ::std::basic_string<CharType, Traits, Allocator> string;
    typename string::size_type eqPos = variableValuePair.find(separatorChar)
                          , valStart = ::std::basic_string<CharType, Traits, Allocator>::npos;

    if (eqPos== string::npos )
       {
        var = variableValuePair;
       }
    else
       {
        valStart = eqPos+1;
        
        if ( eqPos>0 && ( variableValuePair[eqPos-1]==(CharType)':' ||
                          variableValuePair[eqPos-1]==(CharType)'+' ||
                          variableValuePair[eqPos-1]==(CharType)'<'
                        )
           )
           {
            eqPos--;
            if (variableValuePair[eqPos]==(CharType)'<')
               {
                bSubst = true;
               }
            else
               {
                if (variableValuePair[eqPos]==(CharType)'+') bAppend = true; // else ==':'
                if ( eqPos>0 && variableValuePair[eqPos-1]==(CharType)'<')
                   {
                    eqPos--;
                    bSubst = true;
                   }
               }
           }
        var = string(variableValuePair, 0, eqPos);
        val = string(variableValuePair, valStart);
        ::marty::util::trim(var, ::marty::util::CIsSpace<CharType>());
       }
                          
   }

//-----------------------------------------------------------------------------
template < typename CharType
         , typename Traits
         , typename Allocator
         >
::std::basic_string<CharType, Traits, Allocator> 
prepareMacroName( const ::std::basic_string<CharType, Traits, Allocator>  &name, int flags )
   {
    if (flags&smf_uppercaseNames)
       return marty::util::upperCase(name, ::std::locale("C"));
    else if (flags&smf_lowercaseNames)
       return marty::util::lowerCase(name, ::std::locale("C"));
    return name;
   }

//-----------------------------------------------------------------------------
template < typename CharType
         , typename Traits
         , typename Allocator
         >
void varToMacro( ::std::basic_string<CharType, Traits, Allocator> variableValuePair
               , std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > &macroses
               , bool bToUpper = false
               , bool bToLower = false
               , const ::std::basic_string<CharType, Traits, Allocator> &prependStr = ::std::basic_string<CharType, Traits, Allocator>()
               , const ::std::basic_string<CharType, Traits, Allocator> &appendStr = ::std::basic_string<CharType, Traits, Allocator>()
               , int flags = smf_ArgsAllowed|smf_ConditionAllowed|smf_AppendVarValueAllowed|smf_SetVarValueSubstitutionAllowed
               , std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > *pEnvCache = 0
               )
   {
    ::marty::util::trim(variableValuePair, ::marty::util::CIsSpace<CharType>() );
    if (variableValuePair.empty() || variableValuePair[0]==(CharType)'#') return; //continue;
    ::std::basic_string<CharType, Traits, Allocator> var, val;
    bool bAppend = false;
    bool bSubst  = false;
    splitToMacrosPair( variableValuePair, var, val, bAppend, bSubst, (CharType)'=' );

    if (flags&smf_uppercaseNames)
       bToUpper = true;
    if (flags&smf_lowercaseNames)
       bToLower = true;

    if (bToUpper) flags |= smf_uppercaseNames;
    if (bToLower) flags |= smf_lowercaseNames;

    if (bToUpper)
       {
        var = marty::util::upperCase(var, ::std::locale("C"));
       }
    else if (bToLower)
       {
        var = marty::util::lowerCase(var, ::std::locale("C"));
       }

    if (!prependStr.empty())
       {
        var = prependStr + var;
       }
    if (!appendStr.empty())
       {
        var.append(appendStr);
       }

    if (!var.empty() && var[0]==(CharType)'%') return; // macros names started with % not alowed

    if (bSubst && (flags&smf_SetVarValueSubstitutionAllowed))
       {
        val = substMacroses( macroses, val
                           , ((flags&smf_changeDot) ? true : false)
                           , ((flags&smf_changeSlash) ? true : false)
                           , pEnvCache
                           , flags
                           );
       }

    if (bAppend && (flags&smf_AppendVarValueAllowed))
       macroses[var].append(val); // += val;
    else
       macroses[var].assign(val); //  = val;
   }


//-----------------------------------------------------------------------------
template < typename CharType
         , typename Traits
         , typename Allocator
         >
void varsToMacroses( const ::std::basic_string<CharType, Traits, Allocator> vars
                   , std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > &macroses
                   , bool bToUpper = false
                   , bool bToLower = false
                   , const ::std::basic_string<CharType, Traits, Allocator> &prependStr = ::std::basic_string<CharType, Traits, Allocator>()
                   , const ::std::basic_string<CharType, Traits, Allocator> &appendStr = ::std::basic_string<CharType, Traits, Allocator>()
                   , int flags = smf_ArgsAllowed|smf_ConditionAllowed|smf_AppendVarValueAllowed|smf_SetVarValueSubstitutionAllowed
                   , std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > *pEnvCache = 0
                   )
   {
    std::vector< ::std::basic_string<CharType, Traits, Allocator> > singles;
    //::boost::algorithm::split(singles, vars, util::CIsExactChar<'\n'>(), boost::algorithm::token_compress_on);
    ::marty::util::split(singles, vars, ::marty::util::CIsExactChar<'\n'>(), ::marty::util::token_compress_on);
    typename std::vector< ::std::basic_string<CharType, Traits, Allocator> >::iterator it = singles.begin();
    for(; it!=singles.end(); ++it)
       {
        #ifndef USE_OLD_VERSION_BACKUP

        varToMacro(*it, macroses, bToUpper, bToLower, prependStr, appendStr, flags, pEnvCache);

        #else

        //boost::algorithm::trim(*it);
        ::marty::util::trim(*it, ::marty::util::CIsSpace<CharType>() );
        if (it->empty() || (*it)[0]==(CharType)'#') continue;
        ::std::basic_string<CharType, Traits, Allocator> var, val;
        bool bAppend = false;

        splitToMacrosPair( *it, var, val, bAppend, (CharType)'=' );

        /*
        typename ::std::basic_string<CharType, Traits, Allocator>::size_type eqPos = it->find((CharType)'=')
                                                                          , valStart=::std::basic_string<CharType, Traits, Allocator>::npos;
        if (eqPos==::std::basic_string<CharType, Traits, Allocator>::npos)
           {
            var = *it;
           }
        else
           {
            valStart = eqPos+1;
            if (eqPos>0 && ((*it)[eqPos-1]==(CharType)':' || (*it)[eqPos-1]==(CharType)'+'))
               {
                eqPos--;
                if ((*it)[eqPos]==(CharType)'+') bAppend = true;
               }
            var = ::std::basic_string<CharType, Traits, Allocator>(*it, 0, eqPos);
            val = ::std::basic_string<CharType, Traits, Allocator>(*it, valStart);
            //boost::algorithm::trim(var);
            ::marty::util::trim(var, ::marty::util::CIsSpace<CharType>());
           }
        */

        if (bAppend)
           macroses[var] += val;
        else
           macroses[var]  = val;

        #endif // USE_OLD_VERSION_BACKUP
       }   
   }


//-----------------------------------------------------------------------------
template < char startBr
         , char endBr
         , char Ch
         , typename CharType
         , typename Traits
         , typename Allocator
         >
typename ::std::basic_string<CharType, Traits, Allocator>::size_type 
findChar(const ::std::basic_string<CharType, Traits, Allocator> &str, typename ::std::basic_string<CharType, Traits, Allocator>::size_type pos = 0)
   {
    int depth = 0;
    typename ::std::basic_string<CharType, Traits, Allocator>::size_type size = str.size();
    for(; pos<size; ++pos)
       {
        if (str[pos]==(CharType)startBr) { ++depth; continue; }
        if (str[pos]==(CharType)endBr)   { --depth; continue; }
        if (!depth && str[pos]==(CharType)Ch) return pos;
       }
    return ::std::basic_string<CharType, Traits, Allocator>::npos;
   }

/*
template <char startBr, char endBr, char Ch>
std::string::size_type findChar(const std::string str, std::string::size_type pos = 0)
   {
    int depth = 0;
    std::string::size_type size = str.size();
    for(; pos<size; ++pos)
       {
        if (str[pos]==startBr) { ++depth; continue; }
        if (str[pos]==endBr)   { --depth; continue; }
        if (!depth && str[pos]==Ch) return pos;
       }
    return std::string::npos;
   }

//-----------------------------------------------------------------------------
template <char startBr, char endBr, char Ch>
std::wstring::size_type findChar(const std::wstring str, std::wstring::size_type pos = 0)
   {
    int depth = 0;
    std::wstring::size_type size = str.size();
    for(; pos<size; ++pos)
       {
        if (str[pos]==(wchar_t)startBr) { ++depth; continue; }
        if (str[pos]==(wchar_t)endBr)   { --depth; continue; }
        if (!depth && str[pos]==(wchar_t)Ch) return pos;
       }
    return std::wstring::npos;
   }
*/
//-----------------------------------------------------------------------------
template < typename CharType
         , typename Traits
         , typename Allocator
         >
bool getMacroText( const std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > &macroses
                 , const ::std::basic_string<CharType, Traits, Allocator> &macroName, ::std::basic_string<CharType, Traits, Allocator>  *pMacroText
                 , std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> >       *pEnvCache
                 )
   {
    typename std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> >::const_iterator it = macroses.find(macroName);
    if (it!=macroses.end())
       {
        if (pMacroText) *pMacroText = it->second;
        return true;
       }
    if (macroName.empty()) return false;
    //const char *pEnvVar = getenv(macroName.c_str()); // HACK: thread unsafe
    //if (!pEnvVar) return false;
    //if (pMacroText) *pMacroText = pEnvVar;
    ::std::basic_string<CharType, Traits, Allocator> tmpVal;
    bool res = false;

    if (pEnvCache)
       res = ::marty::env::getVar(*pEnvCache, macroName, tmpVal);
    else 
       res = ::marty::env::getVar(macroName, tmpVal);

    //if (!::marty::env::getVar(macroName, tmpVal)) return false;
    if (!res) return false;

    if (pMacroText) *pMacroText = tmpVal;
    return true;
   }

//-----------------------------------------------------------------------------
template < typename CharType
         , typename Traits
         , typename Allocator
         >
bool getMacroText( const std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > &macroses
                 , const ::std::basic_string<CharType, Traits, Allocator> &macroName
                 , ::std::basic_string<CharType, Traits, Allocator> &macroText
                 , std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > *pEnvCache
                 )
   {
    return getMacroText(macroses, macroName, &macroText, pEnvCache);
   }

//-----------------------------------------------------------------------------
template < typename CharType
         , typename Traits
         , typename Allocator
         >
bool macroExist( const std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > &macroses
               , const ::std::basic_string<CharType, Traits, Allocator> &macroName
               , std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > *pEnvCache
               )
   {
    return getMacroText(macroses, macroName, ( ::std::basic_string<CharType, Traits, Allocator> *)0, pEnvCache);
   }

//-----------------------------------------------------------------------------
template < typename CharType
         , typename Traits
         , typename Allocator
         >
bool macroExistButEmpty( const std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > &macroses
                       , const ::std::basic_string<CharType, Traits, Allocator> &macroName
                       , std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > *pEnvCache
                       )
   {
    ::std::basic_string<CharType, Traits, Allocator> macroText;
    if (!getMacroText(macroses, macroName, macroText, pEnvCache)) return false;
    return  /* ::boost::algorithm:: */ ::marty::util::trim_copy(macroText, ::marty::util::CIsSpace<CharType>() ).empty();
   }



//-----------------------------------------------------------------------------
template < typename CharType
         , typename Traits
         , typename Allocator
         >
::std::basic_string<CharType, Traits, Allocator>
substMacroses( const std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > &macroses
             , const ::std::basic_string<CharType, Traits, Allocator> &str
             , std::set< ::std::basic_string<CharType, Traits, Allocator> > &usedMacroses
             , bool changeDot=true
             , bool changeSlash=true
             , std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > *pEnvCache = 0
             , int flags = smf_ArgsAllowed|smf_ConditionAllowed
             )
   {
    ::std::basic_string<CharType, Traits, Allocator> percentZero(1, (CharType)'%'); percentZero.append(1, (CharType)'0');

    typename ::std::basic_string<CharType, Traits, Allocator>::const_iterator it=str.begin();
    ::std::basic_string<CharType, Traits, Allocator> res; res.reserve(str.size());
    //bool prevSlash = false;
    for(; it!=str.end(); ++it)
       {
        if (*it!=(CharType)'$')
           res.append(1, *it);
        else
           {
            ++it;
            if (it==str.end()) continue;
            
            if (*it==(CharType)'$')
               { 
                res.append(1, *it);
                continue;
               }
            
            if (*it!=(CharType)'(')
               { 
                res.append(1, (CharType)'$');
                res.append(1, *it);
                continue;
               }

            ++it;
            if (it==str.end()) continue;

            typename ::std::basic_string<CharType, Traits, Allocator>::const_iterator start = it;
            int brCnt = 1;            
            for(; it!=str.end(); ++it)
               {
                if (*it==(CharType)'(') { ++brCnt; continue; }
                if (*it==(CharType)')') 
                   {
                    --brCnt;
                    if (!brCnt) break;
                   }
               }

            if (it==str.end()) 
               {
                res.append(start, it);
                continue;
               }

            ::std::basic_string<CharType, Traits, Allocator> macroName = ::std::basic_string<CharType, Traits, Allocator>(start, it);
            //std::string::size_type qPos = macroName.find('?', 0);
            typename ::std::basic_string<CharType, Traits, Allocator>::size_type qPos = findChar<'(', ')', '?'>(macroName);
            //findChar(const std::string str)
            // ? not found, not an conditional
            if (qPos==::std::basic_string<CharType, Traits, Allocator>::npos)
               {
                std::vector< ::std::basic_string<CharType, Traits, Allocator> > parts;
                typename ::std::basic_string<CharType, Traits, Allocator>::size_type startPos = 0, nextPos = findChar<'(', ')', ':'>(macroName, 0);
                do {
                    if (nextPos!=::std::basic_string<CharType, Traits, Allocator>::npos)
                       {
                        parts.push_back(::std::basic_string<CharType, Traits, Allocator>(macroName, startPos, nextPos-startPos));
                        startPos = nextPos+1;
                        nextPos = findChar<'(', ')', ':'>(macroName, startPos);
                       }
                    else
                       {
                        parts.push_back(::std::basic_string<CharType, Traits, Allocator>(macroName, startPos));
                        break;
                       }
                    
                   } while(1);

                //
                if (parts.size()<=1)
                   {
                    ::std::basic_string<CharType, Traits, Allocator> 
                           macroNameChanged = prepareMacroName(changeDotsAndSlash(macroName, changeDot, changeSlash), flags);
                    if (usedMacroses.find(macroNameChanged)!=usedMacroses.end())
                       continue; // allready used

                    ::std::basic_string<CharType, Traits, Allocator> macroText;
                    if (!getMacroText(macroses, macroNameChanged, macroText, pEnvCache))
                       continue; // macro not found

                    std::set< ::std::basic_string<CharType, Traits, Allocator> > usedMacrosesCopy = usedMacroses;
                    usedMacrosesCopy.insert(macroNameChanged);
                    res.append(substMacroses(macroses, macroText, usedMacrosesCopy, changeDot, changeSlash, pEnvCache, flags));
                    continue;
                   }
                else
                   {
                    if (!(flags&smf_ArgsAllowed))
                       {
                        throw std::runtime_error("Parametrised macroses not allowed");
                       }

                    ::std::basic_string<CharType, Traits, Allocator> macroNameChanged = prepareMacroName(changeDotsAndSlash(parts[0], changeDot, changeSlash), flags);
                    
                    if (usedMacroses.find(macroNameChanged)!=usedMacroses.end())
                       continue; // allready used

                    ::std::basic_string<CharType, Traits, Allocator> macroText;
                    if (!getMacroText(macroses, macroNameChanged, macroText, pEnvCache))
                       continue; // macro not found

                    std::set< ::std::basic_string<CharType, Traits, Allocator> > usedMacrosesCopy = usedMacroses;
                    usedMacrosesCopy.insert(macroNameChanged);

                    std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > tmpMacroses = macroses;
                    
                    typename std::vector< ::std::basic_string<CharType, Traits, Allocator> >::size_type pi = 1, piSize = parts.size();

                    for(; pi<piSize; ++pi)
                       {
                        std::set< ::std::basic_string<CharType, Traits, Allocator> > usedMacrosesCopy2 = usedMacrosesCopy;
                        parts[pi] = substMacroses(macroses, parts[pi], usedMacrosesCopy2, changeDot, changeSlash, pEnvCache, flags);
                       }

                    tmpMacroses[percentZero] = util::intToStr< CharType, ::std::char_traits<CharType>, ::std::allocator<CharType> >(int(parts.size())-1);
                    pi = 1;

                    for(; pi<piSize; ++pi)
                       {
                        ::std::basic_string<CharType, Traits, Allocator> idxStr( util::intToStr< CharType, ::std::char_traits<CharType>, ::std::allocator<CharType> >(int(pi)));
                        ::std::basic_string<CharType, Traits, Allocator> paramMacroName(1, (CharType)'%'); paramMacroName.append(idxStr);
                        //::std::basic_string<CharType, Traits, Allocator> paramTestVal(1, (CharType)'$');
                        //paramTestVal.append(1, (CharType)'('); paramTestVal.append(1, (CharType)'%'); paramTestVal.append(idxStr); paramTestVal.append(1, (CharType)')');
                        //parts[pi]
                        //if (paramTestVal!=parts[pi])
                           tmpMacroses[ paramMacroName ] = parts[pi];
                       }
                    res.append(substMacroses(tmpMacroses, macroText, usedMacrosesCopy, changeDot, changeSlash, pEnvCache, flags));
                    continue;
                   }
               }

            //flags = smf_ArgsAllowed|smf_ConditionAllowed
            if (!(flags&smf_ConditionAllowed))
               {
                throw std::runtime_error("Conditional macroses not allowed");
               }

            ::std::basic_string<CharType, Traits, Allocator> macroNameCond = changeDotsAndSlash(::std::basic_string<CharType, Traits, Allocator>(macroName, 0, qPos), changeDot, changeSlash);
            macroNameCond = prepareMacroName(macroNameCond, flags);
            ++qPos;
            if (qPos>=macroName.size())
               {
                continue; // no true or false branches
               }

            if (macroName[qPos]!='*' && macroName[qPos]!='+')
               {
                throw std::runtime_error( ::std::string("Conditional macro inclusion (body: '")
                                        + MARTY_CON_NS str2con(macroName)
                                        + ::std::string("') - invalid condition, ?* nor ?+ used")
                                        );
                //std::cout<<"Conditional macro inclusion (body: '"<<macroName<<"') - invalid condition, ?* nor ?+ used\n";
                continue;
               }

            bool onlyExist = macroName[qPos]=='*';
            typename ::std::basic_string<CharType, Traits, Allocator>::size_type truthBranchStart = ++qPos;
            if (truthBranchStart>=macroName.size())
               {
                continue; // no true or false branches
               }

            typename ::std::basic_string<CharType, Traits, Allocator>::size_type colonPos = findChar<'(', ')', ':'>(macroName, truthBranchStart);
            
            ::std::basic_string<CharType, Traits, Allocator> truthPart, falsePart;
            if (colonPos==::std::basic_string<CharType, Traits, Allocator>::npos || colonPos>=macroName.size())
               {
                truthPart = ::std::basic_string<CharType, Traits, Allocator>(macroName, truthBranchStart);
               }
            else
               {
                typename ::std::basic_string<CharType, Traits, Allocator>::size_type truthBranchLen = colonPos-truthBranchStart;
                truthPart = ::std::basic_string<CharType, Traits, Allocator>(macroName, truthBranchStart, truthBranchLen);
                falsePart = ::std::basic_string<CharType, Traits, Allocator>(macroName, truthBranchStart + truthBranchLen+1);
               }

            bool cond = false;
            ::std::basic_string<CharType, Traits, Allocator> macroText;
            if (getMacroText(macroses, macroNameCond, macroText, pEnvCache))
               { // macro exist
                if (onlyExist)
                   cond = true;
                else
                   {
                    if (! /* ::boost::algorithm:: */ ::marty::util::trim_copy(macroText, ::marty::util::CIsSpace<CharType>()).empty())
                       cond = true;
                   }
               }

            macroText = cond ? truthPart : falsePart;
            ::std::basic_string<CharType, Traits, Allocator> str = substMacroses(macroses, macroText, usedMacroses, changeDot, changeSlash, pEnvCache, flags);
            res.append(str);
           }
       }

    return res;
   }

//-----------------------------------------------------------------------------
template < typename CharType
         , typename Traits
         , typename Allocator
         >
::std::basic_string<CharType, Traits, Allocator>
substMacroses( const std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > &macroses
             , const ::std::basic_string<CharType, Traits, Allocator> &str
             , bool changeDot
             , bool changeSlash
             , std::map< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > *pEnvCache
             , int flags // = smf_ArgsAllowed|smf_ConditionAllowed
             )
   {
    std::set< ::std::basic_string<CharType, Traits, Allocator> > usedMacroses;
    return substMacroses(macroses, str, usedMacroses, changeDot, changeSlash, pEnvCache, flags);
   }




}; // namespace macro
}; // namespace marty


#endif /* MARTY_MACROSES_H */

