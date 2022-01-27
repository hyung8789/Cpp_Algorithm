#ifndef _TOKENGEN_H_
#define _TOKENGEN_H_

//TODO : 토큰 생성 모듈 분리 및 기능 추가에 따른 스택 계산기 모듈 단위 리팩토링
//TODO : 이에 따른 스택 계산기에 의존적인 주석 및 README 수정 (스택에 삽입 위해 => 현재 읽은 문자에 대해 처리를 위해)

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

enum class EXPR_READ_DIRECTION : const int
{
	/***
		< 대상 표현식 읽는 방향에 따른 토큰 처리 >

		1) 왼쪽에서 오른쪽
		: 기존과 같은 방법
		
		2) 오른쪽에서 왼쪽
		: 한 자리를 초과하는 정수 혹은 '.'을 포함하는 실수일 경우 기존과 같은 방법으로 처리 할 수 없으며, 다음 방법이 존재 (피연산자의 길이가 n이라고 가정)
		
			M1) 현재까지 읽어들인 피연산자 문자열에 대해 처리를 위해 읽기가 중지되는 시점에, 버퍼에 역순으로 배치 된 읽어들인 피연산자 문자열을 정방향으로 재정렬
				M1-2) 제자리 변환 (문자열 반 길이의 루프 간 양 측 Swap) : 시간 복잡도 O(n/2)
				M1-1) 제자리 변환이 아닌 별도의 결과 출력 버퍼 사용 : 시간 복잡도 O(n), 공간 복잡도 O(n)

			M2) 피연산자 문자열을 읽어들이기 시작한 시점에 해당 피연산자의 길이를 미리 예측하여, 정렬을 수행하지 않고 피연산자 문자열을 역방향으로 버퍼에 읽어들이기
				피연산자의 길이를 예측하는 과정 (피연산자의 길이가 예측이 끝나지 않는 시점에는 현재 읽어들인 피연산자를 버퍼에 배치 불가) : 시간 복잡도 O(n)
				+ 피연산자의 길이를 예측하는 과정에서 읽은 문자를 다음에 다시 읽는 비효율적인 문제
	***/

	LEFT_TO_RIGHT = 0, //왼쪽에서 오른쪽
	RIGHT_TO_LEFT //오른쪽에서 왼쪽
}; //표현식 읽는 방향

typedef struct
{
	char str[MAX_STR_LEN]; //읽은 문자열
	SYMBOL_TYPE symbolType; //기호 타입
	unsigned int readCount; //읽은 문자 개수
}Token;

void ReverseInplaceStr(char*, size_t);
inline int CharToDecAscii(char);
inline int SingleNumToDecAscii(int);
SYMBOL_TYPE CharToSymbolType(char);

int GetSymbolTypePriority(SYMBOL_TYPE);

void GenNextToken(const char*, Token*, EXPR_READ_DIRECTION);
#endif