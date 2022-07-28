// StretchBlt = Picture Control의 크기에 맞게 이미지 출력
// CImage / image.Load() = Image 파일 경로를 통해서 불러오기

// dlg.h
CStatic m_bmp_imgae_draw;	// Picture Control 변수

// Dlg.cpp
// 1번째 방법
TCHAR fileFilter[] = _T("bmp(*.bmp)|*.bmp|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, fileFilter);
	if (dlg.DoModal() == IDOK)
	{
		m_strPathName = dlg.GetPathName();
		CRect rect;
		m_bmp_image.GetWindowRect(rect);
		CDC* dc;
		dc = m_bmp_image.GetDC();	// m_bmp_image == Picture Control 변수 추가
		CImage image;
		image.Load(m_strPathName);

		image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
		ReleaseDC(dc);
	}

// 2번째 방법
// 컬러 이미지 출력

    BITMAPINFOHEADER hInfo;
	CRect rect;
	m_bmp_imgae_draw.GetWindowRect(rect);	// Picture Control의 변수를 CRect의 rect
	CDC* dc;
	dc = m_bmp_imgae_draw.GetDC();				
	COLORREF rgb;
	int gg = 0;
	for (int x = 0; x < hInfo.biHeight; x++) {
		for (int y = 0; y < hInfo.biWidth; y++) {
			dc->SetPixel(y, x, RGB(m_pColor_image[gg + 0], m_pColor_image[gg + 1], m_pColor_image[gg + 2]));
			gg += 3;
		}
	}
	ReleaseDC(dc);

//  void SetPixel(int x, int y, COLORREF color) throw(); == SetPixel() 함수 원형
/*
x = 설정할 픽셀의 가로 위치입니다.
y = 설정할 픽셀의 세로 위치입니다.
color = 픽셀을 설정할 색입니다.
*/