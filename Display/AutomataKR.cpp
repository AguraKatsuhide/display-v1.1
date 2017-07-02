
#include "stdafx.h"
#include "AutomataKR.h"


CAutomataKR::CAutomataKR(void)
{
	Clear();
	BASE_CODE = 0xac00;		// 기초음성(가)
	LIMIT_MIN = 0xac00;		// 음성범위 MIN(가)
	LIMIT_MAX = 0xd7a3;		// 음성범위 MAX

	memset( MID_SUB, 0, sizeof(int) * 21 );

	//	/* 초성 19자 0 ~ 18 */
	SOUND_TABLE[0] = L'ㄱ';			SOUND_TABLE[1] = L'ㄲ';			SOUND_TABLE[2] = L'ㄴ';			SOUND_TABLE[3] = L'ㄷ';
	SOUND_TABLE[4] = L'ㄸ';			SOUND_TABLE[5] = L'ㄹ';			SOUND_TABLE[6] = L'ㅁ';			SOUND_TABLE[7] = L'ㅂ';
	SOUND_TABLE[8] = L'ㅃ';			SOUND_TABLE[9] = L'ㅅ';			SOUND_TABLE[10] = L'ㅆ';		SOUND_TABLE[11] = L'ㅇ';
	SOUND_TABLE[12] = L'ㅈ';		SOUND_TABLE[13] = L'ㅉ';		SOUND_TABLE[14] = L'ㅊ';		SOUND_TABLE[15] = L'ㅋ';
	SOUND_TABLE[16] = L'ㅌ';		SOUND_TABLE[17] = L'ㅍ';		SOUND_TABLE[18] = L'ㅎ';

	//	/* 중성 21자 19 ~ 39 */
	SOUND_TABLE[19] = L'ㅏ';		SOUND_TABLE[20] = L'ㅐ';		SOUND_TABLE[21] = L'ㅑ';		SOUND_TABLE[22] = L'ㅒ';
	SOUND_TABLE[23] = L'ㅓ';		SOUND_TABLE[24] = L'ㅔ';		SOUND_TABLE[25] = L'ㅕ';		SOUND_TABLE[26] = L'ㅖ';
	SOUND_TABLE[27] = L'ㅗ';		SOUND_TABLE[28] = L'ㅘ';		SOUND_TABLE[29] = L'ㅙ';		SOUND_TABLE[30] = L'ㅚ';
	SOUND_TABLE[31] = L'ㅛ';		SOUND_TABLE[32] = L'ㅜ';		SOUND_TABLE[33] = L'ㅝ';		SOUND_TABLE[34] = L'ㅞ';
	SOUND_TABLE[35] = L'ㅟ';		SOUND_TABLE[36] = L'ㅠ';		SOUND_TABLE[37] = L'ㅡ';		SOUND_TABLE[38] = L'ㅢ';
	SOUND_TABLE[39] = L'ㅣ';

	//	/* 종성 28자 40 ~ 67 */
	SOUND_TABLE[40] = L' ';			SOUND_TABLE[41] = L'ㄱ';		SOUND_TABLE[42] = L'ㄲ';		SOUND_TABLE[43] = L'ㄳ';
	SOUND_TABLE[44] = L'ㄴ';		SOUND_TABLE[45] = L'ㄵ';		SOUND_TABLE[46] = L'ㄶ';		SOUND_TABLE[47] = L'ㄷ';
	SOUND_TABLE[48] = L'ㄹ';		SOUND_TABLE[49] = L'ㄺ';		SOUND_TABLE[50] = L'ㄻ';		SOUND_TABLE[51] = L'ㄼ';
	SOUND_TABLE[52] = L'ㄽ';		SOUND_TABLE[53] = L'ㄾ';		SOUND_TABLE[54] = L'ㄿ';		SOUND_TABLE[55] = L'ㅀ';
	SOUND_TABLE[56] = L'ㅁ';		SOUND_TABLE[57] = L'ㅂ';		SOUND_TABLE[58] = L'ㅄ';		SOUND_TABLE[59] = L'ㅅ';
	SOUND_TABLE[60] = L'ㅆ';		SOUND_TABLE[61] = L'ㅇ';		SOUND_TABLE[62] = L'ㅈ';		SOUND_TABLE[63] = L'ㅊ';
	SOUND_TABLE[64] = L'ㅋ';		SOUND_TABLE[65] = L'ㅌ';		SOUND_TABLE[66] = L'ㅍ';		SOUND_TABLE[67] = L'ㅎ';


	// 초성 합성 테이블
	MIXED_CHO_CONSON[0][0] = 0;		MIXED_CHO_CONSON[0][1] = 0;		MIXED_CHO_CONSON[0][2] = 1;
	MIXED_CHO_CONSON[3][0] = 3;		MIXED_CHO_CONSON[3][1] = 3;		MIXED_CHO_CONSON[3][2] = 4;
	MIXED_CHO_CONSON[6][0] = 7;		MIXED_CHO_CONSON[6][1] = 7;		MIXED_CHO_CONSON[6][2] = 8;
	MIXED_CHO_CONSON[9][0] = 9;		MIXED_CHO_CONSON[9][1] = 9;		MIXED_CHO_CONSON[9][2] = 10;
	MIXED_CHO_CONSON[11][0] = 12;	MIXED_CHO_CONSON[11][1] = 12;	MIXED_CHO_CONSON[11][2] = 13;

	// 중성 합성 테이블
	MIXED_JONG_CONSON[0][0] = 41;	MIXED_JONG_CONSON[0][1] = 41;	MIXED_JONG_CONSON[0][2] = 42;
	MIXED_JONG_CONSON[18][0] = 59;	MIXED_JONG_CONSON[18][1] = 59;	MIXED_JONG_CONSON[18][2] = 60;

	//// 초성,중성 모음 합성 테이블
	MIXED_VOWEL[2][0] = 19; 			MIXED_VOWEL[2][1] = 39;				MIXED_VOWEL[2][2] = 20;
	MIXED_VOWEL[3][0] = 21; 			MIXED_VOWEL[3][1] = 39;				MIXED_VOWEL[3][2] = 22;
	MIXED_VOWEL[6][0] = 23; 			MIXED_VOWEL[6][1] = 39;				MIXED_VOWEL[6][2] = 24;
	MIXED_VOWEL[7][0] = 25; 			MIXED_VOWEL[7][1] = 39;				MIXED_VOWEL[7][2] = 26;
	MIXED_VOWEL[10][0] = 27; 			MIXED_VOWEL[10][1] = 19;			MIXED_VOWEL[10][2] = 28;
	MIXED_VOWEL[11][0] = 27; 			MIXED_VOWEL[11][1] = 20;			MIXED_VOWEL[11][2] = 29;
	MIXED_VOWEL[12][0] = 27; 			MIXED_VOWEL[12][1] = 39;			MIXED_VOWEL[12][2] = 30;
	MIXED_VOWEL[13][0] = 33; 			MIXED_VOWEL[13][1] = 39;			MIXED_VOWEL[13][2] = 34;
	MIXED_VOWEL[14][0] = 28; 			MIXED_VOWEL[14][1] = 39;			MIXED_VOWEL[14][2] = 29;
	MIXED_VOWEL[15][0] = 32; 			MIXED_VOWEL[15][1] = 23;			MIXED_VOWEL[15][2] = 33;
	MIXED_VOWEL[16][0] = 32; 			MIXED_VOWEL[16][1] = 24;			MIXED_VOWEL[16][2] = 34;
	MIXED_VOWEL[17][0] = 32; 			MIXED_VOWEL[17][1] = 39;			MIXED_VOWEL[17][2] = 35;
	MIXED_VOWEL[19][0] = 37; 			MIXED_VOWEL[19][1] = 39;			MIXED_VOWEL[19][2] = 38;

	//// 종성 합성 테이블
	MIXED_JONG_CONSON[3][0] =  41;			MIXED_JONG_CONSON[3][1] =  59;			MIXED_JONG_CONSON[3][2] =  43;
	MIXED_JONG_CONSON[4][0] = 44;			MIXED_JONG_CONSON[4][1] = 62;			MIXED_JONG_CONSON[4][2] = 45;
	MIXED_JONG_CONSON[5][0] =  44;			MIXED_JONG_CONSON[5][1] = 67;			MIXED_JONG_CONSON[5][2] =  46;
	MIXED_JONG_CONSON[8][0] = 48;			MIXED_JONG_CONSON[8][1] = 41;			MIXED_JONG_CONSON[8][2] = 49;	
	MIXED_JONG_CONSON[9][0] = 48;			MIXED_JONG_CONSON[9][1] = 56;			MIXED_JONG_CONSON[9][2] =  50;
	MIXED_JONG_CONSON[10][0] = 48;			MIXED_JONG_CONSON[10][1] = 57;			MIXED_JONG_CONSON[10][2] =  51;
	MIXED_JONG_CONSON[12][0] = 48;			MIXED_JONG_CONSON[12][1] = 59;			MIXED_JONG_CONSON[12][2] =  52;
	MIXED_JONG_CONSON[14][0] = 48;			MIXED_JONG_CONSON[14][1] = 67;			MIXED_JONG_CONSON[14][2] =  55;
	MIXED_JONG_CONSON[17][0] = 57;			MIXED_JONG_CONSON[17][1] = 59;			MIXED_JONG_CONSON[17][2] =  58;

	//// 종성 분해 테이블
	DIVIDE_JONG_CONSON[1][0] = 41;			DIVIDE_JONG_CONSON[1][1] = 59;			DIVIDE_JONG_CONSON[1][2] =  43;
	DIVIDE_JONG_CONSON[2][0] =  44;			DIVIDE_JONG_CONSON[2][1] = 62;			DIVIDE_JONG_CONSON[2][2] =  45;
	DIVIDE_JONG_CONSON[3][0] =  44;			DIVIDE_JONG_CONSON[3][1] = 67;			DIVIDE_JONG_CONSON[3][2] =  46;
	DIVIDE_JONG_CONSON[4][0] = 48;			DIVIDE_JONG_CONSON[4][1] = 41;			DIVIDE_JONG_CONSON[4][2] =  49;
	DIVIDE_JONG_CONSON[5][0] = 48;			DIVIDE_JONG_CONSON[5][1] = 56;			DIVIDE_JONG_CONSON[5][2] =  50;
	DIVIDE_JONG_CONSON[6][0] = 48;			DIVIDE_JONG_CONSON[6][1] = 57;			DIVIDE_JONG_CONSON[6][2] =  51;
	DIVIDE_JONG_CONSON[7][0] = 48;			DIVIDE_JONG_CONSON[7][1] = 66;			DIVIDE_JONG_CONSON[7][2] =  54;
	DIVIDE_JONG_CONSON[8][0] = 48;			DIVIDE_JONG_CONSON[8][1] = 59;			DIVIDE_JONG_CONSON[8][2] =  52;
	DIVIDE_JONG_CONSON[9][0] = 48;			DIVIDE_JONG_CONSON[9][1] = 65;			DIVIDE_JONG_CONSON[9][2] =  53;
	DIVIDE_JONG_CONSON[10][0] = 48;			DIVIDE_JONG_CONSON[10][1] = 67;			DIVIDE_JONG_CONSON[10][2] =  55;
	DIVIDE_JONG_CONSON[11][0] = 57;			DIVIDE_JONG_CONSON[11][1] = 59;			DIVIDE_JONG_CONSON[11][2] =  58;

	//// Backspace시 모음 제거 순서
	MID_SUB[28-19] = 27;					MID_SUB[29-19] = 27;					MID_SUB[30-19] = 27;
	MID_SUB[33-19] = 32;					MID_SUB[34-19] = 32;					MID_SUB[35-19] = 32;

}

