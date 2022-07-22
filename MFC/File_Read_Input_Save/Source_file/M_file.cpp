
#include "M_file.h"

// 생성자  = 멤버변수 초기화
M_File::M_File() {
    m_strPath.clear();
    m_pFile = NULL;
    nMea_checksum = 0;
    strReadData = NULL;
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

    m_pFile = fopen( strNMEA.c_str(), "r");

    if (m_pFile == NULL)
        printf("[error] fopen error");
    else
        m_strPath = strNMEA;
}

// 3. 데이터 읽는 클래스
string* M_File::GetNextData() {

    //std::string* strReadData = NULL;

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

//Exclusive한 Message와 Gps Data가 같은지 
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
            check_made ^= value;    // Exclusive �Ϸ�
        }
    }

    if (nMea_checksum == check_made) {
        return true;
    }
    else {
        return false;
    }
}

// 생성자  = 멤버변수 초기화
WriteNMEAData::WriteNMEAData() {
    m_Write_str.clear();
    m_fout.clear();
}

// 파일 쓰기 
WriteNMEAData::WriteNMEAData(string str) {
    
    m_fout.open(str.c_str());

    if (!m_fout) {
        printf("No such file exists");
    }
    else
        m_Write_str = str;
}

// 파일 쓰기 소멸자
WriteNMEAData::~WriteNMEAData() {
    cout << "End Wirte file" << "\n";
    if (m_fout.is_open())
        m_fout.close();
}


bool WriteNMEAData::WriteData(string write_str) {

    bool bRet = false;

    // str을 파일에 저장
    
    if (!str.empty()) {
        
        if (m_fout.is_open()) {
            m_fout.write(str.c_str(), str.size());
            m_fout.flush();
            bRet = true;
        }
    }
    else {
        bRet = false;
    }

    // return : true : write ok, false, failed write
    
    return bRet;

}