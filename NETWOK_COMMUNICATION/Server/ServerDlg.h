
// ChatServerDlg.h: 헤더 파일
//

#pragma once
#pragma warning(disable:4996)
#include "stdAfx.h"
#include <string>
#include <string.h> // memcpy (Memory Copy)
#include <list>
#include <memory>

#define DATA_SEND_SIZE		1024

// CChatServerDlg 대화 상자
class CChatServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CChatServerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	CSocServer m_socServer;		// 서버용 소켓
	CSocCom* m_socCom;			// 통신용 소켓
	FILE* m_pFile;				// File Pointer
	CString m_CstrPathName;		// File Path
	std::string m_strPathName;	

	// Receive ListData
	std::list<BYTE*> m_Packet_list;
	BYTE m_Packet[1030];
	int m_PacketLen;
	bool m_bFindAA;
	int m_arrcnt;
	int m_Current_FilePointer;
	int m_cnt;

	// Receiver File Infomation
	std::string m_strPureName;	// 절대 경로 뺀 파일 이름
	int m_FileLen;				// 파일 사이즈(길이)

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATSERVER_DIALOG };
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
	afx_msg LRESULT OnAccept(WPARAM wParam, LPARAM lParam);	// 클라이언트 접속 요청이 왔을때 실행되는 메시지 함수
	afx_msg LRESULT OnReceive(WPARAM wParam, LPARAM lParam);	// 클라이언트에서 오는 데이터 수신할때 실행,,
	
	afx_msg LRESULT SerialCommunicationStratOfHeader(WPARAM wParam, LPARAM lParam); // FIELD HEADER AA
	afx_msg LRESULT SendCommunicationEnable(WPARAM wParam, LPARAM lParam);			// FIELD HEADER A1
	afx_msg LRESULT RespondPossibleToCommunication(WPARAM wParam, LPARAM lParam);	// FIELD HEADER A2
	afx_msg LRESULT Send_Same_FileName(WPARAM wParam, LPARAM lParam);				// FIELD HEADER B1
	afx_msg LRESULT SendFileLength(WPARAM wParam, LPARAM lParam);					// FIELD HEADER B3
	afx_msg LRESULT SendData(WPARAM wParam, LPARAM lParam);							// FIELD HEDAER C1
	afx_msg LRESULT SendClose(WPARAM wParam, LPARAM lParam);						// FIELD HEDAER D1


	DECLARE_MESSAGE_MAP()
private:
	void Send_Communication_Enable();			// FIELD HEADER A1
	void Respond_Possible_to_Communication();	// FIELD HEADER A2
	void Send_Same_FileName();					// FIELD HEADER B1
	void Send_FileLength();						// FIELD HEADER B3
	void Send_Data();							// FIELD HEADER C1
	void Send_Close();							// FIELD HEADER D1
	
public:
	afx_msg void OnClickedButtonSend();
	std::string strPathSubstr(std::string strTemp);
	CString m_strSend;
	CListBox m_list;
	CString m_strStatus;
	afx_msg void OnBnClickedFileOpen();

	// List Box에 [Send Data]00 보낼 함수
	int SendComData(BYTE* pData, int nLen);
	afx_msg void OnBnClickedServerOpen();
};
