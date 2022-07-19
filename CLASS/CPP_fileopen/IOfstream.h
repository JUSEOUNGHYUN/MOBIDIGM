#include <iostream>
#include <stdlib.h> // strtol header
#include <string>
#include <cstdio>
#include <cstring>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>

using std::cout;

class IOfstream
{
private:	
	std::string m_strPath;
	std::ifstream m_ifstream;
public:
	IOfstream();
	IOfstream(std::string strPath);
	~IOfstream();
	std::string* GetNextData();
	bool isCorrectCRC(std::string strPath);
};

class WriteNMEAData : public IOfstream {
private:
	std::string m_Write_str;
	std::ofstream m_fout;
public:
	WriteNMEAData();
	WriteNMEAData(std::string str);
	~WriteNMEAData();
	bool WriteData(std::string write_str);
};

