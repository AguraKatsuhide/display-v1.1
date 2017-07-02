
#include "stdafx.h"
#include "AutomataKR.h"


CAutomataKR::CAutomataKR(void)
{
	Clear();
	BASE_CODE = 0xac00;		// ��������(��)
	LIMIT_MIN = 0xac00;		// �������� MIN(��)
	LIMIT_MAX = 0xd7a3;		// �������� MAX

	memset( MID_SUB, 0, sizeof(int) * 21 );

	//	/* �ʼ� 19�� 0 ~ 18 */
	SOUND_TABLE[0] = L'��';			SOUND_TABLE[1] = L'��';			SOUND_TABLE[2] = L'��';			SOUND_TABLE[3] = L'��';
	SOUND_TABLE[4] = L'��';			SOUND_TABLE[5] = L'��';			SOUND_TABLE[6] = L'��';			SOUND_TABLE[7] = L'��';
	SOUND_TABLE[8] = L'��';			SOUND_TABLE[9] = L'��';			SOUND_TABLE[10] = L'��';		SOUND_TABLE[11] = L'��';
	SOUND_TABLE[12] = L'��';		SOUND_TABLE[13] = L'��';		SOUND_TABLE[14] = L'��';		SOUND_TABLE[15] = L'��';
	SOUND_TABLE[16] = L'��';		SOUND_TABLE[17] = L'��';		SOUND_TABLE[18] = L'��';

	//	/* �߼� 21�� 19 ~ 39 */
	SOUND_TABLE[19] = L'��';		SOUND_TABLE[20] = L'��';		SOUND_TABLE[21] = L'��';		SOUND_TABLE[22] = L'��';
	SOUND_TABLE[23] = L'��';		SOUND_TABLE[24] = L'��';		SOUND_TABLE[25] = L'��';		SOUND_TABLE[26] = L'��';
	SOUND_TABLE[27] = L'��';		SOUND_TABLE[28] = L'��';		SOUND_TABLE[29] = L'��';		SOUND_TABLE[30] = L'��';
	SOUND_TABLE[31] = L'��';		SOUND_TABLE[32] = L'��';		SOUND_TABLE[33] = L'��';		SOUND_TABLE[34] = L'��';
	SOUND_TABLE[35] = L'��';		SOUND_TABLE[36] = L'��';		SOUND_TABLE[37] = L'��';		SOUND_TABLE[38] = L'��';
	SOUND_TABLE[39] = L'��';

	//	/* ���� 28�� 40 ~ 67 */
	SOUND_TABLE[40] = L' ';			SOUND_TABLE[41] = L'��';		SOUND_TABLE[42] = L'��';		SOUND_TABLE[43] = L'��';
	SOUND_TABLE[44] = L'��';		SOUND_TABLE[45] = L'��';		SOUND_TABLE[46] = L'��';		SOUND_TABLE[47] = L'��';
	SOUND_TABLE[48] = L'��';		SOUND_TABLE[49] = L'��';		SOUND_TABLE[50] = L'��';		SOUND_TABLE[51] = L'��';
	SOUND_TABLE[52] = L'��';		SOUND_TABLE[53] = L'��';		SOUND_TABLE[54] = L'��';		SOUND_TABLE[55] = L'��';
	SOUND_TABLE[56] = L'��';		SOUND_TABLE[57] = L'��';		SOUND_TABLE[58] = L'��';		SOUND_TABLE[59] = L'��';
	SOUND_TABLE[60] = L'��';		SOUND_TABLE[61] = L'��';		SOUND_TABLE[62] = L'��';		SOUND_TABLE[63] = L'��';
	SOUND_TABLE[64] = L'��';		SOUND_TABLE[65] = L'��';		SOUND_TABLE[66] = L'��';		SOUND_TABLE[67] = L'��';


	// �ʼ� �ռ� ���̺�
	MIXED_CHO_CONSON[0][0] = 0;		MIXED_CHO_CONSON[0][1] = 0;		MIXED_CHO_CONSON[0][2] = 1;
	MIXED_CHO_CONSON[3][0] = 3;		MIXED_CHO_CONSON[3][1] = 3;		MIXED_CHO_CONSON[3][2] = 4;
	MIXED_CHO_CONSON[6][0] = 7;		MIXED_CHO_CONSON[6][1] = 7;		MIXED_CHO_CONSON[6][2] = 8;
	MIXED_CHO_CONSON[9][0] = 9;		MIXED_CHO_CONSON[9][1] = 9;		MIXED_CHO_CONSON[9][2] = 10;
	MIXED_CHO_CONSON[11][0] = 12;	MIXED_CHO_CONSON[11][1] = 12;	MIXED_CHO_CONSON[11][2] = 13;

	// �߼� �ռ� ���̺�
	MIXED_JONG_CONSON[0][0] = 41;	MIXED_JONG_CONSON[0][1] = 41;	MIXED_JONG_CONSON[0][2] = 42;
	MIXED_JONG_CONSON[18][0] = 59;	MIXED_JONG_CONSON[18][1] = 59;	MIXED_JONG_CONSON[18][2] = 60;

	//// �ʼ�,�߼� ���� �ռ� ���̺�
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

	//// ���� �ռ� ���̺�
	MIXED_JONG_CONSON[3][0] =  41;			MIXED_JONG_CONSON[3][1] =  59;			MIXED_JONG_CONSON[3][2] =  43;
	MIXED_JONG_CONSON[4][0] = 44;			MIXED_JONG_CONSON[4][1] = 62;			MIXED_JONG_CONSON[4][2] = 45;
	MIXED_JONG_CONSON[5][0] =  44;			MIXED_JONG_CONSON[5][1] = 67;			MIXED_JONG_CONSON[5][2] =  46;
	MIXED_JONG_CONSON[8][0] = 48;			MIXED_JONG_CONSON[8][1] = 41;			MIXED_JONG_CONSON[8][2] = 49;	
	MIXED_JONG_CONSON[9][0] = 48;			MIXED_JONG_CONSON[9][1] = 56;			MIXED_JONG_CONSON[9][2] =  50;
	MIXED_JONG_CONSON[10][0] = 48;			MIXED_JONG_CONSON[10][1] = 57;			MIXED_JONG_CONSON[10][2] =  51;
	MIXED_JONG_CONSON[12][0] = 48;			MIXED_JONG_CONSON[12][1] = 59;			MIXED_JONG_CONSON[12][2] =  52;
	MIXED_JONG_CONSON[14][0] = 48;			MIXED_JONG_CONSON[14][1] = 67;			MIXED_JONG_CONSON[14][2] =  55;
	MIXED_JONG_CONSON[17][0] = 57;			MIXED_JONG_CONSON[17][1] = 59;			MIXED_JONG_CONSON[17][2] =  58;

	//// ���� ���� ���̺�
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

	//// Backspace�� ���� ���� ����
	MID_SUB[28-19] = 27;					MID_SUB[29-19] = 27;					MID_SUB[30-19] = 27;
	MID_SUB[33-19] = 32;					MID_SUB[34-19] = 32;					MID_SUB[35-19] = 32;

}

