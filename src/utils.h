#ifndef UTILS_H
#define UTILS_H


#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

#if !defined(_VECTOR_) && !defined(_STLP_VECTOR) && !defined(__STD_VECTOR__) && !defined(_CPP_VECTOR) && !defined(_GLIBCXX_VECTOR)
    #include <vector>
#endif

#if !defined(_MAP_) && !defined(_STLP_MAP) && !defined(__STD_MAP__) && !defined(_CPP_MAP) && !defined(_GLIBCXX_MAP)
    #include <map>
#endif

#if !defined(_ALGORITHM_) && !defined(_STLP_ALGORITHM) && !defined(__STD_ALGORITHM) && !defined(_CPP_ALGORITHM) && !defined(_GLIBCXX_ALGORITHM)
    #include <algorithm>
#endif

#if !defined(_INC_MALLOC) && !defined(_MALLOC_H_) && !defined(_MALLOC_H)
    #include <malloc.h>
#endif




#ifndef __T
    #define __T(x)      x
#endif

#ifndef _T
    #define _T(x)       __T(x)
#endif

#ifndef TEXT
    #define TEXT        _T
#endif


namespace TextTP
{



namespace Utils
{



struct split_string_functor
{
    split_string_functor( std::vector<std::string> &r, 
                          const char separator)
         : res_str(), res(r), sep(separator) {};

    split_string_functor(const split_string_functor &ssf)
         : res_str(), res(ssf.res), sep(ssf.sep) {};

    split_string_functor& operator=(const split_string_functor &ssf)
         {
          if (&ssf==this) return *this;
          res_str = _T("");
          res = ssf.res;
          sep = ssf.sep;
         }         

    void operator() (const char &ch) 
         {
          if (ch!=sep) { res_str += ch; return; }
          res.push_back(res_str);
          //clear(res_str);
          res_str.clear();
         };
    ~split_string_functor()
         {
          if (res_str.size()) res.push_back(res_str);
          //clear(res_str);
          res_str.clear();
         }

