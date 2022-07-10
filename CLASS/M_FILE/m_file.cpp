
#include <iostream>
#include "M_File.h"

// 생성자  = 멤버변수 초기화
M_File::M_File() {
    m_strPath.clear();
    m_pFile = nullptr;
    m_bIsCorrectCRC = false;
    nMea_checksum = 0;
}

// 2. m_pFile 을 fclose = 소멸자
M_File::~M_File() {
    cout << "file close" << "\n";

    if (m_pFile != nullptr) {
        fclose(m_pFile);
        m_pFile = nullptr;
    }

    m_strPath.clear();
}

// 1. strpath 파일을 열어서 m_pfile 에 fopen 한다
M_File::M_File(string strPath) {
    
    m_pFile = fopen(strPath.c_str(), "r");

    if (m_pFile == nullptr)
        printf("[error] fopen error");

}

// 3. 데이터 읽는 클래스
string M_File::GetNextData(string strPath) {
    m_strPath = strPath;

    char Char_data[1000] = { 0 };
    if (m_pFile != nullptr) {
        fgets(Char_data, 1000, m_pFile);
            m_strPath = Char_data;
            if (!isExistFile(m_strPath)) {
                m_strPath.clear();
            }
    }
    return m_strPath;

    // checksum

}

// 4. 데이터가 끝났는지 확인 클래스
/*
bool M_File::IsCorrectCRC(string nmeaData) {

}
*/

// Message | CheckSum 숫자 맞는지 비교
bool M_File::isExistFile(string strPath) {
    m_strPath = strPath;

    int pos = m_strPath.rfind('*');
    string Second_data = m_strPath.substr(pos + 1);

    nMea_checksum = strtol(Second_data.c_str(), NULL, 16);

    int slen = m_strPath.length();

    int check_made = 0;

    for (int i = 0; i < slen; i++) {
        char value = m_strPath.at(i);
        if (value == '$') {
            continue;
        }
        else if (value == '*') {
            break;
        }
        else {
            check_made ^= value;    // Exclusive 완료
        }
    }

    return nMea_checksum = check_made ? true : false;
}

int main() {

    M_File* str_file = new M_File("C:\\test\\outputSF.txt");

    int32_t cnt = 0;
    string* readData = nullptr;
    while ((readData = str_file->GetNextData()) != nullptr) {
        //printf readData

        printf("[%d] [%s]\n", cnt++, readData->c_str());
    }

    delete str_file;

    // 
}