/*
   Copyright (c) 2007-2009 Alex Martynov, amart at mail.ru
   This code can be used without any limitations.
   This code provided as is, use it on your own risk.
*/

#ifndef MARTY_FILESYS_CPP
#define MARTY_FILESYS_CPP


#ifndef MARTY_FILESYS_H
    #include <marty/filesys.h>
#endif


#ifndef _WIN32

BEGIN_MARTY_FILESYSTEM_NS

//-----------------------------------------------------------------------------
CFilenameEncoder< ::std::string::traits_type::char_type
                , ::std::string::traits_type
                , ::std::string::allocator_type>& 
getFilenameEncoder(const ::std::string &dummy)
   {
    static CFilenameEncoder< ::std::string::traits_type::char_type
                           , ::std::string::traits_type
                           , ::std::string::allocator_type> enc; 
    return enc;
   }

//-----------------------------------------------------------------------------
CFilenameEncoder< ::std::wstring::traits_type::char_type
                , ::std::wstring::traits_type
                , ::std::wstring::allocator_type>& 
getFilenameEncoder(const ::std::wstring &dummy)
   {
    static CFilenameEncoder< ::std::wstring::traits_type::char_type
                           , ::std::wstring::traits_type
                           , ::std::wstring::allocator_type> enc;
    return enc;
   }



END_MARTY_FILESYSTEM_NS

#endif /* WIN32 */

#endif /* MARTY_FILESYS_CPP */

