#include "../../../itextfmt.h"


#if !defined(_MAP_) && !defined(_STLP_MAP) && !defined(__STD_MAP__) && !defined(_CPP_MAP) && !defined(_GLIBCXX_MAP)
    #include <map>
#endif

#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif


#if !defined(_SSTREAM_) && !defined(_STLP_SSTREAM) && !defined(__STD_SSTREAM__) && !defined(_CPP_SSTREAM) && !defined(_GLIBCXX_SSTREAM)
    #include <sstream>
#endif


#if !defined(FMTUTILS_H)
    #include "../../../fmtUtils.h"
#endif


#if !defined(TEXT_FMT_H)
    #include "text-fmt.h"
#endif

extern "C"{
LPSTR WINAPI textFormatApiGetPluginInternalName(PITextFormatApiAllocator allocator, const FARSTANDARDFUNCTIONS* pStdFn);

LPSTR WINAPI textFormatApiFormatText(LPCSTR text, SIZE_T nBytes,
                                 LPCSTR options, PITextFormatApiAllocator allocator,
                                 const FARSTANDARDFUNCTIONS* pStdFns,
                                 LPSTR *resText, SIZE_T *nResBytes);
};



/*
inline 
int findOption(const std::map<std::string, std::string> optMap, const std::string &optName, int optDefVal)
    {
     std::map<std::string, std::string>::const_iterator it = optMap.find(optName);
     if (it==optMap.end()) return optDefVal;
     if (it->second.empty()) return optDefVal;

     char *pEnd = 0;
     
     long res = strtol( it->second.c_str(), &pEnd, 10);
     if (res || pEnd!=it->second.c_str())
        return (int)res;
     return optDefVal;
    }

inline 
std::string findOption(const std::map<std::string, std::string> optMap, const std::string &optName, const std::string &optDefVal)
    {
     std::map<std::string, std::string>::const_iterator it = optMap.find(optName);
     if (it==optMap.end()) return optDefVal;
     if (it->second.empty()) return optDefVal;
     return it->second;
    }

inline 
std::string findOption(const std::map<std::string, std::string> optMap, const std::string &optName, const char *optDefVal)
    {
     if (optDefVal) return findOption(optMap, "format-type", std::string(optDefVal));
     else           return findOption(optMap, "format-type", std::string());
    }
*/

//-----------------------------------------------------------------------------
LPSTR WINAPI textFormatApiGetPluginInternalName(PITextFormatApiAllocator allocator, const FARSTANDARDFUNCTIONS* pStdFn)
    {
     LPSTR name  = "artistic-style";
     LPSTR pRes = (LPSTR)allocator->pfnAllocate((lstrlen(name)+1)*sizeof(CHAR));
     lstrcpy(pRes, name);
     return name;
    }

//-----------------------------------------------------------------------------
LPSTR WINAPI textFormatApiFormatText(LPCSTR text, SIZE_T nBytes,
                                      LPCSTR options, PITextFormatApiAllocator allocator,
                                      const FARSTANDARDFUNCTIONS* pStdFns,
                                      LPSTR *resText, SIZE_T *nResBytes)
    {
     std::map<std::string, std::string> optMap;
     buildOptionsMap(optMap, options);
     
     /*
     std::string tmp;
     std::map<std::string, std::string>::const_iterator it = optMap.begin();
     for(; it!=optMap.end(); ++it)
        {
         tmp += it->first + std::string("=") + it->second + std::string("\n");
        }
     */
/*
     CFormatOptions opts;

     opts.globalIndent    = findOption(optMap, "indent", 0);
     opts.firstLineIndent = findOption(optMap, "para-first-line-indent", 2);
     opts.lineIndent      = findOption(optMap, "line-indent", 0);
     opts.paraWidth       = findOption(optMap, "width", 78);
     opts.formatType      = ftWidth;

     std::string formatTypeStr = findOption(optMap, "format-type", "width");
     if (formatTypeStr=="center") opts.formatType = ftCenter;
     else if (formatTypeStr=="left") opts.formatType = ftLeft;
     else if (formatTypeStr=="right") opts.formatType = ftRight;

     std::string textToFormat(text, nBytes);
     std::vector<std::string> lines;
     splitTextToLines(lines, textToFormat);
     std::string resTextStr = formatText(opts, lines);
*/

     // Debug code
     //std::string resTextStr = textToFormat;
/*
     std::vector<std::string> linesAfter;
     splitTextToLines(linesAfter, resTextStr);

     std::stringstream ss;
     ss<<"Lines before: "<<(unsigned)lines.size()<<"\nLines after: "<<(unsigned)linesAfter.size();
     resTextStr = ss.str();
*/



/*
     // return formatted text

     LPSTR resTextBuf = (LPSTR)allocator->pfnAllocate(resTextStr.size());
     *resText   = resTextBuf;
     *nResBytes = resTextStr.size();
     resTextStr.copy(resTextBuf, resTextStr.size());

     return resTextBuf;
*/
     return 0;
    }

//-----------------------------------------------------------------------------



