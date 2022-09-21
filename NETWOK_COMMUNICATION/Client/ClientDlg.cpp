
// ChatClientDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ChatClient.h"
#include "ChatClientDlg.h"
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


// CChatClientDlg 대화 상자



CChatClientDlg::CChatClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATCLIENT_DIALOG, pParent)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//  m_strIP = _T("");
	m_FilLength = 0;
	m_bFindAA = false;
	memset(m_Packet, 0, 1030);
	m_arrcnt = 0;
	m_PacketLen = 0;
	m_cnt = 0;
}

void CChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_EDIT_IP, m_strIP);
}

BEGIN_MESSAGE_MAP(CChatClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	// *** 사용자 정의 메시지 설정 ***
	ON_MESSAGE(WM_AA_SERIAL_COMMUNICATION_START_OF_HEADER, &CChatClientDlg::SerialCommunicationStratOfHeader)	// FIELD HEADER AA (Start Of Header) 
	ON_MESSAGE(WM_A1_SEND_COMMUNICATION_ENABLE, &CChatClientDlg::SendCommunicationEnable)							// FIELD HEADER A1
	ON_MESSAGE(WM_A2_RESPOND_POSSIBLE_TO_COMMUNICATION, &CChatClientDlg::RespondPossibleToCommunication)		// FIELD HEADER A2
	ON_MESSAGE(WM_B2_RESPOND_SAME_FILENAME, &CChatClientDlg::RespondSameFIleName)								// FIELD HEADER B2
	ON_MESSAGE(WM_B4_RESPOND_FILE_LENGTH, &CChatClientDlg::RespondFileLength)									// FIELD HEADER B4
	ON_MESSAGE(WM_C2_RESPOND_SEND_DATA, &CChatClientDlg::RespondSendData)										// FIELD HEADER C2
	ON_MESSAGE(WM_D2_RESPOND_SEND_CLOSE, &CChatClientDlg::RespondSendClose)										// FIELD HEADER D2
	/*
	ON_MESSAGE(WM_SEND_SAME_FILENAME, &CChatClientDlg::Send_Same_FileName)										// FIELD HEADER B1
	ON_MESSAGE(WM_SEND_FILE_LENGTH, &CChatClientDlg::SendFileLength)											// FIELD HEADER B3
	ON_MESSAGE(WM_SEND_DATA, &CChatClientDlg::SendData)															// FIELD HEADER C1
	ON_MESSAGE(WM_SEND_CLOSE, &CChatClientDlg::SendClose)														// FIELD HEDAER D1
	*/

	ON_MESSAGE(UM_RECEIVE, OnReceive)
	ON_BN_CLICKED(IDC_BUTTON_CONNET, &CChatClientDlg::OnClickedButtonConnet)
END_MESSAGE_MAP()


// CChatClientDlg 메시지 처리기

BOOL CChatClientDlg::OnInitDialog()
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
	this->SetWindowTextW(_T("클라이언트 서버"));

	// IP 초기값 설정
	m_str_IP = _T("127.0.0.1");
	m_strIP.SetWindowTextW(m_str_IP);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChatClientDlg::OnPaint()
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
HCURSOR CChatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 서버에서 보낸 데이터 받기 (CSocket (Receive Function))
LRESULT CChatClientDlg::OnReceive(WPARAM wParam, LPARAM lParam)
{
	BYTE buffer[1030]{};
	int ReceivedLen = m_socCom.Receive(buffer, 1030);

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

				// New Make Packet If Last buffer is 'BB'
				BYTE* Packet = new BYTE[1030];
				// BYTE Packet[1030];

				// Memory Copy from m_Packet to Packet	
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

LRESULT CChatClientDlg::SerialCommunicationStratOfHeader(WPARAM wParam, LPARAM lParam)
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

	if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_A1_SERIAL_COMMUNICATION_SEND) {

		::PostMessage(this->m_hWnd, WM_A2_RESPOND_POSSIBLE_TO_COMMUNICATION, 0, 0);

	}

	else if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_B1_FILENAME_SEND) {

		for (int i = 4; i < packlen - 1; i++) {
			m_strFileName += Packet[i];
		}

		/*
		TCHAR chFilePath[256]{};
		GetModuleFileName(NULL, chFilePath, 256);

		CString strFolderPath(chFilePath);
		strFolderPath = strFolderPath.Left(strFolderPath.ReverseFind('\\'));
		// CString strPure = GetFileName(m_strPathName);
		CString strPure = (CA2CT(m_strFileName.c_str()));
		CString total = strFolderPath + "\\" + strPure;

		m_WriteStrPathName = std::string(CT2CA(total));
		*/

		std::string str = "C:\\test\\test.";
		m_WriteStrPathName = str + m_strFileName;
		
		// todo memorize filename
		TRACE("[filename] %s\n", m_WriteStrPathName);

		DeleteFile(CA2CT(m_WriteStrPathName.c_str()));

		::PostMessage(this->m_hWnd, WM_B2_RESPOND_SAME_FILENAME, 0, 0);
	}

	else if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_B3_FILELENGTH_SEND) {

		m_FilLength = (int)((Packet[4] << 24) & 0xFF000000) + (int)((Packet[5] << 16) & 0xFF0000) + (int)((Packet[6] << 8) & 0xFF00) + (int)((Packet[7]) & 0xFF);

		::PostMessage(this->m_hWnd, WM_B4_RESPOND_FILE_LENGTH, 0, 0);
	}

	else if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_C1_DATA_SEND) {

		FILE* fp = fopen(m_WriteStrPathName.c_str(), "a+b");
		fseek(fp, 0, SEEK_END);

		// AA, Len_H, Len_R, CMD, Tail = 5
		int buffer_len = packlen - 5;

		for (int i = 4; i < packlen; i++) {
			// 0xBB면 break
			if (i == packlen - 1) {
				break;
			}
			else {
				(BYTE)m_WriteByte[i - 4] = Packet[i];
			}
		}

		fwrite(m_WriteByte, buffer_len, 1, fp);

		fclose(fp);

		::PostMessage(this->m_hWnd, WM_C2_RESPOND_SEND_DATA, 0, 0);
	}

	else if (Packet[3] == (BYTE)FIELD_HEADER_PROTOCOL_D1_DATA_RECEIVE) {
		// send D2
		// fclose(m_wfp);
		::PostMessage(this->m_hWnd, WM_D2_RESPOND_SEND_CLOSE, 0, 0);
	}

	// new allocation delete
	m_Packet_list.pop_front();
	delete Packet;

	return 0;
}

