
// MFC_Serial_RecevieDlg.h: 헤더 파일
//

#pragma warning(disable:4996)

#include "CommThread.h"
#include "stdafx.h"
#include <string>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <Windows.h>
#include <fstream>
#include <string.h>
#include <list>

#define SEND_DATA_LEN		1024

// CMFCSerialRecevieDlg 대화 상자
class CMFCSerialRecevieDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCSerialRecevieDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	// Sender
	CString m_strPathName;		// FileAbsolutePath
	std::string m_strPathNamePure;	// File Read str
//	FILE* m_fp;					// File Read buffer
	int m_len;					// FileLength _Sender
	int m_cnt;
	int m_Current_FilePointer;
	
	// Receiver
//	FILE* m_wfp;				// FIle Write buffer
	std::string m_WriteStrPathName;
	std::string m_strFileName;	// FileName substr 
	int m_fileLength;			// FIleLength _Receiver

	BOOL m_bPortInit; // Com Port가 성공적으로 초기화되었는지 표시
	BOOL m_bAnsiStart; // ANSI Escape Character가 왔는지 표시

	// Com Port 변수 
	UINT m_nPort;
	UINT m_nBaudRate;
	CommThread m_ComuPort; // 앞에서 만든 통신용 클래스, Class 처럼 쓸수 있는 객체

	void ProcessErrorMessage(CString msg); // 현재 상태를 상태바에 출력 


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_SERIAL_RECEVIE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	// 사용자 메시지를 사용할 때 발생할 메시지 핸들러 생성 ** ( 중요 ) **
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	// 사용자 메시지를 사용할 때 발생할 메시지 핸들러 생성 ** ( 중요 ) **
	afx_msg LRESULT SendCommunicationEnable(WPARAM wParam, LPARAM lParam);			// FIEDL HEADER A1
	afx_msg LRESULT RespondPossibleToCommunication(WPARAM wParam, LPARAM lParam);	// FIELD HEADER A2
	afx_msg LRESULT Send_Same_FileName(WPARAM wParam, LPARAM lParam);				// FIELD HEADER B1
	afx_msg LRESULT RespondSameFIleName(WPARAM wParam, LPARAM lParam);				// FIELD HEADER B2
	afx_msg LRESULT SendFileLength(WPARAM wParam, LPARAM lParam);					// FIELD HEADER B3
	afx_msg LRESULT RespondFileLength(WPARAM wParam, LPARAM lParam);				// FIELD HEADER B4
	afx_msg LRESULT SendData(WPARAM wParam, LPARAM lParam);							// FIELD HEDAER C1
	afx_msg LRESULT RespondSendData(WPARAM wParam ,LPARAM lParam);					// FIELD HEADER C2
	afx_msg LRESULT SendClose(WPARAM wParam, LPARAM lParam);						// FIELD HEDAER D1
	afx_msg LRESULT RespondSendClose(WPARAM wParam, LPARAM lParam);					// FIELD HEADER D2
	
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_Comport_Select;
	afx_msg void OnBnClickedSerialOpen();
	afx_msg void OnBnClickedSerialClose();
	afx_msg void OnBnClickedDataSend();
	afx_msg void OnBnClickedFileSelect();

	// 파일 절대 경로에서 파일 이름 구하기 (짜르기)
	CString GetFileName(CString m_strPathName);
	CListCtrl m_List;
private:
	void Respond_Possible_to_Communication();	// FIELD HEADER A2
	void Send_Same_FileName();					// FIELD HEADER B1
	void Respond_Same_FileName();				// FIELD HEADER B2
	void Send_FileLength();						// FIELD HEADER B3
	void Respond_FileLength();					// FIELD HEADER B4
	void Send_Data();							// FIELD HEDAER C1
	void Respond_Send_Data();					// FIELD HEADER C2
	void Send_Close();							// FIELD HEDAER D1
	void Respond_Close();						// FIELD HEADER D2
public:
	CListBox m_serialRspondData;				// Serial Port Select Combo Box
	int SendComData(BYTE* pData, int nLen);
	afx_msg void OnBnClickedSerialStart();		// Serial Communication Start Button
	void Send_Communication_Enable();			// FIEDL HEADER A1
	std::string strFIlePathSubstrPure(std::string strTemp);	// File Path Substr
private:
public:
	
};
