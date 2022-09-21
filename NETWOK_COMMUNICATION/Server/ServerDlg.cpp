
// ChatServerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ChatServer.h"
#include "ChatServerDlg.h"
#include "afxdialogex.h"
#include "protocol.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_AA_SERIAL_COMMUNICATION_START_OF_HEADER	WM_USER + 1008	// FIELD HEADER AA
#define WM_A1_SEND_COMMUNICATION_ENABLE				WM_USER + 1009	// FIELD HEADER A1
#define WM_A2_RESPOND_POSSIBLE_TO_COMMUNICATION		WM_USER + 1010	// FIELD HEADER A2
#define WM_B1_SEND_SAME_FILENAME					WM_USER + 1011	// FIELD HEADER B1
#define WM_B2_RESPOND_SAME_FILENAME					WM_USER + 1012	// FIELD HEADER B2
#define WM_B3_SEND_FILE_LENGTH						WM_USER + 1013	// FIELD HEADER B3
#define WM_B4_RESPOND_FILE_LENGTH					WM_USER + 1014	// FIELD HEADER B4
#define WM_C1_SEND_DATA								WM_USER + 1015	// FIELD HEADER C1
#define WM_C2_RESPOND_SEND_DATA						WM_USER + 1016	// FIELD HEADER C2
#define WM_D1_SEND_CLOSE							WM_USER + 1017	// FIELD HEADER D1
#define WM_D2_RESPOND_SEND_CLOSE					WM_USER + 1018	// FIELD HEADER D2


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


// CChatServerDlg 대화 상자



CChatServerDlg::CChatServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATSERVER_DIALOG, pParent)
	, m_strStatus(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_FileLen = 0;
	m_bFindAA = false;
	memset(m_Packet, 0, 1030);
	m_arrcnt = 0;
	m_PacketLen = 0;
	m_cnt = 0;
}

void CChatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_STATIC_STATUS, m_strStatus);
}

BEGIN_MESSAGE_MAP(CChatServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	// *** 사용자 정의 메시지 설정 ***
	ON_MESSAGE(WM_AA_SERIAL_COMMUNICATION_START_OF_HEADER, &CChatServerDlg::SerialCommunicationStratOfHeader)	// FIELD HEADER AA (Start Of Header) 
	ON_MESSAGE(WM_A1_SEND_COMMUNICATION_ENABLE, &CChatServerDlg::SendCommunicationEnable)						// FIELD HEADER A1
	ON_MESSAGE(WM_A2_RESPOND_POSSIBLE_TO_COMMUNICATION, &CChatServerDlg::RespondPossibleToCommunication)		// FIELD HEADER A2
	ON_MESSAGE(WM_B1_SEND_SAME_FILENAME, &CChatServerDlg::Send_Same_FileName)									// FIELD HEADER B1
	ON_MESSAGE(WM_B3_SEND_FILE_LENGTH, &CChatServerDlg::SendFileLength)											// FIELD HEADER B3
	ON_MESSAGE(WM_C1_SEND_DATA, &CChatServerDlg::SendData)														// FIELD HEADER C1
	ON_MESSAGE(WM_D1_SEND_CLOSE, &CChatServerDlg::SendClose)													// FIELD HEDAER D1
	/*
	ON_MESSAGE(WM_RESPOND_SEND_DATA, &CChatServerDlg::RespondSendData)											// FIELD HEADER C2
	ON_MESSAGE(WM_RESPOND_SEND_CLOSE, &CChatServerDlg::RespondSendClose)										// FIELD HEADER D2
	*/

	// 메시지 함수 추가
	ON_MESSAGE(UM_ACCEPT, OnAccept)
	ON_MESSAGE(UM_RECEIVE, OnReceive)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CChatServerDlg::OnClickedButtonSend)
	ON_BN_CLICKED(IDC_FILE_OPEN, &CChatServerDlg::OnBnClickedFileOpen)
	ON_BN_CLICKED(IDC_SERVER_OPEN, &CChatServerDlg::OnBnClickedServerOpen)
END_MESSAGE_MAP()


// CChatServerDlg 메시지 처리기

BOOL CChatServerDlg::OnInitDialog()
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
	this->SetWindowText(_T("채팅 서버"));

	m_socCom = NULL;
/*
	// 서버 소켓을 생성(포트번호 5000)
	m_socServer.Create(8888);
	// 클라이언트의 접속을 기다림
	m_socServer.Listen();
	// 소켓 클래스와 메인 윈도우(여기에서는 CChatServerDlg)를 연결
	m_socServer.Init(this->m_hWnd);

	*/

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CChatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChatServerDlg::OnPaint()
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
HCURSOR CChatServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ex) C:\\test\\Butter.mp3 -> Butter.mp3
std::string CChatServerDlg::strPathSubstr(std::string strTemp)
{
	int pos;

	pos = strTemp.rfind('\\');
	std::string second = strTemp.substr(pos + 1);
	return  second;
}

