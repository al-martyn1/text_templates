/*
   Copyright (c) 2007-2009 Alex Martynov, amart at mail.ru
   This code can be used without any limitations.
   This code provided as is, use it on your own risk.
*/

#ifndef MARTY_LIBAPI_H
#define MARTY_LIBAPI_H

// see boost/tools/jam/src/boehm_gc/threadlibs.c

#ifdef WIN32
    #if !defined(_WINDOWS_)
        #include <windows.h>
    #endif
#else
    #include <dlfcn.h>
    #include <unistd.h>
    #include <errno.h>
#endif

#ifndef MARTY_BASICTCHARDEFS_H
    #include <marty/basictchardefs.h>
#endif

#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

#ifndef MARTY_FILESYS_H
    #include <marty/filesys.h>
#endif

#ifndef MARTY_FILENAME_H
    #include <marty/filename.h>
#endif

#ifdef WIN32
    #include <marty/winapi.h>
#endif

#ifndef MARTY_MHDLDEF_H
    #include <marty/mhdldef.h>
#endif

#if !defined(_STDEXCEPT_) && !defined(_STLP_STDEXCEPT) && !defined(__STD_STDEXCEPT) && !defined(_CPP_STDEXCEPT) && !defined(_GLIBCXX_STDEXCEPT)
    #include <stdexcept>
#endif




#ifndef MARTY_STRING
    #define MARTY_STRING(TC) ::std::basic_string<TC>
#endif

#ifndef MARTY_TCSTRING
    #define MARTY_TCSTRING   MARTY_STRING(TC)
#endif

#ifdef USE_MARTY_NAMESPACE
    #ifndef LIBAPI_IN_MARTY_NAMESPACE
        #define LIBAPI_IN_MARTY_NAMESPACE
    #endif
#endif

#ifdef LIBAPI_IN_MARTY_NAMESPACE
    #ifndef MARTY_NS
        #define BEGIN_MARTY_NS          namespace marty {
        #define END_MARTY_NS            };
        #define MARTY_NS                ::marty::
     #endif
#else
    #ifndef MARTY_NS
        #define BEGIN_MARTY_NS 
        #define END_MARTY_NS 
        #define MARTY_NS                ::
     #endif
#endif

#define MARTY_LIBAPI               MARTY_NS libapi
#define MARTY_LIBAPI_NS            MARTY_NS libapi::
#define BEGIN_MARTY_LIBAPI_NS      BEGIN_MARTY_NS namespace libapi {
#define END_MARTY_LIBAPI_NS        END_MARTY_NS   };





BEGIN_MARTY_LIBAPI_NS

#ifdef LIBAPI_IN_MARTY_NAMESPACE
using ::marty::filename::tstring;
#else
using ::filename::tstring;
#endif


//-----------------------------------------------------------------------------
// return module handle or zero if error
template < typename CharType
         , typename Traits
         , typename Allocator
         >
ABSTRACT_MODULE_HANDLE
loadModule(const ::std::basic_string<CharType, Traits, Allocator> &modname)
   {
    #ifdef WIN32
    UINT prevErrMode = ::SetErrorMode(SEM_NOOPENFILEERRORBOX|SEM_FAILCRITICALERRORS);
    ABSTRACT_MODULE_HANDLE res = ::LoadLibrary( MARTY_FILESYSTEM_NS osFilename(modname).c_str() );
    DWORD lastErr = ::GetLastError();
    ::SetErrorMode(prevErrMode);
    ::SetLastError(lastErr);
    return res;
    #else
    //dlerror(); // clear the error
    //const char *err = 0;
    return ::dlopen(MARTY_FILESYSTEM_NS osFilename(modname).c_str(),  RTLD_NOW|RTLD_LOCAL);
    // if (!dlopenRet) err = dlerror(); // get the human printable error message
    #endif
   }

//-----------------------------------------------------------------------------
// return error code or zero (under linux/posix error code undefined/unknown, but not zero)
inline
int freeModule(ABSTRACT_MODULE_HANDLE handle)
   {
    #ifdef WIN32
    // If the FreeLibrary function succeeds, the return value is nonzero.
    if (!::FreeLibrary(handle)) return ::GetLastError();
    return 0;
    #else
    // The function dlclose() returns 0 on success, and non-zero on error.
    return ::dlclose(handle);
    #endif
   }

//-----------------------------------------------------------------------------
//tstring getModuleFileName(ABSTRACT_MODULE_HANDLE handle);
//-----------------------------------------------------------------------------
// return 0 on siccess, error code if error
template < typename CharType
         , typename Traits
         , typename Allocator
         >
