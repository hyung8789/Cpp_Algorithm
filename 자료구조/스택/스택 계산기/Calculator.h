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
	DOT = '.', //�Ҽ� ǥ���� ���� ��
}; //��ȣ Ÿ��

typedef struct
{
	char str[MAX_STR_LEN]; //���� ���ڿ�
	SYMBOL_TYPE symbolType; //��ȣ Ÿ��
	unsigned int readCount; //���� ���� ����
}Token;

inline double CalcOperation(double, char, double) throw(std::invalid_argument, std::overflow_error, std::underflow_error);
double CalcOperation(double, SYMBOL_TYPE, double) throw(std::invalid_argument, std::overflow_error, std::underflow_error);

inline int CharToDecAscii(char);
inline int SingleNumToDecAscii(int) throw(std::invalid_argument);
SYMBOL_TYPE CharToSymbolType(char) throw(std::invalid_argument);

int GetSymbolTypePriority(SYMBOL_TYPE, bool);

void GenNextToken(const char*, Token*) throw(std::invalid_argument, std::out_of_range);
void GenPostfixExpr(const char*, char*) throw(std::invalid_argument, std::runtime_error, std::logic_error);
double CalcPostfixExpr(const char*) throw(std::invalid_argument);
#endif