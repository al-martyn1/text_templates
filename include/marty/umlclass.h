/*
   Copyright (c) 2007-2009 Alex Martynov, amart at mail.ru
   This code can be used without any limitations.
   This code provided as is, use it on your own risk.
*/

#ifndef MARTY_UMLCLASS_H
#define MARTY_UMLCLASS_H

#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

#if !defined(_VECTOR_) && !defined(_STLP_VECTOR) && !defined(__STD_VECTOR__) && !defined(_CPP_VECTOR) && !defined(_GLIBCXX_VECTOR)
    #include <vector>
#endif

#if !defined(_MAP_) && !defined(_STLP_MAP) && !defined(__STD_MAP__) && !defined(_CPP_MAP) && !defined(_GLIBCXX_MAP)
    #include <map>
#endif

#if !defined(_INC_ERRNO) && !defined(_ERRNO_H_) && !defined(_ERRNO_H)
    #include <errno.h>
#endif

//#include <boost/algorithm/string/trim.hpp>
//#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>


#include <sixml/sixmlx.h>

#include <sixml/serializ.h>


//-----------------------------------------------------------------------------
namespace marty{

namespace uml{

template<char C>
struct CIsExactChar
{
    bool operator()(char c) const
       { return c==C; }
};



//-----------------------------------------------------------------------------
enum tag_EMemberVisibility
    {
     emvPub  = 0,
     emvPri  = 1,
     emvPro  = 2,
     emvImpl = 3
    };

    typedef tag_EMemberVisibility    EMemberVisibility;

//-----------------------------------------------------------------------------
enum tag_EIoKind
    {
     eiokUndef = 0,
     eiokIn    = 1,
     eiokOut   = 2,
     eiokInOut = 3
    };

    typedef tag_EIoKind    EIoKind;


}; // namespace uml
}; // namespace marty


//-----------------------------------------------------------------------------
using marty::uml::tag_EMemberVisibility;

BEGIN_ENUM_META_DATA(tag_EMemberVisibility)
    enumerate( marty::uml::emvPub ,   _T("public")         , _T("pub"));
    enumerate( marty::uml::emvPri ,   _T("private")        , _T("pri"));
    enumerate( marty::uml::emvPro ,   _T("protected")      , _T("pro"));
    enumerate( marty::uml::emvImpl,   _T("implementation") , _T("impl"));
END_ENUM_META_DATA(tag_EMemberVisibility)


//-----------------------------------------------------------------------------
using marty::uml::tag_EIoKind;

BEGIN_ENUM_META_DATA(tag_EIoKind)
    enumerate( marty::uml::eiokUndef ,   _T("undef"), _T("undefined"));
    enumerate( marty::uml::eiokIn    ,   _T("in"));
    enumerate( marty::uml::eiokOut   ,   _T("out"));
    enumerate( marty::uml::eiokInOut ,   _T("inout"));
END_ENUM_META_DATA(tag_EIoKind)



//-----------------------------------------------------------------------------
namespace marty
{
namespace uml
{

//-----------------------------------------------------------------------------
inline
tstring getVisibilityName( tag_EMemberVisibility v)
   {
    switch(v)
       {
        case marty::uml::emvPub: return tstring(_T("public"));
        case marty::uml::emvPri: return tstring(_T("private"));
        case marty::uml::emvPro: return tstring(_T("protected"));
       }
    return tstring(_T(""));
   }

//-----------------------------------------------------------------------------
inline
tstring getIoKindName( tag_EIoKind k)
   {
    if (k==marty::uml::eiokIn) return tstring(_T("const"));
    return tstring(_T(""));
   }


//-----------------------------------------------------------------------------
struct CClassAttributeInfo
{
    // common
    std::string        name        ;
    std::string        type        ;
    std::string        value       ;
    std::string        comment     ;
    bool               ptr         ;
    bool               ref         ;

