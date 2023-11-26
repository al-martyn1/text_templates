/* (c) Alex Martynov, 2009, amart@mail.ru
   Based on Ms-Rem pascal code, see 
   http://www.wasm.ru/article.php?article=apihook_1
   http://www.wasm.ru/article.php?article=apihook_2
   http://www.wasm.ru/article.php?article=apihook_3 
   for details
*/

#ifndef X86_DISASM_H
#define X86_DISASM_H


#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

#ifndef STRICT
    #define STRICT
#endif

#if !defined(_WINDOWS_)
    #include <windows.h>
#endif


#if !defined(_INC_STDDEF) && !defined(_STDDEF_H_) && !defined(_STDDEF_H)
    #include <stddef.h>
#endif

#if !defined(_INC_STDIO) && !defined(_STDIO_H_) && !defined(_STDIO_H)
    #include <stdio.h>
#endif

#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

#if !defined(_IOSTREAM_) && !defined(_STLP_IOSTREAM) && !defined(__STD_IOSTREAM__) && !defined(_CPP_IOSTREAM) && !defined(_GLIBCXX_IOSTREAM)
    #include <iostream>
#endif

#if !defined(_MAP_) && !defined(_STLP_MAP) && !defined(__STD_MAP__) && !defined(_CPP_MAP) && !defined(_GLIBCXX_MAP)
    #include <map>
#endif

#if !defined(_SSTREAM_) && !defined(_STLP_SSTREAM) && !defined(__STD_SSTREAM__) && !defined(_CPP_SSTREAM) && !defined(_GLIBCXX_SSTREAM)
    #include <sstream>
#endif



