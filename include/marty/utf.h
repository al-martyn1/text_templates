/*
   Copyright (c) 2007-2009 Alex Martynov, amart at mail.ru
   This code can be used without any limitations.
   This code provided as is, use it on your own risk.
*/

#ifndef MARTY_UTF_H
#define MARTY_UTF_H


#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif


#if !defined(_INC_STDDEF) && !defined(_STDDEF_H_) && !defined(_STDDEF_H)
    #include <stddef.h>
#endif



/*
Bytes Encoding Form 
00 00 FE FF         UTF-32, big-endian 
FF FE 00 00         UTF-32, little-endian 
FE FF               UTF-16, big-endian 
FF FE               UTF-16, little-endian 
EF BB BF            UTF-8 
*/

#ifdef UTF_IN_MARTY_NAMESPACE
    #ifndef MARTY_NS
        #define BEGIN_MARTY_NS          namespace marty {
        #define END_MARTY_NS            };
        #define MARTY_NS                ::marty::
    #endif
    #define MARTY_UTF_NS                MARTY_NS utf::
    #define MARTY_UTF                   ::marty::utf
#else
    #ifndef MARTY_NS
        #define BEGIN_MARTY_NS 
        #define END_MARTY_NS 
        #define MARTY_NS                ::
    #endif
    #define MARTY_UTF_NS                MARTY_NS utf::
    #define MARTY_UTF                   ::utf
#endif


// encoders/decoders designed like stream converters (filters)

/*

CUtf8Decoder                   - decode from UTF-8 to UCS4
CUtf8Encoder                   - encode from UCS4  to UTF-8
CByteStreamBeToUcs2Decoder     - decode byte stream to UCS2 - big endian UTF16 decoder
CByteStreamLeToUcs2Decoder     - decode byte stream to UCS2 - little endian UTF16 decoder
CUcs2ToByteStreamBeEncoder     - encode UCS2 to byte stream with big endian order
CUcs2ToByteStreamLeEncoder     - encode UCS2 to byte stream with little endian order
CUcs2ToUcs4Decoder             - converting UCS2 (UTF16) 1 or to words sequences to UCS4
CUcs4ToUcs2Encoder             - converting USC4 to sequence of UCS2 words

*/



BEGIN_MARTY_NS

namespace utf
{

typedef unsigned int ucs4;
typedef unsigned short int ucs2;

// http://rfc.net/rfc2279.html UTF8

// THandler is struct like
// struct CSimpleHandler 
// {
//     operator()(ucs4 ucsChar) {}
// };

// utf8 ->ucs4
template <typename THandler>
struct CUtf8Decoder
{
    THandler handler;
    ucs4     buf;
    //int      state;
    int      count;
    int      countMax;    

    CUtf8Decoder(const THandler &th) 
       : handler(th)
       , buf(0)
       //, state(0)
       , count(0)
       , countMax(0) 
       {}

    CUtf8Decoder(const CUtf8Decoder &dc) 
       : handler(dc.handler)
       , buf(dc.buf)
       //, state(dc.state)
       , count(dc.count)
       , countMax(dc.countMax)
       {}
/*
UCS-4 range (hex.)     mask cmp  UTF-8 octet sequence (binary)
   0000 0000-0000 007F  80   00   0xxxxxxx
   0000 0080-0000 07FF  E0   C0   110xxxxx 10xxxxxx
   0000 0800-0000 FFFF  F0   E0   1110xxxx 10xxxxxx 10xxxxxx
                                 
   0001 0000-001F FFFF  F8   F0   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
   0020 0000-03FF FFFF  FC   F8   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
   0400 0000-7FFF FFFF  FE   FC   1111110x 10xxxxxx ... 10xxxxxx
*/

    void operator()(unsigned char ch)
       {
        if (count>=countMax) // first char in sequence
           {
            if ((ch&0x80)==0)         { buf = ch; handler(buf); }
            else if ((ch&0xE0)==0xC0) { buf = ch&~0xE0; count = 0; countMax = 1; }
            else if ((ch&0xF0)==0xE0) { buf = ch&~0xF0; count = 0; countMax = 2; }
            else if ((ch&0xF8)==0xF0) { buf = ch&~0xF8; count = 0; countMax = 3; }
            else if ((ch&0xFC)==0xF8) { buf = ch&~0xFC; count = 0; countMax = 4; }
            else if ((ch&0xFE)==0xFC) { buf = ch&~0xFE; count = 0; countMax = 5; }
            else                      { buf = ch&0x80; handler(buf); }
           }
        else
           {
            buf<<=6; buf |= ch&0x3F; count++;
            if (count>=countMax) handler(buf);
           }
       }

