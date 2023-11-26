
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

#if !defined(_INC_STDIO) && !defined(_STDIO_H_) && !defined(_STDIO_H)
    #include <stdio.h>
#endif

#include "guid.h"


//-----------------------------------------------------------------------------
char* format_guid_part(char *buf, GUID *guid, int part)
{
    static const char *fmt[] = {
                                "0x%08x",
                                "0x%04x",
                                "0x%04x",
                                "0x%02x",
                                "0x%02x",
                                "0x%02x",
                                "0x%02x",
                                "0x%02x",
                                "0x%02x",
                                "0x%02x",
                                "0x%02x"
                               };
    buf[0] = 0;
    switch(part)
        {
         case 0:   sprintf(buf, fmt[part], guid->Data1   ); return buf;
         case 1:   sprintf(buf, fmt[part], guid->Data2   ); return buf;
         case 2:   sprintf(buf, fmt[part], guid->Data3   ); return buf;
         case 3:   sprintf(buf, fmt[part], guid->Data4[0]); return buf;
         case 4:   sprintf(buf, fmt[part], guid->Data4[1]); return buf;
         case 5:   sprintf(buf, fmt[part], guid->Data4[2]); return buf;
         case 6:   sprintf(buf, fmt[part], guid->Data4[3]); return buf;
         case 7:   sprintf(buf, fmt[part], guid->Data4[4]); return buf;
         case 8:   sprintf(buf, fmt[part], guid->Data4[5]); return buf;
         case 9:   sprintf(buf, fmt[part], guid->Data4[6]); return buf;
         case 10:  sprintf(buf, fmt[part], guid->Data4[7]); return buf;
        }
    return buf;
}

//-----------------------------------------------------------------------------
char* format_guid_olecreate(char *buf, GUID *guid)
{
 sprintf(buf, "0x%08x, 0x%04x, 0x%04x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x", 
              guid->Data1, guid->Data2, guid->Data3,
              guid->Data4[0], guid->Data4[1], guid->Data4[2], guid->Data4[3],
              guid->Data4[4], guid->Data4[5], guid->Data4[6], guid->Data4[7]
        );
 return buf;
}

//-----------------------------------------------------------------------------
char* format_guid_src(char *buf, GUID *guid)
{
 sprintf(buf, "{ 0x%08x, 0x%04x, 0x%04x, { 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x } }", 
              guid->Data1, guid->Data2, guid->Data3,
              guid->Data4[0], guid->Data4[1], guid->Data4[2], guid->Data4[3],
              guid->Data4[4], guid->Data4[5], guid->Data4[6], guid->Data4[7]
        );
 return buf;
}

//-----------------------------------------------------------------------------
char* format_guid_registry(char *buf, GUID *guid)
{
 sprintf(buf, "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X", 
              guid->Data1, guid->Data2, guid->Data3,
              guid->Data4[0], guid->Data4[1], guid->Data4[2], guid->Data4[3],
              guid->Data4[4], guid->Data4[5], guid->Data4[6], guid->Data4[7]
        );
 return buf;
}

//-----------------------------------------------------------------------------
char* format_guid_clang(char *buf, GUID *guid)
{
 sprintf(buf, "%08X_%04X_%04X_%02X%02X_%02X%02X%02X%02X%02X%02X",
              guid->Data1, guid->Data2, guid->Data3,
              guid->Data4[0], guid->Data4[1], guid->Data4[2], guid->Data4[3],
              guid->Data4[4], guid->Data4[5], guid->Data4[6], guid->Data4[7]
        );
 return buf;
}
