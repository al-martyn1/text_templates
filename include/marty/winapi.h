/*
   Copyright (c) 2007-2009 Alex Martynov, amart at mail.ru
   This code can be used without any limitations.
   This code provided as is, use it on your own risk.
*/

#ifndef MARTY_WINAPI_H
#define MARTY_WINAPI_H

/* Copyright (c) 2004-2007 Alex Martynov, amart at mail dot ru
 */

// define WINAPI_IN_MARTY_NAMESPACE for use ::marty::winapi::* insted of ::winapi::*
// also you can define MARTY_NAMESPACE macro, which automaticaly define WINAPI_IN_MARTY_NAMESPACE

/*
#ifdef USE_MARTY_NAMESPACE
    #ifndef MARTY_NAMESPACE
        #define MARTY_NAMESPACE
    #endif
#endif
*/

#ifndef MARTY_BASICTCHARDEFS_H
    #include <marty/basictchardefs.h>
#endif



#ifdef USE_MARTY_NAMESPACE
    #ifndef WINAPI_IN_MARTY_NAMESPACE
        #define WINAPI_IN_MARTY_NAMESPACE
    #endif
#endif

#ifdef WINAPI_IN_MARTY_NAMESPACE
    #ifndef MARTY_NS
        #define BEGIN_MARTY_NS          namespace marty {
        #define END_MARTY_NS            };
        #define MARTY_NS                ::marty::
    #endif
    #define MARTY_WINAPI_NS         MARTY_NS winapi::
    #define MARTY_WINAPI            MARTY_NS winapi
#else
    #ifndef MARTY_NS
        #define BEGIN_MARTY_NS 
        #define END_MARTY_NS 
        #define MARTY_NS                ::
    #endif
    #define MARTY_WINAPI_NS        MARTY_NS winapi::
    #define MARTY_WINAPI           MARTY_NS winapi
#endif

#define BEGIN_MARTY_WINAPI_NS      BEGIN_MARTY_NS namespace winapi {
#define END_MARTY_WINAPI_NS        END_MARTY_NS   };

#ifdef WINAPI_IN_MARTY_NAMESPACE
    #define USING_MARTY_WINAPI using namespace ::marty::winapi
#else
    #define USING_MARTY_WINAPI using namespace ::winapi
#endif

#if !defined(_WINDOWS_)
    #include <windows.h>
#endif

#if !defined(_INC_MALLOC) && !defined(_MALLOC_H_) && !defined(_MALLOC_H)
    #include <malloc.h>
#endif



#if !defined(WINAPI_NO_SHFOLDER) && defined(WIN32) && defined(__GNUC__)
    #define WINAPI_NO_SHFOLDER
#endif /* MinGW does not support ShFolder.h */


#if !defined(WINAPI_NO_SHFOLDER)

/* ShlObj.h need to be included before ShFolder.h */
#ifndef _SHLOBJ_H_
    #include <ShlObj.h>
#endif

#ifndef _SHFOLDER_H_
    #include <ShFolder.h>
#endif

#endif /* WINAPI_NO_SHFOLDER */



#if !defined(_VECTOR_) && !defined(_STLP_VECTOR) && !defined(__STD_VECTOR__) && !defined(_CPP_VECTOR) && !defined(_GLIBCXX_VECTOR)
    #include <vector>
#endif

#if !defined(_ALGORITHM_) && !defined(_STLP_ALGORITHM) && !defined(__STD_ALGORITHM) && !defined(_CPP_ALGORITHM) && !defined(_GLIBCXX_ALGORITHM)
    #include <algorithm>
#endif


#ifndef FILENAME_OLD_NAMES
    #define FILENAME_OLD_NAMES
#endif

#ifndef MARTY_FILENAME_H
    #include <marty/filename.h>
#endif


#ifdef WINAPI_IN_MARTY_NAMESPACE
namespace marty {
#endif


namespace winapi
{

#ifndef FILENAME_USING_TSTRING
    #ifdef FILENAME_IN_MARTY_NAMESPACE
        //using ::marty::filename::tstring;
        typedef ::marty::filename::tstring tstring;
    #else
        //using ::filename::tstring;
        typedef ::filename::tstring        tstring;
    #endif
#endif


inline
bool getVersionInfo(OSVERSIONINFO &info)
   {
    info.dwOSVersionInfoSize = sizeof(info);
    return GetVersionEx(&info) ? true : false;
   }

inline
DWORD getWinVer()
   {
    OSVERSIONINFO info;
    if (!getVersionInfo(info)) return 0x0400;
    return info.dwMajorVersion<<8 | info.dwMinorVersion;
   }

inline
bool isWinNt()
   {
    OSVERSIONINFO info;
    if (!getVersionInfo(info)) return false;
    return info.dwPlatformId==VER_PLATFORM_WIN32_NT;
   }





/*
   getter(TCHAR *buf, size_t size)
*/

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/*
struct TGetter
{
   TGetter() {}
   DWORD operator() (TCHAR *buf, size_t size) const
      { return ::WiapiCall( ... , buf, (DWORD)size); }
};
*/

// Шаблон предназначен для написания оберток WinAPI функций.
// T getter получает указатель на буфер и размер буфера,
// вызывает нужный WinAPI вызов, передавая при необходимости
// дополнительные параметры. Системная функция должна относится к такому типу 
// функций, которые при недостаточной длине буфера возвращают его же длину.
// Шаблон getApiStringValue несколько раз вызывает заданный getter, до тех пор пока
// возвращенная длина не будет меньше переданной. 
// getApiStringValue инкапсулирует стратегию выделения памяти. В данной реализации
// используется __alloca с увеличением размера выделяемой памяти в два раза при неудаче.
// Данный способ делает более сложным вызов системных функций (надо написать класс getter,
// который хранит доп параметры и собственно саму функцию-обертку), но таким образом
// можно поменять стратегию выделения памяти при необходимости.

template <typename T, typename TCH /*  = TCHAR */ >
::std::basic_string< TCH, ::std::char_traits<TCH>, ::std::allocator<TCH> >
getApiStringValue(const T &getter)
   {
    size_t size = 1024;
    TCH *buf = (TCH*)_alloca(size*sizeof(buf[0]));
    DWORD res = getter( buf, size);
    while(res && (res==size || res==(size-1)))
       {
        size *= 2;
        buf = (TCH*)_alloca(size*sizeof(buf[0]));
        res = getter( buf, size);
       }
    buf[res] = 0;

    typedef ::std::basic_string< TCH, ::std::char_traits<TCH>, ::std::allocator<TCH> > string;
    return string(buf);
   }

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/* MAKELANGID(
 *   USHORT usPrimaryLanguage,  // primary language identifier
 *   USHORT usSubLanguage       // sublanguage identifier
 * );
 * LANG_USER_DEFAULT
 */
inline
char* formatMessageA(unsigned err, char *buf, unsigned bufSize)
{
    DWORD dwChars = ::FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS
                                    , 0, err
                                    , MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
                                    , buf, bufSize, NULL 
                                    );
    if (dwChars == 0)
        return "Unknown error code";