void CChatServerDlg::OnBnClickedFileOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*
	TCHAR fileFilter[] = _T("nmea(*.nmea)|*.nmea|");
	*/
	TCHAR fileFilter[] = _T("mp3(*.mp3)|*.mp3|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_CREATEPROMPT, fileFilter);

	if (dlg.DoModal() == IDOK)
	{
		m_CstrPathName = dlg.GetPathName(); // MFC 파일열기 대화상자(CFileDialog) 선택한 파일의 전체 경로 반환.
		m_strPathName = std::string(CT2CA(m_CstrPathName));

		// 절대경로 뺀 파일 이름
		m_strPureName = strPathSubstr(std::string(CT2CA(m_CstrPathName)));
	}
}
// 클라이언트 연결 요청이 왔기 때문에 Accept 함수로 접속
// 실제 접속을 담당하는 것은 CSocServer
// 이렇게 접속한 소켓은 (GetAcceptSocCom)을 이용해 얻어옴
// 
LRESULT CChatServerDlg::OnAccept(WPARAM wParam, LPARAM lParam)
{
	// 클라이언트에서 IP,Port 번호로 접속하면 Listen 상태에서 Accept 상태로 바뀐다. 
	// == Server 와 Client가 붙는다.
	m_strStatus = "접속 성공";
	
	// 통신용 소켓 생성
	m_socCom = new CSocCom;

	// 서버소켓과 통신소켓을 연결
	m_socCom = m_socServer.GetAcceptSocCom();
	m_socCom->Init(this->m_hWnd);
	
	PostMessage(WM_A1_SEND_COMMUNICATION_ENABLE, 0, 0);
	UpdateData(FALSE);
	
	return TRUE;
}

// 데이터를 보내는 것 ==  소켓 클래스의 멤버 함수인 (Send)이용
// 데이터 받을 때 == 소켓 클래스의 오버라이딩한 OnReceive 메시지 함수 사용
LRESULT CChatServerDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{
	BYTE buffer[1030]{};
	// CSocket Receive Function return value == int
	int ReceivedLen = m_socCom->Receive(buffer, 1030);

	for (int i = 0; i < ReceivedLen; i++) {

		if (m_bFindAA == false && buffer[i] == FIELD_HEADER_PROTOCOL_AA_SERIAL_COMMUNICATION_START_OF_HEADER) {
			m_bFindAA = true;
			m_arrcnt = 0;
		}

		m_Packet[m_arrcnt] = buffer[i];

		if (m_arrcnt == 2) {
			int a = (int)((m_Packet[1] << 8) & 0xFF00);
			int b = (int)(m_Packet[2] & 0xFF);
			m_PacketLen = a + b;
		}

		if (m_PacketLen == (m_arrcnt + 1)) {
			if (m_Packet[m_PacketLen - 1] == FIELD_HEADER_PROTOCOL_BB_SERIAL_COMMUNICATION_END_OF_HEADER) {
				TRACE("[Make Packet]\n");

				BYTE* Packet = new BYTE[1030];

				memcpy(Packet, m_Packet, m_PacketLen);

				m_Packet_list.push_back(Packet);

				PostMessage(WM_AA_SERIAL_COMMUNICATION_START_OF_HEADER, NULL, NULL);
			}
			else {
				TRACE("[Doesn't Make Packet]\n");
			}

			m_bFindAA = false;
		}

		m_arrcnt++;
	}

	return 0;
}

LRESULT CChatServerDlg::SerialCommunicationStratOfHeader(WPARAM wParam, LPARAM lParam)
{
	BYTE* Packet = NULL;
	Packet = m_Packet_list.front();

	// Len_H + Len_R
	int packlen = (int)((Packet[1] << 8) & 0xFF00) + (int)(Packet[2] & 0xFF);

	CString str;
	CString strTemp;

	str = _T("[RecvData] ");
	for (int i = 0; i < packlen; i++) {
		strTemp.Format(_T("%02X "), Packet[i]);
		str += strTemp;
	}

	// [RecvData] ...................
	m_list.AddString(str);

	
	if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_A2_SERIAL_COMMUNICATION_RECEIVE) {
		//
		//m_Packet_list.pop_front();
	}

	else if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_B2_FILENAME_RECEIVE) {

		::PostMessage(this->m_hWnd, WM_B3_SEND_FILE_LENGTH, 0, 0);
	}

	else if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_B4_FILELENGTH_LENGTH) {
		// send C1
		::PostMessage(this->m_hWnd, WM_C1_SEND_DATA, 0, 0);
	}

	else if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_C2_RESOPOND_SEND_DATA) {
		// send C1
		::PostMessage(this->m_hWnd, WM_C1_SEND_DATA, 0, 0);
	}

	else if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_D2_RESPOND_RECEIVE_DATA) {

		
	}

	// new allocation delete
	m_Packet_list.pop_front();
	delete Packet;

	return 0;
}