    // for attributes only
    EMemberVisibility  visibility  ; // 0 - pub, 1 - private, 2 - protected, 3 - implementation (?)
    bool               classScope  ; // static member
    bool               constAttr   ; // constant, value must be taken

    // for params only
    EIoKind            ioKind      ; // 1 - in, 2 - out, 3 - in out, 0 - undef

    CClassAttributeInfo() 
       : name()
       , type()
       , value()
       , comment()
       , ptr(false)
       , ref(false)
       , visibility(emvPub)
       , classScope(false)
       , constAttr(false)
       , ioKind(eiokUndef)
       {}

    bool isStaticConstant() const
       {
        if (constAttr && classScope && !value.empty()) return true;
        return false;
       }


    STRUCT_LAYOUT_DEFAULT(CClassAttributeInfo)
            attribute(_T("name")       , &CClassAttributeInfo::name                    );
            attribute(_T("type")       , &CClassAttributeInfo::type                    );
            attribute(_T("value")      , &CClassAttributeInfo::value       , tstring() );
            attribute(_T("const")      , &CClassAttributeInfo::constAttr   , false     );
            attribute(_T("ptr")        , &CClassAttributeInfo::ptr         , false     );
            attribute(_T("ref")        , &CClassAttributeInfo::ref         , false     );
            simple   (_T("comment")    , &CClassAttributeInfo::comment     , tstring() );
            attribute(_T("visibility") , &CClassAttributeInfo::visibility  , emvPub    );
            attribute(_T("class-scope"), &CClassAttributeInfo::classScope  , false     );
            attribute(_T("inout-kind") , &CClassAttributeInfo::ioKind      , eiokUndef );
    END_STRUCT_LAYOUT()

    bool checkAndCorrectIfPtrType()
       {
        if (!type.empty())
           {
            ptr = ref = false;
            if (type[type.size()-1]=='*')
               ptr = true;
            else if (type[type.size()-1]=='&')
               ref = true;
            if (ptr || ref)
               type.erase(type.size()-1);
           }
        else
           {
            ptr = ref = false;
           }
        return ptr || ref;
       }

};

typedef CClassAttributeInfo CMethodParameterInfo;


inline
bool isPtrType(std::string &resType, const std::string &type)
   {
    if (!type.empty())
       {
        if (type[type.size()-1]=='*')
           {
            resType = std::string(type, 0, type.size()-1);
            return true;
           }
        resType = type;
        return false;
       }
    else
       {
        resType = std::string();
        return false;
       }
   }


struct CMethodCode
{
    std::string lang;
    bool        bInline;
    std::string text;

    STRUCT_LAYOUT_DEFAULT(CMethodCode)
            attribute(_T("lang")       , &CMethodCode::lang     , tstring() );
            attribute(_T("inline")     , &CMethodCode::bInline  , true      );
            simple   (0                , &CMethodCode::text     , tstring() );
    END_STRUCT_LAYOUT()

};


struct CClassMethodInfo
{
    // common
    std::string        name        ;
    std::string        stereotype  ; // event_handler
    std::string        type        ;
    EMemberVisibility  visibility  ; // 0 - pub, 1 - private, 2 - protected, 3 - implementation (?)
    bool               virtualMethod;
    bool               abstractMethod; // pureVirtual
    bool               query       ; // const method
    bool               classScope  ; // static member
    std::string        comment     ;

    bool isPtrReturnType(std::string &resType) const
       {
        return isPtrType(resType, type);
       }

    std::vector<CMethodParameterInfo> parameters;

    std::vector<CMethodCode>  code;


