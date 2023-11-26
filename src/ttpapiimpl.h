/*%<
*/
#ifndef TTPAPIIMPL_H
#define TTPAPIIMPL_H

#include "ttpapi.h"
#include "ttpver.h"
#include <string.h>

#include <filename.h>

#ifndef TTPCONF_H
    #include "ttpconf.h"
#endif

//-----------------------------------------------------------------------------
/*%>
%block CVersionInfoImpl */
class CVersionInfoImpl : public IPluginVersionInfo
{
        LONG volatile usageCount;

    public:

        CVersionInfoImpl() : usageCount(0) {}

        INLINE_IMPLEMENT_TTPAPI_ADDREF(usageCount)
        INLINE_IMPLEMENT_TTPAPI_RELEASE(usageCount)
        INLINE_IMPLEMENT_TTPAPI_DESTROY()

        BEGIN_INTERFACE_MAP()
            IMPLEMENT_INTERFACE(IPluginVersionInfo, IID_PLUGINVERSIONINFO)
        END_INTERFACE_MAP()

        TTPAPISTDMETHOD(int, getInternalName)(char *buf, int bufSize)
           {
            int requiredLen = (int)strlen(TTP_INTERNAL_NAME)+1;
            if (!buf) return requiredLen;
            if (bufSize<(requiredLen)) return 0;
            strcpy(buf, TTP_INTERNAL_NAME);
            return requiredLen;
           }

        TTPAPISTDMETHOD(unsigned, getVersion)()
           {
            return MAKETTPVERSION(TTPVER_MAJOR, TTPVER_MINOR);
           }

        TTPAPISTDMETHOD(int, getValueString)(const char *valueName, char *buf, int bufSize)
           {
            return -1;
           }
};
/*%endblock
%< */

//-----------------------------------------------------------------------------
class CITtpConfigImpl : public ITtpConfig
{
        LONG volatile      usageCount;
        TextTP::TTPConfig  config;

    public:

        CITtpConfigImpl(const TextTP::TTPConfig &cfg) : usageCount(0), config(cfg) {}

        INLINE_IMPLEMENT_TTPAPI_ADDREF(usageCount)
        INLINE_IMPLEMENT_TTPAPI_RELEASE(usageCount)
        INLINE_IMPLEMENT_TTPAPI_DESTROY()

        BEGIN_INTERFACE_MAP()
            IMPLEMENT_INTERFACE(ITtpConfig, IID_TTPCONFIG)
        END_INTERFACE_MAP()

        TTPAPISTDMETHOD(void, updateForFile)(const char *editorFileName)
           {
            if (!editorFileName) return;
            readCustomConfig(editorFileName, Config);
           }

        TTPAPISTDMETHOD(void, updateForPath)(const char *path)
           {
            if (!path) return;
            readCustomConfig(filename::appendPath(path, "tmp.tmp" ), Config);
           }

        TTPAPISTDMETHOD(int, getFileType)(const char *fileName, char *typeNameBuf, int typeNameBufSize)
           {
            if (!fileName) return -1;
            std::string fileType;
            if (!lookupForFileType(config, fileName, fileType, Info.StdFn()))
               return 0;

            if (!typeNameBuf)
                return (int)fileType.size()+1;

            if (typeNameBufSize<((int)fileType.size()+1))
               return -1;

            strcpy(typeNameBuf, fileType.c_str());
            return (int)fileType.size()+1;
           }
};

//-----------------------------------------------------------------------------
class CITtpApiImpl : public ITtpApi
{
        LONG volatile usageCount;

    public:

        CITtpApiImpl() : usageCount(0) {}

        INLINE_IMPLEMENT_TTPAPI_ADDREF(usageCount)
        INLINE_IMPLEMENT_TTPAPI_RELEASE(usageCount)
        INLINE_IMPLEMENT_TTPAPI_DESTROY()

        BEGIN_INTERFACE_MAP()
            IMPLEMENT_INTERFACE(ITtpApi, IID_TTPAPI)
        END_INTERFACE_MAP()

        TTPAPISTDMETHOD(ITtpConfig*, getConfig)()
           {
            return static_cast<ITtpConfig*>(new CITtpConfigImpl(Config));
           }

        TTPAPISTDMETHOD(BOOL, matchMaskA)(const char *name, const char *mask)
           {
            if (!name || !mask) return FALSE;
            return filename::matchMaskI(name, mask) ? TRUE : FALSE;
           }
/*
        TTPAPISTDMETHOD(BOOL, matchMaskW)(const wchar_t *name, const wchar_t *mask)
           {
            return 0;
           }
*/
};





#endif /* TTPAPIIMPL_H */
/*%> */

