#ifndef TPL_H
#define TPL_H


#if !defined(_MAP_) && !defined(_STLP_MAP) && !defined(__STD_MAP__) && !defined(_CPP_MAP) && !defined(_GLIBCXX_MAP)
    #include <map>
#endif

#if !defined(_VECTOR_) && !defined(_STLP_VECTOR) && !defined(__STD_VECTOR__) && !defined(_CPP_VECTOR) && !defined(_GLIBCXX_VECTOR)
    #include <vector>
#endif

#if !defined(_STRING_) && !defined(_STLP_STRING) && !defined(__STD_STRING) && !defined(_CPP_STRING) && !defined(_GLIBCXX_STRING)
    #include <string>
#endif

#if !defined(_INC_TIME) && !defined(_TIME_H_) && !defined(_TIME_H)
    #include <time.h>
#endif


#include "cfarcls.h"

namespace TextTP
{

bool templateFindCursor(std::vector<std::string> &lines, int &pos, int &line);
bool templateFindCursorInLine(std::string &str, int &pos);
bool templateIsFile(const std::string &tpl_name, std::string &file_name);
//std::string templateGetText(const std::string &tpl_name);
std::string templateParse( const std::string &tpl_text, 
                       const std::string &editor_file_name, 
                       const std::string &tpl_name);
bool templateGetValue( const std::string &tpl_name, 
                       std::map<std::string, std::string> &val_map, 
                       const std::string &val_name, 
                       std::string &value);

void templateMakeStdMacrosses( std::map<std::string, std::string> &val_map, 
                               const std::string &editor_file_name);
void templateMakeDateTimeMacrosses( std::map<std::string, std::string> &val_map,
                                    struct tm *ptm,
                                    const std::string &type);
std::string templateMakeCSrcName(const std::string &str);
std::string templateCIncGuardName(const std::string &str);
std::string templateCIncFileName(const std::string &str);


void Warning(int title_id, const std::string &text);
void Warning(int title_id, const char *text);

}; // namespace TextTP


#endif /* TPL_H */