    void operator()(char ch) { operator()((unsigned char)ch); }

    void operator()(const unsigned char *pch, size_t nBytes)
       { const unsigned char *pend = pch + nBytes; for(; pch!=pend; ++pch) operator()(*pch); }

    void operator()(const char *pch, size_t nBytes)
       { operator()((const unsigned char *)pch, nBytes); }

};


// THandler is struct like
// struct CSimpleHandler 
// {
//     operator()([unsigned] char ба) {}
// };
// ucs4 -> utf8
template <typename THandler>
struct CUtf8Encoder
{
    THandler handler;

    CUtf8Encoder(const THandler &th) 
       : handler(th)
       {}

    CUtf8Encoder(const CUtf8Encoder &dc) 
       : handler(dc.handler)
       {}
/*
UCS-4 range (hex.)     mask cmp  UTF-8 octet sequence (binary)
   0000 0000-0000 007F  80   00   0xxxxxxx
   0000 0080-0000 07FF  E0   C0   110xxxxx 10xxxxxx
   0000 0800-0000 FFFF  F0   E0   1110xxxx 10xxxxxx 10xxxxxx
                                 
   0001 0000-001F FFFF  F8   F0   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
   0020 0000-03FF FFFF  FC   F8   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
   0400 0000-7FFF FFFF  FE   FC   1111110x 10xxxxxx ... 10xxxxxx
*/

    void operator()(ucs4 uch)
       {
        unsigned char buf[6];
        int count = 0;
        int maxCount = 0;
        unsigned char  orMask = 0;
        //unsigned char  orMask = 0;

        if (uch>=0x04000000)      { maxCount = 6; orMask = 0xFC; }
        else if (uch>=0x00200000) { maxCount = 5; orMask = 0xF8; }
        else if (uch>=0x00010000) { maxCount = 4; orMask = 0xF0; }
        else if (uch>=0x00000800) { maxCount = 3; orMask = 0xE0; }
        else if (uch>=0x00000080) { maxCount = 2; orMask = 0xC0; }
        else                      { maxCount = 1; orMask = 0x00; }

        while(count!=maxCount)
           {
            count++;               // most significant bits
            if (count==maxCount) { buf[count-1] = uch | orMask; }
            else                 { buf[count-1] = uch&0x3F | 0x80; uch>>=6; }
           }

        for(count=maxCount; count>0; --count)  handler(buf[count-1]);
       }

    void operator()(const ucs4 *puch, size_t nChars) 
       { const ucs4 *pend = puch + nChars; for(; puch!=pend; ++puch) operator()(*puch); }

};


// convert byte stream to ucs2
// THandler is struct like
// struct CSimpleHandler 
// {
//     operator()(ucs2 ucsChar) {}
// };

template <typename THandler>
struct CByteStreamBeToUcs2Decoder /* ucs2 aka utf-16 big-endian stream */
{
    THandler handler;
    ucs2     buf; 
    bool     state;

    CByteStreamBeToUcs2Decoder(const THandler &th) 
       : handler(th), buf(0), state(false)
       {}

    CByteStreamBeToUcs2Decoder(const CByteStreamBeToUcs2Decoder &dc) 
       : handler(dc.handler), buf(dc.buf), state(dc.state)
       {}

    void operator()(unsigned char ch)
       {
        if (state) // recv next byte
           { 
            handler(buf | (ucs2)ch); state = false;
           }
        else
           {
            buf = ((ucs2)ch)<<8; state = true;
           }
       }

    void operator()(char ch) { operator()((unsigned char)ch); }

    void operator()(const unsigned char *pch, size_t nBytes)
       { const unsigned char *pend = pch + nBytes; for(; pch!=pend; ++pch) operator()(*pch); }

