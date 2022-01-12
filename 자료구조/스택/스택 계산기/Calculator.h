#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#define MAX_STR_LEN 256
#define ARRAY_LEN(X) (sizeof(X)/sizeof(X[0])) //배열의 길이 (

enum class SYMBOL_TYPE : const char
{
	LEFT_PARENTHESIS = '(',
	RIGHT_PARENTHESIS = ')',
	PLUS = '+',
	MINUS = '-',
	MULTIPLY = '*',
	DIVIDE = '/',
	SPACE = ' ', //공백
	OPERAND //피연산자 (숫자 혹은 소수 표현을 위한 '.' 기호)
}; //기호 타입

typedef struct
{
	char str[MAX_STR_LEN]; //읽은 문자열
	SYMBOL_TYPE symbolType; //기호 타입
	unsigned int readCount; //읽은 문자 개수
}Token;

inline int CharToDecAscii(char);
inline int IntToDecAscii(int) throw(std::invalid_argument);

SYMBOL_TYPE GetSymbolTypeFromChar(char) throw(std::invalid_argument);
int GetSymbolTypePriority(SYMBOL_TYPE, bool);

void CreateNextToken(const char*, Token*) throw(std::invalid_argument, std::out_of_range);
void GenPostfixExpr(const char*, char*) throw(std::invalid_argument);
double CalcPostfixExpr(const char*);
#endif