namespace marty
{
namespace x86
{

typedef size_t        CODE_SIZE_T;
typedef unsigned char CODE_T;
typedef       CODE_T* CODE_PTR;
typedef const CODE_T* CODE_CONST_PTR;
typedef unsigned short OPCODE_FLAGS_T;
typedef unsigned short OPCODE_T;
typedef unsigned short CODE1BYTE_T;  // one byte instruction
typedef unsigned short CODE2BYTES_T; // two bytes instruction
typedef unsigned       ADDR32_T; // two bytes instruction


typedef void (*GENERIC_FN_PTR)(void);


// Instruction prefix types
// see [intel] 2.2. INSTRUCTION PREFIXES

enum EInstructionPrefixType
{
    // not a prefix
    iptNone            = 0,
    // Lock and repeat prefixes
    iptRepeat          = 1,
    // Segment override
    iptSegment         = 2,
    // Operand-size override
    iptOperandSize     = 3,
    // Address-size override
    iptAddressSize     = 4,
    // Streaming SIMD Extensions prefix (2-byte opcode)
    iptSimdExt         = 5
};

/*
// not a prefix
const int iptNone            = 0;

// Lock and repeat prefixes
const int iptRepeat          = 1;
// Segment override
const int iptSegment         = 2;
// Operand-size override
const int iptOperandSize     = 3;
// Address-size override
const int iptAddressSize     = 4;
// Streaming SIMD Extensions prefix (2-byte opcode)
const int iptSimdExt         = 5;
*/

inline
EInstructionPrefixType getPrefixType( CODE_T code )
   {
    switch(code)
       {
        case 0xF0:  // LOCK prefix.
        case 0xF2:  // REPNE/REPNZ prefix (used only with string instructions).
        case 0xF3:  // REP prefix (used only with string instructions).
        //case 0xF3:  // REPE/REPZ prefix (used only with string instructions).
        //case 0xF3:  // Streaming SIMD Extensions prefix.
                    return iptRepeat;

        case 0x26:  // ES segment override prefix.
        case 0x2E:  // CS segment override prefix.
        case 0x36:  // SS segment override prefix.
        case 0x3E:  // DS segment override prefix.
        case 0x64:  // FS segment override prefix.
        case 0x65:  // GS segment override prefix.
                    return iptSegment;

        case 0x66:  // Operand-size override, 66H
                    return iptOperandSize;

        case 0x67:  // Address-size override, 67H
                    return iptAddressSize;

        case 0x0F:  // Streaming SIMD Extensions prefix.
                    return iptSimdExt;

        default:    return iptNone; // code is not a prefix
       }   
   }



inline
OPCODE_FLAGS_T getOpcodeFlags1(unsigned opcode)
   {
    static OPCODE_FLAGS_T opcodes1[256] = {
        0x4211, 0x42E4, 0x2011, 0x20E4, 0x8401, 0x8C42, 0x0000, 0x0000, 0x4211, 0x42E4,
        0x2011, 0x20E4, 0x8401, 0x8C42, 0x0000, 0x0000, 0x4211, 0x42E4, 0x2011, 0x20E4,
        0x8401, 0x8C42, 0x0000, 0x0000, 0x4211, 0x42E4, 0x2011, 0x20E4, 0x8401, 0x8C42,
        0x0000, 0x0000, 0x4211, 0x42E4, 0x2011, 0x20E4, 0x8401, 0x8C42, 0x0000, 0x8000,
        0x4211, 0x42E4, 0x2011, 0x20E4, 0x8401, 0x8C42, 0x0000, 0x8000, 0x4211, 0x42E4,
        0x2011, 0x20E4, 0x8401, 0x8C42, 0x0000, 0x8000, 0x0211, 0x02E4, 0x0011, 0x00E4,
        0x0401, 0x0C42, 0x0000, 0x8000, 0x6045, 0x6045, 0x6045, 0x6045, 0x6045, 0x6045,
        0x6045, 0x6045, 0x6045, 0x6045, 0x6045, 0x6045, 0x6045, 0x6045, 0x6045, 0x6045,
        0x0045, 0x0045, 0x0045, 0x0045, 0x0045, 0x0045, 0x0045, 0x0045, 0x6045, 0x6045,
        0x6045, 0x6045, 0x6045, 0x6045, 0x6045, 0x6045, 0x0000, 0x8000, 0x00E4, 0x421A,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0C00, 0x2CE4, 0x0400, 0x24E4, 0x0000, 0x0000,
        0x0000, 0x0000, 0x1400, 0x1400, 0x1400, 0x1400, 0x1400, 0x1400, 0x1400, 0x1400,
        0x1400, 0x1400, 0x1400, 0x1400, 0x1400, 0x1400, 0x1400, 0x1400, 0x0510, 0x0DA0,
        0x0510, 0x05A0, 0x0211, 0x02E4, 0xA211, 0xA2E4, 0x4211, 0x42E4, 0x2011, 0x20E4,
        0x42E3, 0x20E4, 0x00E3, 0x01A0, 0x0000, 0xE046, 0xE046, 0xE046, 0xE046, 0xE046,
        0xE046, 0xE046, 0x8000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000,
        0x8101, 0x8142, 0x0301, 0x0342, 0x0000, 0x0000, 0x0000, 0x0000, 0x0401, 0x0C42,
        0x0000, 0x0000, 0x8000, 0x8000, 0x0000, 0x0000, 0x6404, 0x6404, 0x6404, 0x6404,
        0x6404, 0x6404, 0x6404, 0x6404, 0x6C45, 0x6C45, 0x6C45, 0x6C45, 0x6C45, 0x6C45,
        0x6C45, 0x6C45, 0x4510, 0x45A0, 0x0800, 0x0000, 0x20E4, 0x20E4, 0x4510, 0x4DA0,
        0x0000, 0x0000, 0x0800, 0x0000, 0x0000, 0x0400, 0x0000, 0x0000, 0x4110, 0x41A0,
        0x4110, 0x41A0, 0x8400, 0x8400, 0x0000, 0x8000, 0x0008, 0x0008, 0x0008, 0x0008,
        0x0008, 0x0008, 0x0008, 0x0008, 0x1400, 0x1400, 0x1400, 0x1400, 0x8401, 0x8442,
        0x0601, 0x0642, 0x1C00, 0x1C00, 0x0000, 0x1400, 0x8007, 0x8047, 0x0207, 0x0247,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0008, 0x0008, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x4110, 0x01A0
      };
    return opcodes1[opcode&0xFF];
   }


inline
OPCODE_FLAGS_T getOpcodeFlags2(unsigned opcode)
   {
    static OPCODE_FLAGS_T opcodes2[256] = {
        0x0118, 0x0120, 0x20E4, 0x20E4, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0xFFFF, 0xFFFF, 0xFFFF, 0x0110, 0x0000, 0x052D, 0x003F, 0x023F, 0x003F, 0x023F,
        0x003F, 0x003F, 0x003F, 0x023F, 0x0110, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
        0xFFFF, 0xFFFF, 0x4023, 0x4023, 0x0223, 0x0223, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
        0x003F, 0x023F, 0x002F, 0x023F, 0x003D, 0x003D, 0x003F, 0x003F, 0x0000, 0x8000,
        0x8000, 0x8000, 0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x20E4, 0x20E4, 0x20E4, 0x20E4, 0x20E4, 0x20E4,
        0x20E4, 0x20E4, 0x20E4, 0x20E4, 0x20E4, 0x20E4, 0x20E4, 0x20E4, 0x20E4, 0x20E4,
        0x4227, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F,
        0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x00ED, 0x00ED, 0x00ED, 0x00ED,
        0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED,
        0x0065, 0x00ED, 0x04ED, 0x04A8, 0x04A8, 0x04A8, 0x00ED, 0x00ED, 0x00ED, 0x0000,
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0265, 0x02ED, 0x1C00, 0x1C00,
        0x1C00, 0x1C00, 0x1C00, 0x1C00, 0x1C00, 0x1C00, 0x1C00, 0x1C00, 0x1C00, 0x1C00,
        0x1C00, 0x1C00, 0x1C00, 0x1C00, 0x4110, 0x4110, 0x4110, 0x4110, 0x4110, 0x4110,
        0x4110, 0x4110, 0x4110, 0x4110, 0x4110, 0x4110, 0x4110, 0x4110, 0x4110, 0x4110,
        0x0000, 0x0000, 0x8000, 0x02E4, 0x47E4, 0x43E4, 0xC211, 0xC2E4, 0x0000, 0x0000,
        0x0000, 0x42E4, 0x47E4, 0x43E4, 0x0020, 0x20E4, 0xC211, 0xC2E4, 0x20E4, 0x42E4,
        0x20E4, 0x22E4, 0x2154, 0x211C, 0xFFFF, 0xFFFF, 0x05A0, 0x42E4, 0x20E4, 0x20E4,
        0x2154, 0x211C, 0xA211, 0xA2E4, 0x043F, 0x0224, 0x0465, 0x24AC, 0x043F, 0x8128,
        0x6005, 0x6005, 0x6005, 0x6005, 0x6005, 0x6005, 0x6005, 0x6005, 0xFFFF, 0x00ED,
        0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x02ED, 0x20AC, 0x00ED, 0x00ED, 0x00ED, 0x00ED,
        0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED,
        0x003F, 0x02ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED,
        0xFFFF, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED,
        0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x00ED, 0x0000                            
      };
    return opcodes2[opcode&0xFF]; // clears high part of opcode
   }

inline
OPCODE_FLAGS_T getOpcodeFlags3(unsigned ext, unsigned opcode)
   {
    static OPCODE_FLAGS_T opcodes3[10][16] = {
         {0x0510, 0xFFFF, 0x4110, 0x4110, 0x8110, 0x8110, 0x8110, 0x8110, 0x0510, 0xFFFF,
          0x4110, 0x4110, 0x8110, 0x8110, 0x8110, 0x8110},
         {0x0DA0, 0xFFFF, 0x41A0, 0x41A0, 0x81A0, 0x81A0, 0x81A0, 0x81A0, 0x0DA0, 0xFFFF,
          0x41A0, 0x41A0, 0x81A0, 0x81A0, 0x81A0, 0x81A0},
         {0x0120, 0x0120, 0x0120, 0x0120, 0x0120, 0x0120, 0x0120, 0x0120, 0x0036, 0x0036,
          0x0030, 0x0030, 0x0036, 0x0036, 0x0036, 0x0036},
         {0x0120, 0xFFFF, 0x0120, 0x0120, 0x0110, 0x0118, 0x0110, 0x0118, 0x0030, 0x0030,
          0x0000, 0x0030, 0x0000, 0x0000, 0x0000, 0x0000},
         {0x0120, 0x0120, 0x0120, 0x0120, 0x0120, 0x0120, 0x0120, 0x0120, 0x0036, 0x0036,
          0x0036, 0x0036, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF},
         {0x0120, 0xFFFF, 0x0120, 0x0120, 0xFFFF, 0x0130, 0xFFFF, 0x0130, 0x0036, 0x0036,
          0x0036, 0x0036, 0x0000, 0x0036, 0x0036, 0x0000},
         {0x0128, 0x0128, 0x0128, 0x0128, 0x0128, 0x0128, 0x0128, 0x0128, 0x0236, 0x0236,
          0x0030, 0x0030, 0x0236, 0x0236, 0x0236, 0x0236},
         {0x0128, 0xFFFF, 0x0128, 0x0128, 0x0110, 0xFFFF, 0x0110, 0x0118, 0x0030, 0x0030,
          0x0030, 0x0030, 0x0030, 0x0030, 0xFFFF, 0xFFFF},
         {0x0118, 0x0118, 0x0118, 0x0118, 0x0118, 0x0118, 0x0118, 0x0118, 0x0236, 0x0236,
          0x0030, 0x0236, 0x0236, 0x0236, 0x0236, 0x0236},      
         {0x0118, 0xFFFF, 0x0118, 0x0118, 0x0130, 0x0128, 0x0130, 0x0128, 0x0030, 0x0030,
          0x0030, 0x0030, 0x0000, 0x0036, 0x0036, 0xFFFF}
      };
    return opcodes3[ext][opcode&0xF];
   }

//#ifndef SIZE_OF_CODE_DISABLE_PRINT
/*
void dumpCodeBytes(CODE_CONST_PTR pCode, CODE_SIZE_T instructionSize)
   {
    for(CODE_SIZE_T i=0; i!=instructionSize; ++i)
       {
        if (i) printf(" %02X", (unsigned)*pCode++);
        else   printf("%02X" , (unsigned)*pCode++);
       }
   }
*/

inline
std::string hexString( unsigned hex, unsigned numDigits)
   {
    char buf[32];
    #if defined(_MSC_VER) && _MSC_VER>=1300
    int charsPrinted = sprintf_s(buf, sizeof(buf)/sizeof(buf[0]), "%%0%dX", (numDigits>16 ? 16 : numDigits) );
    #else
    int charsPrinted = sprintf(buf, "%%0%dX", (numDigits>16 ? 16 : numDigits) );
    #endif

    char buf2[32];
    #if defined(_MSC_VER) && _MSC_VER>=1300
    charsPrinted = sprintf_s(buf2, sizeof(buf2)/sizeof(buf2[0]), buf, hex);
    #else
    charsPrinted = sprintf(buf2, buf, hex);
    #endif

    return std::string(buf2);
   }

inline
std::string dumpCodeBytes(CODE_CONST_PTR pCode, CODE_SIZE_T instructionSize)
   {
    ::std::string strRes;
    char buf[32];
    #if defined(_MSC_VER) && _MSC_VER>=1300
    int charsPrinted = sprintf_s(buf, sizeof(buf)/sizeof(buf[0]), "%08X  ", (unsigned)pCode);
    #else
    int charsPrinted = sprintf(buf, "%08X  ", (unsigned)pCode);
    #endif

    strRes.append(buf, charsPrinted);

    for(CODE_SIZE_T i=0; i!=instructionSize; ++i)
       {
        //#if defined(WIN32) || defined(_WIN32)
        #if defined(_MSC_VER) && _MSC_VER>=1300
        charsPrinted = sprintf_s(buf, sizeof(buf)/sizeof(buf[0]), "%02X", (unsigned)*pCode++);
        #else
        charsPrinted = sprintf(buf, "%02X", (unsigned)*pCode++);
        #endif
        if (i) strRes.append( 1, ' ');
        strRes.append(buf, charsPrinted);
       }
    return strRes;
   }

template <typename T>
std::string flagsString( T val, const ::std::map< T, ::std::string > flagsMap )
   {
    if (!val) return ::std::string("<EMPTY>");
    std::string resStr;
    ::std::map< T, ::std::string >::const_iterator it = flagsMap.begin();
    for(; it!=flagsMap.end(); ++it)
       {
        if ((val&it->first) != it->first) continue;
        if (!resStr.empty()) resStr.append(" | ");
        resStr.append(it->second);
        val &= ~it->first;
       }
    if (resStr.empty())
       {
        return hexString( (unsigned)val, 8);
       }
    return resStr;
   }

template <typename T>
struct CAddFlag
{
    //::std::map< T, ::std::string > &m;
    T                              val;
    const ::std::string            flagStr;