// FIELD HEADER A1 SendCommunicationEnable USER DEFINE MESSAGE				|| Sender
LRESULT CChatServerDlg::SendCommunicationEnable(WPARAM wParam, LPARAM lParam) {

	Send_Communication_Enable();

	return 0;
}

// FIELD HEADER A2 Respond_Possible_to_Communication USER DEFINE MESSAGE	|| Receiver
LRESULT CChatServerDlg::RespondPossibleToCommunication(WPARAM wParam, LPARAM lParam)
{

	Respond_Possible_to_Communication();

	return 0;
}

// FIELD HEADER B1 Send_Same_FileName USER DEFINE MESSAGE					|| Sender
LRESULT CChatServerDlg::Send_Same_FileName(WPARAM wParam, LPARAM lParam)
{
	Send_Same_FileName();

	return 0;
}

// FIELD HEADER B3 Send_FileLength USER DEFINE MESSAGE						|| Sender
LRESULT CChatServerDlg::SendFileLength(WPARAM wParam, LPARAM lParam) {

	Send_FileLength();

	return 0;
}

// FIELD HEADER C1 SendData USER DEFINE MESSAGE								|| Sender
LRESULT CChatServerDlg::SendData(WPARAM wParam, LPARAM lParam) {

	Send_Data();

	return 0;
}

// FIELD HEADER D1 SendClose USER DEFINE MESSAGE
LRESULT CChatServerDlg::SendClose(WPARAM wParam, LPARAM lParam) {

	Send_Close();

	return 0;
}

// 0xA1
void CChatServerDlg::Send_Communication_Enable()
{
	BYTE str_a1[5]{};

	str_a1[0] = 0xAA;
	str_a1[1] = (byte)((5 >> 8) & 0xFF);
	str_a1[2] = (byte)(5 & 0xFF);
	str_a1[3] = 0xA1;
	str_a1[4] = 0xBB;

	SendComData(str_a1, 5);
}

void CChatServerDlg::Respond_Possible_to_Communication()
{
	BYTE str_a2[5]{};
	str_a2[0] = 0xAA;
	str_a2[1] = (byte)((5 >> 8) & 0xFF);
	str_a2[2] = (byte)(5 & 0xFF);
	str_a2[3] = 0xA2;
	str_a2[4] = 0xBB;

	SendComData(str_a2, 5);
}

// 0xB1
void CChatServerDlg::Send_Same_FileName()
{
	// Create List
	std::list<BYTE> list_FileName;
	BYTE str_FileName[30]{};

	int FileNameLen = m_strPureName.length();
	int includeProtocolFileNameLen = m_strPureName.length() + 5;

	str_FileName[0] = 0xAA;	// SOH
	str_FileName[1] = (byte)((includeProtocolFileNameLen >> 8) & 0xFF);	// FileName Len_H
	str_FileName[2] = (byte)(includeProtocolFileNameLen & 0xFF);		// FileName Len_R
	str_FileName[3] = 0xB1;

	BYTE* pName = (BYTE*)m_strPureName.c_str();	// std::string -> BYTE*	(TYPE Conversion)

	memcpy(&str_FileName[4], pName, FileNameLen + 4);

	for (int i = 0; i < FileNameLen + 4; i++) {
		list_FileName.push_back(str_FileName[i]);
	}

	// LIst에 Tail(BB) 집어넣기
	list_FileName.push_back(0xBB);
	int FileNameListSize = list_FileName.size();
	BYTE* pFileNameData = new BYTE[FileNameListSize];

	// pFileNameData 배열 모두 0으로 초기화
	memset(pFileNameData, '\0', FileNameListSize);

	for (int i = 0; i < includeProtocolFileNameLen; i++) {
		pFileNameData[i] = list_FileName.front();
		list_FileName.pop_front();
	}

	//m_socCom->Send(pFileNameData, includeProtocolFileNameLen);

	SendComData(pFileNameData, includeProtocolFileNameLen);

	delete[] pFileNameData;
}