    void splitCodeByLangs()
       {
        std::vector<CMethodCode> tmpCode;
        std::vector<CMethodCode>::const_iterator it = code.begin();
        for(; it!=code.end(); ++it)
           {
            if (it->lang.empty())
               tmpCode.push_back(*it);
            else
               {
                CMethodCode c = *it;
                std::vector< std::string > codeLangList;
                ::boost::algorithm::split(codeLangList, c.lang, CIsExactChar<','>(), boost::algorithm::token_compress_on);
                std::vector< std::string >::const_iterator clit = codeLangList.begin();
                for(; clit!=codeLangList.end(); ++clit)
                   {
                    c.lang = *clit;
                    tmpCode.push_back(c);
                   }
               }
           }
        tmpCode.swap(code);
       }

    const CMethodCode* getCodeForLang(const std::string &lang, bool bExact) const
       {
        std::vector<CMethodCode>::const_iterator it = code.begin();
        for(; it!=code.end(); ++it)
           {
            if (it->lang==lang || (bExact && it->lang.empty()))
               {
                const CMethodCode &c = *it;
                return &c;
               }
           }
        return 0;
       }

    CMethodCode* getCodeForLang(const std::string &lang, bool bExact)
       {
        std::vector<CMethodCode>::iterator it = code.begin();
        for(; it!=code.end(); ++it)
           {
            if (it->lang==lang || (bExact && it->lang.empty()))
               {
                CMethodCode &c = *it;
                return &c;
               }
           }
        return 0;
       }

    bool addCode(const CMethodCode &c)
       {
        CMethodCode *pCode = getCodeForLang(c.lang, true);
        if (pCode)
           {
            *pCode = c;
            return false;
           }
        code.push_back(c);
        return true;
       }

    bool addCode(const std::string &lang, const std::string &text, bool bInline)
       {
        CMethodCode c;
        c.lang = lang;
        c.text = text;
        c.bInline = bInline;
        return addCode(c);
       }

    CClassMethodInfo()
       : name()
       , stereotype()
       , type()
       , visibility(emvPub)
       , virtualMethod(false)
       , abstractMethod(false)
       , query(false)
       , classScope(false)
       , comment()
       , parameters()
       , code()
       {}

    STRUCT_LAYOUT_DEFAULT(CClassMethodInfo)
            attribute(_T("name")       , &CClassMethodInfo::name                    );
            attribute(_T("stereotype") , &CClassMethodInfo::stereotype  , tstring() );
            attribute(_T("type")       , &CClassMethodInfo::type                    );
            attribute(_T("visibility") , &CClassMethodInfo::visibility  , emvPub    );
            attribute(_T("virtual")    , &CClassMethodInfo::virtualMethod , false   );
            attribute(_T("abstract")   , &CClassMethodInfo::abstractMethod, false   );
            attribute(_T("query")      , &CClassMethodInfo::query       , false     );
            attribute(_T("class-scope"), &CClassMethodInfo::classScope  , false     );
            simple   (_T("comment")    , &CClassMethodInfo::comment     , tstring() );
            array    (_T("parameters") , &CClassMethodInfo::parameters  , _T("parameter") );
            array    (0                , &CClassMethodInfo::code        , _T("code") );
    END_STRUCT_LAYOUT()
};




template <typename T>
struct IsEqualByName
{
    std::string   name;
    IsEqualByName() : name() {}
    IsEqualByName(const IsEqualByName &e) : name(e.name) {}
    IsEqualByName(const std::string &n) : name(n) {}
    bool operator()(const T &t)
       {
        return t.name==name;
       }
};

template <typename T>
struct LessByName
{
    bool operator()(const T &t1, const T &t2)
       {
        return t1.name<t2.name;
       }
};

template <typename T>
struct IsEqualByComment
{
    std::string   comment;
    IsEqualByComment() : comment() {}
    IsEqualByComment(const IsEqualByComment &e) : comment(e.comment) {}
    IsEqualByComment(const std::string &c) : comment(c) {}
    bool operator()(const T &t)
       {
        return t.comment==comment;
       }

};


struct COptionAttr
{
    std::string     name;
    std::string     value;

