#include "Core.h"

/// <summary>
/// 대상 문자열 제자리 좌우반전
/// </summary>
/// <param name="targetStr">대상 문자열</param>
void ReverseInplaceStr(char targetStr[])
{
	if (targetStr == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	size_t targetStrLen = strlen(targetStr); //대상 문자열의 길이 ('\0' 제외 한 길이)
	if (targetStrLen == 1) //do nothing
		return;

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
			srcCharDecAscii <= SingleNumToDecAscii(9))) //0 (dec ascii : 48) ~ 9 (dec ascii : 57) 가 아닐 경우
			throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

		return SYMBOL_TYPE::OPERAND;
	}

	return (SYMBOL_TYPE)srcChar;
}

/// <summary>
/// 대상 문자열을 기호 타입으로 변환
/// </summary>
/// <param name="srcStr">대상 문자열</param>
/// <returns>대상 문자열의 기호 타입</returns>
SYMBOL_TYPE StrToSymbolType(const char* srcStr)
{
	if (srcStr == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	size_t srcStrLen = strlen(srcStr); //대상 문자열의 길이 ('\0' 제외 한 길이)

	if (srcStrLen == 1) //단일 문자인 경우
		return CharToSymbolType((const char)srcStr[0]); //단일 문자에 대한 기호 타입 변환 결과 반환

	/***
		< 단일 문자가 아닌 피연산자 (한 자리를 초과하는 정수 혹은 '.'을 포함하는 실수) 판별 >

		1) GenNextToken 의 '.'을 포함하는 실수의 조건을 따름
	***/

	bool isDotAlreadyExists = false; //실수 표현을 위한 '.' 의 최초 한 번 존재 여부

	for (size_t i = 0; i < srcStrLen; i++)
	{
		switch (CharToDecAscii((const char)srcStr[i])) //대상 문자열의 각 자리의 단일 문자에 대한 10진 아스키 코드에 따라
		{
		case (const int)SYMBOL_TYPE::DOT:
			if (isDotAlreadyExists ||
				!(i >= 1 && (i + 1) < srcStrLen)) //이미 '.'가 존재하거나, 현재 문자의 이전, 다음 문자가 존재하지 않을 경우
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args ('.' already exists or invalid floating point)"));

			isDotAlreadyExists = true;
			break;

		default:
			if (!(CharToDecAscii((const char)srcStr[i]) >= SingleNumToDecAscii(0) &&
				CharToDecAscii((const char)srcStr[i]) <= SingleNumToDecAscii(9))) //0 (dec ascii : 48) ~ 9 (dec ascii : 57) 가 아닐 경우
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));
			break;
		}
	}

	return SYMBOL_TYPE::OPERAND;
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
/// <param name="srcExpr">대상 표현식 (전체 문자열 혹은 Slice 된 문자열)</param>
/// <param name="dstToken">대상 표현식으로부터 생성되어 출력 될 토큰</param>
/// <param name="exprReadDirection">대상 표현식 읽는 방향</param>
void GenNextToken(const char* srcExpr, Token* dstToken, EXPR_READ_DIRECTION exprReadDirection)
{
	if (srcExpr == NULL || dstToken == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (strlen(srcExpr) > sizeof(dstToken->str) - 1) //대상 표현식이 출력 버퍼의 '\0' 제외 한 크기를 초과 할 경우
		throw std::out_of_range(std::string(__func__) + std::string(" : out of range"));

	memset(dstToken->str, '\0', sizeof(dstToken->str));
	dstToken->readCount = 0;
	dstToken->ignoredCount = 0;

	/***
		< '.'을 포함하는 실수의 조건 >

		1) '.'은 '.'을 포함하는 실수에서 맨 처음에 단독으로 존재 할 수 없으며,
		어떠한 피연산자가 최소 한 번은 존재 한 다음에만 존재

		2) '.'은 '.'을 포함하는 실수에서 반드시 한 번만 존재

		3) '.'을 포함하는 실수에서 '.'이 존재한 뒤, 피연산자가 최소 한 번은 반드시 존재하여야 하고,
		최소 한 번 이상의 피연산자가 존재 후 더 이상 피연산자가 존재하지 않을 경우,
		이 전체를 하나의 실수로 간주하여 '.' 및 피연산자를 포함 한 하나의 피연산자 토큰으로 처리

		---

		< 대상 표현식으로부터 토큰 생성 >

		! '.'을 포함하는 실수의 경우 '.'과 피연산자를 포함한 실수를 하나의 피연산자 토큰으로 처리

		1) 대상 표현식에 대해 다 읽거나, 현재까지 읽은 문자에 대해 처리를 위하여 읽기 중지가 발생하는 시점까지 반복

		2) 현재 읽은 문자의 기호 타입에 따라,

			2-1) 현재 읽은 문자의 기호 타입이 피연산자인 경우
				2-1-1) 현재 문자의 다음 문자가 존재하며, 현재 문자의 다음 문자의 기호 타입이 피연산자, 실수 표현을 위한 '.'인 경우
				: 계속 읽기

				2-1-2) 현재 문자의 다음 문자가 존재하며, 현재 문자의 다음 문자의 기호 타입이 피연산자, 실수 표현을 위한 '.'이 아닐 경우
				: 현재까지 읽은 문자에 대해 처리를 위하여 읽기 중지 (오른쪽에서 왼쪽으로 읽었을 경우, 현재까지 읽은 문자 좌우반전)

			2-2) 현재 읽은 문자의 기호 타입이 실수 표현을 위한 '.'인 경우 (이미 '.'가 존재하거나, 현재 문자의 이전, 다음 문자가 존재하지 않을 경우 잘못 된 표현식 예외 발생)
				2-2-1) 현재 문자의 이전, 다음 문자가 존재하며, 현재 문자의 이전, 다음 문자가 피연산자이면
				: 계속 읽기

				2-2-2) 현재 문자의 이전, 다음 문자가 존재하지 않거나, 현재 문자의 이전, 다음 문자가 피연산자가 아닐 경우
				: 잘못 된 표현식 예외 발생

			2-3) 현재 읽은 문자의 기호 타입이 ' ' (피연산자 간 구분을 위한 공백)인 경우
			: 현재 읽은 문자 무시를 위하여 현재 읽은 문자 제거, 읽은 문자 개수 감소 및 무시 된 문자 개수 증가 
			현재 읽은 문자의 다음 문자부터 계속 읽기

			2-4) 현재 읽은 문자의 기호 타입이 '(', ')', 연산자인 경우
			: 현재 읽은 문자에 대해 처리를 위하여 읽기 중지
	***/

	size_t srcExprLen = strlen(srcExpr); //대상 표현식의 길이 ('\0' 제외 한 길이)
	size_t srcExprNextReadIndex =
		(exprReadDirection == EXPR_READ_DIRECTION::LEFT_TO_RIGHT) ? 0 : //읽을 위치 : 대상 문자열의 시작
		(exprReadDirection == EXPR_READ_DIRECTION::RIGHT_TO_LEFT) ? (srcExprLen - 1) : //읽을 위치 : 대상 문자열의 끝
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args")); //대상 표현식의 다음에 읽을 인덱스
	bool isDotAlreadyExists = false; //실수 표현을 위한 '.' 의 최초 한 번 존재 여부

	while ((dstToken->readCount + dstToken->ignoredCount) < srcExprLen) //대상 표현식을 다 읽을 때 까지
	{
		dstToken->str[dstToken->readCount] = srcExpr[srcExprNextReadIndex]; //한 문자씩 처리
		dstToken->symbolType = CharToSymbolType(srcExpr[srcExprNextReadIndex]); //읽은 문자에 대한 기호 타입 할당
		dstToken->readCount++; //읽은 문자 개수 증가

		srcExprNextReadIndex =
			(exprReadDirection == EXPR_READ_DIRECTION::LEFT_TO_RIGHT) ? srcExprNextReadIndex + 1 :
			(exprReadDirection == EXPR_READ_DIRECTION::RIGHT_TO_LEFT) ? srcExprNextReadIndex - 1 :
			throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args")); //대상 표현식의 다음에 읽을 위치 할당

		//TODO : reverse 토큰 생성 간 잘못 된 기호 타입 오류
		/*
		* tmp sliced string : 7 1 * 5 2 - /
Current Token Str (Right -> Left) : /
tmp sliced string : 7 1 * 5 2 - 
Current Token Str (Right -> Left) : -
tmp sliced string : 7 1 * 5 2 
Current Token Str (Right -> Left) : 2
tmp sliced string : 7 1 * 5 
Current Token Str (Right -> Left) : 5
tmp sliced string : 7 1 * 
Current Token Str (Right -> Left) : *
tmp sliced string : 7 1 
Current Token Str (Right -> Left) : 1
tmp sliced string : 7 
CharToSymbolType : Invalid Args
		*/


		switch (dstToken->symbolType)
		{
		case SYMBOL_TYPE::OPERAND:
			if (dstToken->readCount < srcExprLen) //현재 문자의 다음 문자가 존재 할 경우
			{
				switch (CharToSymbolType(srcExpr[srcExprNextReadIndex]))
				{
				case SYMBOL_TYPE::OPERAND:
				case SYMBOL_TYPE::DOT:
					break; //현재 문자의 다음 문자의 기호 타입이 피연산자, 실수 표현을 위한 '.'이면 계속 읽기

				default: //현재 문자의 다음 문자의 기호 타입이 '(', ')', 연산자, 피연산자 간 구분을 위한 공백인 경우
					if (exprReadDirection == EXPR_READ_DIRECTION::RIGHT_TO_LEFT) //오른쪽에서 왼쪽으로 읽었을 경우
						ReverseInplaceStr(dstToken->str); //현재까지 읽은 문자열에 대해 좌우반전

					goto END_PROC; //현재까지 읽은 문자에 대해 처리를 위하여 읽기 중지
				}
			}

			break;

		case SYMBOL_TYPE::DOT:
			if (isDotAlreadyExists ||
				!(dstToken->readCount >= 2 && dstToken->readCount < srcExprLen)) //이미 '.'가 존재하거나, 현재 문자의 이전, 다음 문자가 존재하지 않을 경우
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcExpr)"));

			isDotAlreadyExists = true;

			if (!(CharToSymbolType(dstToken->str[dstToken->readCount - 2]) == SYMBOL_TYPE::OPERAND &&
				CharToSymbolType(srcExpr[srcExprNextReadIndex]) == SYMBOL_TYPE::OPERAND)) //현재 문자의 이전, 다음 문자의 기호 타입이 피연산자가 아닐 경우
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcExpr)"));

			break; //현재 문자의 이전, 다음 문자가 존재하며, 현재 문자의 이전, 다음 문자가 피연산자이면 계속 읽기

		case SYMBOL_TYPE::SPACE:
			dstToken->str[--(dstToken->readCount)] = '\0'; //현재 읽은 문자 제거 및 읽은 문자 개수 감소
			dstToken->ignoredCount++; //무시 된 문자 개수 증가
			continue; //현재 읽은 문자 무시 및 다음 문자 읽기

		default: //'(', ')', 연산자인 경우
			goto END_PROC; //현재 읽은 문자에 대해 처리를 위하여 읽기 중지
		}
	}

END_PROC: //do nothing
	return;
}