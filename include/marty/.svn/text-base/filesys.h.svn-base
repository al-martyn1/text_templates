/*
   Copyright (c) 2007-2009 Alex Martynov, amart at mail.ru
   This code can be used without any limitations.
   This code provided as is, use it on your own risk.
*/

#ifndef MARTY_FILESYS_H
#define MARTY_FILESYS_H

#ifndef MARTY_BASICTCHARDEFS_H
    #include <marty/basictchardefs.h>
#endif


//-----------------------------------------------------------------------------
/* std/stl headers */

#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

#if !defined(_VECTOR_) && !defined(_STLP_VECTOR) && !defined(__STD_VECTOR__) && !defined(_CPP_VECTOR) && !defined(_GLIBCXX_VECTOR)
    #include <vector>
#endif

#if !defined(_ALGORITHM_) && !defined(_STLP_ALGORITHM) && !defined(__STD_ALGORITHM) && !defined(_CPP_ALGORITHM) && !defined(_GLIBCXX_ALGORITHM)
    #include <algorithm>
#endif

#if !defined(_UTILITY_) && !defined(_STLP_UTILITY) && !defined(__STD_UTILITY__) && !defined(_CPP_UTILITY) && !defined(_GLIBCXX_UTILITY)
    #include <utility>
#endif


#if !defined(_SET_) && !defined(_STLP_SET) && !defined(__STD_SET__) && !defined(_CPP_SET) && !defined(_GLIBCXX_SET)
    #include <set>
#endif

#if !defined(_EXCEPTION_) && !defined(__EXCEPTION__) && !defined(_STLP_EXCEPTION) && !defined(__STD_EXCEPTION)

    #include <exception>
#endif

#if !defined(_STDEXCEPT_) && !defined(_STLP_STDEXCEPT) && !defined(__STD_STDEXCEPT) && !defined(_CPP_STDEXCEPT) && !defined(_GLIBCXX_STDEXCEPT)
    #include <stdexcept>
#endif

#if !defined(_INC_ERRNO) && !defined(_ERRNO_H_) && !defined(_ERRNO_H)
    #include <errno.h>
#endif

#if !defined(_INC_STRING) && !defined(__STRING_H_) && !defined(_STRING_H)
    #include <string.h>
#endif

#ifdef _WIN32
    #if !defined(_INC_MALLOC) && !defined(_MALLOC_H_) && !defined(_MALLOC_H)
        #include <malloc.h>
    #endif
#else
    #include <alloca.h>
    #ifndef _alloca
        #define _alloca  alloca
    #endif
#endif

#ifdef _WIN32
    #include <io.h>
    #ifdef __GNUC__
        typedef int errno_t;
    #endif
#endif
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>



#ifdef _WIN32    
    #include <marty/winapi.h>
    #include <direct.h>
    #include <wchar.h>
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <dirent.h>
#endif

#ifndef MARTY_FILENAME_H
    #include <marty/filename.h>
#endif




#ifndef MARTY_STRING
    #define MARTY_STRING(TC) ::std::basic_string<TC>
#endif

#ifndef MARTY_TCSTRING
    #define MARTY_TCSTRING   MARTY_STRING(TC)
#endif

/*
#ifdef USE_MARTY_NAMESPACE
    #ifndef MARTY_NAMESPACE
        #define MARTY_NAMESPACE
    #endif
#endif
*/

#ifdef USE_MARTY_NAMESPACE
    #ifndef FILESYSTEM_IN_MARTY_NAMESPACE
        #define FILESYSTEM_IN_MARTY_NAMESPACE
    #endif
#endif

#ifdef FILESYSTEM_IN_MARTY_NAMESPACE
    #ifndef MARTY_NS
        #define BEGIN_MARTY_NS          namespace marty {
        #define END_MARTY_NS            };
        #define MARTY_NS                ::marty::
    #endif
    #define MARTY_FILESYSTEM_NS     MARTY_NS filesystem::
    #define MARTY_FILESYSTEM        MARTY_NS filesystem
#else
    #ifndef MARTY_NS
        #define BEGIN_MARTY_NS 
        #define END_MARTY_NS 
        #define MARTY_NS                ::
    #endif
    #define MARTY_FILESYSTEM_NS     MARTY_NS filesystem::
    #define MARTY_FILESYSTEM        MARTY_NS filesystem
#endif

#define BEGIN_MARTY_FILESYSTEM_NS  BEGIN_MARTY_NS namespace filesystem {
#define END_MARTY_FILESYSTEM_NS    END_MARTY_NS   };



BEGIN_MARTY_FILESYSTEM_NS

//using MARTY_NS filename::tstring;
typedef MARTY_NS filename::tstring tstring;


tstring getCurrentDirectory();
void getCurrentDirectory(::std::string &dir);
void getCurrentDirectory(::std::wstring &dir);
::std::string rawGetCurrentDirectory();
// file name conversions

struct CINameEncoder
{
    virtual void toChar(const ::std::wstring &w, ::std::string &c) const = 0;
    virtual void toWide(const ::std::string  &c, ::std::wstring &w) const = 0;

    // simple copy
    virtual void toChar(const ::std::string  &w, ::std::string &c) const = 0;  
    virtual void toWide(const ::std::wstring &c, ::std::wstring &w) const = 0; 

    virtual ~CINameEncoder() {}
};

struct CINameEncoderPlain7Bit : public CINameEncoder
{
    void toChar(const ::std::wstring &w, ::std::string &c) const
       {
        c.clear(); c.reserve(w.size());
        ::std::wstring::const_iterator it = w.begin();
        for(; it!=w.end(); ++it)
           {
            c.append(1, (char)(unsigned)(*it & 0x7F));
           }
       }

    void toWide(const ::std::string  &c, ::std::wstring &w) const
       {
        w.clear(); w.reserve(c.size());
        ::std::string::const_iterator it = c.begin();
        for(; it!=c.end(); ++it)
           {
            w.append(1, (wchar_t)(unsigned char)(*it));
           }
       }

    void toChar(const ::std::string  &w, ::std::string &c) const  // simple copy
       { c.assign(w); }