CAutomataKR::~CAutomataKR(void)
{
}

// 버퍼 초기화
void  CAutomataKR::Clear()
{
	m_nStatus		= HS_FIRST;
	completeText	= L"";
	ingWord			= NULL;
	m_completeWord	= NULL;
}

// 키코드 입력 및 조합 (정해진 int 코드값을 입력 받아 한글조합)
wchar_t  CAutomataKR::SetKeyCode(int nKeyCode)
{
	m_completeWord = NULL;

	// 특수키 입력
	if(nKeyCode < 0)
	{
		m_nStatus = HS_FIRST;

		if(nKeyCode == KEY_CODE_SPACE) // 띄어쓰기
		{
			if(ingWord != NULL)
				completeText += ingWord;
			else
				completeText += L' ';

			ingWord = NULL;
		}
		else if(nKeyCode == KEY_CODE_ENTER) // 내려쓰기
		{
			if(ingWord != NULL)
				completeText += ingWord;

			completeText += L"\r\n";

			ingWord = NULL;
		}
		else if(nKeyCode == KEY_CODE_BACKSPACE) // 지우기
		{
			if(ingWord == NULL)
			{
				if(completeText.GetLength() > 0)
				{
					if(completeText.Right(1) == L"\n")
						completeText = completeText.Left(completeText.GetLength() -2);
					else
						completeText = completeText.Left(completeText.GetLength() -1);
				}
			}
			else
			{
				m_nStatus = DownGradeIngWordStatus(ingWord);
			}
		}

		return m_completeWord;
	}

	switch(m_nStatus)
	{
	case HS_FIRST:
		// 초성
		m_nPhonemez[0]	= nKeyCode;
		ingWord			= SOUND_TABLE[m_nPhonemez[0]];
		m_nStatus		= nKeyCode > 18 ? HS_FIRST_C : HS_FIRST_V;
		break;

	case HS_FIRST_C:
		// 모음 + 모음
		if(nKeyCode > 18)	// 모음
		{
			if(MixVowel(&m_nPhonemez[0], nKeyCode) == FALSE)
			{
				m_completeWord = SOUND_TABLE[m_nPhonemez[0]];
				m_nPhonemez[0] = nKeyCode;
			}
		}
		else				// 자음
		{
			m_completeWord	= SOUND_TABLE[m_nPhonemez[0]];
			m_nPhonemez[0]	= nKeyCode;
			m_nStatus		= HS_FIRST_V;
		}
		break;

	case HS_FIRST_V:
		// 자음 + 자음
		if(nKeyCode > 18)	// 모음
		{
			m_nPhonemez[1]	= nKeyCode;
			m_nStatus		= HS_MIDDLE_STATE;
		}
		else				// 자음
		{
			if(!MixInitial(nKeyCode))
			{
				m_completeWord	= SOUND_TABLE[m_nPhonemez[0]];
				m_nPhonemez[0]	= nKeyCode;
				m_nStatus		= HS_FIRST_V;
			}
		}
		break;

	case HS_MIDDLE_STATE:
		// 초성 + 모음 + 모음
		if(nKeyCode > 18)
		{
			if(MixVowel(&m_nPhonemez[1], nKeyCode) == FALSE)
			{
				m_completeWord	= CombineHangle(1);
				m_nPhonemez[0]	= nKeyCode;
				m_nStatus		= HS_FIRST_C;
			}
		}
		else
		{
			int jungCode = ToFinal(nKeyCode);

			if(jungCode > 0)
			{
				m_nPhonemez[2]	= jungCode;
				m_nStatus		= HS_END_STATE;
			}
			else
			{
				m_completeWord	= CombineHangle(1);
				m_nPhonemez[0]	= nKeyCode;
				//m_nStatus		= HS_FIRST; // 2013/07/30 전승혁 고침.
				m_nStatus = HS_FIRST_V;
			}
		}
		break;

	case HS_END:
		// 초성 + 중성 + 종성
		if(nKeyCode > 18)  
		{
			m_completeWord	= CombineHangle(1);
			m_nPhonemez[0]	= nKeyCode;
			m_nStatus		= HS_FIRST;
		}
		else
		{
			int jungCode = ToFinal(nKeyCode);
			if(jungCode > 0)
			{
				m_nPhonemez[2]	= jungCode;
				m_nStatus		= HS_END_STATE;
			}
			else
			{
				m_completeWord	= CombineHangle(1);
				m_nPhonemez[0]	= nKeyCode;
				m_nStatus		= HS_FIRST;
			}  
		}
		break;

	case HS_END_STATE:
		// 초성 + 중성 + 자음(종) + 자음(종)
		if(nKeyCode > 18)
		{
			m_completeWord = CombineHangle(1);

			m_nPhonemez[0]	= ToInitial(m_nPhonemez[2]);
			m_nPhonemez[1]	= nKeyCode;
			m_nStatus		= HS_MIDDLE_STATE;
		}
		else
		{
			int jungCode = ToFinal(nKeyCode);
			if(jungCode > 0)
			{
				m_nStatus = HS_END_EXCEPTION;

				if(!MixFinal(jungCode))
				{
					m_completeWord	= CombineHangle(2);
					m_nPhonemez[0]	= nKeyCode;
					m_nStatus		= HS_FIRST_V;
				}
			}
			else
			{
				m_completeWord	= CombineHangle(2);
				m_nPhonemez[0]	= nKeyCode;
				m_nStatus		= HS_FIRST_V;
			}  
		}
		break;

	case HS_END_EXCEPTION:
		// 초성 + 중성 + 종성(곁자음)
		if(nKeyCode > 18)  
		{
			DecomposeConsonant();
			m_nPhonemez[1]	= nKeyCode;
			m_nStatus		= HS_MIDDLE_STATE;
		}
		else
		{
			int jungCode = ToFinal(nKeyCode);
			if(jungCode > 0)
			{
				m_nStatus = HS_END_EXCEPTION;

				if(!MixFinal(jungCode))
				{
					m_completeWord	= CombineHangle(2);
					m_nPhonemez[0]	= nKeyCode;
					m_nStatus		= HS_FIRST_V;
				}
			}
			else
			{
				m_completeWord	= CombineHangle(2);
				m_nPhonemez[0]	= nKeyCode;
				m_nStatus		= HS_FIRST_V; 
			}
		}
		break;
	}

	// 현재 보이는 글자상태
	CombineIngWord(m_nStatus);

	// 완성 문자열 만들기
	if(m_completeWord != NULL) 
		completeText += m_completeWord;

	return m_completeWord;
}

