/*%< (c)2005-2007 Alex Martynov, amart@mail.ru
 * This source file is a part of "Text Templates" - a plugin for file manager FAR.
 * You can use, modify, redistribute this source file without any limitations.
 * This source file is distributed "as is". No waranty of any kind is expressed or 
 * implied. Use it at your own risk.
 */

#ifndef TTPAPI_H
#define TTPAPI_H

#if !defined(_WINDOWS_)
    #include <windows.h>
#endif

#include <Tlhelp32.h>
#pragma comment( lib, "advapi32" )

#ifndef interface
    #define interface struct
#endif

#if defined(_MSC_VER) || defined(__BORLANDC__)
    #define TTPAPISTDCALLTYPE        __stdcall
    #define TTPAPIVARARGCALLTYPE     __cdecl
#else /* ! (_MSC_VER || __BORLANDC__) */
    #define TTPAPISTDCALLTYPE
    #define TTPAPIVARARGCALLTYPE
    #error TTPAPISTDCALL and TTPAPIVARARGCALL not defined for this platform.
#endif

#define TTPAPIPURE = 0
#define TTPAPINOTPURE

#define TTPAPISTDMETHOD(retType, name)    virtual retType TTPAPISTDCALLTYPE    name
#define TTPAPIVARARGMETHOD(retType, name) virtual retType TTPAPIVARARGCALLTYPE name


/*%>
%block BLOCK_CreateObjectProc */
typedef 
void* (TTPAPISTDCALLTYPE *ttpapiCreateObjectProc)( char const*, char const*);
extern "C"
void* TTPAPISTDCALLTYPE ttpapiCreateObject( char const* objectId
                                          , char const* interfaceId
                                          );

#define TTPAPICREATEOBJECTPROCNAME     "ttpapiCreateObject"
/*%endblock
%< */



/*%>
%block BLOCK_IUnknown */
interface ITtpApiUnknown
{
   TTPAPISTDMETHOD(unsigned, addRef)()  TTPAPIPURE ;
   TTPAPISTDMETHOD(unsigned, release)() TTPAPIPURE ;
   TTPAPISTDMETHOD(void*, queryInterface)(char const* interfaceId) TTPAPIPURE;
};

#define IID_TTPAPIUNKNOWN     "ITtpApiUnknown"
/*%endblock
%< */


/*%>
%block BLOCK_IPluginVersionInfo */
interface IPluginVersionInfo : public ITtpApiUnknown
{
    TTPAPISTDMETHOD(int, getInternalName)(char *buf, int bufSize) TTPAPIPURE ;
    TTPAPISTDMETHOD(unsigned, getVersion)() TTPAPIPURE ;
    TTPAPISTDMETHOD(int, getValueString)( const char *valueName, 
                                          char *buf, int bufSize) TTPAPIPURE ;
};

#define IID_PLUGINVERSIONINFO     "IPluginVersionInfo"
#define CLSID_PLUGINVERSIONINFO   "PluginVersionInfo"
/*%endblock
%< */

#define MAKETTPVERSION(major,minor) ((major)<<16) | (minor)

#define TTP_INTERNAL_NAME         "TextTemplates"


/*%>
%block BLOCK_ttpapi_findPlugin */
namespace ttpapi
{

template <typename THandler>
void enumTtpapiModules(THandler handler)
   {
    HANDLE h = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);
    if (h==INVALID_HANDLE_VALUE)
       return;

    MODULEENTRY32 me = { sizeof(me) };
    for(BOOL bRes = ::Module32First( h, &me ); bRes ; bRes = ::Module32Next( h, &me ) )
       {
        ttpapiCreateObjectProc proc = 
             (ttpapiCreateObjectProc)::GetProcAddress( 
                                          me.hModule, 
                                          TTPAPICREATEOBJECTPROCNAME
                                                     );
        if (!proc) continue;

        IPluginVersionInfo *pIPluginVersionInfo = 
             (IPluginVersionInfo*)proc( CLSID_PLUGINVERSIONINFO, 
                                        IID_PLUGINVERSIONINFO
                                      );
        if (!pIPluginVersionInfo) continue;

        bool hdlres = handler(me.hModule, me.szModule, 
                              me.szExePath, proc, pIPluginVersionInfo);
        pIPluginVersionInfo->release();
        if (!hdlres)
            break;
       }
    ::CloseHandle(h);
   }

struct CTTPFinder
{
    ttpapiCreateObjectProc *pProc;
    const char             *pluginName;
    unsigned               minVersion;

    CTTPFinder( ttpapiCreateObjectProc *pP, 
                const char *pN = TTP_INTERNAL_NAME, 
                unsigned mv = 0) 
       : pProc(pP)
       , pluginName(pN)
       , minVersion(mv)
       {}

    CTTPFinder(const CTTPFinder &finder) 
       : pProc(finder.pProc)
       , pluginName(finder.pluginName)
       , minVersion(finder.minVersion)
       {}

    bool operator()(HMODULE hMod, 
                    const char* modName, 
                    const char* modExe, 
                    ttpapiCreateObjectProc createProc, 
                    IPluginVersionInfo *pIPluginVersionInfo)
       {
        char internalNameBuf[256];
        int res = pIPluginVersionInfo->getInternalName(
                                          internalNameBuf, 
                                          (int)sizeof(internalNameBuf)
                                                      );
        if (res<=0) internalNameBuf[0] = 0;

        if (strcmp(internalNameBuf, pluginName)
            || (minVersion && minVersion>pIPluginVersionInfo->getVersion()))
           return true; // continue search

        *pProc = createProc;
        return false; // stop 
       }
};