    void toWide(const ::std::wstring &c, ::std::wstring &w) const // simple copy
       { w.assign(c); }
};

struct CINameEncoderPlain8Bit : public CINameEncoder
{
    void toChar(const ::std::wstring &w, ::std::string &c) const
       {
        c.clear(); c.reserve(w.size());
        ::std::wstring::const_iterator it = w.begin();
        for(; it!=w.end(); ++it)
           {
            c.append(1, (char)(unsigned)(*it & 0xFF));
           }
       }

    void toWide(const ::std::string  &c, ::std::wstring &w) const
       {
        w.clear(); w.reserve(c.size());
        ::std::string::const_iterator it = c.begin();
        for(; it!=c.end(); ++it)
           {
            w.append(1, (wchar_t)(unsigned char)(*it));
           }
       }

    void toChar(const ::std::string  &w, ::std::string &c) const  // simple copy
       { c.assign(w); }

    void toWide(const ::std::wstring &c, ::std::wstring &w) const // simple copy
       { w.assign(c); }

};


template < typename CharType
         , typename Traits
         , typename Allocator
>
bool lenPriorStringLess(const ::std::basic_string<CharType, Traits, Allocator> & s1, const ::std::basic_string<CharType, Traits, Allocator> & s2)
{
 if (s1.size()==s2.size()) return s1<s2;
 if (s1.size()<s2.size())  return true;
 return false;
}


template < typename CharType
         , typename Traits
         , typename Allocator
>
bool lenPriorStringGreater(const ::std::basic_string<CharType, Traits, Allocator> & s1, const ::std::basic_string<CharType, Traits, Allocator> & s2)
{
 if (s1.size()==s2.size()) return s1>s2;
 if (s1.size()>s2.size())  return true;
 return false;
}


template < typename CharType
         , typename Traits
         , typename Allocator
>
bool lenPriorStringGreaterStrEncPair(const ::std::pair< ::std::basic_string<CharType, Traits, Allocator>, CINameEncoder*> &p1, const ::std::pair< ::std::basic_string<CharType, Traits, Allocator>, CINameEncoder*> &p2)
{
 return lenPriorStringGreater(p1.first, p2.first);
}
/*
template<typename TF, typename TS>
bool firstEquals(const ::std::pair<TF, TS> &p1, const ::std::pair<TF, TS> &p2)
   {
    return p1.first==p2.first;
   }
*/

template < typename CharType
         , typename Traits=::std::char_traits<CharType>
         , typename Allocator=::std::allocator<CharType>
>
struct CFindNamePairFirst
{
    ::std::basic_string<CharType, Traits, Allocator> searchFor;
    CFindNamePairFirst(const ::std::basic_string<CharType, Traits, Allocator> &sf) : searchFor(sf) {}
    bool operator()(const ::std::pair<std::basic_string<CharType, Traits, Allocator>, CINameEncoder*> &val)
       {
        return val.first==searchFor;
       }
};


template < typename CharType
         , typename Traits=::std::char_traits<CharType>
         , typename Allocator=::std::allocator<CharType>
>
struct CFindNamePairFirstStartsWith
{
    ::std::basic_string<CharType, Traits, Allocator> searchFor;
    CFindNamePairFirstStartsWith(const ::std::basic_string<CharType, Traits, Allocator> &sf) : searchFor(sf) {}
    bool operator()(const ::std::pair<std::basic_string<CharType, Traits, Allocator>, CINameEncoder*> &val)
       {
        if (searchFor.compare(0, val.first.size(), val.first, 0, val.first.size()))
           { // not match
            //resSubPath
            return false;
           }
        return true;
       }
};

template < typename CharType
         , typename Traits=::std::char_traits<CharType>
         , typename Allocator=::std::allocator<CharType>
>
class CFilenameEncoder
{
    typedef CINameEncoder                                     *encoder_ptr_t;
    typedef ::std::basic_string<CharType, Traits, Allocator>   string_type;
    typedef ::std::pair<string_type, encoder_ptr_t>            path_encoder_pair_t;

    encoder_ptr_t pDefEncoder;
    ::std::vector<path_encoder_pair_t> encodersList;

    encoder_ptr_t findEncoder(const string_type &path)
       {
        #ifdef FILENAMES_ARE_CASE_INSENSITIVE
        string_type searchPath = filename::utils::lowerCase(filename::makeCanonical(path), MARTY_NS filename::utils::makeCurrentLocale());
        #else
        string_type searchPath = filename::makeCanonical(path);
        #endif

        typename ::std::vector<path_encoder_pair_t>::iterator it
               = ::std::find_if(encodersList.begin(), encodersList.end(), CFindNamePairFirstStartsWith<typename string_type::traits_type::char_type, typename string_type::traits_type, typename string_type::allocator_type>(searchPath));
        if (it!=encodersList.end())
           {
            return it->second;
           }
        return pDefEncoder;
       }

    public:

    CFilenameEncoder() 
       : pDefEncoder(new CINameEncoderPlain7Bit())
       , encodersList()
       {}

    ~CFilenameEncoder()
       {
        delete pDefEncoder;
        typename ::std::vector<path_encoder_pair_t>::iterator it = encodersList.begin();
        for(; it!=encodersList.end(); ++it)
           {
            encoder_ptr_t pEnc = it->second;
            delete pEnc;
           }
       }

    void setEncoderForPath(const string_type &path, encoder_ptr_t pe)
       {
        #ifdef FILENAMES_ARE_CASE_INSENSITIVE
        string_type pathNew = filename::utils::lowerCase(filename::makeCanonical(path), MARTY_NS filename::utils::makeCurrentLocale());
        #else
        string_type pathNew = filename::makeCanonical(path);
        #endif

        filename::makeValidDirectoryNameInplace(pathNew); // removes trailing slash if it present
        pathNew.append(1, PATH_PREFFERED_SEPARATOR); // and adds its unconditionaly

        typename ::std::vector<path_encoder_pair_t>::iterator it 
               = ::std::find_if(encodersList.begin(), encodersList.end(), CFindNamePairFirst<typename string_type::traits_type::char_type, typename string_type::traits_type, typename string_type::allocator_type>(pathNew));
        if (it!=encodersList.end())
           {
            encoder_ptr_t peOld = it->second;
            delete peOld;
            it->second = pe;
            //encodersList.erase(it);
            return;
           }

        encodersList.push_back(::std::make_pair(pathNew, pe));
        ::std::sort(encodersList.begin(), encodersList.end(), lenPriorStringGreaterStrEncPair<typename string_type::traits_type::char_type, typename string_type::traits_type, typename string_type::allocator_type>);
       }

