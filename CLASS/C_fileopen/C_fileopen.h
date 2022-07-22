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

class C_fileopen
{
private:
	std::string m_strPath;	// 
	FILE* m_rFile;
public:
	C_fileopen();
	C_fileopen(std::string strPath);
	~C_fileopen();
	std::string* strReadData;
	std::string* GetNextData();
	bool isCorrectCRC(std::string strPath);
};

class WriteNMEAData : public C_fileopen {
private:
	std::string m_write_strPath;
	FILE* m_wFile;
public:
	WriteNMEAData();
	WriteNMEAData(std::string str);	
	~WriteNMEAData();
	bool WriteData(std::string write_str);
};

