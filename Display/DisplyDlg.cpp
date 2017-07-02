
// DisplayDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Display.h"
#include "DisplayDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDisplayDlg 대화 상자

DWORD WINAPI ThreadFunc( LPVOID lParam )
{
	CDisplayDlg *pDlg = (CDisplayDlg*)lParam;
	while( pDlg->m_bThread )
	{
		pDlg->ReadPacket();
	}
	return 0;
}

CDisplayDlg::CDisplayDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDisplayDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDisplayDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDisplayDlg::OnClickedButtonClear)
END_MESSAGE_MAP()


// CDisplayDlg 메시지 처리기

BOOL CDisplayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	m_font.CreateFont(85,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,_T("맑은 고딕"));

	GetDlgItem(IDC_EDIT_AUTOMATA)->SetFont( &m_font );
	GetDlgItem(IDC_EDIT_INPUT)->SetFont( &m_font );
	GetDlgItem(IDC_EDIT_INPUT_REC)->SetFont( &m_font );
	GetDlgItem(IDC_STATIC_INPUT_CHAR)->SetFont( &m_font );

//	SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOSIZE ); 

	memset( m_strIP, 0, 256 );
	InitWindow();
	InitNetwork();
	memset( m_strXML, 0, 4 );
	memset( m_strResult, 0, 10 );
	m_ptPrevPoint.x = -1;
	m_ptPrevPoint.y = -1;
	m_ptCurPoint.x = -1;
	m_ptCurPoint.y = -1;
	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_pImage = cvCreateImage( cvSize(402,402), IPL_DEPTH_8U, 3 );
	
	m_bThread = TRUE;
	m_hThread = CreateThread( NULL, 0, ThreadFunc, this, CREATE_SUSPENDED, &m_dwThreadID );
	SetThreadPriority( m_hThread, THREAD_PRIORITY_NORMAL );
	ResumeThread( m_hThread );
	m_hForeWnd = ::GetForegroundWindow();
//	SetTimer( 0, 50, NULL );
	
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDisplayDlg::ResetImage()
{
	// OnPaint에 사용되기 때문에 Invalidate 하면 안됨.
	memset( m_pImage->imageData, 240, m_pImage->imageSize );
	cvRectangle( m_pImage, cvPoint(0,0), cvPoint(401,401), cvScalar(0,0,0) );
}

void CDisplayDlg::ReadINI()
{
	TCHAR path[512];
	GetCurrentDirectory(512,path);  //프로젝트 경로
	strcat_s(path,512,"\\program.ini");
	m_sPort = GetPrivateProfileInt(TEXT("IPSetting"),TEXT("Port"),-1,path);
//	GetPrivateProfileString( TEXT("IPSetting"),TEXT("IP"), TEXT("127.0.0.1"), m_strIP, 256, path );
}

void CDisplayDlg::InitWindow()
{
	MoveWindow( 0, 0, 647, 920 );
	GetDlgItem(IDC_EDIT_AUTOMATA)->MoveWindow( 10, 10, 620, 300 );
//	GetDlgItem(IDC_EDIT_INPUT)->MoveWindow( 90, 180, 80, 30 );
	GetDlgItem(IDC_EDIT_INPUT_REC)->MoveWindow( 170, 770, 300, 110 );
	GetDlgItem(IDC_BUTTON_CLEAR)->MoveWindow( 60, 800, 80, 50 );
	SetWindowTextA( "Display" );
//	GetDlgItem(IDC_STATIC_INPUT_CHAR)->MoveWindow( 10, 185, 70, 25 );
}

void CDisplayDlg::InitNetwork()
{
	// 소켓 초기화
	ReadINI();
	m_bConnected = TRUE;
	if( WSAStartup(0x202, &m_wsaData) == SOCKET_ERROR )
	{
		WSACleanup();
		m_bConnected = FALSE;
		return;
	}
	memset( &m_socketInfo, 0, sizeof(m_socketInfo) );
	m_socket = socket( AF_INET, SOCK_DGRAM, 0 );
	if( m_socket == INVALID_SOCKET )
	{
		WSACleanup();
		m_bConnected = FALSE;
		return;
	}

	memset( &m_socketInfo, 0, sizeof(m_socketInfo) );
	m_socketInfo.sin_family = AF_INET;
	m_socketInfo.sin_port = htons(m_sPort);
	m_socketInfo.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// 받는 소켓 생성하는데 필요함.
	if( ::bind( m_socket, (struct sockaddr*)&m_socketInfo, sizeof(m_socketInfo) ) == SOCKET_ERROR )
	{
		closesocket( m_socket );
		WSACleanup();
		m_bConnected = FALSE;
		return;
	}
}

