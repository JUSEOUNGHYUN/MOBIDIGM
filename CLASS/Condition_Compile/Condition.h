// C언어 fopen Read = fread, Write = fwrite
// c++ fstream Read = ifstream, Write = ofstream

#include <iostream>
#include <stdlib.h> // strtol header
#include <string>
#include <cstdio>
#include <cstring>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>

//#define USE_CPP   fstram을 쓰고 싶으면 매크로 활성화 | fopen을 쓰고 싶으면 매크로 비활성화 지금은 비활성화 상태
using namespace std;

class M_File
{
private:
    string m_strPath;       // fopen_read
#if defined (USE_CPP)
    ifstream m_ifstream;    // ifstream_read
#else
    FILE* m_pFile;          // 파일
#endif
   
public:
    M_File();  // 생성자
    M_File(string strPath); // open file handle 생성자 오버로딩
    ~M_File(); // close file handle
    string* strReadData;
    string* GetNextData();
    bool isCorrectCRC(string strPath); // Message | CheckSum 맞는지 비교
};

class WriteNMEAData : public  M_File {
private:
    string m_Write_str;            // ofstream_write     
#if defined(USE_CPP)
    ofstream m_fout;    // 파일을 쓸 파일명
#else
    FILE* m_wFile;          // 파일
#endif

public:
    WriteNMEAData();    // 파일 초기화
    WriteNMEAData(string str);  // 
    ~WriteNMEAData();
    bool WriteData(string write_str);

};