// FIELD HEADER A1 SendCommunicationEnable USER DEFINE MESSAGE				|| Sender
LRESULT CChatClientDlg::SendCommunicationEnable(WPARAM wParam, LPARAM lParam) {

	Send_Communication_Enable();

	return 0;
}

// FIELD HEADER A2 Respond_Possible_to_Communication USER DEFINE MESSAGE	|| Receiver
LRESULT CChatClientDlg::RespondPossibleToCommunication(WPARAM wParam, LPARAM lParam)
{

	Respond_Possible_to_Communication();

	return 0;
}

// FIELD HEADER B2 Respond_Same_FileName USER DEFINE MESSAGE				|| Receiver
LRESULT CChatClientDlg::RespondSameFIleName(WPARAM wParam, LPARAM lParam)
{

	Respond_Same_FileName();

	return 0;
}

// FIELD HEADER B4 RespondFileLength USER DEFINE MESSAGE					|| Receiver
LRESULT CChatClientDlg::RespondFileLength(WPARAM wParam, LPARAM lParam)
{

	Respond_FileLength();

	return 0;

}

// FIELD HEADER C2 RespondSendData USER DEFINE MESSAGE
LRESULT CChatClientDlg::RespondSendData(WPARAM wParam, LPARAM lParam) {

	Respond_Send_Data();

	return 0;
}

// FIELD HEADER D2 RespondSendClose USER DEFINE MESSAGE
LRESULT CChatClientDlg::RespondSendClose(WPARAM wParam, LPARAM lParam) {

	Respond_Close();

	return 0;
}

// 서버에 접속 요청
// 소켓 클라이언트의 Create함수에서 인자가 있으면 서버로 인식, 
// 결과로 클라이언트로 판단해서 동작
// Create로 소켓을 생성했다면 --> 실제 접속은 Connect 함수를 이용
// 첫번째 인자를 이용해(m_strIP는 에디터 컨트롤과 연결된 멤버 변수)
// 접속할 곳의 IP 주소를 넘기며 두번째 인자를 통해 접속할 포트 번호를 지정

void CChatClientDlg::OnClickedButtonConnet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 접속
	UpdateData(TRUE);

	// 클라이언트 소켓 생성
	m_socCom.Create();

	// IP주소와 포트 번호를 지정
	
	m_socCom.Connect(m_str_IP, 8888);

	// 클라이언트 소켓 초기화
	m_socCom.Init(this->m_hWnd);
	
}

// 0xA1
void CChatClientDlg::Send_Communication_Enable()
{
	BYTE str_a1[5]{};

	str_a1[0] = 0xAA;
	str_a1[1] = (byte)((5 >> 8) & 0xFF);
	str_a1[2] = (byte)(5 & 0xFF);
	str_a1[3] = 0xA1;
	str_a1[4] = 0xBB;

	SendComData(str_a1, 5);
}