    void operator()(const char *pch, size_t nBytes)
       { operator()((const unsigned char *)pch, nBytes); }

};



template <typename THandler>
struct CByteStreamLeToUcs2Decoder /* ucs2 aka utf-16 little-endian stream */
{
    THandler handler;
    ucs2     buf; 
    bool     state;

    CByteStreamLeToUcs2Decoder(const THandler &th) 
       : handler(th), buf(0), state(false)
       {}

    CByteStreamLeToUcs2Decoder(const CByteStreamLeToUcs2Decoder &dc) 
       : handler(dc.handler), buf(dc.buf), state(dc.state)
       {}

    void operator()(unsigned char ch)
       {
        if (state) // recv next byte
           { 
            handler(buf | (((ucs2)ch)<<8)); state = false;
           }
        else
           {
            buf = ch; state = true;
           }
       }

    void operator()(char ch) { operator()((unsigned char)ch); }

    void operator()(const unsigned char *pch, size_t nBytes)
       { const unsigned char *pend = pch + nBytes; for(; pch!=pend; ++pch) operator()(*pch); }

    void operator()(const char *pch, size_t nBytes)
       { operator()((const unsigned char *)pch, nBytes); }

};

// THandler is struct like
// struct CSimpleHandler 
// {
//     operator()([unsigned] char ба) {}
// };


template <typename THandler>
struct CUcs2ToByteStreamBeEncoder
{
    THandler handler;

    CUcs2ToByteStreamBeEncoder(const THandler &th) 
       : handler(th)
       {}

    CUcs2ToByteStreamBeEncoder(const CUcs2ToByteStreamBeEncoder &dc) 
       : handler(dc.handler)
       {}

    void operator()(ucs2 uch)
       {
        handler((unsigned char)((uch&0xFF00)>>8));
        handler((unsigned char)(uch&0xFF));
       }

    void operator()(const ucs2 *puch, size_t nChars) 
       { const ucs2 *pend = puch + nChars; for(; puch!=pend; ++puch) operator()(*puch); }

};


template <typename THandler>
struct CUcs2ToByteStreamLeEncoder
{
    THandler handler;

    CUcs2ToByteStreamLeEncoder(const THandler &th)
       : handler(th)
       {}

    CUcs2ToByteStreamLeEncoder(const CUcs2ToByteStreamLeEncoder &dc)
       : handler(dc.handler)
       {}

    void operator()(ucs2 uch)
       {
        handler((unsigned char)(uch&0xFF));
        handler((unsigned char)((uch&0xFF00)>>8));
       }

    void operator()(const ucs2 *puch, size_t nChars) 
       { const ucs2 *pend = puch + nChars; for(; puch!=pend; ++puch) operator()(*puch); }

};

// http://rfc.net/rfc2781.html UTF-16

template <typename THandler>
struct CUcs2ToUcs4Decoder
{
    THandler handler;
    ucs4     buf;
    bool     state;

    CUcs2ToUcs4Decoder(const THandler &th)
       : handler(th), buf(0), state(false)
       {}

    CUcs2ToUcs4Decoder(const CUcs2ToUcs4Decoder &dc)
       : handler(dc.handler), buf(dc.buf), state(dc.state)
       {}

    void operator()(ucs2 uch)
       {
        // http://rfc.net/rfc2781.html - 2.2 Decoding UTF-16
        if (!state) // wait first 16 bit symbol - W1
           {
            if (uch<0xD800 || uch>0xDFFF)
               handler(uch); // the character value U is the value of W1. Terminate.
            else
               { // if W1 is not between 0xD800 and 0xDBFF the sequence is in error, we ignoring that
                buf = (uch&0x3FF)<<10; // 10 low-order bits of W1 as its 10 high-order bits 
                state = true; // wait for W2
               }
           }
        else // W2 received
           {
            handler(uch | (uch&0x3FF));
            state = false; // go to wait W1 again
           }
       }

    void operator()(const ucs2 *puch, size_t nChars) 
       { const ucs2 *pend = puch + nChars; for(; puch!=pend; ++puch) operator()(*puch); }
};



template <typename THandler>
struct CUcs4ToUcs2Encoder
{
    THandler handler;

