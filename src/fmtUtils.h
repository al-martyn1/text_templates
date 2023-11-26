/* (c)2005-2007 Alex Martynov, amart@mail.ru
 * This source file is a part of "Text Templates" - a plugin for file manager FAR.
 * You can use, modify, redistribute this source file without any limitations.
 * This source file is distributed "as is". No waranty of any kind is expressed or 
 * implied. Use it at your own risk.
 */
#ifndef FMTUTILS_H
#define FMTUTILS_H


#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

#ifndef STRICT
    #define STRICT
#endif

#if !defined(_WINDOWS_)
    #include <windows.h>
#endif


#if !defined(_VECTOR_) && !defined(_STLP_VECTOR) && !defined(__STD_VECTOR__) && !defined(_CPP_VECTOR) && !defined(_GLIBCXX_VECTOR)
    #include <vector>
#endif

#if !defined(_MAP_) && !defined(_STLP_MAP) && !defined(__STD_MAP__) && !defined(_CPP_MAP) && !defined(_GLIBCXX_MAP)
    #include <map>
#endif

#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

#if !defined(_ITERATOR_) && !defined(_STLP_ITERATOR) && !defined(__STD_ITERATOR__) && !defined(_CPP_ITERATOR) && !defined(_GLIBCXX_ITERATOR)
    #include <iterator>
#endif

#if !defined(_ALGORITHM_) && !defined(_STLP_ALGORITHM) && !defined(__STD_ALGORITHM) && !defined(_CPP_ALGORITHM) && !defined(_GLIBCXX_ALGORITHM)
    #include <algorithm>
#endif


#if !defined(ITEXTFMT_H)
    #include "itextfmt.h"
#endif


//-----------------------------------------------------------------------------
inline
void splitLineToWords(std::vector<std::string> &words, const std::string &line)
   {
    std::string curWord;
    std::string::size_type pos = 0, size = line.size();
    for(; pos<size; ++pos)
       {
        if (line[pos]==' ' || line[pos]=='\t')
           {
            if (!curWord.empty())
               {
                words.push_back(curWord);
                curWord.clear();
               }
           }
        else
           {
            curWord.append(1, line[pos]);
           }
       }

    if (!curWord.empty())
       words.push_back(curWord);
   }

//-----------------------------------------------------------------------------
inline
void splitTextToLines(std::vector<std::string> &lines, const std::string &text)
   {
    std::string curLine;
    std::string::size_type pos = 0, size = text.size();
    bool lastCharIsNewLine = false;
    for(; pos<size; ++pos)
       {
        if (text[pos]=='\r') continue;
        if (text[pos]=='\n')
           {
            lines.push_back(curLine);
            curLine.clear();
            lastCharIsNewLine = true;
            continue;
           }
        lastCharIsNewLine = false;
        curLine.append(1, text[pos]);
       }
    if (!curLine.empty() || lastCharIsNewLine)
       lines.push_back(curLine);
   }


//-----------------------------------------------------------------------------
inline
void splitText(std::vector<std::string> &vect, const std::string &text, char ch)
   {
    std::string curLine;
    std::string::size_type pos = 0, size = text.size();
    bool lastCharIsNewLine = false;
    for(; pos<size; ++pos)
       {
        if (text[pos]==ch)
           {
            vect.push_back(curLine);
            curLine.clear();
            lastCharIsNewLine = true;
            continue;
           }
        lastCharIsNewLine = false;
        curLine.append(1, text[pos]);
       }
    if (!curLine.empty() || lastCharIsNewLine)
       vect.push_back(curLine);
   }

//-----------------------------------------------------------------------------
inline
void splitToPair(const std::string& str, std::string &first, std::string &second, char sign)
   {
    std::string::size_type pos = str.find(sign);
    if (pos==std::string::npos)
       {
        first = str;
        return;
       }
    first = std::string(str, 0, pos);
    second = std::string(str, pos+1);
   }

//-----------------------------------------------------------------------------
inline
void buildOptionsMap(std::map<std::string, std::string> &optMap, LPCSTR options)
   {
    if (!options) return;
    std::string str = options;
    while(!str.empty())
       {
        std::pair<std::string, std::string> pair;
        splitToPair(str, pair.first, pair.second, '=');
        optMap.insert(pair);

        //std::string first, second;


        //std::vector<std::string> vect;
        //splitText(vect, str, '=');
        //if (vect.size()<2) optMap[vect[0]] = std::string();
        //else               optMap[vect[0]] = vect[1];

        options += str.size()+1;
        str = options;
       }
   }

//-----------------------------------------------------------------------------
LPVOID WINAPI textFormatApiAllocate(SIZE_T nBytes);
BOOL WINAPI textFormatApiFree(LPVOID pMem);
void initIAllocator(PITextFormatApiAllocator pIAllocator);