CAutomataKR::~CAutomataKR(void)
{
}

// ���� �ʱ�ȭ
void  CAutomataKR::Clear()
{
	m_nStatus		= HS_FIRST;
	completeText	= L"";
	ingWord			= NULL;
	m_completeWord	= NULL;
}

// Ű�ڵ� �Է� �� ���� (������ int �ڵ尪�� �Է� �޾� �ѱ�����)
wchar_t  CAutomataKR::SetKeyCode(int nKeyCode)
{
	m_completeWord = NULL;

	// Ư��Ű �Է�
	if(nKeyCode < 0)
	{
		m_nStatus = HS_FIRST;

		if(nKeyCode == KEY_CODE_SPACE) // ����
		{
			if(ingWord != NULL)
				completeText += ingWord;
			else
				completeText += L' ';

			ingWord = NULL;
		}
		else if(nKeyCode == KEY_CODE_ENTER) // ��������
		{
			if(ingWord != NULL)
				completeText += ingWord;

			completeText += L"\r\n";

			ingWord = NULL;
		}
		else if(nKeyCode == KEY_CODE_BACKSPACE) // �����
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
		// �ʼ�
		m_nPhonemez[0]	= nKeyCode;
		ingWord			= SOUND_TABLE[m_nPhonemez[0]];
		m_nStatus		= nKeyCode > 18 ? HS_FIRST_C : HS_FIRST_V;
		break;

	case HS_FIRST_C:
		// ���� + ����
		if(nKeyCode > 18)	// ����
		{
			if(MixVowel(&m_nPhonemez[0], nKeyCode) == FALSE)
			{
				m_completeWord = SOUND_TABLE[m_nPhonemez[0]];
				m_nPhonemez[0] = nKeyCode;
			}
		}
		else				// ����
		{
			m_completeWord	= SOUND_TABLE[m_nPhonemez[0]];
			m_nPhonemez[0]	= nKeyCode;
			m_nStatus		= HS_FIRST_V;
		}
		break;

	case HS_FIRST_V:
		// ���� + ����
		if(nKeyCode > 18)	// ����
		{
			m_nPhonemez[1]	= nKeyCode;
			m_nStatus		= HS_MIDDLE_STATE;
		}
		else				// ����
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
		// �ʼ� + ���� + ����
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
				//m_nStatus		= HS_FIRST; // 2013/07/30 ������ ��ħ.
				m_nStatus = HS_FIRST_V;
			}
		}
		break;

	case HS_END:
		// �ʼ� + �߼� + ����
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
		// �ʼ� + �߼� + ����(��) + ����(��)
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
		// �ʼ� + �߼� + ����(������)
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

	// ���� ���̴� ���ڻ���
	CombineIngWord(m_nStatus);

	// �ϼ� ���ڿ� �����
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

