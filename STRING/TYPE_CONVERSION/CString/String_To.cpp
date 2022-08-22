/*
--- CT2CA ---
CString -> string

1.
CString m_strPathName;
std::string strPathName = std::string(CT2CA(m_strPathName));

2. 
CString m_strPathName;
string str = string(pDlg->CT2CA(m_strPathName))
- pDlg = 지역변수의 m_strPathName를 사용하겠다는 스레드

--- CT2CT ---
string -> CString

string front_Data;
CA2CT(front_Data.c_str());

*/

