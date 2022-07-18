
// File_openDlg.h: 헤더 파일
//

#pragma once

#include <thread>
#include <list>
#include <string>
#include <windef.h>
#include <vector>

// CFileopenDlg 대화 상자
class CFileopenDlg : public CDialogEx
{
// 생성입니다.
public:
	CFileopenDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	CWinThread* m_pReadThread;	// 스레드 포인터
	CWinThread* m_pWriteThread;
	CWinThread* m_pSaveThread;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILE_OPEN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnLbnSelchangeList8();
	CListBox m_ctrlNMEAList;
	static UINT ReadThread(LPVOID _mothod);	// 스레드를 리스트에 추가
	static UINT WriteThread(LPVOID _mothod); // 리스트를 CListBox에 출력
	static UINT SaveThread(LPVOID _mothod); // 파일을 다른 이름으로 저장

	// list에 데이터가 추가되는걸 감지해서 CListBox에 데이터 추가
	std::list <std::string> m_listNMEADataString;	
	CString m_strPathName;	// 파일 절대경로 받을 CString
	CString m_strWritePathName;

	// UTF-8 문자열 to UTF-16 wstring conversion
	//std::wstring utf8_to_utf16(const std::string& utf8);


};