    COptionAttr() : name(), value() {}
    COptionAttr(const std::string &n, const std::string &v) : name(n), value(v) {}
    COptionAttr(const COptionAttr &o) : name(o.name), value(o.value) {}
    COptionAttr& operator=(const COptionAttr &o)
       {
        if (&o!=this)
           {
            name  = o.name;
            value = o.value;
           }
        return *this;
       }

    STRUCT_LAYOUT_DEFAULT(COptionAttr)
            attribute (_T("name")  , &COptionAttr::name , tstring() );
            simple    ( 0          , &COptionAttr::value, tstring() );
    END_STRUCT_LAYOUT()

};


struct CClassOptions
{
    //::std::map< std::string, std::string > optionMap;
    ::std::vector< COptionAttr > optionAttrs;
    std::string                  text;

    STRUCT_LAYOUT_DEFAULT(CClassOptions)
            array    (_T("tags")    , &CClassOptions::optionAttrs  , _T("tag") );
            simple   (_T("text")    , &CClassOptions::text         , tstring() );
    END_STRUCT_LAYOUT()

    CClassOptions() : optionAttrs(), text() {}
    CClassOptions(const CClassOptions &co) : optionAttrs(co.optionAttrs), text(co.text) {}
    CClassOptions& operator=(const CClassOptions &co)
       {
        if (&co!=this)
           {
            optionAttrs = co.optionAttrs;
            text        = co.text;
           }
        return *this;
       }

    void attrsFromMap(const std::map< std::string, std::string > &optionMap)
       {
        std::map< std::string, std::string >::const_iterator it = optionMap.begin();
        for(; it!=optionMap.end(); ++it)
           {
            optionAttrs.push_back(COptionAttr(it->first, it->second));
           }
       }

    void attrsToMap(std::map< std::string, std::string > &optionMap) const
       {
        ::std::vector< COptionAttr >::const_iterator it = optionAttrs.begin();
        for(; it!=optionAttrs.end(); ++it)
           {
            optionMap[it->name] = it->value;
           }
       }
};




struct CClassInfo
{
    // common
    std::string        name        ;
    std::string        stereotype  ;
    std::string        comment     ;
    
    std::vector<CClassAttributeInfo>  attributes;
    std::vector<CClassMethodInfo>     methods;

    //std::string includesList;
    //std::vector<std::string> includesList;

    std::vector<CClassOptions>   includes;
    //std::vector<CClassOptions>   code;

    CClassAttributeInfo* getAttrInfo(const std::string &attrName)
       {
        std::vector<CClassAttributeInfo>::iterator it = std::find_if(attributes.begin(), attributes.end(), IsEqualByName<CClassAttributeInfo>(attrName));
        if (it==attributes.end()) return 0;
        CClassAttributeInfo &a = *it;
        return &a;
       }

    const CClassAttributeInfo* getAttrInfo(const std::string &attrName) const
       {
        std::vector<CClassAttributeInfo>::const_iterator it = std::find_if(attributes.begin(), attributes.end(), IsEqualByName<CClassAttributeInfo>(attrName));
        if (it==attributes.end()) return 0;
        const CClassAttributeInfo &a = *it;
        return &a;
       }

    CClassMethodInfo* getMethodInfo(const std::string &methodName)
       {
        std::vector<CClassMethodInfo>::iterator it = std::find_if(methods.begin(), methods.end(), IsEqualByName<CClassMethodInfo>(methodName));
        if (it==methods.end()) return 0;
        CClassMethodInfo &m = *it;
        return &m;
       }

    const CClassMethodInfo* getMethodInfo(const std::string &methodName) const 
       {
        std::vector<CClassMethodInfo>::const_iterator it = std::find_if(methods.begin(), methods.end(), IsEqualByName<CClassMethodInfo>(methodName));
        if (it==methods.end()) return 0;
        const CClassMethodInfo &m = *it;
        return &m;
       }

