#include "stdafx.h"
#include "KeyPrint.h"
#include <TlHelp32.h>
#include <Psapi.h>

#pragma data_seg("SHAREDATA")
static int g_bGetMode = 0;
static DWORD g_nKeyMode = -1;
static DWORD g_nKeySentence = -1;
#pragma data_seg()
#pragma comment( linker, "/SECTION:.SHAREDATA,RWS")

typedef int* (CALLBACK* GetMode)();
typedef DWORD* (CALLBACK* GetKeyMode)();

CKeyPrint::CKeyPrint(void)
{
	TCHAR path[512];
	GetCurrentDirectory(512,path);  //프로젝트 경로
	strcat(path,"\\KeyMode.dll");
	m_hDll = LoadLibrary(path);
}


CKeyPrint::~CKeyPrint(void)
{
	FreeLibrary( m_hDll );
}

BOOL CKeyPrint::Inject( DWORD dwPID, LPCTSTR szDllName )
{
	GetMode GetModePtr = NULL;
	GetKeyMode GetKeyModePtr = NULL;
	
	HANDLE hProcess, hThread, hThread2;
	HMODULE hMod;
	LPVOID pRemoteBuf;
	DWORD dwBufSize = lstrlen(szDllName) + 1;
	LPTHREAD_START_ROUTINE pThreadProc;
	LPTHREAD_START_ROUTINE pThreadProc2;

	if( !(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)) )
		return FALSE;

	pRemoteBuf = VirtualAllocEx( hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE );
	BOOL bWrite = WriteProcessMemory( hProcess, pRemoteBuf, (LPVOID)szDllName, dwBufSize, NULL );
	hMod = GetModuleHandle( "kernel32.dll" );
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress( hMod, "LoadLibraryA" );
	hThread = CreateRemoteThread( hProcess, NULL, 0, pThreadProc, pRemoteBuf, 0, NULL );
	WaitForSingleObject( hThread, INFINITE );
	Sleep(10);

	GetModePtr = (GetMode)GetProcAddress( m_hDll, "GetMode" );
	GetKeyModePtr = (GetKeyMode)GetProcAddress( m_hDll, "GetKeyMode" );

	m_nMode = *GetModePtr();
	m_nKey = *GetKeyModePtr();

	CloseHandle( hThread );
	CloseHandle( hProcess );
	
	return TRUE;
}

BOOL CKeyPrint::Eject( DWORD dwPID, LPCTSTR szDllName )
{
	BOOL bMore = FALSE, bFound = FALSE;
	HANDLE hSnapshot, hProcess, hThread;
	HMODULE hModule = NULL;
	MODULEENTRY32 me = { sizeof(me) };
	LPTHREAD_START_ROUTINE pThreadProc;

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);

	bMore = Module32First(hSnapshot, &me);
	while( bMore )
    {
		if( !_stricmp((LPCTSTR)me.szModule, szDllName) )
		{
			bFound = TRUE;
			break;
		}
		bMore = Module32Next(hSnapshot, &me);
	}

	if( !bFound )
	{
		CloseHandle(hSnapshot);
		return FALSE;
	}

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	hModule = GetModuleHandle("kernel32.dll");
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "FreeLibrary");
	hThread = CreateRemoteThread(hProcess, NULL, 0, 
								 pThreadProc, me.modBaseAddr, 
								 0, NULL);
	WaitForSingleObject(hThread, INFINITE); 

	CloseHandle(hThread);
	CloseHandle(hProcess);
	CloseHandle(hSnapshot);

	return TRUE;
}

BOOL CKeyPrint::CheckInjection( DWORD dwPID, LPCTSTR szDllName )
{
	m_nMode = 0;
	HMODULE hMods[1024];
	DWORD cbNeeded;
	TCHAR szModName[MAX_PATH];
	HANDLE hProcess;
	
	if( !(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)) )
		return FALSE;

	if( EnumProcessModules( hProcess, hMods, sizeof(hMods), &cbNeeded ) )
	{
		for( int k = 0; k < (cbNeeded / sizeof(HMODULE)); k++ )
		{
			if( GetModuleFileNameEx( hProcess, hMods[k], szModName, sizeof(szModName)/sizeof(TCHAR)))
			{
				if( strstr( szModName, szDllName ) )
				{
					CloseHandle( hProcess );
					return TRUE;
				}
			}
		}
	}
	CloseHandle( hProcess );
	return FALSE;
}

void CKeyPrint::InputKey( CString wcs )
{
	HWND hWnd = GetForegroundWindow();
	DWORD dwPID = 0;
	GetWindowThreadProcessId( hWnd, &dwPID );

	TCHAR path[512];
	GetCurrentDirectory(512,path);  //프로젝트 경로
	strcat(path,"\\KeyMode.dll");

	SoundOut(wcs);

	m_cXMLLang = wcs.GetAt(0);
	m_nXMLCode = atoi(wcs.Right(2).GetBuffer());
/*
	if( CheckInjection( dwPID, path ) )
	{
		if( GetCurrentProcessId() == dwPID )
			return;
		Eject( dwPID, "KeyMode.dll" );
		CheckInjection( dwPID, path );
	}

	m_nInject = Inject( dwPID, path );
	if( !m_nInject )
		return;
	
	if( m_nKey == 0 )
	{
		m_nKey = 1;
		keybd_event( VK_HANGUL, MapVirtualKey(VK_HANGUL, 0), 0, 0 );
		keybd_event( VK_HANGUL, MapVirtualKey(VK_HANGUL, 0), KEYEVENTF_KEYUP, 0 );
	}

	PrintKey();
	
	Eject( dwPID, "KeyMode.dll" );
*/
}

