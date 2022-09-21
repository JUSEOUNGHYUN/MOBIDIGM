
// ChatClientDlg.h: 헤더 파일
//

#pragma once
#pragma warning(disable:4996)
#include "CsocCom.h"
#include <string>
#include <list>
#include <memory>

#define DATA_SEND_SIZE		1024

// CChatClientDlg 대화 상자
class CChatClientDlg : public CDialogEx
{
// 생성입니다.
public:
	CChatClientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	CsocCom m_socCom;
	
	// Receive ListData
	std::list<BYTE*> m_Packet_list;
	BYTE m_Packet[1030];
	int m_PacketLen;
	bool m_bFindAA;
	int m_arrcnt;

	int m_cnt;

	// Receive File Data
	std::string m_strFileName;
	int m_FilLength;
	std::string m_WriteStrPathName;
	
	// Fwrite
	BYTE m_WriteByte[DATA_SEND_SIZE]{};	// File Write buffer

	// Client IP
	CString m_str_IP;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATCLIENT_DIALOG };
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
	afx_msg LRESULT OnReceive(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT SerialCommunicationStratOfHeader(WPARAM wParam, LPARAM lParam); // FIELD HEADER AA
	afx_msg LRESULT SendCommunicationEnable(WPARAM wParam, LPARAM lParam);			// FIELD HEADER A1
	afx_msg LRESULT RespondPossibleToCommunication(WPARAM wParam, LPARAM lParam);	// FIELD HEADER A2
	afx_msg LRESULT RespondSameFIleName(WPARAM wParam, LPARAM lParam);				// FIELD HEADER B2
	afx_msg LRESULT RespondFileLength(WPARAM wParam, LPARAM lParam);				// FIELD HEADER B4
	afx_msg LRESULT RespondSendData(WPARAM wParam, LPARAM lParam);					// FIELD HEADER C2
	afx_msg LRESULT RespondSendClose(WPARAM wParam, LPARAM lParam);					// FIELD HEADER D2
	// 사용자 메시지를 사용할 때 발생할 메시지 핸들러 생성 ** ( 중요 ) **
	/*
	afx_msg LRESULT Send_Same_FileName(WPARAM wParam, LPARAM lParam);				// FIELD HEADER B1
	afx_msg LRESULT SendFileLength(WPARAM wParam, LPARAM lParam);					// FIELD HEADER B3
	afx_msg LRESULT SendData(WPARAM wParam, LPARAM lParam);							// FIELD HEDAER C1
	afx_msg LRESULT SendClose(WPARAM wParam, LPARAM lParam);						// FIELD HEDAER D1
	*/


	DECLARE_MESSAGE_MAP()

private:
	void Send_Communication_Enable();				// FIELD HEADER A1
	void Respond_Possible_to_Communication();		// FIELD HEADER A2
	void Respond_Same_FileName();					// FIELD HEADER B2
	void Respond_FileLength();						// FIELD HEADER B4
	void Respond_Send_Data();						// FIELD HEADER C2
	void Respond_Close();							// FIELD HEADER D2
	/*
	*/
public:
//	CString m_strIP;
	CListBox m_list;
	afx_msg void OnClickedButtonConnet();

	// List Box에 [Send Data]00 보낼 함수
	int SendComData(BYTE* pData, int nLen);

	CEdit m_strIP;
};
