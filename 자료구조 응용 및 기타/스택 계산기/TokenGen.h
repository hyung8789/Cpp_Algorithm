#ifndef _TOKENGEN_H_
#define _TOKENGEN_H_

#define MAX_STR_LEN 256

enum class SYMBOL_TYPE : const char
{
	LEFT_PARENTHESIS = '(',
	RIGHT_PARENTHESIS = ')',
	PLUS = '+',
	MINUS = '-',
	MULTIPLY = '*',
	DIVIDE = '/',
	OPERAND, //�ǿ�����
	SPACE = ' ', //�ǿ����� �� ������ ���� ����
	DOT = '.', //�Ǽ� ǥ���� ���� ��
}; //��ȣ Ÿ��

enum class EXPR_READ_DIRECTION : const int
{
	/***
		< ��� ǥ���� �д� ���⿡ ���� ��ū ó�� >

		1) ���ʿ��� ������ (������)
		: ������ ���� ���

		2) �����ʿ��� ���� (������)
		: �� �ڸ��� �ʰ��ϴ� ���� Ȥ�� '.'�� �����ϴ� �Ǽ��� ��� ������ ���� ������� ó�� �� �� ������, ���� ����� ����

			M1) ������� �о���� �ǿ����� ���ڿ��� ���� ó���� ���� �бⰡ �����Ǵ� ������, ���ۿ� �������� ��ġ �� �о���� �ǿ����� ���ڿ��� ���������� ������
				M1-1) ���ڸ� ��ȯ�� �ƴ� ������ ��� ��� ���� ���
				M1-2) ���ڸ� ��ȯ (���ڿ� �� ������ ���� �� �� �� Swap)

			M2) �ǿ����� ���ڿ��� �о���̱� ������ ������ �ش� �ǿ������� ���̸� �̸� �����Ͽ�, ������ �������� �ʰ� �ǿ����� ���ڿ��� ���������� ���ۿ� �о���̱�
				�ǿ������� ���̸� �����ϴ� ���� (�ǿ������� ���̰� ������ ������ �ʴ� �������� ���� �о���� �ǿ����ڸ� ���ۿ� ��ġ �Ұ�)
				+ �ǿ������� ���̸� �����ϴ� �������� ���� ���ڸ� ������ �ٽ� �д� ��ȿ������ ����
	***/

	LEFT_TO_RIGHT = 0, //���ʿ��� ������ (������)
	RIGHT_TO_LEFT //�����ʿ��� ���� (������)
}; //ǥ���� �д� ����

typedef struct
{
	char _str[MAX_STR_LEN]; //���� ���ڿ�
	SYMBOL_TYPE _symbolType; //��ȣ Ÿ��
	size_t _readCount; //���� ���� ����
}TOKEN;

SYMBOL_TYPE CharToSymbolType(char);
SYMBOL_TYPE StrToSymbolType(const char*);

int GetSymbolTypePriority(SYMBOL_TYPE);
void GenNextToken(const char*, TOKEN*, EXPR_READ_DIRECTION);
#endif