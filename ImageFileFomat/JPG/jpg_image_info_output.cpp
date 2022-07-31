#include <iostream>
#include <cstdio>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <string>

/*
1. JFIF 버전
2. JFIF density unit
3. JFIF x density
4. JFIF y density
5. JFIF thumbnail 가로 크기
6. JFIF thumbnail 세로 크기
7. thumbnail 이미지 추출후 파일 저장
*/

typedef struct _JFIFHeader	// jpg 헤더 구조체 
{
	BYTE SOI[2];		// 00h  Start of Image Marker     
	BYTE APP0[2];       // 02h  Application Use Marker
	BYTE Length[2];     //  04h  Length of APP0 Field      
	BYTE Identifier[5]; //  06h  "JFIF" (zero terminated) Id String 
	BYTE Version[2];    //  07h  JFIF Format Revision      
	BYTE Units;         //  09h  Units used for Resolution 
	BYTE Xdensity[2];   //  0Ah  Horizontal Resolution     
	BYTE Ydensity[2];   //  0Ch  Vertical Resolution       
	BYTE XThumbnail;    //  0Eh  Horizontal Pixel Count    
	BYTE YThumbnail;    //  0Fh  Vertical Pixel Count      
} JFIFFILEHEADER;

#define print(n) printf("%ld\n", n)

void main() {
	JFIFFILEHEADER jfifheader;
	FILE* fp = fopen("C:\\test\\minion.jpg", "rb");

	fread(&jfifheader, sizeof(jfifheader), 1, fp);

	std::string strIden;
	printf("JFIF IDENTIFIER = ");
	for (int i = 0; i < sizeof(jfifheader.Identifier); i++) {
		strIden.push_back(jfifheader.Identifier[i]);
	}
	std::cout << strIden << "\n";
	//////////////////
	// JFIF VERSION //
	//////////////////

	printf("JFIF VERSION = ");
	for (int i = 0; i < sizeof(jfifheader.Version); i++) {
		printf("%02X ", jfifheader.Version[i]);
	}
	std::cout << "\n";

	///////////////////////
	// JFIF DENSITY UNIT //
	///////////////////////

	printf("JFIF DENSITY UNIT = %02X \n", jfifheader.Units);

	////////////////////
	// JFIF X DENSITY //
	////////////////////

	printf("JFIF X DENSITY = ");
	for (int i = 0; i < sizeof(jfifheader.Xdensity); i++) {
		printf("%02X ", jfifheader.Xdensity[i]);
	}
	std::cout << "\n";

	////////////////////
	// JFIF Y DENSITY //
	////////////////////

	printf("JFIF Y DENSITY = ");
	for (int i = 0; i < sizeof(jfifheader.Ydensity); i++) {
		printf("%02X ", jfifheader.Ydensity[i]);
	}
	std::cout << "\n";

	///////////////////////////////
	// JFIF THUMBNAIL HORIZONTAL //
	///////////////////////////////
	printf("JFIF THUMBNAIL HORIZONTA = %d\n", jfifheader.XThumbnail);

	/////////////////////////////
	// JFIF THUMBNAIL VERTICAL //
	/////////////////////////////
	printf("JFIF THUMBNAIL VERTICAL = %d\n", jfifheader.YThumbnail);

	///////////////////////////////
	// JFIF THUMBNAIL IMAGE DATA //
	///////////////////////////////
	fseek(fp, 0, SEEK_END);		// pointer file end position
	int len = ftell(fp);		// File length
	fseek(fp, -2, SEEK_CUR);	// Pointer second-to-last position in file
	BYTE Thumbnail[2]{};

	fread(Thumbnail, 2, 1, fp);

	printf("JFIF THUMBNAIL IMAGE DATA = ");
	std::string strThumb;
	for (int j = 0; j < sizeof(Thumbnail); j++) {
		printf("%02X ", Thumbnail[j]);
	}

	fclose(fp);
}

/*
1. JFIF 버전
2. JFIF density unit
3. JFIF x density
4. JFIF y density
5. JFIF thumbnail 가로 크기
6. JFIF thumbnail 세로 크기
7. thumbnail 이미지 추출후 파일 저장
*/
