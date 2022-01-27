#ifndef _TOKENGEN_H_
#define _TOKENGEN_H_

//TODO : ��ū ���� ��� �и� �� ��� �߰��� ���� ���� ���� ��� ���� �����丵
//TODO : �̿� ���� ���� ���⿡ �������� �ּ� �� README ���� (���ÿ� ���� ���� => ���� ���� ���ڿ� ���� ó���� ����)

#define MAX_STR_LEN 256

#include <cfenv> // https://www.cplusplus.com/reference/cfenv

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

		1) ���ʿ��� ������
		: ������ ���� ���
		
		2) �����ʿ��� ����
		: �� �ڸ��� �ʰ��ϴ� ���� Ȥ�� '.'�� �����ϴ� �Ǽ��� ��� ������ ���� ������� ó�� �� �� ������, ���� ����� ���� (�ǿ������� ���̰� n�̶�� ����)
		
			M1) ������� �о���� �ǿ����� ���ڿ��� ���� ó���� ���� �бⰡ �����Ǵ� ������, ���ۿ� �������� ��ġ �� �о���� �ǿ����� ���ڿ��� ���������� ������
				M1-2) ���ڸ� ��ȯ (���ڿ� �� ������ ���� �� �� �� Swap) : �ð� ���⵵ O(n/2)
				M1-1) ���ڸ� ��ȯ�� �ƴ� ������ ��� ��� ���� ��� : �ð� ���⵵ O(n), ���� ���⵵ O(n)

			M2) �ǿ����� ���ڿ��� �о���̱� ������ ������ �ش� �ǿ������� ���̸� �̸� �����Ͽ�, ������ �������� �ʰ� �ǿ����� ���ڿ��� ���������� ���ۿ� �о���̱�
				�ǿ������� ���̸� �����ϴ� ���� (�ǿ������� ���̰� ������ ������ �ʴ� �������� ���� �о���� �ǿ����ڸ� ���ۿ� ��ġ �Ұ�) : �ð� ���⵵ O(n)
				+ �ǿ������� ���̸� �����ϴ� �������� ���� ���ڸ� ������ �ٽ� �д� ��ȿ������ ����
	***/

	LEFT_TO_RIGHT = 0, //���ʿ��� ������
	RIGHT_TO_LEFT //�����ʿ��� ����
}; //ǥ���� �д� ����

typedef struct
{
	char str[MAX_STR_LEN]; //���� ���ڿ�
	SYMBOL_TYPE symbolType; //��ȣ Ÿ��
	unsigned int readCount; //���� ���� ����
}Token;

void ReverseInplaceStr(char*, size_t);
inline int CharToDecAscii(char);
inline int SingleNumToDecAscii(int);
SYMBOL_TYPE CharToSymbolType(char);

int GetSymbolTypePriority(SYMBOL_TYPE);

void GenNextToken(const char*, Token*, EXPR_READ_DIRECTION);
#endif