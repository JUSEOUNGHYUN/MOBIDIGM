
// MFC_Serial_RecevieDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFC_Serial_Recevie.h"
#include "MFC_Serial_RecevieDlg.h"
#include "afxdialogex.h"
#include "CommThread.h"
#include "protocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_SERIAL_COMMUNICATION_START_OF_HEADER WM_USER + 8	 // FIELD HEADER AA
#define WM_SEND_COMMUNICATION_ENABLE			WM_USER + 9	 // FIELD HEADER A1
#define WM_RESPOND_POSSIBLE_TO_COMMUNICATION	WM_USER + 10 // FIELD HEADER A2
#define WM_SEND_SAME_FILENAME					WM_USER + 11 // FIELD HEADER B1
#define WM_RESPOND_SAME_FILENAME				WM_USER + 12 // FIELD HEADER B2
#define WM_SEND_FILE_LENGTH						WM_USER + 13 // FIELD HEADER B3
#define WM_RESPOND_FILE_LENGTH					WM_USER + 14 // FIELD HEADER B4
#define WM_SEND_DATA							WM_USER + 15 // FIELD HEADER C1
#define WM_RESPOND_SEND_DATA					WM_USER + 16 // FIELD HEADER C2
#define WM_SEND_CLOSE							WM_USER + 17 // FIELD HEADER D1
#define WM_RESPOND_SEND_CLOSE					WM_USER + 18 // FIELD HEADER D2
/*
*/


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

// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
// ********************************************************************  MFC Class 생성자 ******************************************************************** //
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// // 
// CMFCSerialRecevieDlg 대화 상자
CMFCSerialRecevieDlg::CMFCSerialRecevieDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_SERIAL_RECEVIE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_cnt = 0;
	m_bFindAA = false;
	memset(m_Packet, 0, 1030);
	m_arrcnt = 0;
	m_PacketLen = 0;
}

void CMFCSerialRecevieDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// ComboBox 변수 생성
	DDX_Control(pDX, IDC_COMBO1, m_Comport_Select);
	DDX_Control(pDX, IDC_SERIAL_LIST, m_serialRspondData);
}