void CKeyPrint::SoundOut( CString str )
{
	TCHAR path[512];
	GetCurrentDirectory( 512, path );

	strcat( path, "\\Sound\\" );
	strcat( path, str.GetBuffer() );
	strcat( path, ".wav" );
	PlaySound( NULL, NULL, NULL );
	PlaySound( path, NULL, SND_ASYNC );
}
void CKeyPrint::PrintKey()
{
	switch( m_cXMLLang )
	{
	case 'k' :
	case 'K' :
		PrintKoreanKey();
		break;
	case 'o' :
	case 'O' :
		PrintControlKey();
		break;
	case 'l' :
	case 'L' :
		PrintEnglishLargeKey();
		break;
	case 's' :
	case 'S' :
		PrintEnglishSmallKey();
		break;
	case 'n' :
	case 'N' :
		PrintNumberKey();
		break;
	case 'c':
	case 'C':
		PrintCharacterKey();
		break;
	}
}

void CKeyPrint::PrintNumberKey()
{
	switch( m_nXMLCode )
	{
	case 1:		PushKey( '1', 0 );		break;
	case 2:		PushKey( '2', 0 );		break;
	case 3:		PushKey( '3', 0 );		break;
	case 4:		PushKey( '4', 0 );		break;
	case 5:		PushKey( '5', 0 );		break;
	case 6:		PushKey( '6', 0 );		break;
	case 7:		PushKey( '7', 0 );		break;
	case 8:		PushKey( '8', 0 );		break;
	case 9:		PushKey( '9', 0 );		break;
	case 10:	PushKey( '0', 0 );		break;
	}
}

void CKeyPrint::PrintCharacterKey()
{
	switch( m_nXMLCode )
	{
	case 1:		PushKey( '`', 1 );		break;
	case 2:		PushKey( '`', 1 );		break;
	case 3:		PushKey( '1', 1 );		break;
	case 4:		PushKey( '2', 1 );		break;
	case 5:		PushKey( '3', 1 );		break;
	case 6:		PushKey( '4', 1 );		break;
	case 7:		PushKey( '5', 1 );		break;
	case 8:		PushKey( '6', 1 );		break;
	case 9:		PushKey( '7', 1 );		break;
	case 10:	PushKey( '8', 1 );		break;
	case 11:	PushKey( '9', 1 );		break;
	case 12:	PushKey( '0', 1 );		break;
	case 13:	PushKey( '-', 0 );		break;
	case 14:	PushKey( '=', 1 );		break;
	case 15:	PushKey( '-', 1 );		break;
	case 16:	PushKey( '=', 0 );		break;
	case 17:	PushKey( '\\', 0 );		break;
	case 18:	PushKey( '\\', 1 );		break;
	case 19:	PushKey( '[', 1 );		break;
	case 20:	PushKey( '[', 0 );		break;
	case 21:	PushKey( ']', 1 );		break;
	case 22:	PushKey( ']', 0 );		break;
	case 23:	PushKey( ';', 0 );		break;
	case 24:	PushKey( ';', 1 );		break;
	case 25:	PushKey( '\'', 0 );		break;
	case 26:	PushKey( '\'', 1 );		break;
	case 27:	PushKey( ',', 1 );		break;
	case 28:	PushKey( ',', 0 );		break;
	case 29:	PushKey( '.', 1 );		break;
	case 30:	PushKey( '.', 0 );		break;
	case 31:	PushKey( '/', 1 );		break;
	case 32:	PushKey( '/', 0 );		break;
	}
}

void CKeyPrint::PrintEnglishSmallKey()
{
	switch( m_nXMLCode )
	{
	case 1:		PushKey( 'A', 0 );		break;
	case 2:		PushKey( 'B', 0 );		break;
	case 3:		PushKey( 'C', 0 );		break;
	case 4:		PushKey( 'D', 0 );		break;
	case 5:		PushKey( 'E', 0 );		break;
	case 6:		PushKey( 'F', 0 );		break;
	case 7:		PushKey( 'G', 0 );		break;
	case 8:		PushKey( 'H', 0 );		break;
	case 9:		PushKey( 'I', 0 );		break;
	case 10:	PushKey( 'J', 0 );		break;
	case 11:	PushKey( 'K', 0 );		break;
	case 12:	PushKey( 'L', 0 );		break;
	case 13:	PushKey( 'M', 0 );		break;
	case 14:	PushKey( 'N', 0 );		break;
	case 15:	PushKey( 'O', 0 );		break;
	case 16:	PushKey( 'P', 0 );		break;
	case 17:	PushKey( 'Q', 0 );		break;
	case 18:	PushKey( 'R', 0 );		break;
	case 19:	PushKey( 'S', 0 );		break;
	case 20:	PushKey( 'T', 0 );		break;
	case 21:	PushKey( 'U', 0 );		break;
	case 22:	PushKey( 'V', 0 );		break;
	case 23:	PushKey( 'W', 0 );		break;
	case 24:	PushKey( 'X', 0 );		break;
	case 25:	PushKey( 'Y', 0 );		break;
	case 26:	PushKey( 'Z', 0 );		break;
	}
}