    CUcs4ToUcs2Encoder(const THandler &th)
       : handler(th)
       {}

    CUcs4ToUcs2Encoder(const CUcs4ToUcs2Encoder &dc)
       : handler(dc.handler)
       {}

    void operator()(ucs4 uch)
       { // http://rfc.net/rfc2781.html - 2.1 Encoding UTF-16
        if (uch<0x10000) handler((ucs2)uch);
        else
           {
            ucs4 uchApos = uch - 0x10000;
            // Initialize W1 and W2, to 0xD800 and 0xDC00, respectively.
            // Assign the 10 high-order bits of the 20-bit U' to the 10 low-order
            // bits of W1 and the 10 low-order bits of U' to the 10 low-order
            // bits of W2.
            handler((ucs2)(0xD800 | ((uchApos>>10)&0x3FF)));
            handler((ucs2)(0xDC00 | (uchApos&0x3FF)));
           }
       }

    void operator()(const ucs4 *puch, size_t nChars) 
       { const ucs4 *pend = puch + nChars; for(; puch!=pend; ++puch) operator()(*puch); }

};

//00 00 FE FF         UTF-32, big-endian 
//FF FE 00 00         UTF-32, little-endian 
// input is a stream of chars
// THandler receives ucs4 chars
template <typename THandler>
struct CUtf32BeToUtf32Decoder
{
    THandler handler;
    unsigned byteNo;
    ucs4     buf;

    CUtf32BeToUtf32Decoder(const THandler &th)
       : handler(th), byteNo(0), buf(0) {}

    CUtf32BeToUtf32Decoder(const CUtf32BeToUtf32Decoder &dc)
       : handler(dc.handler), byteNo(dc.byteNo), buf(dc.buf) {}

    unsigned state() const { return byteNo&0x03; }

    void operator()(unsigned char ch)
       {
        buf |= (((ucs4)ch) << 8*(3-state()));
        byteNo++;
        if (!state()) { handler(buf); buf = 0; }
       }

    void operator()(char ch) { operator()((unsigned char)ch); }

    void operator()(const unsigned char *pch, size_t nBytes)
       { const unsigned char *pend = pch + nBytes; for(; pch!=pend; ++pch) operator()(*pch); }

    void operator()(const char *pch, size_t nBytes)
       { operator()((const unsigned char *)pch, nBytes); }

};


template <typename THandler>
struct CUtf32ToUtf32BeEncoder
{
    THandler handler;

    CUtf32ToUtf32BeEncoder(const THandler &th)
       : handler(th) {}
    CUtf32ToUtf32BeEncoder(const CUtf32ToUtf32BeEncoder &dc)
       : handler(dc.handler) {}

    void operator()(ucs4 uch)
       {
        handler((uch>>24)&0xFF);
        handler((uch>>16)&0xFF);
        handler((uch>>8 )&0xFF);
        handler((uch    )&0xFF);
       }
    void operator()(const ucs4 *puch, size_t nChars) 
       { const ucs4 *pend = puch + nChars; for(; puch!=pend; ++puch) operator()(*puch); }
};


template <typename THandler>
struct CUtf32LeToUtf32Decoder
{
    THandler handler;
    unsigned byteNo;
    ucs4     buf;

    CUtf32LeToUtf32Decoder(const THandler &th)
       : handler(th), byteNo(0), buf(0) {}

    CUtf32LeToUtf32Decoder(const CUtf32LeToUtf32Decoder &dc)
       : handler(dc.handler), byteNo(dc.byteNo), buf(dc.buf) {}

    unsigned state() const { return byteNo&0x03; }

    void operator()(unsigned char ch)
       {
        buf |= (((ucs4)ch) << 8*(state()));
        byteNo++;
        if (!state()) { handler(buf); buf = 0; }
       }

    void operator()(char ch) { operator()((unsigned char)ch); }

    void operator()(const unsigned char *pch, size_t nBytes)
       { const unsigned char *pend = pch + nBytes; for(; pch!=pend; ++pch) operator()(*pch); }

    void operator()(const char *pch, size_t nBytes)
       { operator()((const unsigned char *)pch, nBytes); }

};

template <typename THandler>
struct CUtf32ToUtf32LeEncoder
{
    THandler handler;

