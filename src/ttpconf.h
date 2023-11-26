#ifndef TTPCONF_H
#define TTPCONF_H


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

#if !defined(_FUNCTIONAL_) && !defined(_STLP_FUNCTIONAL) && !defined(__STD_FUNCTIONAL__) && !defined(_CPP_FUNCTIONAL) && !defined(_GLIBCXX_FUNCTIONAL)
    #include <functional>
#endif

#if !defined(_DEQUE_) && !defined(_STLP_DEQUE) && !defined(__STD_DEQUE__) && !defined(_CPP_DEQUE) && !defined(_GLIBCXX_DEQUE)
    #include <deque>
#endif

#if !defined(_INC_MALLOC) && !defined(_MALLOC_H_) && !defined(_MALLOC_H)
    #include <malloc.h>
#endif


#if !defined(_IOSTREAM_) && !defined(_STLP_IOSTREAM) && !defined(__STD_IOSTREAM__) && !defined(_CPP_IOSTREAM) && !defined(_GLIBCXX_IOSTREAM)
    #include <iostream>
#endif


#ifndef __ATLCONV_H__
    #include <atlconv.h>
#endif

#ifndef FILENAME_H
    #include <filename.h>
#endif

#include "Serializer.h"
#include "SerializerUtils.h"
#include "SerializerXml.h"

#include "utils.h"
#include "fmtUtils.h"


#ifndef _T
    #define _T(str) str
#endif


#define LAYOUT_DEFAULT(struct_name)                                  \
struct LayoutDefault : public Rsdn::Serializer::Layout<struct_name>  \
{                                                                    \
    LayoutDefault()                                                  \
        {

#define END_LAYOUT()   \
        }              \
};




enum TextTP_CommentTypeEnum
{
    ctStream, ctLines
};

using Rsdn::Serializer::Primitives::EnumMetaData;

struct TextTP_CommentTypeEnum_EnumMetaData 
     : public EnumMetaData<TextTP_CommentTypeEnum>
{
    TextTP_CommentTypeEnum_EnumMetaData()// : EnumMetaData()
    {
        Enumerator(_T("stream"), ctStream);
        Enumerator(_T("lines"), ctLines );
    }
};

ENUM_METADATA(TextTP_CommentTypeEnum, TextTP_CommentTypeEnum_EnumMetaData)

inline
std::ostream& operator<<(std::ostream &s, TextTP_CommentTypeEnum e)
   {
    if (e==ctStream) s<<"stream";
    else             s<<"lines";
    return s;
   }


/*
namespace Rsdn       {
namespace Serializer {

//#define ENUM_METADATA(EnumType, EnumMetaData)
static const TextTP_CommentTypeEnum_EnumMetaData& EnumData_Instance(void)
{
    static TextTP_CommentTypeEnum_EnumMetaData _;
    return _;
}

template <>
struct Primitive<TextTP_CommentTypeEnum>
{
    static tstring ToString(const TextTP_CommentTypeEnum& e)
    { return EnumData_Instance().ToString(e); }

    static TextTP_CommentTypeEnum Parse(const tstring& s)
    {  return EnumData_Instance().Parse(s);  }
};

template <>
const MetaClass<TextTP_CommentTypeEnum>& Serializer::DefaultMetaClass<TextTP_CommentTypeEnum>()
    { static PrimClassMeta<TextTP_CommentTypeEnum> _; return _; }

}; // Rsdn
}; // Serializer
*/

/*
namespace TextTP
{

enum CommentTypeEnum
{
    ctStream, ctLines
};

using Rsdn::Serializer::Primitives::EnumMetaData;

struct CommentTypeEnum_EnumMetaData 
     : public EnumMetaData<CommentTypeEnum>
{
    CommentTypeEnum_EnumMetaData()// : EnumMetaData()
    {
        Enumerator(_T("stream"), ctStream);
        Enumerator(_T("lines"), ctLines );
    }
};

}; // namespace TextTP

namespace Rsdn{
namespace Serializer{
namespace Primitives{
using TextTP::CommentTypeEnum;
using TextTP::CommentTypeEnum_EnumMetaData;
ENUM_METADATA(CommentTypeEnum, CommentTypeEnum_EnumMetaData);
}; // Rsdn
}; // Serializer
}; // Primitives
*/


