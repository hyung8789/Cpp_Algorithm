#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#define MAX_STR_LEN 256
#define ARRAY_LEN(X) (sizeof(X)/sizeof(X[0])) //�迭�� ���� (

enum class SYMBOL_TYPE : const char
{
	LEFT_PARENTHESIS = '(',
	RIGHT_PARENTHESIS = ')',
	PLUS = '+',
	MINUS = '-',
	MULTIPLY = '*',
	DIVIDE = '/',
	SPACE = ' ', //����
	OPERAND //�ǿ����� (���� Ȥ�� �Ҽ� ǥ���� ���� '.' ��ȣ)
}; //��ȣ Ÿ��

typedef struct
{
	char str[MAX_STR_LEN]; //���� ���ڿ�
	SYMBOL_TYPE symbolType; //��ȣ Ÿ��
	unsigned int readCount; //���� ���� ����
}Token;

inline int CharToDecAscii(char);
inline int IntToDecAscii(int) throw(std::invalid_argument);

SYMBOL_TYPE GetSymbolTypeFromChar(char) throw(std::invalid_argument);
int GetSymbolTypePriority(SYMBOL_TYPE, bool);

void CreateNextToken(const char*, Token*) throw(std::invalid_argument, std::out_of_range);
void GenPostfixExpr(const char*, char*) throw(std::invalid_argument);
double CalcPostfixExpr(const char*);
#endif