// 동적 메모리 할당 == new || delete

// hInfo.biWidth* hInfo.biHeight * 3 = 이미지 데이터(RGB 데이터)
// m_pColor_image = 컬러 이미지 동적 할당
// m_pGrey_image = 흑백 이미지 동적 할당


// dlg.h

unsigned char* m_pColor_image;	// BMP image 컬러 출력
unsigned char* m_pGrey_image;	// BMP image 흑백 출력

// Dlg.cpp
if (m_pColor_image != NULL) {
			delete m_pColor_image;
		}
		m_pColor_image = new unsigned char[hInfo.biWidth* hInfo.biHeight*3]; // RGB 데이터만큼 데이터 메모리 할당 받는다.

		fread(m_pColor_image, hInfo.biWidth * hInfo.biHeight * 3, 1, fp);	

		if (m_pGrey_image != NULL) {
			delete m_pGrey_image;
		}

		m_pGrey_image = new unsigned char[hInfo.biWidth * hInfo.biHeight];

		// RGB에서 R값 추출
		unsigned char rgbR;

		// i는 3씩 증가 || j는 1씩 증가
		for (int i = 0, j=0; i < hInfo.biWidth * hInfo.biHeight * 3; i+=3, j++) {
			m_pGrey_image[j] = m_pColor_image[i];
		}