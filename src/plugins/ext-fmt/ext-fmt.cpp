#include "../../itextfmt.h"


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
    #include "../../fmtUtils.h"
#endif




std::string formatSysErr(DWORD errCode);
HANDLE CreateExtFmtTempFile(char *fileNameBuf, /* must be 2*MAX_PATH */
                            DWORD dwFlagsAndAttributes
                           );
bool templateParse( const std::string &tpl_text, 
                    std::map<std::string, std::string> &cmdLineParsingParams,
                    std::string &text,
                    std::string &errMacroName,
                    bool eraseUsedNames = false);


//-----------------------------------------------------------------------------
inline 
std::string formatSysErr()
{
   return formatSysErr(GetLastError());
}

//-----------------------------------------------------------------------------
std::string formatSysErr(DWORD errCode)
{
  char* lpMsgBuf = 0; 
  DWORD fmtRes = FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                                0, errCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                (LPTSTR)&lpMsgBuf, 0, 0 );

  if (!fmtRes)
     {
      std::stringstream ss;
      ss<<"Code: "<<errCode;
      return ss.str();
     }

  CharToOem(lpMsgBuf,lpMsgBuf);
  std::string resStr(lpMsgBuf, (std::string::size_type)fmtRes );
  LocalFree( lpMsgBuf );
  return resStr;
}

//-----------------------------------------------------------------------------
//ERROR_ACCESS_DENIED
//ERROR_ALREADY_EXISTS
HANDLE CreateExtFmtTempFile(char *fileNameBuf, /* must be 2*MAX_PATH */
                            DWORD dwFlagsAndAttributes
                           )
   {
    SECURITY_ATTRIBUTES saAttr; 
    saAttr.nLength = sizeof(saAttr);
    saAttr.bInheritHandle = TRUE; 
    saAttr.lpSecurityDescriptor = 0;

    char tempPath[MAX_PATH];
    DWORD dwRes = GetTempPath(sizeof(tempPath)/sizeof(tempPath[0]), tempPath);
    if (!dwRes)
       {
        lstrcpy(tempPath, "C:\\");
       }
    else
       {
        if (tempPath[dwRes-1]!='\\' && tempPath[dwRes-1]!='/')
           {
            tempPath[dwRes++] = '\\';
           }
        tempPath[dwRes] = 0;
       }


    for(int i=0; i<10000; ++i)
       {                                //Path ~eft0000.tmp
        int iRews = wsprintf( fileNameBuf, "%s~eft%04d.tmp", tempPath, i);
        fileNameBuf[iRews] = 0;
        HANDLE hHandleWrite = CreateFile( fileNameBuf, 
                                   GENERIC_READ|GENERIC_WRITE,
                                   0, //FILE_SHARE_READ|FILE_SHARE_WRITE,
                                   &saAttr, 
                                   CREATE_NEW,
                                   dwFlagsAndAttributes,
                                   0);
        if (hHandleWrite==INVALID_HANDLE_VALUE)
           continue;

        return hHandleWrite;
/*
        hHandleRead  = CreateFile( fileNameBuf, 
                                   GENERIC_READ,
                                   FILE_SHARE_READ|FILE_SHARE_WRITE,
                                   0, // &saAttr, // not inheritable
                                   OPEN_EXISTING,
                                   dwFlagsAndAttributes&(~FILE_FLAG_DELETE_ON_CLOSE),
                                   0);
*/
       }

    return INVALID_HANDLE_VALUE;
   }

//-----------------------------------------------------------------------------
bool templateParse( const std::string &tpl_text, 
                    std::map<std::string, std::string> &cmdLineParsingParams,
                    std::string &text,
                    std::string &errMacroName,
                    bool eraseUsedNames)
{
    std::map<std::string, bool> paramsUsed;
    //std::map<std::string, std::string> values;

    //std::string 
    text = tpl_text;
    std::string::size_type pos = text.find('$');

    while(pos!=text.npos)
        {
         std::string::size_type pos2 = text.find('$', pos+1);
         if (pos2==text.npos)
            pos = pos2;
         else
            {
             std::string::size_type name_len = pos2 - pos - 1;
             std::string val_name(text, pos+1, name_len);

             std::map<std::string, std::string>::iterator parIt = cmdLineParsingParams.find(val_name);
             if (parIt==cmdLineParsingParams.end())
                {
                 errMacroName = val_name;
                 return false;
                }

             paramsUsed[parIt->first] = true;
             text.replace(pos, name_len + 2, parIt->second);

             pos = text.find('$', pos + parIt->second.size());
            }
        }

    if (!eraseUsedNames) return true;
    // remove used params
    std::map<std::string, bool>::iterator uIt = paramsUsed.begin();
    for(; uIt!=paramsUsed.end(); ++uIt)
       {
        cmdLineParsingParams.erase(cmdLineParsingParams.find(uIt->first));
       }
    return true;
}

