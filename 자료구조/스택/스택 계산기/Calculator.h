#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

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

typedef struct
{
	char str[MAX_STR_LEN]; //���� ���ڿ�
	SYMBOL_TYPE symbolType; //��ȣ Ÿ��
	unsigned int readCount; //���� ���� ����
}Token;

inline double CalcOperation(double, char, double);
double CalcOperation(double, SYMBOL_TYPE, double);

inline int CharToDecAscii(char);
inline int SingleNumToDecAscii(int);
SYMBOL_TYPE CharToSymbolType(char);

int GetSymbolTypePriority(SYMBOL_TYPE);

void GenNextToken(const char*, Token*);
void GenPostfixExpr(const char*, char*);
double CalcPostfixExpr(const char*);
#endif