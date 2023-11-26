#ifndef GUID_H
#define GUID_H

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


char* format_guid_part(char *buf, GUID *guid, int part);
char* format_guid_olecreate(char *buf, GUID *guid);
char* format_guid_src(char *buf, GUID *guid);
char* format_guid_registry(char *buf, GUID *guid);
char* format_guid_clang(char *buf, GUID *guid);


#endif /* GUID_H */