    CAddFlag( /* ::std::map< T, ::std::string > &_m, */ T v, const ::std::string &fs )
       //: m(_m)
       : val(v)
       , flagStr(fs) 
       {}

    //CAddFlag( /* ::std::map< T, ::std::string > &_m, */ T v, const char *str )
    //   //: m(_m)
    //   : val(v)
    //   , flagStr(str) 
    //   {}
};

template <typename T>
CAddFlag<T> addFlag( T t, const ::std::string &fs)
   {
    return CAddFlag<T>(t, fs);
   }

template <typename T>
CAddFlag<T> addFlag( T t, const char*fs)
   {
    return CAddFlag<T>(t, ::std::string(fs));
   }

template <typename T>
::std::map< T, ::std::string > & operator%( ::std::map< T, ::std::string > &m, const CAddFlag< T > &af )
   {
    m[af.val] = af.flagStr;
    return m;
   }

template <typename T>
::std::map< T, ::std::string > makeFlags( T v, const ::std::string &fs )
   {
    ::std::map< T, ::std::string > m;
    m[v] = fs;
    return m;
   }





//inline
template <typename TStream>
CODE_SIZE_T instructionSize( bool mode32, CODE_CONST_PTR pCode, OPCODE_T *pOpcode, const TStream &stream)
   {
    //stream<<"Test";

    bool addr32    = mode32;
    bool operand32 = mode32;

    CODE_T repeatCode      = 0;
    CODE_T segmentOverride = 0;
    bool   twoByteOpcode   = false;

    OPCODE_FLAGS_T opcodeFlags = 0;
    OPCODE_T       opcode = 0;

    CODE_CONST_PTR pCodeOrg = pCode;

    EInstructionPrefixType prefixType = getPrefixType( *pCode );
    while(prefixType!=iptNone && !twoByteOpcode)
       {
        switch(prefixType)
           {
            case iptRepeat     : repeatCode      = *pCode++; 
                                 break;
            case iptSegment    : segmentOverride = *pCode++; 
                                 break;
            case iptOperandSize: operand32 = !operand32; // inverse operand size (switch 16/32)
                                 ++pCode;
                                 break;
            case iptAddressSize: addr32 = !addr32; // inverse addr size (switch 16/32)
                                 ++pCode;
                                 break;
            case iptSimdExt    : twoByteOpcode = true;
                                 break;
           }
        if (!twoByteOpcode) // long opcode prefix is last
           prefixType = getPrefixType( *pCode );
       }

    opcode = (OPCODE_T)*pCode++;

    if (!twoByteOpcode)
       {
        opcodeFlags = getOpcodeFlags1(opcode);
       }
    else
       {
        opcode = (OPCODE_T)*pCode++;
        opcodeFlags = getOpcodeFlags2(opcode);
        opcode += 0x0F00;
       }

    if (pOpcode) *pOpcode = opcode; // return opcode for analizing to upper level

    CODE_SIZE_T operandSize = operand32 ? 4 : 2;

    CODE_T       modrm = 0;
    CODE_T       rm    = 0;
    CODE_SIZE_T  displacementSize  = 0; 

    if (opcodeFlags&0x0038)
       {
        modrm = *pCode++;
        rm    = modrm & 0x07;

        // displacement to be added to index in operations with indexed addresses
        switch(modrm&0xC0) // 0b1100 0000 - mask mod field
           {
            case 0x40: displacementSize = 1; // 8 bit signed displacement folows the mod/rm byte
                       break;
            case 0x80: if (addr32) displacementSize = 4; // 16/32 bit displacement folows the mod/rm byte
                       else        displacementSize = 2; 
                       break;
            default:   displacementSize = 0;
           }

        if ((modrm&0xC0)!=0xC0 && addr32)
           {
            if (rm==4)
               {
                // need SIB byte
                CODE_T sib = *pCode++;
                //rm = sib & 0x07; // ???
                // Rm := byte(Code^) and $7;
               }
            else if (rm==5 && !(modrm&0xC0))
               {
                displacementSize = 4;
               }           
           }
        pCode += displacementSize;

        if ((opcodeFlags&0x0038)==0x0008)
           { // op code extention used from mod/rm filed reg/opcode
            unsigned extIdx = 0;
            switch(opcode)
               {
                case 0xF6:  extIdx = 0;  break;
                case 0xF7:  extIdx = 1;  break;
                case 0xD8:  extIdx = 2;  break;
                case 0xD9:  extIdx = 3;  break;
                case 0xDA:  extIdx = 4;  break;
                case 0xDB:  extIdx = 5;  break;
                case 0xDC:  extIdx = 6;  break;
                case 0xDD:  extIdx = 7;  break;
                case 0xDE:  extIdx = 8;  break;
                case 0xDF:  extIdx = 9;  break;
               }
            if ((modrm&0xC0) != 0xC0)
               opcodeFlags = getOpcodeFlags3( extIdx, (modrm>>3)&0x07 );
            else
               opcodeFlags = getOpcodeFlags3( extIdx, (modrm>>3)&0x07 + 8 );
          // if ((Modrm and $c0) <> $c0) then
          //   Flags := Opcodes3[Extend][(Modrm shr 3) and $7] 
          // else
          //   Flags := Opcodes3[Extend][((Modrm shr 3) and $7) + 8];
           }

       }

    switch(opcodeFlags&0x0C00)
       {
        case 0x0400:  pCode += 1;
                break;
        case 0x0800:  pCode += 2;
                break;
        case 0x0C00:  pCode += operandSize;
                     //opSized = operand32 ? 4 : 2;
                break;
        default:
            {            
             switch(opcode)
                {
                 case 0x9A: case 0xEA: pCode += operandSize + 2; break;
                 case 0xC8:            pCode += 3;               break;
                 case 0xA0:
                 case 0xA1:
                 case 0xA2:
                 case 0xA3:
                         if (addr32) pCode += 4;
                         else        pCode += 2;
                         break;
                }
            }

       }

    return pCode - pCodeOrg;
   }



struct CInstructionHandlerBase
{