namespace TextTP
{


struct FiletypeExtentionsInfo
{
    std::string    file_type;
    std::string    mask_list;
    bool           use_path;

    LAYOUT_DEFAULT(FiletypeExtentionsInfo)
        AttributeM(_T("type"  ), &FiletypeExtentionsInfo::file_type/*, std::string("")*/);
        //AttributeM(_T("extention"  ), &FiletypeExtentionsInfo::ext_list/*, std::string("")*/);
        AttributeM(_T("mask"  ), &FiletypeExtentionsInfo::mask_list/*, std::string("")*/);
        Attribute(_T("use-path"  ), &FiletypeExtentionsInfo::use_path, false);
    END_LAYOUT()
}; // struct FiletypeExtentionsInfo


struct FiletypeExtentionsInfo2
{
    std::string                file_type;
    std::vector<std::string>   mask_list;
    bool                       use_path;

    FiletypeExtentionsInfo2() 
       : file_type(), mask_list(), use_path(false) 
       {}

    FiletypeExtentionsInfo2(const std::string &ft, const std::vector<std::string> &ml, bool up)
       : file_type(ft), mask_list(ml), use_path(up)
       {}

    FiletypeExtentionsInfo2(const FiletypeExtentionsInfo2& i2) 
       : file_type(i2.file_type)
       , mask_list(i2.mask_list)
       , use_path(i2.use_path) 
       {}

    FiletypeExtentionsInfo2(const FiletypeExtentionsInfo& i) 
       : file_type(i.file_type)
       , mask_list()
       , use_path(i.use_path) 
       {
        Utils::split_string(mask_list, i.mask_list, ',');       
       }

    FiletypeExtentionsInfo2& operator=(const FiletypeExtentionsInfo2& i2) 
       {
        file_type = i2.file_type;
        mask_list = i2.mask_list;
        use_path  = i2.use_path;
        return *this;
       }
    FiletypeExtentionsInfo2& operator=(const FiletypeExtentionsInfo& i) 
       {
        file_type = i.file_type;
        mask_list.clear();
        use_path = i.use_path;
        Utils::split_string(mask_list, i.mask_list, ',');
        return *this;
       }

}; // struct FiletypeExtentionsInfo




struct TemplateInfo
{
    std::string    tpl_name;
    std::string    tpl_text;

    LAYOUT_DEFAULT(TemplateInfo)
        AttributeM(_T("name"  ), &TemplateInfo::tpl_name/*, std::string("")*/);
        Simple    ( 0 , &TemplateInfo::tpl_text, std::string("Template text not defined"));
        //Simple(_T("text" ), &TemplateInfo::tpl_text, std::string("Template text not defined"));
    END_LAYOUT()
}; // TemplateInfo


struct SelectionBlockInfo
{
    std::string    begin;
    std::string    end;

    LAYOUT_DEFAULT(SelectionBlockInfo)
        AttributeM(_T("begin"  ), &SelectionBlockInfo::begin);
        AttributeM(_T("end"  ), &SelectionBlockInfo::end);
    END_LAYOUT()
}; // SelectionBlockInfo


struct SelectionInfo
{
    std::string    file_type;
    bool           case_sensitive;
    std::vector<SelectionBlockInfo>  selBlockInfoVec;

    LAYOUT_DEFAULT(SelectionInfo)
        Attribute (_T("case-sensitive"), &SelectionInfo::case_sensitive, bool(true));
        AttributeM(_T("file-type")     , &SelectionInfo::file_type);
        Array     (0                   , &SelectionInfo::selBlockInfoVec, _T("block" ));
    END_LAYOUT()

}; // SelectionInfo



//<block-selections>
//    <selection file-type="c-src,cpp-src" case-sensitive="true">
//        <block begin="{" end="}"/>
//        <block begin="/*" end="*/"/>
//    </selection>
//
//    <selection file-type="pas-src" case-sensitive="false">
//        <block begin="Begin" end="End"/>
//        <!--block begin="/*" end="*/"/-->
//    </selection>
//
//</block-selections>





typedef ::TextTP_CommentTypeEnum CommentTypeEnum;

struct CommentInfo
{
    std::string    begin_sequence;
    std::string    end_sequence  ;
    std::string    sequence      ;
    unsigned       indent;
    bool           end_at_new_line;
    unsigned       new_line_indent_before;
    std::string    file_type;
    CommentTypeEnum comment_type;