inline
ttpapiCreateObjectProc findPlugin( const char *pluginName = TTP_INTERNAL_NAME, 
                                   unsigned minVersion = 0)
   {
    ttpapiCreateObjectProc procRes = 0;
    enumTtpapiModules(CTTPFinder(&procRes, pluginName, minVersion) );
    return procRes;
   }

}; // namespace ttpapi
/*%endblock
%< */


/*%>
%block BLOCK_ITtpConfig */
interface ITtpConfig : public ITtpApiUnknown
{
    TTPAPISTDMETHOD(void, updateForFile)( 
                                const char *editorFileName
                                        ) TTPAPIPURE;
    TTPAPISTDMETHOD(void, updateForPath)(const char *path) TTPAPIPURE;
    TTPAPISTDMETHOD(int, getFileType)(const char *fileName, 
                                      char *typeNameBuf, 
                                      int typeNameBufSize) TTPAPIPURE;
};

#define IID_TTPCONFIG     "ITtpConfig"
/*%endblock
%< */


/*%>
%block BLOCK_ITtpApi */
interface ITtpApi : public ITtpApiUnknown
{
    TTPAPISTDMETHOD(ITtpConfig*, getConfig)() TTPAPIPURE;
    TTPAPISTDMETHOD(BOOL, matchMaskA)( const char *name, 
                                       const char *mask) TTPAPIPURE ;
    //TTPAPISTDMETHOD(BOOL, matchMaskW)(const wchar_t *name, const wchar_t *mask) TTPAPIPURE ;
};

#define IID_TTPAPI     "ITtpApi"
#define CLSID_TTP      TTP_INTERNAL_NAME
/*%endblock
%< */


/*%>
%block BLOCK_ttpapi_getFileType */
namespace ttpapi
{

inline
bool getFileType( ttpapiCreateObjectProc pluginApiProc, 
                  const std::string &fileName, 
                  std::string &fileType)
   {
    if (!pluginApiProc || fileName.empty()) return false;

    ITtpApi *pITtpApi = (ITtpApi*)pluginApiProc(CLSID_TTP, IID_TTPAPI);
    if (!pITtpApi) return false;

    ITtpConfig *pITtpConfig = pITtpApi->getConfig();
    if (!pITtpConfig) return false;

    pITtpConfig->updateForFile(fileName.c_str());

    char buf[256];
    int res = pITtpConfig->getFileType( fileName.c_str(), 
                                        buf, (int)sizeof(buf));
    pITtpConfig->release();
    pITtpApi->release();

    if (res<0) // not enough buffer or other error occurs
       return false;

    if (res==0) // file type not found
       return false;

    fileType.assign(buf);
    return true;   
   }
}; // namespace ttpapi
/*%endblock
%< */



#define INLINE_IMPLEMENT_TTPAPI_ADDREF(objUsageCountVar)  \
unsigned                                                  \
TTPAPISTDCALLTYPE addRef()                                \
   {                                                      \
    InterlockedIncrement(&objUsageCountVar);              \
    return objUsageCountVar;                              \
   }

#define INLINE_IMPLEMENT_TTPAPI_RELEASE(objUsageCountVar) \
unsigned                                                  \
TTPAPISTDCALLTYPE release()                               \
   {                                                      \
    if (InterlockedDecrement(&objUsageCountVar)!=0)       \
       {                                                  \
        return objUsageCountVar;                          \
       }                                                  \
    destroy();                                            \
    return 0;                                             \
   }

#define IMPLEMENT_TTPAPI_ADDREF(className, objUsageCountVar)  \
unsigned                                                      \
TTPAPISTDCALLTYPE className::addRef()                         \
   {                                                          \
    InterlockedIncrement(&objUsageCountVar);                  \
    return objUsageCountVar;                              \
   }

#define IMPLEMENT_TTPAPI_RELEASE(className, objUsageCountVar) \
unsigned                                                      \
TTPAPISTDCALLTYPE className::release()                        \
   {                                                          \
    if (InterlockedDecrement(&objUsageCountVar)!=0)           \
       {                                                      \
        return objUsageCountVar;                              \
       }                                                      \
    destroy();                                                \
    return 0;                                                 \
   }

#define INLINE_IMPLEMENT_TTPAPI_DESTROY()                 \
void destroy()                                            \
   {                                                      \
    delete this;                                          \
   }

#define IMPLEMENT_TTPAPI_DESTROY(className)               \
void className::destroy()                                 \
   {                                                      \
    delete this;                                          \
   }

#define BEGIN_INTERFACE_MAP()                                     \
TTPAPISTDMETHOD(void*, queryInterface)(char const* interfaceId)   \
   {                                                              \
    if (!strcmp(interfaceId, IID_TTPAPIUNKNOWN))                  \
       {                                                          \
        addRef();                                                 \
        return static_cast<ITtpApiUnknown*>(this);                \
       }

#define BEGIN_INTERFACE_MAP2(interfaceClassName)                  \
TTPAPISTDMETHOD(void*, queryInterface)(char const* interfaceId)   \
   {                                                              \
    if (!strcmp(interfaceId, IID_TTPAPIUNKNOWN))                  \
       {                                                          \
        addRef();                                                 \
        return static_cast<ITtpApiUnknown*>(static_cast<interfaceClassName*>(this));\
       }

#define END_INTERFACE_MAP()                                       \
    return 0;                                                     \
   }

#define IMPLEMENT_INTERFACE(interfaceName, idStr)                 \
     else if (!strcmp(interfaceId, idStr))                        \
        {                                                         \
         addRef();                                                \
         return static_cast<interfaceName*>(this);                \
        }

#endif /* TTPAPI_H */

/*%> */
