/*
--- CStringA --- 
- CRT를 지원하는 ANSI 문자 유형 문자열입니다. 
- MBCS(Mulci-Byte Character Set)
- 문자열 , 아스키코드(ASCII) 문자열

--- CStringW ---
- CRT를 지원하는 유니코드 문자 유형 문자열입니다.
- WCS(Wide Character String)
- 유니코드 문자열

CString  : CRT를 지원하는 ANSI 및 유니코드 문자 유형입니다.

--- MB TO UNICODE ---
char* pszText = "MB TO UNICODE" ;
CString strUnicode( pszText ) ;

--- UNICODE TO MB ---
wchar_t* pszText = _T("UNICODE TO MB") ;
CStringA strMB( pszText ) ;

--- CStringA TO CString ---
CStringA strMB ;
CString strUNICODE( strMB ) ;
*/