    LAYOUT_DEFAULT(CommentInfo)
        Attribute (_T("begin-sequence") , &CommentInfo::begin_sequence, std::string(""));
        Attribute (_T("end-sequence")   , &CommentInfo::end_sequence, std::string(""));
        Attribute (_T("sequence")       , &CommentInfo::sequence, std::string(""));
        Attribute (_T("indent")         , &CommentInfo::indent, unsigned(0));
        Attribute (_T("end-at-new-line"), &CommentInfo::end_at_new_line, bool(false));
        Attribute (_T("new-line-indent-before"), &CommentInfo::new_line_indent_before, unsigned(0));
        AttributeM(_T("file-type")      , &CommentInfo::file_type/*, std::string("")*/);
        Attribute (_T("comment-type")   , &CommentInfo::comment_type, ctLines);
    END_LAYOUT()
}; // CommentInfo



struct ShortcutInfo
{
    std::string    shortcut;
    std::string    file_type;
    std::string    tpl_name;

    bool           line_before;
    bool           line_after;
    bool           keep_indent;

    LAYOUT_DEFAULT(ShortcutInfo)
        AttributeM(_T("name"  ),  &ShortcutInfo::shortcut/*, std::string("")*/);
        AttributeM(_T("file-type" ),  &ShortcutInfo::file_type/*, std::string("")*/);
        Attribute (_T("template"  ),  &ShortcutInfo::tpl_name, std::string(""));
        Attribute (_T("line-before" ), &ShortcutInfo::line_before, false);
        Attribute (_T("line-after"  ), &ShortcutInfo::line_after, false);
        Attribute (_T("keep-indent"  ), &ShortcutInfo::keep_indent, false);
    END_LAYOUT()

    inline
    bool operator<(const ShortcutInfo &shi) const
    {
     return shortcut < shi.shortcut;
    }

    inline
    bool partial_cmp_name(const std::string &str) const
    {
     if (str.empty()) return true;
     else if (str.size() == shortcut.size()) return str == shortcut;
     else if (str.size()  > shortcut.size()) return false;
     else return shortcut.compare(0, str.size(), str)==0;
    }

}; // struct ShortcutInfo


struct TextFormatterOption
{
    std::string      name;
    std::string      value;

    LAYOUT_DEFAULT(TextFormatterOption)
        AttributeM(_T("name"  ),  &TextFormatterOption::name);
        Simple(0, &TextFormatterOption::value, std::string(""));
    END_LAYOUT()
};

struct TextFormatterStyle
{
    std::string      name;       // style-name
    std::string      file_type;  // 
    std::string      plugin_name;
    std::string      hotkey;
    bool             useCRLF;
    bool             keep_indent;
    std::vector<TextFormatterOption> options;

    LAYOUT_DEFAULT(TextFormatterStyle)
        AttributeM(_T("name"  )   ,  &TextFormatterStyle::name);
        AttributeM(_T("file-type"),  &TextFormatterStyle::file_type);
        AttributeM(_T("plugin")   ,  &TextFormatterStyle::plugin_name);
        Attribute(_T("hotkey")    ,  &TextFormatterStyle::hotkey, std::string(""));
        Attribute(_T("useCRLF")   ,  &TextFormatterStyle::useCRLF, bool(true));
        Attribute(_T("keep-indent"), &TextFormatterStyle::keep_indent, bool(false));
        Array(_T("options" ), &TextFormatterStyle::options, _T("option" ));
    END_LAYOUT()

