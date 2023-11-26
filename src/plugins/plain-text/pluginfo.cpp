#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

#if !defined(_VECTOR_) && !defined(_STLP_VECTOR) && !defined(__STD_VECTOR__) && !defined(_CPP_VECTOR) && !defined(_GLIBCXX_VECTOR)
    #include <vector>
#endif

#if !defined(_IOSTREAM_) && !defined(_STLP_IOSTREAM) && !defined(__STD_IOSTREAM__) && !defined(_CPP_IOSTREAM) && !defined(_GLIBCXX_IOSTREAM)
    #include <iostream>
#endif

#if !defined(_MAP_) && !defined(_STLP_MAP) && !defined(__STD_MAP__) && !defined(_CPP_MAP) && !defined(_GLIBCXX_MAP)
    #include <map>
#endif


#include "../../itextfmt.h"
#include "../../fmtUtils.h"


int main(int argc, char* argv[])
   {
    std::string pathList;
    if (argc<2)
       {
        CHAR buf[1024];
        DWORD res = GetCurrentDirectory(sizeof(buf), buf);
        if (res)
           {
            buf[res] = 0;
            pathList = buf;
           }
       }
    else
       {
        pathList = argv[1];
       }


    std::cout<<"Path list: "<<pathList<<"\n";

    std::map<std::string, TextFormatPluginInfo> plugMap;
    ITextFormatApiAllocator IAllocator;
    initIAllocator(&IAllocator);

    loadTextFormatPlugins(plugMap, pathList, &IAllocator);
    std::map<std::string, TextFormatPluginInfo>::iterator it = plugMap.begin();
    for( ;it!=plugMap.end(); ++it)
       {
        std::cout<<it->first<<" - "<<(char*)(it->second.pluginFileName ? it->second.pluginFileName : "unknown file name")<<"\n";
        LPSTR resStr = 0;
        SIZE_T resStrSize = 0;
        /*
        std::string text = "some text\nsome text\nsome some text\n";

        LPSTR pRes = it->second.formatText( text.c_str(), text.size(), 
                                            "var1=val1\0var2=val2\0var3=val3\0",
                                             &IAllocator, 0 // std fns,
                                             &resStr, &resStrSize);
        if (pRes)
           {
            std::string tmp((char*)resStr, (std::string::size_type)resStrSize);
            std::cout<<tmp<<"\n---\n";
            IAllocator.pfnFree((LPVOID)it->second.pluginFileName);
           }
        */
       }

    freeTextFormatPluginsMap(plugMap, &IAllocator);
    return 0;
   }