BEGIN_MESSAGE_MAP(CMFCSerialRecevieDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	// *** 사용자 정의 메시지 설정 ***
	ON_MESSAGE(WM_SERIAL_COMMUNICATION_START_OF_HEADER, &CMFCSerialRecevieDlg::SerialCommunicationStratOfHeader)// FIELD HEADER AA (Start Of Header) 
	ON_MESSAGE(WM_SEND_COMMUNICATION_ENABLE, &CMFCSerialRecevieDlg::SendCommunicationEnable)					// FIELD HEADER A1
	ON_MESSAGE(WM_RESPOND_POSSIBLE_TO_COMMUNICATION, &CMFCSerialRecevieDlg::RespondPossibleToCommunication)		// FIELD HEADER A2
	ON_MESSAGE(WM_SEND_SAME_FILENAME, &CMFCSerialRecevieDlg::Send_Same_FileName)								// FIELD HEADER B1
	ON_MESSAGE(WM_RESPOND_SAME_FILENAME, &CMFCSerialRecevieDlg::RespondSameFIleName)							// FIELD HEADER B2
	ON_MESSAGE(WM_SEND_FILE_LENGTH, &CMFCSerialRecevieDlg::SendFileLength)										// FIELD HEADER B3
	ON_MESSAGE(WM_RESPOND_FILE_LENGTH, &CMFCSerialRecevieDlg::RespondFileLength)								// FIELD HEADER B4
	ON_MESSAGE(WM_SEND_DATA, &CMFCSerialRecevieDlg::SendData)													// FIELD HEADER C1
	ON_MESSAGE(WM_RESPOND_SEND_DATA, &CMFCSerialRecevieDlg::RespondSendData)									// FIELD HEADER C2
	ON_MESSAGE(WM_SEND_CLOSE, &CMFCSerialRecevieDlg::SendClose)													// FIELD HEDAER D1
	ON_MESSAGE(WM_RESPOND_SEND_CLOSE, &CMFCSerialRecevieDlg::RespondSendClose)									// FIELD HEADER D2

	// 기존 사용자 정의 메시지
	ON_MESSAGE(WM_MYMESSAGE, &CMFCSerialRecevieDlg::OnMyMessage)
	ON_BN_CLICKED(IDC_SERIAL_OPEN, &CMFCSerialRecevieDlg::OnBnClickedSerialOpen)
	ON_BN_CLICKED(IDC_SERIAL_CLOSE, &CMFCSerialRecevieDlg::OnBnClickedSerialClose)
	ON_BN_CLICKED(IDC_DATA_SEND, &CMFCSerialRecevieDlg::OnBnClickedDataSend)
	ON_BN_CLICKED(IDC_FILE_SELECT, &CMFCSerialRecevieDlg::OnBnClickedFileSelect)
	ON_BN_CLICKED(IDC_SERIAL_START, &CMFCSerialRecevieDlg::OnBnClickedSerialStart)
END_MESSAGE_MAP()


// CMFCSerialRecevieDlg 메시지 처리기

BOOL CMFCSerialRecevieDlg::OnInitDialog()
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
	
	m_bFindAA = false;


	// 콤보 박스 선택지 추가
	m_Comport_Select.AddString(_T("COM1"));
	m_Comport_Select.AddString(_T("COM2"));
	m_Comport_Select.AddString(_T("COM3"));
	m_Comport_Select.AddString(_T("COM4"));
	m_Comport_Select.AddString(_T("COM5"));
	m_Comport_Select.AddString(_T("COM6"));
	m_Comport_Select.AddString(_T("COM7"));
	m_Comport_Select.AddString(_T("COM8"));
	m_Comport_Select.SetCurSel(0);

	// 콤보 박스 Init 설정 ** 콤보 박스 선택하기 전 COM'0'으로 설정
	m_Comport_Select.SetCurSel(0);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCSerialRecevieDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCSerialRecevieDlg::OnPaint()
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
HCURSOR CMFCSerialRecevieDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 시리얼 포트 오픈 버튼 구현
void CMFCSerialRecevieDlg::OnBnClickedSerialOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString selectDropListString;
	m_Comport_Select.GetLBText(m_Comport_Select.GetCurSel(), selectDropListString);

	if (!m_ComuPort.OpenPort(selectDropListString, CBR_115200, m_Comport_Select.GetCurSel(), AfxGetApp()->m_pMainWnd->m_hWnd)) {

		ProcessErrorMessage(_T("포트 초기화 실패"));
	}
	else {
		CString strTemp;
		strTemp.Format(_T("포트 COM%d Init 성공"), m_Comport_Select.GetCurSel() + 1);
		ProcessErrorMessage(strTemp);

		// 초기화 됐는지 표시하는 변수
		m_bPortInit = TRUE;
		m_bFindAA = false;
	}
}

void CMFCSerialRecevieDlg::OnBnClickedFileSelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*
	TCHAR fileFilter[] = _T("jpeg(*.jpeg)|*.jpeg|");
	TCHAR fileFilter[] = _T("mp3(*.mp3)|*.mp3|");
	*/
	TCHAR fileFilter[] = _T("nmea(*.nmea)|*.nmea|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_CREATEPROMPT, fileFilter);

	if (dlg.DoModal() == IDOK)
	{
		m_strPathName = dlg.GetPathName();

		// Cstr = outputSF.nmea (파일 이름 빼고 절대 경로 짜르기)
		CString Cstr = GetFileName(m_strPathName);
		
		m_strPathNamePure = std::string(CT2CA(m_strPathName));
	}
}

// ex) C:\\test\\Butter.mp3 -> Butter.mp3
std::string CMFCSerialRecevieDlg::strFIlePathSubstrPure(std::string strTemp) {

	int pos;

	pos = strTemp.rfind('\\');
	std::string second = strTemp.substr(pos + 1);
	return second;
}

// FIle Absolute Path -> FileName Function
CString CMFCSerialRecevieDlg::GetFileName(CString m_strPathName) {

	CString strFileName;

	strFileName = m_strPathName.Right(m_strPathName.GetLength() - m_strPathName.ReverseFind('\\') - 1);

	return strFileName;
}