void CDisplayDlg::DeleteNetwork()
{
	if( m_bConnected )
	{
		closesocket( m_socket );
		WSACleanup();
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDisplayDlg::OnPaint()
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
		CPaintDC dc(this);
		ResetImage();

		if( m_vecPoints.size() > 1 )
		{
			for( int i = 0; i < m_vecPoints.size()-1; i++ )
			{
				int x1 = m_vecPoints[i].pt.x * 4;
				int y1 = m_vecPoints[i].pt.y * 4;
				int x2 = m_vecPoints[i+1].pt.x * 4;
				int y2 = m_vecPoints[i+1].pt.y * 4;
				cvCircle( m_pImage, cvPoint(x1,y1), 5, cvScalar(0,0,0), -1 );
				cvCircle( m_pImage, cvPoint(x2,y2), 5, cvScalar(0,0,0), -1 );
				cvLine( m_pImage, cvPoint(x1,y1), cvPoint(x2,y2), cvScalar(0,0,0), 2 );
			}
		}
		if( m_ptCurPoint.x >= 0 )
		{
			cvCircle( m_pImage, cvPoint(m_ptCurPoint.x*4, m_ptCurPoint.y*4), 5, cvScalar(255, 128, 0), -1 );
		}
		RECT rt;
		rt.top = 320;
		rt.bottom = 720;
		rt.left = 120;
		rt.right = 520;
		CvvImage image;
		image.CopyOf( m_pImage );
		image.DrawToHDC( dc.m_hDC, &rt );
		image.Destroy();
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDisplayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDisplayDlg::ReadPacket()
{
	// Display로 부터 UDP 통신 받아오는 부분
	char pBuf[1024] = {0};

	int nGUISize = sizeof(m_socketInfo);
	int nRecvSize = recvfrom( m_socket, pBuf, 1024, 0, (struct sockaddr*)&m_socketInfo, &nGUISize );
	if( nRecvSize > 0 )
	{
		// 받아온 부분 출력
		printf( "UDP RECV \"%s\" ", pBuf );
		printf( "FROM %d.%d.%d.%d(%d)\r\n", 
			m_socketInfo.sin_addr.S_un.S_un_b.s_b1, m_socketInfo.sin_addr.S_un.S_un_b.s_b2, 
			m_socketInfo.sin_addr.S_un.S_un_b.s_b3, m_socketInfo.sin_addr.S_un.S_un_b.s_b4,
			m_sPort );
		int nPointPos = -1;
		int nX = -1;
		int nY = -1;
		char nBuf;
		int nCurPos = 0;
		BOOL bRec = FALSE;
		// 데이터 파싱
		while( (nBuf = pBuf[nCurPos]) != NULL )
		{
			char strtemp[3] = {0};
			switch( pBuf[nCurPos++] )
			{
			case 'P' :			// Position (점의 순서. 단, 99가 최고며 그 이상일 경우 계속 99로 들어옴)
			case 'p' :
				strtemp[0] = pBuf[nCurPos++];
				strtemp[1] = pBuf[nCurPos++];
				nPointPos = atoi( strtemp );
				break;
			case 'X' :			// Position의 X좌표
			case 'x' :
				strtemp[0] = pBuf[nCurPos++];
				strtemp[1] = pBuf[nCurPos++];
				nX = atoi( strtemp );
				break;
			case 'Y' :			// Position의 Y좌표
			case 'y' :
				strtemp[0] = pBuf[nCurPos++];
				strtemp[1] = pBuf[nCurPos++];
				nY = atoi( strtemp );
				break;
			default :			// 인식 결과 (K01등)
				memset( m_strXML, 0, 4 );
				m_strXML[0] = nBuf;
				m_strXML[1] = pBuf[nCurPos++];
				m_strXML[2] = pBuf[nCurPos++];
				bRec = TRUE;
				break;
			}
			if( nPointPos != -1 && nX != -1 && nY != -1 )
			{
				IDXPOINT pt;
				pt.pt.x = nX;
				pt.pt.y = nY;
				pt.nIndex = nPointPos;
				if( nPointPos > 0 )
				{
					m_ptCurPoint.x = -1;
					m_ptCurPoint.y = -1;
					m_ptPrevPoint.x = -1;
					m_ptPrevPoint.y = -1;
					if( nPointPos == 1 )		// Position 번호가 1이 들어오면 새로 그림
					{
	//					memset( m_strXML, 0, 4 );
						m_vecPoints.clear();
						Invalidate(FALSE);
					}
					m_vecPoints.push_back( pt );
				}
				else
				{
					m_ptCurPoint.x = nX;
					m_ptCurPoint.y = nY;
				}

				nPointPos = -1;
				nX = -1;
				nY = -1;
				Invalidate(FALSE);
			}
		}
		if( bRec )		RecKorean();
	}
}

void CDisplayDlg::RecKorean()
{
	// 인식 결과 파싱
	CString str;
	str.Format( "%s", m_strXML );
	m_KeyPrint.InputKey( str );
	GetDlgItem(IDC_EDIT_INPUT)->SetWindowText( m_strXML );
	// 한글
	if( m_KeyPrint.m_cXMLLang == 'K' ||
		m_KeyPrint.m_cXMLLang == 'k' )
	{
		if( m_KeyPrint.m_nXMLCode == 0 )
		{
			memset( m_strResult, 0, 10 );
		}
		else
		{
			PrintResultKorean();
			PutText();
		}
	}
	// Control
	if( m_KeyPrint.m_cXMLLang == 'o' ||
		m_KeyPrint.m_cXMLLang == 'O' )
	{
		PrintResultOperation();
		switch( m_KeyPrint.m_nXMLCode )
		{
		case 1:
//			if( m_automata.ingWord != NULL )
//				m_automata.SetKeyCode( KEY_CODE_SPACE );
			m_automata.SetKeyCode( KEY_CODE_SPACE );
			break;
		case 2:
			m_automata.SetKeyCode( KEY_CODE_BACKSPACE );
			break;
		case 3:
			m_automata.SetKeyCode( KEY_CODE_ENTER );
			break;
		case 5:
			m_automata.SetKeyCode( KEY_CODE_BACKSPACE );
			break;
		}
		ShowText();
	}

	GetDlgItem(IDC_EDIT_INPUT_REC)->SetWindowTextA( m_strResult );
}

void CDisplayDlg::PrintResultOperation()
{
	memset( m_strResult, 0, 10 );
	switch( m_KeyPrint.m_nXMLCode )
	{
	case 1:		sprintf_s( m_strResult, 10, "SPACE" );		break;
	case 2:		sprintf_s( m_strResult, 10, "BACK" );		break;
	case 3:		sprintf_s( m_strResult, 10, "ENTER" );		break;
	}
}

void CDisplayDlg::PrintResultKorean()
{
	memset( m_strResult, 0, 10 );
	switch( m_KeyPrint.m_nXMLCode )
	{
	case 1:		sprintf_s( m_strResult, 10, "ㄱ" );		break;
	case 2:		sprintf_s( m_strResult, 10, "ㄲ" );		break;
	case 3:		sprintf_s( m_strResult, 10, "ㄴ" );		break;
	case 4:		sprintf_s( m_strResult, 10, "ㄷ" );		break;
	case 5:		sprintf_s( m_strResult, 10, "ㄸ" );		break;
	case 6:		sprintf_s( m_strResult, 10, "ㄹ" );		break;
	case 7:		sprintf_s( m_strResult, 10, "ㅁ" );		break;
	case 8:		sprintf_s( m_strResult, 10, "ㅂ" );		break;
	case 9:		sprintf_s( m_strResult, 10, "ㅃ" );		break;
	case 10:	sprintf_s( m_strResult, 10, "ㅅ" );		break;
	case 11:	sprintf_s( m_strResult, 10, "ㅆ" );		break;
	case 12:	sprintf_s( m_strResult, 10, "ㅇ" );		break;
	case 13:	sprintf_s( m_strResult, 10, "ㅈ" );		break;
	case 14:	sprintf_s( m_strResult, 10, "ㅉ" );		break;
	case 15:	sprintf_s( m_strResult, 10, "ㅊ" );		break;
	case 16:	sprintf_s( m_strResult, 10, "ㅋ" );		break;
	case 17:	sprintf_s( m_strResult, 10, "ㅌ" );		break;
	case 18:	sprintf_s( m_strResult, 10, "ㅍ" );		break;
	case 19:	sprintf_s( m_strResult, 10, "ㅎ" );		break;
	case 20:	sprintf_s( m_strResult, 10, "ㅏ" );		break;
	case 21:	sprintf_s( m_strResult, 10, "ㅐ" );		break;
	case 22:	sprintf_s( m_strResult, 10, "ㅑ" );		break;
	case 23:	sprintf_s( m_strResult, 10, "ㅒ" );		break;
	case 24:	sprintf_s( m_strResult, 10, "ㅓ" );		break;
	case 25:	sprintf_s( m_strResult, 10, "ㅔ" );		break;
	case 26:	sprintf_s( m_strResult, 10, "ㅕ" );		break;
	case 27:	sprintf_s( m_strResult, 10, "ㅖ" );		break;
	case 28:	sprintf_s( m_strResult, 10, "ㅗ" );		break;
	case 29:	sprintf_s( m_strResult, 10, "ㅛ" );		break;
	case 30:	sprintf_s( m_strResult, 10, "ㅜ" );		break;
	case 31:	sprintf_s( m_strResult, 10, "ㅠ" );		break;
	case 32:	sprintf_s( m_strResult, 10, "ㅡ" );		break;
	case 33:	sprintf_s( m_strResult, 10, "ㅣ" );		break;
	}
}

void CDisplayDlg::PutText()
{
	wchar_t wcUnicode = 0;
	WCHAR pText[MAX_PATH] = {0};
	int nLen = MultiByteToWideChar( CP_ACP, 0, m_strResult, -1, NULL, NULL );
	MultiByteToWideChar( CP_ACP, 0, m_strResult, -1, pText, nLen );
	wcUnicode = pText[0];
	int nKey = m_automata.GetKeyCode( wcUnicode );
	m_automata.SetKeyCode( nKey );
	ShowText();
}

void CDisplayDlg::ShowText()
{
	CString str = (CString)m_automata.completeText;
	str += (CString)m_automata.ingWord;
	GetDlgItem(IDC_EDIT_AUTOMATA)->SetWindowText( str );
	((CEdit*)(GetDlgItem(IDC_EDIT_AUTOMATA)))->SetSel( str.GetLength(), str.GetLength() );
	int nLineCount = ((CEdit*)(GetDlgItem(IDC_EDIT_AUTOMATA)))->GetLineCount();
	RECT rt;
	((CEdit*)(GetDlgItem(IDC_EDIT_AUTOMATA)))->GetWindowRect(&rt);
	int nMaxLine = (rt.bottom - rt.top) / 85;
	if( nLineCount > nMaxLine )
	{
		m_automata.m_completeWord = NULL;
		m_automata.completeText = L"";
		ShowText();
	}
}

void CDisplayDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//*
	HWND hWnd = ::GetForegroundWindow();
	if( hWnd != m_hForeWnd )
	{
		m_hForeWnd = hWnd;
		m_automata.Clear();
		ShowText();
		memset( m_strResult, 0, 10 );
		memset( m_strXML, 0, 4 );
		GetDlgItem(IDC_EDIT_INPUT_REC)->SetWindowText( m_strResult );
		GetDlgItem(IDC_EDIT_INPUT)->SetWindowText( m_strXML );
		m_vecPoints.clear();
//		Invalidate( TRUE );
	}
//*/

	CDialogEx::OnTimer(nIDEvent);
}


void CDisplayDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_vecPoints.clear();
//	KillTimer( 0 );
	m_bThread = FALSE;
	DeleteNetwork();
}


void CDisplayDlg::OnClickedButtonClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_automata.Clear();
	ShowText();
	GetDlgItem(IDC_EDIT_INPUT_REC)->SetWindowText("");
}
