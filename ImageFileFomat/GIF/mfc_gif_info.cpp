m_strPathName = dlg.GetPathName();
		FILE* fp;

		typedef struct tagGIFFILEHEADER {
			WORD    gfWidth;			// GIF Width 2Byte
			WORD	gfHeight;			// GIF Height 2Byte
			char    gfReserved1;
			char    gfBKColor;			// GIF Background Color 1Byte
			char    gfPixelAspectRatio; // GIF Pixel Aspect Ratio 1Byte
		} GIFFILEHEADER;

		GIFFILEHEADER gfheader;
		BYTE gifheader[6]{};
		std::string strVersion;

		fp = fopen((CStringA)m_strPathName, "r");

		fread(gifheader, 6, 1, fp);

		for (int i = 3; i < sizeof(gifheader); i++) {
			strVersion.push_back(gifheader[i]);
		}

		fread(&gfheader, sizeof(gfheader), 1, fp);

		int bit_per_pixel = (gfheader.gfReserved1 & 0x70) >> 4;
		int Color_table_sort_flag = (gfheader.gfReserved1 & 0x08) >> 3;

		
		CString strTemp;
		strTemp = CA2CT(strVersion.c_str());
		SetDlgItemTextW(IDC_GIF_VERSION, strTemp);
		strTemp.Format(L"%d", gfheader.gfWidth);
		SetDlgItemTextW(IDC_IMAGE_HORIZONTAL_SIZE,strTemp);
		strTemp.Format(L"%d", gfheader.gfHeight);
		SetDlgItemTextW(IDC_IMAGE_VERTICAL_SIZE, strTemp);
		strTemp.Format(L"%d", bit_per_pixel);
		SetDlgItemTextW(IDC_BITS_PER_PIXEL, strTemp);
		strTemp.Format(L"%d", Color_table_sort_flag);
		SetDlgItemTextW(IDC_COLOR_TABLE_SORT_FLAG, strTemp);
		strTemp.Format(L"%d", gfheader.gfPixelAspectRatio);
		SetDlgItemTextW(IDC_PIXEL_ASPECT_RATIO, strTemp);

		fclose(fp);