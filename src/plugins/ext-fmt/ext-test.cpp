
#if !defined(_IOSTREAM_) && !defined(_STLP_IOSTREAM) && !defined(__STD_IOSTREAM__) && !defined(_CPP_IOSTREAM) && !defined(_GLIBCXX_IOSTREAM)
    #include <iostream>
#endif


#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif


#if !defined(_FSTREAM_) && !defined(_STLP_FSTREAM) && !defined(__STD_FSTREAM__) && !defined(_CPP_FSTREAM) && !defined(_GLIBCXX_FSTREAM)
    #include <fstream>
#endif


int main(int argc, char* argv[])
   {
    if (argc<2)
       {
        std::cerr<<"ext-test: not output filename taken\n";
        return 1;
       }

    std::ofstream out(argv[1]);
    if (!out)
       {
        std::cerr<<"ext-test: filed to open output file\n";
        return 1;
       }

    for(int i=0; i<argc; ++i)
       {
        out<<argv[i]<<"\n";
       }

    return 0;
   }