    bool addMethodCode(const std::string& methodName, const std::string &lang, const std::string &text, bool bInline)
       {
        CClassMethodInfo *pmi = getMethodInfo(methodName);
        if (!pmi) return false;
        CMethodCode c;
        c.text = text;
        c.lang = lang;
        c.bInline = bInline;
        return pmi->addCode(c);
       }

     void splitMethodsCodeByLangs()
       {
        std::vector<CClassMethodInfo>::iterator it = methods.begin();
        for(; it!=methods.end(); ++it)
           {
            it->splitCodeByLangs();
           }
       }

     const CMethodCode* getMethodCodeForLang(const std::string &methodName, const std::string &lang, bool bExact) const
        {
         const CClassMethodInfo* pmi = getMethodInfo(methodName);
         if (!pmi) return 0;
         return pmi->getCodeForLang(lang, bExact);
        }

    /* void splitCodeByLangs()
     * const CMethodCode* getCodeForLang(const std::string &lang, bool bExact) const
     * CMethodCode* getCodeForLang(const std::string &lang, bool bExact)
     * bool addCode(const CMethodCode &c)
     */


    const CClassOptions* findClassOptions( const std::vector<CClassOptions> &optList, 
                                           const std::map<std::string, std::string> &exactValues) const
       {
        std::vector<CClassOptions>::const_iterator optIt = optList.begin();
        for(; optIt!=optList.end(); ++optIt)
           {
            const CClassOptions &options = *optIt;
            std::map<std::string, std::string> optTags;
            options.attrsToMap(optTags);
            // перебираем все явно указанные значения
            std::map<std::string, std::string>::const_iterator evIt = exactValues.begin();
            for(; evIt!=exactValues.end(); ++evIt)
               {
                std::map<std::string, std::string>::const_iterator tagIt = optTags.find(evIt->first);
                if (tagIt==optTags.end())
                   break; // такой опции/тэга вообще нет
               
                std::vector< std::string > valList;
                ::boost::algorithm::split(valList, tagIt->second, CIsExactChar<','>(), boost::algorithm::token_compress_on);
                
                std::vector< std::string >::const_iterator valIt = std::find(valList.begin(), valList.end(), evIt->second);
                if (valIt==valList.end())
                   break; // заданное значение отсутствует

                //return &options;
               }
            if (evIt==exactValues.end())
               {
                return &options;
               }
           }
        return 0;
       }

    const CClassOptions* getIncludes( const std::string &lang) const
       {
        std::map<std::string, std::string> exactValues;
        exactValues["lang"] = lang;
        return findClassOptions(includes, exactValues);
       }

/*
    const CClassOptions* getMethodCode( const std::string &lang, const std::string &methodName) const
       {
        std::map<std::string, std::string> exactValues;
        exactValues["lang"]   = lang;
        exactValues["method"] = methodName;
        return findClassOptions(code, exactValues);
       }
*/

    /*
    std::vector<CClassAttributeInfo>::const_iterator attrBegin() const
       { return attributes.begin(); }
    */

    CClassInfo()
       : name()
       , stereotype()
       , comment()
       , attributes()
       , methods()
       , includes()
       //, code()
       {}

    STRUCT_LAYOUT_DEFAULT(CClassInfo)
            attribute(_T("name")       , &CClassInfo::name                    );
            attribute(_T("stereotype") , &CClassInfo::stereotype  , tstring() );
            simple   (_T("comment")    , &CClassInfo::comment     , tstring() );
            //array    (_T("methods-code"),&CClassInfo::code        , _T("code") );
            array    (_T("includes")   , &CClassInfo::includes    , _T("include") );
            array    (_T("attributes") , &CClassInfo::attributes  , _T("attribute") );
            array    (_T("methods")    , &CClassInfo::methods     , _T("method") );
    END_STRUCT_LAYOUT()
};




}; // namespace uml
}; // namespace marty




#endif /* MARTY_UMLCLASS_H */

