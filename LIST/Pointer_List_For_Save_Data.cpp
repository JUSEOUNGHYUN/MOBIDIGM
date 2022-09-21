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
	int FileSize = ftell(fp);      // Decimal
	fseek(fp, 0, SEEK_SET);

	BYTE data[5]{};
	int message_size = sizeof(data);

	data[0] = (byte)((FileSize >> 24) & 0xFF);
	data[1] = (byte)((FileSize >> 16) & 0xFF);
	data[2] = (byte)((FileSize >> 8) & 0xFF);
	data[3] = (byte)(FileSize & 0xFF);

	// ****************************** List Ponter ******************************
	std::list<BYTE*> pListData;
	
    pListData.push_back(data);

	BYTE* Packet;

    // pListData에 있는 모든 데이터가 Packet에 저장됨
    // all data that in pListData is save in Packet
    // All data from pListData is stored in the packet
	Packet = pListData.front(); 

	pListData.pop_front();
	
	for (int i = 0; i < 4; i++) {
		printf("%02X ", Packet[i]);
	}
    // **************************************************************************

    return 0;
}

// Pointer List 
/*
1. for문을 돌리지 않아도 Packet = pListData.front() 으로 리스트에 있는 모든 데이터가 Packet에 저장된다.
2. Count가 좀 애매하다. (The count is ambiguous.)
*/