    CUtf32ToUtf32LeEncoder(const THandler &th)
       : handler(th) {}
    CUtf32ToUtf32LeEncoder(const CUtf32ToUtf32LeEncoder &dc)
       : handler(dc.handler) {}

    void operator()(ucs4 uch)
       {
        handler((uch    )&0xFF);
        handler((uch>>8 )&0xFF);
        handler((uch>>16)&0xFF);
        handler((uch>>24)&0xFF);
       }

    void operator()(const ucs4 *puch, size_t nChars) 
       { const ucs4 *pend = puch + nChars; for(; puch!=pend; ++puch) operator()(*puch); }
};



// THandler receives ucs2 chars
template <typename THandler>
struct CUtf8ToUtf16Decoder
{
    typedef CUcs4ToUcs2Encoder<THandler> CProxyHandler;

    CProxyHandler                proxy;
    CUtf8Decoder<CProxyHandler>  decoder;

    CUtf8ToUtf16Decoder(const THandler &handler)
       : proxy(handler), decoder(proxy) {}
    CUtf8ToUtf16Decoder(const CUtf8ToUtf16Decoder &dc)
       : proxy(dc.proxy), decoder(dc.decoder) {}

    void operator()(unsigned char ch) { decoder(ch); }
    void operator()(char ch)          { decoder(ch); }
    void operator()(const unsigned char *pch, size_t nBytes) { decoder(pch, nBytes); }
    void operator()(const char *pch, size_t nBytes)          { decoder(pch, nBytes); }
};

// THandler receives plain chars (bytes)
template <typename THandler>
struct CUtf16ToUtf8Encoder
{
    typedef CUtf8Encoder<THandler>       CProxyHandler;

    CProxyHandler                      proxy;
    CUcs2ToUcs4Decoder<CProxyHandler>  encoder;

    CUtf16ToUtf8Encoder(const THandler &handler)
       : proxy(handler), encoder(proxy) {}
    CUtf16ToUtf8Encoder(const CUtf16ToUtf8Encoder &dc)
       : proxy(dc.proxy), encoder(dc.encoder) {}

    void operator()(ucs2 uch)                     { encoder(uch); }
    void operator()(const ucs2 *puch, size_t nChars) { encoder(puch, nChars); }
};


template < typename CharType
         , typename Traits    =::std::char_traits<CharType>
         , typename Allocator =::std::allocator<CharType>
         >
struct CCharsCollector
{
    ::std::basic_string<CharType, Traits, Allocator> &buf;
    CCharsCollector(const CCharsCollector &cc) : buf(cc.buf) {}
    CCharsCollector(::std::basic_string<CharType, Traits, Allocator> &b) : buf(b) {}
    void operator()(CharType ch)
       { buf.append(1,ch); }
};



#ifdef _WIN32
    const bool fWin32 = true;
#else
    const bool fWin32 = false;
#endif

inline
::std::wstring fromUtf8(const ::std::string &utfStr)
   {
    ::std::wstring res;
    res.reserve(utfStr.size());

    CCharsCollector<wchar_t> collector(res);

    if (fWin32 || (sizeof(wchar_t)<4))
       { // wchar_t is ucs2
        CUtf8ToUtf16Decoder<CCharsCollector<wchar_t> > decoder(collector);
        decoder(utfStr.data(), (int)utfStr.size());
       }
    else
       {
        CUtf8Decoder<CCharsCollector<wchar_t> >        decoder(collector);
        decoder(utfStr.data(), (int)utfStr.size());
       }
    return res;
   }

inline
::std::string toUtf8(const ::std::wstring &ucsStr)
   {
    ::std::string res;
    res.reserve(2*ucsStr.size());
    CCharsCollector<char> collector(res);

    if (fWin32 || (sizeof(wchar_t)<4))
       { // wchar_t is ucs2
        CUtf16ToUtf8Encoder<CCharsCollector<char> >  encoder(collector);
        encoder((const ucs2*)ucsStr.data(), (int)ucsStr.size());
       }
    else
       {
        CUtf8Encoder<CCharsCollector<char> >         encoder(collector);
        encoder((const ucs4*)ucsStr.data(), (int)ucsStr.size());
       }

    return res;
    //if (res.capacity()>res.size()*2)
    //::std::string tmp; // create an empty string
    //tmp.swap(res); // free unised space allocated by reserve
    //return tmp;
   }



// Automatic detect stream decoding UCS Decoder
template <typename THandler>
struct CUcsDecoder
{
    enum decoderType
        {
         dt_unknown, // dt_detect
         dt_utf32be,
         dt_utf32le,
         dt_utf16be,
         dt_utf16le,
         dt_utf8
        };