    void setDefEncoder(encoder_ptr_t pd)
       {
        if (!pd) return;
        delete pDefEncoder;
        pDefEncoder = pd;
       }

    ::std::string encodeName(const string_type &path, const string_type &name)
       {
        encoder_ptr_t pe = findEncoder(path);
        if (!pe)
           throw ::std::logic_error("Encoder not found - the default encoder is also null");
        std::string res;
        pe->toChar(name, res);
        return res;
       }

    ::std::string encodeName(const string_type &name)
       {
        using MARTY_FILENAME_NS makeCanonical;
        using MARTY_FILENAME_NS isAbsolutePath;
        using MARTY_FILENAME_NS appendPath;

        if (isAbsolutePath( name ))
           {
            return encodeName( name, name );
           }
        string_type curDir;
        getCurrentDirectory(curDir);
        const string_type fullPath = makeCanonical( appendPath(curDir, name) );
        return encodeName( fullPath, name );
       }

    string_type decodeName(const ::std::string &name)
       {
        //isPathSeparator
        //if (MARTY_FILENAME_NS isAbsolutePath( fromOsFilename(name) ))
        if (MARTY_FILENAME_NS isAbsolutePath( ::std::make_pair(name.begin(), name.end())
                                            , MARTY_FILENAME_NS isPathSeparator<char>(C_PATH_SEPARATORS)
                                            )
           )
           {
            return decodeName(name, name);
           }
        return decodeName(rawGetCurrentDirectory(), name);
       }

    string_type decodeName(const ::std::string &path, const ::std::string &name)
       {
        CINameEncoderPlain7Bit bit7_encoder;
        string_type tmp;
        bit7_encoder.toWide(path, tmp);

        encoder_ptr_t pe = findEncoder(tmp);
        if (!pe)
           throw ::std::logic_error("Encoder not found - the default encoder is also null");

        string_type res;
        pe->toWide(name, res);
        return res;
       }
};

//-----------------------------------------------------------------------------
/*
template < typename CharType
         , typename Traits
         , typename Allocator
>
CFilenameEncoder<CharType, Traits, Allocator>& getFilenameEncoder(const ::std::basic_string<CharType, Traits, Allocator> &dummy);
*/




CFilenameEncoder< ::std::string::traits_type::char_type
                , ::std::string::traits_type
                , ::std::string::allocator_type>& 
getFilenameEncoder(const ::std::string &dummy);

CFilenameEncoder< ::std::wstring::traits_type::char_type
                , ::std::wstring::traits_type
                , ::std::wstring::allocator_type>& 
getFilenameEncoder(const ::std::wstring &dummy);




//-----------------------------------------------------------------------------
inline
void setFilenameDefEncoder(CINameEncoder* pe)
   {
    getFilenameEncoder(::std::string()) .setDefEncoder(pe);
    getFilenameEncoder(::std::wstring()).setDefEncoder(pe);
   }

//-----------------------------------------------------------------------------
template < typename CharType
         , typename Traits
         , typename Allocator
>
void setFilenameEncoder(const ::std::basic_string<CharType, Traits, Allocator> &rootPath, CINameEncoder* pe)
   {
    getFilenameEncoder(rootPath).setEncoderForPath(rootPath, pe);
   }

//-----------------------------------------------------------------------------
inline
void filenameEncode(::std::string &encodeTo, const ::std::string &fn)
   {
    encodeTo = fn;
   }

//-----------------------------------------------------------------------------
inline
void filenameEncode(::std::string &encodeTo, const ::std::string &path, const ::std::string &fn)
   {
    encodeTo = fn;
   }

//-----------------------------------------------------------------------------
inline
void filenameEncode(::std::string &encodeTo, const ::std::wstring &fn)
   {
    encodeTo = getFilenameEncoder(fn).encodeName(fn);
   }

//-----------------------------------------------------------------------------
inline
void filenameEncode(::std::string &encodeTo, const ::std::wstring &path, const ::std::wstring &fn)
   {
    encodeTo = getFilenameEncoder(path).encodeName(path, fn);
   }

/*
template < typename CharType
         , typename Traits
         , typename Allocator
>
::std::string filenameEncode(const ::std::basic_string<CharType, Traits, Allocator> &fn)
   {
    // small optimization
    if (sizeof(CharType)==sizeof(::std::string::traits_type::char_type))
       return fn;

    return getFilenameEncoder(fn).encodeName(fn);
   }
*/
//-----------------------------------------------------------------------------
/*
template < typename CharType
         , typename Traits
         , typename Allocator
>
::std::string filenameEncode(const ::std::basic_string<CharType, Traits, Allocator> &path, const ::std::basic_string<CharType, Traits, Allocator> &fn)
   {
    // small optimization
    if (sizeof(CharType)==sizeof(::std::string::traits_type::char_type))
       return fn;

    return getFilenameEncoder(path).encodeName(path, fn);
   }
*/
//-----------------------------------------------------------------------------
// if it gets compiler error, try
// filenameDecode<::std::string::traits_type::char_type, ::std::string::traits_type, ::std::wstring::allocator_type>
// or
// filenameDecode<::std::wstring::traits_type::char_type, ::std::wstring::traits_type, ::std::wstring::allocator_type>
/*
template < typename CharType
         , typename Traits
         , typename Allocator
>
::std::basic_string<CharType, Traits, Allocator>
filenameDecode(const ::std::string &fn)
   {
    // small optimization
    if (sizeof(CharType)==sizeof(::std::string::traits_type::char_type))
       return fn;

    return getFilenameEncoder(fn).decodeName(fn);
   }

//-----------------------------------------------------------------------------
template < typename CharType
         , typename Traits
         , typename Allocator
>
::std::basic_string<CharType, Traits, Allocator>
filenameDecode(const ::std::string &path, const ::std::string &fn)
   {
    // small optimization
    if (sizeof(CharType)==sizeof(::std::string::traits_type::char_type))
       return fn;

    return getFilenameEncoder(fn).decodeName(path, fn);
   }
*/

