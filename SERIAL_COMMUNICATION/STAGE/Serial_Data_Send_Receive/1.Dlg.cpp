
// MFC_Study_WorkDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFC_Study_Work.h"
#include "MFC_Study_WorkDlg.h"
#include "afxdialogex.h"
#include "CommThread.h"
#include "protocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCStudyWorkDlg 대화 상자



CMFCStudyWorkDlg::CMFCStudyWorkDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_STUDY_WORK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_sendLen = -1;
}


void CMFCStudyWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combo_Comport_Select);
	DDX_Control(pDX, IDC_LIST1, m_listSerialRespondData);
}

BEGIN_MESSAGE_MAP(CMFCStudyWorkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_MESSAGE(WM_MYMESSAGE, &CMFCStudyWorkDlg::OnMyMessage)
	ON_BN_CLICKED(IDC_FILE_SELECT, &CMFCStudyWorkDlg::OnBnClickedFileSelect)
	ON_BN_CLICKED(IDC_SERIAL_OPEN, &CMFCStudyWorkDlg::OnBnClickedSerialOpen)
	ON_BN_CLICKED(IDC_SERIAL_RECEIVE_DATA, &CMFCStudyWorkDlg::OnBnClickedSerialReceiveData)
END_MESSAGE_MAP()


// CMFCStudyWorkDlg 메시지 처리기

BOOL CMFCStudyWorkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: Add the additional initialization tasks here.

	// Add the ComboBox Options 
	m_combo_Comport_Select.AddString(_T("COM1"));		// nSelect = 0
	m_combo_Comport_Select.AddString(_T("COM2"));		// nSelect = 1
	m_combo_Comport_Select.AddString(_T("COM3"));		// nSelect = 2
	m_combo_Comport_Select.AddString(_T("COM4"));		// nSelect = 3
	m_combo_Comport_Select.AddString(_T("COM5"));		// nSelect = 4
	m_combo_Comport_Select.AddString(_T("COM6"));		// nSelect = 5
	m_combo_Comport_Select.AddString(_T("COM7"));		// nSelect = 6
	m_combo_Comport_Select.AddString(_T("COM8"));		// nSelect = 7

	// Combo Box Initialization Settings. set to 'COM1' before selecting a ComboBox
	m_combo_Comport_Select.SetCurSel(0);

	return TRUE;  // // Returns TRUE if no focus is set on the control.
}

void CMFCStudyWorkDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// A code is required to draw an icon when adding a Minimize button.
// 문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCStudyWorkDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCStudyWorkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 시리얼 포트 오픈 버튼 구현
// 콤보박스 목록에서 하나를 누르면 GetCurSel()이 그걸 가져오고 
void CMFCStudyWorkDlg::OnBnClickedSerialOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString selectDropString;
	m_combo_Comport_Select.GetLBText(m_combo_Comport_Select.GetCurSel(), selectDropString);

	// CBR_115200 == BaudRate(DCB) == 115200bps
	// m_combo_Comport_Select.GetCurSel() == wPortID
	if (!m_ComuPort.OpenPort(selectDropString, CBR_115200, m_combo_Comport_Select.GetCurSel(), AfxGetApp()->m_pMainWnd->m_hWnd)) {
		ProcessErrorMessage(_T("Port Initialization Failed!"));
	}
	else {
		CString strTemp;
		strTemp.Format(_T("Port COM%d Initialization Succeed"), m_combo_Comport_Select.GetCurSel()+1);
		ProcessErrorMessage(strTemp);
	}
}

// FileOpen CFileDialog
void CMFCStudyWorkDlg::OnBnClickedFileSelect()
{
	// TODO: Add the Control Notification Handler in here
	TCHAR fileFilter[] = _T("nmea(*.nmea)|*.nmea|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_CREATEPROMPT, fileFilter);

	if (dlg.DoModal() == IDOK)
	{
		m_CstrPathName = dlg.GetPathName(); // MFC FIleOpen CFileDialog(대화상자) Return to Total path of the selected File 
		m_strPathName = std::string(CT2CA(m_CstrPathName));
	}
}

LRESULT CMFCStudyWorkDlg::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
	BYTE buffer;

	m_sendLen = this->m_ComuPort.m_QueueRead.GetSize();

	for (int i = 0; i < m_sendLen; i++) {
		this->m_ComuPort.m_QueueRead.GetByte(&buffer);
		m_sendMessage = buffer;

		CString strTemp;

		TRACE("[message size] = [%d]\n", m_sendMessage.size());

		for (int i = 0; i < sizeof(buffer); i++) {
			strTemp.Format(_T("%02X "), (BYTE)m_sendMessage.at(i));
			// (BYTE) message.at(i) == AA 00 05 A1 BB
			// message.at(i) == FFFFFFFAA 00 05 FFFFFFFA1 FFFFFFFBB
			m_CstrSendData += strTemp;
		}
	}

	return 0;
}


// Process Error Message
void CMFCStudyWorkDlg::ProcessErrorMessage(CString msg) {

	AfxMessageBox(msg);
}


// Recive Data
void CMFCStudyWorkDlg::OnBnClickedSerialReceiveData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	

	CString str = _T("[RecvData] ") + m_CstrSendData;

	m_listSerialRespondData.AddString(str);
	
}

int CMFCStudyWorkDlg::CommSendData(BYTE* pData, int nLen)
{
	// TODO: 여기에 구현 코드 추가.
	CString Cstr = _T("[SendData] ");
	for (int i = 0; i < nLen; i++) {
		CString CstrTemp;
		CstrTemp.Format(_T("%02X "), pData[i]);
		Cstr += CstrTemp;
	}

	m_listSerialRespondData.AddString(Cstr);
	m_ComuPort.WriteComm(pData, nLen);

	return 0;
}
