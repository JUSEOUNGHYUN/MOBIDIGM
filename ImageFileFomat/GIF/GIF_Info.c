#include <string>
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <iostream>


#define _CRT_SECURE_NO_WARNINGS

typedef struct tagGIFFILEHEADER {
	WORD    gfWidth;			// GIF Width 2Byte
	WORD	gfHeight;			// GIF Height 2Byte
	char    gfReserved1;			
	char    gfBKColor;			// GIF Background Color 1Byte
	char    gfPixelAspectRatio; // GIF Pixel Aspect Ratio 1Byte
} GIFFILEHEADER;

void main()
{
	GIFFILEHEADER gfheader;
	BYTE gifheader[6]{};

	FILE* fp = fopen("C:\\test\\giphy.gif.", "rb");

	std::string strVersion;

	fread(gifheader, 6, 1, fp);

	for (int i = 3; i < sizeof(gifheader); i++) {
		strVersion.push_back(gifheader[i]);
	}

	std::cout <<"GIF Version = "<<strVersion << "\n";

	fread(&gfheader, sizeof(gfheader), 1, fp);

	int bit_per_pixel = (gfheader.gfReserved1 & 0x70) >> 4;
	int Color_table_sort_flag = (gfheader.gfReserved1 & 0x08) >> 3;
	int Global_color_table_size = (gfheader.gfReserved1 & 0x07);
	int Global_color_table_flag = (gfheader.gfReserved1 & 0x80) >> 7;


	printf("이미지 가로크기 = %d\n", gfheader.gfWidth);
	printf("이미지 세로 크기 = %d\n", gfheader.gfHeight);
	printf("픽셀당 비트수 = %d\n", bit_per_pixel);
	printf("글로벌색상테이블 sort 값 = %d\n", Color_table_sort_flag);
	printf("pixel aspect ratio = %d : %d \n", gfheader.gfPixelAspectRatio, (gfheader.gfPixelAspectRatio+15)/40);
	printf("Global_color_table_size = %d\n", Global_color_table_size);
	printf("Global_color_table_flag = %d\n", Global_color_table_flag);



	fclose(fp);
}