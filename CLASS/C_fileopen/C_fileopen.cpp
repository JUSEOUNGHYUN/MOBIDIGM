// 

#include "C_fileopen.h"

C_fileopen::C_fileopen() {
	m_strPath.clear();
	strReadData = NULL;
	m_rFile = NULL;
}

C_fileopen::~C_fileopen() {
	if (m_rFile != NULL) {
		fclose(m_rFile);
		m_rFile = NULL;
	}
	m_strPath.clear();
}

C_fileopen::C_fileopen(std::string strNMEA) {
	m_rFile = fopen(strNMEA.c_str(), "r");
	if (m_rFile == NULL)
		printf("File not found");
	else
		m_strPath = strNMEA;
}

std::string* C_fileopen::GetNextData() {
	char Char_data[1000] = { 0 };
	if (m_rFile != NULL) {
		fgets(Char_data, 1000, m_rFile);

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
	return nullptr;
}

bool C_fileopen::isCorrectCRC(std::string strNMEA) {
	int pos = strNMEA.rfind('*');
	std::string Second_data = strNMEA.substr(pos + 1);
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
			check_made ^= value;   
		}
	}
	if (nMea_checksum == check_made) {
		return true;
	}
	else {
		return false;
	}
}

WriteNMEAData::WriteNMEAData() {
	m_write_strPath.clear();
	m_wFile = NULL;
}

WriteNMEAData::~WriteNMEAData() {
	if (m_wFile != NULL) {
		fclose(m_wFile);
		m_wFile = NULL;
	}
	m_write_strPath.clear();
}

WriteNMEAData::WriteNMEAData(std::string str) {
	m_wFile = fopen(str.c_str(), "w");
	if (m_wFile != NULL) {
		printf("File not found");
	}
	else {
		m_write_strPath = str;
	}
}

bool WriteNMEAData::WriteData(std::string str) {

	char cRead_ch[100] = { 0 };
	// str을 파일에 저장
	bool bRet = false;

	if (!str.empty()) {
		fwrite(str.c_str(), str.size(), 1, m_wFile);
		//fputs(str.c_str(), m_wFile);
			bRet = true;
	}
	else {
		bRet = false;
	}
	// return : true : write ok, false, failed write
	return bRet;
}