    enum detectState
        {
         ds_start,
         ds_waitUtf32BeZero2,
         //ds_waitUtf32BeZero3,
         ds_startLeWait2,
         ds_startUtf16BeWait2,
         ds_waitUtf32BeWaitFE,
         ds_waitUtf32BeWaitFF,
         ds_waitUtfLeWaitZero3,
         ds_waitUtfLeWaitZero4,
         ds_startUtf8Wait2,
         ds_startUtf8Wait3
        };

    detectState   detectState;
    decoderType   curType;

    CUtf32BeToUtf32Decoder<THandler>                                 utf32BeDecoder;
    CUtf32LeToUtf32Decoder<THandler>                                 utf32LeDecoder;
    CByteStreamBeToUcs2Decoder< CUcs2ToUcs4Decoder<THandler> >       utf16BeDecoder;
    CByteStreamLeToUcs2Decoder< CUcs2ToUcs4Decoder<THandler> >       utf16LeDecoder;
    CUtf8Decoder<THandler>                                           utfDecoder;
                                    /* auto detect encoding by first bytes contents */
    CUcsDecoder(const THandler &th, decoderType dt = dt_unknown)
       : detectState(ds_start)
       , curType(dt_unknown)
       , utf32BeDecoder(th)
       , utf32LeDecoder(th)
       , utf16BeDecoder(CUcs2ToUcs4Decoder<THandler>(th))
       , utf16LeDecoder(CUcs2ToUcs4Decoder<THandler>(th))
       , utfDecoder(th)
       {}

    CUcsDecoder(const CUcsDecoder &dc)
       : detectState(dc.detectState)
       , curType(dc.curType)
       , utf32BeDecoder(dc.utf32BeDecoder)
       , utf32LeDecoder(dc.utf32LeDecoder)
       , utf16BeDecoder(dc.utf16BeDecoder)
       , utf16LeDecoder(dc.utf16LeDecoder)
       , utfDecoder(dc.utfDecoder)
       {}

    void operator()(char ch) { operator()((unsigned char)ch); }

    void operator()(const unsigned char *pch, size_t nBytes)
       { const unsigned char *pend = pch + nBytes; for(; pch!=pend; ++pch) operator()(*pch); }

    void operator()(const char *pch, size_t nBytes)
       { operator()((const unsigned char *)pch, nBytes); }

