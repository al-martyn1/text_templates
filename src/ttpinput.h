#ifndef TTPUNPUT_H
#define TTPUNPUT_H


#define TTPINP_CANCEL   0    /* User cancels input */
#define TTPINP_DIALOG   1    /* Dialog used for obtaining shortcut */
#define TTPINP_TEXT     2    /* Shortcut name is in editor */



namespace TextTP
{





//---------------------------------------------------------
struct DialogData
{
    TextTP::TTPConfig                   Config;

    std::string                         editor_file_name;
    std::string                         editor_file_type;
    std::vector<TextTP::ShortcutInfo>   shortcuts;
    TextTP::ShortcutInfo                found_shortcut;
    bool                                bfound;
    bool                                bs_pressed;

 DialogData( const TextTP::TTPConfig &cfg, const std::string &ed_fname);


};
//---------------------------------------------------------



// -1 - error, 0 - no sush file, 1 - ok
int readCustomInfo(const std::string &file, CCustomInfo &ci, std::string &why);
void readCustomConfig(const std::string &ed_fname, TextTP::TTPConfig &cfg);
bool lookupForCustomFile(const std::string &editorFileName, std::string &foundFileName);
bool lookupForFileType(const TextTP::TTPConfig &cfg, const std::string &fileName, std::string &foundFileType, const FARSTANDARDFUNCTIONS* fns);

template<typename IterType, typename VectorType>
void findInfo(const std::string &fileType, IterType B, IterType E, VectorType &V)
   {
    for(; B!=E; ++B)
       {
        if (B->file_type=="*")
           {
            V.push_back(*B);
            continue;
           }
        std::vector<std::string> fileTypesVec;
        TextTP::Utils::split_string(fileTypesVec, B->file_type, ',');
        std::vector<std::string>::const_iterator ftIt = fileTypesVec.begin();
          for(; ftIt!=fileTypesVec.end(); ++ftIt)
             {
              if (*ftIt==fileType)
                 {
                  V.push_back(*B);
                  continue;
                 }
             }
       }   
   }


/* return TTPINP_* */
int  getShortcutInput( DialogData &dd, TextTP::ShortcutInfo &shi, const std::string &editor_file_name, 
                       int &Line, int &Pos, int &len);
void InsertTemplate  ( DialogData &dd, const TextTP::ShortcutInfo &shi, const std::string &editor_file_name,
                       int input_type, int &Line, int Pos, int len);

void doTextTPJob();



}; // namespace TextTP


#endif /* TTPUNPUT_H */

