// Dlg.cpp
typedef struct _JFIFHeader
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

		JFIFFILEHEADER jfifheader;
		FILE* fp = fopen((CStringA)m_strPathName, "rb");

		fread(&jfifheader, sizeof(jfifheader), 1, fp);
		CString strTemp;
		//std::string strVersion;
		CString CstrVersion;	


		// JFIF VERSION
		for (int i = 0; i < sizeof(jfifheader.Version); i++) {
			int value = (int)(jfifheader.Version[i]);
			strTemp.Format(L"%d", value);
			CstrVersion += strTemp;
		}

		SetDlgItemTextW(IDC_JFIF_VERSION, CstrVersion);


		// JFIF DENSITY UNIT
		strTemp.Format(L"%d", jfifheader.Units);
		SetDlgItemTextW(IDC_JFIF_DENSITY_UNIT, strTemp);


		// JFIF X DENSITY
		for (int i = 0; i < sizeof(jfifheader.Xdensity); i++) {
			strTemp.Format(L"%d", jfifheader.Xdensity[i]);
		}
		SetDlgItemTextW(IDC_JFIF_X_DENSITY, strTemp);


		// JFIF Y DENSITY
		for (int i = 0; i < sizeof(jfifheader.Ydensity); i++) {
			strTemp.Format(L"%d", jfifheader.Ydensity[i]);
		}
		SetDlgItemTextW(IDC_JFIF_Y_DENSITY, strTemp);


		// JFIF THUMBNAIL HORIZONTAL
		strTemp.Format(L"%d", jfifheader.XThumbnail);
		SetDlgItemTextW(IDC_JFIF_THUMBNAIL_HORIZONTAL_SIZE, strTemp);


		// JFIF THUMBNAIL VERTICAL
		strTemp.Format(L"%d", jfifheader.YThumbnail);
		SetDlgItemTextW(IDC_THUMBNAIL_VERTICAL_SIZE, strTemp);


		// JFIF THUMBNAIL IMAGE DATA
		fseek(fp, 0, SEEK_END);	// pointer file end position
		int len = ftell(fp);					// File length
		fseek(fp, -2, SEEK_CUR);	// Pointer second-to-last position in file
		BYTE Thumbnail[2]{};

		fread(Thumbnail, 2, 1, fp);

		CString CstrThumb;
		for (int j = 0; j < sizeof(Thumbnail); j++) {
			int value = (int)(Thumbnail[j]);
			strTemp.Format(L"%02X ", value);
			CstrThumb += strTemp;
		}
			SetDlgItemTextW(IDC_THUMBNAIL_IMAGE_DATA, CstrThumb);

		fclose(fp);