    std::string getEOL() const
       {
        if (useCRLF) return std::string("\r\n");
        return std::string("\n");
       }
};


/* struct CSingleBreakSymbolReader
 * {
 *     std::string value;
 *     std::string separator;
 *     LAYOUT_DEFAULT(CSingleBreakSymbolReader)
 *         AttributeM(_T("value")    ,  &CSingleBreakSymbolReader::value);
 *         Attribute(_T("separator") ,  &CSingleBreakSymbolReader::separator, std::string());
 *     END_LAYOUT()
 * }; // CSingleBreakSymbolReader
 *  
 * struct CBreakSymbolPair
 * {
 *     std::string begin;
 *     std::string end;
 *     LAYOUT_DEFAULT(CBreakSymbolPair)
 *         AttributeM(_T("begin"),  &CBreakSymbolPair::begin);
 *         AttributeM(_T("end")  ,  &CBreakSymbolPair::end);
 *     END_LAYOUT()
 * }; // CBreakSymbolPair
 *  
 * struct CBreakSymbolsReader
 * {
 *     std::vector<CSingleBreakSymbolReader>  singles;
 *     std::vector<CBreakSymbolPair>          pairs;
 *     LAYOUT_DEFAULT(CBreakSymbolsReader)
 *         Array(_T("value")    ,  &CBreakSymbolsReader::singles, _T("break-symbol"));
 *         Array(_T("separator") ,  &CBreakSymbolsReader::pairs , _T("break-symbol-pair"));
 *     END_LAYOUT()
 * }; // CSingleBreakSymbolReader
 */

struct CBreakSymbolPair
{
    std::string begin;
    std::string end;
    LAYOUT_DEFAULT(CBreakSymbolPair)
        AttributeM(_T("begin"),  &CBreakSymbolPair::begin);
        AttributeM(_T("end")  ,  &CBreakSymbolPair::end);
    END_LAYOUT()

    bool operator< (const CBreakSymbolPair &bp) const
       {
        if (begin.size() > bp.begin.size()) return true;
        if (end.size()   > bp.end.size())   return true;
        return false;
       }

}; // CBreakSymbolPair

inline
bool lessByLen(const std::string &s1, const std::string &s2)
   {
    return s1.size() > s2.size();
   }


struct CBreakSymbols
{
    std::string                        file_type;
    bool                               escapeBs;
    bool                               escapeDblQuot;
    bool                               singleQuoteStrings;
    bool                               doubleQuoteStrings;

    std::vector<std::string>           singles;
    std::vector<CBreakSymbolPair>      pairs;
    LAYOUT_DEFAULT(CBreakSymbols)
        AttributeM(_T("file-type"),  &CBreakSymbols::file_type);
        Attribute( _T("use-backslash-escape"),  &CBreakSymbols::escapeBs, true);
        Attribute( _T("use-duplicate-quotation-escape"),  &CBreakSymbols::escapeDblQuot, false);
        Attribute( _T("use-single-quotes"),  &CBreakSymbols::singleQuoteStrings, true);
        Attribute( _T("use-double-quotes"),  &CBreakSymbols::doubleQuoteStrings, true);
        Array(0 ,  &CBreakSymbols::singles, _T("break-symbol"));
        Array(0 ,  &CBreakSymbols::pairs , _T("break-symbol-pair"));
    END_LAYOUT()

    void sortSymbols()
       {
        std::stable_sort(singles.begin(), singles.end(), lessByLen);
        std::stable_sort(pairs.begin(), pairs.end() );
       }

}; // CSingleBreakSymbolReader


struct FileInclude
{
    std::string    file;
    LAYOUT_DEFAULT(FileInclude)
        Attribute(_T("file"  ), &FileInclude::file, std::string(""));
    END_LAYOUT()
};


struct EditorSettingChangeConfig
{
    int         tabSize;
    int         expandTabsToSpaces;
    int         autoIndent;
    int         cursorBeyondEol;
};

struct EditorSettingChangeReadConfig
{
    std::string file_type;
    int         tabSize;
    int         expandTabsToSpaces;
    int         autoIndent;
    int         cursorBeyondEol;
    LAYOUT_DEFAULT(EditorSettingChangeReadConfig)
        AttributeM(_T("file-type"  ), &EditorSettingChangeReadConfig::file_type);
        Attribute (_T("tab-size"   ),            &EditorSettingChangeReadConfig::tabSize, -1);
        Attribute (_T("expand-tabs-to-spaces" ), &EditorSettingChangeReadConfig::expandTabsToSpaces, -1);
        Attribute (_T("auto-indent"           ), &EditorSettingChangeReadConfig::autoIndent, -1);
        Attribute (_T("cursor-beyond-eol"     ), &EditorSettingChangeReadConfig::cursorBeyondEol, -1);
    END_LAYOUT()

