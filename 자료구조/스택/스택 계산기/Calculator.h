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
	OPERAND, //피연산자
	SPACE = ' ', //피연산자 간 구분을 위한 공백
	DOT = '.', //실수 표현을 위한 점
}; //기호 타입

typedef struct
{
	char str[MAX_STR_LEN]; //읽은 문자열
	SYMBOL_TYPE symbolType; //기호 타입
	unsigned int readCount; //읽은 문자 개수
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