    return buf;
}

inline
wchar_t* formatMessageW(unsigned err, wchar_t *buf, unsigned bufSize)
{
    DWORD dwChars = ::FormatMessageW( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS
                                    , 0, err
                                    , MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
                                    , buf, bufSize, NULL 
                                    );
    if (dwChars == 0)
        return L"Unknown error code";

    return buf;
}



template <typename ProcType>
ProcType getProcAddress(HMODULE hModule, const char *procName)
   {
    return (ProcType)::GetProcAddress(hModule, procName);
   }



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

// Надо бы переделать с использованием Interlocked функций для поддержки потокобезопасности
class CModule;

class CModuleRefCounted
{
    friend class CModule;

        HMODULE  hModule;
        int      refCount;

    public:

        CModuleRefCounted(HMODULE hModule) : hModule(hModule), refCount(1) {}
        int addRef()  { return ++refCount; }
        int release()         
           {
            int res = --refCount;
            if (res<=0)
               {
                if (hModule) ::FreeLibrary(hModule);
                delete this;
               }
            return res;
           }
};


// CModule - предназначен для хранения в stl-контейнерах map, vector
class CModule
{
        CModuleRefCounted *pRefCountedHModule;

    public:

        CModule(const TCHAR *modName)
           : pRefCountedHModule(0)
           { 
            pRefCountedHModule = new CModuleRefCounted(
                                                       modName 
                                                       ? ::LoadLibrary(modName)
                                                       : (HMODULE)0
                                                      );
           }

        CModule(const tstring &modName)
           : pRefCountedHModule(0)
           { 
            pRefCountedHModule = new CModuleRefCounted(
                                                       !modName.empty()
                                                       ? ::LoadLibrary(modName.c_str())
                                                       : (HMODULE)0
                                                      );
           }

        CModule(HMODULE hModule)
           : pRefCountedHModule(0)
           {
            pRefCountedHModule = new CModuleRefCounted(hModule);
           }

        CModule(const CModule &cmod)
           : pRefCountedHModule(cmod.pRefCountedHModule)
           {
            if (pRefCountedHModule) pRefCountedHModule->addRef();
           }

        CModule& operator=(const CModule &cmod)
           {
            if (&cmod==this) return *this;
            CModuleRefCounted  *pOldRefCountedHModule = pRefCountedHModule;
            pRefCountedHModule = cmod.pRefCountedHModule;
            if (pRefCountedHModule)    pRefCountedHModule->addRef();
            if (pOldRefCountedHModule) pOldRefCountedHModule->release();
            return *this;
           }

        ~CModule()
           { 
            if (pRefCountedHModule) pRefCountedHModule->release();
           }

        operator HMODULE () const
           {
            if (!pRefCountedHModule) return 0;
            return pRefCountedHModule->hModule;
           }

        bool operator!() const
           {
            if (!pRefCountedHModule) return true;
            return pRefCountedHModule->hModule==0;
           }

