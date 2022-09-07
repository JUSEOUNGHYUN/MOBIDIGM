#pragma warning(disable:4996)

#include <iostream>
#include <cstdio>
#include <string>
#include <Windows.h>
#include <list>
#include <iterator>

#define SEND_DATA_LEN		256

int main() {

	std::string str = "C:\\test\\Butter.mp3";
	FILE* fp = fopen(str.c_str(), "rb");
	BYTE m_Packet[30]{};

	fseek(fp, 0, SEEK_END);
	int FileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// List Ponter
	std::list<BYTE*> pListData;

	// LIst
	std::list<BYTE> listData;

	BYTE data[5]{};
	int message_size = sizeof(data);

    // Hexadecimal 
	data[0] = (byte)((FileSize >> 24) & 0xFF);
	data[1] = (byte)((FileSize >> 16) & 0xFF);
	data[2] = (byte)((FileSize >> 8) & 0xFF);
	data[3] = (byte)(FileSize & 0xFF);

	// ****************************** Hexadecimal To Decimal Method 1 ******************************
	int a = (int)((data[1] << 24) & 0xFF000000);
	int b = (int)((data[2] << 16) & 0xFF0000);
	int c = (int)((data[3] << 8) & 0xFF00);
	int d = (int)(data[4] & 0xFF);
	int TotalLen = a + b + c + d;
	std::cout << TotalLen << "\n"; // Decimal

	//****************************** Hexadecimal To Decimal Method 2 ******************************
	int SendLength = (int)((data[1] << 24) & 0xFF000000) + (int)((data[2] << 16) & 0xFF0000) + (int)((data[3] << 8) & 0xFF00) + (int)(data[4] & 0xFF);
	std::cout << SendLength << "\n"; // Decimal

	return 0;
}
