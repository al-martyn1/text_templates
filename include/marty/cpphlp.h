#ifndef MARTY_CPPHLP_H
#define MARTY_CPPHLP_H

/*
#if !defined(_IOSTREAM_) && !defined(_STLP_IOSTREAM) && !defined(__STD_IOSTREAM__) && !defined(_CPP_IOSTREAM) && !defined(_GLIBCXX_IOSTREAM)
    #include <iostream>
#endif
*/

#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

#if !defined(_VECTOR_) && !defined(_STLP_VECTOR) && !defined(__STD_VECTOR__) && !defined(_CPP_VECTOR) && !defined(_GLIBCXX_VECTOR)
    #include <vector>
#endif

#if !defined(_ALGORITHM_) && !defined(_STLP_ALGORITHM) && !defined(__STD_ALGORITHM) && !defined(_CPP_ALGORITHM) && !defined(_GLIBCXX_ALGORITHM)
    #include <algorithm>
#endif

#ifndef MARTY_FILENAME_H
    #include <marty/filename.h>
#endif
   
#ifndef MARTY_UTIL_H
    #include <marty/util.h>
#endif

#if !defined(_STDEXCEPT_) && !defined(_STLP_STDEXCEPT) && !defined(__STD_STDEXCEPT) && !defined(_CPP_STDEXCEPT) && !defined(_GLIBCXX_STDEXCEPT)
    #include <stdexcept>
#endif


#if !defined(_SSTREAM_) && !defined(_STLP_SSTREAM) && !defined(__STD_SSTREAM__) && !defined(_CPP_SSTREAM) && !defined(_GLIBCXX_SSTREAM)
    #include <sstream>
#endif