//-----------------------------------------------------------------------------
inline
void filenameDecode(::std::string &decodeTo, const ::std::string &fn)
   {
    decodeTo = fn; // getFilenameEncoder(decodeTo).decodeName(fn);
   }

//-----------------------------------------------------------------------------
inline
void filenameDecode(::std::string &decodeTo, const ::std::string &path, const ::std::string &fn)
   {
    decodeTo = fn; // getFilenameEncoder(decodeTo).decodeName(path, fn);
   }

//-----------------------------------------------------------------------------
inline
void filenameDecode(::std::wstring &decodeTo, const ::std::string &fn)
   {
    decodeTo = getFilenameEncoder(decodeTo).decodeName(fn);
   }

//-----------------------------------------------------------------------------
inline
void filenameDecode(::std::wstring &decodeTo, const ::std::string &path, const ::std::string &fn)
   {
    decodeTo = getFilenameEncoder(decodeTo).decodeName(path, fn);
   }




#ifdef _WIN32

    /*
    inline void osFilename2(::std::wstring &nameTo, const ::std::wstring &name )
       {
        nameTo = MARTY_WINAPI_NS makeUnicodeLongFilename(name);
       }

    inline void osFilename2(::std::wstring &nameTo, const ::std::wstring &path, const ::std::wstring &name )
       {
        nameTo = MARTY_WINAPI_NS makeUnicodeLongFilename(name);
       }
    */

    #if defined(UNICODE) || defined(_UNICODE)
        inline ::std::wstring osFilename    (const ::std::wstring &name)
           { return MARTY_WINAPI_NS makeUnicodeLongFilename(name); }
        inline ::std::wstring osFilename    (const ::std::wstring &path, const ::std::wstring &name)
           { return osFilename(name); }
        inline ::std::wstring fromOsFilename(const ::std::wstring &name)
           { return MARTY_WINAPI_NS makeFilenameFromLongUnicode(name); }
        inline ::std::wstring fromOsFilename(const ::std::wstring &path, const ::std::wstring &name)
           { return fromOsFilename(name); }
    #else
        inline ::std::string  osFilename    (const ::std::string &name)
           { return name; }
        inline ::std::string  osFilename    (const ::std::string &path, const ::std::string &name)
           { return name; }
        inline ::std::string  fromOsFilename(const ::std::string &name)
           { return name; }
        inline ::std::string  fromOsFilename(const ::std::string &path, const ::std::string &name)
           { return name; }
    #endif

#else /* non-Win32 */

    #ifdef _UNICODE
        inline ::std::string  osFilename    (const ::std::wstring &name)
            { ::std::string tmp; filenameEncode(tmp, name); return tmp; }

        inline ::std::string  osFilename    (const ::std::wstring &path, const ::std::wstring &name)
            { ::std::string tmp; filenameEncode(tmp, path, name); return tmp; }

        inline ::std::wstring fromOsFilename(const ::std::string &name)
            { ::std::wstring tmp; filenameDecode(tmp, name); return tmp; }

        inline ::std::wstring fromOsFilename(const ::std::string &path, const ::std::string &name)
            { ::std::wstring tmp; filenameDecode(tmp, path, name); return tmp; }

    #else
        inline ::std::string  osFilename    (const ::std::string &name)
            { return name; }
        inline ::std::string  osFilename    (const ::std::string &path, const ::std::string &name)
            { return name; }
        inline ::std::string fromOsFilename (const ::std::string &name)
            { return name; }
        inline ::std::string fromOsFilename (const ::std::string &path, const ::std::string &name)
            { return name; }

    #endif

    inline void fromOsFilename2(::std::string &nameTo, const ::std::string &name)
        { nameTo = name; }

    inline void fromOsFilename2(::std::wstring &nameTo, const ::std::string &name)
        { filenameDecode(nameTo, name); }


#endif


//-----------------------------------------------------------------------------
typedef int fileopenmode;

/*
#ifdef _WIN32
enum fileopenmode{
    o_rdonly = _O_RDONLY,
    o_wronly = _O_WRONLY,
    o_rdwr   = _O_RDWR,
    o_append = _O_APPEND,
    o_creat  = _O_CREAT,
    o_trunc  = _O_TRUNC,
    o_excl   = _O_EXCL // error if create and file exists (create only, not open)
};
#else
enum fileopenmode{
    o_rdonly = O_RDONLY,
    o_wronly = O_WRONLY,
    o_rdwr   = O_RDWR,
    o_append = O_APPEND,
    o_creat  = O_CREAT,
    o_trunc  = O_TRUNC,
    o_excl   = O_EXCL // error if create and file exists (create only, not open)
};
#endif
*/

#ifdef _WIN32
    const int o_rdonly = _O_RDONLY;
    const int o_wronly = _O_WRONLY;
    const int o_rdwr   = _O_RDWR;
    const int o_append = _O_APPEND;
    const int o_creat  = _O_CREAT;
    const int o_trunc  = _O_TRUNC;
    const int o_excl   = _O_EXCL; // error if create and file exists (create only, not open)
#else
    const int o_rdonly = O_RDONLY;
    const int o_wronly = O_WRONLY;
    const int o_rdwr   = O_RDWR;
    const int o_append = O_APPEND;
    const int o_creat  = O_CREAT;
    const int o_trunc  = O_TRUNC;
    const int o_excl   = O_EXCL; // error if create and file exists (create only, not open)
#endif


// NOTE: Other flags are non-portable, but may be used on apropriate platform if needed

