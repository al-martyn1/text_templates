
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

#ifndef STRICT
    #define STRICT
#endif

#if !defined(_WINDOWS_)
    #include <windows.h>
#endif

#if !defined(_OBJBASE_H_)
    #include <objbase.h>
#endif

#if !defined(_INC_STRING) && !defined(__STRING_H_) && !defined(_STRING_H)
    #include <string.h>
#endif

#if !defined(_INC_WCHAR) && !defined(_WCHAR_H_) && !defined(_WCHAR_H)
    #include <wchar.h>
#endif

#if !defined(_INC_FLOAT) && !defined(_FLOAT_H_) && !defined(_FLOAT_H)
    #include <float.h>
#endif

#if !defined(_FUNCTIONAL_) && !defined(_STLP_FUNCTIONAL) && !defined(__STD_FUNCTIONAL__) && !defined(_CPP_FUNCTIONAL) && !defined(_GLIBCXX_FUNCTIONAL)
    #include <functional>
#endif

#if !defined(_ALGORITHM_) && !defined(_STLP_ALGORITHM) && !defined(__STD_ALGORITHM) && !defined(_CPP_ALGORITHM) && !defined(_GLIBCXX_ALGORITHM)
    #include <algorithm>
#endif

#if !defined(_INC_MALLOC) && !defined(_MALLOC_H_) && !defined(_MALLOC_H)
    #include <malloc.h>
#endif

#if !defined(_EXCEPTION_) && !defined(__EXCEPTION__) && !defined(_STLP_EXCEPTION) && !defined(__STD_EXCEPTION)
    #include <exception>
#endif

#if !defined(_STDEXCEPT_) && !defined(_STLP_STDEXCEPT) && !defined(__STD_STDEXCEPT) && !defined(_CPP_STDEXCEPT) && !defined(_GLIBCXX_STDEXCEPT)
    #include <stdexcept>
#endif

#if !defined(_VECTOR_) && !defined(_STLP_VECTOR) && !defined(__STD_VECTOR__) && !defined(_CPP_VECTOR) && !defined(_GLIBCXX_VECTOR)
    #include <vector>
#endif

#if !defined(_MAP_) && !defined(_STLP_MAP) && !defined(__STD_MAP__) && !defined(_CPP_MAP) && !defined(_GLIBCXX_MAP)
    #include <map>
#endif

#if !defined(_MEMORY_) && !defined(_STLP_MEMORY) && !defined(__STD_MEMORY__) && !defined(_CPP_MEMORY) && !defined(_GLIBCXX_MEMORY)
    #include <memory>
#endif

#ifndef FILENAME_H
    #include <filename.h>
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit

#ifndef __ATLBASE_H__
    #include <atlbase.h>
#endif

#ifndef __ATLDEF_H__
    #include <atldef.h>
#endif


//#include <stdio.h>
//#include "far/plugin.hpp"
#include "cfarcls.h"
#include "ttpconf.h"

#include "ttprc.h"
#include "tpl.h"
#include "utils.h"
#include "ttpinput.h"
#include "settings.h"
#include "autocom.h"
#include "ccomment.h"

#include "far/farkeys.hpp"


#if !defined(_SSTREAM_) && !defined(_STLP_SSTREAM) && !defined(__STD_SSTREAM__) && !defined(_CPP_SSTREAM) && !defined(_GLIBCXX_SSTREAM)
    #include <sstream>
#endif

#define SS std::stringstream ss


extern Far::CPluginStartupInfo         Info;
extern TextTP::TTPConfig               Config;


#define INIT_MENU_STRING(var, rcId, str)      \
        do {                                  \
            const char *tmp = ::Info.GetMsg(rcId);  \
            lstrcpy(var, tmp!=0 ? tmp : str); \
           } while(0)