int CAutomataKR::GetKeyCode(wchar_t wcUnicode)
{
	for( long nCnt = 0; nCnt < sizeof(SOUND_TABLE); ++nCnt)
	{
		if( SOUND_TABLE[nCnt] == wcUnicode )
			return nCnt;
	}

	return -1;
}

// 초성으로 변환
int  CAutomataKR::ToInitial(int nKeyCode)
{
	switch(nKeyCode)
	{
	case 41: return 0;	// ㄱ
	case 42: return 1;	// ㄲ
	case 44: return 2;	// ㄴ
	case 47: return 3;	// ㄷ
	case 48: return 5;	// ㄹ
	case 56: return 6;	// ㅁ
	case 57: return 7;	// ㅂ
	case 59: return 9;	// ㅅ
	case 60: return 10;	// ㅆ
	case 61: return 11;	// ㅇ
	case 62: return 12;	// ㅈ
	case 63: return 14;	// ㅊ
	case 64: return 15;	// ㅋ
	case 65: return 16;	// ㅌ
	case 66: return 17;	// ㅍ
	case 67: return 18;	// ㅎ
	}
	return -1;
}

// 종성으로 변환
int  CAutomataKR::ToFinal(int nKeyCode)
{
	switch(nKeyCode)
	{
	case 0: return 41;	// ㄱ
	case 1: return 42;	// ㄲ
	case 2: return 44;	// ㄴ
	case 3: return 47;	// ㄷ
	case 5: return 48;	// ㄹ
	case 6: return 56;	// ㅁ
	case 7: return 57;	// ㅂ
	case 9: return 59;	// ㅅ
	case 10: return 60;	// ㅆ
	case 11: return 61;	// ㅇ
	case 12: return 62;	// ㅈ
	case 14: return 63;	// ㅊ
	case 15: return 64;	// ㅋ
	case 16: return 65;	// ㅌ
	case 17: return 66;	// ㅍ
	case 18: return 67;	// ㅎ
	}
	return -1;
}