    operator EditorSettingChangeConfig() const
       {
        EditorSettingChangeConfig escc;
        escc.tabSize            = tabSize;
        escc.expandTabsToSpaces = expandTabsToSpaces;
        escc.autoIndent         = autoIndent;
        escc.cursorBeyondEol    = cursorBeyondEol;
        return escc;
       }
};



struct TTPReadConfig
{
    std::vector<FiletypeExtentionsInfo>  filetypes_info;
    std::vector<TemplateInfo>            tpl_info;
    std::vector<ShortcutInfo>            shortcuts;
    std::vector<FileInclude>             includes;
    std::vector<CommentInfo>             comment_sequences;
    std::vector<SelectionInfo>           selection_info;
    std::vector<TextFormatterStyle>      formatting_styles;
    std::vector<CBreakSymbols>           break_symbols;
    std::vector<EditorSettingChangeReadConfig> editorSettingConfig;

    LAYOUT_DEFAULT(TTPReadConfig)
        Array(_T("templates" )            , &TTPReadConfig::tpl_info, _T("template" ));
        Array(_T("shortcuts" )            , &TTPReadConfig::shortcuts, _T("shortcut" ));
        Array(_T("comment-sequences")     , &TTPReadConfig::comment_sequences, _T("comment" ));
        Array(_T("file-types" )           , &TTPReadConfig::filetypes_info, _T("file-type" ));
        Array(_T("block-selections" )     , &TTPReadConfig::selection_info, _T("selection" ));
        Array(_T("formatting" )           , &TTPReadConfig::formatting_styles, _T("style" ));
        Array(_T("incremental-selection") , &TTPReadConfig::break_symbols, _T("break-symbols" ));
        Array(_T("editor-settings" )      , &TTPReadConfig::editorSettingConfig, _T("settings" ));
        Array(0, &TTPReadConfig::includes, _T("include" ));
    END_LAYOUT()

}; // struct TTPReadConfig


struct CCustomInfo
{
    std::string    config_file;
    std::string    template_dir;
    LAYOUT_DEFAULT(CCustomInfo)
        Simple(_T("config-file"  ), &CCustomInfo::config_file, std::string(""));
        Simple(_T("template-dir"  ), &CCustomInfo::template_dir, std::string(""));
    END_LAYOUT()
}; // struct CCustomInfo



struct CRemoveStylesIfPluginNotLoaded
{
    const std::map<std::string, TextFormatPluginInfo> &plugMap;

    CRemoveStylesIfPluginNotLoaded(const std::map<std::string, TextFormatPluginInfo> &pm) : plugMap(pm) {}
    CRemoveStylesIfPluginNotLoaded(const CRemoveStylesIfPluginNotLoaded &rm) : plugMap(rm.plugMap) {}
    bool operator() (const TextFormatterStyle &tfs)
       {
        /* std::string tmp("Lookup for plugin: ");
         * tmp += tfs.plugin_name;
         */

        std::map<std::string, TextFormatPluginInfo>::const_iterator it = plugMap.find(tfs.plugin_name);

        /* if (it==plugMap.end()) tmp += " - not found";
         * else                   tmp += " - found";        
         * MessageBox(0, tmp.c_str(), "CRemoveStylesIfPluginNotLoaded", MB_OK );
         */

        return it==plugMap.end();
       }
};


struct TTPConfig
{
    //std::map<std::string, std::string>   file_types; // by extention
    std::vector<FiletypeExtentionsInfo2> file_types;
    std::map<std::string, std::string>   templates;
    std::vector<ShortcutInfo>            shortcuts;
    std::vector<CommentInfo>             comment_sequences;
    std::vector<SelectionInfo>           selection_info;
    std::vector<TextFormatterStyle>      formatting_styles;
    std::vector<CBreakSymbols>           break_symbols;