namespace marty
{
namespace cpp
{


//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
::std::basic_string<CharType, Traits, Allocator>
namespaceNameToPathName( const ::std::basic_string<CharType, Traits, Allocator> &ns )
    {
     typedef ::std::basic_string<CharType, Traits, Allocator> string_type;

     typename string_type::size_type pos = 0, size = ns.size();
     string_type resStr; resStr.reserve(size);

     bool semicolonWait2 = false;
     for(; pos!=size; ++pos)
        {
         if (ns[pos]==(CharType)':')
            {
             if (semicolonWait2)
                { // second
                 resStr.append(1,(CharType)'/');
                 semicolonWait2 = false;
                }
             else // first
                {
                 semicolonWait2 = true;
                }
            }
         else // other char
            {
             if (semicolonWait2)
                { // other char after single semicolon. This is an error (in C++), but we accept this
                 resStr.append(1,(CharType)'/');
                 semicolonWait2 = false;
                 resStr.append(1,ns[pos]);
                }
             else
                {
                 resStr.append(1,ns[pos]);
                }
            }
        }
     return resStr;
    }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
void
splitNamespacePathName( const ::std::basic_string<CharType, Traits, Allocator>              &nsAsPath
                  , ::std::vector< ::std::basic_string<CharType, Traits, Allocator> > &nsParts
                  )
   {
    MARTY_FILENAME::splitPath( nsAsPath, nsParts );
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
void
splitNamespaceName( const ::std::basic_string<CharType, Traits, Allocator>                  &ns
                  , ::std::vector< ::std::basic_string<CharType, Traits, Allocator> > &nsParts
                  )
   {
    splitNamespacePathName( namespaceNameToPathName(ns), nsParts );
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
size_t
getNamespacePathNameDepth( const ::std::basic_string<CharType, Traits, Allocator> &nsAsPath )
   {
    ::std::vector< ::std::basic_string<CharType, Traits, Allocator> > nsParts;
    MARTY_FILENAME::splitPath( nsAsPath, nsParts );
    return nsParts.size();
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
size_t
getNamespaceNameDepth( const ::std::basic_string<CharType, Traits, Allocator> &ns )
   {
    return getNamespacePathNameDepth( namespaceNameToPathName(ns) );
   }

//-----------------------------------------------------------------------------
template< typename CharType, bool keepCase, bool bUpper >
struct CToCppNameTransformator
{
    CharType operator()(CharType ch) const
        { 
         if (ch>=(CharType)'0' && ch<=(CharType)'9') return ch;
         if (ch>=(CharType)'A' && ch<=(CharType)'Z') 
            {
             if (keepCase) return ch;
             else if (bUpper) return ch;
             else return (ch - (CharType)'A') + (CharType)'a';
            }
         if (ch>=(CharType)'a' && ch<=(CharType)'z') 
            {
             if (keepCase) return ch;
             else if (!bUpper) return ch;
             else return (ch - (CharType)'a') + (CharType)'A';
            }
         return '_';
        }

    ::std::basic_string<CharType, ::std::char_traits<CharType>, ::std::allocator<CharType> >
    operator()( const ::std::basic_string<CharType, ::std::char_traits<CharType>, ::std::allocator<CharType> > &str)
       {
        typedef ::std::basic_string<CharType, ::std::char_traits<CharType>, ::std::allocator<CharType> > string_type;
        string_type res; res.reserve(str.size());
        typename string_type::const_iterator it = str.begin();
        for(; it!=str.end(); ++it)
           {
            res.append( 1, this->operator()(*it) );
           }
        return res;
       }
};


template< typename CharType >
struct CMakeFirstUpperOtherLower
{
    CToCppNameTransformator< CharType, false, true >  upperTransformer;
    CToCppNameTransformator< CharType, false, false > lowerTransformer;

    CMakeFirstUpperOtherLower() : upperTransformer(), lowerTransformer() {}

    ::std::basic_string<CharType, ::std::char_traits<CharType>, ::std::allocator<CharType> >
    operator()( const ::std::basic_string<CharType, ::std::char_traits<CharType>, ::std::allocator<CharType> > &str)
       {
        typedef ::std::basic_string<CharType, ::std::char_traits<CharType>, ::std::allocator<CharType> > string_type;
        string_type res; res.reserve(str.size());
        typename string_type::const_iterator it = str.begin();
        for(; it!=str.end(); ++it)
           {
            if (it==str.begin()) res.append( 1, upperTransformer(*it) );
            else                 res.append( 1, lowerTransformer(*it) );
           }
        return res;
       }

};


//-----------------------------------------------------------------------------
template <typename CharType> struct CIsUnderscore { bool operator()(CharType ch) const { return ch==(CharType)'_'; } };

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
::std::basic_string<CharType, Traits, Allocator>
generateMacro( const ::std::basic_string<CharType, Traits, Allocator> &name )
   {
    typedef ::std::basic_string<CharType, Traits, Allocator> string_type;
    string_type res = name;
    std::transform(res.begin(), res.end(), res.begin(), CToCppNameTransformator<CharType,false,true>());
    return res;
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
::std::basic_string<CharType, Traits, Allocator>
generateUserMacro( const ::std::basic_string<CharType, Traits, Allocator> &name )
   {
    typedef ::std::basic_string<CharType, Traits, Allocator> string_type;
    string_type res = name;
    std::transform(res.begin(), res.end(), res.begin(), CToCppNameTransformator<CharType,false,true>());
    ::marty::util::trim(res, CIsUnderscore<CharType>());
    return res;
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
void generateGuardDefinePair( const ::std::basic_string<CharType, Traits, Allocator> &name
                            , ::std::basic_string<CharType, Traits, Allocator> &openStr
                            , ::std::basic_string<CharType, Traits, Allocator> &closeStr
                            , const ::std::basic_string<CharType, Traits, Allocator> &optionalValue
                            , const ::std::basic_string<CharType, Traits, Allocator> &firstIndent
                            , const ::std::basic_string<CharType, Traits, Allocator> &relIndent
                            , const ::std::basic_string<CharType, Traits, Allocator> &strIfndef
                            , const ::std::basic_string<CharType, Traits, Allocator> &strDefine
                            , const ::std::basic_string<CharType, Traits, Allocator> &strEndif
                            )
   {
    typedef ::std::basic_string<CharType, Traits, Allocator> string_type;
    string_type macroName = generateMacro( name );

    string_type tmp;

    openStr.append(firstIndent);
    openStr.append(strIfndef);
    openStr.append(1,(CharType)' ');
    openStr.append(macroName);
    openStr.append(1,(CharType)'\n');
    openStr.append(firstIndent);
    openStr.append(relIndent);
    openStr.append(strDefine);
    openStr.append(macroName);
    if (!optionalValue.empty())
       {
        openStr.append(4,(CharType)' ');
        openStr.append(optionalValue);
       }
    openStr.append(1,(CharType)'\n');

    closeStr.append(firstIndent);
    closeStr.append(strEndif);
    closeStr.append(1,(CharType)' ');
    closeStr.append(2,(CharType)'//');
    closeStr.append(1,(CharType)' ');
    closeStr.append(macroName);
    closeStr.append(1,(CharType)'\n');
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
void generateUserGuardDefinePair( const ::std::basic_string<CharType, Traits, Allocator> &name
                            , ::std::basic_string<CharType, Traits, Allocator> &openStr
                            , ::std::basic_string<CharType, Traits, Allocator> &closeStr
                            , const ::std::basic_string<CharType, Traits, Allocator> &optionalValue
                            , const ::std::basic_string<CharType, Traits, Allocator> &firstIndent
                            , const ::std::basic_string<CharType, Traits, Allocator> &relIndent
                            , const ::std::basic_string<CharType, Traits, Allocator> &strIfndef
                            , const ::std::basic_string<CharType, Traits, Allocator> &strDefine
                            , const ::std::basic_string<CharType, Traits, Allocator> &strEndif
                            )
   {
    typedef ::std::basic_string<CharType, Traits, Allocator> string_type;
    string_type macroName = generateUserMacro( name );

    string_type tmp;

    openStr.append(firstIndent);
    openStr.append(strIfndef);
    openStr.append(1,(CharType)' ');
    openStr.append(macroName);
    openStr.append(1,(CharType)'\n');
    openStr.append(firstIndent);
    openStr.append(relIndent);
    openStr.append(strDefine);
    openStr.append(1,(CharType)' ');
    openStr.append(macroName);
    if (!optionalValue.empty())
       {
        openStr.append(4,(CharType)' ');
        openStr.append(optionalValue);
       }
    openStr.append(1,(CharType)'\n');

    closeStr.append(firstIndent);
    closeStr.append(strEndif);
    closeStr.append(1,(CharType)' ');
    closeStr.append(2,(CharType)'//');
    closeStr.append(1,(CharType)' ');
    closeStr.append(macroName);
    closeStr.append(1,(CharType)'\n');
   }

//-----------------------------------------------------------------------------
inline
void generateGuardDefinePair( const ::std::wstring &name
                            , ::std::wstring &openStr
                            , ::std::wstring &closeStr
                            , const ::std::wstring &optionalValue
                            , const ::std::wstring &firstIndent
                            , const ::std::wstring &relIndent = L"    "
                            )
   {
    generateGuardDefinePair( name, openStr, closeStr, optionalValue, firstIndent, relIndent, ::std::wstring(L"#ifndef"), ::std::wstring(L"#define"), ::std::wstring(L"#endif") );
   }

//-----------------------------------------------------------------------------
inline
void generateGuardDefinePair( const ::std::string &name
                            , ::std::string &openStr
                            , ::std::string &closeStr
                            , const ::std::string &optionalValue
                            , const ::std::string &firstIndent
                            , const ::std::string &relIndent = "    "
                            )
   {
    generateGuardDefinePair( name, openStr, closeStr, optionalValue, firstIndent, relIndent, ::std::string("#ifndef"), ::std::string("#define"), ::std::string("#endif") );
   }

//-----------------------------------------------------------------------------
inline
void generateUserGuardDefinePair( const ::std::wstring &name
                            , ::std::wstring &openStr
                            , ::std::wstring &closeStr
                            , const ::std::wstring &optionalValue
                            , const ::std::wstring &firstIndent
                            , const ::std::wstring &relIndent = L"    "
                            )
   {
    generateUserGuardDefinePair( name, openStr, closeStr, optionalValue, firstIndent, relIndent, ::std::wstring(L"#ifndef"), ::std::wstring(L"#define"), ::std::wstring(L"#endif") );
   }

//-----------------------------------------------------------------------------
inline
void generateUserGuardDefinePair( const ::std::string &name
                            , ::std::string &openStr
                            , ::std::string &closeStr
                            , const ::std::string &optionalValue
                            , const ::std::string &firstIndent
                            , const ::std::string &relIndent = "    "
                            )
   {
    generateUserGuardDefinePair( name, openStr, closeStr, optionalValue, firstIndent, relIndent, ::std::string("#ifndef"), ::std::string("#define"), ::std::string("#endif") );
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator, typename TTransformPred >
::std::basic_string<CharType, Traits, Allocator>
generateTypeNameFromNameParts( const ::std::vector< ::std::basic_string<CharType, Traits, Allocator> > &_nameParts
                        , const ::std::basic_string<CharType, Traits, Allocator> &typeNamePrefix
                        , size_t useLastPartsNumber
                        , const TTransformPred &transformPred
                        )
   {
    ::std::vector< ::std::basic_string<CharType, Traits, Allocator> > nameParts = _nameParts;
    if (!nameParts.empty() && !nameParts[0].empty() && nameParts[0][0]==(CharType)'/')
       {
        nameParts[0].erase( 0, 1 );
       }
    //MARTY_FILENAME::splitPath( pathName, nameParts );
    std::transform(nameParts.begin(), nameParts.end(), nameParts.begin(), transformPred );

    if (nameParts.empty())  return typeNamePrefix + ::std::basic_string<CharType, Traits, Allocator>(7, (CharType)'T');
    if (nameParts.size()<2) return typeNamePrefix + nameParts[0];

    if (useLastPartsNumber<1) useLastPartsNumber = 1;
    if (useLastPartsNumber>nameParts.size()) useLastPartsNumber = nameParts.size();


    ::std::vector< ::std::basic_string<CharType, Traits, Allocator> > useParts;
    useParts.reserve(nameParts.size());
    ::std::vector< ::std::basic_string<CharType, Traits, Allocator> >::const_reverse_iterator rit = nameParts.rbegin();
    size_t i = 0;
    for(; rit!=nameParts.rend() && i!=useLastPartsNumber; ++rit, ++i)
       {
        useParts.push_back(*rit);
       }

    ::std::basic_string<CharType, Traits, Allocator> resTypeName = typeNamePrefix;
    rit = useParts.rbegin();
    for(; rit!=useParts.rend(); ++rit)
       {
        resTypeName.append(*rit);
       }
    return resTypeName;
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator, typename TTransformPred >
::std::basic_string<CharType, Traits, Allocator>
generateTypeNameFromPathName( const ::std::basic_string<CharType, Traits, Allocator> &pathName
                        , const ::std::basic_string<CharType, Traits, Allocator> &typeNamePrefix
                        , size_t useLastPartsNumber
                        , const TTransformPred &transformPred
                        )
   {
    ::std::vector< ::std::basic_string<CharType, Traits, Allocator> > nameParts;
    MARTY_FILENAME::splitPath( pathName, nameParts );
    return generateTypeNameFromNameParts( nameParts, typeNamePrefix, useLastPartsNumber, transformPred );
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator, typename TTransformPred >
::std::basic_string<CharType, Traits, Allocator>
generateTypeNameFromName( const ::std::basic_string<CharType, Traits, Allocator> &name
                        , const ::std::basic_string<CharType, Traits, Allocator> &typeNamePrefix
                        , size_t useLastPartsNumber
                        , const TTransformPred &transformPred
                        )
   {
    ::std::vector< ::std::basic_string<CharType, Traits, Allocator> > nameParts;
    MARTY_FILENAME::splitPath( namespaceNameToPathName(name), nameParts );
    return generateTypeNameFromNameParts( nameParts, typeNamePrefix, useLastPartsNumber, transformPred );
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
::std::basic_string<CharType, Traits, Allocator>
generateIdentifier( const ::std::basic_string<CharType, Traits, Allocator> &name )
   {
    typedef ::std::basic_string<CharType, Traits, Allocator> string_type;
    string_type res = name;
    std::transform(res.begin(), res.end(), res.begin(), CToCppNameTransformator<CharType,true,false>());
    return res;
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
::std::basic_string<CharType, Traits, Allocator>
generateUserIdentifier( const ::std::basic_string<CharType, Traits, Allocator> &name )
   {
    typedef ::std::basic_string<CharType, Traits, Allocator> string_type;
    string_type res = name;
    std::transform(res.begin(), res.end(), res.begin(), CToCppNameTransformator<CharType,true,false>());
    ::marty::util::trim(res, CIsUnderscore<CharType>());
    return res;
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
::std::basic_string<CharType, Traits, Allocator>
generateLowerIdentifier( const ::std::basic_string<CharType, Traits, Allocator> &name )
   {
    typedef ::std::basic_string<CharType, Traits, Allocator> string_type;
    string_type res = name;
    std::transform(res.begin(), res.end(), res.begin(), CToCppNameTransformator<CharType,false,false>());
    return res;
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
::std::basic_string<CharType, Traits, Allocator>
generateUserLowerIdentifier( const ::std::basic_string<CharType, Traits, Allocator> &name )
   {
    typedef ::std::basic_string<CharType, Traits, Allocator> string_type;
    string_type res = name;
    std::transform(res.begin(), res.end(), res.begin(), CToCppNameTransformator<CharType,false,false>());
    ::marty::util::trim(res, CIsUnderscore<CharType>());
    return res;
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
void generateNamespacePathNamePair( const ::std::basic_string<CharType, Traits, Allocator>  nsName
                              , ::std::basic_string<CharType, Traits, Allocator>           &openPart
                              , ::std::basic_string<CharType, Traits, Allocator>           &closePart
                              , const ::std::basic_string<CharType, Traits, Allocator>      firstIndent
                              , const ::std::basic_string<CharType, Traits, Allocator>      nextIndent
                              , const ::std::basic_string<CharType, Traits, Allocator>      nsKeyword
                              , bool  makeUserName = true
                              )
   {
    ::std::vector< ::std::basic_string<CharType, Traits, Allocator> > nsParts;
    splitNamespacePathName( nsName, nsParts );

    ::std::vector< ::std::basic_string<CharType, Traits, Allocator> > indents;
    indents.reserve(nsParts.size());

    ::std::basic_string<CharType, Traits, Allocator> curIndent = firstIndent;
    ::std::vector< ::std::basic_string<CharType, Traits, Allocator> >::const_iterator nsIt = nsParts.begin();
    for( ;nsIt!=nsParts.end(); ++nsIt )
       {
        if (nsIt->size()==1 && (*nsIt)[0]==(CharType)'/') continue; // skip unsignificant slash
        openPart.append(curIndent);
        openPart.append(nsKeyword);
        openPart.append(1, (CharType)' ');
        if (makeUserName)
            openPart.append(generateUserIdentifier(*nsIt));
        else
            openPart.append(generateIdentifier(*nsIt));
        openPart.append(1, (CharType)'{');
        openPart.append(1, (CharType)'\n');
        indents.push_back(curIndent);
        curIndent.append(nextIndent);
       }

    ::std::vector< ::std::basic_string<CharType, Traits, Allocator> >::const_reverse_iterator nsRit = nsParts.rbegin();
    ::std::vector< ::std::basic_string<CharType, Traits, Allocator> >::const_reverse_iterator indIt = indents.rbegin();
    for(; nsRit!=nsParts.rend() && indIt!=indents.rend(); ++nsRit, ++indIt)
       {
        if (nsRit->size()==1 && (*nsRit)[0]==(CharType)'/') continue; // skip unsignificant slash
        closePart.append(*indIt);
        closePart.append(1, (CharType)'}');
        closePart.append(1, (CharType)';');
        closePart.append(1, (CharType)' ');
        closePart.append(2, (CharType)'/');
        closePart.append(1, (CharType)' ');
        closePart.append(nsKeyword);
        closePart.append(1, (CharType)' ');
        if (makeUserName)
            closePart.append(generateUserIdentifier(*nsRit));
        else
            closePart.append(generateIdentifier(*nsRit));
        closePart.append(1, (CharType)'\n');
       }
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
void generateNamespaceNamePair( const ::std::basic_string<CharType, Traits, Allocator>      nsName
                              , ::std::basic_string<CharType, Traits, Allocator>           &openPart
                              , ::std::basic_string<CharType, Traits, Allocator>           &closePart
                              , const ::std::basic_string<CharType, Traits, Allocator>      firstIndent
                              , const ::std::basic_string<CharType, Traits, Allocator>      nextIndent
                              , const ::std::basic_string<CharType, Traits, Allocator>      nsKeyword
                              , bool  makeUserName = true
                              )
   {
    generateNamespacePathNamePair<CharType,Traits,Allocator>( namespaceNameToPathName(nsName), openPart, closePart, firstIndent, nextIndent, nsKeyword, makeUserName);
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
::std::basic_string<CharType, Traits, Allocator>
generateNamespacePathNamePairOpen( const ::std::basic_string<CharType, Traits, Allocator>  nsName
                                 , const ::std::basic_string<CharType, Traits, Allocator>      firstIndent
                                 , const ::std::basic_string<CharType, Traits, Allocator>      nextIndent
                                 , const ::std::basic_string<CharType, Traits, Allocator>      nsKeyword
                                 , bool  makeUserName = true
                                 )
   {
    ::std::basic_string<CharType, Traits, Allocator> openPart, closePart;
    generateNamespacePathNamePair<CharType,Traits,Allocator>( nsName, openPart, closePart, firstIndent, nextIndent, nsKeyword, makeUserName );
    return openPart;
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
::std::basic_string<CharType, Traits, Allocator>
generateNamespacePathNamePairClose( const ::std::basic_string<CharType, Traits, Allocator>  nsName
                                 , const ::std::basic_string<CharType, Traits, Allocator>      firstIndent
                                 , const ::std::basic_string<CharType, Traits, Allocator>      nextIndent
                                 , const ::std::basic_string<CharType, Traits, Allocator>      nsKeyword
                                 , bool  makeUserName = true
                                 )
   {
    ::std::basic_string<CharType, Traits, Allocator> openPart, closePart;
    generateNamespacePathNamePair<CharType,Traits,Allocator>( nsName, openPart, closePart, firstIndent, nextIndent, nsKeyword, makeUserName );
    return closePart;
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
::std::basic_string<CharType, Traits, Allocator>
generateNamespaceNamePairOpen( const ::std::basic_string<CharType, Traits, Allocator>  nsName
                                 , const ::std::basic_string<CharType, Traits, Allocator>      firstIndent
                                 , const ::std::basic_string<CharType, Traits, Allocator>      nextIndent
                                 , const ::std::basic_string<CharType, Traits, Allocator>      nsKeyword
                                 , bool  makeUserName = true
                                 )
   {
    ::std::basic_string<CharType, Traits, Allocator> openPart, closePart;
    generateNamespaceNamePair<CharType,Traits,Allocator>( nsName, openPart, closePart, firstIndent, nextIndent, nsKeyword, makeUserName );
    return openPart;
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
::std::basic_string<CharType, Traits, Allocator>
generateNamespaceNamePairClose( const ::std::basic_string<CharType, Traits, Allocator>  nsName
                                 , const ::std::basic_string<CharType, Traits, Allocator>      firstIndent
                                 , const ::std::basic_string<CharType, Traits, Allocator>      nextIndent
                                 , const ::std::basic_string<CharType, Traits, Allocator>      nsKeyword
                                 , bool  makeUserName = true
                                 )
   {
    ::std::basic_string<CharType, Traits, Allocator> openPart, closePart;
    generateNamespaceNamePair<CharType,Traits,Allocator>( nsName, openPart, closePart, firstIndent, nextIndent, nsKeyword, makeUserName );
    return closePart;
   }

//-----------------------------------------------------------------------------
// < ::std::wstring::value_type, ::std::wstring::traits_type, ::std::wstring::allocator_type >
inline
void generateNamespacePathNamePair( const ::std::string &nsName
                                  , ::std::string &openPart
                                  , ::std::string &closePart
                                  , const ::std::string &firstIndent = ""
                                  , const ::std::string &nextIndent  = "    "
                                  )
   {
    generateNamespacePathNamePair( nsName, openPart, closePart, firstIndent, nextIndent, ::std::string("namespace") );
   }

//-----------------------------------------------------------------------------
inline
void generateNamespacePathNamePair( const ::std::wstring &nsName
                                  , ::std::wstring &openPart
                                  , ::std::wstring &closePart
                                  , const ::std::wstring &firstIndent = L""
                                  , const ::std::wstring &nextIndent  = L"    "
                                  )
   {
    generateNamespacePathNamePair( nsName, openPart, closePart, firstIndent, nextIndent, ::std::wstring(L"namespace") );
   }

//-----------------------------------------------------------------------------
inline
void generateNamespaceNamePair( const ::std::string &nsName
                                  , ::std::string &openPart
                                  , ::std::string &closePart
                                  , const ::std::string &firstIndent = ""
                                  , const ::std::string &nextIndent  = "    "
                                  )
   {
    generateNamespaceNamePair( nsName, openPart, closePart, firstIndent, nextIndent, ::std::string("namespace") );
   }

//-----------------------------------------------------------------------------
inline
void generateNamespaceNamePair( const ::std::wstring &nsName
                                  , ::std::wstring &openPart
                                  , ::std::wstring &closePart
                                  , const ::std::wstring &firstIndent = L""
                                  , const ::std::wstring &nextIndent  = L"    "
                                  )
   {
    generateNamespaceNamePair( nsName, openPart, closePart, firstIndent, nextIndent, ::std::wstring(L"namespace") );
   }

//-----------------------------------------------------------------------------
inline
::std::string generateNamespacePathNamePairOpen( const ::std::string &nsName
                                               , const ::std::string &firstIndent = ""
                                               , const ::std::string &nextIndent  = "    "
                                               )
   {
    return generateNamespacePathNamePairOpen( nsName, firstIndent, nextIndent, ::std::string("namespace"));
   }

//-----------------------------------------------------------------------------
inline
::std::string generateNamespaceNamePairOpen( const ::std::string &nsName
                                               , const ::std::string &firstIndent = ""
                                               , const ::std::string &nextIndent  = "    "
                                               )
   {
    return generateNamespaceNamePairOpen( nsName, firstIndent, nextIndent, ::std::string("namespace"));
   }

//-----------------------------------------------------------------------------
inline
::std::string generateNamespacePathNamePairClose( const ::std::string &nsName
                                               , const ::std::string &firstIndent = ""
                                               , const ::std::string &nextIndent  = "    "
                                               )
   {
    return generateNamespacePathNamePairClose( nsName, firstIndent, nextIndent, ::std::string("namespace"));
   }

//-----------------------------------------------------------------------------
inline
::std::string generateNamespaceNamePairClose( const ::std::string &nsName
                                               , const ::std::string &firstIndent = ""
                                               , const ::std::string &nextIndent  = "    "
                                               )
   {
    return generateNamespaceNamePairClose( nsName, firstIndent, nextIndent, ::std::string("namespace"));
   }

//-----------------------------------------------------------------------------
inline
::std::wstring generateNamespacePathNamePairOpen( const ::std::wstring &nsName
                                               , const ::std::wstring &firstIndent = L""
                                               , const ::std::wstring &nextIndent  = L"    "
                                               )
   {
    return generateNamespacePathNamePairOpen( nsName, firstIndent, nextIndent, ::std::wstring(L"namespace"));
   }

//-----------------------------------------------------------------------------
inline
::std::wstring generateNamespaceNamePairOpen( const ::std::wstring &nsName
                                               , const ::std::wstring &firstIndent = L""
                                               , const ::std::wstring &nextIndent  = L"    "
                                               )
   {
    return generateNamespaceNamePairOpen( nsName, firstIndent, nextIndent, ::std::wstring(L"namespace"));
   }

//-----------------------------------------------------------------------------
inline
::std::wstring generateNamespacePathNamePairClose( const ::std::wstring &nsName
                                               , const ::std::wstring &firstIndent = L""
                                               , const ::std::wstring &nextIndent  = L"    "
                                               )
   {
    return generateNamespacePathNamePairClose( nsName, firstIndent, nextIndent, ::std::wstring(L"namespace"));
   }

//-----------------------------------------------------------------------------
inline
::std::wstring generateNamespaceNamePairClose( const ::std::wstring &nsName
                                               , const ::std::wstring &firstIndent = L""
                                               , const ::std::wstring &nextIndent  = L"    "
                                               )
   {
    return generateNamespaceNamePairClose( nsName, firstIndent, nextIndent, ::std::wstring(L"namespace"));
   }








//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator
        , typename FirstLineFormatter
        , typename InnerLineFormatter
        , typename LastLineFormatter
        , typename ItemType
        >
void
generateGenericCode( ::std::basic_string<CharType, Traits, Allocator> &resStr
                   , const ::std::vector< ItemType >                  &items
                   , const FirstLineFormatter                         &firstFormatter
                   , const InnerLineFormatter                         &innerFormatter
                   , const LastLineFormatter                          &lastFormatter
                   )
   {
    firstFormatter( resStr );

    ::std::vector< ItemType >::const_iterator itemIt = items.begin();
    size_t curItemIndex = 0;
    for(; itemIt!=items.end(); ++itemIt, ++curItemIndex)
       {
        bool bFirst = (itemIt == items.begin());
        bool bLast  = (curItemIndex+1 >= items.size());
        innerFormatter( resStr, bFirst, bLast, *itemIt );
       }

    lastFormatter( resStr );   
   }

// http://habrahabr.ru/blogs/cpp/54762/

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator
        , typename FirstLineFormatter
        , typename InnerLineFormatter
        , typename LastLineFormatter
        >
void
generateGenericCode( ::std::basic_string<CharType, Traits, Allocator> &resStr
                   , const ::std::vector< ::std::pair< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > > &items
                   , const FirstLineFormatter &firstFormatter
                   , const InnerLineFormatter &innerFormatter
                   , const LastLineFormatter  &lastFormatter
                   )
   {
    ::std::basic_string<CharType, Traits, Allocator>::size_type maxFirstLen = 0;

    ::std::vector< ::std::pair< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > >::const_iterator itemIt = items.begin();
    for(; itemIt!=items.end(); ++itemIt)
       {
        if (itemIt->first.size() > maxFirstLen) maxFirstLen = itemIt->first.size();
       }
    ++maxFirstLen;

    firstFormatter( resStr );

    itemIt = items.begin();
    size_t curItemIndex = 0;
    for(; itemIt!=items.end(); ++itemIt, ++curItemIndex)
       {
        bool bFirst = (itemIt == items.begin());
        bool bLast  = (curItemIndex+1 >= items.size());
        innerFormatter( resStr, bFirst, bLast, maxFirstLen, itemIt->first, itemIt->second );
       }

    lastFormatter( resStr );   
   }

//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
struct CTypeFirstLastFormatterStub // do-nothing object
{
    CTypeFirstLastFormatterStub() {}
    typedef ::std::basic_string<CharType, Traits, Allocator> string_type;
    void operator()( string_type &str ) const {}
};


template< typename CharType, typename Traits, typename Allocator >
struct CTypeFirstLastFormatterBase
{
    typedef ::std::basic_string<CharType, Traits, Allocator> string_type;
    string_type    indent;
    string_type    keyword;
    string_type    name;
    CTypeFirstLastFormatterBase(const string_type &_indent, const string_type &_keyword, const string_type &_name)
       : indent(_indent), keyword(_keyword), name(_name)
       {}
};

template< typename CharType, typename Traits, typename Allocator >
struct CTypeFirstLineFormatterBase : public CTypeFirstLastFormatterBase< CharType, Traits, Allocator>
{
    typedef ::std::basic_string<CharType, Traits, Allocator>           string_type;
    typedef CTypeFirstLastFormatterBase< CharType, Traits, Allocator>  base_type;
    CTypeFirstLineFormatterBase(const string_type &_indent, const string_type &_keyword, const string_type &_name)
       : base_type(_indent, _keyword, _name)
       {}

    void operator()( string_type &str ) const
       {
        str.append( indent );
        if (!keyword.empty())
           {
            str.append( keyword );
            str.append(1, (CharType)' ');
           }
        if (!name.empty())
           {
            str.append( name );
           }
        str.append(1, (CharType)'{');
        str.append(1, (CharType)'\n');
       }
};

template< typename CharType, typename Traits, typename Allocator >
struct CTypeLastLineFormatterBase : public CTypeFirstLastFormatterBase< CharType, Traits, Allocator>
{
    typedef ::std::basic_string<CharType, Traits, Allocator>           string_type;
    typedef CTypeFirstLastFormatterBase< CharType, Traits, Allocator>  base_type;
    bool    markWithComment;
    CTypeLastLineFormatterBase(const string_type &_indent, const string_type &_keyword, const string_type &_name, bool _markWithComment = true)
       : base_type(_indent, _keyword, _name), markWithComment(_markWithComment)
       {}

    void operator()( string_type &str ) const
       {
        str.append( indent );
        str.append(1, (CharType)'}');
        str.append(1, (CharType)';');
        if (markWithComment)
           {
            str.append(1, (CharType)' ');
            str.append(2, (CharType)'/');
            str.append(1, (CharType)' ');
            if (!keyword.empty())
               {
                str.append( keyword );
                str.append(1, (CharType)' ');
               }
            str.append( name );
           }
        str.append(1, (CharType)'\n');
       }
};


template< typename CharType, typename Traits, typename Allocator >
struct CTypeInnerFormatterBase
{
    typedef ::std::basic_string<CharType, Traits, Allocator> string_type;
    string_type    indent;
    CTypeInnerFormatterBase(const string_type &_indent)
       : indent(_indent)
       {}
};

template< typename CharType, typename Traits, typename Allocator >
struct CEnumInnerFormatter : public CTypeInnerFormatterBase< CharType, Traits, Allocator>
{
    typedef ::std::basic_string<CharType, Traits, Allocator>       string_type;
    typedef CTypeInnerFormatterBase< CharType, Traits, Allocator>  base_type;
    bool    prettyFormat;
    CEnumInnerFormatter(const string_type &_indent, bool _prettyFormat) 
       : base_type(_indent), prettyFormat(_prettyFormat)
       {}

    void operator()( string_type &str, bool bFirst, bool bLast, size_t maxFirstLen, const string_type &first, const string_type &second) const
       {
        str.append( indent );
        str.append( first );
        if (second.size())
           {
            if (first.size() < maxFirstLen)
               {
                if (prettyFormat)
                   str.append( maxFirstLen - first.size(), (CharType)' ');
                else
                   str.append( 1, (CharType)' ');
               }
            str.append(1, (CharType)'=');
            str.append(1, (CharType)' ');
            str.append( second );
           }
        if (!bLast)
           str.append(1, (CharType)','); // not last item
        str.append(1, (CharType)'\n');
       }
};


// sixml markup support
template< typename CharType, typename Traits, typename Allocator >
struct CEnumSixmlMetaInnerFormatter : public CTypeInnerFormatterBase< CharType, Traits, Allocator>
{
    typedef ::std::basic_string<CharType, Traits, Allocator>       string_type;
    typedef CTypeInnerFormatterBase< CharType, Traits, Allocator>  base_type;
    bool prettyFormat;
    bool keepCase;
    bool asFlags;
    string_type  nsName;
    string_type  enumName;
    string_type  enumMetaFuncName;

    CEnumSixmlMetaInnerFormatter(const string_type &_indent, bool pf, bool kc, bool af, const string_type &_nsName, const string_type &_enumName, const string_type &_enumMetaFuncName) 
       : base_type(_indent), prettyFormat(pf), keepCase(kc), asFlags(af), nsName(_nsName), enumName(_enumName), enumMetaFuncName(_enumMetaFuncName)
       {}

    string_type makeStrTQuoted( const string_type &str) const
       {
        string_type res;
        res.append( 1, (CharType)'_');
        res.append( 1, (CharType)'T');
        res.append( 1, (CharType)'(');
        res.append( 1, (CharType)'\"');
        res.append(str);
        res.append( 1, (CharType)'\"');
        res.append( 1, (CharType)')');
        return res;
       }

    void operator()( string_type &str, bool bFirst, bool bLast, size_t maxFirstLen, const string_type &first, const string_type &second) const
       {
        ::std::basic_ostringstream<CharType> os;

        string_type fqEnumName = ::marty::cpp::makeFullQualifiedCppName( nsName, enumName );
        string_type fqEnumItemName = fqEnumName;
        fqEnumItemName.append(2,(CharType)':');
        fqEnumItemName.append(first);
        if (asFlags) 
           {
            fqEnumItemName = second; //
            if (fqEnumItemName.empty()) fqEnumItemName.append(1,(CharType)'0');
           }

        //maxFirstLen 
        os<<indent<<enumMetaFuncName<< (CharType)'(' << (CharType)' ' << fqEnumItemName;
        if (prettyFormat)
           {
            if (!asFlags)
               os<< string_type( maxFirstLen - first.size(), (CharType)' ');
            else
               os<< string_type( (fqEnumItemName.size()>12 ? (size_t)0 : (size_t)12-fqEnumItemName.size()), (CharType)' ');
           }
        else
           os<< string_type( 1, (CharType)' ');

        //asFlags
        string_type strOrg   = first; // asFlags ? first : second;
        string_type strLower = strOrg;
        string_type strUpper = strOrg;
        std::transform(strLower.begin(), strLower.end(), strLower.begin(), CToCppNameTransformator<CharType, false, false>());
        std::transform(strUpper.begin(), strUpper.end(), strUpper.begin(), CToCppNameTransformator<CharType, false, true>());

        os<< (CharType)',' << (CharType)' ' << makeStrTQuoted(strOrg);
        if (!keepCase)
           {
            if (strOrg!=strLower)
               os << (CharType)',' << (CharType)' ' << makeStrTQuoted(strLower);
            if (strOrg!=strUpper)
               os << (CharType)',' << (CharType)' ' << makeStrTQuoted(strUpper);
           }
        os << (CharType)' ' << (CharType)')' << (CharType)';' << (CharType)'\n';
        str.append( os.str() );
       }
};


template< typename CharType, typename Traits, typename Allocator >
struct CEnumSixmlMetaFirstLastLineFormatter : public CTypeFirstLastFormatterBase< CharType, Traits, Allocator>
{
    typedef ::std::basic_string<CharType, Traits, Allocator>           string_type;
    typedef CTypeFirstLastFormatterBase< CharType, Traits, Allocator>  base_type;
    CEnumSixmlMetaFirstLastLineFormatter(const string_type &_indent, const string_type &_keyword, const string_type &_name)
       : base_type(_indent, _keyword, _name)
       {}

    void operator()( string_type &str ) const
       {
        str.append( indent );
        str.append( keyword );
        str.append(1, (CharType)'(');
        str.append( name );
        str.append(1, (CharType)')');
        str.append(1, (CharType)'\n');
       }
};

//CEnumSixmlMetaFirstLastLineFormatter(const string_type &_indent, const string_type &_keyword, const string_type &_name)
//struct CEnumSixmlMetaInnerFormatter : public CTypeInnerFormatterBase< CharType, Traits, Allocator>



//innerFormatter( resStr, bFirst, bLast, maxItemSize, itemIt->first, itemIt->second );
//innerFormatter( resStr, bFirst, bLast, *itemIt );


//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
void
generateEnum( ::std::basic_string<CharType, Traits, Allocator> &resStr
            , const ::std::basic_string<CharType, Traits, Allocator> &enumName
            , const ::std::vector< ::std::pair< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > > &enumItems
            , const ::std::basic_string<CharType, Traits, Allocator> &firstIndent
            , const ::std::basic_string<CharType, Traits, Allocator> &relIndent // relative from first line indent
            , bool  prettyFormat
            , const ::std::basic_string<CharType, Traits, Allocator> &enumKeyword
            , bool  markEndWithComment
            )
   {
    // (16 chars per item + firstIndent + relIndent ) * sizeof vector
    resStr.reserve( resStr.size() + (firstIndent.size() + relIndent.size() + 16) * enumItems.size() + enumKeyword.size() + firstIndent.size()*2 + enumName.size() + 16);
    generateGenericCode( resStr, enumItems
                       , CTypeFirstLineFormatterBase<CharType, Traits, Allocator>( firstIndent, enumKeyword, enumName)
                       , CEnumInnerFormatter<CharType, Traits, Allocator>(firstIndent+relIndent, prettyFormat)
                       , CTypeLastLineFormatterBase<CharType, Traits, Allocator>( firstIndent, enumKeyword, enumName, markEndWithComment)
                       );
   }


template< typename CharType, typename Traits, typename Allocator >
::std::basic_string<CharType, Traits, Allocator>
generateEnum( const ::std::basic_string<CharType, Traits, Allocator> &enumName
            , const ::std::vector< ::std::pair< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > > &enumItems
            , const ::std::basic_string<CharType, Traits, Allocator> &firstIndent
            , const ::std::basic_string<CharType, Traits, Allocator> &relIndent // relative from first line indent
            , bool  prettyFormat
            , const ::std::basic_string<CharType, Traits, Allocator> &enumKeyword
            , bool  markEndWithComment
            )
   {
    ::std::basic_string<CharType, Traits, Allocator> res;
    generateEnum( res, enumName, enumItems, firstIndent, relIndent, prettyFormat, enumKeyword, markEndWithComment );
    return res;
   }

//-----------------------------------------------------------------------------
inline
void
generateEnum( ::std::wstring &res
            , const ::std::wstring &enumName
            , const ::std::vector< ::std::pair< ::std::wstring, ::std::wstring > > &enumItems
            , const ::std::wstring &firstIndent = L""
            , const ::std::wstring &relIndent   = L"    "
            , bool  prettyFormat                = true
            )
   {
    generateEnum( res, enumName, enumItems, firstIndent, relIndent, prettyFormat, ::std::wstring(L"enum"), true );
   }

//-----------------------------------------------------------------------------
inline
void
generateEnum( ::std::string &res
            , const ::std::string &enumName
            , const ::std::vector< ::std::pair< ::std::string, ::std::string > > &enumItems
            , const ::std::string &firstIndent = ""
            , const ::std::string &relIndent   = "    "
            , bool  prettyFormat               = true
            )
   {
    generateEnum( res, enumName, enumItems, firstIndent, relIndent, prettyFormat, ::std::string("enum"), true );
   }

//-----------------------------------------------------------------------------

inline
::std::wstring
generateEnum( const ::std::wstring &enumName
            , const ::std::vector< ::std::pair< ::std::wstring, ::std::wstring > > &enumItems
            , const ::std::wstring &firstIndent = L""
            , const ::std::wstring &relIndent   = L"    "
            , bool  prettyFormat                = true
            )
   {
    return generateEnum( enumName, enumItems, firstIndent, relIndent, prettyFormat, ::std::wstring(L"enum"), true );
   }

//-----------------------------------------------------------------------------
inline
::std::string
generateEnum( const ::std::string &enumName
            , const ::std::vector< ::std::pair< ::std::string, ::std::string > > &enumItems
            , const ::std::string &firstIndent = ""
            , const ::std::string &relIndent   = "    "
            , bool  prettyFormat               = true
            )
   {
    return generateEnum( enumName, enumItems, firstIndent, relIndent, prettyFormat, ::std::string("enum"), true );
   }


/*
struct CStruct{

    string     str;
    int        b;

    CStruct( string _str, int _b)
       : str(_str)
       , b(_b)
       {}

    CStruct( const CStruct &cs )
       : str(cs.str)
       , b(cs.b)
       {
       }

    void swap( const CStruct &cs )
       {
        str.swap(cs.str);
        ::std::swap(b, cs.b);
       }

    CStruct& operator=( const CStruct &cs )
       {
        if (&cs==this) return *this;
        CStruct tmp(cs);
        this->swap(tmp);
        return *this;
       }
};
*/

template< typename string_type >
class CGuardedDeclaration
{
    protected:

        string_type &strGuardedContent;
        string_type closingExpr;

        CGuardedDeclaration( const CGuardedDeclaration &);
        CGuardedDeclaration&operator=( const CGuardedDeclaration &);

        CGuardedDeclaration(string_type &content) : strGuardedContent(content), closingExpr() {}
    public:
        CGuardedDeclaration(string_type &content, const string_type &_startingExpr, const string_type &_closingExpr )
            : strGuardedContent(content), closingExpr(_closingExpr) 
            {
             strGuardedContent.append(_startingExpr);
            }
        ~CGuardedDeclaration() { strGuardedContent.append(closingExpr); }
};


template< typename string_type >
struct CIncludeGuardT : public CGuardedDeclaration<string_type>
{
    typedef CGuardedDeclaration<string_type>           base_type;
    CIncludeGuardT(string_type &content, const string_type &name) : base_type(content)
       {
        string_type startingExpr;
        generateGuardDefinePair( name, startingExpr, closingExpr, string_type()  /* val */ , string_type()  /* indent */ );
        content.append(startingExpr);
       }
};

typedef CIncludeGuardT< ::std::string >  CIncludeGuard;
typedef CIncludeGuardT< ::std::wstring > CIncludeGuardW;


template< typename string_type >
struct CUserIncludeGuardT : public CGuardedDeclaration<string_type>
{
    typedef CGuardedDeclaration<string_type>           base_type;
    CUserIncludeGuardT(string_type &content, const string_type &name) : base_type(content)
       {
        string_type startingExpr;
        generateUserGuardDefinePair( name, startingExpr, closingExpr, string_type()  /* val */ , string_type()  /* indent */, string_type() );
        content.append(startingExpr);
       }
};

typedef CUserIncludeGuardT< ::std::string >  CUserIncludeGuard;
typedef CUserIncludeGuardT< ::std::wstring > CUserIncludeGuardW;



template< typename CharType, typename Traits, typename Allocator >
void
makeFullQualifiedCppNamePath( const ::std::basic_string<CharType, Traits, Allocator> &ns
                        , const ::std::basic_string<CharType, Traits, Allocator> &typeOnlyName
                        , ::std::vector< ::std::basic_string<CharType, Traits, Allocator> > &resPath
                         )
   {
    typedef ::std::basic_string<CharType, Traits, Allocator> string_type;
    string_type nsAsPath = namespaceNameToPathName(ns);
    splitNamespacePathName( nsAsPath, resPath );
    resPath.push_back(typeOnlyName);    
    std::transform(resPath.begin(), resPath.end(), resPath.begin(), CToCppNameTransformator<CharType,true,true>());
   }

template< typename CharType, typename Traits, typename Allocator >
::std::basic_string<CharType, Traits, Allocator>
makeFullQualifiedCppName( const ::std::basic_string<CharType, Traits, Allocator> &ns, const ::std::basic_string<CharType, Traits, Allocator> &typeOnlyName )
   {
    typedef ::std::basic_string<CharType, Traits, Allocator> string_type;

    ::std::vector<string_type> pathParts;
    makeFullQualifiedCppNamePath(ns, typeOnlyName, pathParts);

    string_type res;
    ::std::vector<string_type>::const_iterator ppIt = pathParts.begin();
    for(; ppIt!=pathParts.end(); ++ppIt)
       {
        res.append(2, (CharType)':');
        res.append(*ppIt);
       }
    return res;
   }

template< typename CharType, typename Traits, typename Allocator >
::std::basic_string<CharType, Traits, Allocator>
makeFullQualifiedCppNameAsOne( const ::std::basic_string<CharType, Traits, Allocator> &ns, const ::std::basic_string<CharType, Traits, Allocator> &typeOnlyName )
   {
    typedef ::std::basic_string<CharType, Traits, Allocator> string_type;

    ::std::vector<string_type> pathParts;
    makeFullQualifiedCppNamePath(ns, typeOnlyName, pathParts);

    string_type res;
    ::std::vector<string_type>::const_iterator ppIt = pathParts.begin();
    for(; ppIt!=pathParts.end(); ++ppIt)
       {
        if (!res.empty()) res.append(1, (CharType)'_');
        res.append(*ppIt);
       }
    return res;
   }


//-----------------------------------------------------------------------------
template< typename CharType, typename Traits, typename Allocator >
void
generateEnumSixmlMeta( ::std::basic_string<CharType, Traits, Allocator> &resStr
            , const ::std::basic_string<CharType, Traits, Allocator> &enumNs
            , const ::std::basic_string<CharType, Traits, Allocator> &enumName
            , const ::std::vector< ::std::pair< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > > &enumItems
            , const ::std::basic_string<CharType, Traits, Allocator> &firstIndent
            , const ::std::basic_string<CharType, Traits, Allocator> &relIndent // relative from first line indent
            , bool  prettyFormat
            , bool  keepCase
            , bool  asFlags
            , const ::std::basic_string<CharType, Traits, Allocator> &beginKeyword
            , const ::std::basic_string<CharType, Traits, Allocator> &endKeyword
            , const ::std::basic_string<CharType, Traits, Allocator> &innerKeyword
            )
   {
    // (16 chars per item + firstIndent + relIndent ) * sizeof vector
    resStr.reserve( resStr.size() + (firstIndent.size() + relIndent.size() + innerKeyword.size() + 32) * enumItems.size() + beginKeyword.size() + endKeyword.size() + firstIndent.size()*2 + 16);
    ::std::basic_string<CharType, Traits, Allocator> asOne = makeFullQualifiedCppNameAsOne(enumNs, enumName);
    generateGenericCode( resStr, enumItems
                       , CEnumSixmlMetaFirstLastLineFormatter<CharType, Traits, Allocator>( firstIndent, beginKeyword, asOne)
                       , CEnumSixmlMetaInnerFormatter<CharType, Traits, Allocator>(firstIndent+relIndent, prettyFormat, keepCase, asFlags, enumNs, enumName, innerKeyword)
                       , CEnumSixmlMetaFirstLastLineFormatter<CharType, Traits, Allocator>( firstIndent, endKeyword, asOne)
                       );
   }



template< typename CharType, typename Traits, typename Allocator >
::std::basic_string<CharType, Traits, Allocator>
generateEnumSixmlMeta(const ::std::basic_string<CharType, Traits, Allocator> &enumNs
            , const ::std::basic_string<CharType, Traits, Allocator> &enumName
            , const ::std::vector< ::std::pair< ::std::basic_string<CharType, Traits, Allocator>, ::std::basic_string<CharType, Traits, Allocator> > > &enumItems
            , const ::std::basic_string<CharType, Traits, Allocator> &firstIndent
            , const ::std::basic_string<CharType, Traits, Allocator> &relIndent // relative from first line indent
            , bool  prettyFormat
            , bool  keepCase
            , bool  asFlags
            , const ::std::basic_string<CharType, Traits, Allocator> &beginKeyword
            , const ::std::basic_string<CharType, Traits, Allocator> &endKeyword
            , const ::std::basic_string<CharType, Traits, Allocator> &innerKeyword
            )
   {
    ::std::basic_string<CharType, Traits, Allocator> res;
    generateEnumSixmlMeta( res, enumNs, enumName, enumItems, firstIndent, relIndent, prettyFormat, keepCase, asFlags, beginKeyword, endKeyword, innerKeyword );
    return res;
   }

inline void generateEnumSixmlMeta(::std::string &resStr
            , const ::std::string &enumNs
            , const ::std::string &enumName
            , const ::std::vector< ::std::pair< ::std::string, ::std::string > > &enumItems
            , const ::std::string &firstIndent
            , const ::std::string &relIndent // relative from first line indent
            , bool  prettyFormat
            , bool  keepCase
            , bool  asFlags = false
            )
   {
    generateEnumSixmlMeta( resStr, enumNs, enumName, enumItems, firstIndent, relIndent, prettyFormat, keepCase, asFlags, ::std::string("BEGIN_ENUM_META_DATA"), ::std::string("END_ENUM_META_DATA"), ::std::string("enumerate") );
   }

inline void generateEnumSixmlMeta(::std::wstring &resStr
            , const ::std::wstring &enumNs
            , const ::std::wstring &enumName
            , const ::std::vector< ::std::pair< ::std::wstring, ::std::wstring > > &enumItems
            , const ::std::wstring &firstIndent
            , const ::std::wstring &relIndent // relative from first line indent
            , bool  prettyFormat
            , bool  keepCase
            , bool  asFlags = false
            )
   {
    generateEnumSixmlMeta( resStr, enumNs, enumName, enumItems, firstIndent, relIndent, prettyFormat, keepCase, asFlags, ::std::wstring(L"BEGIN_ENUM_META_DATA"), ::std::wstring(L"END_ENUM_META_DATA"), ::std::wstring(L"enumerate") );
   }



// class/struct
// method - private/protected/public/disabled
// type - integral/complex ( has swap )


template< typename CharType, typename Traits, typename Allocator >
struct CClassMemberInfo
{
    typedef ::std::basic_string<CharType, Traits, Allocator>  string_type;

    // type flags
    static const unsigned int tfTypeFComplex            = 0x00000001;
    static const unsigned int tfTypeFZeroInit           = 0x00000002;      // can be initialized with zero
    static const unsigned int tfTypeFHasSwap            = 0x00000004;      // user type has swap method
    
    // common type types                         
    static const unsigned int tfTypeIntegral            = 0x00000002;      // intregral 
    static const unsigned int tfTypeIntegralNoZeroInit  = 0x00000000;      // can't be initialized with zero
    static const unsigned int tfTypeComplex             = 0x00000001;      // typeFComplex
    static const unsigned int tfTypeComplexHasSwap      = 0x00000005;      // typeFComplex|typeFHasSwap
    static const unsigned int tfTypeMask                = 0x000000FF;      // typeFComplex|typeFHasSwap

    static const unsigned int tfVisibilityFMask          = 0xFF000000;
    static const unsigned int tfVisibilityFPublic        = 0x00000000;
    static const unsigned int tfVisibilityFProtected     = 0x01000000;
    static const unsigned int tfVisibilityFPrivate       = 0x02000000;
    
    static const unsigned int tfSixmlSimple             = 0x00000000;
    static const unsigned int tfSixmlFAttr              = 0x00000100;
    static const unsigned int tfSixmlAttr               = 0x00000100;
    static const unsigned int tfSixmlFSelf              = 0x00000200; // for complex or vectors no subtag used
    static const unsigned int tfSixmlFVector            = 0x00000400;
    static const unsigned int tfSixmlVector             = 0x00000400;
    static const unsigned int tfSixmlAttrVector         = 0x00000500;
    static const unsigned int tfSixmlMask               = 0x0000FF00;

    unsigned int  typeFlags;
    string_type   typeName;
    string_type   memberName;

    string_type   defValue; // used in ctor and layout
    // sixm support members
    string_type   tagName; // or attr
    string_type   itemTagName; // for vectors
    
    CClassMemberInfo() 
       : typeFlags(0)
       , typeName()
       , memberName()
       , tagName()
       , itemTagName()
       {}
    CClassMemberInfo( unsigned int _typeFlags
                    , const string_type &_typeName
                    , const string_type &_memberName
                    , const string_type &_tagName     = string_type()
                    , const string_type &_defValue    = string_type()
                    , const string_type &_itemTagName = string_type()
                    )
       : typeFlags(_typeFlags)
       , typeName(_typeName)
       , memberName(_memberName)
       , defValue(_defValue)
       , tagName(_tagName)
       , itemTagName(_itemTagName)
       {
        if (defValue.empty() && (typeFlags&tfTypeFZeroInit))
           defValue.append( 1, (CharType)'0' );
        if (tagName.empty()) tagName = memberName;
       }
};



template< typename CharType, typename Traits=::std::char_traits<CharType>, typename Allocator=::std::allocator<CharType> >
struct CClassInfoT
{
    //static const unsigned int cfVisibilityFNoImpl     = 0x08;
    static const unsigned int cfVisibilityMask        = 0xFF000000;
    static const unsigned int cfVisibilityFNoImpl      = 0x08000000;
    static const unsigned int cfVisibilityFPublic      = 0x00000000;
    static const unsigned int cfVisibilityFProtected   = 0x01000000;
    static const unsigned int cfVisibilityFPrivate     = 0x02000000;
    static const unsigned int cfVisibilityFDisabled    = 0x08000000; // same as cfVisibilityFNoImpl
    static const unsigned int cfVisibilityPublic       = 0x00000000;
    static const unsigned int cfVisibilityProtected    = 0x01000000;
    static const unsigned int cfVisibilityPrivate      = 0x02000000;
    static const unsigned int cfVisibilityDisabled     = 0x0A000000; // 0x02000000|0x08000000 - cfVisibilityFNoImpl|cfVisibilityPrivate
    static const unsigned int cfStruct                 = 0x00000000;
    static const unsigned int cfClass                  = 0x02000000;
    
    static const unsigned int cfClassMethodMask        = 0x000000FF;
    static const unsigned int cfClassMethodCtor        = 0x00000001; // default ctor
    static const unsigned int cfClassMethodInitCtor    = 0x00000002; // init ctor
    static const unsigned int cfClassMethodCopyCtor    = 0x00000003; // copy ctor
    static const unsigned int cfClassMethodSwap        = 0x00000004;
    static const unsigned int cfClassMethodAssignOp    = 0x00000005;
    static const unsigned int cfClassMethodSixmlLayout = 0x00000006; // really it's not a method, but a inner struct

    typedef ::std::basic_string<CharType, Traits, Allocator>  string_type;
    typedef CClassMemberInfo<CharType, Traits, Allocator>     CMemberInfo;

    string_type                                               className;
    unsigned int                                              defaultVisibility;
    ::std::vector< unsigned int >                             methods; // cfClassMethod*
    ::std::vector< CMemberInfo >                              members;

    CClassInfoT() : className(), defaultVisibility(cfStruct), methods(), members() {}
    CClassInfoT( const string_type &cn, unsigned int dv = cfStruct) : className(cn), defaultVisibility(dv), methods(), members() {}


    typename ::std::vector< CMemberInfo >::const_iterator findMember( const string_type &mn ) const
       {
        typename ::std::vector< CMemberInfo >::const_iterator it = members.begin();
        for(; it!=members.end(); ++it)
           if (it->memberName==mn) break;
        return it;
       }

    typename ::std::vector< CMemberInfo >::iterator findMember( const string_type &mn )
       {
        typename ::std::vector< CMemberInfo >::iterator it = members.begin();
        for(; it!=members.end(); ++it)
           if (it->memberName==mn) break;
        return it;
       }

    bool memberExist(const string_type &mn)
       {
        if (findMember(mn)==members.end()) return true;
        return false;
       }

    // true if mn is unique (or new unique name was generated)
    bool generateMemberName( string_type &mn, bool bVec ) const
       {
        const CharType suffixes[] = { (CharType)'s', (CharType)'2', (CharType)'3', (CharType)'4', (CharType)'5', (CharType)'6', (CharType)'7', (CharType)'8', (CharType)'9', (CharType)'_' };
        if (findMember(mn)==members.end()) return true;
        string_type resMemberName;
        size_t suff = 0, size = sizeof(suffixes) / sizeof(suffixes[0]);
        if (!bVec) ++suff;
        for(; suff!=size; ++suff)
           {
            ::std::string testCppName = mn;
            testCppName.append(1, suffixes[suff]);
            if (findMember(testCppName)==members.end()) { mn = testCppName; return true; }
           }

        suff = 0;
        if (!bVec) ++suff;
        ::std::string testCppName = mn;
        for(; suff!=size; ++suff)
           {
            testCppName.append(1, suffixes[suff]);
            if (findMember(testCppName)==members.end()) { mn = testCppName; return true; }
           }

        return false;
       }


    void generateCodeAppendTo( string_type &str
                             , const string_type &firstIndent
                             , const string_type &relIndent
                             ) const
       {
        ::std::basic_ostringstream<CharType> os;
        generateCode( os, firstIndent, relIndent );
        str.append( os.str() );
       }

    static
    string_type toStr( const char *s, string_type &strTo )
       {
        strTo.clear();
        while(*s)
           {
            strTo.append( 1, (CharType)*s);
            ++s;
           }
        return strTo;
       }

    string_type makeVectorTypeName( const string_type &typeName )
       {
        string_type tmp1, tmp2;
        return toStr( "::std::vector< ", tmp1 ) + typeName + toStr( " >", tmp2 );
       }
     


    static
    string_type getVisibilityName( unsigned int v )
       {
        string_type tmp;
        switch(v)
           {
            case cfVisibilityFPublic   :  return toStr("public"    ,tmp);
            case cfVisibilityFProtected:  return toStr("protected" ,tmp);
            case cfVisibilityFPrivate  :  return toStr("private"   ,tmp);
            default:                      return toStr("unknown"   ,tmp);
           }
       }

        void addSimplePod( const string_type &typeName, const string_type &memberName
                         , const string_type &tagName = string_type()
                         , const string_type &defVal  = string_type()
                         );

        void addSimplePodVector( const string_type &typeName, const string_type &memberName
                         , const string_type &itemTagName
                         , const string_type &tagName     = string_type()
                         );

        void addComplex( const string_type &typeName, const string_type &memberName
                         , bool hasSwap
                         , const string_type &tagName = string_type()
                         , const string_type &defVal  = string_type()
                         , const string_type &layoutDefVal = string_type()
                         );

        void addComplexVector( const string_type &typeName, const string_type &memberName
                         , const string_type &itemTagName
                         , const string_type &tagName     = string_type()
                         );

        void addAttrPod( const string_type &typeName, const string_type &memberName
                       , const string_type &attrName
                       , const string_type &defVal   = string_type()
                       );
        
        void addAttrPodVector( const string_type &typeName, const string_type &memberName
                       , const string_type &attrName
                       );
        
        // string type is integral in sixml terms but can't be initialized with zero
        void addSimpleString( const string_type &typeName, const string_type &memberName
                         , const string_type &tagName = string_type()
                         , const string_type &defVal  = string_type()
                         );
        
        void addSimpleStringVector( const string_type &typeName, const string_type &memberName
                         , const string_type &itemTagName
                         , const string_type &tagName     = string_type()
                         );
        
        void addAttrString( const string_type &typeName, const string_type &memberName
                       , const string_type &attrName
                       , const string_type &defVal   = string_type()
                       );
        
        void addAttrStringVector( const string_type &typeName, const string_type &memberName
                       , const string_type &attrName
                       );
/*
        void addComplex( const string_type &typeName, const string_type &memberName
                       , const string_type &tagName = string_type()
                       , const string_type &defVal = string_type(), bool hasSwapMethod = true
                       );
*/
/*
    CClassMemberInfo( unsigned int _typeFlags
                    , const string_type &_typeName
                    , const string_type &_memberName
                    , const string_type &_tagName     = string_type()
                    , const string_type &_defValue    = string_type()
                    , const string_type &_itemTagName = string_type()
                    )
*/


    void generateCode( ::std::basic_ostream<CharType> &os
                     , const string_type &firstIndent
                     , const string_type &relIndent
                     ) const
       {
        string_type vIndent = firstIndent; vIndent.append(relIndent);
        string_type indent  = vIndent;     indent.append(relIndent);

        unsigned int curVisibility = defaultVisibility; // structs has public def visibility (0), classes - private (2)
        if (curVisibility!=cfStruct) curVisibility = cfClass;

        string_type t; // tmp
        os<<firstIndent<< (curVisibility ? toStr("class", t) : toStr("struct", t) ) << toStr(" ", t) << className<<toStr("\n", t);
        os<<firstIndent<<toStr("{\n", t);

        typename string_type::size_type maxMemberTypeNameLen = 0;
        typename string_type::size_type maxMemberNameLen     = 0;

        ::std::vector< CMemberInfo >::const_iterator mit = members.begin();
        for(; mit!=members.end(); ++mit)
           {
            if (mit->typeName.size()>maxMemberTypeNameLen) maxMemberTypeNameLen = mit->typeName.size();
            if (mit->memberName.size()>maxMemberNameLen)   maxMemberNameLen     = mit->memberName.size();
           }

        for(mit=members.begin(); mit!=members.end(); ++mit)
           {
            unsigned int memberVisibility = mit->typeFlags&CMemberInfo::tfVisibilityFMask;
            if (curVisibility!=memberVisibility)
               {
                curVisibility = memberVisibility;
                if (mit!=members.begin())
                   os<<toStr("\n", t);
                os<<vIndent<<getVisibilityName(curVisibility)<<toStr(":\n", t);
               }
            os<<indent<<mit->typeName
              <<string_type( maxMemberTypeNameLen - mit->typeName.size() + 4, (CharType)' ')
              <<mit->memberName<<toStr(";\n", t);
           }

        bool hasSwap = false;
        os<<toStr("\n", t);
        ::std::vector< unsigned int >::const_iterator thit = methods.begin();
        for(; thit!=methods.end(); ++thit)
           {
            if (  ((*thit) & cfClassMethodMask) == cfClassMethodSwap 
               && ((*thit) & cfVisibilityMask ) != cfVisibilityDisabled )
               hasSwap = true;
           }

        bool hasAssignUsedSwap = false;
        thit = methods.begin();
        for(; thit!=methods.end(); ++thit)
           {
            if (((*thit) & cfClassMethodMask) == cfClassMethodAssignOp)
               {
                if (hasSwap) hasAssignUsedSwap = true;
               }
           }

        //::std::vector< unsigned int >::const_iterator 
        thit = methods.begin();
        for(; thit!=methods.end(); ++thit)
           {
            unsigned int methodFlags = (unsigned int)*thit;
            unsigned int methodVisibility = (methodFlags & cfVisibilityMask) & ~cfVisibilityFDisabled;
            //unsigned int methodVisibility = ((*thit) & cfVisibilityMask);
            if (curVisibility!=methodVisibility)
               {
                curVisibility = methodVisibility;
                //if (mit!=members.begin())
                //   os<<toStr("\n", t);
                os<<vIndent<<getVisibilityName(curVisibility)<<toStr(":\n", t);
               }

            bool declOnly = false;
            if ((curVisibility == cfVisibilityPrivate) && (methodFlags&cfVisibilityFDisabled))
               declOnly = true;

            //cfVisibilityFDisabled
            switch( (*thit & (cfClassMethodMask)) )
               {
                case cfClassMethodCtor:
                        generateCtor( os, indent, maxMemberTypeNameLen, maxMemberNameLen, declOnly );
                        break;
                case cfClassMethodInitCtor:
                        generateInitCtor( os, indent, maxMemberTypeNameLen, maxMemberNameLen, declOnly );
                        break;
                case cfClassMethodCopyCtor:
                        generateCopyCtor( os, indent, maxMemberTypeNameLen, maxMemberNameLen, declOnly && !hasAssignUsedSwap );
                        break;
                case cfClassMethodSwap:
                        generateSwap( os, indent, maxMemberTypeNameLen, maxMemberNameLen, declOnly );
                        break;
                case cfClassMethodAssignOp:
                        if (hasSwap) generateAssignOp      ( os, indent, maxMemberTypeNameLen, maxMemberNameLen, declOnly );
                        else         generateAssignOpNoSwap( os, indent, maxMemberTypeNameLen, maxMemberNameLen, declOnly );
                        break;
                case cfClassMethodSixmlLayout:
                        generateSixmlLayout( os, indent, maxMemberTypeNameLen, maxMemberNameLen, declOnly );
                        break;
                //default:
               }
            os<<toStr("\n", t);
           }

        os<<toStr("\n", t)<<firstIndent<< toStr("}; // ", t) <<className <<toStr("\n", t);
       }

    void generateCtor( ::std::basic_ostream<CharType> &os
                     , const string_type &indent
                     , typename string_type::size_type maxMemberTypeNameLen
                     , typename string_type::size_type maxMemberNameLen
                     , bool declarationOnly
                     ) const
       {
        string_type t; // tmp
        os<<indent<<className<<toStr("()", t);
        if (declarationOnly) { os<<toStr(";\n", t); return; }

        os<<toStr("\n", t);

        ::std::vector< CMemberInfo >::const_iterator mit = members.begin();
        for(; mit!=members.end(); ++mit)
           {
            os<<indent<<toStr("   ", t);

            if (mit==members.begin()) os<<toStr(": ", t);
            else                      os<<toStr(", ", t);

            os<<mit->memberName<<toStr("(", t);

            if (!mit->defValue.empty())                                os<<mit->defValue;
            else if (mit->typeFlags & CMemberInfo::tfTypeFZeroInit)    os<<(CharType)'0';

            os<<toStr(")\n", t);
           }       
        os<<indent<<toStr("   {}\n", t);
       }

    void generateInitCtor( ::std::basic_ostream<CharType> &os
                     , const string_type &indent
                     , typename string_type::size_type maxMemberTypeNameLen
                     , typename string_type::size_type maxMemberNameLen
                     , bool declarationOnly
                     ) const
       {
        string_type t; // tmp
        os<<indent<<className<<toStr("(", t);
        ::std::vector< CMemberInfo >::const_iterator mit = members.begin();
        for(; mit!=members.end(); ++mit)
           {
            if (mit == members.begin()) os<<toStr(" ", t);
            else                        os<<toStr("\n", t)<<indent<<string_type(className.size(), (CharType)' ')<<toStr(", ", t);

            bool byRef = false;
            // если есть swap или это структура или вектор, то по ссылке
            if (mit->typeFlags & (CMemberInfo::tfTypeFComplex | CMemberInfo::tfTypeFHasSwap | CMemberInfo::tfSixmlFVector))
               byRef = true;
            if (byRef)
               os<<toStr("const ", t);

            os<<mit->typeName;
            if (byRef)  os<<toStr(" &_", t);
            else        os<<toStr(" _", t);

            os<<mit->memberName;
           }

        os<<toStr("\n", t)<<indent<<string_type(className.size(), (CharType)' ')<<toStr(")", t);
        if (declarationOnly) { os<<toStr(";\n", t); return; }
        os<<toStr("\n", t);

        //::std::vector< CMemberInfo >::const_iterator 
        mit = members.begin();
        for(; mit!=members.end(); ++mit)
           {
            os<<indent<<toStr("   ", t);

            if (mit==members.begin()) os<<toStr(": ", t);
            else                      os<<toStr(", ", t);

            os<<mit->memberName<<toStr("(", t);
            os<<toStr("_", t)<<mit->memberName;
            os<<toStr(")\n", t);
           }       
        os<<indent<<toStr("   {}\n", t);
       }

    void generateCopyCtor( ::std::basic_ostream<CharType> &os
                     , const string_type &indent
                     , typename string_type::size_type maxMemberTypeNameLen
                     , typename string_type::size_type maxMemberNameLen
                     , bool declarationOnly
                     ) const
       {
        string_type t; // tmp
        os<<indent<<className<<toStr("( const ", t)<<className<<toStr(" &c )", t);
        if (declarationOnly) { os<<toStr(";\n", t); return; }

        os<<toStr("\n", t);

        ::std::vector< CMemberInfo >::const_iterator mit = members.begin();
        for(; mit!=members.end(); ++mit)
           {
            os<<indent<<toStr("   ", t);

            if (mit==members.begin()) os<<toStr(": ", t);
            else                      os<<toStr(", ", t);

            os<<mit->memberName<<toStr("( c.", t)<<mit->memberName<<toStr(" )\n", t);
           }       
        os<<indent<<toStr("   {}\n", t);
       }

    void generateSwap( ::std::basic_ostream<CharType> &os
                     , const string_type &indent
                     , typename string_type::size_type maxMemberTypeNameLen
                     , typename string_type::size_type maxMemberNameLen
                     , bool declarationOnly
                     ) const
       {
        string_type t; // tmp
        os<<indent<<toStr("void swap( ", t)<<className<<toStr(" &c )", t);
        if (declarationOnly) { os<<toStr(";\n", t); return; }

        os<<toStr("\n", t);
        os<<indent<<toStr("   {\n", t);

        ::std::vector< CMemberInfo >::const_iterator mit = members.begin();
        for(; mit!=members.end(); ++mit)
           {
            os<<indent<<toStr("    ", t);
            if (mit->typeFlags & (CMemberInfo::tfTypeFHasSwap | CMemberInfo::tfSixmlFVector) )
               {
                os<<mit->memberName<<toStr(".swap( c.", t)<<mit->memberName<<toStr(" );\n", t);
               }
            else
               {
                os<<toStr("::std::swap( ", t)<<mit->memberName<<toStr(", c.", t)<<mit->memberName<<toStr(" );\n", t);
               }
            //if (mit->typeName.size()>maxMemberTypeNameLen) maxMemberTypeNameLen = mit->typeName.size();
            //if (mit->memberName.size()>maxMemberNameLen)   maxMemberNameLen     = mit->memberName.size();
           }       
        os<<indent<<toStr("   }\n", t);
       }

    void generateAssignOp( ::std::basic_ostream<CharType> &os
                     , const string_type &indent
                     , typename string_type::size_type maxMemberTypeNameLen
                     , typename string_type::size_type maxMemberNameLen
                     , bool declarationOnly
                     ) const
       {
        string_type t; // tmp
        os<<indent<<className<<toStr("& operator=( const ", t)<<className<<toStr(" &c )", t);
        if (declarationOnly) { os<<toStr(";\n", t); return; }
        os<<toStr("\n", t);
        os<<indent<<toStr("   {\n", t);
        os<<indent<<toStr("    if (&c==this) return *this;\n", t);
        os<<indent<<toStr("    ", t)<<className<<toStr(" tmp( c );\n", t);
        os<<indent<<toStr("    tmp.swap( *this );\n", t);
        os<<indent<<toStr("    return *this;\n", t);
        os<<indent<<toStr("   }\n", t);
       }

    void generateAssignOpNoSwap( ::std::basic_ostream<CharType> &os
                     , const string_type &indent
                     , typename string_type::size_type maxMemberTypeNameLen
                     , typename string_type::size_type maxMemberNameLen
                     , bool declarationOnly
                     ) const
       {
        string_type t; // tmp
        os<<indent<<className<<toStr("& operator=( const ", t)<<className<<toStr(" &c )", t);
        if (declarationOnly) { os<<toStr(";\n", t); return; }
        os<<toStr("\n", t);

        os<<indent<<toStr("   {\n", t);
        os<<indent<<toStr("    if (&c==this) return *this;\n", t);
        ::std::vector< CMemberInfo >::const_iterator mit = members.begin();
        for(; mit!=members.end(); ++mit)
           {
            os<<indent<<toStr("    ", t)<<mit->memberName
              << string_type( maxMemberNameLen-mit->memberName.size(), (CharType)' ' )
              <<toStr(" = c.",t)<<mit->memberName<<toStr(";\n", t);
           }       
        os<<indent<<toStr("    return *this;\n", t);
        os<<indent<<toStr("   }\n", t);
       }

    void generateSixmlLayoutItem( ::std::basic_ostream<CharType> &os
                                , const string_type &indent
                                , typename string_type::size_type  maxMemberTypeNameLen
                                , typename string_type::size_type  maxMemberNameLen
                                , typename string_type::size_type &kwdLen
                                , typename string_type::size_type &tagNameLen
                                , const CMemberInfo                mInfo
                                , bool                             onlyCalc
                                ) const
       {
        string_type t; // tmp

        unsigned int memberVisibility = mInfo.typeFlags&CMemberInfo::tfVisibilityFMask;
        unsigned int memberType       = mInfo.typeFlags&CMemberInfo::tfTypeMask;
        unsigned int memberSixmType   = mInfo.typeFlags&CMemberInfo::tfSixmlMask;
        if (memberVisibility!=CMemberInfo::tfVisibilityFPublic)
           {
            if (!onlyCalc)
                os<<indent<<toStr("// '", t)<<mInfo.memberName<<toStr("' member ignored cause it is ", t)
                  <<getVisibilityName(memberVisibility)<<toStr("\n", t);
            return;
           }

        if (memberSixmType & CMemberInfo::tfSixmlFVector)
           { // vector
            if (memberSixmType & CMemberInfo::tfSixmlFAttr)
               { // array_attribute
                if (onlyCalc)
                   {
                    kwdLen      = 15; // len of 'array_attribute'
                    tagNameLen = mInfo.tagName.size(); // must be non-empty
                   }
                else
                   {
                    os<<indent<<toStr("array_attribute", t)<< string_type(kwdLen-15, (CharType)' ')
                      <<toStr("( _T(\"", t)<<mInfo.tagName<< toStr("\")", t) << string_type(tagNameLen-mInfo.tagName.size(), (CharType)' ')
                      <<toStr(", &", t)<<className<<toStr("::", t)
                      <<mInfo.memberName<< string_type(maxMemberNameLen-mInfo.memberName.size(), (CharType)' ')
                      <<toStr(" );\n", t);
                   }
               }
            else
               { // array
                if (onlyCalc)
                   {
                    kwdLen      = 5; // len of 'array'
                    if (memberSixmType&CMemberInfo::tfSixmlFSelf)
                       tagNameLen = 1;
                    else
                       tagNameLen = mInfo.tagName.size(); // must be non-empty
                   }
                else
                   {
                    os<<indent<<toStr("array", t)<< string_type(kwdLen-5, (CharType)' ')<<toStr("( ", t);
                    if (memberSixmType&CMemberInfo::tfSixmlFSelf)
                        os<<toStr("0", t) << string_type(tagNameLen-1+6, (CharType)' ');
                    else
                        os<<toStr("_T(\"", t)<<mInfo.tagName<< toStr("\")", t) << string_type(tagNameLen-mInfo.tagName.size(), (CharType)' ');
                    os<<toStr(", &", t)<<className<<toStr("::", t)
                      <<mInfo.memberName<< string_type(maxMemberNameLen-mInfo.memberName.size(), (CharType)' ')
                      <<toStr(" , _T(\"", t)<<mInfo.itemTagName<<toStr("\") );\n", t);
                   }
               }
           }
        else
           {
            if (memberType & CMemberInfo::tfTypeFComplex)
               {
                if (onlyCalc)
                   {
                    kwdLen = 7; // len of complex
                    if (memberSixmType & CMemberInfo::tfSixmlFSelf)
                       tagNameLen = 1; // tagName - '0'
                    else
                       tagNameLen = mInfo.tagName.size();
                   }
                else
                   {
                    os<<indent<<toStr("complex", t)<< string_type(kwdLen-7, (CharType)' ')<<toStr("( ", t);
                    if (memberSixmType&CMemberInfo::tfSixmlFSelf)
                        os<<toStr("0", t) << string_type(tagNameLen-1+6, (CharType)' ');
                    else
                        os<<toStr("_T(\"", t)<<mInfo.tagName<< toStr("\")", t) << string_type(tagNameLen-mInfo.tagName.size(), (CharType)' ');
                    os<<toStr(", &", t)<<className<<toStr("::", t)
                      <<mInfo.memberName<< string_type(maxMemberNameLen-mInfo.memberName.size(), (CharType)' ');
                    if (!mInfo.itemTagName.empty()) // itemTagName contain def val for complex (in layout)
                       {
                        os<<toStr(" , ", t)<<mInfo.itemTagName;
                       }
                    os<<toStr(" );\n", t);
                      //<<toStr(" , _T(\"", t)<<mInfo.itemTagName<<toStr("\") );\n", t);
                   }
               }
            else
               {
                if (memberSixmType&CMemberInfo::tfSixmlFAttr)
                   {
                    if (onlyCalc)
                       {
                        kwdLen = 9; // len of attribute
                        //if (memberSixmType & CMemberInfo::tfSixmlFSelf)
                        //   tagNameLen = 1; // tagName - '0'
                        //else
                           tagNameLen = mInfo.tagName.size();
                       }
                    else
                       {
                        os<<indent<<toStr("attribute", t)<< string_type(kwdLen-9, (CharType)' ')<<toStr("( ", t);
                        os<<toStr("_T(\"", t)<<mInfo.tagName<< toStr("\")", t) << string_type(tagNameLen-mInfo.tagName.size(), (CharType)' ');
                        os<<toStr(", &", t)<<className<<toStr("::", t)
                          <<mInfo.memberName<< string_type(maxMemberNameLen-mInfo.memberName.size(), (CharType)' ');
                        if (!mInfo.defValue.empty())
                           {
                            os<<toStr(" , ", t)<<mInfo.typeName<<(CharType)'('<<mInfo.defValue<<(CharType)')';
                           }
                        os<<toStr(" );\n", t);
                       }
                   }
                else
                   {
                    if (onlyCalc)
                       {
                        kwdLen = 6; // len of simple
                        //if (memberSixmType & CMemberInfo::tfSixmlFSelf)
                        //   tagNameLen = 1; // tagName - '0'
                        //else
                           tagNameLen = mInfo.tagName.size();
                       }
                    else
                       {
                        os<<indent<<toStr("simple", t)<< string_type(kwdLen-6, (CharType)' ')<<toStr("( ", t);
                        if (memberSixmType&CMemberInfo::tfSixmlFSelf)
                            os<<toStr("0", t) << string_type(tagNameLen-1+6, (CharType)' ');
                        else
                            os<<toStr("_T(\"", t)<<mInfo.tagName<< toStr("\")", t) << string_type(tagNameLen-mInfo.tagName.size(), (CharType)' ');
                        os<<toStr(", &", t)<<className<<toStr("::", t)
                          <<mInfo.memberName<< string_type(maxMemberNameLen-mInfo.memberName.size(), (CharType)' ');
                        if (!mInfo.defValue.empty())
                           {
                            os<<toStr(" , ", t)<<mInfo.typeName<<(CharType)'('<<mInfo.defValue<<(CharType)')';
                           }
                        os<<toStr(" );\n", t);
                       }
                   }

                /*
                switch(memberSixmType)
                   {
                    case :  
                            break;
                    case :  
                            break;
                    default:            
                   }
                */
               }
           }

/*
            simple(   _T("flag2")     , &CFlagContainer::flag2);
            attribute(_T("enum1")     , &CFlagContainer::e1);
            simple(   _T("enum2")     , &CFlagContainer::e2);
            array( _T("flags-vec")            , &CSimpleTestContainerAttrs::flagsVec, _T("flags-vec-item") );
            array_attribute( _T("ints-attr")  , &CSimpleTestContainerAttrs::intVec2 );
            complex( 0                        , &CSimpleTestContainerAttrs::complex1 );
            complex( _T("complex2")           , &CSimpleTestContainerAttrs::complex2 );


    static const unsigned int tfSixmlSimple             = 0x00000000;
    static const unsigned int tfSixmlFAttr              = 0x00000100;
    static const unsigned int tfSixmlAttr               = 0x00000100;
    static const unsigned int tfSixmlFSelf              = 0x00000200; // for complex no subtag used
    static const unsigned int tfSixmlFVector            = 0x00000400;
    static const unsigned int tfSixmlVector             = 0x00000400;
    static const unsigned int tfSixmlAttrVector         = 0x00000500;
    static const unsigned int tfSixmlMask               = 0x0000FF00;
*/

       
       }

    void generateSixmlLayout( ::std::basic_ostream<CharType> &os
                     , const string_type &indent
                     , typename string_type::size_type maxMemberTypeNameLen
                     , typename string_type::size_type maxMemberNameLen
                     , bool declarationOnly // ignored
                     ) const
       {

        typename string_type::size_type kwdMaxLen      = 0;
        typename string_type::size_type tagMaxNameLen = 0;

        string_type t; // tmp
        os<<indent<<toStr("STRUCT_LAYOUT_DEFAULT( ", t)<<className<<toStr(" )\n", t);
        string_type ind2 = indent; ind2.append(4,(CharType)' ');
        ::std::vector< CMemberInfo >::const_iterator mit = members.begin();
        for(; mit!=members.end(); ++mit)
           {
            typename string_type::size_type kwdLen = 0;
            typename string_type::size_type tagNameLen = 0;
            generateSixmlLayoutItem( os, ind2, maxMemberTypeNameLen, maxMemberNameLen, kwdLen, tagNameLen, *mit, true );
            //kwdLen += 5; // _T("")
            tagNameLen += 6;
            if (kwdMaxLen<kwdLen)         kwdMaxLen     = kwdLen;
            if (tagMaxNameLen<tagNameLen) tagMaxNameLen = tagNameLen;
           }

        mit = members.begin();
        for(; mit!=members.end(); ++mit)
           {
            generateSixmlLayoutItem( os, ind2, maxMemberTypeNameLen, maxMemberNameLen, kwdMaxLen, tagMaxNameLen, *mit, false );
           }

        os<<indent<<toStr("END_STRUCT_LAYOUT()\n", t);
       }

}; // CClassInfoT


typedef CClassInfoT<char> CClassInfo;


template< typename CharType, typename Traits, typename Allocator >
void CClassInfoT<CharType,Traits,Allocator>::addComplex( const string_type &typeName, const string_type &memberName
                 , bool hasSwap
                 , const string_type &tagName // = string_type()
                 , const string_type &defVal  // = string_type()
                 , const string_type &layoutDefVal
                 )
   {
    unsigned int flags = CMemberInfo::tfTypeComplex; // | CMemberInfo::tfVisibilityFProtected;
    if (tagName.empty()) flags |= CMemberInfo::tfSixmlFSelf;
    if (hasSwap)         flags |= CMemberInfo::tfTypeFHasSwap;
    members.push_back( CMemberInfo( flags, typeName, memberName, tagName, defVal, layoutDefVal ) );
   }

template< typename CharType, typename Traits, typename Allocator >
void CClassInfoT<CharType,Traits,Allocator>::addComplexVector( const string_type &typeName, const string_type &memberName
                 , const string_type &itemTagName
                 , const string_type &tagName     // = string_type()
                 )
   {
    unsigned int flags = CMemberInfo::tfTypeComplex | CMemberInfo::tfSixmlFVector; // | CMemberInfo::tfVisibilityFProtected;
    if (tagName.empty()) flags |= CMemberInfo::tfSixmlFSelf;
    members.push_back( CMemberInfo( flags, makeVectorTypeName(typeName), memberName, tagName, string_type() /* defVal */, itemTagName ) );
   }

template< typename CharType, typename Traits, typename Allocator >
void CClassInfoT<CharType,Traits,Allocator>::addSimplePod( const string_type &typeName, const string_type &memberName
                 , const string_type &tagName
                 , const string_type &defVal
                 )
   {
    unsigned int flags = CMemberInfo::tfTypeIntegral; // | CMemberInfo::tfVisibilityFProtected;
    if (tagName.empty()) flags |= CMemberInfo::tfSixmlFSelf;
    members.push_back( CMemberInfo( flags, typeName, memberName, tagName, defVal ) );
   }

template< typename CharType, typename Traits, typename Allocator >
void CClassInfoT<CharType,Traits,Allocator>::addSimplePodVector( const string_type &typeName, const string_type &memberName
                 , const string_type &itemTagName
                 , const string_type &tagName
                 )
   {
    unsigned int flags = CMemberInfo::tfTypeIntegral | CMemberInfo::tfSixmlFVector;
    if (tagName.empty()) flags |= CMemberInfo::tfSixmlFSelf;
    members.push_back( CMemberInfo( flags, makeVectorTypeName(typeName), memberName, tagName, string_type(), itemTagName ) );
   }

template< typename CharType, typename Traits, typename Allocator >
void CClassInfoT<CharType,Traits,Allocator>::addAttrPod( const string_type &typeName, const string_type &memberName
               , const string_type &attrName
               , const string_type &defVal
               )
   {
    members.push_back( CMemberInfo( CMemberInfo::tfTypeIntegral | CMemberInfo::tfSixmlFAttr
                                  , typeName, memberName, attrName, defVal )
                     );
   }

template< typename CharType, typename Traits, typename Allocator >
void CClassInfoT<CharType,Traits,Allocator>::addAttrPodVector( const string_type &typeName, const string_type &memberName
               , const string_type &attrName
               )
   {
    members.push_back( CMemberInfo( CMemberInfo::tfTypeIntegral | CMemberInfo::tfSixmlFAttr | CMemberInfo::tfSixmlFVector
                                  , makeVectorTypeName(typeName), memberName, attrName )
                     );
   }

// string type is integral in sixml terms but can't be initialized with zero
template< typename CharType, typename Traits, typename Allocator >
void CClassInfoT<CharType,Traits,Allocator>::addSimpleString( const string_type &typeName, const string_type &memberName
                 , const string_type &tagName
                 , const string_type &defVal
                 )
   {
    unsigned int flags = CMemberInfo::tfTypeIntegralNoZeroInit | CMemberInfo::tfTypeFHasSwap;
    if (tagName.empty()) flags |= CMemberInfo::tfSixmlFSelf;
    members.push_back( CMemberInfo( flags, typeName, memberName, tagName, defVal ) );
   }

template< typename CharType, typename Traits, typename Allocator >
void CClassInfoT<CharType,Traits,Allocator>::addSimpleStringVector( const string_type &typeName, const string_type &memberName
                 , const string_type &itemTagName
                 , const string_type &tagName
                 )
   {
    unsigned int flags = CMemberInfo::tfTypeIntegralNoZeroInit | CMemberInfo::tfTypeFHasSwap | CMemberInfo::tfSixmlFVector;
    if (tagName.empty()) flags |= CMemberInfo::tfSixmlFSelf;
    members.push_back( CMemberInfo( flags, makeVectorTypeName(typeName), memberName, tagName, string_type(), itemTagName ) );
   }

template< typename CharType, typename Traits, typename Allocator >
void CClassInfoT<CharType,Traits,Allocator>::addAttrString( const string_type &typeName, const string_type &memberName
               , const string_type &attrName
               , const string_type &defVal
               )
   {
    members.push_back( CMemberInfo( CMemberInfo::tfTypeIntegralNoZeroInit | CMemberInfo::tfTypeFHasSwap | CMemberInfo::tfSixmlFAttr
                                  , typeName, memberName, attrName, defVal )
                     );
   }

template< typename CharType, typename Traits, typename Allocator >
void CClassInfoT<CharType,Traits,Allocator>::addAttrStringVector( const string_type &typeName, const string_type &memberName
               , const string_type &attrName
               )
   {
    members.push_back( CMemberInfo( CMemberInfo::tfTypeIntegralNoZeroInit | CMemberInfo::tfTypeFHasSwap | CMemberInfo::tfSixmlFAttr | CMemberInfo::tfSixmlFVector
                                  , makeVectorTypeName(typeName), memberName, attrName )
                     );
   }

/*
template< typename CharType, typename Traits, typename Allocator >
void CClassInfoT<CharType,Traits,Allocator>::addComplex( const string_type &typeName, const string_type &memberName
               , const string_type &tagName, const string_type &defVal, bool hasSwapMethod
               )
   {
    unsigned int flags = CMemberInfo::tfTypeFComplex;
    if (hasSwapMethod)
       flags |= CMemberInfo::tfTypeFHasSwap;
    if (tagName.empty()) flags |= CMemberInfo::tfSixmlFSelf;
    members.push_back( CMemberInfo( flags, typeName, memberName, tagName, defVal ) );
   }
*/

// attr, simple, complex

/*
    static const unsigned int tfVisibilityFMask          = 0xFF000000;
    static const unsigned int tfVisibilityFPublic        = 0x00000000;
    static const unsigned int tfVisibilityFProtected     = 0x01000000;
    static const unsigned int tfVisibilityFPrivate       = 0x02000000;
        /*
        unsigned int  typeFlags;
        string_type   typeName;
        string_type   memberName;
        string_type   defValue; // used in ctor and layout
        string_type   tagName; // or attr
        string_type   itemTagName; // for vectors
        */



/*
            simple(   _T("flag2")     , &CFlagContainer::flag2);
            attribute(_T("enum1")     , &CFlagContainer::e1);
            simple(   _T("enum2")     , &CFlagContainer::e2);
            array( _T("flags-vec")            , &CSimpleTestContainerAttrs::flagsVec, _T("flags-vec-item") );
            array_attribute( _T("ints-attr")  , &CSimpleTestContainerAttrs::intVec2 );
            complex( 0                        , &CSimpleTestContainerAttrs::complex1 );
            complex( _T("complex2")           , &CSimpleTestContainerAttrs::complex2 );

    // type flags
    static const unsigned int tfTypeFComplex            = 0x00000001;
    static const unsigned int tfTypeFZeroInit           = 0x00000002;      // can be initialized with zero
    static const unsigned int tfTypeFHasSwap            = 0x00000004;      // user type has swap method
    
    // common type types                         
    static const unsigned int tfTypeIntegral            = 0x00000002;      // intregral 
    static const unsigned int tfTypeIntegralNoZeroInit  = 0x00000000;      // can't be initialized with zero
    static const unsigned int tfTypeComplex             = 0x00000001;      // typeFComplex
    static const unsigned int tfTypeComplexHasSwap      = 0x00000005;      // typeFComplex|typeFHasSwap
    static const unsigned int tfTypeMask                = 0x000000FF;      // typeFComplex|typeFHasSwap

    static const unsigned int tfVisibilityFMask          = 0xFF000000;
    static const unsigned int tfVisibilityFPublic        = 0x00000000;
    static const unsigned int tfVisibilityFProtected     = 0x01000000;
    static const unsigned int tfVisibilityFPrivate       = 0x02000000;
    
    static const unsigned int tfSixmlSimple             = 0x00000000;
    static const unsigned int tfSixmlFAttr              = 0x00000100;
    static const unsigned int tfSixmlAttr               = 0x00000100;
    static const unsigned int tfSixmlFSelf              = 0x00000200; // for complex no subtag used
    static const unsigned int tfSixmlFVector            = 0x00000400;
    static const unsigned int tfSixmlVector             = 0x00000400;
    static const unsigned int tfSixmlAttrVector         = 0x00000500;
    static const unsigned int tfSixmlMask               = 0x0000FF00;

*/

//            , const ::std::string &firstIndent = ""
//            , const ::std::string &relIndent   = "    "

//-----------------------------------------------------------------------------

}; // namespace cpp
}; // namespace marty


#endif /* MARTY_CPPHLP_H */

