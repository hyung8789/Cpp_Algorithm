#include "Core.h"

/// <summary>
/// 대상 피연산자 1 및 대상 피연산자 2에 대해 연산자 문지에 따른 계산 결과 반환
/// </summary>
/// <param name="a">대상 피연산자 1</param>
/// <param name="srcChar">연산자 문자</param>
/// <param name="b">대상 피연산자 2</param>
/// <returns>대상 피연산자 1 및 대상 피연산자 2에 대해 연산자 문자에 따른 계산 결과</returns>
inline double CalcOperation(double a, char srcChar, double b)
{
	return CalcOperation(a, CharToSymbolType(srcChar), b);
}

/// <summary>
/// 대상 피연산자 1 및 대상 피연산자 2에 대해 연산자 기호 타입에 따른 계산 결과 반환
/// </summary>
/// <param name="a">대상 피연산자 1</param>
/// <param name="op">연산자 기호 타입</param>
/// <param name="b">대상 피연산자 2</param>
/// <returns>대상 피연산자 1 및 대상 피연산자 2에 대해 연산자 기호 타입에 따른 계산 결과</returns>
double CalcOperation(double a, SYMBOL_TYPE op, double b)
{
	double retVal = 0.0;
	int feFlags = 0; //Floating Point Exception Flags
	feclearexcept(FE_ALL_EXCEPT); //Clear Exception Flags

	switch (op)
	{
	case SYMBOL_TYPE::PLUS:
		retVal = a + b;
		break;

	case SYMBOL_TYPE::MINUS:
		retVal = a - b;
		break;

	case SYMBOL_TYPE::MULTIPLY:
		retVal = a * b;
		break;

	case SYMBOL_TYPE::DIVIDE:
		retVal = a / b;
		break;

	default:
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));
	}

	feFlags = fetestexcept(FE_ALL_EXCEPT);
	if (feFlags & FE_OVERFLOW)
		throw std::overflow_error(std::string(__func__) + std::string(" : overflow occured"));
	if (feFlags & FE_UNDERFLOW)
		throw std::underflow_error(std::string(__func__) + std::string(" : underflow occured"));
	if (feFlags & FE_DIVBYZERO)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (Div by 0)"));

	return retVal;
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
void GenNextToken(const char* srcExpr, Token* dstToken)
{
	if (srcExpr == NULL || dstToken == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (strlen(srcExpr) > sizeof(dstToken->str) - 1) //'\0' 제외 한 크기를 초과 할 경우
		throw std::out_of_range(std::string(__func__) + std::string(" : out of range"));

	memset(dstToken->str, '\0', sizeof(dstToken->str));
	dstToken->readCount = 0;

	/***
		< 대상 표현식으로부터 토큰 생성 >

		1) 대상 표현식에 대해 다 읽거나, 스택에 삽입 위하여 읽기 중지 시 까지 반복

		2) 현재 읽은 문자의 기호 타입에 따라,

			2-1) 현재 읽은 문자의 기호 타입이 피연산자인 경우
			: 현재 문자의 다음 문자가 존재하며, 현재 문자의 다음 문자의 기호 타입이 피연산자, 실수 표현을 위한 '.'이면 계속 읽기
			현재 문자의 다음 문자가 존재하며, 현재 문자의 다음 문자의 기호 타입이 피연산자, 실수 표현을 위한 '.'이 아닐 경우 스택에 삽입 위하여 읽기 중지
			('.'을 포함하는 실수의 경우 '.'과 피연산자를 포함한 실수를 하나의 피연산자 토큰으로 처리)

			2-2) 현재 읽은 문자의 기호 타입이 실수 표현을 위한 '.'인 경우
			: 현재 문자의 이전, 다음 문자가 존재하며, 현재 문자의 이전, 다음 문자가 피연산자이면 계속 읽기
			현재 문자의 이전, 다음 문자가 존재하지 않거나, 현재 문자의 이전, 다음 문자가 피연산자가 아닐 경우 잘못 된 표현식 예외 발생
			'.'이 이미 이전에 존재하였을 경우 잘못 된 표현식 예외 발생

			2-3) 현재 읽은 문자의 기호 타입이 ('(', ')'), 연산자, 피연산자 간 구분을 위한 공백인 경우
			: 스택에 삽입 혹은 기타 처리를 위하여 읽기 중지
		
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
			if (!isDotAlreadyExists) //아직 '.'가 존재하지 않은 경우
			{
				isDotAlreadyExists = true;

				if (dstToken->readCount >= 2 && dstToken->readCount < strlen(srcExpr)) //현재 문자의 이전, 다음 문자가 존재 할 경우
				{
					SYMBOL_TYPE prevSymbolType = CharToSymbolType(dstToken->str[dstToken->readCount - 2]); //현재 문자의 이전 기호 문자
					SYMBOL_TYPE nextSymbolType = CharToSymbolType(srcExpr[dstToken->readCount]); //현재 문자의 다음 기호 문자

					if (!(prevSymbolType == SYMBOL_TYPE::OPERAND && nextSymbolType == SYMBOL_TYPE::OPERAND)) //현재 문자의 이전, 다음 문자의 기호 타입이 피연산자가 아닐 경우
						throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcExpr)"));
				}
				else
				{
					throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcExpr)"));
				}
			}
			else //이미 '.'가 존재 할 경우
			{
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcExpr)"));
			}
			break;

		default: //'(', ')', 연산자, 피연산자 간 구분을 위한 공백인 경우
			goto END_PROC; //스택에 삽입 위하여 읽기 중지
		}
	}

END_PROC: //do nothing
	return;
}

/// <summary>
/// 대상 중위 표기식으로부터 후위 표기식 생성
/// </summary>
/// <param name="srcInfixExpr">대상 중위 표기식</param>
/// <param name="dstPostfixExpr">대상 중위 표기식으로부터 변환되어 출력 될 후위 표기식</param>
void GenPostfixExpr(const char* srcInfixExpr, char* dstPostfixExpr)
{
	/***
		< 중위 표기식을 후위 표기식으로 변환 >

		! 중위 표기식에서 연산 우선순위를 위한 괄호를 최우선적으로 처리하여야 함
		! '(' 및 ')'는 변환 된 후위 표기식의 결과로서 출력하지 않음

		1) 사용자로부터 중위 표기식 입력

		2) 중위 표기식의 왼쪽부터 순차적으로 토큰 분리 (괄호, 피연산자 간 구분을 위한 공백, 피연산자, 연산자)
		: 토큰 분리 시 '.'을 포함하는 실수의 경우 '.'과 피연산자를 포함한 실수를 하나의 피연산자 기호 타입의 토큰으로 처리

		3) 현재 분리 된 토큰의 기호 타입에 따라,

			3-1) '(' 인 경우
			: 현재 분리 된 토큰의 기호를 스택에 삽입

			3-2) ')' 인 경우
			: 현재 스택에서 '(' 가 맨 처음으로 나올 때 까지 노드의 기호를 꺼내어 순차적으로 변환 된 후위 표기식의 결과로서 출력
			('(' 가 한 번 나오지 않을 경우 잘못 된 중위 표기식 예외 발생)

			3-3) 연산자 ('+', '-', '*', '/') 인 경우
			: 우선순위가 높은 연산자에 대해 먼저 계산을 위해, 후위 표기식의 결과로서 출력 위한 연산자 우선순위 판별 수행
			더 이상 스택에 현재 토큰의 기호 타입보다 높은 우선순위를 가진 기호 타입이 존재하지 않게 되는 시점에 현재 분리 된 토큰의 기호를 스택에 삽입

				3-3-1) 현재 스택의 최상위 노드의 기호 타입에 대한 우선순위 >= 현재 분리 된 토큰의 기호 타입에 대한 우선순위
				: 우선순위가 높은 연산자에 대해 먼저 계산을 위해, 현재 스택의 최상위 노드를 스택에서 꺼내 해당 기호를 변환 된 후위 표기식의 결과로서 출력
				(단, 현재 스택의 최상의 노드의 기호가 '(' 일 경우, 변환 된 후위 표기식의 결과로서 출력하지 않음)

				3-3-2) 현재 스택의 최상위 노드의 기호 타입에 대한 우선순위 < 현재 분리 된 토큰의 기호 타입에 대한 우선순위
				: 우선순위가 높은 기호에 대해 먼저 계산을 위해 후위 표기식의 결과로서 출력 위한 판별 중지

			3-4) 피연산자 (정수 혹은 '.'을 포함 한 실수), ' ' (피연산자 간 구분을 위한 공백) 인 경우
			: 현재 분리 된 토큰의 기호를 스택에 삽입하지 않고 즉시 변환 된 후위 표기식의 결과로서 출력

			3-5) '.' (실수 표현을 위한 점) 인 경우
			: '.'은 단일 토큰으로서 처리 되지 않으며, '.'을 포함 한 실수로서 하나의 피연산자로 처리되어야만 함
			이에 따라, 논리 예외 발생

		4) 현재 중위 표기식에 더 이상 토큰으로 분리 할 것이 없으면,
		스택에 남은 노드의 기호들을 순차적으로 모두 변환 된 후위 표기식의 결과로서 출력
	***/

	if (srcInfixExpr == NULL || dstPostfixExpr == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	LinkedListStack* stack = NULL;

	unsigned int currentReadPos = 0; //현재까지 읽은 위치
	unsigned int infixExprLength = strlen(srcInfixExpr); //중위 표기식 길이
	bool isValidExpr = false; //유효한 표현식 여부
	Token token;

	LLS_CreateStack(&stack);

	while (currentReadPos < infixExprLength) //중위 표기식을 다 읽을 때까지
	{
		GenNextToken(&srcInfixExpr[currentReadPos], &token); //현재까지 읽은 위치부터 토큰 생성

		switch (token.symbolType)
		{
		case SYMBOL_TYPE::LEFT_PARENTHESIS:
			LLS_Push(&stack, LLS_CreateNode(token.str)); //현재 분리 된 토큰의 기호를 스택에 삽입
			break;

		case SYMBOL_TYPE::RIGHT_PARENTHESIS:
			isValidExpr = false;

			while (!LLS_IsEmpty(&stack)) //현재 스택에서 '(' 가 나올 때 까지 노드의 기호를 꺼내어 순차적으로 변환 된 후위 표기식의 결과로서 출력
			{
				Node* poppedNode = LLS_Pop(&stack);

				if (CharToSymbolType(poppedNode->data[0]) == SYMBOL_TYPE::LEFT_PARENTHESIS) //'(' 를 만날 경우
				{
					isValidExpr = true;
					LLS_DeallocateNode(&poppedNode);
					break;
				}
				else //피연산자, 연산자인 경우
				{
					if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, poppedNode->data) != 0) //후위 표기식에 출력
						throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

					LLS_DeallocateNode(&poppedNode);
				}
			}

			if (!isValidExpr) //'(' 가 한 번 나오지 않을 경우
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong InfixExpr)"));
			break;

		case SYMBOL_TYPE::OPERAND:
			if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, token.str) != 0) //현재 분리 된 토큰의 기호를 후위 표기식에 출력
				throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));
		case SYMBOL_TYPE::SPACE:
			if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, " ") != 0) //피연산자 간 구분을 위한 공백을 후위 표기식에 출력
				throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));
			break;

		case SYMBOL_TYPE::DOT:
			throw std::logic_error(std::string(__func__) + std::string(" : '.' is part of operand"));

		default: //연산자인 경우
			while (!LLS_IsEmpty(&stack)) //우선순위가 높은 연산자에 대해 먼저 계산을 위해, 후위 표기식의 결과로서 출력 위한 연산자 우선순위 판별
			{
				SYMBOL_TYPE peekedNodeSymbolType = CharToSymbolType(LLS_Peek(&stack)->data[0]); //현재 스택의 최상위 노드의 연산자에 대한 기호 타입

				if (GetSymbolTypePriority(peekedNodeSymbolType) >=
					GetSymbolTypePriority(token.symbolType)) //현재 스택의 최상위 노드의 기호 타입에 대한 우선순위 >= 현재 분리 된 토큰의 기호 타입에 대한 우선순위
				{
					Node* poppedNode = LLS_Pop(&stack);

					if (peekedNodeSymbolType != SYMBOL_TYPE::LEFT_PARENTHESIS) //'(' 가 아닌 경우에만 우선순위가 높은 연산자에 대해 먼저 계산을 위해 후위 표기식으로 출력
					{
						if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, poppedNode->data) != 0)
							throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));
					}

					LLS_DeallocateNode(&poppedNode);
				}
				else //현재 스택의 최상위 노드의 기호 타입에 대한 우선순위 < 현재 분리 된 토큰의 기호 타입에 대한 우선순위
				{
					break;
				}
			}

			LLS_Push(&stack, LLS_CreateNode(token.str)); //현재 분리 된 토큰의 기호를 스택에 삽입
			break;
		}

		currentReadPos += token.readCount; //다음에 읽을 위치부터 다시 토큰 생성
	}

	while (!LLS_IsEmpty(&stack)) //스택에 남은 노드의 기호들을 순차적으로 모두 변환 된 후위 표기식의 결과로서 출력
	{
		Node* poppedNode = LLS_Pop(&stack);

		if (CharToSymbolType(poppedNode->data[0]) != SYMBOL_TYPE::LEFT_PARENTHESIS) //'(' 가 아닌 경우
			if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, poppedNode->data) != 0) //후위 표기식에 출력
				throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

		LLS_DeallocateNode(&poppedNode);
	}

	LLS_DeallocateLinkedListStack(&stack);
}