        template <typename ProcType>
        ProcType getProcAddress(const char *procName)
           {
            if (!pRefCountedHModule) return 0;
            return winapi::getProcAddress<ProcType>(pRefCountedHModule->hModule, procName);
           }
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// getter
struct CGetModuleFileName
{
    HMODULE hModule;
    CGetModuleFileName(HMODULE hModule) : hModule(hModule) {}
    /* DWORD operator() (TCHAR *buf, size_t size) const
     *    { return ::GetModuleFileName( hModule, buf, (DWORD)size); }
     */
    DWORD operator() (char *buf, size_t size) const
       { return ::GetModuleFileNameA( hModule, buf, (DWORD)size); }
    DWORD operator() (wchar_t *buf, size_t size) const
       { return ::GetModuleFileNameW( hModule, buf, (DWORD)size); }
};

/*
template<typename CharType>
::std::basic_string<CharType, ::std::char_traits<CharType>, ::std::allocator<CharType> >
getModuleFileName(HMODULE hModule)
   {
    return getApiStringValue<CGetModuleFileName, CharType>(CGetModuleFileName(hModule));
   }
*/
//-----------------------------------------------------------------------------
inline
void getModuleFileName(HMODULE hModule, ::std::string &filename)
   {
    filename = getApiStringValue<CGetModuleFileName, char>(CGetModuleFileName(hModule));
   }

//-----------------------------------------------------------------------------
inline
void getModuleFileName(HMODULE hModule, ::std::wstring &filename)
   {
    filename = getApiStringValue<CGetModuleFileName, wchar_t>(CGetModuleFileName(hModule));
   }

//-----------------------------------------------------------------------------
struct CGetWindowText
{
    HWND hwnd;
    CGetWindowText(HWND _hwnd) : hwnd(_hwnd) {}
    /* DWORD operator() (TCHAR *buf, size_t size) const
     *    { return ::GetModuleFileName( hModule, buf, (DWORD)size); }
     */
    DWORD operator() (char *buf, size_t size) const
       { return ::GetWindowTextA( hwnd, buf, (int)size); }
    DWORD operator() (wchar_t *buf, size_t size) const
       { return ::GetWindowTextW( hwnd, buf, (int)size); }
};
//-----------------------------------------------------------------------------
inline
void getWindowText(HWND hwnd, ::std::string &text)
   {
    text = getApiStringValue<CGetWindowText, char>(CGetWindowText(hwnd));
   }

//-----------------------------------------------------------------------------
inline
void getWindowText(HWND hwnd, ::std::wstring &text)
   {
    text = getApiStringValue<CGetWindowText, wchar_t>(CGetWindowText(hwnd));
   }

//-----------------------------------------------------------------------------
inline
void getDlgItemText(HWND hwndDlg, int ctrlId, ::std::string &text)
   {
    getWindowText( ::GetDlgItem(hwndDlg,ctrlId), text );
   }

//-----------------------------------------------------------------------------
inline
void getDlgItemText(HWND hwndDlg, int ctrlId, ::std::wstring &text)
   {
    getWindowText( ::GetDlgItem(hwndDlg,ctrlId), text );
   }

//-----------------------------------------------------------------------------
inline
BOOL setWindowText( HWND hwnd, const ::std::string &text )
   {
    return ::SetWindowTextA( hwnd, text.c_str() );
   }

//-----------------------------------------------------------------------------
inline
BOOL setWindowText( HWND hwnd, const ::std::wstring &text )
   {
    return ::SetWindowTextW( hwnd, text.c_str() );
   }

inline
BOOL clrWindowText( HWND hwnd )
   {
    return ::SetWindowTextA( hwnd, "" );
   }

//-----------------------------------------------------------------------------
inline
BOOL setDlgItemText( HWND hwndDlg, int ctrlId, const ::std::string &text )
   {
    return ::SetWindowTextA( ::GetDlgItem(hwndDlg,ctrlId), text.c_str() );
   }

//-----------------------------------------------------------------------------
inline
BOOL setDlgItemText( HWND hwndDlg, int ctrlId, const ::std::wstring &text )
   {
    return ::SetWindowTextW( ::GetDlgItem(hwndDlg,ctrlId), text.c_str() );
   }

//-----------------------------------------------------------------------------
inline
BOOL clrDlgItemText( HWND hwndDlg, int ctrlId )
   {
    return clrWindowText( ::GetDlgItem(hwndDlg,ctrlId) );
   }


/*
#define BST_UNCHECKED      0x0000
#define BST_CHECKED        0x0001
#define BST_INDETERMINATE  0x0002
#define BST_PUSHED         0x0004
#define BST_FOCUS          0x0008
*/

inline
int getCheck( HWND hwndButton )
   {
    return (int)::SendMessage(hwndButton, BM_GETCHECK, 0, 0L);
   }

inline
int getCheck( HWND hwndDlg, int ctrlId )
   {
    return getCheck( ::GetDlgItem(hwndDlg,ctrlId) );
   }

// alias for getCheck
inline
int getDlgItemCheck( HWND hwndDlg, int ctrlId )
   {
    return getCheck(hwndDlg, ctrlId);
   }

inline
int getDlgRadioGroupCheck(HWND hwndDlg, int ctrlIdFirst, int numOfRadios)
   {
    for(int i=0; i!=numOfRadios; ++i)
       if (getDlgItemCheck(hwndDlg, ctrlIdFirst+i)==BST_CHECKED) return i;
    return -1;
   }

inline
void setCheck( HWND hwndButton, int checkState )
   {
    ::SendMessage(hwndButton, BM_SETCHECK, checkState, 0L);
   }

inline
void setCheck( HWND hwndDlg, int ctrlId, int checkState )
   {
    setCheck( ::GetDlgItem(hwndDlg,ctrlId), checkState );
   }

// alias for setCheck
inline
void setDlgItemCheck( HWND hwndDlg, int ctrlId, int checkState )
   {
    setCheck(hwndDlg, ctrlId, checkState);
   }

inline
void setDlgRadioGroupCheck(HWND hwndDlg, int ctrlIdFirst, int checkIdx)
   {
    if (checkIdx<0) return;
    //for(i=0; i!=numOfRadios; ++i)
    //   if (getDlgItemCheck(hwndDlg, ctrlIdFirst+i)==BST_CHECKED) return i;
    //return -1;
    setDlgItemCheck( hwndDlg, ctrlIdFirst+checkIdx, BST_CHECKED );
   }


inline
void enableWindow( HWND hwnd, BOOL bEnable )
   {
    ::EnableWindow(hwnd, bEnable);
   }

inline
void enableDlgItem( HWND hwndDlg, int ctrlId, BOOL bEnable )
   {
    enableWindow( ::GetDlgItem(hwndDlg,ctrlId), bEnable );
   }

inline
void enableDlgItems( HWND hwndDlg, const int *pItems, SIZE_T itemsNum, BOOL bEnable )
   {
    const int *pItemsEnd = pItems + itemsNum;
    for(; pItems!=pItemsEnd; ++pItems)
       {
        enableDlgItem( hwndDlg, *pItems, bEnable );
       }
   }


inline
HWND setFocusDlgItem( HWND hwndDlg, int ctrlId )
   {
    return ::SetFocus( ::GetDlgItem(hwndDlg,ctrlId) );
   }

inline
int getFocusDlgItem( HWND hwndDlg )
   {
    HWND hwndFocus = ::GetFocus();
    if (!hwndFocus) return -1;
    HWND hwndParent = ::GetParent( hwndFocus );
    if (hwndParent!=hwndDlg) return -1;
    //return reinterpret_cast<int>(reinterpret_cast<INT_PTR>(::GetMenu( hwndFocus )));
    return PtrToLong( ::GetMenu( hwndFocus ) );
   }




inline
bool getUserName( ::std::string &name )
   {
    char buf[1024];
    DWORD size = sizeof(buf)/sizeof(buf[0]);
    if (!::GetUserNameA( buf, &size)) return false;
    name = ::std::string(buf);
    return true;
   }

//-----------------------------------------------------------------------------
inline
bool getUserName( ::std::wstring &name )
   {
    wchar_t buf[1024];
    DWORD size = sizeof(buf)/sizeof(buf[0]);
    if (!::GetUserNameW( buf, &size)) return false;
    name = ::std::wstring(buf);
    return true;
   }

//-----------------------------------------------------------------------------
inline
tstring getSystemDirectory()
   {
    TCHAR ch = 0;
    TCHAR *buf = &ch;
    DWORD size = ::GetSystemDirectory(buf, 1);
    if (!size) return tstring();
    buf = (TCHAR*)_alloca(size*sizeof(TCHAR));
    ::GetSystemDirectory(buf, size);
    return tstring(buf);
   }

//-----------------------------------------------------------------------------
inline
void getSystemDirectory(::std::string &dir)
   {
    char ch = 0;
    char *buf = &ch;
    DWORD size = ::GetSystemDirectoryA(buf, 1);
    if (!size) { dir = ::std::string(); return; }
    buf = (char*)_alloca(size*sizeof(char));
    ::GetSystemDirectoryA(buf, size);
    dir = ::std::string(buf);
   }
//-----------------------------------------------------------------------------
inline
void getSystemDirectory(::std::wstring &dir)
   {
    wchar_t ch = 0;
    wchar_t *buf = &ch;
    DWORD size = ::GetSystemDirectoryW(buf, 1);
    if (!size) { dir = ::std::wstring(); return; }
    buf = (wchar_t*)_alloca(size*sizeof(wchar_t));
    ::GetSystemDirectoryW(buf, size);
    dir = ::std::wstring(buf);
   }
//-------------------------------------------------------------------- 
inline
tstring getWindowsDirectory()
   {
    TCHAR  ch = 0;
    TCHAR *buf = &ch;
    DWORD size = ::GetWindowsDirectory(buf, 1);
    if (!size) return tstring();
    buf = (TCHAR*)_alloca(size*sizeof(TCHAR));
    ::GetWindowsDirectory(buf, size);
    return tstring(buf);
   }
//-------------------------------------------------------------------- 
inline
void getWindowsDirectory(::std::string &dir)
   {
    char  ch = 0;
    char *buf = &ch;
    DWORD size = ::GetWindowsDirectoryA(buf, 1);
    if (!size) { dir = ::std::string(); return; }
    buf = (char*)_alloca(size*sizeof(char));
    ::GetWindowsDirectoryA(buf, size);
    dir = ::std::string(buf);
   }
//-------------------------------------------------------------------- 
inline
void getWindowsDirectory(::std::wstring &dir)
   {
    wchar_t  ch = 0;
    wchar_t *buf = &ch;
    DWORD size = ::GetWindowsDirectoryW(buf, 1);
    if (!size) { dir = ::std::wstring(); return; }
    buf = (wchar_t*)_alloca(size*sizeof(wchar_t));
    ::GetWindowsDirectoryW(buf, size);
    dir = ::std::wstring(buf);
   }
//-------------------------------------------------------------------- 
inline
tstring getCurrentDirectory()
   {
    TCHAR  ch = 0;
    TCHAR *buf = &ch;
    DWORD size = ::GetCurrentDirectory(1, buf);
    if (!size) return tstring();
    buf = (TCHAR*)_alloca(size*sizeof(TCHAR));
    ::GetCurrentDirectory(size, buf);
    return tstring(buf);
   }

//-------------------------------------------------------------------- 
inline
void getCurrentDirectory(::std::string &dir)
   {
    char  ch = 0;
    char *buf = &ch;
    DWORD size = ::GetCurrentDirectoryA(1, buf);
    if (!size) { dir = ::std::string(); return; }
    buf = (char*)_alloca(size*sizeof(char));
    ::GetCurrentDirectoryA(size, buf);
    dir = ::std::string(buf);
   }

//-------------------------------------------------------------------- 
inline
void getCurrentDirectory(::std::wstring &dir)
   {
    wchar_t  ch = 0;
    wchar_t *buf = &ch;
    DWORD size = ::GetCurrentDirectoryW(1, buf);
    if (!size) { dir = ::std::wstring(); return; }
    buf = (wchar_t*)_alloca(size*sizeof(wchar_t));
    ::GetCurrentDirectoryW(size, buf);
    dir = ::std::wstring(buf);
   }

//-------------------------------------------------------------------- 
inline
BOOL setCurrentDirectory(const tstring &dirName)
   {
    if (dirName.empty())
       {
        //SetLastError(ERROR_CANNOT_MAKE);
        SetLastError(ERROR_INVALID_NAME);
        return FALSE;
       }
    
    return ::SetCurrentDirectory(dirName.c_str());
   }


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
inline
tstring makeUnicodeLongFilename(const tstring &name)
   {
    if (name.size()>=3 && (tstring(name, 0, 3)==_T("\\\\.") || tstring(name, 0, 3)==_T("\\\\?")) )
       {
        // already long name or device name (such as \\.\COMX)
        return name; // return untouched name
       }

    const TCHAR* sepList = _T("/\\");
    const TCHAR sep = _T('\\');

    if (filename::isDriveAbsolutePath(name, sepList)) // make \\?\D:\<path> 
       return tstring(_T("\\\\?\\")) + filename::makeCanonical(name, sepList, sep );

    if (filename::isUncAbsolutePath(name, sepList)) // make \\?\UNC\Server\Share from \\Server\Share
       return tstring(_T("\\\\?\\UNC")) + tstring(filename::makeCanonical(name, sepList, sep ), 1, tstring::npos);

    return name; // other names can't be longer than 260 chars
   }

//-----------------------------------------------------------------------------
inline
tstring makeFilenameFromLongUnicode(const tstring &name)
   {
    const TCHAR* sepList = _T("/\\");
    const TCHAR sep = _T('\\');
    tstring canon = filename::makeCanonical(name, sepList, sep );

    tstring uncPrefix(_T("\\\\?\\UNC"));
    tstring lnPrefix(_T("\\\\?\\"));

    if (!canon.compare(0, uncPrefix.size(), uncPrefix, 0, uncPrefix.size()))
       { // match
        tstring res = _T("\\");
        res.append(canon, uncPrefix.size(), canon.size() - uncPrefix.size());
        return res;
       }
    if (!canon.compare(0, lnPrefix.size(), lnPrefix, 0, lnPrefix.size()))
       { // match
        return tstring(canon, lnPrefix.size());
       }
    return name;
   }

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-------------------------------------------------------------------- 
//-------------------------------------------------------------------- 
//-------------------------------------------------------------------- 
inline
void fileTimeToUlargeInteger( const FILETIME * const pf, ULARGE_INTEGER *pu)
   {
    pu->LowPart  = pf->dwLowDateTime;
    pu->HighPart = pf->dwHighDateTime;
   }

//-------------------------------------------------------------------- 
inline
BOOL getFileTime( 
                  HANDLE hFile,
                  LPFILETIME lpCreationTime,
                  LPFILETIME lpLastAccessTime,
                  LPFILETIME lpLastWriteTime
                )
   {
    return ::GetFileTime( hFile, lpCreationTime, 
                          lpLastAccessTime, lpLastWriteTime);
   }

//-------------------------------------------------------------------- 
inline
BOOL getFileTime(
                 HANDLE hFile,
                 PULARGE_INTEGER lpCreationTime,
                 PULARGE_INTEGER lpLastAccessTime,
                 PULARGE_INTEGER lpLastWriteTime
                )
   {
    FILETIME creationTime, lastAccessTime, lastWriteTime;
    BOOL res = ::GetFileTime( hFile, &creationTime, &lastAccessTime, &lastWriteTime);
    if (!res) return res;

    if (lpCreationTime) fileTimeToUlargeInteger( &creationTime, lpCreationTime);
    if (lpLastAccessTime) fileTimeToUlargeInteger( &lastAccessTime, lpLastAccessTime);
    if (lpLastWriteTime) fileTimeToUlargeInteger( &lastWriteTime, lpLastWriteTime);
    
    return TRUE;
   }

//-------------------------------------------------------------------- 
inline
BOOL getFileTime(
                 const tstring &filename,
                 LPFILETIME lpCreationTime,
                 LPFILETIME lpLastAccessTime,
                 LPFILETIME lpLastWriteTime
                )
   {
    HANDLE hFile = CreateFile( 
                               #ifndef WINAPI_NO_UNICODE_LONG_FILENAMES_SUPPORT
                               makeUnicodeLongFilename(filename).c_str(),
                               #else
                               filename.c_str(), 
                               #endif
                               GENERIC_READ, 
                               FILE_SHARE_READ|FILE_SHARE_WRITE, 0, 
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

    if (hFile==INVALID_HANDLE_VALUE) return false;
    BOOL res = getFileTime( hFile, lpCreationTime, lpLastAccessTime, lpLastWriteTime);
    CloseHandle(hFile);
    return res;
   }

//-------------------------------------------------------------------- 
inline
BOOL getFileTime(
                 const tstring &filename,
                 PULARGE_INTEGER lpCreationTime,
                 PULARGE_INTEGER lpLastAccessTime,
                 PULARGE_INTEGER lpLastWriteTime
                )
   {
    HANDLE hFile = CreateFile( 
                               #ifndef WINAPI_NO_UNICODE_LONG_FILENAMES_SUPPORT
                               makeUnicodeLongFilename(filename).c_str(),
                               #else
                               filename.c_str(), 
                               #endif
                               GENERIC_READ, 
                               FILE_SHARE_READ|FILE_SHARE_WRITE, 0, 
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

    if (hFile==INVALID_HANDLE_VALUE) return false;
    BOOL res = getFileTime( hFile, lpCreationTime, lpLastAccessTime, lpLastWriteTime);
    CloseHandle(hFile);
    return res;
   }

//-------------------------------------------------------------------- 
inline
BOOL createDirectory(const tstring &dirName, LPSECURITY_ATTRIBUTES lpSecurityAttributes = 0)
   {
    if (dirName.empty())
       {
        //SetLastError(ERROR_CANNOT_MAKE);
        SetLastError(ERROR_INVALID_NAME);
        return FALSE;
       }
    
    return ::CreateDirectory(
                             #ifndef WINAPI_NO_UNICODE_LONG_FILENAMES_SUPPORT
                             makeUnicodeLongFilename(dirName).c_str(),
                             #else
                             dirName.c_str(), 
                             #endif
                             lpSecurityAttributes
                            );
   }

//-------------------------------------------------------------------- 
inline
BOOL forceCreateDirectory(const tstring &dirName, LPSECURITY_ATTRIBUTES lpSecurityAttributes = 0)
   {
    tstring canonDirName = MARTY_NS filename::makeCanonical(dirName);
    ::std::vector<tstring> pathParts;
    MARTY_NS filename::splitPath( canonDirName, pathParts );
    ::std::vector<tstring>::const_iterator pit = pathParts.begin();

    tstring newName;
    for( ; pit!=pathParts.end(); ++pit)
       {
        MARTY_NS filename::appendPathInplace(newName, *pit);
        if (!createDirectory(newName))
           {
            DWORD err = ::GetLastError();
            if (err==ERROR_ACCESS_DENIED && pit==pathParts.begin())
               continue;
            if (err==ERROR_ALREADY_EXISTS)
               continue;
            return FALSE;
           }
       }
    return TRUE;
   }

//QuadPart




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


/* #ifndef CSIDL_FLAG_CREATE
 *     #define CSIDL_FLAG_CREATE               0x8000        // combine with CSIDL_ value to force folder creation in SHGetFolderPath()
 * #endif
 *  
 * #ifndef CSIDL_FLAG_DONT_VERIFY
 *     #define CSIDL_FLAG_DONT_VERIFY          0x4000        // combine with CSIDL_ value to return an unverified folder path
 * #endif
 *  
 * #ifndef CSIDL_FLAG_MASK
 *     #define CSIDL_FLAG_MASK                 0xFF00        // mask for all possible flag values
 * #endif
 */

/* typedef HRESULT (__stdcall * PFNSHGETFOLDERPATHA)(HWND, int, HANDLE, DWORD, LPSTR);  // "SHGetFolderPathA"
 * typedef HRESULT (__stdcall * PFNSHGETFOLDERPATHW)(HWND, int, HANDLE, DWORD, LPWSTR); // "SHGetFolderPathW"
 *  
 * #ifdef UNICODE
 * #define SHGetFolderPath     SHGetFolderPathW
 * #define PFNSHGETFOLDERPATH  PFNSHGETFOLDERPATHW
 * #else
 * #define SHGetFolderPath     SHGetFolderPathA
 * #define PFNSHGETFOLDERPATH  PFNSHGETFOLDERPATHA
 * #endif
 */

#if !defined(WINAPI_NO_SHFOLDER)

#define SHGETFOLDERPATHA_PROCNAME "SHGetFolderPathA"
#define SHGETFOLDERPATHW_PROCNAME "SHGetFolderPathW"

#if defined(_UNICODE) || defined(UNICODE)
    #define SHGETFOLDERPATH_PROCNAME  SHGETFOLDERPATHW_PROCNAME
#else
    #define SHGETFOLDERPATH_PROCNAME  SHGETFOLDERPATHA_PROCNAME
#endif


// Обертка для ShGetFolderPath - будет возвращать false на старых системах,
// где нет такой функции, 
inline
bool shGetFolderPath( HWND hwndOwner,
                      int nFolder,
                      tstring &path,
                      DWORD dwFlags
                    )
   {
    CModule mod(_T("shell32.dll"));

    PFNSHGETFOLDERPATH proc = mod.getProcAddress<PFNSHGETFOLDERPATH>(SHGETFOLDERPATH_PROCNAME);
    if (!proc) return false;

    TCHAR buf[MAX_PATH];
    HRESULT hRes = proc(hwndOwner, nFolder, 0, dwFlags, buf);
    if (hRes==S_FALSE) return false;
    if (hRes==E_FAIL) return false;
    if (hRes==E_INVALIDARG ) return false;

    if (hRes==S_OK)
    //if (S_OK==proc(hwndOwner, nFolder, 0, dwFlags, buf))
       {
        path = buf;
        return true;
       }

    return false;
   }

#endif /* WINAPI_NO_SHFOLDER */


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------




template< typename TEnumerator >
struct CThreadWindowEnumeratorConstImpl
{
    static
    BOOL CALLBACK EnumProc( HWND hwnd, LPARAM lParam )
       {
        const TEnumerator* pEnum = (const TEnumerator*)lParam;
        return pEnum->operator()(hwnd) ? TRUE : FALSE;       
       }
};

template< typename TEnumerator >
struct CThreadWindowEnumeratorImpl
{
    static
    BOOL CALLBACK EnumProc( HWND hwnd, LPARAM lParam )
       {
        TEnumerator* pEnum = (TEnumerator*)lParam;
        return pEnum->operator()(hwnd) ? TRUE : FALSE;       
       }
};

template< typename TEnumerator >
bool enumThreadWindows( DWORD dwThreadId, const TEnumerator &enumerator )
   {
    //CThreadWindowEnumeratorConstImpl<TEnumerator> enumeratorImpl;
    return ::EnumThreadWindows( dwThreadId
                              , CThreadWindowEnumeratorConstImpl<TEnumerator>::EnumProc
                              ,(LPARAM)&enumerator
                              ) ? true : false;
   }

template< typename TEnumerator >
bool enumThreadWindows( DWORD dwThreadId, TEnumerator &enumerator )
   {
    //CThreadWindowEnumeratorConstImpl<TEnumerator> enumeratorImpl;
    return ::EnumThreadWindows( dwThreadId
                              , CThreadWindowEnumeratorImpl<TEnumerator>::EnumProc
                              ,(LPARAM)&enumerator
                              ) ? true : false;
   }






//-----------------------------------------------------------------------------
#if defined(UNICODE) || defined(_UNICODE)
     // do not touch WINAPI_NO_UNICODE_LONG_FILENAMES_SUPPORT
    //#if defined(WINAPI_DISABLE_UNICODE_LONG_FILENAMES_SUPPORT)
#else /* ANSI verison compiled, force disable lfn support */
    #if !defined(WINAPI_NO_UNICODE_LONG_FILENAMES_SUPPORT)
        #define WINAPI_NO_UNICODE_LONG_FILENAMES_SUPPORT
    #endif
#endif
    
//-----------------------------------------------------------------------------
    



//-----------------------------------------------------------------------------
// FindFirstFile/FindNextFile wrappers
//-----------------------------------------------------------------------------


/* Sample handler
 * struct CPrintFileFindData
 * {
 *     bool operator()(tstring &path, const WIN32_FIND_DATA &fndData)
 *        {
 *         std::cout<<getAttrString(fndData.dwFileAttributes)<<"  "<<fndData.cFileName<<"\n";
 *         return true;
 *        }
 * }
 */


//-----------------------------------------------------------------------------
// поиск файла по маске
// базовая версия обертка для WinAPI функций FindFirstFile/FindNextFile
template <typename TFindHandler>
DWORD findFile(const tstring path, const tstring mask, TFindHandler handler)
   {
    tstring pathMask = filename::appendPath(path,  
                                            (mask.empty() ? tstring(_T("*")) : mask),
                                            _T("/\\"), _T('\\')
                                           );
    #ifndef WINAPI_NO_UNICODE_LONG_FILENAMES_SUPPORT
    pathMask = makeUnicodeLongFilename(pathMask);
    #endif

    WIN32_FIND_DATA fndData;
    HANDLE hFind = ::FindFirstFile( pathMask.c_str(), &fndData );
    if (hFind==INVALID_HANDLE_VALUE) return ::GetLastError();

    /* if (!handler(path, fndData))
     *    { 
     *     CloseHandle(hFind);
     *     return 0;
     *    }
     */

    do{
       if (!handler(path, fndData))
          break;
      } while(::FindNextFile( hFind, &fndData ));

    /* while(::FindNextFile( hFind, &fndData ))
     *    {
     *     if (!handler(path, fndData))
     *        break;
     *    }
     */

    ::FindClose(hFind);
    return 0;
   }

//-----------------------------------------------------------------------------
// Borland C++ can't convert char to tstring
//#ifdef __BORLANDC__
/* template <typename TFindHandler>
 * DWORD findFile(const TCHAR* path, const tstring &mask, TFindHandler handler)
 *    {
 *     findFile(tstring(path), mask, handler);
 *    }
 * template <typename TFindHandler>
 * DWORD findFile(const tstring &path, const TCHAR* &mask, TFindHandler handler)
 *    {
 *     findFile(path, tstring(mask), handler);
 *    }
 * template <typename TFindHandler>
 * DWORD findFile(const TCHAR* path, const TCHAR* mask, TFindHandler handler)
 *    {
 *     findFile(tstring(path), tstring(mask), handler);
 *    }
 */
//#endif

//-----------------------------------------------------------------------------
template <typename TFindHandler>
DWORD findFile(const tstring pathMask, TFindHandler handler)
   {
    return findFile( MARTY_FILENAME_NS getPath( pathMask /* , _T("/\\") */ )
                   , MARTY_FILENAME_NS getFile( pathMask /* , _T("/\\") */ )
                   , handler
                   );
    //return findFile(filename::path( pathMask, _T("/\\")), filename::file( pathMask, _T("/\\")), handler);
   }

/* //#ifdef __BORLANDC__
 * template <typename TFindHandler>
 * DWORD findFile(const TCHAR* pathMask, TFindHandler handler)
 *    {
 *     return findFile(filename::path( pathMask, _T("/\\")), filename::file( pathMask, _T("/\\")), handler);
 *    }
 * //#endif
 */

//-----------------------------------------------------------------------------

namespace utils
{

struct CFindCounter
{
    int &count;
    CFindCounter(int &c) : count(c) {}
    bool operator()(const tstring &path, const WIN32_FIND_DATA &fndData)
       { ++count; return true; }

};

}; // namespace utils


//-----------------------------------------------------------------------------
inline
int fileExist(const tstring &pathName )
   {
    //HANDLE hFile
    /*
    int counter = 0;
    findFile<utils::CFindCounter>(pathName, utils::CFindCounter(counter));
    //winapi::findFile<winapi::utils::CFindCounter>(pathName, winapi::utils::CFindCounter(counter));
    //winapi::findFile<winapi::utils::CFindCounter>(winapi::utils::CFindCounter(counter), pathName, tstring(_T("*")));
    return counter;
    */
    if (pathName.empty()) return 0;

    HANDLE hFile = CreateFile( 
                               #ifndef WINAPI_NO_UNICODE_LONG_FILENAMES_SUPPORT
                               makeUnicodeLongFilename(pathName).c_str(),
                               #else
                               pathName.c_str(), 
                               #endif
                               GENERIC_READ, 
                               FILE_SHARE_READ|FILE_SHARE_WRITE, 0, 
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
    DWORD err = GetLastError();
    CloseHandle(hFile);
    if (hFile!=INVALID_HANDLE_VALUE)
       return 1; // no error, file exist and readable

    if (err==ERROR_SHARING_VIOLATION)
       return 1; // file exist, but share access mode invalid

    //ERROR_FILE_NOT_FOUND
    return 0;
   }

//-----------------------------------------------------------------------------
// return number of found files
/* mask allowed */
inline
int maskExist(const tstring &pathName )
   {
    int counter = 0;
    findFile<utils::CFindCounter>(pathName, utils::CFindCounter(counter));
    //winapi::findFile<winapi::utils::CFindCounter>(pathName, winapi::utils::CFindCounter(counter));
    //winapi::findFile<winapi::utils::CFindCounter>(winapi::utils::CFindCounter(counter), pathName, tstring(_T("*")));
    return counter;
   }


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

namespace utils
{

//-----------------------------------------------------------------------------
struct removeTralingSlash
{
    void operator() (tstring &path)
       { filename::pathRemoveTrailingSlashInplace(path, _T("/\\")); }
};

//-----------------------------------------------------------------------------
// Добавляет путь в вектор, если там еще такого нет
inline
void addPathIfNotFound(std::vector<tstring> &pathList, const tstring& path, const tstring& subDir, const std::vector<tstring> *pExcludePathList)
   {
    tstring newPath = filename::appendPath(path, subDir, _T("/\\"), _T('\\'));

    std::vector<tstring>::const_iterator it = std::find_if(
                                                     pathList.begin(),
                                                     pathList.end(),
                                                     filename::compareTo<filename::compareProcType>(newPath, filename::equal)
                                                    );
    if (it!=pathList.end())
       return; // path allready in list

    if (pExcludePathList)
       {
        it = std::find_if(
                          pExcludePathList->begin(),
                          pExcludePathList->end(),
                          filename::compareTo<filename::compareProcType>(newPath, filename::equal)
                         );
        if (it!=pExcludePathList->end())
           return; // path found in exclude list
       }

    pathList.push_back(newPath);
   }

//-----------------------------------------------------------------------------
template <typename TH>
struct CFindFilesRecurceProxy
{
    TH                    handler;
    bool                  recurceAllowed;
    bool                  buildDirListMode;
    bool                  callHandlerAllowed;
    std::vector<tstring>  &pathList;
    const std::vector<tstring> * pExcludePathList;
    bool                  &breakFlag;

    CFindFilesRecurceProxy( TH h, 
                            std::vector<tstring> &pl,
                            const std::vector<tstring> *pEx,
                            bool bRecurce,
                            bool bdl,
                            bool cha,
                            bool &brf)
       : handler(h)
       , pathList(pl)
       , pExcludePathList(pEx)
       , recurceAllowed(bRecurce)
       , buildDirListMode(bdl)
       , callHandlerAllowed(cha)
       , breakFlag(brf)
       { }

    void setup( bool bRecurce, bool bdl, bool cha)
       {
        recurceAllowed     = bRecurce;
        buildDirListMode    = bdl;
        callHandlerAllowed = cha;
        //breakFlag = false;
       }


    bool operator()(const tstring &path, const WIN32_FIND_DATA &fndData)
       {
        if ((fndData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) && recurceAllowed && buildDirListMode)
           {
            if (fndData.cFileName!=tstring(_T(".")) && fndData.cFileName!=tstring(_T("..")))
               addPathIfNotFound( pathList, path, fndData.cFileName, pExcludePathList);
            //std::cout<<"*** PathList size: "<<(unsigned)pathList.size()<<"\n";
           }

        if (callHandlerAllowed)
           {
            if (!handler(path, fndData))
               {
                breakFlag = true;
                return false;
               }
           }
        return true;
       }
}; // struct CFindFilesRecurceProxy

}; // namespace utils

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// рекурсивный поиск по маске. вспомогательная функция
// если маска задана отличной от *, то сначала производится поиск каталогов по маске *
template <typename TFindHandler>
DWORD findFileAux( const std::vector<tstring> &pathList, 
                   const std::vector<tstring> *pExcludePathList, 
                   const tstring &mask, 
                   TFindHandler handler, 
                   bool traverseSubFolders)
   {
    std::vector<tstring> pathListCopy = pathList;
    std::for_each(pathListCopy.begin(), pathListCopy.end(), utils::removeTralingSlash());

    bool breakFlag = false;
    utils::CFindFilesRecurceProxy<TFindHandler> proxy(handler, pathListCopy, pExcludePathList, true, true, false, breakFlag);

    if (mask!=_T("*") && traverseSubFolders)
       {
        std::vector<tstring>::size_type i = 0;
        for(; i<pathListCopy.size(); ++i)
            findFile(pathListCopy[i], _T("*"), proxy);
       }

    proxy.setup( traverseSubFolders, mask==_T("*") && traverseSubFolders, true);
    std::vector<tstring>::size_type i = 0;
    for(; i<pathListCopy.size(); ++i)
       {
        findFile( pathListCopy[i], mask, proxy);
        if (breakFlag) return 0;
       }

    return 0;
   }

// 
template <typename TFindHandler>
DWORD findFile( const std::vector<tstring> &pathList, 
                const std::vector<tstring> &excludePathList, 
                const tstring &mask, 
                TFindHandler handler, 
                bool traverseSubFolders)
   {
    return findFileAux( pathList, &excludePathList, mask, handler, traverseSubFolders);
   }

template <typename TFindHandler>
DWORD findFile( const std::vector<tstring> &pathList, 
                const std::vector<tstring> &excludePathList, 
                const TCHAR *mask, 
                TFindHandler handler, 
                bool traverseSubFolders)
   {
    return findFileAux( pathList, &excludePathList, tstring(mask), handler, traverseSubFolders);
   }

template <typename TFindHandler>
DWORD findFile( const std::vector<tstring> &pathList, 
                const tstring &mask, 
                TFindHandler handler, 
                bool traverseSubFolders)
   {
    return findFileAux( pathList, 0, mask, handler, traverseSubFolders);
   }

template <typename TFindHandler>
DWORD findFile( const std::vector<tstring> &pathList, 
                const TCHAR *mask, 
                TFindHandler handler, 
                bool traverseSubFolders)
   {
    return findFileAux( pathList, 0, tstring(mask), handler, traverseSubFolders);
   }

/*  
 *  
 * template <typename TFindHandler>
 * DWORD findFile(const std::vector<tstring> &pathList, const TCHAR *mask, TFindHandler handler, bool traverseSubFolders = true)
 *    {
 *     return findFile(pathList, tstring(mask), handler, traverseSubFolders);
 *    }
 */


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
struct CWin32EnumDrivesData
{
    TCHAR     driveName[4];
    UINT      driveType;

    BOOL      driveInfoValid;
    TCHAR     volumeName[_MAX_PATH];
    DWORD     volumeSerial;
    DWORD     maxComponentLength;
    DWORD     filesystemFlags;
    TCHAR     filesystemName[_MAX_PATH];

    BOOL      spaceInfoValid;
    DWORD     sectorsPerCluster;
    DWORD     bytesPerSector;
    DWORD     numOfFreeClusters;
    DWORD     numOfTotalClusters;

};


// void Handler(const CWin32EnumDrivesData &enumData)

template <typename THandler>
void enumLogicalDrives(THandler handler)
   {
    CWin32EnumDrivesData enumData;
    //tstrcpy(enumData.driveName, _T("C:\\"));
    _tcscpy(enumData.driveName, _T("C:\\"));

    DWORD logicalDrivesMask = ::GetLogicalDrives();
    for(TCHAR driveLetter=_T('A'); driveLetter<=_T('Z'); ++driveLetter, logicalDrivesMask>>=1)
       {
        if (!(logicalDrivesMask&1)) continue;
        enumData.driveName[0] = driveLetter;
        enumData.driveType = ::GetDriveType(enumData.driveName);

        enumData.driveInfoValid = ::GetVolumeInformation( enumData.driveName,
                                                          enumData.volumeName,
                                                          DWORD(sizeof(enumData.volumeName)/sizeof(enumData.volumeName[0])),
                                                          &enumData.volumeSerial,
                                                          &enumData.maxComponentLength,
                                                          &enumData.filesystemFlags,
                                                          enumData.filesystemName,
                                                          DWORD(sizeof(enumData.filesystemName)/sizeof(enumData.filesystemName[0]))
                                                        );
        enumData.spaceInfoValid = ::GetDiskFreeSpace( enumData.driveName,
                                                      &enumData.sectorsPerCluster,
                                                      &enumData.bytesPerSector,
                                                      &enumData.numOfFreeClusters,
                                                      &enumData.numOfTotalClusters
                                                    );
        handler(enumData);
       }
   
   }





}; // namespace winapi

#ifdef WINAPI_IN_MARTY_NAMESPACE
}; // namespace marty
#endif



#endif /* MARTY_WINAPI_H */

