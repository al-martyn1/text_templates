
#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif


#if !defined(_IOSTREAM_) && !defined(_STLP_IOSTREAM) && !defined(__STD_IOSTREAM__) && !defined(_CPP_IOSTREAM) && !defined(_GLIBCXX_IOSTREAM)
    #include <iostream>
#endif



int main(int argc, char* argv[])
   {
    std::string str;
    str = "123";
    std::cout<<"["<<str<<"], size: "<<(unsigned)str.size()<<"\n";
    str.append(1, char(0));
    std::cout<<"["<<str<<"], size: "<<(unsigned)str.size()<<"\n";
    return 0;
   }