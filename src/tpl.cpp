
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

#if !defined(_MEMORY_) && !defined(_STLP_MEMORY) && !defined(__STD_MEMORY__) && !defined(_CPP_MEMORY) && !defined(_GLIBCXX_MEMORY)
    #include <memory>
#endif

#if !defined(_INC_WCHAR) && !defined(_WCHAR_H_) && !defined(_WCHAR_H)
    #include <wchar.h>
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

#if !defined(_ALGORITHM_) && !defined(_STLP_ALGORITHM) && !defined(__STD_ALGORITHM) && !defined(_CPP_ALGORITHM) && !defined(_GLIBCXX_ALGORITHM)
    #include <algorithm>
#endif

#if !defined(_INC_FLOAT) && !defined(_FLOAT_H_) && !defined(_FLOAT_H)
    #include <float.h>
#endif

#if !defined(_INC_MALLOC) && !defined(_MALLOC_H_) && !defined(_MALLOC_H)
    #include <malloc.h>
#endif

#include <algorithm>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit

#ifndef __ATLBASE_H__
    #include <atlbase.h>
#endif

#ifndef __ATLDEF_H__
    #include <atldef.h>
#endif
/*
#ifndef MARTY_CPPHLP_H
    #include <marty/cpphlp.h>
#endif
*/

//#include <marty/filename.h>

#ifndef MARTY_CASECONV_H
    #include <marty/caseconv.h>
#endif

#ifdef FILENAME_IN_MARTY_NAMESPACE
    using namespace marty;
#endif

#include "tpl.h"
#include "cfarcls.h"
#include "ttprc.h"
#include "ttpconf.h"
#include "autocom.h"
#include "guid.h"


//#include <iostream>
//#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <time.h>


#ifndef SIZEOF_ARRAY
    #define SIZEOF_ARRAY(a)   (sizeof(a)/sizeof(a[0]))
#endif