// 1. MFC Serial Communication Sequence (OnMyMessage = Make Packet) 
LRESULT CMFCSerialRecevieDlg::OnMyMessage(WPARAM wParam, LPARAM lParam) {

	BYTE buffer;
	BYTE total;
	int SendLength = 0;
	
	std::string message;
	int len = -1;

	len = this->m_ComuPort.m_QueueRead.GetSize();

	for (int i = 0; i < len; i++) {
		this->m_ComuPort.m_QueueRead.GetByte(&buffer);

		if (m_bFindAA==false  && buffer == FIELD_HEADER_PROTOCOL_AA_SERIAL_COMMUNICATION_START_OF_HEADER) {
			m_bFindAA = true;
			m_arrcnt = 0;
		}

		m_Packet[m_arrcnt] = buffer;
		
		if (m_arrcnt == 2) {
			int a = (int)((m_Packet[1] << 8) & 0xFF00);
			int b = (int)(m_Packet[2] & 0xFF);
			m_PacketLen = a + b;
		}
		
		if (m_PacketLen == (m_arrcnt + 1) ) {
			if (m_Packet[m_PacketLen - 1] == FIELD_HEADER_PROTOCOL_BB_SERIAL_COMMUNICATION_END_OF_HEADER) {
				TRACE("[Make Packet]\n");
				
				// New Make Packet If Last buffer is 'BB'
				BYTE* Packet = new BYTE[1030];
				// BYTE Packet[1030];

				// Memory Copy from m_Packet to Packet	
				memcpy(Packet, m_Packet, m_PacketLen);

				m_Packet_list.push_back(Packet);

				PostMessage(WM_SERIAL_COMMUNICATION_START_OF_HEADER, NULL, NULL);
			}
			else {
				TRACE("[Doesn't Make Packet]\n");
			}
			m_bFindAA = false;
		}

		m_arrcnt++;
	}

// 조건부 추가

	return 0;
}

// ------------------------------------------------------------ User Define Message Programing ------------------------------------------------------------

// 2. MFC Serial Communication Sequence (OnMyMessage 에서 만든 Packet을 [RecvData] 00 00 00 00)