namespace TextTP
{


void findMinIndent(const std::string &line, std::string &ind, int &curIndent);


//-----------------------------------------------------------------------------
void findMinIndent(const std::string &line, std::string &ind, int &curIndent)
   {
    std::string::size_type pos = line.find_first_not_of(" \t");
    if (pos==std::string::npos) 
       {
        return;
       }
    if (curIndent < (int)pos) 
       {
        return;
       }
    // SS;
    // ss<<"found space or tab at pos: "<<(unsigned)pos<<"\n";
    // MessageBox(0, ss.str().c_str(), ss.str().c_str(), 0);

    curIndent = (int)pos;
    ind = std::string(line, 0, pos);
   }

//-----------------------------------------------------------------------------
void addOption(std::string &optList, const std::string& name, const std::string& value)
   {
    optList.append(name);
    optList.append(1, '=');
    optList.append(value);
    optList.append(1, char(0));
   }

//-----------------------------------------------------------------------------
void formattingText()
   {
    EditorInfo edinfo;
    ::Info.EditorCtrlGetInfo( &edinfo );
    if (!edinfo.FileName) return;


    int startLine = edinfo.CurLine, endLine = edinfo.CurLine+1;
    int startPos = 0, endPos = 0;
    bool selected = false;

    if (edinfo.BlockType==BTYPE_STREAM || edinfo.BlockType==BTYPE_COLUMN)
       {
        selected = true;
        startLine = edinfo.BlockStartLine;
        for(endLine=startLine; endLine<edinfo.TotalLines; ++endLine)
           {
            EditorGetString egs = { endLine, 0, 0, 0, 0, 0};
            if (!::Info.EditorCtrlGetString(&egs))
               {
                return; //failed to get info about string
               }

            if (egs.SelStart<0) // Line does not contain selection
               {
                break;
               }

            if (egs.SelEnd>=0)
               {
                endPos = egs.SelEnd;
               }

            if (endLine==startLine)
               {
                startPos = egs.SelStart;
               }
           }
        //Warning(IDM_COMMENTING_CODE, ss.str());
        //ss<<"/**************/\n";
        if (endPos==0)
           {
            endPos = -1; // последняя строка выделена до конца
            --endLine;
           }
        if (startLine==(endLine-1) && startPos==endPos)
           {
            edinfo.BlockType=BTYPE_NONE;
            selected = false;
           }
        if (startPos<0)
           {
            startPos = 0;
            endPos = -1;
           }
        if (startLine==endLine) ++endLine;
       }

    if (!selected)
       return;

    TextTP::TTPConfig  cfgCopy = Config;
    readCustomConfig(edinfo.FileName, cfgCopy);


    cfgCopy.eraseUnisedFormatterStyles();

    std::string editorFileType;
    if (!lookupForFileType(cfgCopy, edinfo.FileName, editorFileType, Info.StdFn()))
        {
         return;
        } 

    std::vector<TextFormatterStyle> fmtStylesVec;
    findInfo(editorFileType, cfgCopy.formatting_styles.begin(), cfgCopy.formatting_styles.end(), fmtStylesVec);
    if (fmtStylesVec.empty())
        {
         //Warning(IDM_COMMENTING_CODE, "There are no styles after findInfo.");
         return;
        } 


    // Lookup for style
    const TextFormatterStyle *pStyle = 0;

    try{
        FarMenuItem * pMenuItems = new FarMenuItem[fmtStylesVec.size()];

        std::vector<TextFormatterStyle>::const_iterator fmtIt = fmtStylesVec.begin();
        for(FarMenuItem* pItem = pMenuItems; fmtIt!=fmtStylesVec.end(); ++fmtIt, ++pItem)
           {
            std::string itemText( fmtIt->hotkey.size() ? std::string("&") + fmtIt->hotkey : fmtIt->hotkey );
            if (!itemText.empty()) itemText += " ";
            itemText += fmtIt->name;

            size_t textLen = sizeof(pItem->Text) - 1; // maxLen
            if (textLen>itemText.size()) textLen = itemText.size();

            #if defined(_MSC_VER) && _MSC_VER>=1400
            #pragma warning( push )
            #pragma warning( disable : 4996 )
            #endif

            itemText.copy(pItem->Text, textLen, 0);

            #if defined(_MSC_VER) && _MSC_VER>=1400
            #pragma warning( pop )
            #endif

            pItem->Text[textLen] = '\0';

            pItem->Selected = 0;
            pItem->Checked = 0;
            pItem->Separator = 0;

           }

        char title[128];
        INIT_MENU_STRING(title, IDM_FORMATTING_TEXT, "Formatting text");
        int menuRes = ::Info.Menu( -1, -1, 0,
                                   FMENU_WRAPMODE, title, 0,
                                   0, /* Help topic */
                                   0, 0, 
                                   pMenuItems,
                                   (int)fmtStylesVec.size()
                                 );
        if (menuRes<0 || menuRes>=(int)fmtStylesVec.size())
           return; // invalid choise

        pStyle = &fmtStylesVec[menuRes];
       }
    catch(...)
       {
        Warning(IDM_FORMATTING_TEXT, "Failed to allocate memory or other error");
       }

    if (!pStyle) 
       {
        Warning(IDM_FORMATTING_TEXT, "Choised style not found. This is never occurs");
        return;
       }


    // lookup for plugin info struct
    std::map<std::string, TextFormatPluginInfo>::const_iterator plugIt = cfgCopy.plugMap.find(pStyle->plugin_name);
    if (plugIt==cfgCopy.plugMap.end())
       {
        Warning(IDM_FORMATTING_TEXT, "Choised style use plugin that not found. This is never occurs");
        return;
       }

    // build plugin options
    std::string optionList;
    
    addOption(optionList, "__PluginBasePath", cfgCopy.pluginPath);
    addOption(optionList, "__PluginFileName", plugIt->second.pluginFileName);
    //addOption(optionList, "__PluginPath", Utils::extractPath(plugIt->second.pluginFileName));
    addOption(optionList, "__PluginPath", filename::pathRemoveTrailingSlash(filename::getPath( plugIt->second.pluginFileName )));


    std::vector<TextFormatterOption>::const_iterator optIt = pStyle->options.begin();
    for(; optIt!=pStyle->options.end(); ++optIt)
       {
        addOption(optionList, optIt->name, optIt->value);
        // optionList.append(optIt->name);
        // optionList.append(1, '=');
        // optionList.append(optIt->value);
        // optionList.append(1, char(0));
       }
    optionList.append(1, char(0));



    PITextFormatApiAllocator pIAllocator = &Config.IAllocator;

    std::string indentText;
    int         minIndent = 256;


    std::string textToFormat;
    int lineNo = startLine;
    for(; lineNo<endLine; ++lineNo)
       {
        std::string str; // , eol;
        if (!::Info.EditorCtrlGetString(lineNo, str))
           {
            textToFormat.append(pStyle->getEOL());
            continue;
           }
        if (lineNo!=startLine) textToFormat.append(pStyle->getEOL());
        textToFormat.append(str);
        if (pStyle->keep_indent) 
           findMinIndent(str, indentText, minIndent);
       }

    // std::string tmpStr("Debug - before formatting:\n[");
    // tmpStr += textToFormat + std::string("]");
    // Warning(IDM_FORMATTING_TEXT, tmpStr.c_str());
    
    //std::string tmpText("Line 1: Nothing Nothing thing \nLine 2: Nothing Nothing Nothing Nothing Nothing\nLine 3: Nothing Nothing Nothing Nothing \n\nLine 5: Nothing");
    LPSTR resultText = 0;
    SIZE_T resSize   = 0;
    LPSTR fmtRes = plugIt->second.formatText(
                                             textToFormat.data(), (SIZE_T)textToFormat.size(),
                                             optionList.data(), pIAllocator, 
                                             ::Info.StdFn(),
                                             &resultText, &resSize
                                            );
    if (!fmtRes)
       {
        std::string pluginErrorMsg("Plugin 'textFormatApiFormatText' method call failed");
        if (resultText)
           {
            pluginErrorMsg += std::string("\n") + std::string(resultText, resSize);
            pIAllocator->pfnFree(resultText);
           }
        Warning(IDM_FORMATTING_TEXT, pluginErrorMsg.c_str());
        return;
       }

    if (!resultText)
       {
        Warning(IDM_FORMATTING_TEXT, "Plugin returns invalid data in taken buffer address");
        return;
       }

    std::string strResText(resultText, resSize);
    pIAllocator->pfnFree(resultText);

    // tmpStr = "Debug - after formatting:\n[";
    // tmpStr += strResText + std::string("]");
    // Warning(IDM_FORMATTING_TEXT, tmpStr.c_str());

    // debug
    //for(std::string::size_type pos = 0; pos<indentText.size(); ++pos) indentText[pos] = '-';


    int lineCountSrc = endLine - startLine;

    int insertLineNo = startLine - 1;
    int selCnt = ::Info.EditorCtrlInsertStrings(insertLineNo, strResText, indentText);
    
    for(int i=0; i<lineCountSrc; ++i)
       {
        ::Info.EditorCtrlDeleteLine(startLine+selCnt);
       }



    ::Info.EditorCtrlSelect( BTYPE_STREAM, startLine,
                             0, 0, 
                             selCnt+1);

    

/*
    int lineCountSrc = endLine - startLine;

    std::vector<std::string> resultLinesVec;
    splitTextToLines(resultLinesVec, strResText);


    int lineCoutRes = (int)resultLinesVec.size();

    int addLinesCount = lineCoutRes - lineCountSrc;
    bool bRemove = false;
    if (addLinesCount<0)
       {
        addLinesCount = -addLinesCount;
        bRemove = true;
       }

    for(int i=0; i<addLinesCount; ++i)
       {
        if (bRemove)
           EditorCtrlDeleteLine(startLine);
        else
           //
       }
*/





    // ::Info.EditorCtrlSetString(lineNo, str, eol);


    // std::string tmpStr("Debug - formatted text:\n");
    // tmpStr += strResText;
    //  
    // Warning(IDM_FORMATTING_TEXT, tmpStr.c_str());


// typedef LPSTR (WINAPI *textFormatApiFormatTextProc)(LPCSTR text, SIZE_T nBytes,
//                                       LPCSTR options, PITextFormatApiAllocator allocator,
//                                       const FARSTANDARDFUNCTIONS* pStdFns, /* Can be NULL */
//                                       LPSTR *resText, SIZE_T *nResBytes);



// typedef struct tagTextFormatPluginInfo
// {
//     HMODULE hPluginModule;
//     LPSTR   pluginFileName;
//     textFormatApiGetPluginInternalNameProc    getPluginInternalName;
//     textFormatApiFormatTextProc               formatText;
//  
// } TextFormatPluginInfo, *PTextFormatPluginInfo;




    // SS;
    // ss<<"User choise: "<<pStyle->name<<" ("<<pStyle->plugin_name<<")";
    // Warning(IDM_FORMATTING_TEXT, ss.str());

       
    // std::string      name;       // style-name
    // std::string      file_type;  // 
    // std::string      plugin_name;
    // std::string      hotkey;
    // std::vector<TextFormatterOption> options;


// struct FarMenuItem
// {
//   char Text[128];
//   int  Selected;
//   int  Checked;
//   int  Separator;
// };





    //cfgCopy.unloadFormatterPlugins();
   }


}; // namespace TextTP