    template <typename T>
    const CInstructionHandlerBase& operator<<( const T &t) const
       {
        return *this;
       }

    bool isRetOpcode(OPCODE_T opcode) const { if (opcode==0xC3 || opcode==0xC2) return true; return false;}

    bool operator()( CODE_CONST_PTR pCode, CODE_SIZE_T size, OPCODE_T opcode) const
       {
        return !isRetOpcode(opcode);
       }
};

template <typename THandler>
CODE_SIZE_T functionSize( bool mode32, CODE_CONST_PTR pCode, const THandler &handler)
   {
    CODE_CONST_PTR pCodeOrg = pCode;
    OPCODE_T opcode   = 0;
    CODE_SIZE_T iSize = 0;
    do {
        pCode += iSize;
        iSize = instructionSize( mode32, pCode, &opcode, handler);
       } while( handler(pCode, iSize, opcode) );
    return pCode - pCodeOrg;
   }

struct CInstructionPrintDumpStdCout : public CInstructionHandlerBase
{
    // buffer for printing instruction disassebler
    //mutable ::std::ostringstream oss;
    mutable ::std::string instructionAsmBuf;

    template <typename T>
    const CInstructionPrintDumpStdCout& operator<<( const T &t) const
       {
        ::std::ostringstream oss;
        oss<<t;
        instructionAsmBuf.append(oss.str());
        return *this;
       }

