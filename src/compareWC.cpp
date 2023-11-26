#include "compareWC.h"

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
       { // ������ ��� �����������, � ����� ����� ��� ���
        if (strWC[posWC]=='*') return true; // ��������� ������ � ����� ����� - '*' - �������� � ������� ���-�� ��������
       }
    // '*' - �� ��� ������ � �����
    // ������ ������ ���� ������ ������
    if (size!=sizeWC) return false; // ������ ������ ������ - ����� �� �������� �����
    return true; // ����� ����� ������ - ����� �������
}
//--------------------------------------------------------------
*/

//-----------------------------------------------------------------------------
std::string getStringNoWC(const std::string &strWC, std::string::size_type pos)
{
    std::string res;
    std::string::size_type sizeWC = strWC.size();
    for(; pos<sizeWC && strWC[pos]!='*' && strWC[pos]!='?'; ++pos)
       {
        res.append(1, strWC[pos]);
       }
    return res;
}

//-----------------------------------------------------------------------------
bool compareWildcardStrings(const std::string &str, const std::string &strWC)
{
    std::string::size_type sizeWC = strWC.size(), size = str.size(), posWC = 0, pos = 0; //, nextPosMult = std::string::npos;

    for (; pos<size && posWC<sizeWC; )
        {
         if (strWC[posWC]=='*') 
            {
             while(posWC<sizeWC && strWC[posWC]=='*') ++posWC;
             //nextPosMult = posWC;
             if (posWC>=sizeWC) 
                {
                 #ifdef CMPWC_TEST
                 std::cout<<"'*' found at end of mask\n";
                 #endif
                 return true; // '*' found at end of mask
                }
             std::string nextPart = getStringNoWC(strWC, posWC);
             //
             #ifdef CMPWC_TEST
             std::cout<<"nextPart: "<<nextPart<<"\n";
             #endif
             if (nextPart.empty())
                { // �� ������ �������� ��� �������� �����������
                 #ifdef CMPWC_TEST
                 std::cout<<"nextPart is empty\n";
                 #endif
                 for (; pos<size; ++pos)
                     {
                      #ifdef CMPWC_TEST
                      std::cout<<"try compare '"<<std::string(str, pos)<<"' with '"<<std::string(strWC, posWC)<<"'\n";
                      #endif
                      if (compareWildcardStrings(std::string(str, pos), std::string(strWC, posWC)))
                         {
                          #ifdef CMPWC_TEST
                          std::cout<<"strings are equal\n";
                          #endif
                          return true;
                         }
                     }
                 #ifdef CMPWC_TEST
                 std::cout<<"strings are not equal\n";
                 #endif
                 return false;
                }
             // ���� �������� ��� �������� �����������
             #ifdef CMPWC_TEST
             std::cout<<"search '"<<nextPart<<"' in string\n";
             #endif
             pos = str.find(nextPart, pos);
             if (pos==std::string::npos) 
                {
                 #ifdef CMPWC_TEST
                 std::cout<<"not found, return false\n";
                 #endif
                 return false; // �� ������, ����� �� ��������
                }
             #ifdef CMPWC_TEST
             std::cout<<"found, continue\n";
             #endif
             // ������� ��������� �� ������ ���������
             pos   += nextPart.size();
             posWC += nextPart.size();
             #ifdef CMPWC_TEST
             std::cout<<"str: '"<<std::string(str, pos)<<"', mask: '"<<std::string(strWC, posWC)<<"'\n";
             #endif
             --posWC; --pos;
             #ifdef CMPWC_TEST
             std::cout<<"str: '"<<std::string(str, pos)<<"', mask: '"<<std::string(strWC, posWC)<<"'\n";
             #endif
            }
         else
            {
             if (strWC[posWC]=='?') 
                {
                 ++pos, ++posWC;
                 continue; // skip one char
                }
             if (strWC[posWC]!=str[pos]) 
                {
                 return false;
                }
             ++pos, ++posWC;
            }
        }
    /*/
    if (pos>=size && posWC<sizeWC)
       { // ������ ��� �����������, � ����� ����� ��� ���
        if (strWC[posWC]=='*') return true; // ��������� ������ � ����� ����� - '*' - �������� � ������� ���-�� ��������
       }
    // '*' - �� ��� ������ � �����
    // ������ ������ ���� ������ ������
    if (size!=sizeWC) return false; // ������ ������ ������ - ����� �� �������� �����
    */
    return true; // ����� ����� ������ - ����� �������
}
//--------------------------------------------------------------


