#include <cstdio>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstring>
#include <string.h>
#include <Windows.h>

typedef struct MP3APIC {
	BYTE	APIC[4];
	BYTE	APICSIZE[4];
	BYTE	APICFLAG[2];
}MP3APIC;

int main() {

	// 파일 read
	FILE* fp = fopen("C:\\test\\004 나연 (TWICE) - POP!.mp3", "rb");

	// APIC를 찾을 BYTE
	BYTE tag3apic[3000];

	// APIC Header struct
	MP3APIC apic;

	// APIC가 있는 위치 찾을 포인터
	int find_APIC = 0;

	fread(tag3apic, sizeof(tag3apic), 1, fp);

	// APIC 단어 찾기
	for (int i = 0; i < sizeof(tag3apic); i++) {
		if (tag3apic[i] == 'A' && tag3apic[i+1] == 'P' && tag3apic[i+2] == 'I' && tag3apic[i+3] == 'C') {
			// APIC라는 단어가 있으면 그 위치 반환
			find_APIC = i;
		}
	}

	// APIC 단어가 있는 위치로 이동
	fseek(fp, find_APIC, SEEK_SET);
	int APIClen = ftell(fp);

	// APIC SIZE를 알기 위한 fread
	fread(&apic, sizeof(apic), 1, fp);

	// 16진수로 되어있는 문자를 일단 integer로 바꾼다.
	int apicIndex = 0;

	// 16진수 문자들을 합칠 문자열
	std::string strApic;

	// APIC Header에 있는 SIZE로 APIC IMAGE 길이 알기
	for (int i = 0; i < sizeof(apic.APICSIZE); i++) {
		apicIndex = apic.APICSIZE[i];
		char hexadecimal[20] = { 0, };   
		int position = 0;
		while (1){
			int mod = apicIndex % 16; 
			if (mod < 10) {
				hexadecimal[position] = 48 + mod;
			}
			else    {
				hexadecimal[position] = 65 + (mod - 10);
			}
			apicIndex = apicIndex / 16;
			position++;    
			if (apicIndex == 0)    
				break;
		}
		for (int i = position - 1; i >= 0; i--){
			strApic += hexadecimal[i];
		}
	}
	
	int APIC_IMAGE_SIZE = 0;
	APIC_IMAGE_SIZE = strtol(strApic.c_str(), NULL, 16);
	
	// APIC_IMAGE_SIZE에 대한 동적 할당
	unsigned char* apic_image = NULL;

	if (apic_image != NULL) {
		delete apic_image;
	}

	// APIC_IMAGE_SIZE 동적 할당
	apic_image = new unsigned char[APIC_IMAGE_SIZE];

	fread(apic_image, APIC_IMAGE_SIZE, 1, fp);

	for (int i = 0; i < APIC_IMAGE_SIZE; i++) {
		printf("%02X ", apic_image[i]);
	}

	fclose(fp);
	return 0;
}