/// <summary>
/// 대상 후위 표기식으로부터 계산 결과 반환
/// </summary>
/// <param name="srcPostfixExpr">대상 후위 표기식</param>
/// <returns>대상 후위 표기식에 대한 계산 결과</returns>
double CalcPostfixExpr(const char* srcPostfixExpr)
{
	/***
		< 후위 표기식에 대한 처리 및 계산 >

		! 후위 표기식 : 중위 표기식으로부터 '(', ')' 및 연산자 우선순위에 따라 변환
		! '.'을 포함하는 실수의 경우 '.'과 피연산자를 포함한 실수를 하나의 피연산자 토큰으로 처리하였으므로, 2-2에서 처리

		ex :
		중위 표기식 : ((1 - 2 * 2) + 1) * 3
		변환 된 후위 표기식 : 122*-1+3*

		1) 왼쪽부터 순차적으로 토큰 분리 (피연산자 간 구분을 위한 공백, 피연산자, 연산자)

		2) 분리 된 토큰의 기호 타입에 따라,
			2-1) 피연산자 간 구분을 위한 공백인 경우
			: 계산을 위하여 해당 토큰 무시

			2-2) 피연산자인 경우
			: 토큰을 스택에 삽입

			2-3) 연산자인 경우
			: 스택에 존재하는 피연산자를 2회 꺼낸 후 피연산자에 대해 역순으로 현재 토큰의 연산자와 계산 수행 및 계산 결과를 다시 스택에 삽입

		3) 후위 표기식을 끝까지 읽었을 경우 현재 스택에 남아있는 최종 계산 결과 반환
	***/

	//TODO : 중위 표현식에 대한 유효성 검사를 단순화 하기 위해 변환 된 후위 표현식을 통한 유효성 검사 수행
	// (ex : 1*+2 입력 시 스택에서 2회 제거 할 수 없는 경우)
	//중위 표현식에서 유효성 검사를 수행할 경우 파스 트리를 작성해야 함

	if (srcPostfixExpr == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	LinkedListStack* stack = NULL;

	unsigned int currentReadPos = 0; //현재까지 읽은 위치
	unsigned int postfixExprLength = strlen(srcPostfixExpr); //후위 표기식 길이
	Token token;

	double retVal = 0.0; //최종 계산 결과

	LLS_CreateStack(&stack);

	while (currentReadPos < postfixExprLength) //후위 표기식을 다 읽을 때까지
	{
		GenNextToken(&srcPostfixExpr[currentReadPos], &token); //현재까지 읽은 위치부터 토큰 생성

		switch (token.symbolType)
		{
		case SYMBOL_TYPE::LEFT_PARENTHESIS:
		case SYMBOL_TYPE::RIGHT_PARENTHESIS:
		case SYMBOL_TYPE::DOT:
			throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcPostfixExpr)"));

		case SYMBOL_TYPE::SPACE:
			break;

		case SYMBOL_TYPE::OPERAND:
			LLS_Push(&stack, LLS_CreateNode(token.str));
			break;

		default: //연산자인 경우
			Node* operandNode1 = LLS_Pop(&stack); //연산자 뒤에 들어 갈 피연산자
			Node* operandNode2 = LLS_Pop(&stack); //연산자 앞에 들어 갈 피연산자
			double tmpResult = CalcOperation(atof(operandNode2->data), token.symbolType, atof(operandNode1->data)); //중간 계산 결과
			char tmpResultBuffer[_CVTBUFSIZE] = { '\0', }; //중간 계산 결과 변환 위한 버퍼

			_gcvt_s(tmpResultBuffer, _CVTBUFSIZE, tmpResult, 10); //부동 실수점을 문자열로 변환
			LLS_Push(&stack, LLS_CreateNode(tmpResultBuffer));

			LLS_DeallocateNode(&operandNode1);
			LLS_DeallocateNode(&operandNode2);
			break;
		}

		currentReadPos += token.readCount; //다음에 읽을 위치부터 다시 토큰 생성
	}

	Node* resultNode = LLS_Pop(&stack);
	retVal = atof(resultNode->data);
	LLS_DeallocateNode(&resultNode);
	LLS_DeallocateLinkedListStack(&stack);

	return retVal;
}