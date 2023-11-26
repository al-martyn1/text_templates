//----------------------------------------------------------)
//
//  Util.cpp
//
//  Designed by Andrey Martynov
//  mailto:rek@.rsdn.ru
//
//----------------------------------------------------------)


#if !defined(_INC_TCHAR) && !defined(_TCHAR_H_) && !defined(_TCHAR_H)
    #include <tchar.h>
#endif

#if !defined(_FUNCTIONAL_) && !defined(_STLP_FUNCTIONAL) && !defined(__STD_FUNCTIONAL__) && !defined(_CPP_FUNCTIONAL) && !defined(_GLIBCXX_FUNCTIONAL)
    #include <functional>
#endif


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#endif

//#include <afx.h>
//#include <afxwin.h>         // MFC core and standard components
//#include <afxext.h>         // MFC extensions
//#include <afxdtctl.h>     // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>           // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit


#ifndef __ATLBASE_H__
    #include <atlbase.h>
#endif


#if !defined(_VECTOR_) && !defined(_STLP_VECTOR) && !defined(__STD_VECTOR__) && !defined(_CPP_VECTOR) && !defined(_GLIBCXX_VECTOR)
    #include <vector>
#endif

#if !defined(_MAP_) && !defined(_STLP_MAP) && !defined(__STD_MAP__) && !defined(_CPP_MAP) && !defined(_GLIBCXX_MAP)
    #include <map>
#endif

#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

#if !defined(_MEMORY_) && !defined(_STLP_MEMORY) && !defined(__STD_MEMORY__) && !defined(_CPP_MEMORY) && !defined(_GLIBCXX_MEMORY)
    #include <memory>
#endif

#if !defined(_ALGORITHM_) && !defined(_STLP_ALGORITHM) && !defined(__STD_ALGORITHM) && !defined(_CPP_ALGORITHM) && !defined(_GLIBCXX_ALGORITHM)
    #include <algorithm>
#endif

#if !defined(_INC_MALLOC) && !defined(_MALLOC_H_) && !defined(_MALLOC_H)
    #include <malloc.h>
#endif

#if !defined(_INC_FLOAT) && !defined(_FLOAT_H_) && !defined(_FLOAT_H)
    #include <float.h>
#endif

#if !defined(_EXCEPTION_) && !defined(__EXCEPTION__) && !defined(_STLP_EXCEPTION) && !defined(__STD_EXCEPTION)
    #include <exception>
#endif

#if !defined(_STDEXCEPT_) && !defined(_STLP_STDEXCEPT) && !defined(__STD_STDEXCEPT) && !defined(_CPP_STDEXCEPT) && !defined(_GLIBCXX_STDEXCEPT)
    #include <stdexcept>
#endif


//#include "stdafx.h"
#include "SerializerUtils.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

#ifndef _CONSOLE
#define _CONSOLE
#endif

namespace Rsdn  {
namespace Utils {

//-----------------------------------------------------------------------------
tstring ErrorInfo()
{
    tstring str;
    try
    {
        CComPtr<IErrorInfo> pErrorInfo;
        HRESULT hr = ::GetErrorInfo(0, &pErrorInfo);
        if (hr == S_FALSE)
            return _T("No ErrorInfo");
        if (hr == S_OK)
        {
            CComBSTR bstrDescr;
            if (FAILED(pErrorInfo->GetDescription(&bstrDescr)))
                throw std::exception();
            str += /*_T(" Description: ") + */ tstring(CW2T(bstrDescr));

            CComBSTR bstrSource;
            if (FAILED(pErrorInfo->GetSource(&bstrSource)))
                throw std::exception();
            //str += _T(", Source: ") + tstring(CW2T(bstrSource));
            str += tstring(_T(""), 1);
            tstring::size_type rpos = str.find(_T('\r'));
            while(rpos!=str.npos)
                {
                 str.erase(rpos, 1);
                 rpos = str.find(_T('\r'), rpos);
                }
            /*
            rpos = str.find(_T('\n'));
            while(rpos!=str.npos)
                {
                 str.erase(rpos, 1);
                 rpos = str.find(_T('\n'), rpos);
                }
            */
        }
    }
    catch (...)
    {
    }

    #ifdef _CONSOLE
    char *tmp = (char*)_alloca(str.size()+1);
    //str.copy(tmp, str.size()); tmp[str.size()] = 0;
    CharToOemBuff( str.c_str(), tmp, str.size());
    str = tmp;
    #endif

    return str;
}

//-----------------------------------------------------------------------------
tstring ErrorMsg(uint errorCode)
{
    tchar szMsg[1024] = "";
    DWORD dwChars = ::FormatMessage( 
                          FORMAT_MESSAGE_FROM_SYSTEM
                        | FORMAT_MESSAGE_IGNORE_INSERTS
                        , 0, errorCode
                        , MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
                        , szMsg, sizeof(szMsg), NULL 
                        );
    if (dwChars == 0)
        return _T("Unknown error code");

    #ifdef _CONSOLE
    std::string str(szMsg);
    //char *tmp = (char*)_alloca(str.size()+1);
    //str.copy(tmp, str.size()); tmp[str.size()] = 0;
    CharToOemBuff( str.c_str(), szMsg, str.size());
    str = szMsg;
    return str;
    #else
    return szMsg;
    #endif
}

} // namespace Utils {
} // namespace Rsdn  {
//==========================================================)