extern Far::CPluginStartupInfo  Info;
extern TextTP::TTPConfig        Config;
//extern std::string              plugin_tpl_dir_name;
namespace TextTP
{




//-----------------------------------------------------------------------------
void Warning(int title_id, const char *text)
{
    Warning(title_id, std::string(text ? text : "<NULL>"));
}

//-----------------------------------------------------------------------------
void Warning(int title_id, const std::string &text)
{
    char buf[64];
    const char *msg_title = ::Info.GetMsg(title_id);
    
    if (!msg_title)
       {
        ::Info.StdFn()->sprintf(buf, "Text Templates (title strid=%d)", title_id);
       }
    std::string msg(msg_title ? msg_title : buf);
    msg += "\n";
    msg += text;
    msg += "\nOk";
    ::Info.Message( FMSG_WARNING|FMSG_ALLINONE, 0, /*HelpTopic*/
                    (const char * const *)msg.c_str(), 1, 1);
}

//-----------------------------------------------------------------------------
bool templateFindCursorInLine(std::string &str, int &pos)
{
    std::string::size_type p = str.find('@');
    while(p!=str.npos)
        {
         if ((p+1 >= str.size()) || str[p+1]!='@') 
            {
             pos = (int)p;
             str.erase(p, 1);
             return true;
            }
         // double @ (@@)
         str.erase(p, 1);
         p = str.find('@', p+1);
        }
    return false;
}

//-----------------------------------------------------------------------------
bool templateFindCursor(std::vector<std::string> &lines, int &pos, int &line)
{
    pos = 0; line = 0;
    std::vector<std::string>::iterator it = lines.begin();
    for (; it!=lines.end(); it++, line++)
        {
         if (templateFindCursorInLine(*it, pos))
            return true;
        }
    return false;
}

//-----------------------------------------------------------------------------
/*
bool templateIsFile(const std::string &tpl_name, std::string &file_name)
{
    std::string prefix_file = "file:";
    if (tpl_name.size() < prefix_file.size()) return false;
    bool res = tpl_name.compare(0, prefix_file.size(), prefix_file)==0;
    if (res)
       {
        file_name = std::string(tpl_name, prefix_file.size());
       }
    return res;
}
*/

//-----------------------------------------------------------------------------
/*
std::string templateGetText(const std::string &tpl_name)
{
    std::string tpl_text;
    std::vector<std::string> dirs;
    //dirs.push_back(plugin_tpl_dir_name);
    std::string tpl_file;

    if (Config.getTemplateText( tpl_name, tpl_file, tpl_text)) return tpl_text;

    char buf[1024];
    const char *fmt = ::Info.GetMsg(tpl_file.empty() ? IDM_TPL_NOT_FOUND_ERR : IDM_TPL_FILE_ERR);
    if (!fmt)
       {
        ::Info.StdFn()->sprintf( buf, "Message string '%s' not found", 
                                 (tpl_file.empty() ? "IDM_TPL_NOT_FOUND_ERR" : "IDM_TPL_FILE_ERR"));
       }
    else
       {
        ::Info.StdFn()->sprintf( buf, fmt, (tpl_file.empty() ? tpl_name.c_str() : tpl_file.c_str()));
       }

    Warning(IDM_TEXTTP_TITLE, buf);
    return std::string();
}
*/

//-----------------------------------------------------------------------------
std::string templateParse( const std::string &tpl_text, 
                       const std::string &editor_file_name, 
                       const std::string &tpl_name)
{
    std::map<std::string, std::string> values;

    templateMakeStdMacrosses( values, editor_file_name);    

    std::string text = tpl_text;
    std::string::size_type pos = text.find('$');

    while(pos!=text.npos)
        {
         std::string::size_type pos2 = text.find('$', pos+1);
         if (pos2!=text.npos)
            {
             std::string::size_type name_len = pos2 - pos - 1;
             std::string val_name(text, pos+1, name_len);
             std::string value;

             if (!templateGetValue( tpl_name, values, val_name, value))
                return std::string(); // user cancels input

             text.replace(pos, name_len + 2, value);

             pos = text.find('$', pos + value.size());
            }
         else 
            pos = pos2;
        }

    pos = text.find(_T('\r'));
    while(pos!=text.npos)
        {
         text.erase(pos, 1);
         pos = text.find(_T('\r'), pos);
        }

    return text;
}

//-----------------------------------------------------------------------------
bool templateGetValue( const std::string &tpl_name, 
                       std::map<std::string, std::string> &val_map, 
                       const std::string &val_name, 
                       std::string &value)
{
    std::map<std::string, std::string>::iterator it = val_map.find(val_name);
    if (it!=val_map.end())
       {
        value = it->second;
        return true;
       }

    char subtitle[256];

    const char *subtitle_fmt = ::Info.GetMsg(IDM_ENTER_PARAM_VALUE);
    if (!subtitle_fmt)
       subtitle_fmt = "Enter value for %s parameter";
    ::Info.StdFn()->sprintf(subtitle, subtitle_fmt, val_name.c_str());

    std::string history_name("TextTPInputValueHistoryFor");
    history_name += tpl_name + val_name;

    char input_buf[256];

    int inp_res = ::Info.InputBox( ::Info.GetMsg(IDM_TEXTTP_TITLE),
                                   subtitle, history_name.c_str(),
                                   0, /* Src text */
                                   input_buf, SIZEOF_ARRAY(input_buf)-1,
                                   0, /* HelpTopic */
                                   FIB_BUTTONS);
    if (!inp_res) return false;
    value = input_buf;
    val_map[val_name] = value;
    return true;
}

//-----------------------------------------------------------------------------
std::string templateMakeCSrcName(const std::string &str)
{
    std::string res = str;
    std::string::size_type i = 0;
    for (; i<res.size(); i++ )
        {
         if (res[i]=='\r' || res[i]=='\n' || res[i]=='\t') continue;
         if (res[i]>='A' && res[i]<='Z') continue;
         if (res[i]>='0' && res[i]<='9') continue;
         if (res[i]>126) 
            res[i] = 'X';
         else if (res[i]>='a' && res[i]<='z')
            {
             res[i] = res[i] - 'a' + 'A';
             continue;
            }
         else res[i] = '_';
        }
    return res;   
}

//-----------------------------------------------------------------------------
struct toCppMacro
{
    private:
        toCppMacro& operator=(const toCppMacro&) { return *this; }
    public:
        toCppMacro(){}
        char operator()(char ch)
           {
            if (ch>='a' && ch<='z') return ch - 'a' + 'A';
            if (ch>='A' && ch<='Z') return ch;
            if (ch>='0' && ch<='9') return ch;
            return '_';
           }
        wchar_t operator()(wchar_t ch)
           {
            if (ch>=L'a' && ch<=L'z') return ch - L'a' + L'A';
            if (ch>=L'A' && ch<=L'Z') return ch;
            if (ch>=L'0' && ch<=L'9') return ch;
            return '_';
           }
};

//-----------------------------------------------------------------------------
std::string templateCIncGuardName(const std::string &str)
{
    ::std::vector< ::std::string > pathParts;
    //marty::
    filename::splitPath( str, pathParts );
    
    ::std::vector< ::std::string >::iterator pit = pathParts.begin();
    for(; pit!=pathParts.end(); ++pit)
       {
        ::std::transform(pit->begin(), pit->end(), pit->begin(), toCppMacro());
       }

    ::std::vector< ::std::string >::const_reverse_iterator rpit = pathParts.rbegin();

    for(; rpit!=pathParts.rend(); ++rpit)
       {
        if (*rpit=="INCLUDE" || *rpit=="INC" || *rpit=="H" || *rpit=="SOURCE" || *rpit=="SRC")
           {
            if (*rpit=="INCLUDE" || *rpit=="INC" || *rpit=="H")
               {  /* custom = false; */  }
            else 
               rpit++;
            break;           
           }
       }

    ::std::vector< ::std::string >::const_iterator cpit = rpit.base();
/*
    bool custom = true;
    if (cpit==pathParts.end())
       custom = false;
*/
/*
    if (cpit==pathParts.end())
       cpit = pathParts.end() - 1;
    else
       ++cpit;
*/
    ::std::string res;
    for(; cpit!=pathParts.end(); ++cpit)
       {
        if (!res.empty()) res.append(1, '_');
        res.append(*cpit);
       }
    return res;
/*
    if (custom)
       return ::std::string("\"") + res + ::std::string("\"");
    else
       return ::std::string("<") + res + ::std::string(">");
*/
}

//-----------------------------------------------------------------------------
std::string templateCIncFileName(const std::string &str)
{
    ::std::vector< ::std::string > pathParts;
    //marty::
    filename::splitPath( str, pathParts );
    
    bool custom = true;
    ::std::vector< ::std::string >::const_reverse_iterator crpit = pathParts.rbegin();
    for(; crpit!=pathParts.rend(); ++crpit)
       {
        ::std::string tmp = *crpit;
        ::std::transform(tmp.begin(), tmp.end(), tmp.begin(), marty::util::asciiUppercaseChar());
        if (tmp=="INCLUDE" || tmp=="INC" || tmp=="H" || tmp=="SOURCE" || tmp=="SRC")
           {
            if (tmp=="INCLUDE" || tmp=="INC" || tmp=="H")
               custom = false;
            else 
               crpit++;
            break;
           }
       }

    ::std::vector< ::std::string >::const_iterator cpit = crpit.base();
    ::std::string res;
    for(; cpit!=pathParts.end(); ++cpit)
       {
        if (!res.empty()) res.append(1, '/');
        res.append(*cpit);
       }

    if (custom)
       return ::std::string("\"") + res + ::std::string("\"");
    else
       return ::std::string("<") + res + ::std::string(">");
}


//-----------------------------------------------------------------------------
inline std::string mkDTVarName(const std::string &type, const std::string &name)
{
    std::string res("_");
    res += type;
    res += "DT_";
    //res += "_";
    res += name;
    res += "_";
    return res;
}

//-----------------------------------------------------------------------------
void templateMakeDateTimeMacrosses( std::map<std::string, std::string> &val_map,
                                    struct tm *ptm,
                                    const std::string &type)
{
    if (!ptm) return;

    static char *str_days[7]   = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
    static char *str_shdays[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    static char *str_mon[12]   = {"January", "February", "March", "April", "May", "June",
                "July", "August", "September", "October", "November", "December"};
    static char *str_shmon[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                                  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    val_map[mkDTVarName(type, "a")] = str_shdays[ptm->tm_wday];
    val_map[mkDTVarName(type, "A")] = str_days[ptm->tm_wday];
    val_map[mkDTVarName(type, "b")] = str_shmon[ptm->tm_mon];
    val_map[mkDTVarName(type, "B")] = str_mon[ptm->tm_mon];
    val_map[mkDTVarName(type, "p")] = ptm->tm_hour<12 ? "AM" : "PM";

    char buf[128];

    ::Info.StdFn()->sprintf(buf, "%d", ptm->tm_mday);
    val_map[mkDTVarName(type, "d")] = buf;

    ::Info.StdFn()->sprintf(buf, "%02d", ptm->tm_mday);
    val_map[mkDTVarName(type, "dd")] = buf;

    ::Info.StdFn()->sprintf(buf, "%d", ptm->tm_hour);
    val_map[mkDTVarName(type, "H")] = buf;

    ::Info.StdFn()->sprintf(buf, "%02d", ptm->tm_hour);
    val_map[mkDTVarName(type, "HH")] = buf;

    int ampm_hour = ptm->tm_hour%12;
    if (!ampm_hour) ampm_hour = 12;

    ::Info.StdFn()->sprintf(buf, "%d", ampm_hour);
    val_map[mkDTVarName(type, "h")] = buf;

    ::Info.StdFn()->sprintf(buf, "%02d", ampm_hour);
    val_map[mkDTVarName(type, "hh")] = buf;

    ::Info.StdFn()->sprintf(buf, "%d", ptm->tm_mon+1);
    val_map[mkDTVarName(type, "m")] = buf;

    ::Info.StdFn()->sprintf(buf, "%02d", ptm->tm_mon+1);
    val_map[mkDTVarName(type, "mm")] = buf;

    ::Info.StdFn()->sprintf(buf, "%d", ptm->tm_min);
    val_map[mkDTVarName(type, "M")] = buf;

    ::Info.StdFn()->sprintf(buf, "%02d", ptm->tm_min);
    val_map[mkDTVarName(type, "MM")] = buf;

    ::Info.StdFn()->sprintf(buf, "%d", ptm->tm_sec);
    val_map[mkDTVarName(type, "S")] = buf;

    ::Info.StdFn()->sprintf(buf, "%02d", ptm->tm_sec);
    val_map[mkDTVarName(type, "SS")] = buf;

    ::Info.StdFn()->sprintf(buf, "%d", ptm->tm_wday);
    val_map[mkDTVarName(type, "w")] = buf;

    ::Info.StdFn()->sprintf(buf, "%02d", ptm->tm_year%100);
    val_map[mkDTVarName(type, "y")] = buf;

    ::Info.StdFn()->sprintf(buf, "%04d", 1900+ptm->tm_year);
    val_map[mkDTVarName(type, "Y")] = buf;
        
    val_map[mkDTVarName(type, "TIME")] = val_map[mkDTVarName(type, "HH")] + std::string(":") + 
                                         val_map[mkDTVarName(type, "MM")] + std::string(":") + 
                                         val_map[mkDTVarName(type, "SS")];

    val_map[mkDTVarName(type, "DATE")] = val_map[mkDTVarName(type, "dd")] + std::string(".") + 
                                         val_map[mkDTVarName(type, "mm")] + std::string(".") + 
                                         val_map[mkDTVarName(type, "Y")];

    val_map[mkDTVarName(type, "LDATE")] = val_map[mkDTVarName(type, "A")] + std::string(", ") + 
                                          val_map[mkDTVarName(type, "B")] + std::string(" ") + 
                                         val_map[mkDTVarName(type, "d")] + std::string(", ") + 
                                         val_map[mkDTVarName(type, "Y")];

    val_map[mkDTVarName(type, "SDATE")] = val_map[mkDTVarName(type, "a")] + std::string(", ") + 
                                          val_map[mkDTVarName(type, "b")] + std::string(" ") + 
                                         val_map[mkDTVarName(type, "d")] + std::string(", ") + 
                                         val_map[mkDTVarName(type, "Y")];


/*
    ::Info.StdFn()->sprintf(buf, "%d", ptm->);
    val_map[mkDTVarName(type, "")] = buf;
*/


//Wed Jan 02 02:03:55 1980\n\0
/*
tm_sec   Seconds after minute (0 - 59). 
tm_min   Minutes after hour (0 - 59). 
tm_hour  Hours after midnight (0 - 23). 
tm_mday  Day of month (1 - 31). 
tm_mon   Month (0 - 11; January = 0). 
tm_year  Year (current year minus 1900). 
tm_wday  Day of week (0 - 6; Sunday = 0). 
tm_yday  Day of year (0 - 365; January 1 = 0). 
tm_isdst 
*/
}

//-----------------------------------------------------------------------------
void templateMakeStdMacrosses( std::map<std::string, std::string> &val_map, 
                               const std::string &editor_file_name)
{
    val_map[""] = "$";

    std::string::size_type ext_pos = editor_file_name.rfind('.');
    std::string ext = ext_pos==std::string::npos ? std::string() : std::string(editor_file_name, ext_pos+1);

    std::string::size_type path_pos1 = editor_file_name.rfind('\\');
    std::string::size_type path_pos2 = editor_file_name.rfind('/');
    std::string::size_type path_pos = path_pos1 < path_pos2 ? path_pos1 : path_pos2;
    std::string path = path_pos==std::string::npos ? std::string() : std::string(editor_file_name, 0, path_pos);

    if (path_pos==std::string::npos)
       path_pos = 0;
    else
       path_pos++;
    std::string::size_type name_len = ext_pos==std::string::npos ? std::string::npos : ext_pos - path_pos;
    std::string name = std::string(editor_file_name, path_pos, name_len);

    val_map["_file_"]      = editor_file_name;
    val_map["_file_name_"] = name;
    val_map["_file_path_"] = path;
    val_map["_file_ext_"]  = ext;
    val_map["_FILE_"]      = templateMakeCSrcName(editor_file_name);
    val_map["_FILE_NAME_"] = templateMakeCSrcName(name);
    val_map["_FILE_PATH_"] = templateMakeCSrcName(path);
    val_map["_FILE_EXT_"]  = templateMakeCSrcName(ext);

    val_map["_‘_INC_GUARD_"]  =  /* ::std::string("QQQ") + */  templateCIncGuardName(editor_file_name);
    val_map["_á_inc_file_"]   =  /* ::std::string("QQQ") + */  templateCIncFileName(editor_file_name);

    //val_map[""] = 
    //val_map[""] = 

    char* pch_clpb = ::Info.StdFn()->PasteFromClipboard();    
    std::string str_clpb = pch_clpb ? pch_clpb : "";
    std::string::size_type pos = str_clpb.find('\r');
    while(pos!=str_clpb.npos)
        {
         str_clpb.erase(pos, 1);
         pos = str_clpb.find('\r', pos);
        }

    val_map["_clipboard_"]  = str_clpb;
    val_map["_CLIPBOARD_"]  = templateMakeCSrcName(str_clpb);
    if (pch_clpb) 
       ::Info.StdFn()->DeleteBuffer( pch_clpb );


    time_t t;
    time( &t );

    struct tm *ptm = localtime( &t );
    templateMakeDateTimeMacrosses( val_map, ptm, "L");
    ptm = gmtime( &t );
    templateMakeDateTimeMacrosses( val_map, ptm, "G");


    INIT_COM();

    GUID guid;
    CoCreateGuid(&guid);
    
    char guid_buf[256];
    val_map["_GUID_REGISTRY_"] = format_guid_registry(guid_buf, &guid);
    val_map["_GUID_CMACRO_"]   = format_guid_clang(guid_buf, &guid);
    val_map["_GUID_DATA1_"]    = format_guid_part(guid_buf, &guid, 0);
    val_map["_GUID_DATA2_"]    = format_guid_part(guid_buf, &guid, 1);
    val_map["_GUID_DATA3_"]    = format_guid_part(guid_buf, &guid, 2);
    val_map["_GUID_DATA40_"]   = format_guid_part(guid_buf, &guid, 3);
    val_map["_GUID_DATA41_"]   = format_guid_part(guid_buf, &guid, 4);
    val_map["_GUID_DATA42_"]   = format_guid_part(guid_buf, &guid, 5);
    val_map["_GUID_DATA43_"]   = format_guid_part(guid_buf, &guid, 6);
    val_map["_GUID_DATA44_"]   = format_guid_part(guid_buf, &guid, 7);
    val_map["_GUID_DATA45_"]   = format_guid_part(guid_buf, &guid, 8);
    val_map["_GUID_DATA46_"]   = format_guid_part(guid_buf, &guid, 9);
    val_map["_GUID_DATA47_"]   = format_guid_part(guid_buf, &guid, 10);
}
//-----------------------------------------------------------------------------




}; // namespace TextTP