// Packet[3]의 필드 헤더에 해당하는 조건문으로 들어감 = Enter the Conditional Statement Corresponding To The Field Header Of Packet[3]
// FIELD HEADER AA SerialCommunicationStratOfHeader USER DEFINE MESSAGE
LRESULT CMFCSerialRecevieDlg::SerialCommunicationStratOfHeader(WPARAM wParam, LPARAM lParam) {

	TRACE("[List Count] = [%d]\n", m_Packet_list.size());
	
	BYTE* Packet;
	Packet = m_Packet_list.front();
	
	int packlen = (int)((Packet[1] >> 8) & 0xFF) + (int)(Packet[2] & 0xFF);

	m_Packet_list.pop_front();

	CString str;
	CString strtemp;

	str = _T("[RecvData] ");
	for (int i = 0; i < packlen; i++) {
		strtemp.Format(_T("%02X "), Packet[i]);
		str += strtemp;
	}

	//RecvData OOO
	m_serialRspondData.AddString(str);
	/////
	// ------------------------------------ Sender ------------------------------------

	if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_A2_SERIAL_COMMUNICATION_RECEIVE) {
		// send B1
		// 
	}

	else if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_B2_FILENAME_RECEIVE) {
		// Send_FileLength();
		::PostMessage(this->m_hWnd, WM_SEND_FILE_LENGTH, 0, 0);

	}
	else if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_B4_FILELENGTH_LENGTH) {
		// send C1
		::PostMessage(this->m_hWnd, WM_SEND_DATA, 0, 0);

	}
	else if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_C2_RESOPOND_SEND_DATA) {
		// send C1
		::PostMessage(this->m_hWnd, WM_SEND_DATA, 0, 0);

	}
	else if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_D2_RESPOND_RECEIVE_DATA) {

	}


	// ------------------------------------ Receiver ------------------------------------

	else if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_A1_SERIAL_COMMUNICATION_SEND) {
		// send A2
		// Respond_Possible_to_Communication();
		// 사용자 정의 메세지
		::PostMessage(this->m_hWnd, WM_RESPOND_POSSIBLE_TO_COMMUNICATION, 0, 0);
	}
	else if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_B1_FILENAME_SEND) {
		// send B2
		// Respond_Same_FileName();

		// -------------------- 42 75 74 74 65 72 2E 6D 70 33 -> Butter.mp3 --------------------	// 8/30
		// -------------------- AA 00 0A B1 42 75 74 74 65 72 2E 6D 70 33 BB --------------------	// 8/31

		// 4 = AA 00 0A B1 건너뛴다.
		// message.size() - 1 = Tail(BB) not import
		for (int i = 4; i < packlen - 1; i++) {
			m_strTemp += Packet[i];
		}

		// send C2
		/*
		std::string NewPathName = "D:\\디렉터리\\MFC_Serial_Recevie\\x64\\Debug\\";
		std::string second = strFIlePathSubstrPure(strTemp);
		m_WriteStrPathName = NewPathName + second;
		*/

		// MFC 실행 파일의 전체 경로 가져오기 (exe까지 전부 가져옴)
		TCHAR chFilePath[256]{};
		GetModuleFileName(NULL, chFilePath, 256);

		CString strFolderPath(chFilePath);
		strFolderPath = strFolderPath.Left(strFolderPath.ReverseFind('\\'));
		// CString strPure = GetFileName(m_strPathName);
		CString strPure = (CA2CT(m_strTemp.c_str()));
		CString total = strFolderPath + "\\" + strPure;

		m_WriteStrPathName = std::string(CT2CA(total));

		// todo memorize filename
		TRACE("[filename] %s\n", m_WriteStrPathName);

		// create file pointer
		//m_wfp = fopen(m_WriteStrPathName.c_str(), "wb");

		// if m_WriteStrPathName exist  then remove m_WriteStrPathName.
		DeleteFile(CA2CT(m_WriteStrPathName.c_str()));


		::PostMessage(this->m_hWnd, WM_RESPOND_SAME_FILENAME, 0, 0);
	}
	else if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_B3_FILELENGTH_SEND) {
		// send B4
		// todo memorize file length
		// -------------------- 00 29 ED B5 -> 2,747,829 --------------------
		int a = (int)((Packet[4] << 24) & 0xFF000000);
		int b = (int)((Packet[5] << 16) & 0xFF0000);
		int c = (int)((Packet[6] << 8) & 0xFF00);
		int d = (int)((Packet[7]) & 0xFF);
		m_fileLength = a + b + c + d;

		TRACE("[file length] %d\n", m_fileLength);
		::PostMessage(this->m_hWnd, WM_RESPOND_FILE_LENGTH, 0, 0);
	}
	// fopen -> move last positoin of file -> fwrite -> fclose
	else if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_C1_DATA_SEND) {

		// Create a FILE With Name m_WriteStrPathName (m_WriteStrPathName 이름으로 파일 생성)
		FILE* fp = fopen(m_WriteStrPathName.c_str(), "a+b");
		fseek(fp, 0, SEEK_END);
		 
		// TODO : write data
		// 1. string to byte
		// m_WriteByte[SEND_DATA_LEN];

		// AA, LEN_H, LEN_R, TAIL = 4
		int buffer_len = packlen - 4;

		// i = 4 = AA, LEN_H, LEN_R, CMD
		// packlen - 1 = FINISH TAIL SUBSTR
		for (int i = 4; i < packlen; i++) {
			// 0xBB면 break
			if (i == packlen - 1) {
				break;
			}
			else {
				(BYTE)m_WriteByte[i - 4] = Packet[i];
			}
		}

		// 2. fwrite 
		fwrite(m_WriteByte, buffer_len - 1, 1, fp);

		fclose(fp);

		::PostMessage(this->m_hWnd, WM_RESPOND_SEND_DATA, 0, 0);
	}

	else if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_D1_DATA_RECEIVE) {
		// send D2

		//todo fclose fp that is written data
		// fclose(m_wfp);
		::PostMessage(this->m_hWnd, WM_RESPOND_SEND_CLOSE, 0, 0);

	}

	/// <summary>
	/// /
	/// </summary>
	/// <param name="wParam"></param>
	/// <param name="lParam"></param>
	/// <returns></returns>
	delete Packet;


	return 0;

}


// FIELD HEADER A1 SendCommunicationEnable USER DEFINE MESSAGE				|| Sender
LRESULT CMFCSerialRecevieDlg::SendCommunicationEnable(WPARAM wParam, LPARAM lParam) {

	Send_Communication_Enable();

	return 0;
}

// FIELD HEADER A2 Respond_Possible_to_Communication USER DEFINE MESSAGE	|| Receiver
LRESULT CMFCSerialRecevieDlg::RespondPossibleToCommunication(WPARAM wParam, LPARAM lParam) {

	Respond_Possible_to_Communication();

	return 0;
}

