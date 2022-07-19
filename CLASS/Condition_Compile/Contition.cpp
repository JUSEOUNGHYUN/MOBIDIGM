#include "Condition.h"


// 생성자  = 멤버변수 초기화
M_File::M_File() {
    m_strPath.clear();
    strReadData = NULL;
#if defined(USE_CPP)
    m_ifstream.clear();
#else
    m_pFile = NULL;
#endif
}

// 2. m_pFile 을 fclose = 소멸자
M_File::~M_File() {
    cout << "file close" << "\n";

#if defined(USE_CPP)
    if(!m_ifstream.eof()){
        m_ifstream.close();
        m_ifstream.clear();
    }
#else
    if (m_pFile != NULL) {
        fclose(m_pFile);
        m_pFile = NULL;
    }
#endif
    m_strPath.clear();
}

// 1. strpath 파일을 열어서 m_pfile 에 fopen 한다
M_File::M_File(string strNMEA) {
#if defined(USE_CPP)
    m_ifstream.open(strNMEA.c_str(), std::ios::in);
    if (!m_ifstream.is_open()) {
        cout << "File not found";
    }
#else
    m_pFile = fopen(strNMEA.c_str(), "r");
    if (m_pFile == NULL)
        printf("File not found");
#endif
        
    else
        m_strPath = strNMEA;
}

// 3. 데이터 읽는 클래스
string* M_File::GetNextData() {

#if defined(USE_CPP)
    std::string* strReadData;
    char read_ch[100] = { 0 };

    if (m_ifstream.is_open()) {
        m_ifstream.getline(read_ch, 100);

        int len = strlen(read_ch);

        if (len != 0) {
            strReadData = new std::string(read_ch);

            strReadData->push_back(0x0A);
            
            if (!isCorrectCRC(*strReadData)) {
                delete strReadData;
                strReadData = NULL;
            }
            return strReadData;
        }
    }
#else
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
            return strReadData;
        }
    }
#endif
    return nullptr;
}

//Exclusive한 Message와 Gps Data가 같은지 
bool M_File::isCorrectCRC(string strNMEA) {
    int pos = strNMEA.rfind('*');
    string Second_data = strNMEA.substr(pos + 1);
    int slen = strNMEA.length();
    int check_made = 0;
    int nMea_checksum = 0;
    nMea_checksum = strtol(Second_data.c_str(), NULL, 16);
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
#if defined(USE_CPP)  
    m_fout.clear();
#else
    m_wFile = NULL;
#endif

}

// 파일 쓰기 
WriteNMEAData::WriteNMEAData(string str) {
#if defined (USE_CPP)
    m_fout.open(str.c_str());
    if (!m_fout) {
        printf("No such file exists");
    }
#else
    m_wFile = fopen(str.c_str(), "w");
    if (m_wFile != NULL) {
        printf("File not found");
    }
#endif
    else
        m_Write_str = str;

}


// 파일 쓰기 소멸자
WriteNMEAData::~WriteNMEAData() {
#if defined(USE_CPP)
    cout << "End Wirte file" << "\n";
    if (m_fout.is_open()) {
        m_fout.close();
    }
#else
    if (m_wFile != NULL) {
        fclose(m_wFile);
        m_wFile = NULL;
}
#endif
    m_Write_str.clear();

}

bool WriteNMEAData::WriteData(string str) {

    // str을 파일에 저장
    bool bRet = false;

    if (!str.empty()) {
#if defined(USE_CPP)
        if (m_fout.is_open()) {
            m_fout.write(str.c_str(), str.size());
            m_fout.flush();
            bRet = true;
        }
    }
#else
        //fputs(str.c_str(), m_wFile);
        fwrite(str.c_str(), str.size(), 1, m_wFile);
            bRet = true;
#endif
    }
    else {
        bRet = false;
    }
    // return : true : write ok, false, failed write
    return bRet;

}