int getModuleFileName(ABSTRACT_MODULE_HANDLE handle, ::std::basic_string<CharType, Traits, Allocator> &retFileName)
   {
    #ifdef WIN32
    ::std::basic_string<CharType, Traits, Allocator> res;
    MARTY_WINAPI_NS getModuleFileName(handle, res);
    if (res.empty()) return GetLastError(); // keep retFileName untouched
    retFileName.swap(res);
    return 0;
    #else
    // ldd -v elf
    // nm elf
    // nm elf | grep " T "
    // are good helpers

    bool needFree = false;
    void* pAddr = 0;
    if (!handle)
       {
        //ABSTRACT_MODULE_HANDLE 
        int dlflags = RTLD_LAZY; // 0
        #ifdef RTLD_NOLOAD
        dlflags |= RTLD_NOLOAD;
        #endif
        handle = ::dlopen( 0,  dlflags); // get main module handle
        //pAddr = dlsym(handle, "main");
        needFree = true;
        /*
        printf("CP 1\n");
        if (!handle)
           printf("dlopen(0) failed\n");
        else
           printf("dlopen(0) OK\n");
        */
       }
    else
       {
        // see boost/tools/jam/src/boehm_gc/dyn_load.c
        //pAddr = dlsym(handle, "_DYNAMIC");
       }

    pAddr = dlsym(handle, "_init"); // this works for test module
    if (!pAddr)
       {
        //printf("CP 2\n");
        pAddr = dlsym(handle, "_fini");
       }

    if (!pAddr && needFree)  // needFree signals that the name of main module requested
       {
        char buf[4096];
        int len = readlink("/proc/self/exe", buf, sizeof(buf)/sizeof(buf[0])-1);
        if (len>=0)
           {
            buf[len] = 0;
            ::dlclose(handle);
            //::std::basic_string<CharType, Traits, Allocator> res = MARTY_FILESYSTEM_NS fromOsFilename(::std::string(buf));
            ::std::basic_string<CharType, Traits, Allocator> res;
            MARTY_FILESYSTEM_NS fromOsFilename2(res, ::std::string(buf));
            if (res.empty()) 
                return ENOENT;
            retFileName.swap(res);
            return 0;
           }

        //printf("CP 3\n");
        // none of below calls works for main module, why?
        pAddr = dlsym(handle, "main");
        if (!pAddr)
           pAddr = dlsym(handle, "_main");
        if (!pAddr)
           pAddr = dlsym(handle, "start");
        if (!pAddr)
           pAddr = dlsym(handle, "_start");

        /*
        // getModuleFileName modified to template fn, code below  not work in new version
        if (!pAddr) // none of above calls are success for main module (handle=0)
           pAddr = (void*)getModuleFileName; // this is a work-around
        */
       }

    if (!pAddr)
       {
        if (needFree)
           ::dlclose(handle);
        //printf("CP 4\n");
        return ENOENT; // invalid handle or other error
       }

    Dl_info info;
    int dladdRes = dladdr(pAddr, &info);

    if (needFree)
       ::dlclose(handle);

    if (!dladdRes || !info.dli_fname) 
       {
        //printf("CP 5\n");
        return ENOENT;
       }

    //::std::basic_string<CharType, Traits, Allocator> res = MARTY_FILESYSTEM_NS fromOsFilename(info.dli_fname);
    ::std::basic_string<CharType, Traits, Allocator> res;
    MARTY_FILESYSTEM_NS fromOsFilename2(res, info.dli_fname);
    if (res.empty()) 
       {
        //printf("CP 6\n");
        return ENOENT;
       }
    retFileName.swap(res);
    //printf("CP 7\n");
    return 0;
    #endif
   }

//-----------------------------------------------------------------------------
/*
inline
tstring getModuleFileName(ABSTRACT_MODULE_HANDLE handle)
   {
    tstring res;
    if (getModuleFileName(handle, res)) return tstring();
    return res;
   }
*/
//-----------------------------------------------------------------------------
typedef void (*function_pointer_t)();

/*
template<typename TypeTo, typename TypeFrom>
TypeTo union_cast(TypeFrom f)
{
    union dummy{ TypeFrom from; TypeTo to; }  u;
    u.from = f;
    return u.to;
}
*/


/* dont work in GCC 3.4 and older - template with only return template value couses duplicated synmbol */
/* inline
 * function_pointer_t rawGetProcAddress(ABSTRACT_MODULE_HANDLE handle, const char *procName)
 *    {
 *     #ifdef WIN32
 *     //return reinterpret_cast<void*>(::GetProcAddress(handle, procName));
 *     //return union_cast<function_pointer_t>(::GetProcAddress(handle, procName));
 *     return (function_pointer_t)::GetProcAddress(handle, procName);
 *     #else
 *     //return ::dlsym(handle, procName);
 *     //return union_cast<function_pointer_t>(::dlsym(handle, procName)); // union cast generates some other error messages
 *     return (function_pointer_t)::dlsym(handle, procName);
 *     #endif
 *    }
 *  
 *  
 * template<typename ProcTypeT>
 * ProcTypeT getProcAddress(ABSTRACT_MODULE_HANDLE handle, const char *procName)
 *    {
 *     return reinterpret_cast<ProcTypeT>(rawGetProcAddress(handle, procName));
 *    }
 */

