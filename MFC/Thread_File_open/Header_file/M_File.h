#include <iostream>
#include <stdlib.h> // strtol header
#include <string>
#include <cstdio>
#include <cstring>
#include <string.h>
#include <stdlib.h>

using namespace std;

class M_File
{
private:
    string m_strPath;       // 데이터를 읽어올 문자열 
    FILE* m_pFile;          // 파일
    int nMea_checksum;      // Exclusive 한 숫자
public:
    M_File();  // 생성자
    M_File(string strPath); // open file handle 생성자 오버로딩
    ~M_File(); // close file handle
    string* GetNextData();
    bool isCorrectCRC(string strPath); // Message | CheckSum 맞는지 비교 
};