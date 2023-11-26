
#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

#if !defined(_VECTOR_) && !defined(_STLP_VECTOR) && !defined(__STD_VECTOR__) && !defined(_CPP_VECTOR) && !defined(_GLIBCXX_VECTOR)
    #include <vector>
#endif

#if !defined(_IOSTREAM_) && !defined(_STLP_IOSTREAM) && !defined(__STD_IOSTREAM__) && !defined(_CPP_IOSTREAM) && !defined(_GLIBCXX_IOSTREAM)
    #include <iostream>
#endif

#if !defined(_FSTREAM_) && !defined(_STLP_FSTREAM) && !defined(__STD_FSTREAM__) && !defined(_CPP_FSTREAM) && !defined(_GLIBCXX_FSTREAM)
    #include <fstream>
#endif

#if !defined(_IOSTREAM_) && !defined(_STLP_IOSTREAM) && !defined(__STD_IOSTREAM__) && !defined(_CPP_IOSTREAM) && !defined(_GLIBCXX_IOSTREAM)
    #include <iostream>
#endif

#if !defined(_ITERATOR_) && !defined(_STLP_ITERATOR) && !defined(__STD_ITERATOR__) && !defined(_CPP_ITERATOR) && !defined(_GLIBCXX_ITERATOR)
    #include <iterator>
#endif

#if !defined(_MAP_) && !defined(_STLP_MAP) && !defined(__STD_MAP__) && !defined(_CPP_MAP) && !defined(_GLIBCXX_MAP)
    #include <map>
#endif

#include "../../fmtUtils.h"


#if !defined(TEXT_FMT_H)
    #include "text-fmt.h"
#endif


//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    if (argc<2)
       {
        std::cout<<"No filename taken, exiting\n";
        return 0;
       }

    unsigned fmtType = ftWidth;
    int fileNameIdx = 1;

    if (std::string(argv[fileNameIdx])=="-width")
       { fileNameIdx++; fmtType = ftWidth; }
    else if (std::string(argv[fileNameIdx])=="-center")
       { fileNameIdx++; fmtType = ftCenter; }
    else if (std::string(argv[fileNameIdx])=="-left")
       { fileNameIdx++; fmtType = ftLeft; }
    else if (std::string(argv[fileNameIdx])=="-right")
       { fileNameIdx++; fmtType = ftRight; }

    if (argc<fileNameIdx+1)
       {
        std::cout<<"No filename taken, exiting\n";
        return 0;
       }


    std::ifstream in(argv[fileNameIdx], std::ios::in | std::ios::binary); 
    if (!in) 
       {
        std::cout<<"Failed to open file '"<<argv[fileNameIdx]<<"'\n";
        return 0;
       }

    std::string text;

    std::istream_iterator<char> inBegin(in), end;
    in.unsetf(std::ios::skipws);
    copy(inBegin, end, std::inserter(text, text.end()));

    std::vector<std::string> lines;
    splitTextToLines(lines, text);

    CFormatOptions fmtOptions;
    fmtOptions.globalIndent = 2;
    fmtOptions.firstLineIndent = 2;
    //fmtOptions.lastLineIndent  = 5;
    fmtOptions.lineIndent      = 0;
    fmtOptions.paraWidth       = 78;
    fmtOptions.formatType      = fmtType;

    std::cout<<formatText(fmtOptions, lines);

/*
    for(std::vector<std::string>::iterator it=lines.begin(); it!=lines.end(); ++it)
       {
        std::cout<<"-- "<<*it<<"\n";
       
       }
*/
    return 0;
}