    std::deque<std::string>              template_dirs;
    std::string                          pluginPath;
    std::map<std::string, EditorSettingChangeConfig>   editorSettingConfig;


    // used in temp copy only
    ITextFormatApiAllocator              IAllocator;
    std::map<std::string, TextFormatPluginInfo> plugMap;

    inline TTPConfig() : file_types(), templates(), shortcuts(), 
                         comment_sequences(), selection_info(), 
                         formatting_styles(), template_dirs(), 
                         plugMap(), pluginPath(), IAllocator(),
                         break_symbols()
        {
         initIAllocator(&IAllocator);
        }

    TTPConfig& operator=(const TTPConfig &ttp) 
        {
         if (&ttp==this) return *this;
         file_types = ttp.file_types;
         templates = ttp.templates;
         shortcuts = ttp.shortcuts;
         comment_sequences = ttp.comment_sequences;
         selection_info = ttp.selection_info; 
         formatting_styles = ttp.formatting_styles;
         template_dirs = ttp.template_dirs; 
         plugMap = ttp.plugMap;
         pluginPath = ttp.pluginPath;
         IAllocator = ttp.IAllocator;
         break_symbols = ttp.break_symbols;
         return *this;
        }


    void setPluginPath(const std::string &p)
        {
         //pluginPath = Utils::extractPath(p);
         pluginPath = filename::pathRemoveTrailingSlash(filename::getPath( p ));

         /* pluginPath = p;
          * std::string::size_type slPos = pluginPath.find_last_of('\\');
          * if (slPos == std::string::npos) return;
          * pluginPath.erase(slPos, std::string::npos);
          */
        }

    /* void buildPluginPath(HMODULE hMainPluginModule)
     *     {
     *      if (!pluginPath.empty()) return;
     *      char buf[2048];
     *      DWORD res = GetModuleFileName( hMainPluginModule, buf, sizeof(buf));
     *      if (!res) return;
     *      buf[res] = 0;
     *      int i = (int)res;
     *      for(; i>=0; --i)
     *         if (buf[i]=='\\')
     *            {
     *             buf[i] = 0;
     *             pluginPath = buf;
     *             return;
     *            }
     *     }
     */

    inline
    void AssignConfig(const TTPReadConfig &cfg, const FARSTANDARDFUNCTIONS* stdfns)
    {
        //file_types.insert(file_types.end(), cfg.filetypes_info.begin(), cfg.filetypes_info.end());

        {
            std::vector<FiletypeExtentionsInfo>::const_iterator it = cfg.filetypes_info.begin();

            for (; it!=cfg.filetypes_info.end(); it++)
                {
                 if (it->file_type.empty()) continue;
                 std::vector<std::string> mask_list;
                 Utils::split_string(mask_list, it->mask_list, ',');
                 if (mask_list.empty()) continue;
             
                 Utils::trim(mask_list, stdfns);
                 Utils::strlwr(mask_list, stdfns);

                 file_types.push_back(FiletypeExtentionsInfo2(it->file_type, mask_list, it->use_path));
                 //lowerCase
             
                 /* std::vector<std::string>::iterator eit = mask_list.begin();
                  * for(; eit!=mask_list.end(); eit++)
                  *     {
                  *      if (it->file_type.empty()) continue;
                  *      file_types[*eit] = it->file_type;
                  *     }
                  */
                }
        }

        {
            std::vector<TemplateInfo>::const_iterator it = cfg.tpl_info.begin();
            for (; it!=cfg.tpl_info.end(); it++)
                {
                 if (it->tpl_name.empty()) continue;
                 templates[it->tpl_name] = it->tpl_text;                
                }
        }

        //shortcuts = cfg.shortcuts;
        shortcuts.insert(shortcuts.end(), cfg.shortcuts.begin(), cfg.shortcuts.end());
        comment_sequences.insert(comment_sequences.end(), cfg.comment_sequences.begin(), cfg.comment_sequences.end());
        selection_info.insert(selection_info.end(), cfg.selection_info.begin(), cfg.selection_info.end());
        formatting_styles.insert(formatting_styles.end(), cfg.formatting_styles.begin(), cfg.formatting_styles.end());

        {
         std::vector<EditorSettingChangeReadConfig>::const_iterator it = cfg.editorSettingConfig.begin();
         for(; it!=cfg.editorSettingConfig.end(); ++it)
            {
             if (it->file_type.empty()) continue;
             //Warning(10, "assign 1 item");
             //Info.Message( "Warning!", "assign 1 item", FMSG_WARNING);
             editorSettingConfig[it->file_type] = *it;
            }
        }

/*
        std::vector<CBreakSymbols>::size_type bsFirstIdx = break_symbols.size();
        break_symbols.insert(break_symbols.end(), cfg.break_symbols.begin(), cfg.break_symbols.end());
        std::vector<CBreakSymbols>::iterator brIt = break_symbols.begin() + bsFirstIdx;

        for(; brIt!=break_symbols.end(); ++brIt)
           {
            brIt->sortSymbols();
           }
*/
        break_symbols.insert(break_symbols.end(), cfg.break_symbols.begin(), cfg.break_symbols.end());
        std::vector<CBreakSymbols>::iterator brIt = break_symbols.begin();
        for(; brIt!=break_symbols.end(); ++brIt)
           {
            brIt->sortSymbols();
           }




        

/*
        std::vector<TextFormatterStyle>::iterator newFmtEnd = std::remove_if( 
                                                     formatting_styles.begin(), 
                                                     formatting_styles.end(), 
                                                     CRemoveStylesIfPluginNotLoaded(plugMap)
                                                                            );
        formatting_styles.erase (newFmtEnd, formatting_styles.end() );
*/
    }