/*
    open  , _open_s, _open, _wsopen_s
    int creat(const char *path, mode_t mode);
    chmod(2), close(2), dup(2), getdtablesize(2), lseek(2), read(2), umask(2), write(2)

    close
    read
    write
    eof

*/

#if   defined(_WIN32_CHAR)
#elif defined(_WIN32_UNICODE)
#elif defined(_POSIX_CHAR)
#else      // _POSIX_UNICODE
#endif


typedef int handle_t;

const handle_t hStdin   = 0;
const handle_t hStdout  = 1;
const handle_t hStderr  = 2;
const handle_t hInvalidHandle = -1;

inline
unsigned getFileOpNativeError()
   {
    #ifdef _WIN32
    return GetLastError();
    #else
    return errno;
    #endif
   }


//#if defined(_WIN32_CHAR)
#if defined(WIN32)
inline
handle_t openFile(const ::std::string &filename, fileopenmode oflag, bool createReadonly = false)
   {
    if (filename.empty()) { errno = EINVAL; return -1; }

    // force binary mode alwais
    oflag = fileopenmode((oflag | (fileopenmode)_O_BINARY) & ((fileopenmode)(~_O_TEXT)));
    int handle;

    int pmode = _S_IREAD;
    if (!createReadonly) pmode |= _S_IWRITE;

    #ifndef __GNUC__
        //#if defined(UNICODE) || defined(_UNICODE)
            //errno_t res = ::_sopen_s( &handle, filename.c_str(), (int)oflag, _SH_DENYNO, pmode);
        //#else
            errno_t res = ::_sopen_s( &handle, filename.c_str(), (int)oflag, _SH_DENYNO, pmode);
        //#endif
    #else
    //#if defined(UNICODE) || defined(_UNICODE)
    //handle = ::_open(osFilename(filename).c_str(), (int)oflag, pmode);
    handle = ::_open(filename.c_str(), (int)oflag, pmode);
    errno_t res = errno;
    #endif
    if (handle==-1) errno = res;

    return handle;
   }
//#elif defined(_WIN32_UNICODE)
inline
handle_t openFile(const ::std::wstring &filename, fileopenmode oflag, bool createReadonly = false)
   {
    if (filename.empty()) { errno = EINVAL; return -1; }

    // force binary mode alwais
    oflag = fileopenmode((oflag | (fileopenmode)_O_BINARY) & ((fileopenmode)(~_O_TEXT)));
    int handle;

    int pmode = _S_IREAD;
    if (!createReadonly) pmode |= _S_IWRITE;

    #ifndef __GNUC__
        #if defined(UNICODE) || defined(_UNICODE)
        errno_t res = ::_wsopen_s( &handle, osFilename(filename).c_str(), (int)oflag, _SH_DENYNO, pmode);
        #else
        errno_t res = ::_wsopen_s( &handle, filename.c_str(), (int)oflag, _SH_DENYNO, pmode);
        #endif
    #else
        #if defined(UNICODE) || defined(_UNICODE)
        handle = ::_wopen( osFilename(filename).c_str(), (int)oflag, pmode);
        #else
        handle = ::_wopen( filename.c_str(), (int)oflag, pmode);
        #endif
    errno_t res = errno;
    #endif
    if (handle==-1) errno = res;

    return handle;
   }
#else
template < typename CharType
         , typename Traits
         , typename Allocator
>
handle_t openFile(const ::std::basic_string<CharType, Traits, Allocator> &filename, fileopenmode oflag, bool createReadonly = false)
   {
    if (filename.empty()) { errno = EINVAL; return -1; }

    int pmode = S_IROTH|S_IRGRP|S_IRUSR;
    if (!createReadonly) pmode |= S_IWUSR;

    return ::open( osFilename(filename).c_str(), (int)oflag, pmode);
   }

template < typename CharType >
handle_t openFile(const CharType *filename, fileopenmode oflag, bool createReadonly = false)
   {
    if (!filename) { errno = EINVAL; return -1; }
    return openFile(::std::basic_string<CharType, ::std::char_traits<CharType>, ::std::allocator<CharType> >(filename), oflag, createReadonly );
   }

#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



/* wrappers maked for full set of file functions */

//-----------------------------------------------------------------------------
inline 
int closeFile( handle_t fd )
   {
    #if defined(_WIN32) && !defined(__GNUC__)
    return ::_close(fd);
    #else
    return ::close(fd);
    #endif
   }

struct CHFile
{
    handle_t hFile;
    CHFile(  ) : hFile(hInvalidHandle) {}
    CHFile( handle_t h ) : hFile(h) {}
    operator handle_t() const { return hFile; }
    ~CHFile( ) { closeFile(hFile); }

};


//-----------------------------------------------------------------------------
inline 
int readFile(handle_t fd, void* buf, unsigned numOfBytesToRead)
   {
    #if defined(_WIN32) && !defined(__GNUC__)
    return (int)::_read(fd, buf, numOfBytesToRead);
    #else
    return (int)::read(fd, buf, numOfBytesToRead);
    #endif
   }

//-----------------------------------------------------------------------------
inline 
int writeFile(handle_t fd, const void* buf, unsigned numOfBytesToWrite)
   {
    #if defined(_WIN32) && !defined(__GNUC__)
    return (int)::_write(fd, buf, numOfBytesToWrite);
    #else
    return (int)::write(fd, buf, numOfBytesToWrite);
    #endif
   }

inline
::std::string errorStr(int ec)
   {
    #if defined(_WIN32)
        #if defined(_MSC_VER)
        char buf[1024];
        if (strerror_s(buf, sizeof(buf)/sizeof(buf[0]), ec))
           return ::std::string("Error while formating error message");
        return ::std::string(buf);
        #else // mingw and others
        return ::std::string(::strerror(ec));
        #endif
    #else // Linux and others
        char buf[1024];
        #if (defined(__USE_XOPEN2K) || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE>=600)) && !defined(__USE_GNU)
        int res   = strerror_r(ec, buf, sizeof(buf)/sizeof(buf[0]));
        if (res) return ::std::string("Error while formating error message");
        #else
        char *res = strerror_r(ec, buf, sizeof(buf)/sizeof(buf[0]));
        if (!res) return ::std::string("Error while formating error message");
        return ::std::string(res);
        #endif
    #endif
   }