// 자음분해
void  CAutomataKR::DecomposeConsonant()
{
	int i = 0;
	if(m_nPhonemez[3] > 40 || m_nPhonemez[4] > 40)
	{
		do
		{
			if(DIVIDE_JONG_CONSON[i][2] == m_nPhonemez[2])
			{
				m_nPhonemez[3] = DIVIDE_JONG_CONSON[i][0];
				m_nPhonemez[4] = DIVIDE_JONG_CONSON[i][1];

				m_completeWord = CombineHangle(3);
				m_nPhonemez[0]	 = ToInitial(m_nPhonemez[4]);
				return;
			}
		}
		while(++i< 13);
	}

	m_completeWord = CombineHangle(1);
	m_nPhonemez[0]	 = ToInitial(m_nPhonemez[2]);
}

// 초성합성
bool  CAutomataKR::MixInitial(int nKeyCode)
{
	if(nKeyCode >= 19)
		return FALSE;

	int i = 0;
	do
	{
		if(MIXED_CHO_CONSON[i][0] == m_nPhonemez[0] && MIXED_CHO_CONSON[i][1] == nKeyCode)
		{
			m_nPhonemez[0] = MIXED_CHO_CONSON[i][2];
			return TRUE;
		}
	}
	while(++i < 14);

	return FALSE;
}

