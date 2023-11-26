#ifndef CCOMMENT_H
#define CCOMMENT_H



//#define TTPINP_CANCEL   0    /* User cancels input */
//#define TTPINP_DIALOG   1    /* Dialog used for obtaining shortcut */
//#define TTPINP_TEXT     2    /* Shortcut name is in editor */



namespace TextTP
{

void doCommentingJob();
void configureCommenting();
int findFirstNonSpace(const EditorInfo &edinfo, const char *str, int strLen);
int findLastNonSpace(const EditorInfo &edinfo, const char *str, int strLen);
int findLeftmostNonSpace(const EditorInfo &edinfo, int startLine, int endLine);
int findMaxStringLen(const EditorInfo &edinfo, int startLine, int endLine);

inline
std::string makeCharString(int nChars, char ch)
   {
    return std::string((std::string::size_type)nChars, ch);
   }

inline
std::string makeSpaceString(int nSpaces)
   {
    return std::string((std::string::size_type)nSpaces, ' ');
    /* std::string res;
     * for(int i=0; i<nSpaces; ++i)
     *    res += " ";
     * return res;
     */
   }


}; // namespace TextTP


#endif /* CCOMMENT_H */


