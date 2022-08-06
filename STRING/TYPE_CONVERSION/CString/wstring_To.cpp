
// 객체에서 원시 Buffer 구하기
std::string =>str.c_str()
CString =>str.operator LPCWSTR()


변환 예제 :
CString cstr = _T("CString문자열");
std::string str = "string문자열";
std::wstring wstr = L"wstring문자열";


CString cstr2;
std::string str2;
std::wstring wstr2;


//string => CString 으로 변환cstr2 = 
CA2CT(str.c_str());MessageBox(cstr2.GetBuffer(0), _T("string => CString"), MB_OK);

//wstring => CString 으로 변환cstr2 = 
wstr.c_str();MessageBox(cstr2.GetBuffer(0), _T("wstring => CString"), MB_OK);

//CString => string 으로 변환str = 
CT2CA(cstr.operator LPCWSTR());MessageBox(CA2CT(str.c_str()), _T("CString => string"), MB_OK);

//wstring => string 으로 변환str = 
CT2CA(wstr.c_str());MessageBox(CA2CT(str.c_str()), _T("wstring => string"), MB_OK);

//CString => wstringwstr = 
cstr.operator LPCWSTR();MessageBox(wstr.c_str(), _T("CString => wstring"), MB_OK);

//string => wstringwstr = 
CA2CT(str.c_str());MessageBox(wstr.c_str(), _T("string => wstring"), MB_OK);