// 종성합성
bool  CAutomataKR::MixFinal(int nKeyCode)
{
	if(nKeyCode <= 40) return FALSE;

	int i = 0;
	do
	{
		if(MIXED_JONG_CONSON[i][0] == m_nPhonemez[2] && MIXED_JONG_CONSON[i][1] == nKeyCode)
		{
			m_nPhonemez[3] = m_nPhonemez[2];
			m_nPhonemez[4] = nKeyCode;
			m_nPhonemez[2] = MIXED_JONG_CONSON[i][2];

			return TRUE;
		}
	}
	while(++i < 22);

	return FALSE;
}

// 모음합성
bool  CAutomataKR::MixVowel(int * currentCode, int inputCode)
{
	int i = 0;
	do
	{
		if(MIXED_VOWEL[i][0] == * currentCode && MIXED_VOWEL[i][1] == inputCode)
		{
			* currentCode = MIXED_VOWEL[i][2];
			return TRUE;
		}
	}
	while(++i< 21);

	return FALSE;
}

// 한글조합
wchar_t  CAutomataKR::CombineHangle(int cho, int jung, int jong)
{
	// 초성 * 21 * 28 + (중성 - 19) * 28 + (종성 - 40) + BASE_CODE;
	return BASE_CODE - 572 + jong + cho * 588 + jung * 28;
}

