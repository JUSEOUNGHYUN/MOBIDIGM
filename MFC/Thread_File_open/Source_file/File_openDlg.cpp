
// File_openDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "File_open.h"
#include "File_openDlg.h"
#include "afxdialogex.h"
#include "M_File.h"
#include "locale.h"

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


// CFileopenDlg 대화 상자



CFileopenDlg::CFileopenDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILE_OPEN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CFileopenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST8, m_ctrlNMEAList);
}

BEGIN_MESSAGE_MAP(CFileopenDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CFileopenDlg::OnBnClickedButton1)
	ON_LBN_SELCHANGE(IDC_LIST8, &CFileopenDlg::OnLbnSelchangeList8)
END_MESSAGE_MAP()


// CFileopenDlg 메시지 처리기

BOOL CFileopenDlg::OnInitDialog()
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

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_pReadThread = NULL;	// Read thread 초기화
	m_pWriteThread = NULL;	// Wirte thread 초기화

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CFileopenDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CFileopenDlg::OnPaint()
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
HCURSOR CFileopenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFileopenDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	TCHAR fileFilter[] = _T("NMEA(*.nmea) | *.nmea | ");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, fileFilter);
	if (dlg.DoModal() == IDOK)
	{

		m_strPathName = dlg.GetPathName();	// 파일 절대 경로 

		m_pReadThread = AfxBeginThread(ReadThread, this); //CFileDialog의 변수를 쓰겠다.
		m_pWriteThread = AfxBeginThread(WriteThread, this); //CFileDialog의 변수를 쓰겠다.
		if (m_pReadThread == NULL) {
			
			//AfxMessageBox(L"Error!");
		}
		else {
			//AfxMessageBox(L"Good!");
		}


		if (m_pWriteThread == NULL) {
			//AfxMessageBox(L"Errorss!");
		}
		else {
			//AfxMessageBox(L"OK!");
		}
	}
}

// 스레드를 리스트에 추가
UINT CFileopenDlg::ReadThread(LPVOID _mothod) { // Buton 함수의 this가 _mothod를 가리킴
	CFileopenDlg* pDlg = (CFileopenDlg*)_mothod; // _mothod가 pDlg를 가리킴
									// pDlg가 클래스의 public 변수를 사용할 수 있음.

	string str = string(CT2CA(pDlg->m_strPathName));		// CString -> string
	M_File* str_file = new M_File(str);
	string* readData = NULL;
	while ((readData = str_file->GetNextData()) != NULL) {
		//printf readData
		if (readData != NULL) {
			pDlg->m_listNMEADataString.push_back(*readData);

			delete readData;
		}
	}
	if (str_file != NULL) {
		delete str_file;
	}

	return 0;
}

// 리스트를 CListBox에 출력
UINT CFileopenDlg::WriteThread(LPVOID _mothod) {
	CFileopenDlg* pDlg = (CFileopenDlg*)_mothod; 

	while (pDlg->m_listNMEADataString.size() != 0 ) {
		// ReadThread에서 가져온 List의 크기가 영어 아니면
		string front_Data = pDlg->m_listNMEADataString.front();
		pDlg->m_ctrlNMEAList.AddString(CA2CT(front_Data.c_str()));
		pDlg->m_listNMEADataString.pop_front();

		Sleep(100);
	}

	return 0;
}

void CFileopenDlg::OnLbnSelchangeList8() {

}