    bool operator()( CODE_CONST_PTR pCode, CODE_SIZE_T size, OPCODE_T opcode) const
       {
        ::std::string strDump = dumpCodeBytes(pCode, size);
        if (!instructionAsmBuf.empty())
           {
            if (strDump.size()<34) strDump.append( 34 - strDump.size(), ' ');
            strDump.append( 2, ' ');
           }
        ::std::cout<<strDump<<instructionAsmBuf<<"\n";
        instructionAsmBuf.clear();
        return !isRetOpcode(opcode);
       }
};

inline
CODE_SIZE_T functionSize( bool mode32, CODE_CONST_PTR pCode)
   {
    return functionSize( mode32, pCode,  /* CInstructionPrintDumpStdCout */ CInstructionHandlerBase() );
   }


// VirtualProtect helper
inline
bool setProtectForCode(CODE_CONST_PTR pCode, DWORD newProtect, DWORD *oldProtect)
   {
    MEMORY_BASIC_INFORMATION mbi;
    SIZE_T resSize = ::VirtualQuery( (LPCVOID)pCode, &mbi, sizeof(mbi));
    if (resSize<sizeof(mbi)) return false;

    DWORD tmpOldProtect = 0;
    BOOL bRes = VirtualProtect( mbi.BaseAddress, mbi.RegionSize, newProtect, &tmpOldProtect);
    if (!bRes) return false;

    if (oldProtect) *oldProtect = tmpOldProtect;
    return true;
   }

class CVirtualAutoProtect
{
    DWORD           oldProtect;
    CODE_CONST_PTR  ptr;
    bool            needRestore;
public:
    CVirtualAutoProtect(CODE_CONST_PTR p, DWORD newProtect)
       : oldProtect(0), ptr(p), needRestore(false)
       {
        needRestore = setProtectForCode( ptr, newProtect, &oldProtect);
       }