wchar_t  CAutomataKR::CombineHangle(int status)
{
	switch(status)
	{
		//초성 + 중성
	case 1: return CombineHangle(m_nPhonemez[0], m_nPhonemez[1], 40);

		//초성 + 중성 + 종성
	case 2: return CombineHangle(m_nPhonemez[0], m_nPhonemez[1], m_nPhonemez[2]);

		//초성 + 중성 + 곁자음01
	case 3: return CombineHangle(m_nPhonemez[0], m_nPhonemez[1], m_nPhonemez[3]);
	}
	return ' ';
}

void  CAutomataKR::CombineIngWord(int status)
{
	switch(m_nStatus)
	{
	case HS_FIRST:
	case HS_FIRST_V:
	case HS_FIRST_C:
		ingWord = SOUND_TABLE[m_nPhonemez[0]];
		break;

	case HS_MIDDLE_STATE:
	case HS_END:
		ingWord = CombineHangle(1);
		break;

	case HS_END_STATE:
	case HS_END_EXCEPTION:
		ingWord = CombineHangle(2);
		break;
	}
}

int  CAutomataKR::DownGradeIngWordStatus(wchar_t word)
{
	int iWord = word;

	//초성만 있는 경우
	if(iWord < LIMIT_MIN || iWord > LIMIT_MAX)
	{
		ingWord = NULL;

		return HS_FIRST;
	}

	int totalWord	= iWord - BASE_CODE;
	int iFirstWord	= totalWord / 28 / 21;	//초성
	int iMiddleWord = totalWord / 28 % 21;	//중성
	int iLastWord	= totalWord % 28;		//종성

	m_nPhonemez[0] = iFirstWord; //초성저장

	if(iLastWord == 0)	//종성이 없는 경우
	{
		ingWord = SOUND_TABLE[m_nPhonemez[0]];
		if( MID_SUB[ iMiddleWord ] )
		{
			m_nPhonemez[1] = MID_SUB[ iMiddleWord ];
			ingWord  = CombineHangle(1);
			return HS_MIDDLE_STATE;
		}
		return HS_FIRST_V;
	}

	m_nPhonemez[1] = iMiddleWord + 19; //중성저장

	iLastWord += 40;

	for(int i = 0; i < 13; i++)
	{
		if(iLastWord == DIVIDE_JONG_CONSON[i][2])
		{
			ingWord = CombineHangle(3);
			m_nPhonemez[2] = DIVIDE_JONG_CONSON[i][0]; // 종성저장
			return HS_END_EXCEPTION;
		}
	}

	ingWord = CombineHangle(1);

	return HS_MIDDLE_STATE;
}