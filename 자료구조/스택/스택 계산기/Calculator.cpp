#include "Core.h"

/// <summary>
/// 대상 피연산자 1 및 대상 피연산자 2에 대해 연산자 문지에 따른 계산 결과 반환
/// </summary>
/// <param name="a">대상 피연산자 1</param>
/// <param name="srcChar">연산자 문자</param>
/// <param name="b">대상 피연산자 2</param>
/// <returns>대상 피연산자 1 및 대상 피연산자 2에 대해 연산자 문자에 따른 계산 결과</returns>
inline double CalcOperation(double a, char srcChar, double b) throw(std::invalid_argument, std::overflow_error, std::underflow_error)
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
double CalcOperation(double a, SYMBOL_TYPE op, double b) throw(std::invalid_argument, std::overflow_error, std::underflow_error)
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
inline int SingleNumToDecAscii(int srcSingleNum) throw(std::invalid_argument)
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
SYMBOL_TYPE CharToSymbolType(char srcChar) throw(std::invalid_argument)
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
/// <param name="isInStack">대상 기호 타입의 스택 상에 존재 여부 ('(' 전용)</param>
/// <returns>우선순위 (클 수록 높은 연산 우선순위)</returns>
int GetSymbolTypePriority(SYMBOL_TYPE srcSymbol, bool isInStack)
{
	int priority = -1; //우선순위 (클 수록 높은 연산 우선순위)

	switch (srcSymbol)
	{
	case SYMBOL_TYPE::LEFT_PARENTHESIS:
		/***
			기존 스택에 '('가 존재하는 상황에, 새로운 '('가 입력 되었을 경우, 
			새로운 '('가 기존 '(' 보다 더 높은 우선순위를 가진다.
			'('가 기존 스택에 존재하지 않을 경우, 다른 어떠한 기호 타입보다도 최우선적인 우선순위를 가진다.
		***/

		if (isInStack) //기존 스택에 존재하는 '('일 경우
			priority = 2;
		else
			priority = 3;
		break;

	case SYMBOL_TYPE::PLUS:
	case SYMBOL_TYPE::MINUS:
		priority = 0;
		break;

	case SYMBOL_TYPE::MULTIPLY:
	case SYMBOL_TYPE::DIVIDE:
		priority = 1;
		break;

	default: //')', 피연산자, 피연산자 간 구분을 위한 공백, 소수 표현을 위한 점
		/***
			')', 피연산자, 피연산자 간 구분을 위한 공백, 소수 표현을 위한 점은 우선순위를 관리하지 않음 (즉시 처리를 위하여 최하위 우선순위)
			---
			- 즉시 처리 : 해당 기호 타입에 대해 우선순위 간 비교를 수행하지 않고, 후위 표기식으로 즉시 출력하거나, 무시 혹은 기타 처리 수행
		***/

		break;
	}

	return priority;
}

