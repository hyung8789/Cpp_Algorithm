#include "Core.h"

/// <summary>
/// 대상 문자열 제자리 좌우반전
/// </summary>
/// <param name="targetStr">대상 문자열</param>
/// <param name="targetStrLen">대상 문자열의 길이 ('\0' 제외 한 길이)</param>
void ReverseInplaceStr(char* targetStr, size_t targetStrLen)
{
	if (targetStr == NULL || targetStrLen == 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	for (size_t i = 0; i < (targetStrLen / 2); i++)
	{
		char tmp = targetStr[i];
		targetStr[i] = targetStr[targetStrLen - i - 1];
		targetStr[targetStrLen - i - 1] = tmp;
	}
}

/// <summary>
/// 대상 문자를 10진 아스키 코드로 변환
/// </summary>
/// <param name="srcChar">대상 문자</param>
/// <returns>변환 된 10진 아스키 코드</returns>
inline int CharToDecAscii(char srcChar)
{
	return (int)srcChar;
}

/// <summary>
/// 대상 0~9 범위의 단일 숫자를 10진 아스키 코드로 변환
/// </summary>
/// <param name="srcSingleNum">대상 0~9 범위의 단일 숫자</param>
/// <returns>변환 된 10진 아스키 코드</returns>
inline int SingleNumToDecAscii(int srcSingleNum)
{
	if (srcSingleNum < 0 || srcSingleNum > 9)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	return '0' + srcSingleNum;
}

/// <summary>
/// 대상 문자를 기호 타입으로 변환
/// </summary>
/// <param name="srcChar">대상 문자</param>
/// <returns>대상 문자의 기호 타입</returns>
SYMBOL_TYPE CharToSymbolType(char srcChar)
{
	int srcCharDecAscii = CharToDecAscii(srcChar); //대상 문자의 10진 아스키 코드

	switch (srcChar) //대상 문자에 따라
	{
	case (const char)SYMBOL_TYPE::LEFT_PARENTHESIS:
	case (const char)SYMBOL_TYPE::RIGHT_PARENTHESIS:
	case (const char)SYMBOL_TYPE::PLUS:
	case (const char)SYMBOL_TYPE::MINUS:
	case (const char)SYMBOL_TYPE::MULTIPLY:
	case (const char)SYMBOL_TYPE::DIVIDE:
	case (const char)SYMBOL_TYPE::SPACE:
	case (const char)SYMBOL_TYPE::DOT:
		break;

	default: //피연산자 혹은 잘못 된 입력
		if (!(srcCharDecAscii >= SingleNumToDecAscii(0) &&
			srcCharDecAscii <= SingleNumToDecAscii(9))) //0 (dec ascii : 48) ~ 9 (dec ascii : 57) 가 아닐 경우 (피연산자가 아닌 경우)
			throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

		return SYMBOL_TYPE::OPERAND;
	}

	return (SYMBOL_TYPE)srcChar;
}

/// <summary>
/// 대상 기호 타입에 대한 연산자 우선순위 반환
/// </summary>
/// <param name="srcSymbol">대상 기호 타입</param>
/// <returns>우선순위 (클 수록 높은 연산 우선순위)</returns>
int GetSymbolTypePriority(SYMBOL_TYPE srcSymbol)
{
	int priority = -1; //우선순위 (클 수록 높은 연산 우선순위)

	switch (srcSymbol)
	{
	case SYMBOL_TYPE::LEFT_PARENTHESIS:
		break;

	case SYMBOL_TYPE::PLUS:
	case SYMBOL_TYPE::MINUS:
		priority = 0;
		break;

	case SYMBOL_TYPE::MULTIPLY:
	case SYMBOL_TYPE::DIVIDE:
		priority = 1;
		break;

	default: //')', 피연산자, 피연산자 간 구분을 위한 공백, 실수 표현을 위한 점은 우선순위를 관리하지 않음
		throw std::logic_error(std::string(__func__) + std::string(" : unmanaged priority"));
	}

	return priority;
}

/// <summary>
/// 대상 표현식으로부터 토큰 생성
/// </summary>
/// <param name="srcExpr">대상 표현식</param>
/// <param name="dstToken">대상 표현식으로부터 생성되어 출력 될 토큰</param>
void GenNextToken(const char* srcExpr, Token* dstToken, EXPR_READ_DIRECTION exprReadDirection)
{
	if (srcExpr == NULL || dstToken == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (strlen(srcExpr) > sizeof(dstToken->str) - 1) //'\0' 제외 한 크기를 초과 할 경우
		throw std::out_of_range(std::string(__func__) + std::string(" : out of range"));

	memset(dstToken->str, '\0', sizeof(dstToken->str));
	dstToken->readCount = 0;

	//TODO : 읽는 방향 변경, 읽는 방향이 역순일 경우 
	//2-1) 현재 읽은 문자의 기호 타입이 피연산자인 경우에서
	//현재 문자의 다음 문자가 존재하며, 현재 문자의 다음 문자의 기호 타입이 피연산자, 실수 표현을 위한 '.'이 아닐 경우
	//현재까지 읽은 문자에 대해 처리를 위하여 읽기 중지 이전에 문자열 좌우반전 수행
	//그 외, 나머지 처리사항은 정방향 역방향 모두 동일

	/***
		< 대상 표현식으로부터 토큰 생성 >

		1) 대상 표현식에 대해 다 읽거나, 현재까지 읽은 문자에 대해 처리를 위하여 읽기 중지가 발생하는 시점 전까지 반복

		2) 현재 읽은 문자의 기호 타입에 따라,

			2-1) 현재 읽은 문자의 기호 타입이 피연산자인 경우
			: 현재 문자의 다음 문자가 존재하며, 현재 문자의 다음 문자의 기호 타입이 피연산자, 실수 표현을 위한 '.'이면 계속 읽기
			현재 문자의 다음 문자가 존재하며, 현재 문자의 다음 문자의 기호 타입이 피연산자, 실수 표현을 위한 '.'이 아닐 경우 현재까지 읽은 문자에 대해 처리를 위하여 읽기 중지
			('.'을 포함하는 실수의 경우 '.'과 피연산자를 포함한 실수를 하나의 피연산자 토큰으로 처리)

			2-2) 현재 읽은 문자의 기호 타입이 실수 표현을 위한 '.'인 경우
			: 현재 문자의 이전, 다음 문자가 존재하며, 현재 문자의 이전, 다음 문자가 피연산자이면 계속 읽기
			현재 문자의 이전, 다음 문자가 존재하지 않거나, 현재 문자의 이전, 다음 문자가 피연산자가 아닐 경우 잘못 된 표현식 예외 발생
			'.'이 이미 이전에 존재하였을 경우 잘못 된 표현식 예외 발생

			2-3) 현재 읽은 문자의 기호 타입이 '(', ')', 연산자, 피연산자 간 구분을 위한 공백인 경우
			: 현재 읽은 문자에 대해 처리를 위하여 읽기 중지

		---

		< '.'을 포함하는 실수의 조건 >

		1) '.'은 '.'을 포함하는 실수에서 맨 처음에 단독으로 존재 할 수 없으며, 어떠한 피연산자가 최소 한 번은 존재 한 다음에만 존재

		2) '.'은 '.'을 포함하는 실수에서 반드시 한 번만 존재

		3) '.'을 포함하는 실수에서 '.'이 존재한 뒤, 피연산자가 최소 한 번은 반드시 존재하여야 하고,
		최소 한 번 이상의 피연산자가 존재 후 더 이상 피연산자가 존재하지 않을 경우,
		이 전체를 하나의 실수로 간주하여 '.' 및 피연산자를 포함 한 하나의 피연산자 토큰으로 처리
	***/

	bool isDotAlreadyExists = false; //실수 표현을 위한 '.' 의 최초 한 번 존재 여부

	while (dstToken->readCount < strlen(srcExpr))
	{
		dstToken->str[dstToken->readCount] = srcExpr[dstToken->readCount]; //왼쪽부터 한 문자씩 처리
		dstToken->symbolType = CharToSymbolType(srcExpr[dstToken->readCount]); //읽은 문자에 대한 기호 타입 할당
		dstToken->readCount++; //읽은 문자 개수 증가

		switch (dstToken->symbolType)
		{
		case SYMBOL_TYPE::OPERAND:
			if (dstToken->readCount < strlen(srcExpr)) //현재 문자의 다음 문자가 존재 할 경우
			{
				switch (CharToSymbolType(srcExpr[dstToken->readCount]))
				{
				case SYMBOL_TYPE::OPERAND:
				case SYMBOL_TYPE::DOT:
					break; //현재 문자의 다음 문자의 기호 타입이 피연산자, 실수 표현을 위한 '.'이면 계속 읽기

				default:
					goto END_PROC; //스택에 삽입 위하여 읽기 중지
				}
			}

			break;

		case SYMBOL_TYPE::DOT:
			if (isDotAlreadyExists ||
				!(dstToken->readCount >= 2 && dstToken->readCount < strlen(srcExpr))) //이미 '.'가 존재하거나, 현재 문자의 이전, 다음 문자가 존재하지 않을 경우
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcExpr)"));

			isDotAlreadyExists = true;

			if (!(CharToSymbolType(dstToken->str[dstToken->readCount - 2]) == SYMBOL_TYPE::OPERAND &&
				CharToSymbolType(srcExpr[dstToken->readCount]) == SYMBOL_TYPE::OPERAND)) //현재 문자의 이전, 다음 문자의 기호 타입이 피연산자가 아닐 경우
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcExpr)"));

			break; //현재 문자의 이전, 다음 문자가 존재하며, 현재 문자의 이전, 다음 문자가 피연산자이면 계속 읽기

		default: //'(', ')', 연산자, 피연산자 간 구분을 위한 공백인 경우
			goto END_PROC; //스택에 삽입 위하여 읽기 중지
		}
	}

END_PROC: //do nothing
	return;
}