    void operator()(unsigned char ch)
       {
        switch(curType)
           {
            case dt_unknown:   
                               {
                                /*
                                Bytes Encoding Form 
                                00 00 FE FF         UTF-32, big-endian 
                                FF FE 00 00         UTF-32, little-endian 
                                FE FF               UTF-16, big-endian 
                                FF FE               UTF-16, little-endian 
                                EF BB BF            UTF-8 
                                */
                                switch(detectState)
                                    {
                                     case ds_start:
                                                    switch(ch)
                                                       {
                                                        case 0x00: detectState = ds_waitUtf32BeZero2;  break;
                                                        case 0xFF: detectState = ds_startLeWait2;      break;
                                                        case 0xFE: detectState = ds_startUtf16BeWait2; break;
                                                        case 0xEF: detectState = ds_startUtf8Wait2;    break;
                                                        default:   curType = dt_utf8; operator()(ch); break;
                                                       };
                                                    break;
                                     // utf32be
                                     // check 00 00 
                                     case ds_waitUtf32BeZero2:
                                                    if (ch==0)    detectState = ds_waitUtf32BeWaitFE; //ds_waitUtf32BeZero3;
                                                    else          { curType = dt_utf8; operator()(ch); } // error
                                                    break;
                                     // check 00 00 FE
                                     case ds_waitUtf32BeWaitFE:
                                                    if (ch==0xFE) detectState = ds_waitUtf32BeWaitFF;
                                                    else          { curType = dt_utf8; operator()(ch); } // error
                                                    break;
                                     // check 00 00 FE FF
                                     case ds_waitUtf32BeWaitFF:
                                                    if (ch==0xFF) curType = dt_utf32be;
                                                    else          { curType = dt_utf8; operator()(ch); } // error
                                                    break;
                                     // le 16/32
                                     // check FF FE
                                     case ds_startLeWait2:
                                                    if (ch==0xFE) detectState = ds_waitUtfLeWaitZero3;
                                                    else          { curType = dt_utf8; operator()(ch); } // error
                                                    break;
                                     // check FF FE 00
                                     case ds_waitUtfLeWaitZero3:
                                                    if (ch==0)    detectState = ds_waitUtfLeWaitZero4;
                                                    else          { curType = dt_utf16le; operator()(ch); }
                                                    break;
                                     // check FF FE 00 00
                                     case ds_waitUtfLeWaitZero4:
                                                    if (ch==0)    curType = dt_utf32be;
                                                    else          { curType = dt_utf16le; operator()((unsigned char)(0)); operator()(ch); } // error
                                                    break;
                                     // check FE FF
                                     case ds_startUtf16BeWait2:
                                                    if (ch==0xFF) curType = dt_utf16be;
                                                    else          { curType = dt_utf8; operator()(ch); } // error
                                                    break;
                                     // check EF BB
                                     case ds_startUtf8Wait2:
                                                    if (ch==0xBB) detectState = ds_startUtf8Wait3;
                                                    else          { curType = dt_utf8; operator()(ch); } // error
                                                    break;
                                     // check EF BB BF
                                     case ds_startUtf8Wait3:
                                                    curType = dt_utf8;
                                                    if (ch!=0xBF) operator()(ch); // error
                                                    break;
                                    };
                               }
                               break;

            case dt_utf32be:   utf32BeDecoder(ch); break;
            case dt_utf32le:   utf32LeDecoder(ch); break;
            case dt_utf16be:   utf16BeDecoder(ch); break;
            case dt_utf16le:   utf16LeDecoder(ch); break;
            case dt_utf8   :   utfDecoder    (ch); break;
           };
       }

};


// Stream UCS Encoder
// receives ucs4, output - byte stream
template <typename THandler>
struct CUcsEncoder
{
    enum decoderType
        {
         dt_unknown,
         dt_utf32be,
         dt_utf32le,
         dt_utf16be,
         dt_utf16le,
         dt_utf8
        };

    decoderType   curType;
    bool          signatured;

    CUtf32ToUtf32BeEncoder<THandler>                                 utf32BeEncoder;
    CUtf32ToUtf32LeEncoder<THandler>                                 utf32LeEncoder;
    CUcs4ToUcs2Encoder< CUcs2ToByteStreamBeEncoder<THandler> >       utf16BeEncoder;
    CUcs4ToUcs2Encoder< CUcs2ToByteStreamLeEncoder<THandler> >       utf16LeEncoder;
    CUtf8Encoder<THandler>                                           utfEncoder;
    THandler                                                         handler;

    CUcsEncoder(const THandler &th, decoderType dt)
       : curType(dt)
       , signatured(false)
       , utf32BeEncoder(th)
       , utf32LeEncoder(th)
       , utf16BeEncoder(CUcs2ToByteStreamBeEncoder<THandler>(th))
       , utf16LeEncoder(CUcs2ToByteStreamLeEncoder<THandler>(th))
       , utfEncoder(th)
       , handler(th)
       {}

    CUcsEncoder(const CUcsEncoder &dc)
       : curType(dc.curType)
       , signatured(dc.signatured)
       , utf32BeEncoder(dc.utf32BeEncoder)
       , utf32LeEncoder(dc.utf32LeEncoder)
       , utf16BeEncoder(dc.utf16BeEncoder)
       , utf16LeEncoder(dc.utf16LeEncoder)
       , utfEncoder(dc.utfEncoder)
       , handler(dc.handler)
       {}

            /*
            Bytes Encoding Form 
            00 00 FE FF         UTF-32, big-endian
            FF FE 00 00         UTF-32, little-endian 
            FE FF               UTF-16, big-endian 
            FF FE               UTF-16, little-endian 
            EF BB BF            UTF-8 
            */