// FIELD HEADER B1 Send_Same_FileName USER DEFINE MESSAGE					|| Sender
LRESULT CMFCSerialRecevieDlg::Send_Same_FileName(WPARAM wParam, LPARAM lParam) {

	Send_Same_FileName();

	return 0;
}

// FIELD HEADER B2 Respond_Same_FileName USER DEFINE MESSAGE				|| Receiver
LRESULT CMFCSerialRecevieDlg::RespondSameFIleName(WPARAM wParam, LPARAM lParam) {

	Respond_Same_FileName();

	return 0;
}

// FIELD HEADER B3 Send_FileLength USER DEFINE MESSAGE		
LRESULT CMFCSerialRecevieDlg::SendFileLength(WPARAM wParam, LPARAM lParam) {

	Send_FileLength();

	return 0;
}

// FIELD HEADER B4 RespondFileLength USER DEFINE MESSAGE
LRESULT CMFCSerialRecevieDlg::RespondFileLength(WPARAM wParam, LPARAM lParam) {

	Respond_FileLength();

	return 0;
}

// FIELD HEADER C1 SendData USER DEFINE MESSAGE
LRESULT CMFCSerialRecevieDlg::SendData(WPARAM wParam, LPARAM lParam) {

	Send_Data();

	return 0;
}

// FIELD HEADER C2 RespondSendData USER DEFINE MESSAGE
LRESULT CMFCSerialRecevieDlg::RespondSendData(WPARAM wParam, LPARAM lParam) {

	Respond_Send_Data();

	return 0;
}

// FIELD HEADER D1 SendClose USER DEFINE MESSAGE
LRESULT CMFCSerialRecevieDlg::SendClose(WPARAM wParam, LPARAM lParam) {

	Send_Close();

	return 0;
}

// FIELD HEADER D2 RespondSendClose USER DEFINE MESSAGE
LRESULT CMFCSerialRecevieDlg::RespondSendClose(WPARAM wParam, LPARAM lParam) {

	Respond_Close();

	return 0;
}

// 프로세스 에러 메시지
void CMFCSerialRecevieDlg::ProcessErrorMessage(CString msg)
{
	AfxMessageBox(msg);
}

// 시리얼 포트 닫기 버튼 구현
void CMFCSerialRecevieDlg::OnBnClickedSerialClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	::PostMessage(this->m_hWnd, WM_SEND_CLOSE, 0, 0);
	// m_ComuPort.ClosePort();
}

// File Name, Length 보낼 Button
void CMFCSerialRecevieDlg::OnBnClickedDataSend()
{
	//----------------------------------------------- Serial Communication File Name Send [0xB1] -----------------------------------------------
	// File Name

	CString strTemp = GetFileName(m_strPathName);
	m_strFileName = CT2CA(strTemp);
	m_arrcnt = 0;
	m_cnt = 0;
	m_Current_FilePointer = 0;
	m_PacketLen = 0;
	// Send_Same_FileName();
	::PostMessage(this->m_hWnd, WM_SEND_SAME_FILENAME, 0, 0);

	//::PostMessage(this->m_hWnd, WM_SEND_DATA, 0, 0);

}

// 0xA1
void CMFCSerialRecevieDlg::Send_Communication_Enable()
{
	std::list<BYTE> A1;
	BYTE str_a1[5]{};

	str_a1[0] = 0xAA;
	str_a1[1] = (byte)((5 >> 8) & 0xFF);
	str_a1[2] = (byte)(5 & 0xFF);
	str_a1[3] = 0xA1;
	str_a1[4] = 0xBB;

	for (int i = 0; i < sizeof(str_a1); i++) {
		A1.push_back(str_a1[i]);
	}
	

	SendComData(str_a1, 5);
}

// 0xA2
void CMFCSerialRecevieDlg::Respond_Possible_to_Communication()
{
	// TODO: 여기에 구현 코드 추가.

	// str_a2 = AA 00 05 A2 BB
	BYTE str_a2[5]{};
	str_a2[0] = 0xAA;
	str_a2[1] = (byte)((5 >> 8) & 0xFF);
	str_a2[2] = (byte)(5 & 0xFF);
	str_a2[3] = 0xA2;
	str_a2[4] = 0xBB;

	SendComData(str_a2, 5);

}

