#include <stdio.h>
#include <Windows.h>

int main() {

	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	FILE* fp;
	fp = fopen("C:\\test\\adidas.bmp", "r");

	if (fp == NULL) {
		printf("No BMP");
		exit(10);
	}

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	if (hf.bfType == 0x4D42) {
		printf("BMP File\n");
	}
	else {
		printf("No BMP\n");
	}

	printf("1. image horizontal size = %d\n", hInfo.biWidth);           // 이미지 가로 크기
	printf("2. image vertical size = %d\n", hInfo.biHeight);            // 이미지 세로 크기
	printf("3. Bits per pixel = %d\n", hInfo.biBitCount);               // 픽셀당 비트 수
	printf("4. Compression type = %d\n", hInfo.biCompression);          // 압축 유형
	printf("5. Bitmap data size = %d\n", hInfo.biSizeImage);            // 비트맵 데이터 크기
	printf("6. horizontal resolution = %d\n", hInfo.biXPelsPerMeter);   // 수평 해상도
	printf("7. vertical resolution = %d\n", hInfo.biYPelsPerMeter);     // 수직 해상도
	printf("8. number of color indexes = %d\n", hInfo.biClrImportant);  // 컬러 인덱스 개수
	fclose(fp);

	return 0;
}