inline
::std::string errorStr()
   {
    return errorStr(errno);
   }


template <typename THandler, unsigned maxSize, char delim>
struct CLineBuf
{
    THandler     handler;
    char         buf[maxSize];
    unsigned     curSize;

    CLineBuf(const THandler &h) : handler(h), curSize(0) {}
    bool operator()(char ch)
       {
        if (ch==delim)
           { // call line handler and clear buf
            if (!handler(buf, curSize)) return false;
            curSize = 0;
            return true;
           }
        if (curSize>=maxSize)
           { // call handler on buffer overflow
            if (!handler(buf, curSize)) return false;
            curSize = 0;
           }
        buf[curSize++] = ch;
        return true;
       }

    ~CLineBuf()
       {
        if (curSize)
           handler(buf, curSize);
       }

};


template <typename THandler, unsigned maxLineSize, char delim>
int readFileLines(handle_t fd, const THandler &h)
   {
    CLineBuf<THandler, maxLineSize, delim> lineBuf(h);
    char buf[4096];
    int readed = readFile(fd, buf, sizeof(buf));
    while(readed>0)
       {
        char *ptmp = buf;
        char *pend = ptmp + readed;
        for(; ptmp!=pend; ++ptmp)
           if (!lineBuf(*ptmp)) return 0;
        readed = readFile(fd, buf, sizeof(buf));
       }
    return readed;
   }


//-----------------------------------------------------------------------------
//#if defined(_WIN32)
inline
::std::string rawGetCurrentDirectory() // used under Unix/Linux
   {
    size_t size = 1024;
    char *buf = (char*)_alloca(size*sizeof(buf[0]));
    #ifdef _WIN32
    char* pRes = _getcwd( buf, int(size));
    #else
    char* pRes = getcwd( buf, int(size));
    #endif
    while(!pRes && errno==ERANGE)
       {
        size *= 2;
        buf = (char*)_alloca(size*sizeof(buf[0]));
        #ifdef _WIN32
        pRes = _getcwd( buf, int(size));
        #else
        pRes = getcwd( buf, int(size));
        #endif
       }
    if (!pRes) return ::std::string();
    return ::std::string(pRes);
   }
//-----------------------------------------------------------------------------
inline
#ifdef _UNICODE
::std::wstring getCurrentDirectory()
#else
::std::string getCurrentDirectory()
#endif
   {
    #if defined(_WIN32)
    return fromOsFilename(winapi::getCurrentDirectory());
    #else
    return fromOsFilename(rawGetCurrentDirectory());
    #endif
   }

inline
void getCurrentDirectory(::std::string &dir)
   {
    #if defined(_WIN32)
    winapi::getCurrentDirectory(dir);
    #else
    dir = rawGetCurrentDirectory();
    #endif
   }

inline
void getCurrentDirectory(::std::wstring &dir)
   {
    #if defined(_WIN32)
    winapi::getCurrentDirectory(dir);
    #else
    fromOsFilename2(dir, rawGetCurrentDirectory());
    #endif   
   }



//-----------------------------------------------------------------------------
inline
#ifdef _UNICODE
int setCurrentDirectory(const ::std::wstring &dir)
#else
int setCurrentDirectory(const ::std::string &dir)
#endif
   {
    #if defined(_WIN32)
    return (int)winapi::setCurrentDirectory(osFilename(dir));
    #else
    int res = chdir(osFilename(dir).c_str());
    if (res)
       {
        errno = res;
        return 0;
       }
    return 1;
    #endif
   }

//-----------------------------------------------------------------------------
inline
#ifdef _UNICODE
int createDirectory(const ::std::wstring &dir)
#else
int createDirectory(const ::std::string &dir)
#endif
   {
    #if defined(_WIN32)
    return (int)winapi::createDirectory(osFilename(dir));
    #else
    int res = mkdir(osFilename(dir).c_str(), S_IRWXU|S_IROTH|S_IXOTH|S_IRGRP|S_IXGRP);
    if (res) return 0;
    return 1;
    //mkdir(1), chmod(2), mount(2), rmdir(2), unlink(2)   
    #endif
   }

/*
struct enumHandler
{
    // Linux/Posix - attrs is a dirent.d_type
    // Win32       - attrs is file attributes
    // to test is file a directory, use efIsDir(attrs)
    bool operator()(const tstring &path, const tstring &name, int attrs)
       {
        return true; // continue
       }
};
*/

#ifdef _WIN32

inline
bool efIsDir(int attrsOrType)
   {
    return attrsOrType&FILE_ATTRIBUTE_DIRECTORY ? true : false;
   }

template < typename CharType
         , typename Traits
         , typename Allocator
         , typename THandler
         >
int enumFilesAux(::std::basic_string<CharType, Traits, Allocator> path, THandler hdl)
   {
    //std::cout<<"CP16\n";
    ::std::basic_string<CharType, Traits, Allocator> pathMask 
         = MARTY_FILENAME_NS appendPath(path, ::std::basic_string<CharType, Traits, Allocator>(1, CharType('*')));
    MARTY_FILENAME_NS stripSlashInplace(path);
    ::std::basic_string<CharType, Traits, Allocator> osPathMask = osFilename( pathMask );

    //std::cout<<"CP17\n";
    WIN32_FIND_DATA fndData;
    HANDLE hFind = ::FindFirstFile( osPathMask.c_str(), &fndData );
    //std::cout<<"CP18\n";
    if (hFind==INVALID_HANDLE_VALUE) 
       {
        DWORD err = ::GetLastError();
        if (err==ERROR_NO_MORE_FILES) return 0; // no more files, it's not an error        
        return 0;
       }

    do{
       //std::cout<<"CP19\n";
       if (!hdl(path, fromOsFilename(path, ::std::basic_string<CharType, Traits, Allocator>(fndData.cFileName)), fndData.dwFileAttributes))
          break;
       //std::cout<<"CP20\n";
      } while(::FindNextFile( hFind, &fndData ));

    //std::cout<<"CP21\n";
    ::FindClose(hFind);
    return 0;
   }

