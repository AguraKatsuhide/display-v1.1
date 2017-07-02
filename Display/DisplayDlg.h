
// DisplayDlg.h : ��� ����
//

#pragma once

#include "KeyPrint.h"
#include "AutomataKR.h"
#include <vector>
#include <list>

#include <opencv\cv.h>
#include <opencv\cvaux.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h>

#include "CvvImage.h"

using namespace std;

typedef struct IDXPOINT
{
	int nIndex;
	CPoint pt;
} IDXPOINT;

// CDisplayDlg ��ȭ ����
class CDisplayDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CDisplayDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DISPLAY_DIALOG };
	void ReadPacket();
	BOOL m_bThread;
	CKeyPrint m_KeyPrint;
	void RecKorean();
	char m_strResult[10];
	void PrintResultKorean();
	void PrintResultOperation();
	CFont m_font;
	void ResetImage();

	HWND m_hForeWnd;

	IplImage *m_pImage;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

	void InitNetwork();
	void DeleteNetwork();
	void InitWindow();
	void ReadINI();

	WSADATA m_wsaData;
	SOCKET m_socket;
	SOCKADDR_IN m_socketInfo;
	short m_sPort;
	BOOL m_bConnected;
	char m_strIP[256];

	HANDLE m_hThread;
	DWORD m_dwThreadID;
	vector<IDXPOINT> m_vecPoints;
	char m_strXML[4];
	CPoint m_ptPrevPoint;
	CPoint m_ptCurPoint;
	BOOL m_bDraw;

	CAutomataKR m_automata;
	void PutText();
	void ShowText();
// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnClickedButtonClear();
};
