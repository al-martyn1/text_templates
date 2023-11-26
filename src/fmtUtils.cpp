/* (c)2005-2007 Alex Martynov, amart@mail.ru
 * This source file is a part of "Text Templates" - a plugin for file manager FAR.
 * You can use, modify, redistribute this source file without any limitations.
 * This source file is distributed "as is". No waranty of any kind is expressed or 
 * implied. Use it at your own risk.
 */
#include "itextfmt.h"


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