// 0xB1
void CMFCSerialRecevieDlg::Send_Same_FileName()
{
	// TODO: 여기에 구현 코드 추가
	
	// creat List
	std::list<BYTE> list_b1;
	BYTE str_b1[30]{};

	// 파일이름 길이
	int at = m_strFileName.size();
	int FileNameLen = at + 5;

	str_b1[0] = 0xAA;								// Header
	str_b1[1] = (byte)((FileNameLen >> 8) & 0xFF);	// read len_H
	str_b1[2] = (byte)(FileNameLen & 0xFF);			// read len_R
	str_b1[3] = 0xB1;								// Field Header

	BYTE* pName = (BYTE*)m_strFileName.c_str();

	memcpy(&str_b1[4], pName, m_strFileName.size() + 4);

	// List에 (헤더), (길이_H), (길이_R), (필드 헤더), (데이터) 집어 넣기
	for (int i = 0; i < FileNameLen - 1; i++) {
		list_b1.push_back(str_b1[i]);
	}

	// List에 (Tail) 집어 넣기
	list_b1.push_back(0xBB);

	// List Size
	int B1_list_size = list_b1.size();
	BYTE* pB1_Data = new BYTE[B1_list_size];

	for (int i = 0; i < B1_list_size; i++) {
		str_b1[i] = list_b1.front();
		list_b1.pop_front();
		pB1_Data[i] = str_b1[i];
	}

	SendComData(pB1_Data, B1_list_size);
}

// 0xB2
void CMFCSerialRecevieDlg::Respond_Same_FileName()
{
	std::list<BYTE> list_b2;
	BYTE str_b2[30]{};

	// 파일이름 길이
	int at = m_strTemp.size();
	int FileNameLen = at + 5;

	str_b2[0] = 0xAA;								// Header
	str_b2[1] = (byte)((FileNameLen >> 8) & 0xFF);	// read len_H
	str_b2[2] = (byte)(FileNameLen & 0xFF);			// read len_R
	str_b2[3] = 0xB2;

	BYTE* pName = (BYTE*)m_strTemp.c_str();

	memcpy(&str_b2[4], pName, m_strTemp.size() + 4);

	// List에 (헤더), (길이_H), (길이_R), (필드 헤더), (데이터) 집어 넣기
	for (int i = 0; i < FileNameLen - 1; i++) {
		list_b2.push_back(str_b2[i]);
	}

	// List에 (Tail) 집어 넣기
	list_b2.push_back(0xBB);

	// List Size
	int B2_list_size = list_b2.size();
	BYTE* pB2_Data = new BYTE[B2_list_size];

	for (int i = 0; i < B2_list_size; i++) {
		str_b2[i] = list_b2.front();
		list_b2.pop_front();
		pB2_Data[i] = str_b2[i];
	}

	SendComData(pB2_Data, B2_list_size);
	// TODO: 여기에 구현 코드 추가.
}

// 0xB3
void CMFCSerialRecevieDlg::Send_FileLength()
{
	// TODO: 여기에 구현 코드 추가.
	FILE* fp = fopen(m_strPathNamePure.c_str(), "rb");

	fseek(fp, 0, SEEK_END);
	m_len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	// creat List
	std::list<BYTE> list_b3;
	BYTE str_b3[9]{};

	str_b3[0] = 0xAA;						// Header
	str_b3[1] = (byte)((9 >> 8) & 0xFF);	// read len_H
	str_b3[2] = (byte)(9 & 0xFF);			// read len_R
	str_b3[3] = 0xB3;						// FIELD Header

	// File Length
	str_b3[4] = (byte)((m_len >> 24) & 0xFF);
	str_b3[5] = (byte)((m_len >> 16) & 0xFF);
	str_b3[6] = (byte)((m_len >> 8) & 0xFF);
	str_b3[7] = (byte)(m_len & 0xFF);

	str_b3[8] = 0xBB;						// Tail

	SendComData(str_b3, 9);

	fclose(fp);
}

