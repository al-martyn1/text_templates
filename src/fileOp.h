#ifndef FILEOP_H
#define FILEOP_H


#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

#ifndef MAX_EXT_SIZE
    #define MAX_EXT_SIZE     15
#endif


std::string getFileExt(const std::string &fn);
std::string getFilePath(const std::string &fn);
bool FileExists(const char *fname);
std::string makeCanonicalFileName(const std::string &name);
std::string makeSystemFileName(const std::string &name);


#endif /* FILEOP_H */

