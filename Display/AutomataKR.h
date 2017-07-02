
#pragma once


#define		KEY_CODE_SPACE			-1		// 띄어쓰기
#define		KEY_CODE_ENTER			-2		// 내려쓰기
#define		KEY_CODE_BACKSPACE		-3		// 지우기

class CAutomataKR
{
public:
	int BASE_CODE; //= 0xac00;		// 기초음성(가)
	int LIMIT_MIN; //= 0xac00;		// 음성범위 MIN(가)
	int LIMIT_MAX; //= 0xd7a3;		// 음성범위 MAX
	int test;
	wchar_t		ingWord;		// 작성중 글자
	CStringW	completeText;	// 완성 문자열
	int			m_nStatus;		// 단어조합상태
	wchar_t		m_completeWord;	// 완성글자
	int			m_nPhonemez[5]; // 음소[초,중,종,곁자음1,곁자음2]
	
	// 음 테이블 , 생성자에서 생성됨(하드코딩)
	wchar_t SOUND_TABLE[68];

private:
	enum	HAN_STATUS		// 단어조합상태
	{
		HS_FIRST = 0,		// 초성
		HS_FIRST_V,			// 자음 + 자음 
		HS_FIRST_C,			// 모음 + 모음
		HS_MIDDLE_STATE,	// 초성 + 모음 + 모음
		HS_END,				// 초성 + 중성 + 종성
		HS_END_STATE,		// 초성 + 중성 + 자음 + 자음
		HS_END_EXCEPTION	// 초성 + 중성 + 종성(곁자음)
	};

	
	int MIXED_CHO_CONSON[14][3];
	int MIXED_VOWEL[21][3];
	int MIXED_JONG_CONSON[22][3];
	int DIVIDE_JONG_CONSON[13][3] ;
	int MID_SUB[21];

public:
	CAutomataKR(void);
	~CAutomataKR(void);

	// 버퍼 초기화
	void  Clear();	
	// 키코드 입력 및 조합 (정해진 int 코드값을 입력 받아 한글조합)
	wchar_t  SetKeyCode(int nKeyCode);
	// 유니코드에서 키코드를 얻어 온다.
	int GetKeyCode(wchar_t);
	// 초성으로 변환
	int  ToInitial(int nKeyCode);
	// 종성으로 변환
	int  ToFinal(int nKeyCode);
	// 자음분해
	void  DecomposeConsonant();
	// 초성합성
	bool  MixInitial(int nKeyCode);
	// 종성합성
	bool  MixFinal(int nKeyCode);
	// 모음합성
	bool  MixVowel(int * currentCode, int inputCode);
	// 한글조합
	wchar_t  CombineHangle(int cho, int jung, int jong);

	int  DownGradeIngWordStatus(wchar_t word);

	void  CombineIngWord(int status);

	wchar_t  CombineHangle(int status);	
};