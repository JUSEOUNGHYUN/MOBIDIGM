#include "IOfstream.h"

IOfstream::IOfstream() {
	m_strPath.clear();
	m_ifstream.clear();
}

IOfstream::~IOfstream() {
	cout << "File close"<<"\n";

	if (!m_ifstream.eof()) {
		m_ifstream.close();
		m_ifstream.clear();
	}
	m_strPath.clear();
}

IOfstream::IOfstream(std::string strNMEA) {
	
	m_strPath.clear();
	m_ifstream.clear();

	m_ifstream.open(strNMEA.c_str(), std::ios::in);

	if (!m_ifstream.is_open()) {
		cout << "File not found";
	}
	else {
		m_strPath = strNMEA;
	}
}

std::string* IOfstream::GetNextData() {

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
	return nullptr;
}

bool IOfstream::isCorrectCRC(std::string strNMEA) {

	int pos = strNMEA.rfind('*');
	std::string Second_data = strNMEA.substr(pos + 1);

	int nMea_checksum = strtol(Second_data.c_str(), NULL, 16);

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

WriteNMEAData::WriteNMEAData() {
	m_Write_str.clear();
	m_fout.clear();
}

WriteNMEAData::WriteNMEAData(std::string str) {

	m_fout.open(str.c_str(), std::ios::out);

	if (!m_fout.is_open()) {
		printf("No such file exists");
	}
	else
		m_Write_str = str;
}

WriteNMEAData::~WriteNMEAData() {
	cout << "End Wirte file" << "\n";
	if (m_fout.is_open())
		m_fout.close();
	
}

bool WriteNMEAData::WriteData(std::string str) {

	bool bRet = false;
	// str을 파일에 저장
	/*
	m_ifstream.seekg(0, m_ifstream.end);
	int size = m_ifstream.tellg();

	char* buffer = new char[size];
	*/ 

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