#include "pch.h"
#include "M_File.h"

// 생성자  = 멤버변수 초기화
M_File::M_File() {
    m_strPath.clear();
    m_pFile = NULL;
    nMea_checksum = 0;
}

// 2. m_pFile 을 fclose = 소멸자
M_File::~M_File() {
    cout << "file close" << "\n";

    if (m_pFile != NULL) {
        fclose(m_pFile);
        m_pFile = NULL;
    }
    m_strPath.clear();
}

// 1. strpath 파일을 열어서 m_pfile 에 fopen 한다
M_File::M_File(string strNMEA) {

    m_pFile = fopen(strNMEA.c_str(), "r");

    if (m_pFile == NULL)
        printf("[error] fopen error");
    else
        m_strPath = strNMEA;
}

// 3. 데이터 읽는 클래스
string* M_File::GetNextData() {

    std::string* strReadData = NULL;

    char Char_data[1000] = { 0 };
    if (m_pFile != NULL) {
        fgets(Char_data, 1000, m_pFile);

        int len = strlen(Char_data);
        if (len != 0) {
            strReadData = new std::string(Char_data);
            // checksum
            if (!isCorrectCRC(*strReadData)) {
                delete strReadData;
                strReadData = NULL;
            }
        }
    }
    return strReadData;

}

bool M_File::isCorrectCRC(string strNMEA) {

    int pos = strNMEA.rfind('*');
    string Second_data = strNMEA.substr(pos + 1);

    nMea_checksum = strtol(Second_data.c_str(), NULL, 16);

    int slen = strNMEA.length();

    int check_made = 0;

    for (int i = 0; i < slen; i++) {
        char value = strNMEA.at(i);
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

    if (nMea_checksum == check_made) {
        return true;
    }
    else {
        return false;
    }
}