// 0xA2
void CChatClientDlg::Respond_Possible_to_Communication()
{
	// TODO: 여기에 구현 코드 추가.

	BYTE str_a2[5]{};
	str_a2[0] = 0xAA;
	str_a2[1] = (byte)((5 >> 8) & 0xFF);
	str_a2[2] = (byte)(5 & 0xFF);
	str_a2[3] = 0xA2;
	str_a2[4] = 0xBB;

	SendComData(str_a2, 5);

}

// 0xB2
void CChatClientDlg::Respond_Same_FileName()
{
	std::list<BYTE> list_FileName;
	BYTE fileName[30]{};

	int FileNameLen = m_strFileName.length();
	int includeProtocolFileNameLen = m_strFileName.length() + 5;

	fileName[0] = 0xAA;
	fileName[1] = (byte)((includeProtocolFileNameLen >> 8) & 0xFF);
	fileName[2] = (byte)(includeProtocolFileNameLen & 0xFF);
	fileName[3] = 0xB2;

	BYTE* pName = (BYTE*)m_strFileName.c_str();

	memcpy(&fileName[4], pName, FileNameLen + 4);

	for (int i = 0; i < FileNameLen + 4; i++) {
		list_FileName.push_back(fileName[i]);
	}

	list_FileName.push_back(0xBB);

	int FileNameListSize = list_FileName.size();
	BYTE* pFileNameData = new BYTE[FileNameListSize];

	
	// pFileNameData 배열 모두 0으로 초기화
	memset(pFileNameData, '\0', FileNameListSize);
	

	for (int i = 0; i < includeProtocolFileNameLen; i++) {
		pFileNameData[i] = list_FileName.front();
		list_FileName.pop_front();
	}

	SendComData(pFileNameData, includeProtocolFileNameLen);

	delete[] pFileNameData;
}

// 0xB4
void CChatClientDlg::Respond_FileLength()
{
	BYTE fileLen[9];

	fileLen[0] = 0xAA;
	fileLen[1] = (byte)((9 >> 8) & 0xFF);	// read len_H
	fileLen[2] = (byte)(9 & 0xFF);			// read len_R
	fileLen[3] = 0xB4;

	// File Length
	fileLen[4] = (byte)((m_FilLength >> 24) & 0xFF);
	fileLen[5] = (byte)((m_FilLength >> 16) & 0xFF);
	fileLen[6] = (byte)((m_FilLength >> 8) & 0xFF);
	fileLen[7] = (byte)(m_FilLength & 0xFF);

	fileLen[8] = 0xBB;

	SendComData(fileLen, 9);
}

// 0xC2
void CChatClientDlg::Respond_Send_Data()
{
	std::list<BYTE> list_c2;

	// 1. send file data(1023bytes)
	BYTE* str_c2 = new BYTE[DATA_SEND_SIZE + 5]{};

	str_c2[0] = 0xAA;
	str_c2[1] = (byte)(((DATA_SEND_SIZE + 5) >> 8) & 0xFF);
	str_c2[2] = (byte)((DATA_SEND_SIZE + 5) & 0xFF);
	str_c2[3] = 0xC2;

	int quotient = m_FilLength / DATA_SEND_SIZE;
	int remainder = m_FilLength % DATA_SEND_SIZE;

	if (quotient > m_cnt) {
		for (int i = 0; i < 4; i++) {
			list_c2.push_back(str_c2[i]);
		}

		for (int i = 0; i < DATA_SEND_SIZE; i++) {
			list_c2.push_back(m_WriteByte[i]);
		}

		list_c2.push_back(0xBB);

		int C2_list_size = list_c2.size();

		for (int i = 0; i < C2_list_size; i++) {
			str_c2[i] = list_c2.front();
			list_c2.pop_front();
		}
		m_cnt++;

		SendComData(str_c2, C2_list_size);
		delete[] str_c2;
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

		for (int i = 0; i < C2_list_size; i++) {
			str_c2[i] = list_c2.front();
			list_c2.pop_front();
		}
		SendComData(str_c2, C2_list_size);
		m_cnt++;
		delete[] str_c2;
	}

	else if (quotient < m_cnt) {
		str_c2[0] = 0xAA;
		str_c2[1] = (byte)((5 >> 8) & 0xFF);
		str_c2[2] = (byte)(5 & 0xFF);
		str_c2[3] = 0xD2;
		str_c2[4] = 0xBB;

		SendComData(str_c2, 5);
		delete[] str_c2;
	}

}

void CChatClientDlg::Respond_Close()
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

// List Box에 [Send Data]00 보낼 함수
int CChatClientDlg::SendComData(BYTE* pData, int nLen)
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

	m_socCom.Send(pData, nLen);
	return 0;
}