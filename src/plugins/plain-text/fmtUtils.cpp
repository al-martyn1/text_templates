#include "../../itextfmt.h"


//-----------------------------------------------------------------------------
LPVOID WINAPI textFormatApiAllocate(SIZE_T nBytes)
   {
    return ::HeapAlloc( ::GetProcessHeap(), HEAP_ZERO_MEMORY, nBytes);
   }

//-----------------------------------------------------------------------------
BOOL WINAPI textFormatApiFree(LPVOID pMem)
   {   
    return ::HeapFree( ::GetProcessHeap(), 0, pMem);
   }

//-----------------------------------------------------------------------------
void initIAllocator(PITextFormatApiAllocator pIAllocator)
   {
    pIAllocator->pfnAllocate = textFormatApiAllocate;
    pIAllocator->pfnFree     = textFormatApiFree;

   }