/// <summary>
/// 대상 표현식으로부터 토큰 생성
/// </summary>
/// <param name="srcExpr">대상 표현식</param>
/// <param name="dstToken">대상 표현식으로부터 생성되어 출력 될 토큰</param>
void GenNextToken(const char* srcExpr, Token* dstToken) throw(std::invalid_argument, std::out_of_range)
{
	if (srcExpr == NULL || dstToken == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (strlen(srcExpr) > sizeof(dstToken->str) - 1) //'\0' 제외 한 크기를 초과 할 경우
		throw std::out_of_range(std::string(__func__) + std::string(" : out of range"));

	memset(dstToken->str, '\0', sizeof(dstToken->str));
	dstToken->readCount = 0;

	/***
		1) 대상 표현식에 대해 다 읽거나, 스택에 삽입 위하여 읽기 중지 시 까지 반복

		2) 현재 읽은 문자의 기호 타입이 피연산자 혹은 소수 표현을 위한 점인 경우,
		현재 문자의 다음 문자가 존재하며 현재 문자의 다음 문자의 기호 타입이 피연산자가 아니면 스택에 삽입 위하여 읽기 중지
		('.'을 포함하는 소수의 경우 '.'과 피연산자를 포함한 소수를 하나의 피연산자 토큰으로 처리)

		3) 현재 읽은 문자의 기호 타입이 피연산자가 아닌 괄호, 피연산자 간 구분을 위한 공백, 연산자인 경우, 스택에 삽입 위하여 읽기 중지
		---
		< '.'을 포함하는 소수에 대한 토큰 처리 >

		1) '.'은 '.'을 포함하는 소수에서 맨 처음에 단독으로 존재 할 수 없으며, 어떠한 피연산자가 존재 한 다음에만 존재해야만 한다.

		2) '.'은 '.'을 포함하는 소수에서 한 번만 존재한다.

		3) '.'을 포함하는 소수에서 '.'이 존재한 뒤, 피연산자가 최소 한 번은 존재해야 한다.

		4) '.'을 포함하는 소수에서 '.'이 존재한 뒤, 피연산자가 최소 한 번은 존재하고 그 다음에 피연산자가 존재하지 않을 경우,
		이를 하나의 소수로 간주하여 '.' 및 피연산자를 포함 한 하나의 피연산자 토큰으로 처리한다.
		---
		=> 현재 읽은 문자가 '.' 인 경우, 아직 '.'가 존재하지 않았으면, '.'가 최초 한 번 존재했음을 알리며, 
		현재 읽은 문자의 이전, 다음의 한 문자를 확인하여 피연산자가 아닐 경우 예외 발생
	***/

	bool isDotAlreadyExists = false; //소수 표현을 위한 '.' 의 최초 한 번 존재 여부

	while (dstToken->readCount < strlen(srcExpr))
	{
		dstToken->str[dstToken->readCount] = srcExpr[dstToken->readCount]; //왼쪽부터 한 문자씩 처리
		dstToken->symbolType = CharToSymbolType(srcExpr[dstToken->readCount]); //읽은 문자에 대한 기호 타입 할당
		dstToken->readCount++; //읽은 문자 개수 증가

		switch (dstToken->symbolType)
		{
		case SYMBOL_TYPE::DOT:
			if (!isDotAlreadyExists) //아직 '.'가 존재하지 않은 경우
			{
				isDotAlreadyExists = true;
			
				if (dstToken->readCount >= 2 && dstToken->readCount < strlen(srcExpr)) //현재 문자의 이전, 다음 문자가 존재 할 경우
				{
					SYMBOL_TYPE prevSymbolType = CharToSymbolType(dstToken->str[dstToken->readCount - 2]); //현재 문자의 이전 기호 문자
					SYMBOL_TYPE nextSymbolType = CharToSymbolType(srcExpr[dstToken->readCount]); //현재 문자의 다음 기호 문자

					if(!(prevSymbolType == SYMBOL_TYPE::OPERAND && nextSymbolType == SYMBOL_TYPE::OPERAND)) //현재 문자의 이전, 다음 문자의 기호 타입이 피연산자가 아닐 경우
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
		
		case SYMBOL_TYPE::OPERAND:
			if (dstToken->readCount < strlen(srcExpr)) ////현재 문자의 다음 문자가 존재 할 경우
			{
				switch (CharToSymbolType(srcExpr[dstToken->readCount]))
				{
				case SYMBOL_TYPE::OPERAND:
				case SYMBOL_TYPE::DOT:
					break;

				default: //스택에 삽입 위하여 읽기 중지
					goto END_PROC;
				}
			}


			//if (dstToken->readCount < strlen(srcExpr) &&
			//	CharToSymbolType(srcExpr[dstToken->readCount]) != (SYMBOL_TYPE::OPERAND | SYMBOL_TYPE::DOT)) 며, 현재 문자의 다음 문자의 기호 타입이 피연산자 혹은 점이 아닐 경우
			//	goto END_PROC; //스택에 삽입 위하여 읽기 중지
			break;

		default: //현재 문자의 기호 타입이 피연산자 간 구분을 위한 공백, 괄호, 연산자인 경우
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
void GenPostfixExpr(const char* srcInfixExpr, char* dstPostfixExpr) throw(std::invalid_argument, std::runtime_error, std::logic_error)
{
	/***
		< 중위 표기식을 후위 표기식으로 변환 >

		! 중위 표기식에서 연산 우선순위를 위한 괄호를 최우선적으로 처리하여야 함
		! '(' 및 ')'는 변환 된 후위 표기식의 결과로서 출력하지 않음

		1) 사용자로부터 중위 표기식 입력

		2) 중위 표기식의 왼쪽부터 순차적으로 토큰 분리 (괄호, 피연산자 간 구분을 위한 공백, 피연산자, 연산자)
		: 토큰 분리 시 '.'을 포함하는 소수의 경우 '.'과 피연산자를 포함한 소수를 하나의 피연산자 기호 타입의 토큰으로 처리

		3) 분리 된 토큰의 기호 타입에 따라,
			3-1) 피연산자, 피연산자 간 구분을 위한 공백인 경우
			: 스택에 삽입하지 않고 변환 된 후위 표기식의 결과로서 출력

			3-2) '(' 혹은 연산자인 경우
			: 우선순위가 높은 기호에 대해 먼저 계산을 위해, 후위 표기식의 결과로서 출력 위한 판별 수행
			현재 스택의 최상위 노드의 기호 타입에 대한 우선순위 < 현재 분리 된 토큰의 기호 타입에 대한 우선순위 혹은 스택이 빌 때까지,
			현재 스택의 최상위 노드의 기호 타입에 대한 우선순위와 현재 분리 된 토큰의 기호 타입에 대한 우선순위를 비교

			ex : ((1 - 2 * 2) + 1) * 3
			=> 스택에 '('이 존재하고, 다시 '('에 대한 처리 : 기존 스택에 존재하는 '('의 우선순위 < 새로운 '('의 우선순위 => 3-2-2

				3-2-1) 현재 스택의 최상위 노드의 기호 타입에 대한 우선순위 >= 현재 분리 된 토큰의 기호 타입에 대한 우선순위
				: 현재 스택의 최상위 노드의 기호가 먼저 계산되어야 하므로,
				현재 스택의 최상위 노드를 스택에서 꺼내 해당 기호를 변환 된 후위 표기식의 결과로서 출력,
				현재 분리 된 토큰의 기호를 스택에 삽입

				3-2-2) 현재 스택의 최상위 노드의 기호 타입에 대한 우선순위 < 현재 분리 된 토큰의 기호 타입에 대한 우선순위
				: 현재 분리 된 토큰의 기호를 스택에 삽입,
				우선순위가 높은 기호에 대해 먼저 계산을 위해 후위 표기식의 결과로서 출력 위한 판별 중지

			3-3) ')' 인 경우
			: 현재 스택에서 '(' 가 나올 때 까지 노드의 기호를 꺼내어 순차적으로 변환 된 후위 표기식의 결과로서 출력

		4) 스택에 남은 노드의 기호들을 순차적으로 모두 변환 된 후위 표기식의 결과로서 출력
	***/

	if (srcInfixExpr == NULL || dstPostfixExpr == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	LinkedListStack* stack = NULL;

	unsigned int currentReadPos = 0; //현재까지 읽은 위치
	unsigned int infixExprLength = strlen(srcInfixExpr); //중위 표기식 길이
	Token token;

	LLS_CreateStack(&stack);

	while (currentReadPos < infixExprLength) //중위 표기식을 다 읽을 때까지
	{
		GenNextToken(&srcInfixExpr[currentReadPos], &token); //현재까지 읽은 위치부터 토큰 생성

		switch (token.symbolType)
		{
		case SYMBOL_TYPE::DOT:
			throw std::logic_error(std::string(__func__) + std::string(" : dot is part of operand"));

		case SYMBOL_TYPE::OPERAND:
			if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, token.str) != 0) //후위 표기식에 출력
				throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

		case SYMBOL_TYPE::SPACE:
			if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, " ") != 0) //피연산자 간 구분을 위한 공백을 후위 표기식에 출력
				throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));
			break;

		case SYMBOL_TYPE::RIGHT_PARENTHESIS:
			while (!LLS_IsEmpty(&stack)) //현재 스택에서 '(' 가 나올 때 까지 노드의 기호를 꺼내어 순차적으로 변환 된 후위 표기식의 결과로서 출력
			{
				Node* poppedNode = LLS_Pop(&stack);

				if (CharToSymbolType(poppedNode->data[0]) == SYMBOL_TYPE::LEFT_PARENTHESIS) //'(' 를 만날 경우
				{
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
			break;

		default: //'(', 연산자인 경우
			while (!LLS_IsEmpty(&stack))
			{
				SYMBOL_TYPE peekedNodeSymbolType = CharToSymbolType(LLS_Peek(&stack)->data[0]); //현재 스택의 최상위 노드의 연산자에 대한 기호 타입

				if (GetSymbolTypePriority(peekedNodeSymbolType, true) >=
					GetSymbolTypePriority(token.symbolType, false)) //현재 스택의 최상위 노드의 기호 타입에 대한 우선순위 >= 현재 분리 된 토큰의 기호 타입에 대한 우선순위
				{
					Node* poppedNode = LLS_Pop(&stack);

					if (peekedNodeSymbolType != SYMBOL_TYPE::LEFT_PARENTHESIS) //'(' 가 아닌 경우
					{
						if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, poppedNode->data) != 0) //현재 스택의 최상위 노드의 기호가 먼저 계산되어야 하므로, 후위 표기식에 출력
							throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));
					}

					LLS_DeallocateNode(&poppedNode);
				}
				else //현재 스택의 최상위 노드의 기호 타입에 대한 우선순위 < 현재 분리 된 토큰의 기호 타입에 대한 우선순위
				{
					break;
				}
			}

			LLS_Push(&stack, LLS_CreateNode(token.str)); //토큰을 스택에 삽입
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

	//TODO : 리스트에도 예외 처리 추가
	//TODO : 스택 계산기에 대한 단위 테스트 작성
}

/// <summary>
/// 대상 후위 표기식으로부터 계산 결과 반환
/// </summary>
/// <param name="srcPostfixExpr">대상 후위 표기식</param>
/// <returns>대상 후위 표기식에 대한 계산 결과</returns>
double CalcPostfixExpr(const char* srcPostfixExpr) throw(std::invalid_argument)
{
	/***
		< 후위 표기식에 대한 처리 및 계산 >

		! 후위 표기식 : 중위 표기식으로부터 '(', ')' 및 연산자 우선순위에 따라 변환
		! '.'을 포함하는 소수의 경우 '.'과 피연산자를 포함한 소수를 하나의 피연산자 토큰으로 처리하였으므로, 2-2에서 처리

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
			Node* operandNode1 = LLS_Pop(&stack);
			Node* operandNode2 = LLS_Pop(&stack);
			double tmpResult = CalcOperation(atof(operandNode2->data), token.symbolType, atof(operandNode1->data)); //중간 계산 결과
			char tmpResultBuffer[_CVTBUFSIZE] = { '\0', }; //중간 계산 결과 변환 위한 버퍼

			_gcvt_s(tmpResultBuffer, _CVTBUFSIZE, tmpResult, 10); //부동 소수점을 문자열로 변환
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