//-----------------------------------------------------------------------------
// this works anywhere, but need explicit casting 
// use reinterpret_cast<fn_type>(getProcAddress(handle, procName));
inline
function_pointer_t getProcAddress(ABSTRACT_MODULE_HANDLE handle, const char *procName)
   {
    #ifdef WIN32
    //return reinterpret_cast<void*>(::GetProcAddress(handle, procName));
    //return union_cast<function_pointer_t>(::GetProcAddress(handle, procName));
    return (function_pointer_t)::GetProcAddress(handle, procName);
    #else
    //return ::dlsym(handle, procName);
    //return union_cast<function_pointer_t>(::dlsym(handle, procName)); // union cast generates some other error messages
    return (function_pointer_t)::dlsym(handle, procName);
    #endif
   }



//#ifdef WIN32

#ifdef WIN32
inline
std::string formatSystemError(DWORD err)
   {
    char buf[1024];
    return std::string(MARTY_WINAPI_NS formatMessageA(err, buf, sizeof(buf)));
   }
#endif


class libapi_error : public std::runtime_error
{
        unsigned m_code;
    public:         
        #ifdef WIN32
        libapi_error(DWORD err) : std::runtime_error(formatSystemError(err).c_str()), m_code(err) {}
        #endif
        libapi_error(char *errMsg) : std::runtime_error(errMsg), m_code(EINVAL) {}
        libapi_error(unsigned c, char *errMsg) : std::runtime_error(errMsg), m_code(c) {}

        // ~libapi_error() {}
        libapi_error(const libapi_error& le) : std::runtime_error(le), m_code(le.m_code) {}
        //libapi_error(const libapi_error& le) : std::runtime_error(le), code(le.code) {}

        unsigned code() const { return m_code; }
};                              


class CModuleHandle
{
        ABSTRACT_MODULE_HANDLE     handle;
        bool                       bFree;
        CModuleHandle& operator=(const CModuleHandle &mh) { return *this; }
        CModuleHandle(const CModuleHandle &mh) : handle(0), bFree(false) {}

    public:
        CModuleHandle() : handle(0), bFree(false) {}
        CModuleHandle(ABSTRACT_MODULE_HANDLE h, bool bGetOwnersheep = false)
           : handle(h), bFree(bGetOwnersheep) {}


    private:
        void modfree()
           {
            if (handle && bFree) freeModule(handle); 
            handle = 0; bFree = false;   
           }

    public:

        ~CModuleHandle() { modfree(); }
        ABSTRACT_MODULE_HANDLE get() const { return handle; }

        // return stored handle and replace it with 0 value
        // don't free prev module, this takes handle ownersheep to another routines/objects
        ABSTRACT_MODULE_HANDLE release() 
           {
            ABSTRACT_MODULE_HANDLE res = handle;
            handle = 0;
            bFree = false;
            return res;
           }

        // replace stored handle with newly loaded (with auto free mode)
        // free previously stored handle if needed
        // return newly loaded module handle
        // throws an exception if load error occurs
        ABSTRACT_MODULE_HANDLE load(const TCHAR* modName)
           {
            modfree();

            if (!modName) throw libapi_error("Error: CModuleHandle::load(modName==0)");

            #ifdef WIN32
            handle = loadModule(tstring(modName));
            if (!handle)
               throw libapi_error(::GetLastError());
            #else
            dlerror(); // clear the error
            handle = loadModule(tstring(modName));
            if (!handle)
               {
                char *err = dlerror();
                if (!err) err = "loadModule failed, but no any error information given";
                //#ifdef ENOFILE
                //    throw libapi_error(ENOFILE, err);
                //#else
                    throw libapi_error(ENOENT, err);
                //#endif
               }
            #endif
            bFree = true;
            return handle;
           }

        CModuleHandle(const TCHAR* modName)
           : handle(0), bFree(true)
           {
            load(modName);
           }

        function_pointer_t getProcAddress(const char *procName) const
           {
            return MARTY_LIBAPI_NS getProcAddress(handle, procName);
           }

        tstring getFileName()
           {
            tstring name;
            int res = getModuleFileName(handle, name);
            if (res)
               {
                #ifdef WIN32
                throw libapi_error(res);
                #else
                throw libapi_error(res, "function not supported by underlaying API");
                #endif
               }
            return name;
           }
};






END_MARTY_LIBAPI_NS



#endif /* MARTY_LIBAPI_H */

