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

	data[0] = (byte)((FileSize >> 24) & 0xFF);
	data[1] = (byte)((FileSize >> 16) & 0xFF);
	data[2] = (byte)((FileSize >> 8) & 0xFF);
	data[3] = (byte)(FileSize & 0xFF);

    // ****************************** List ******************************
    // ==================================================================
	for (int i = 0; i < message_size - 1; i++) {
		listData.push_back(data[i]);
	}

	for (int i = 0; i < listData.size(); i++) {
		data[i] = listData.front();
		listData.pop_front();
	}

	for (int i = 0; i < message_size - 1; i++) {
		printf("%02X ", data[i]);
	}
    // ******************************************************************
    // ==================================================================

    return 0;

}

// List

/*
1. 일일히 data를 for문을 돌려서 listData에 넣어야 한다.
2. listData에 있는 data를 가져오고 싶을 때 front(), pop_front()를 반복해야 한다.
3. Pointer_List보다 복잡하다.
*/
