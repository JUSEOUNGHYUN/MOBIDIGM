
// - CString => const char*
CString FileName = _T("file.txt");
const char* Name;
Name = (CStringA)FileName;

// - const char* -> CString 
const char* Name = "test";
CString FileName;
FileName = (CString)Name;
