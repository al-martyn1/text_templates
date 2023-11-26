/*
   Copyright (c) 2007-2009 Alex Martynov, amart at mail.ru
   This code can be used without any limitations.
   This code provided as is, use it on your own risk.
*/

#ifndef MARTY_TMAIN_H
    #include <marty/tmain.h>
#endif


#ifndef WMAIN_SUPPORTED_NATIVE

// #ifndef MARTY_FILESYS_H
//     #include <marty/filesys.h>
// #endif

#ifdef _UNICODE

#if !defined(_INC_MALLOC) && !defined(_MALLOC_H_) && !defined(_MALLOC_H)
    #include <malloc.h>
#endif

static
wchar_t* convertArgvArgToWide(const char *str, wchar_t *wstr)
   {
    wchar_t* res = wstr;
    if (!res || !str) return res;
    // now supported only plain conversion, no localizable encoding/decoding support
    while(*str)
       {
        *wstr++ = (wchar_t)(unsigned char)(*str++);
       }
    return res;
   }

static
unsigned convertArgvArgToWideAuxStrlen(const char *str)
   {
    unsigned i = 0;
    while(str && *str) { ++str; ++i; }
    return i;
   }

int main(int argc, char* argv[])
   {
    wchar_t **wargv[argc] = (wchar_t**)_alloca(sizeof(wchar_t*)*argc);
    for(int idx = 0; idx<argc; ++idx)
       {
        wargv[idx] = (wchar_t*)_alloca(sizeof(wchar_t)*(convertArgvArgToWideAuxStrlen(argv[idx])+1));
        convertArgvArgToWide(argv[idx], wargv[idx]);
       }
    return wmain(argc, wargv);
   }


#endif /* _UNICODE */

#endif