#else

inline
bool efIsDir(int attrsOrType)
   {
    return attrsOrType==DT_DIR ? true : false;
   }

template < typename CharType
         , typename Traits
         , typename Allocator
         , typename THandler
         >
int enumFilesAux(::std::basic_string<CharType, Traits, Allocator> path, THandler hdl)
   {
    //tstring pathCopy 
    //tstring 
    //path = 
    MARTY_FILENAME_NS stripSlashInplace(path);
    //::std::string osPathCopy 
    ::std::string osPath = osFilename(path);

    DIR *pDir = opendir(osPath.c_str());
    if (!pDir) return errno;

    struct dirent *pDirent = readdir(pDir);
    while(pDirent)
       {
        if (!hdl(path, fromOsFilename(osPath, pDirent->d_name), pDirent->d_type))
           break;
        pDirent = readdir(pDir);
       }

    closedir(pDir);
    return 0;
   }

#endif

namespace util
{

template<typename THandler>
struct enumHandlerDirCollectorAndProxy
{
    ::std::vector<tstring> &pathList;
    ::std::set<tstring>    &pathSet;
    //const ::std::vector<tstring> &excludePathList;
    const ::std::set<tstring> &excludePathSet;
    THandler                handler;
    volatile bool          &bContinue;
    bool                    traverseSubdirs;

    enumHandlerDirCollectorAndProxy(::std::vector<tstring> &pl, ::std::set<tstring> &ps, const ::std::set<tstring> &eps, THandler h, volatile bool &bC, bool ts)
       : pathList(pl)
       , pathSet(ps)
       , excludePathSet(eps)
       , handler(h)
       , bContinue(bC)
       , traverseSubdirs(ts)
       {}

    enumHandlerDirCollectorAndProxy(const enumHandlerDirCollectorAndProxy &eh)
       : pathList(eh.pathList)
       , pathSet(eh.pathSet)
       , excludePathSet(eh.excludePathSet)
       , handler(eh.handler)
       , bContinue(eh.bContinue)
       , traverseSubdirs(eh.traverseSubdirs)
       {}

    // Linux/Posix - attrs is a dirent.d_type
    // Win32       - attrs is file attributes
    // to test is file a directory, use efIsDir(attrs)

    bool operator()(const tstring &path, const tstring &name, int attrs)
       {
        if ( efIsDir(attrs))
           {
            //std::cout<<"CP22\n";
            if (MARTY_FILENAME_NS isCurrentDirectorySpecialName(name) || MARTY_FILENAME_NS isUpperDirectorySpecialName(name))
               {
                //std::cout<<"CP23\n";
                return true; // don't call handler on special directories '.' and '..'
               }

            //std::cout<<"CP24\n";
            if (traverseSubdirs)
               {
                //std::cout<<"CP25\n";
                tstring newPath = MARTY_FILENAME_NS makeCanonical(MARTY_FILENAME_NS appendPath(path, name));
                std::set<tstring>::const_iterator it = excludePathSet.find( newPath );

                //std::cout<<"CP26\n";
                if (it==excludePathSet.end())
                   { // not found in exluded paths list
                    it = pathSet.find( newPath );
                    if (it==pathSet.end())
                       {
                        pathList.push_back(newPath);
                        pathSet.insert(newPath);
                       }
                   }
                //std::cout<<"CP27\n";
                /*
                std::vector<tstring>::const_iterator it = std::find_if(
                                                                 pathList.begin(),
                                                                 pathList.end(),
                                                                 MARTY_FILENAME_NS compareTo<MARTY_FILENAME_NS compareProcType>(newPath, filename::equal)
                                                                );
                if (it==pathList.end())
                */
                   
               }
           }
        //std::cout<<"CP28\n";
        bContinue = handler(path, name, attrs); // continue or stop
        //std::cout<<"CP29\n";
        return bContinue;
       }
};

}; // namespace util




template<typename THandler>
int enumFilesPathListEx(const ::std::vector<tstring> &_pathList, const ::std::vector<tstring> &excludePathList, THandler hdl, bool traverseSubdirs = true)
   {
    volatile 
    bool          bContinue = true;
    ::std::vector<tstring> pathList = _pathList;

    ::std::set<tstring> pathSet(pathList.begin(), pathList.end());
    ::std::set<tstring> excludePathSet(excludePathList.begin(), excludePathList.end());
    //pathList.push_back(path);

    util::enumHandlerDirCollectorAndProxy<THandler> proxy(pathList, pathSet, excludePathSet, hdl, bContinue, traverseSubdirs);

    ::std::vector<tstring>::size_type i = 0;
    for(; i!=pathList.size() && bContinue; ++i)
       {
        int res = enumFilesAux(pathList[i], proxy);
        if (res) return res;
       }
    return 0;
   }

template<typename THandler>
int enumFilesEx(const tstring &path, const ::std::vector<tstring> &excludePathList, THandler hdl, bool traverseSubdirs = true)
   {
    //std::cout<<"CP11\n";
    volatile 
    bool          bContinue = true;
    ::std::vector<tstring> pathList(1, path);
    ::std::set<tstring> pathSet(pathList.begin(), pathList.end());
    ::std::set<tstring> excludePathSet;
    //pathList.push_back(path);
    //std::cout<<"CP12\n";
    util::enumHandlerDirCollectorAndProxy<THandler> proxy(pathList, pathSet, excludePathSet, hdl, bContinue, traverseSubdirs);
    //std::cout<<"CP13\n";
    ::std::vector<tstring>::size_type i = 0;
    //std::cout<<"CP14\n";
    for(; i!=pathList.size() && bContinue; ++i)
       {
        //std::cout<<"CP15\n";
        int res = enumFilesAux(pathList[i], proxy);
        if (res) return res;
       }
    return 0;
   }

