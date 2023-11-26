/* Automatic COM initialize/Initialize */
#ifndef AUTOCOM_H
#define AUTOCOM_H

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




class CAutoCom
{
        HRESULT hres;

    public:

        inline
        CAutoCom()
        {
            hres = CoInitialize( 0 );
        }

        inline
        ~CAutoCom()
        {
            if (hres==S_OK || hres==S_FALSE )
               CoUninitialize();
        }
};


#define INIT_COM() CAutoCom  auto_com;

                    
#endif /* AUTOCOM_H */