
// MFC_Study_WorkDlg.h: Header File
//
#pragma warning(disable:4996)

#include "CommThread.h"
#include "stdafx.h"
#include <string>
#include <iostream>	// C++ input / output
#include <cstdio>	// C input / output
#include <cstring>	// memcpy
#include <stdlib.h>
#include <Windows.h>
#include <string.h>	
#include <list>		// std::list

#define SEND_DATA_BYTE		1024	 

// CMFCStudyWorkDlg Communication Box
class CMFCStudyWorkDlg : public CDialogEx
{
// Creation.
public:
	CMFCStudyWorkDlg(CWnd* pParent = nullptr);	// Standard constructor.

	CString m_CstrPathName;	// MFC FIleOpen CFileDialog(대화상자) Return to Total path of the selected File 
	std::string m_strPathName;	

	BOOL m_bPortInit;	// indicates whether ComPort has been succeessfullty initialized.
	BOOL m_bAnsiStart;	// Indicates whether ANSI escape characters came.

	// ComPort variable
	UINT m_nPort;
	UINT n_nBaudRate;
	CommThread m_ComuPort;	// The previously created communication class, Objects that can be used like class

	void ProcessErrorMessage(CString msg);	// output current status to status bar
	
// Communication box data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_STUDY_WORK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support.
	// DDX(Dialog Data Exchange)
	// DDV(Dialog Data Validation) = Set Effective Value
	
// Implement
protected:
	HICON m_hIcon;

	// Generated message map function
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFileSelect();   // File Select Button Function
	afx_msg void OnBnClickedSerialOpen();   // Open Button Function
	CComboBox m_combo_Comport_Select;	// ComboBox member variable
};
