
// DisplayDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Display.h"
#include "DisplayDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDisplayDlg ��ȭ ����

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


// CDisplayDlg �޽��� ó����

BOOL CDisplayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	m_font.CreateFont(85,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,_T("���� ���"));

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
	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_pImage = cvCreateImage( cvSize(402,402), IPL_DEPTH_8U, 3 );
	
	m_bThread = TRUE;
	m_hThread = CreateThread( NULL, 0, ThreadFunc, this, CREATE_SUSPENDED, &m_dwThreadID );
	SetThreadPriority( m_hThread, THREAD_PRIORITY_NORMAL );
	ResumeThread( m_hThread );
	m_hForeWnd = ::GetForegroundWindow();
//	SetTimer( 0, 50, NULL );
	
	
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CDisplayDlg::ResetImage()
{
	// OnPaint�� ���Ǳ� ������ Invalidate �ϸ� �ȵ�.
	memset( m_pImage->imageData, 240, m_pImage->imageSize );
	cvRectangle( m_pImage, cvPoint(0,0), cvPoint(401,401), cvScalar(0,0,0) );
}

void CDisplayDlg::ReadINI()
{
	TCHAR path[512];
	GetCurrentDirectory(512,path);  //������Ʈ ���
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
	// ���� �ʱ�ȭ
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

	// �޴� ���� �����ϴµ� �ʿ���.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CDisplayDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
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

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CDisplayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDisplayDlg::ReadPacket()
{
	// Display�� ���� UDP ��� �޾ƿ��� �κ�
	char pBuf[1024] = {0};

	int nGUISize = sizeof(m_socketInfo);
	int nRecvSize = recvfrom( m_socket, pBuf, 1024, 0, (struct sockaddr*)&m_socketInfo, &nGUISize );
	if( nRecvSize > 0 )
	{
		// �޾ƿ� �κ� ���
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
		// ������ �Ľ�
		while( (nBuf = pBuf[nCurPos]) != NULL )
		{
			char strtemp[3] = {0};
			switch( pBuf[nCurPos++] )
			{
			case 'P' :			// Position (���� ����. ��, 99�� �ְ�� �� �̻��� ��� ��� 99�� ����)
			case 'p' :
				strtemp[0] = pBuf[nCurPos++];
				strtemp[1] = pBuf[nCurPos++];
				nPointPos = atoi( strtemp );
				break;
			case 'X' :			// Position�� X��ǥ
			case 'x' :
				strtemp[0] = pBuf[nCurPos++];
				strtemp[1] = pBuf[nCurPos++];
				nX = atoi( strtemp );
				break;
			case 'Y' :			// Position�� Y��ǥ
			case 'y' :
				strtemp[0] = pBuf[nCurPos++];
				strtemp[1] = pBuf[nCurPos++];
				nY = atoi( strtemp );
				break;
			default :			// �ν� ��� (K01��)
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
					if( nPointPos == 1 )		// Position ��ȣ�� 1�� ������ ���� �׸�
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
	// �ν� ��� �Ľ�
	CString str;
	str.Format( "%s", m_strXML );
	m_KeyPrint.InputKey( str );
	GetDlgItem(IDC_EDIT_INPUT)->SetWindowText( m_strXML );
	// �ѱ�
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
	case 1:		sprintf_s( m_strResult, 10, "��" );		break;
	case 2:		sprintf_s( m_strResult, 10, "��" );		break;
	case 3:		sprintf_s( m_strResult, 10, "��" );		break;
	case 4:		sprintf_s( m_strResult, 10, "��" );		break;
	case 5:		sprintf_s( m_strResult, 10, "��" );		break;
	case 6:		sprintf_s( m_strResult, 10, "��" );		break;
	case 7:		sprintf_s( m_strResult, 10, "��" );		break;
	case 8:		sprintf_s( m_strResult, 10, "��" );		break;
	case 9:		sprintf_s( m_strResult, 10, "��" );		break;
	case 10:	sprintf_s( m_strResult, 10, "��" );		break;
	case 11:	sprintf_s( m_strResult, 10, "��" );		break;
	case 12:	sprintf_s( m_strResult, 10, "��" );		break;
	case 13:	sprintf_s( m_strResult, 10, "��" );		break;
	case 14:	sprintf_s( m_strResult, 10, "��" );		break;
	case 15:	sprintf_s( m_strResult, 10, "��" );		break;
	case 16:	sprintf_s( m_strResult, 10, "��" );		break;
	case 17:	sprintf_s( m_strResult, 10, "��" );		break;
	case 18:	sprintf_s( m_strResult, 10, "��" );		break;
	case 19:	sprintf_s( m_strResult, 10, "��" );		break;
	case 20:	sprintf_s( m_strResult, 10, "��" );		break;
	case 21:	sprintf_s( m_strResult, 10, "��" );		break;
	case 22:	sprintf_s( m_strResult, 10, "��" );		break;
	case 23:	sprintf_s( m_strResult, 10, "��" );		break;
	case 24:	sprintf_s( m_strResult, 10, "��" );		break;
	case 25:	sprintf_s( m_strResult, 10, "��" );		break;
	case 26:	sprintf_s( m_strResult, 10, "��" );		break;
	case 27:	sprintf_s( m_strResult, 10, "��" );		break;
	case 28:	sprintf_s( m_strResult, 10, "��" );		break;
	case 29:	sprintf_s( m_strResult, 10, "��" );		break;
	case 30:	sprintf_s( m_strResult, 10, "��" );		break;
	case 31:	sprintf_s( m_strResult, 10, "��" );		break;
	case 32:	sprintf_s( m_strResult, 10, "��" );		break;
	case 33:	sprintf_s( m_strResult, 10, "��" );		break;
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_vecPoints.clear();
//	KillTimer( 0 );
	m_bThread = FALSE;
	DeleteNetwork();
}


void CDisplayDlg::OnClickedButtonClear()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_automata.Clear();
	ShowText();
	GetDlgItem(IDC_EDIT_INPUT_REC)->SetWindowText("");
}
