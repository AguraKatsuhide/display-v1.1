#pragma once
#include "stdafx.h"
#include <MMSystem.h>


// 사용하지 않음

class CKeyPrint
{
public:
	CKeyPrint(void);
	~CKeyPrint(void);

	void InputKey( CString str );
	char m_cXMLLang;
	int m_nXMLCode;

private:
	BOOL CheckInjection( DWORD dwPID, LPCTSTR szDllName );
	BOOL Inject( DWORD dwPID, LPCTSTR szDllName );
	BOOL Eject( DWORD dwPID, LPCTSTR szDllName );

	void PrintResultKorean();

	void PrintKey();
	void PrintKoreanKey();
	void PrintEnglishSmallKey();
	void PrintEnglishLargeKey();
	void PrintNumberKey();
	void PrintCharacterKey();
	void PrintControlKey();
	void PushKey( char cKey, int nShift );

	void SoundOut( CString str );
	HMODULE m_hDll;
	int m_nMode;
	int m_nKey;
	int m_nInject;
};

