
#if !defined(_IOSTREAM_) && !defined(_STLP_IOSTREAM) && !defined(__STD_IOSTREAM__) && !defined(_CPP_IOSTREAM) && !defined(_GLIBCXX_IOSTREAM)
    #include <iostream>
#endif


#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

#include "../compareWC.cpp"


/*
bool compareWildcardStrings(const std::string &str, const std::string &strWC)
{
    std::string::size_type sizeWC = strWC.size(), size = str.size(), posWC = 0, pos = 0;

    for (; pos<size && posWC<sizeWC; ++pos, ++posWC)
        {
         if (strWC[posWC]=='*') return true; // wildcard ends with '*' - strings are equal
         if (strWC[posWC]=='?') continue; // skip one char
         if (strWC[posWC]!=str[pos]) return false;
        }
    if (pos>=size && posWC<sizeWC)
       { // строка уже закончилась, а маска имени еще нет
        if (strWC[posWC]=='*') return true; // следующий символ в маске имени - '*' - означает и нулевое кол-во симовлов
       }
    // '*' - не был найден в маске
    // строки должны быть равной длинны
    if (size!=sizeWC) return false; // строки разной длинны - маска не подходит имени
    return true; // сроки одной длинны - маска подошла
}
//--------------------------------------------------------------
*/

void testCompare(const std::string &strWC, const std::string &str)
{
    if (compareWildcardStrings(str, strWC))
       std::cout<<"+ "<<strWC<<" equals to "<<str<<"\n";
    else
       std::cout<<"- "<<strWC<<" not equal to "<<str<<"\n";
}
//--------------------------------------------------------------



int main(int argc, char* argv[])
{
    testCompare(".c*p", ".cxxp");

    testCompare(".in*mat*", ".information");
    testCompare(".in*mat*", ".inmat");
    testCompare(".in*mat*", ".infwat");
    testCompare(".in*?at*", ".infwat");
    testCompare(".cpp", ".cxx");
    testCompare(".c??", ".hpp");
    testCompare(".c??", ".cpp");
    testCompare(".c??", ".cxx");
    testCompare(".c*", ".hpp");
    testCompare(".c*", ".cpp");
    testCompare(".c*", ".cxx");
    testCompare(".c*", ".c");

    //testCompare(".", ".");
    return 0;
}