    ~CVirtualAutoProtect()
       {
        setProtectForCode( ptr, oldProtect, 0);
       }
};



enum ECheckHotPatchResult
{
    // good conditions
    chpOk                 =  0,
    chpOkAllready         =  1, // allready hotpached
    chpOkAlternate        =  2,
    chpProcAllreadyPatched=  3,
    // bad conditions
    chpNoSpaceForHP       = -1, // there is no space for hot patch 
                                // - space for short jump (2-bytes NOP) required in function prolog
    chpNoSpaceForLongJump = -2, // there is no space for long relative jump
    chpUnknown            = -3, // short jump installed, but long jump not installed, or other errors
    chpUnknown1           = -4, // some other error 1
    chpUnknown2           = -5, // some other error 2
    chpUnknown3           = -6, // some other error 3
    chpViatualAllocFailed = -6, 
    chpUnknown4           = -7, // some other error 4
    chpUnknown5           = -8, // some other error 5
    chpUnknown6           = -9, // some other error 6
    chpUnknown7           = -10,// some other error 7
    chpUnknown8           = -11,// some other error 8
    chpUnknown9           = -12,// some other error 9
};

inline
const char * getECheckHotPatchResultStr(ECheckHotPatchResult e)
   {
    switch(e)
       {
        case chpOk:             return "Ok";
        case chpOkAllready:     return "Allready hotpached";
        case chpProcAllreadyPatched: return "Proc allready patched (found in patched database)";

        case chpOkAlternate:    return "Alternative method used for not ready for hot patch code";
        case chpNoSpaceForHP:   return "There is no space for hot patch - space for short jump (2-bytes NOP) required in function prolog";
        case chpNoSpaceForLongJump: return "There is no space for long relative jump";
        case chpUnknown:        return "Short jump installed, but long jump not installed, or other error";
        case chpUnknown1:       return "Some other error 1";
        case chpUnknown2:       return "Some other error 2";
        case chpViatualAllocFailed: return "ViatualAlloc failed (alternate hook method)";
        //case chpUnknown3:       return "Some other error 3";
        case chpUnknown4:       return "Some other error 4";
        case chpUnknown5:       return "Some other error 5";
        case chpUnknown6:       return "Some other error 6";
        case chpUnknown7:       return "Some other error 7";
        case chpUnknown8:       return "Some other error 8";
        case chpUnknown9:       return "Some other error 9";

        default:                return "Unknown error";
       }
   }

    

//CODE_SIZE_T functionSize( bool mode32, CODE_CONST_PTR pCode, bool bPrint = true)
inline
ECheckHotPatchResult checkHotPatchPossibility(CODE_CONST_PTR pCode)
   {
    // 8B FF  mov edi,edi - two byte NOP that can be replaced with short relative jump
    // EB F9  jmp $,-5 (jmp [IP-7])
    bool sjAllreadyInstalled = false; // short jump allready installed
    if ((*pCode)!=0x8B || (*(pCode+1))!=0xFF) 
       {
        if ((*pCode)!=0xEB || (*(pCode+1))!=0xF9) 
           return chpNoSpaceForHP; 
        sjAllreadyInstalled = true;
       }

    // systems that allows hot paching allocates 5 bytes before each function with NOP (opcode 90)
    // to reserve space for long relative jump
    pCode -= 5;
    
    if (sjAllreadyInstalled)
       { // is long jump allready installed?
        if (*pCode==0xE9) return chpOkAllready;
        else              return chpUnknown;
       }

    for(unsigned i=0; i!=5; ++i)
       {
        if (*pCode++!=0x90) return chpNoSpaceForLongJump;
       }
    return chpOk;
   }

// Windows Me/98/95:  You cannot use VirtualProtect on any memory region located in the shared virtual address space (from 0x80000000 through 0xBFFFFFFF).

inline
ECheckHotPatchResult installHotPatch( CODE_PTR pPatchedFunction, CODE_PTR pNewFn /* absolute */, ADDR32_T *jumpAddr /* absolute addr of original function code */)
   {
    CODE_PTR pPatchedFunctionOrg = pPatchedFunction;
    const CODE2BYTES_T moveEdi      = 0xFF8B; // 8B FF - mov edi,edi
    const CODE2BYTES_T jmpShortCode = 0xF9EB;

    if ((*pPatchedFunction)!=0x8B || (*(pPatchedFunction+1))!=0xFF)
       { // not a 8B FF  mov edi,edi instruction
        // is short jump installed
        if ((*pPatchedFunction)!=0xEB || (*(pPatchedFunction+1))!=0xF9) return chpNoSpaceForHP; // not installed, something other code found

        pPatchedFunction -= 5;
        if (*pPatchedFunction!=0xE9) return chpUnknown; // short jump installed, but long jump not found
        // Ok, long jump found
        ++pPatchedFunction;
        // calc absolute addr as relative (from jump instruction) + original patched function addr
        if (jumpAddr) *jumpAddr = *((ADDR32_T*)pPatchedFunction) + (ADDR32_T)pPatchedFunctionOrg;

        CVirtualAutoProtect protect(pPatchedFunction, PAGE_EXECUTE_READWRITE);

        *((CODE2BYTES_T*)pPatchedFunctionOrg) = moveEdi; // restore 2 byte nop in function prolog

        // replace long jump address
        ADDR32_T newRel = (ADDR32_T)(pNewFn - pPatchedFunctionOrg);
        *((ADDR32_T*)pPatchedFunction) = newRel;
        
        *((CODE2BYTES_T*)pPatchedFunctionOrg) = jmpShortCode; // set up short jump
        return chpOkAllready; // FN patched, absolute add of previos patch returned
       }

    // 2 byte NOP detected, no any hot patch installed, clear setup
    pPatchedFunction -= 5;
    for(unsigned i=0; i!=5; ++i)
       {
        if (*pPatchedFunction++!=0x90) return chpNoSpaceForLongJump;
       }
       
    pPatchedFunction -= 5;

    CVirtualAutoProtect protect(pPatchedFunction, PAGE_EXECUTE_READWRITE);

    *pPatchedFunction++ = 0xE9; // install long jump  opcode
    ADDR32_T newRel = ADDR32_T(pNewFn - pPatchedFunctionOrg);
    *((ADDR32_T*)pPatchedFunction) = newRel;

    if (jumpAddr) *jumpAddr = (ADDR32_T)(pPatchedFunctionOrg + 2); // skipping chunk

    *((CODE2BYTES_T*)pPatchedFunctionOrg) = jmpShortCode; // set up short jump

    return chpOk;
   }


struct CCodeSizeNotLess : public CInstructionHandlerBase
{
    CODE_SIZE_T  notLess;
    mutable CODE_SIZE_T  curSize;
    CCodeSizeNotLess(CODE_SIZE_T nl) : notLess(nl), curSize(0) {}

