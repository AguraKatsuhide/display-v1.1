
#pragma once


#define		KEY_CODE_SPACE			-1		// ����
#define		KEY_CODE_ENTER			-2		// ��������
#define		KEY_CODE_BACKSPACE		-3		// �����

class CAutomataKR
{
public:
	int BASE_CODE; //= 0xac00;		// ��������(��)
	int LIMIT_MIN; //= 0xac00;		// �������� MIN(��)
	int LIMIT_MAX; //= 0xd7a3;		// �������� MAX
	int test;
	wchar_t		ingWord;		// �ۼ��� ����
	CStringW	completeText;	// �ϼ� ���ڿ�
	int			m_nStatus;		// �ܾ����ջ���
	wchar_t		m_completeWord;	// �ϼ�����
	int			m_nPhonemez[5]; // ����[��,��,��,������1,������2]
	
	// �� ���̺� , �����ڿ��� ������(�ϵ��ڵ�)
	wchar_t SOUND_TABLE[68];

private:
	enum	HAN_STATUS		// �ܾ����ջ���
	{
		HS_FIRST = 0,		// �ʼ�
		HS_FIRST_V,			// ���� + ���� 
		HS_FIRST_C,			// ���� + ����
		HS_MIDDLE_STATE,	// �ʼ� + ���� + ����
		HS_END,				// �ʼ� + �߼� + ����
		HS_END_STATE,		// �ʼ� + �߼� + ���� + ����
		HS_END_EXCEPTION	// �ʼ� + �߼� + ����(������)
	};

	
	int MIXED_CHO_CONSON[14][3];
	int MIXED_VOWEL[21][3];
	int MIXED_JONG_CONSON[22][3];
	int DIVIDE_JONG_CONSON[13][3] ;
	int MID_SUB[21];

public:
	CAutomataKR(void);
	~CAutomataKR(void);

	// ���� �ʱ�ȭ
	void  Clear();	
	// Ű�ڵ� �Է� �� ���� (������ int �ڵ尪�� �Է� �޾� �ѱ�����)
	wchar_t  SetKeyCode(int nKeyCode);
	// �����ڵ忡�� Ű�ڵ带 ��� �´�.
	int GetKeyCode(wchar_t);
	// �ʼ����� ��ȯ
	int  ToInitial(int nKeyCode);
	// �������� ��ȯ
	int  ToFinal(int nKeyCode);
	// ��������
	void  DecomposeConsonant();
	// �ʼ��ռ�
	bool  MixInitial(int nKeyCode);
	// �����ռ�
	bool  MixFinal(int nKeyCode);
	// �����ռ�
	bool  MixVowel(int * currentCode, int inputCode);
	// �ѱ�����
	wchar_t  CombineHangle(int cho, int jung, int jong);

	int  DownGradeIngWordStatus(wchar_t word);

	void  CombineIngWord(int status);

	wchar_t  CombineHangle(int status);	
};