//-----------------------------------------------------------------------------
struct CErrMsgBuilder
{
    PITextFormatApiAllocator allocator;
    LPSTR *resText;
    SIZE_T *nResBytes;
    std::stringstream ss;

    CErrMsgBuilder( PITextFormatApiAllocator pIa,
                    LPSTR *rT, SIZE_T *nB )
       : allocator(pIa), resText(rT), nResBytes(nB), ss() {}

    template<typename T>
    std::stringstream& operator<<(const T &t)
       {
        ss<<t;
        return ss;
       }

    LPSTR ret()
       {
        std::string resTextStr(ss.str());
        LPSTR resTextBuf = (LPSTR)allocator->pfnAllocate(resTextStr.size());
        *resText   = resTextBuf;
        *nResBytes = resTextStr.size();

        #if defined(_MSC_VER) && _MSC_VER>=1400
        #pragma warning( push )
        #pragma warning( disable : 4996 )
        #endif

        resTextStr.copy(resTextBuf, resTextStr.size());

        #if defined(_MSC_VER) && _MSC_VER>=1400
        #pragma warning( pop )
        #endif

        return 0;
       }
};

//-----------------------------------------------------------------------------
#define ERRMSG CErrMsgBuilder errMsg(allocator, resText, nResBytes)
// errMsg.ret()
#define RETERR return errMsg.ret()

//-----------------------------------------------------------------------------
inline
bool hasSpaces(const std::string &str)
   {
    std::string::size_type pos = 0, size = str.size();
    for(; pos<size; ++pos)
       if (str[pos]==' ') return true;
    return false;
   }

//-----------------------------------------------------------------------------
extern "C"{
LPSTR WINAPI textFormatApiGetPluginInternalName(PITextFormatApiAllocator allocator, const FARSTANDARDFUNCTIONS* pStdFn);

LPSTR WINAPI textFormatApiFormatText(LPCSTR text, SIZE_T nBytes,
                                 LPCSTR options, PITextFormatApiAllocator allocator,
                                 const FARSTANDARDFUNCTIONS* pStdFns,
                                 LPSTR *resText, SIZE_T *nResBytes);
};




//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
inline 
std::string findOption(const std::map<std::string, std::string> optMap, const std::string &optName, const std::string &optDefVal)
    {
     std::map<std::string, std::string>::const_iterator it = optMap.find(optName);
     if (it==optMap.end()) return optDefVal;
     if (it->second.empty()) return optDefVal;
     return it->second;
    }

//-----------------------------------------------------------------------------
inline 
std::string findOption(const std::map<std::string, std::string> optMap, const std::string &optName, const char *optDefVal)
    {
     if (optDefVal) return findOption(optMap, optName, std::string(optDefVal));
     else           return findOption(optMap, optName, std::string());
    }