// 0xB4
void CMFCSerialRecevieDlg::Respond_FileLength()
{
	// TODO: 여기에 구현 코드 추가.

	// creat List
	std::list<BYTE> list_b4;
	BYTE str_b4[9]{};

	str_b4[0] = 0xAA;						// Header
	str_b4[1] = (byte)((9 >> 8) & 0xFF);	// read len_H
	str_b4[2] = (byte)(9 & 0xFF);			// read len_R
	str_b4[3] = 0xB4;						// FIELD Header

	// File Length
	str_b4[4] = (byte)((m_fileLength >> 24) & 0xFF);
	str_b4[5] = (byte)((m_fileLength >> 16) & 0xFF);
	str_b4[6] = (byte)((m_fileLength >> 8) & 0xFF);
	str_b4[7] = (byte)(m_fileLength & 0xFF);

	str_b4[8] = 0xBB;						// Tail

	SendComData(str_b4, 9);

}

// fopen / fclose 
// memorize sent file position


// 0xC1
void CMFCSerialRecevieDlg::Send_Data()
{
	// fopen
	FILE* fp;
	fp = fopen(m_strPathNamePure.c_str(), "rb");

	fseek(fp, m_Current_FilePointer, SEEK_SET);

	// create List
	std::list<BYTE> list_c1;
	BYTE* pC1_Data{};

	// 1. send file data(1023bytes)
	BYTE str_c1[SEND_DATA_LEN+4]{};

	str_c1[0] = 0xAA;
	str_c1[1] = (byte)(((SEND_DATA_LEN + 4) >> 8) & 0xFF);
	str_c1[2] = (byte)((SEND_DATA_LEN + 4) & 0xFF);
	str_c1[3] = 0xC1;

	// 2. processig end of file 
	int quotient = m_len / (SEND_DATA_LEN - 1);			// len - 1023
	int remainder = m_len % (SEND_DATA_LEN - 1);

	TRACE("[quotient] = [%d]\n", quotient);
	TRACE("[Read Count] = [%d]\n", m_cnt);
	
	// int32_t read_len = 0;
	if (quotient > m_cnt) {
		fread(&str_c1[4], 1, SEND_DATA_LEN-1, fp);
		
		// Put Data into a List
		for (int i = 0; i < SEND_DATA_LEN + 3; i++) {
			list_c1.push_back(str_c1[i]);
		}

		// (Tail)
		list_c1.push_back(0xBB);

		// List Size
		int C1_list_size = list_c1.size();
		BYTE pC1_Data[SEND_DATA_LEN+4]{};

		for (int i = 0; i < C1_list_size; i++) {
			str_c1[i] = list_c1.front();
			list_c1.pop_front();
			pC1_Data[i] = str_c1[i];
		}
		m_cnt++;

		SendComData(pC1_Data, C1_list_size);
		// memorize sent file position
		m_Current_FilePointer = ftell(fp);
		// TRACE("[file pointer] = [%d]\n", m_Current_FilePointer);
	}
	else if (quotient == m_cnt) {

		str_c1[1] = (byte)(((remainder + 5) >> 8) & 0xFF);
		str_c1[2] = (byte)((remainder + 5) & 0xFF);

		fread(&str_c1[4], 1, remainder, fp);

		// Put Data into a List
		for (int i = 0; i < remainder + 4; i++) {
			list_c1.push_back(str_c1[i]);
		}
		list_c1.push_back(0xBB);

		// List Size
		int C1_list_size = list_c1.size();
		BYTE* pC1_Data = new BYTE[C1_list_size];

		for (int i = 0; i < C1_list_size; i++) {
			str_c1[i] = list_c1.front();
			list_c1.pop_front();
			pC1_Data[i] = str_c1[i];
		}

		SendComData(pC1_Data, C1_list_size);
		// memorize sent file position
		m_Current_FilePointer = ftell(fp);
		TRACE("[Remainder Pointer] = [%d]", m_Current_FilePointer);
		m_cnt++;
	}

	// 3. send d1 when it finished seding all file data.
	else if (quotient < m_cnt) {

		str_c1[0] = 0xAA;
		str_c1[1] = (byte)((5 >> 8) & 0xFF);
		str_c1[2] = (byte)(5 & 0xFF);
		str_c1[3] = 0xD1;
		str_c1[4] = 0xBB;

		SendComData(str_c1, 5);
	}
	
	// fclose
	delete[] pC1_Data;
	fclose(fp);

}