    void operator()(const ucs4 *puch, size_t nChars) 
       { const ucs4 *pend = puch + nChars; for(; puch!=pend; ++puch) operator()(*puch); }

    void operator()(unsigned char ch)
       {
        if (!signatured)
           {
            signatured = true;
            switch(curType)
               {
                case dt_utf32be: 
                                  handler((unsigned char)0x00);
                                  handler((unsigned char)0x00);
                                  handler((unsigned char)0xFE);
                                  handler((unsigned char)0xFF);
                                  break;
                case dt_utf32le:
                                  handler((unsigned char)0xFF);
                                  handler((unsigned char)0xFE);
                                  handler((unsigned char)0x00);
                                  handler((unsigned char)0x00);
                                  break;
                case dt_utf16be:
                                  handler((unsigned char)0xFE);
                                  handler((unsigned char)0xFF);
                                  break;
                case dt_utf16le:
                                  handler((unsigned char)0xFF);
                                  handler((unsigned char)0xFE);
                                  break;
                default: // utf8
                                  handler((unsigned char)0xEF);
                                  handler((unsigned char)0xBB);
                                  handler((unsigned char)0xBF);
                //case dt_utf8
               
               };           
           }

        switch(curType)
           {
            case dt_utf32be:   utf32BeEncoder(ch); break;
            case dt_utf32le:   utf32LeEncoder(ch); break;
            case dt_utf16be:   utf16BeEncoder(ch); break;
            case dt_utf16le:   utf16LeEncoder(ch); break;
            default /*case dt_utf8 */ : utfEncoder    (ch); break;
           };
       }

};


template <typename THandler>
struct CUtfToWcharDecoder
{   
    #ifdef _WIN32

    typedef CUcs4ToUcs2Encoder<THandler> TProxy;
    CUcsDecoder<TProxy>                    decoder;
    CUtfToWcharDecoder(const THandler &handler, typename CUcsDecoder<TProxy>::decoderType dt=CUcsDecoder<TProxy>::dt_unknown)
       : decoder(TProxy(handler), dt)
       {}

    #else

    CUcsDecoder<THandler>                  decoder;
    CUtfToWcharDecoder(const THandler &handler, typename CUcsDecoder<THandler>::decoderType dt=CUcsDecoder<THandler>::dt_unknown)
       : decoder(handler, dt)
       {}

    #endif

    CUtfToWcharDecoder(const CUtfToWcharDecoder &dc)
       : decoder(dc.decoder)
       {}

    void operator()(char ch) { operator()((unsigned char)ch); }

    void operator()(const unsigned char *pch, size_t nBytes)
       { const unsigned char *pend = pch + nBytes; for(; pch!=pend; ++pch) operator()(*pch); }

    void operator()(const char *pch, size_t nBytes)
       { operator()((const unsigned char *)pch, nBytes); }

    void operator()(unsigned char ch)
       {
        decoder.operator()(ch);
       }

};


template <typename THandler>
struct CWcharToUtfEncoder // CUcsEncoder
{
    #ifdef _WIN32
    typedef CUcs2ToUcs4Decoder<CUcsEncoder<THandler> >  TProxy;
    TProxy                                              handler;
    CWcharToUtfEncoder(const THandler &th, typename CUcsEncoder<THandler>::decoderType dt)
       : handler(CUcsEncoder<THandler>(handler, dt))
       {}
    #else
    CUcsEncoder<THandler>                               handler;
    CWcharToUtfEncoder(const THandler &th, typename CUcsEncoder<THandler>::decoderType dt)
       : handler(handler, dt)
       {}
    #endif

    CWcharToUtfEncoder(const CWcharToUtfEncoder &dc)
       : handler(dc.handler)
       {}

    void operator()(const wchar_t *pwc, size_t nChars) 
       { const wchar_t *pend = pwc + nChars; for(; pwc!=pend; ++pwc) operator()(*pwc); }

    void operator()(wchar_t wch)
       {
        handler.operator()(wch);
       }


};





}; // namespace utf

END_MARTY_NS

#endif /* MARTY_UTF_H */