     std::vector<std::string> &res;
     std::string res_str;
     char   sep;
    };


inline
void split_string(std::vector<std::string> &res, const std::string& str, const char ch)
    {
     std::for_each(str.begin(), str.end(), split_string_functor(res, ch));
    }

inline
void removeCR(std::string &str)
    {
     if (str.empty()) return;
     if (str[str.size()-1]!='\r') return;
     str.erase(str.size()-1);
    }

struct removeCR_functor
{
    void operator() (std::string &str) 
         {
          removeCR(str);
         };
    };

inline
void removeCR(std::vector<std::string> &strings)
    {
     std::for_each(strings.begin(), strings.end(), removeCR_functor());
    }
//--------------------------------------------------------------
#if defined(_MSC_VER) && _MSC_VER>=1400
#pragma warning( push )
#pragma warning( disable : 4996 )
#endif
inline
std::string ltrim(const std::string& s, const FARSTANDARDFUNCTIONS* stdfns)
{
    if ( s.empty() || !stdfns || !stdfns->LTrim ) return s;
    char *buf = (char*)_alloca(s.size()+1);
    s.copy(buf, s.size()); buf[s.size()] = 0;
    buf = stdfns->LTrim(buf);
    if (!buf) return s;
    return std::string(buf);
}
//--------------------------------------------------------------
inline
std::string rtrim(const std::string& s, const FARSTANDARDFUNCTIONS* stdfns)
{
    if ( s.empty() || !stdfns || !stdfns->RTrim ) return s;
    char *buf = (char*)_alloca(s.size()+1);
    s.copy(buf, s.size()); buf[s.size()] = 0;
    buf = stdfns->RTrim(buf);
    if (!buf) return s;
    return std::string(buf);
}
//--------------------------------------------------------------
inline
std::string trim(const std::string& s, const FARSTANDARDFUNCTIONS* stdfns)
{
    if ( s.empty() || !stdfns || !stdfns->Trim ) return s;
    char *buf = (char*)_alloca(s.size()+1);
    s.copy(buf, s.size()); buf[s.size()] = 0;
    buf = stdfns->Trim(buf);
    if (!buf) return s;
    return std::string(buf);
}
//--------------------------------------------------------------
inline
std::string strupr(const std::string& s, const FARSTANDARDFUNCTIONS* stdfns)
{
    if ( s.empty() || !stdfns || !stdfns->LStrupr ) return s;
    char *buf = (char*)_alloca(s.size()+1);
    s.copy(buf, s.size()); buf[s.size()] = 0;
    //buf = 
    stdfns->LStrupr(buf);
    //if (!buf) return s;
    return std::string(buf);
}
//--------------------------------------------------------------
inline
std::string strlwr(const std::string& s, const FARSTANDARDFUNCTIONS* stdfns)
{
    if ( s.empty() || !stdfns || !stdfns->LStrlwr ) return s;
    char *buf = (char*)_alloca(s.size()+1);
    s.copy(buf, s.size()); buf[s.size()] = 0;
    //buf = 
    stdfns->LStrlwr(buf);
    //if (!buf) return s;
    return std::string(buf);
}
//--------------------------------------------------------------
#if defined(_MSC_VER) && _MSC_VER>=1400
#pragma warning( pop )
#endif




struct trim_obj
{
    const FARSTANDARDFUNCTIONS* stdfns;
    inline trim_obj(const FARSTANDARDFUNCTIONS* fns) : stdfns(fns) {}
    void operator()(std::string &str)
    { trim(str, stdfns); }
};

struct rtrim_obj
{
    const FARSTANDARDFUNCTIONS* stdfns;
    inline rtrim_obj(const FARSTANDARDFUNCTIONS* fns) : stdfns(fns) {}
    void operator()(std::string &str)
    { rtrim(str, stdfns); }
};

struct ltrim_obj
{
    const FARSTANDARDFUNCTIONS* stdfns;
    inline ltrim_obj(const FARSTANDARDFUNCTIONS* fns) : stdfns(fns) {}
    void operator()(std::string &str)
    { ltrim(str, stdfns); }
};

struct strupr_obj
{
    const FARSTANDARDFUNCTIONS* stdfns;
    inline strupr_obj(const FARSTANDARDFUNCTIONS* fns) : stdfns(fns) {}
    void operator()(std::string &str)
    { strupr(str, stdfns); }
};

struct strlwr_obj
{
    const FARSTANDARDFUNCTIONS* stdfns;
    inline strlwr_obj(const FARSTANDARDFUNCTIONS* fns) : stdfns(fns) {}
    void operator()(std::string &str)
    { strlwr(str, stdfns); }
};


inline std::vector<std::string> trim(std::vector<std::string> &v, const FARSTANDARDFUNCTIONS* fns)
    {
     std::for_each(v.begin(), v.end(), trim_obj(fns));
     return v;
    }
inline std::vector<std::string> ltrim(std::vector<std::string> &v, const FARSTANDARDFUNCTIONS* fns)
    {
     std::for_each(v.begin(), v.end(), ltrim_obj(fns));
     return v;
    }
inline std::vector<std::string> rtrim(std::vector<std::string> &v, const FARSTANDARDFUNCTIONS* fns)
    {
     std::for_each(v.begin(), v.end(), rtrim_obj(fns));
     return v;
    }
inline std::vector<std::string> strupr(std::vector<std::string> &v, const FARSTANDARDFUNCTIONS* fns)
    {
     std::for_each(v.begin(), v.end(), strupr_obj(fns));
     return v;
    }
inline std::vector<std::string> strlwr(std::vector<std::string> &v, const FARSTANDARDFUNCTIONS* fns)
    {
     std::for_each(v.begin(), v.end(), strupr_obj(fns));
     return v;
    }

inline
bool isSpaceOnlyString(const std::string &str)
    {
     std::string::size_type pos = 0, size = str.size();
     for(; pos<size; ++pos)
        if (str[pos]<0 || str[pos]>' ') return false;
     return true;
    }

inline
bool strStartsWith(const std::string &str, const std::string &startsWith, const FARSTANDARDFUNCTIONS* stdfns = 0)
{
   std::string strCopy = stdfns ? ltrim(str, stdfns) : str;
   if (startsWith.size() > strCopy.size()) return false; // shorter str cant starts with longer
   if (startsWith.size() == strCopy.size()) return startsWith == strCopy;
   return startsWith == std::string(strCopy, 0, startsWith.size());
}

inline
bool strEndsWith(const std::string &str, const std::string &endsWith, const FARSTANDARDFUNCTIONS* stdfns = 0)
{
   std::string strCopy = stdfns ? rtrim(str, stdfns) : str;
   if (endsWith.size() > strCopy.size()) return false; // shorter str cant ends with longer
   if (endsWith.size() == strCopy.size()) return endsWith == strCopy;
   return endsWith == std::string(strCopy, strCopy.size() - endsWith.size());
}

/*
inline
std::string extractPath(const std::string &fileName)
   {
    std::string path = fileName;
    std::string::size_type slPos = path.find_last_of('\\');
    if (slPos == std::string::npos) return std::string();
    path.erase(slPos, std::string::npos);
    return path;
   }
*/

inline
bool comparePartOfString(const std::string &str, 
                         std::string::size_type pos,
                         const std::string &strToCompare,
                         bool fwd
                         )
   {
    if (fwd)
       {
        if ((pos+strToCompare.size()) > str.size()) return false;
        return str.compare(pos, strToCompare.size(), strToCompare)==0;
       }
    else
       {
        if (pos<strToCompare.size()) return false;
        return str.compare(pos+1-strToCompare.size(), strToCompare.size(), strToCompare)==0;
       }
   }




}; // namespace Utils


}; // namespace TextTP


#endif /* UTILS_H */