// 0xC2
void CMFCSerialRecevieDlg::Respond_Send_Data()
{
	// TODO: 여기에 구현 코드 추가.

	std::list<BYTE> list_c2;
	BYTE* pC2_Data = new BYTE[SEND_DATA_LEN + 4];

	// 1. send file data(1023bytes)
	BYTE str_c2[SEND_DATA_LEN + 4]{};

	str_c2[0] = 0xAA;
	str_c2[1] = (byte)(((SEND_DATA_LEN + 4) >> 8) & 0xFF);
	str_c2[2] = (byte)((SEND_DATA_LEN + 4) & 0xFF);
	str_c2[3] = 0xC2;

	TRACE("[CNT] = [%d]\n", m_cnt);

	int quotient = m_fileLength / (SEND_DATA_LEN - 1);			// len - 1023
	int remainder = m_fileLength % (SEND_DATA_LEN - 1);

	if (quotient > m_cnt) {
		for (int i = 0; i < 4; i++) {
			list_c2.push_back(str_c2[i]);
		}

		// Put Data into a List
		for (int i = 0; i < SEND_DATA_LEN - 1; i++) {
			list_c2.push_back(m_WriteByte[i]);
		}

		list_c2.push_back(0xBB);

		int C2_list_size = list_c2.size();
		BYTE temp_c2[SEND_DATA_LEN + 5]{};

		for (int i = 0; i < C2_list_size; i++) {
			temp_c2[i] = list_c2.front();
			list_c2.pop_front();
			pC2_Data[i] = temp_c2[i];
		}
		m_cnt++;

		SendComData(pC2_Data, C2_list_size);
	}

	else if (quotient == m_cnt) {
		str_c2[1] = (byte)(((remainder + 5) >> 8) & 0xFF);
		str_c2[2] = (byte)((remainder + 5) & 0xFF);

		for (int i = 0; i < 4; i++) {
			list_c2.push_back(str_c2[i]);
		}

		// Put Data into a List
		for (int i = 0; i < remainder; i++) {
			list_c2.push_back(m_WriteByte[i]);
		}

		list_c2.push_back(0xBB);

		int C2_list_size = list_c2.size();
		BYTE* temp_c2 = new BYTE[remainder +5]{};

		for (int i = 0; i < C2_list_size; i++) {
			temp_c2[i] = list_c2.front();
			list_c2.pop_front();
			pC2_Data[i] = temp_c2[i];
		}
		SendComData(pC2_Data, C2_list_size);
		m_cnt++;
	}

	else if (quotient < m_cnt) {
		str_c2[0] = 0xAA;
		str_c2[1] = (byte)((5 >> 8) & 0xFF);
		str_c2[2] = (byte)(5 & 0xFF);
		str_c2[3] = 0xD2;
		str_c2[4] = 0xBB;

		SendComData(str_c2, 5);
	}
}

// 0xD1
void CMFCSerialRecevieDlg::Send_Close()
{

	// TODO: 여기에 구현 코드 추가.

	BYTE str_d1[5]{};
	str_d1[0] = 0xAA;
	str_d1[1] = (byte)((5 >> 8) & 0xFF);
	str_d1[2] = (byte)(5 & 0xFF);
	str_d1[3] = 0xD2;
	str_d1[4] = 0xBB;

	SendComData(str_d1, 5);
}

// 0xD2
void CMFCSerialRecevieDlg::Respond_Close()
{
	// TODO: 여기에 구현 코드 추가.
	BYTE str_d2[5]{};
	str_d2[0] = 0xAA;
	str_d2[1] = (byte)((5 >> 8) & 0xFF);
	str_d2[2] = (byte)(5 & 0xFF);
	str_d2[3] = 0xD2;
	str_d2[4] = 0xBB;

	SendComData(str_d2, 5);
}

// List Bot에 [Send Data]OO 보낼 함수
int CMFCSerialRecevieDlg::SendComData(BYTE* pData, int nLen)
{
	// TODO: 여기에 구현 코드 추가.

	CString str;
	CString strtemp;

	str = _T("[SendData] ");
	for (int i = 0; i < nLen; i++) {
		strtemp.Format(_T("%02X "), pData[i]);
		str += strtemp;
	}

	m_serialRspondData.AddString(str);

	m_ComuPort.WriteComm(pData, nLen);
	return 0;
}

// Serial Communication Start Button
void CMFCSerialRecevieDlg::OnBnClickedSerialStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Send_Communication_Enable();
	::PostMessage(this->m_hWnd, WM_SEND_COMMUNICATION_ENABLE, 0, 0);
}