    template <typename T>
    const CInstructionPrintDumpStdCout& operator<<( const T &t) const
       {
        return *this;
       }

    bool operator()( CODE_CONST_PTR pCode, CODE_SIZE_T size, OPCODE_T opcode) const
       {
        curSize += size;
        if (curSize>=notLess) return false;
        return true;
       }
};


inline
ECheckHotPatchResult installPatchOldStyle( CODE_PTR pPatchedFunction, CODE_PTR pNewFn /* absolute */, ADDR32_T *jumpAddr /* absolute addr of original function code */)
   {
    // UNDONE: need to freeze all threads
    /*
     1) Вычислить размер кода
     2) Выделить память под код. (*jumpAddr = allocatedMem)
     3) Скопировать код оригинальной функции длинной не менее 5ти байт
     4) В конец перемещенного кода дописать переход на продолжение оригинальной функции
     5) Записать по адресу оригинальной функции код перехода на функцию перехвата
    */

    CODE_PTR pPatchedFunctionOrg = pPatchedFunction;

    // 1
    CCodeSizeNotLess notLess(5);
    CODE_SIZE_T needSize = functionSize( true, pPatchedFunctionOrg, notLess);
    if (needSize<notLess.curSize) needSize = notLess.curSize;

    // 2
    LPVOID allocatedCodeMem = VirtualAlloc( 0, (SIZE_T)(needSize+5), MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE );
    if (!allocatedCodeMem)
       return chpViatualAllocFailed;

    MEMORY_BASIC_INFORMATION mbi;
    SIZE_T vqResSize = ::VirtualQuery( (LPCVOID)pPatchedFunctionOrg, &mbi, sizeof(mbi));
    if (vqResSize<sizeof(mbi))
       return chpViatualAllocFailed;

    DWORD oldProtect = 0;
    BOOL vpRes = VirtualProtect( mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_WRITECOPY, &oldProtect);
    if (!vpRes)
       return chpViatualAllocFailed;

    *jumpAddr = (ADDR32_T)allocatedCodeMem;

    // 3
    memcpy( allocatedCodeMem, (const void*)pPatchedFunctionOrg, needSize );

    // 4
    ADDR32_T jumpFromAddr = (ADDR32_T)allocatedCodeMem + (ADDR32_T)needSize + 5; // IP points to this addr
    ADDR32_T jumpToAddr   = (ADDR32_T)pPatchedFunctionOrg + (ADDR32_T)needSize; // rest of original function
    ADDR32_T newRel = ADDR32_T(jumpToAddr - jumpFromAddr);

    CODE_PTR jumpInstructionAddr = (CODE_PTR)allocatedCodeMem + needSize;
    *jumpInstructionAddr++ = 0xE9;
    *((ADDR32_T*)jumpInstructionAddr) = newRel;

    //VirtualProtect
    // 5
    jumpFromAddr = (ADDR32_T)pPatchedFunctionOrg + 5;
    //jumpToAddr   = (ADDR32_T)allocatedCodeMem; // this is a BUG
    jumpToAddr   = (ADDR32_T)pNewFn;
    
    newRel = ADDR32_T(jumpToAddr - jumpFromAddr);
    *pPatchedFunction++ = 0xE9;
    *((ADDR32_T*)pPatchedFunction) = newRel;

    VirtualProtect( mbi.BaseAddress, mbi.RegionSize, oldProtect, &oldProtect);

    return chpOkAlternate;
   }

//typedef unsigned short CODE1BYTE_T;  // one byte instruction
//typedef unsigned short CODE2BYTES_T; // two bytes instruction
//typedef unsigned       ADDR32_T; // two bytes instruction


inline
ECheckHotPatchResult installPatch( CODE_PTR pPatchedFunction, CODE_PTR pNewFn /* absolute */, ADDR32_T *jumpAddr /* absolute addr of original function code */)
   {
    ECheckHotPatchResult res = installHotPatch( pPatchedFunction, pNewFn, jumpAddr);
    if ((int)res >= 0) return res;
    return installPatchOldStyle( pPatchedFunction, pNewFn, jumpAddr);
   }




}; // namespace x86

}; // namespace marty

#endif /* X86_DISASM_H */

