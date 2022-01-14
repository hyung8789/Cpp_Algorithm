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
	DOT = '.', //소수 표현을 위한 점
}; //기호 타입

typedef struct
{
	char str[MAX_STR_LEN]; //읽은 문자열
	SYMBOL_TYPE symbolType; //기호 타입
	unsigned int readCount; //읽은 문자 개수
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