void CKeyPrint::PrintEnglishLargeKey()
{
	switch( m_nXMLCode )
	{
	case 1:		PushKey( 'A', 1 );		break;
	case 2:		PushKey( 'B', 1 );		break;
	case 3:		PushKey( 'C', 1 );		break;
	case 4:		PushKey( 'D', 1 );		break;
	case 5:		PushKey( 'E', 1 );		break;
	case 6:		PushKey( 'F', 1 );		break;
	case 7:		PushKey( 'G', 1 );		break;
	case 8:		PushKey( 'H', 1 );		break;
	case 9:		PushKey( 'I', 1 );		break;
	case 10:	PushKey( 'J', 1 );		break;
	case 11:	PushKey( 'K', 1 );		break;
	case 12:	PushKey( 'L', 1 );		break;
	case 13:	PushKey( 'M', 1 );		break;
	case 14:	PushKey( 'N', 1 );		break;
	case 15:	PushKey( 'O', 1 );		break;
	case 16:	PushKey( 'P', 1 );		break;
	case 17:	PushKey( 'Q', 1 );		break;
	case 18:	PushKey( 'R', 1 );		break;
	case 19:	PushKey( 'S', 1 );		break;
	case 20:	PushKey( 'T', 1 );		break;
	case 21:	PushKey( 'U', 1 );		break;
	case 22:	PushKey( 'V', 1 );		break;
	case 23:	PushKey( 'W', 1 );		break;
	case 24:	PushKey( 'X', 1 );		break;
	case 25:	PushKey( 'Y', 1 );		break;
	case 26:	PushKey( 'Z', 1 );		break;
	}
}

void CKeyPrint::PushKey( char cKey, int nShift )
{
	if( nShift )
		keybd_event( VK_SHIFT, MapVirtualKey(VK_SHIFT, 0), 0, 0 );
	keybd_event( cKey, 0, 0, 0 );
	keybd_event( cKey, 0, KEYEVENTF_KEYUP, 0 );
	if( nShift )
		keybd_event( VK_SHIFT, MapVirtualKey(VK_SHIFT, 0), KEYEVENTF_KEYUP, 0 );
}

void CKeyPrint::PrintControlKey()
{
	switch( m_nXMLCode )
	{
	case 1:		PushKey( VK_SPACE, 0 );	break;
	case 2:		PushKey( VK_BACK, 0 );	break;
	case 3:		PushKey( VK_RETURN, 0 );break;
	}
}

void CKeyPrint::PrintKoreanKey()
{
	switch( m_nXMLCode )
	{
	case 1:		PushKey( 'R', 0 );		break;
	case 2:		PushKey( 'R', 1 );		break;
	case 3:		PushKey( 'S', 0 );		break;
	case 4:		PushKey( 'E', 0 );		break;
	case 5:		PushKey( 'E', 1 );		break;
	case 6:		PushKey( 'F', 0 );		break;
	case 7:		PushKey( 'A', 0 );		break;
	case 8:		PushKey( 'Q', 0 );		break;
	case 9:		PushKey( 'Q', 1 );		break;
	case 10:	PushKey( 'T', 0 );		break;
	case 11:	PushKey( 'T', 1 );		break;
	case 12:	PushKey( 'D', 0 );		break;
	case 13:	PushKey( 'W', 0 );		break;
	case 14:	PushKey( 'W', 1 );		break;
	case 15:	PushKey( 'C', 0 );		break;
	case 16:	PushKey( 'Z', 0 );		break;
	case 17:	PushKey( 'X', 0 );		break;
	case 18:	PushKey( 'V', 0 );		break;
	case 19:	PushKey( 'G', 0 );		break;
	case 20:	PushKey( 'K', 0 );		break;
	case 21:	PushKey( 'O', 0 );		break;
	case 22:	PushKey( 'I', 0 );		break;
	case 23:	PushKey( 'O', 1 );		break;
	case 24:	PushKey( 'J', 0 );		break;
	case 25:	PushKey( 'P', 0 );		break;
	case 26:	PushKey( 'U', 0 );		break;
	case 27:	PushKey( 'P', 1 );		break;
	case 28:	PushKey( 'H', 0 );		break;
	case 29:	PushKey( 'Y', 0 );		break;
	case 30:	PushKey( 'N', 0 );		break;
	case 31:	PushKey( 'B', 0 );		break;
	case 32:	PushKey( 'M', 0 );		break;
	case 33:	PushKey( 'L', 0 );		break;
	}
}