    inline
    bool isEmpty()
    {
        return file_types.empty() && templates.empty() && shortcuts.empty();
    }
    
    void ReadConfigAux(const char *from_file, std::string &readed_file_name, std::map<std::string, bool> &readed, const FARSTANDARDFUNCTIONS* stdfns);

    inline
    void ReadConfig(const char *from_file, std::string &readed_file_name, const FARSTANDARDFUNCTIONS* stdfns)
    {
     std::map<std::string, bool> readed;
     ReadConfigAux(from_file, readed_file_name, readed, stdfns);

    }

    void eraseUnisedFormatterStyles()
    {
     std::vector<TextFormatterStyle>::iterator newFmtEnd = std::remove_if( 
                                                  formatting_styles.begin(), 
                                                  formatting_styles.end(), 
                                                  CRemoveStylesIfPluginNotLoaded(plugMap)
                                                                         );
     formatting_styles.erase (newFmtEnd, formatting_styles.end() );    
    }

    void loadFormatterPlugins(const std::string &pL = "")
    {
/*
     CHAR buf[1024];
     DWORD res = ::GetCurrentDirectory(sizeof(buf), buf);
     if (res)
        {
         buf[res] = 0;
         pathList = buf;
        }
*/
     std::string pathList(pL.empty() ? pluginPath : pL);
     //MessageBox(0, pathList.c_str(), "loadFormatterPlugins", MB_OK );

     loadTextFormatPlugins(plugMap, pathList, &IAllocator);
       
    }

    void unloadFormatterPlugins()
    {
     //MessageBox(0, "freeFormatterPlugins", "unloadFormatterPlugins", MB_OK );
     freeTextFormatPluginsMap(plugMap, &IAllocator);
    }
    

    protected:
    bool templateIsFile(const std::string &tpl_name, std::string &file_name) const;
    bool templateIsInline(const std::string &tpl_name, std::string &tplText) const;
    bool templateReadFile(const std::string &dir, const std::string& file, std::string &tpl_text) const;
    bool templateReadFile(const std::deque<std::string> &dirs, const std::string& file, std::string &tpl_text) const;
    bool getTemplateText(const std::deque<std::string> &dirs, const std::string& tpl_name, std::string &tpl_file, std::string &tpl_text) const;


    public:

    void readFromFile(const std::string &file);

    inline
    bool getTemplateText(const std::string& tpl_name, std::string &tpl_file, std::string &tpl_text) const
    {
        return getTemplateText(template_dirs, tpl_name, tpl_file, tpl_text);
    }

    
    




}; // struct TTPConfig






}; // namespace TextTP


#endif /* TTPCONF_H */