// 0xB3
void CChatServerDlg::Send_FileLength()
{
	m_pFile = fopen(m_strPathName.c_str(), "rb");

	fseek(m_pFile, 0, SEEK_END);
	m_FileLen = ftell(m_pFile);
	fseek(m_pFile, 0, SEEK_SET);

	BYTE fileLen[9];

	fileLen[0] = 0xAA;
	fileLen[1] = (byte)((9 >> 8) & 0xFF);	// read len_H
	fileLen[2] = (byte)(9 & 0xFF);			// read len_R
	fileLen[3] = 0xB3;

	// File Length
	fileLen[4] = (byte)((m_FileLen >> 24) & 0xFF);
	fileLen[5] = (byte)((m_FileLen >> 16) & 0xFF);
	fileLen[6] = (byte)((m_FileLen >> 8) & 0xFF);
	fileLen[7] = (byte)(m_FileLen & 0xFF);

	fileLen[8] = 0xBB;

	SendComData(fileLen, 9);

	fclose(m_pFile);
}

// 0xC1
void CChatServerDlg::Send_Data()
{
	// fopen
	FILE* fp;
	fp = fopen(m_strPathName.c_str(), "rb");

	fseek(fp, m_Current_FilePointer, SEEK_SET);

	// create List
	std::list<BYTE> list_c1;

	BYTE* str_c1 = new BYTE[DATA_SEND_SIZE + 5]{};

	str_c1[0] = 0xAA;
	str_c1[1] = (byte)(((DATA_SEND_SIZE + 5) >> 8) & 0xFF);
	str_c1[2] = (byte)((DATA_SEND_SIZE + 5) & 0xFF);
	str_c1[3] = 0xC1;

	// 2. processig end of file 
	int quotient = m_FileLen / DATA_SEND_SIZE ;
	int remainder = m_FileLen % DATA_SEND_SIZE;

	if (quotient > m_cnt) {
		fread(&str_c1[4], 1, DATA_SEND_SIZE, fp);

		for (int i = 0; i < DATA_SEND_SIZE + 4; i++) {
			list_c1.push_back(str_c1[i]);
		}

		list_c1.push_back(0xBB);

		int C1_list_size = list_c1.size();

		for (int i = 0; i < C1_list_size; i++) {
			str_c1[i] = list_c1.front();
			list_c1.pop_front();
		}
		m_cnt++;

		SendComData(str_c1, C1_list_size);
		m_Current_FilePointer = ftell(fp);
		delete[] str_c1;
	}

	else if (quotient == m_cnt) {

		str_c1[1] = (byte)(((remainder + 5) >> 8) & 0xFF);
		str_c1[2] = (byte)((remainder + 5) & 0xFF);

		fread(&str_c1[4], 1, remainder, fp);

		for (int i = 0; i < remainder + 4; i++) {
			list_c1.push_back(str_c1[i]);
		}
		list_c1.push_back(0xBB);

		// List Size
		int C1_list_size = list_c1.size();

		for (int i = 0; i < C1_list_size; i++) {
			str_c1[i] = list_c1.front();
			list_c1.pop_front();
		}

		SendComData(str_c1, C1_list_size);
		// memorize sent file position
		m_Current_FilePointer = ftell(fp);
		m_cnt++;

		delete[] str_c1;
	}

	// 3. send d1 when it finished seding all file data.
	else if (quotient < m_cnt) {

		str_c1[0] = 0xAA;
		str_c1[1] = (byte)((5 >> 8) & 0xFF);
		str_c1[2] = (byte)(5 & 0xFF);
		str_c1[3] = 0xD1;
		str_c1[4] = 0xBB;

		SendComData(str_c1, 5);
		delete[] str_c1;
	}

	// fclose
	fclose(fp);

}

void CChatServerDlg::Send_Close()
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
// ServerToClient Connect Success
// ServerOpen 버튼을 누르면 서버는 Listen 상태로 대기
void CChatServerDlg::OnBnClickedServerOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 서버 소켓을 생성(포트번호 8888)
	m_socServer.Create(8888);
	// 클라이언트의 접속을 기다림
	m_socServer.Listen();
	// 소켓 클래스와 메인 윈도우(여기에서는 CChatServerDlg)를 연결
	m_socServer.Init(this->m_hWnd);
}

// 데이터 클라이언트쪽으로 보내기 SendComData(CSocket (Send Function))
void CChatServerDlg::OnClickedButtonSend()
{
	m_Current_FilePointer = 0;
	m_arrcnt = 0;
	m_cnt = 0;
	::PostMessage(this->m_hWnd, WM_B1_SEND_SAME_FILENAME, 0, 0);
}

// List Box에 [Send Data]00 보낼 함수
int CChatServerDlg::SendComData(BYTE* pData, int nLen)
{
	// TODO: 여기에 구현 코드 추가.
	CString str;
	CString strTemp;

	str = _T("[SendData] ");
	for (int i = 0; i < nLen; i++) {
		strTemp.Format(_T("%02X "), pData[i]);
		str += strTemp;
	}

	m_list.AddString(str);

	m_socCom->Send(pData, nLen);

	return 0;
}