// �ʼ����� ��ȯ
int  CAutomataKR::ToInitial(int nKeyCode)
{
	switch(nKeyCode)
	{
	case 41: return 0;	// ��
	case 42: return 1;	// ��
	case 44: return 2;	// ��
	case 47: return 3;	// ��
	case 48: return 5;	// ��
	case 56: return 6;	// ��
	case 57: return 7;	// ��
	case 59: return 9;	// ��
	case 60: return 10;	// ��
	case 61: return 11;	// ��
	case 62: return 12;	// ��
	case 63: return 14;	// ��
	case 64: return 15;	// ��
	case 65: return 16;	// ��
	case 66: return 17;	// ��
	case 67: return 18;	// ��
	}
	return -1;
}

// �������� ��ȯ
int  CAutomataKR::ToFinal(int nKeyCode)
{
	switch(nKeyCode)
	{
	case 0: return 41;	// ��
	case 1: return 42;	// ��
	case 2: return 44;	// ��
	case 3: return 47;	// ��
	case 5: return 48;	// ��
	case 6: return 56;	// ��
	case 7: return 57;	// ��
	case 9: return 59;	// ��
	case 10: return 60;	// ��
	case 11: return 61;	// ��
	case 12: return 62;	// ��
	case 14: return 63;	// ��
	case 15: return 64;	// ��
	case 16: return 65;	// ��
	case 17: return 66;	// ��
	case 18: return 67;	// ��
	}
	return -1;
}

// ��������
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

// �ʼ��ռ�
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

// �����ռ�
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

// �����ռ�
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

// �ѱ�����
wchar_t  CAutomataKR::CombineHangle(int cho, int jung, int jong)
{
	// �ʼ� * 21 * 28 + (�߼� - 19) * 28 + (���� - 40) + BASE_CODE;
	return BASE_CODE - 572 + jong + cho * 588 + jung * 28;
}

wchar_t  CAutomataKR::CombineHangle(int status)
{
	switch(status)
	{
		//�ʼ� + �߼�
	case 1: return CombineHangle(m_nPhonemez[0], m_nPhonemez[1], 40);

		//�ʼ� + �߼� + ����
	case 2: return CombineHangle(m_nPhonemez[0], m_nPhonemez[1], m_nPhonemez[2]);

		//�ʼ� + �߼� + ������01
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

	//�ʼ��� �ִ� ���
	if(iWord < LIMIT_MIN || iWord > LIMIT_MAX)
	{
		ingWord = NULL;

		return HS_FIRST;
	}

	int totalWord	= iWord - BASE_CODE;
	int iFirstWord	= totalWord / 28 / 21;	//�ʼ�
	int iMiddleWord = totalWord / 28 % 21;	//�߼�
	int iLastWord	= totalWord % 28;		//����

	m_nPhonemez[0] = iFirstWord; //�ʼ�����

	if(iLastWord == 0)	//������ ���� ���
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

	m_nPhonemez[1] = iMiddleWord + 19; //�߼�����

	iLastWord += 40;

	for(int i = 0; i < 13; i++)
	{
		if(iLastWord == DIVIDE_JONG_CONSON[i][2])
		{
			ingWord = CombineHangle(3);
			m_nPhonemez[2] = DIVIDE_JONG_CONSON[i][0]; // ��������
			return HS_END_EXCEPTION;
		}
	}

	ingWord = CombineHangle(1);

	return HS_MIDDLE_STATE;
}