//-----------------------------------------------------------------------------
struct CLoadTextFormatPluginsFromPath
{
    std::map<std::string, TextFormatPluginInfo> &plugList;
    PITextFormatApiAllocator                     pIAllocator;

    CLoadTextFormatPluginsFromPath(std::map<std::string, TextFormatPluginInfo> &pl, PITextFormatApiAllocator pIA)
        : plugList(pl), pIAllocator(pIA)
        {}

    CLoadTextFormatPluginsFromPath(const CLoadTextFormatPluginsFromPath &cldr)
        : plugList(cldr.plugList), pIAllocator(cldr.pIAllocator)
        {}        

    void operator()(const std::string &path)
       {
        findInDirectory(path);

        WIN32_FIND_DATA fndData =  { 0 };
        fndData.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;

        std::string mask = path + std::string("\\*.*");

        HANDLE hFind = FindFirstFile( mask.c_str(), &fndData);
        if (hFind==INVALID_HANDLE_VALUE)
           return;

        do {
            std::string dirName = fndData.cFileName;
            if (dirName!="." && dirName!="..")
               operator()(path + std::string("\\") + std::string(fndData.cFileName));
           } while (FindNextFile(hFind, &fndData));

        FindClose(hFind);
       }

    void findInDirectory(const std::string &path)
       {
        WIN32_FIND_DATA fndData =  { 0 };
        fndData.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;

        std::string mask = path + std::string("\\*.dll");

        HANDLE hFind = FindFirstFile( mask.c_str(), &fndData);
        if (hFind==INVALID_HANDLE_VALUE)
           return;

        do {
            std::string modName = path + std::string("\\") + std::string(fndData.cFileName);
            //std::cout<<"Full name: "<<path<<"\\"<<fndData.cFileName<<"\n";
            TextFormatPluginInfo pluginInfo;
            pluginInfo.hPluginModule = ::LoadLibrary( modName.c_str() );
            if (!pluginInfo.hPluginModule)
               {
                //std::cout<<"!pluginInfo.hPluginModule, continue\n";
                continue;
               }

             pluginInfo.getPluginInternalName = (textFormatApiGetPluginInternalNameProc)
                                                GetProcAddress( pluginInfo.hPluginModule,
                                                                TEXTFORMATAPIGETPLUGININTERNALNAMEPROCNAME);
             pluginInfo.formatText            = (textFormatApiFormatTextProc)
                                                GetProcAddress( pluginInfo.hPluginModule,
                                                                TEXTFORMATAPIFORMATTEXTPROCNAME);

             if (!pluginInfo.getPluginInternalName || !pluginInfo.formatText)
                {
                 //std::cout<<"!pluginInfo.getPluginInternalName || !pluginInfo.formatText, continue\n";
                 FreeLibrary(pluginInfo.hPluginModule);
                 continue;
                }

             LPSTR internalName = pluginInfo.getPluginInternalName(pIAllocator, 0);
             if (!internalName)
                {
                 //std::cout<<"!internalName\n";
                 FreeLibrary(pluginInfo.hPluginModule);
                 continue;
                }

             pluginInfo.pluginFileName = (LPSTR)pIAllocator->pfnAllocate(modName.size()+1);
             if (pluginInfo.pluginFileName)
                {
                 #if defined(_MSC_VER) && _MSC_VER>=1400
                 #pragma warning( push )
                 #pragma warning( disable : 4996 )
                 #endif

                 modName.copy(pluginInfo.pluginFileName, modName.size());

                 #if defined(_MSC_VER) && _MSC_VER>=1400
                 #pragma warning( pop )
                 #endif

                 pluginInfo.pluginFileName[modName.size()] = 0;
                }
             plugList[std::string(internalName)] = pluginInfo;

           } while (FindNextFile(hFind, &fndData));

        FindClose(hFind);
       }
};


//-----------------------------------------------------------------------------
inline
void loadTextFormatPlugins(std::map<std::string, TextFormatPluginInfo> &plugMap, const std::string &pathList, PITextFormatApiAllocator pIAllocator)
   {
    std::vector<std::string> pathVect;
    splitText(pathVect, pathList, ';');
    std::for_each(pathVect.begin(), pathVect.end(), CLoadTextFormatPluginsFromPath(plugMap, pIAllocator));
   }

//-----------------------------------------------------------------------------
inline
void freeTextFormatPluginsMap(std::map<std::string, TextFormatPluginInfo> &plugMap, PITextFormatApiAllocator pIAllocator)
   {
    std::map<std::string, TextFormatPluginInfo>::iterator it = plugMap.begin();
    for( ;it!=plugMap.end(); ++it)
       {
        if (it->second.pluginFileName) pIAllocator->pfnFree((LPVOID)it->second.pluginFileName);
        ::FreeLibrary(it->second.hPluginModule);
       }
    plugMap.erase(plugMap.begin(), plugMap.end());
   }
//-----------------------------------------------------------------------------




#endif /* FMTUTILS_H */