template<typename THandler>
int enumFilesPathList(const ::std::vector<tstring> &pathList, THandler hdl, bool traverseSubdirs = true)
   {
    ::std::vector<tstring> excludePathList;
    return enumFilesPathListEx(pathList, excludePathList, hdl, traverseSubdirs);
   }

template<typename THandler>
int enumFiles(const tstring &path, THandler hdl, bool traverseSubdirs = true)
   {
    //std::cout<<"CP9\n";
    ::std::vector<tstring> excludePathList;
    //std::cout<<"CP10\n";
    return enumFilesEx(path, excludePathList, hdl, traverseSubdirs);
   }


struct CFindFileInfo
{
    tstring path;
    tstring file;
    tstring matchedMask;
    int     attrs;

    CFindFileInfo()
       : path()
       , file()
       , attrs(0)
       {}

    CFindFileInfo(const tstring &p, const tstring &f, const tstring &mm, int a)
       : path(p)
       , file(f)
       , matchedMask(mm)
       , attrs(a)
       {}

    CFindFileInfo(const CFindFileInfo &ffi)
       : path(ffi.path)
       , file(ffi.file)
       , matchedMask(ffi.matchedMask)
       , attrs(ffi.attrs)
       {}
};


namespace util
{

struct FilenameMatchMaskPred
{
    std::vector<CFindFileInfo> &matchedFiles;
    const std::vector<tstring> &maskList;
    //const tstring              &mask;
    bool                       cmpWithPath;

    FilenameMatchMaskPred(std::vector<CFindFileInfo> &mf, const std::vector<tstring> &ml, bool cwp)
       : matchedFiles(mf)
       , maskList(ml)
       , cmpWithPath(cwp)
       {}

    FilenameMatchMaskPred(const FilenameMatchMaskPred& fmm)
       : matchedFiles(fmm.matchedFiles)
       , maskList(fmm.maskList)
       , cmpWithPath(fmm.cmpWithPath)
       { }

    bool operator()(const tstring &path, const tstring &name, int attrs)
       {
        //std::cout<<"CP30\n";
        tstring fullname = name;
        //std::cout<<"CP31\n";
        if (cmpWithPath)
           {
            //std::cout<<"CP32\n";
            fullname = MARTY_FILENAME_NS makeCanonical(MARTY_FILENAME_NS appendPath(path, name));
            //std::cout<<"CP33\n";
           }

        for(std::vector<tstring>::const_iterator mit = maskList.begin(); mit!=maskList.end(); ++mit)
           {
            //std::cout<<"CP34\n";
            if (MARTY_FILENAME_NS matchMask(fullname, *mit))
               {
                //std::cout<<"CP35\n";
                matchedFiles.push_back(CFindFileInfo(path, name, *mit, attrs));
                //std::cout<<"CP36\n";
               }
           }
        //std::cout<<"CP37\n";
        return true;
       }

};

}; // namespace util


inline
int findFilesPathListEx( std::vector<CFindFileInfo> &matchedFiles
             , const std::vector<tstring> &pathList
             , const std::vector<tstring> &excludePathList
             , const std::vector<tstring> &maskList
             //, const tstring &mask
             , bool  recurseSubdirs = true
             , bool  forceCmpWithPath = false
             )
   {
    for(std::vector<tstring>::const_iterator mit = maskList.begin(); !forceCmpWithPath && mit!=maskList.end(); ++mit)
       {
        if (!MARTY_FILENAME_NS getPath(*mit).empty())
           forceCmpWithPath = true;
       }

    util::FilenameMatchMaskPred pred(matchedFiles, maskList, forceCmpWithPath);

    return enumFilesPathListEx(pathList, excludePathList, pred, recurseSubdirs);
   }

inline
int findFilesPathList( std::vector<CFindFileInfo> &matchedFiles
             , const std::vector<tstring> &pathList
             , const std::vector<tstring> &maskList
             , bool  recurseSubdirs = true
             , bool  forceCmpWithPath = false
             )
   {
    for(std::vector<tstring>::const_iterator mit = maskList.begin(); !forceCmpWithPath && mit!=maskList.end(); ++mit)
       {
        if (!MARTY_FILENAME_NS getPath(*mit).empty())
           forceCmpWithPath = true;
       }

    util::FilenameMatchMaskPred pred(matchedFiles, maskList, forceCmpWithPath);

    return enumFilesPathList(pathList, pred, recurseSubdirs);
   }

inline
int findFilesEx( std::vector<CFindFileInfo> &matchedFiles
             , const tstring &path
             , const std::vector<tstring> &excludePathList
             , const tstring &mask
             , bool  recurseSubdirs = true
             , bool  forceCmpWithPath = false
             )
   {
    if (!MARTY_FILENAME_NS getPath(mask).empty())
       forceCmpWithPath = true;

    std::vector<tstring> maskList(1, mask);

    util::FilenameMatchMaskPred pred(matchedFiles, maskList, forceCmpWithPath);

    return enumFilesEx(path, excludePathList, pred, recurseSubdirs);
   }

inline
int findFiles( std::vector<CFindFileInfo> &matchedFiles
             , const tstring &path
             , const tstring &mask
             , bool  recurseSubdirs = true
             , bool  forceCmpWithPath = false
             )
   {
    //std::cout<<"CP5.5\n";
    if (!MARTY_FILENAME_NS getPath(mask).empty())
       forceCmpWithPath = true;

    //std::cout<<"CP6\n";
    std::vector<tstring> maskList(1, mask);
    //std::cout<<"CP7\n";
    util::FilenameMatchMaskPred pred(matchedFiles, maskList, forceCmpWithPath);
    //std::cout<<"CP8\n";
    return enumFiles(path, pred, recurseSubdirs);
   }




//DIR *opendir(const char *name);
//struct dirent *readdir(DIR *dir);
//int closedir(DIR *dir);

//tstring stripSlashCopy(const tstring &dir, const TCHAR* pathSeparators = PATH_SEPARATORS)
//void stripSlashInplace(tstring &dir, const TCHAR* pathSeparators = PATH_SEPARATORS)


END_MARTY_FILESYSTEM_NS

#endif /* MARTY_FILESYS_H */