//-----------------------------------------------------------------------------
LPSTR WINAPI textFormatApiGetPluginInternalName(PITextFormatApiAllocator allocator, const FARSTANDARDFUNCTIONS* pStdFn)
    {
     LPSTR name  = "external-formatter";
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

     std::map<std::string, std::string>::const_iterator optIt = optMap.find("command");
     if (optIt==optMap.end())
        {
         ERRMSG;
         errMsg<<"Option 'command' not found";
         RETERR;
        }

     std::string cmd(optIt->second);
     // std::string cmd( hasSpaces(optIt->second) 
     //                ? (std::string("\"") + optIt->second + std::string("\""))
     //                : optIt->second
     //                );

     optIt = optMap.find("command-line");
     std::string cmd_line( optIt!=optMap.end()
                         ? optIt->second
                         : std::string()
                         );

     optIt = optMap.find("__PluginBasePath");
     std::string pluginPath( optIt!=optMap.end()
                         ? optIt->second
                         : std::string()
                         );

     bool keepTempFiles = false;
     optIt = optMap.find("keep-temp-files");
     if (optIt!=optMap.end())
        {
         if (optIt->second=="true" || optIt->second=="1") keepTempFiles = true;
         else if (optIt->second=="false" || optIt->second=="0") keepTempFiles = false;
         else
            {
             ERRMSG;
             errMsg<<"Invalid value '"<<optIt->second<<"' for option 'keep-temp-files' taken";
             RETERR;
            }
        }

     bool ignoreExitCode = false;
     optIt = optMap.find("ignore-exit-code");
     if (optIt!=optMap.end())
        {
         if (optIt->second=="true" || optIt->second=="1") ignoreExitCode = true;
         else if (optIt->second=="false" || optIt->second=="0") ignoreExitCode = false;
         else
            {
             ERRMSG;
             errMsg<<"Invalid value '"<<optIt->second<<"' for option 'ignore-exit-code' taken";
             RETERR;
            }
        }


/*
     bool bWinNT = false;
     OSVERSIONINFO osvi = { sizeof(osvi) }; 
     //vi.dwOSVersionInfoSize = sizeof vi;
     GetVersionEx(&osvi);
     bWinNT = (osvi.dwPlatformId==VER_PLATFORM_WIN32_NT);
*/
     /*
     char tempPath[MAX_PATH];
     GetTempPath(sizeof(tempPath)/sizeof(tempPath[0]), tempPath);
         ERRMSG;
         errMsg<<"Temp Path: "<<tempPath;
         RETERR;
     */

     DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_TEMPORARY;
//     if (bWinNT && !keepTempFiles) attrs |= FILE_FLAG_DELETE_ON_CLOSE;

     char tempBuf[2*MAX_PATH];

     HANDLE hStdIn = CreateExtFmtTempFile(tempBuf, dwFlagsAndAttributes );
     if (hStdIn==INVALID_HANDLE_VALUE)
        {
         ERRMSG;
         errMsg<<"Failed to create temp file for StdIn";
         RETERR;
        }
     std::string stdinFileName = tempBuf;

     HANDLE hStdOut = CreateExtFmtTempFile(tempBuf, dwFlagsAndAttributes );
     if (hStdOut==INVALID_HANDLE_VALUE)
        {
         ERRMSG;
         errMsg<<"Failed to create temp file for StdOut";
         RETERR;
        }
     std::string stdoutFileName = tempBuf;

     HANDLE hStdErr = CreateExtFmtTempFile(tempBuf, dwFlagsAndAttributes );
     if (hStdErr==INVALID_HANDLE_VALUE)
        {
         ERRMSG;
         errMsg<<"Failed to create temp file for StdErr";
         RETERR;
        }
     std::string stderrFileName = tempBuf;

     std::map<std::string, std::string> cmdLineParsingParams;
     cmdLineParsingParams["__PluginBasePath"]  = pluginPath;
     cmdLineParsingParams["InputFile"]  = stdinFileName;
     cmdLineParsingParams["OutputFile"] = stdoutFileName;
     cmdLineParsingParams["ErrFile"]    = stderrFileName;
     cmdLineParsingParams[""] = "$";

     std::string cmd_parsed, wrongMacro;
     if (!templateParse( cmd, cmdLineParsingParams,
                         cmd_parsed, wrongMacro, false))
        {
         ERRMSG;
         errMsg<<"Command parsing failed: unknown macro name: "<<wrongMacro;
         RETERR;
        }

     cmd = hasSpaces(cmd_parsed) 
         ? (std::string("\"") + cmd_parsed + std::string("\""))
         : cmd_parsed;

         
     std::string cmd_line_parsed;
     if (cmd_line.size() && !templateParse( cmd_line, cmdLineParsingParams,
                                            cmd_line_parsed, wrongMacro, true))
        {
         ERRMSG;
         errMsg<<"Command line parsing failed: unknown macro name: "<<wrongMacro;
         RETERR;
        }

     cmd_line = cmd;
     if (!cmd_line_parsed.empty())
        {
         cmd_line.append(1, ' ');
         cmd_line.append(cmd_line_parsed);
        }

     DWORD written = 0;
     if (!WriteFile( hStdIn, text, (DWORD)nBytes, &written, 0))
        {
         ERRMSG;
         errMsg<<"Failed to write to input file: "<<formatSysErr();
         RETERR;
        }

     LONG moveHigh = 0;
     SetFilePointer( hStdIn, 0, &moveHigh, FILE_BEGIN);


     STARTUPINFO si = { 0 };
     si.cb = sizeof(si);
     si.lpReserved = 0; 
     si.lpDesktop = 0; 
     si.lpTitle = 0; 
     si.dwFlags = STARTF_USESTDHANDLES;
     si.cbReserved2 = 0; 
     si.lpReserved2 = 0; 


     optIt = cmdLineParsingParams.find("InputFile");
     if (optIt!=cmdLineParsingParams.end()) // name not used, use std handle
        {
         si.hStdInput  = hStdIn;
        }
     else
        {
         CloseHandle(hStdIn);
         hStdIn = INVALID_HANDLE_VALUE;
        }

     optIt = cmdLineParsingParams.find("OutputFile");
     if (optIt!=cmdLineParsingParams.end()) // name not used, use std handle
        {
         si.hStdOutput = hStdOut;
        }
     else
        {
         CloseHandle(hStdOut);
         hStdOut = INVALID_HANDLE_VALUE;
        }

     optIt = cmdLineParsingParams.find("ErrFile");
     if (optIt!=cmdLineParsingParams.end()) // name not used, use std handle
        {
         si.hStdError  = hStdErr;
        }
     else
        {
         CloseHandle(hStdErr);
         hStdErr = INVALID_HANDLE_VALUE;
        }


     PROCESS_INFORMATION pi = { 0 };

     BOOL cpRes = CreateProcess( 0, (LPSTR)cmd_line.c_str(), 0, 0, TRUE, /* bInheritHandles */
                                 0, /* dwCreationFlags */
                                 0, /* lpEnvironment */
                                 0, /* lpCurrentDirectory */
                                 &si,
                                 &pi
                               );
     if (!cpRes)
        {
         ERRMSG;
         errMsg<<"Spawn '"<<cmd_line<<"' process failed: "<<formatSysErr();
         RETERR;
        }

     if (WaitForSingleObject( pi.hProcess, 10000 )!=WAIT_OBJECT_0)  // 10 sec wait
        {
         ERRMSG;
         errMsg<<"Wait spawned process failed";
         CloseHandle (pi.hThread);
         CloseHandle (pi.hProcess);
         CloseHandle(hStdIn);
         CloseHandle(hStdOut);
         CloseHandle(hStdErr);
         //if (!bWinNT && !keepTempFiles)
         if (!keepTempFiles)
            {
             DeleteFile(stdinFileName .c_str());
             DeleteFile(stdoutFileName.c_str());
             DeleteFile(stderrFileName.c_str());
            }
         RETERR;
        }

     DWORD exitCode = 0;
     GetExitCodeProcess( pi.hProcess, &exitCode);

     CloseHandle (pi.hThread);
     CloseHandle (pi.hProcess);


     // reading result text
     if (hStdOut!=INVALID_HANDLE_VALUE)
        {
         SetFilePointer( hStdOut, 0, &moveHigh, FILE_BEGIN);
        }
     else
        {
         hStdOut = CreateFile( stdoutFileName.c_str(), 
                                   GENERIC_READ,
                                   0, //FILE_SHARE_READ|FILE_SHARE_WRITE,
                                   0, //&saAttr, 
                                   OPEN_EXISTING,
                                   dwFlagsAndAttributes,
                                   0);
        
        }

     std::string resTextStr;
     char readBuf[4096];
     DWORD readed;
     while(ReadFile(hStdOut, readBuf, sizeof(readBuf), &readed, 0) && readed>0)
        {
         resTextStr.append(readBuf, readed);
        }

     if (exitCode && !ignoreExitCode)
        {
         // reading error mesages
         if (hStdErr!=INVALID_HANDLE_VALUE)
            {
             SetFilePointer( hStdErr, 0, &moveHigh, FILE_BEGIN);
            }
         else
            {
             hStdErr = CreateFile( stderrFileName.c_str(), 
                                       GENERIC_READ,
                                       0, //FILE_SHARE_READ|FILE_SHARE_WRITE,
                                       0, //&saAttr, 
                                       OPEN_EXISTING,
                                       dwFlagsAndAttributes,
                                       0);
            
            }
         std::string resErrStr;
         while(ReadFile(hStdErr, readBuf, sizeof(readBuf), &readed, 0) && readed>0)
            {
             resErrStr.append(readBuf, readed);
            }
         CloseHandle(hStdIn);
         CloseHandle(hStdOut);
         CloseHandle(hStdErr);
         //if (!bWinNT && !keepTempFiles)
         if (!keepTempFiles)
            {
             DeleteFile(stdinFileName .c_str());
             DeleteFile(stdoutFileName.c_str());
             DeleteFile(stderrFileName.c_str());
            }
         ERRMSG;
         errMsg<<""<<cmd_line<<"\nSpawn process failed, exit code "<<exitCode<<"\nInfo: "<<resErrStr;
         RETERR;
        }

     CloseHandle(hStdIn);
     CloseHandle(hStdOut);
     CloseHandle(hStdErr);
     
     //if (!bWinNT && !keepTempFiles)
     if (!keepTempFiles)
        {
         DeleteFile(stdinFileName .c_str());
         DeleteFile(stdoutFileName.c_str());
         DeleteFile(stderrFileName.c_str());
        }

     // ERRMSG;
     // errMsg<<"StdIn: "<<stdinFileName<<"\nStdOut: "<<stdoutFileName<<"\nStdErr: "<<stderrFileName<<"\nCommand line: "<<cmd_line;
     // RETERR;

     // return formatted text

     LPSTR resTextBuf = (LPSTR)allocator->pfnAllocate(resTextStr.size());
     *resText   = resTextBuf;
     *nResBytes = resTextStr.size();

     #if defined(_MSC_VER) && _MSC_VER>=1400
     #pragma warning( push )
     #pragma warning( disable : 4996 )
     #endif

     resTextStr.copy(resTextBuf, resTextStr.size());

     #if defined(_MSC_VER) && _MSC_VER>=1400
     #pragma warning( pop )
     #endif

     return resTextBuf;
    }

//-----------------------------------------------------------------------------



