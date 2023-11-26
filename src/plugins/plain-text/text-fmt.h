#ifndef TEXT_FMT_H
#define TEXT_FMT_H

const int ftWidth  = 0;
const int ftCenter = 1;
const int ftLeft   = 2;
const int ftRight  = 3;


struct CFormatOptions
{
    unsigned   globalIndent;
    unsigned   firstLineIndent;
    //unsigned   lastLineIndent;
    unsigned   lineIndent;
    unsigned   paraWidth;
    unsigned   formatType;

    void checkCorrect()
       {
        if (globalIndent>80) globalIndent = 80;
        if (paraWidth<20) paraWidth = 20;
        if (paraWidth>160) paraWidth = 160;
        if (lineIndent>140) lineIndent = 140;
        if (firstLineIndent>140) firstLineIndent = 140;
        //if (lastLineIndent>140) lastLineIndent = 140;

        unsigned totalWidth = lineIndent + paraWidth;
        if (firstLineIndent+20 > totalWidth)
           firstLineIndent = totalWidth - 20;
        //if (lastLineIndent+20 > totalWidth)
        //   lastLineIndent = totalWidth - 20;

        if (formatType>ftRight) formatType = ftWidth;
       }


    
}; // struct CFormatOptions



std::string::size_type findSpace(const std::string &str, std::string::size_type fromPos);
std::string::size_type findNonSpace(const std::string &str, std::string::size_type fromPos);
std::string::size_type findNextSpace(const std::string &str, std::string::size_type fromPos);
std::string formatString(const CFormatOptions &opts, const std::string& str, int width);
std::string formatPara(const CFormatOptions &opts, const std::vector<std::string> &lines);
std::string formatText(const CFormatOptions &opts, const std::vector<std::string> &lines);


//-----------------------------------------------------------------------------
std::string::size_type findSpace(const std::string &str, std::string::size_type fromPos)
   {
    std::string::size_type size = str.size();
    for(; fromPos<size; ++fromPos) if (str[fromPos]==' ') return fromPos;
    return std::string::npos;   
   }

//-----------------------------------------------------------------------------
std::string::size_type findNonSpace(const std::string &str, std::string::size_type fromPos)
   {
    std::string::size_type size = str.size();
    for(; fromPos<size; ++fromPos) if (str[fromPos]!=' ') return fromPos;
    return std::string::npos;
   }

//-----------------------------------------------------------------------------
std::string::size_type findNextSpace(const std::string &str, std::string::size_type fromPos)
   {
    std::string::size_type spPos = findNonSpace(str, fromPos);
    if (spPos==std::string::npos) return spPos;
    return findSpace(str, spPos);
   }

//-----------------------------------------------------------------------------
std::string formatString(const CFormatOptions &opts, const std::string& str, int width)
   {
    if (opts.formatType==ftLeft)
       return str;

    bool lastLine = false;
    if (width<0)
       {
        width = -width;
        lastLine = true;
       }

    if (str.size()>=(unsigned)width)
       return str;

    if (opts.formatType==ftRight)
        return std::string((unsigned)width-str.size(), ' ') + str;
    if (opts.formatType==ftCenter)
        return std::string(((unsigned)width-str.size())/2, ' ') + str;

    if (lastLine)
       return str;

    unsigned spacesInserted = 0;
    std::string res(str);
    std::string::size_type pos = 0;
    while(res.size()<(unsigned)width)
       {
        pos = findNextSpace(res, pos);
        if (pos==std::string::npos)
           {
            if (!spacesInserted)
               return res;
            pos = 0; // next round
           }
        if (pos) 
           {
            res.insert(pos, 1, ' ');
            ++spacesInserted;
           }
       }

    return res;
   }

//-----------------------------------------------------------------------------
std::string formatPara(const CFormatOptions &opts, const std::vector<std::string> &lines)
   {
    std::vector<std::string> words;
    for(std::vector<std::string>::const_iterator it = lines.begin(); it!=lines.end(); ++it)
       {
        splitLineToWords(words, *it);
       }

    unsigned totalWidth = opts.lineIndent + opts.paraWidth;
    unsigned lineLen = opts.paraWidth;
    unsigned firstLineLen = totalWidth - opts.firstLineIndent;

    std::vector<std::string> paraLines;
    std::string curLine;
    //unsigned maxLen = lineLen;

    //std::string res = "---------------------\n";
    for(std::vector<std::string>::const_iterator it = words.begin(); it!=words.end(); )
       {
        unsigned maxLen = paraLines.empty() ? firstLineLen : lineLen;

        // ≈сли строка пуста и текущее слово длиннее макс размера строки
        // добавл€ем слово, форматируем строку (по ширине, по центру, по левому краю, по правому краю)
        // ≈сли при добавление слова длинна строки превысит лимит, то также форматируем

        if ( (curLine.size() + it->size() + 1) > maxLen ||
             (curLine.empty() && it->size() > maxLen)
           )
           { 
            if (curLine.empty())
               curLine = *it++;
            paraLines.push_back(formatString(opts, curLine, maxLen));
            curLine.clear();
            continue;
           }
        if (!curLine.empty()) 
            curLine += std::string(" ");
        curLine += *it++;;
        //res += "\n";
        //res += *it;
       }

    if (!curLine.empty()) 
       paraLines.push_back(formatString(opts, curLine, -((int)lineLen)));


    std::string res;

    for(std::vector<std::string>::const_iterator it = paraLines.begin(); it!=paraLines.end(); ++it)
       {
        if (!res.empty()) res += "\n";
        if (it==paraLines.begin())
           {
            res += std::string( opts.globalIndent + opts.firstLineIndent, ' ') + *it;
           }
        else
           {
            res += std::string( opts.globalIndent + opts.lineIndent, ' ') + *it;
           }
       }

    res += "\n";

    return res;
   }

//-----------------------------------------------------------------------------
std::string formatText(const CFormatOptions &opts, const std::vector<std::string> &lines)
   {
    CFormatOptions options = opts;
    options.checkCorrect();

    std::string res;
    std::vector<std::string> paraLines;
    for(std::vector<std::string>::const_iterator it = lines.begin(); it!=lines.end(); ++it)
       {
        if (!it->empty())
           {
            paraLines.push_back(*it);
           }
        else // ѕуста€ строка - конец параграфа
           { // добавл€ем пустую строчку к предыдущему тексту - отдел€ем параграфы
            if (!res.empty()) res += "\n";
            if (!paraLines.empty())
                {
                 res += formatPara(options, paraLines);
                 paraLines.clear();
                }
           }
       }

    if (!paraLines.empty())
       {
        if (!res.empty()) res += "\n";
        res += formatPara(opts, paraLines);
       }

    return res;
   }





#endif /* TEXT_FMT_H */

