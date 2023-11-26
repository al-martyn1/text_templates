#ifndef COMPAREWC_H
#define COMPAREWC_H


#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

bool compareWildcardStrings(const std::string &str, const std::string &strWC);


#endif /* COMPAREWC_H */

