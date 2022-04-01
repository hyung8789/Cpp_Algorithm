#include "Core.h"

/// <summary>
/// 대상 피연산자 1 및 대상 피연산자 2에 대해 연산자 문지에 따른 계산 결과 반환
/// </summary>
/// <param name="a">대상 피연산자 1</param>
/// <param name="srcChar">연산자 문자</param>
/// <param name="b">대상 피연산자 2</param>
/// <returns>대상 피연산자 1 및 대상 피연산자 2에 대해 연산자 문자에 따른 계산 결과</returns>
double CalcOperation(double a, char srcChar, double b)
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
/// 대상 중위 표현식으로부터 후위 표현식 생성
/// </summary>
/// <param name="srcInfixExpr">대상 중위 표현식</param>
/// <param name="dstPostfixExpr">대상 중위 표현식으로부터 변환되어 출력 될 후위 표현식</param>
void GenPostfixExpr(const char* srcInfixExpr, char* dstPostfixExpr)
{
	/***
		< 중위 표현식을 후위 표현식으로 변환 >

		! 중위 표현식에서 연산 우선순위를 위한 괄호를 최우선적으로 처리하여야 함
		! '(' 및 ')'는 변환 된 후위 표현식의 결과로서 출력하지 않음

		1) 사용자로부터 중위 표현식 입력

		2) 중위 표현식의 왼쪽부터 순차적으로 토큰 분리 (괄호, 피연산자 간 구분을 위한 공백, 피연산자, 연산자)
		: 토큰 분리 시 '.'을 포함하는 실수의 경우 '.'과 피연산자를 포함한 실수를 하나의 피연산자 기호 타입의 토큰으로 처리

		3) 현재 분리 된 토큰의 기호 타입에 따라,

			3-1) '(' 인 경우
			: 현재 분리 된 토큰의 기호를 스택에 삽입

			3-2) ')' 인 경우
			: 현재 스택에서 '(' 가 맨 처음으로 나올 때 까지 노드의 기호를 꺼내어 순차적으로 변환 된 후위 표현식의 결과로서 출력
			('(' 가 한 번 나오지 않을 경우 잘못 된 중위 표현식 예외 발생)

			3-3) 연산자 ('+', '-', '*', '/') 인 경우
			: 우선순위가 높은 연산자에 대해 먼저 계산을 위해, 후위 표현식의 결과로서 출력 위한 연산자 우선순위 판별 수행
			더 이상 스택에 현재 토큰의 기호 타입보다 높은 우선순위를 가진 기호 타입이 존재하지 않게 되는 시점에 현재 분리 된 토큰의 기호를 스택에 삽입

				3-3-1) 현재 스택의 최상위 노드의 기호 타입에 대한 우선순위 >= 현재 분리 된 토큰의 기호 타입에 대한 우선순위
				: 우선순위가 높은 연산자에 대해 먼저 계산을 위해, 현재 스택의 최상위 노드를 스택에서 꺼내 해당 기호를 변환 된 후위 표현식의 결과로서 출력
				(단, 현재 스택의 최상의 노드의 기호가 '(' 일 경우, 변환 된 후위 표현식의 결과로서 출력하지 않음)

				3-3-2) 현재 스택의 최상위 노드의 기호 타입에 대한 우선순위 < 현재 분리 된 토큰의 기호 타입에 대한 우선순위
				: 우선순위가 높은 기호에 대해 먼저 계산을 위해 후위 표현식의 결과로서 출력 위한 판별 중지

			3-4)  ' ' (피연산자 간 구분을 위한 공백), 피연산자 (정수 혹은 '.'을 포함 한 실수) 인 경우
			: 현재 분리 된 토큰의 기호를 스택에 삽입하지 않고 즉시 변환 된 후위 표현식의 결과로서 출력

			3-5) '.' (실수 표현을 위한 점) 인 경우
			: ' ', '.'은 단일 토큰으로서 처리 되지 않으며, 무시되거나 '.'을 포함 한 실수로서 하나의 피연산자로 처리되어야만 함
			이에 따라, 논리 예외 발생

		4) 현재 중위 표현식에 더 이상 토큰으로 분리 할 것이 없으면, 스택에 남은 노드의 기호들을 순차적으로 모두 변환 된 후위 표현식의 결과로서 출력
		: 올바른 중위 표현식은 항상 '('와 ')'의 쌍이 일치 (개수가 일치)해야만 히며, 중위 표현식을 후위 표현식으로 변환하는 과정에서 ')'가 나올 경우, 
		'('가 나올 때 까지 후위 표현식에 출력하므로, 현재 스택에 남은 노드들 중 '('이 존재 할 경우, 잘못 된 중위 표현식 예외 발생
	***/

	if (srcInfixExpr == NULL || dstPostfixExpr == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	LINKED_LIST_STACK* stack = NULL;

	size_t srcInfixExprLen = strlen(srcInfixExpr); //대상 중위 표현식 길이
	size_t srcInfixExprNextReadIndex = 0; //대상 중위 표현식의 다음에 읽을 인덱스

	bool isValidExpr = false; //유효한 표현식 여부
	TOKEN token;

	LLS_CreateStack(&stack);

	while (srcInfixExprNextReadIndex < srcInfixExprLen) //중위 표현식을 다 읽을 때까지
	{
		GenNextToken(&srcInfixExpr[srcInfixExprNextReadIndex], &token, EXPR_READ_DIRECTION::LEFT_TO_RIGHT); //현재까지 읽은 위치부터 토큰 생성

		switch (token._symbolType)
		{
		case SYMBOL_TYPE::LEFT_PARENTHESIS:
			LLS_Push(stack, LLS_CreateNode(token._str)); //현재 분리 된 토큰의 기호를 스택에 삽입
			break;

		case SYMBOL_TYPE::RIGHT_PARENTHESIS:
			isValidExpr = false;

			while (!LLS_IsEmpty(stack)) //현재 스택에서 '(' 가 나올 때 까지 노드의 기호를 꺼내어 순차적으로 변환 된 후위 표현식의 결과로서 출력
			{
				NODE* poppedNode = LLS_Pop(stack);

				if (CharToSymbolType(poppedNode->_data[0]) == SYMBOL_TYPE::LEFT_PARENTHESIS) //'(' 를 만날 경우
				{
					isValidExpr = true;
					LLS_DeallocateNode(&poppedNode);
					break;
				}
				else //피연산자, 연산자인 경우
				{
					if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, poppedNode->_data) != 0) //후위 표현식에 출력
						throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

					LLS_DeallocateNode(&poppedNode);
				}
			}

			if (!isValidExpr) //'(' 가 한 번 나오지 않을 경우
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong InfixExpr)"));
			break;

		case SYMBOL_TYPE::OPERAND:
			if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, token._str) != 0) //현재 분리 된 토큰의 기호를 후위 표현식에 출력
				throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));
		case SYMBOL_TYPE::SPACE:
			if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, " ") != 0) //피연산자 간 구분을 위한 공백을 후위 표현식에 출력
				throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));
			break;

		case SYMBOL_TYPE::DOT:
			throw std::logic_error(std::string(__func__) + std::string(" : '.' is part of operand"));

		default: //연산자인 경우
			while (!LLS_IsEmpty(stack)) //우선순위가 높은 연산자에 대해 먼저 계산을 위해, 후위 표현식의 결과로서 출력 위한 연산자 우선순위 판별
			{
				SYMBOL_TYPE peekedNodeSymbolType = CharToSymbolType(LLS_Peek(stack)->_data[0]); //현재 스택의 최상위 노드의 연산자에 대한 기호 타입

				if (GetSymbolTypePriority(peekedNodeSymbolType) >=
					GetSymbolTypePriority(token._symbolType)) //현재 스택의 최상위 노드의 기호 타입에 대한 우선순위 >= 현재 분리 된 토큰의 기호 타입에 대한 우선순위
				{
					NODE* poppedNode = LLS_Pop(stack);

					if (peekedNodeSymbolType != SYMBOL_TYPE::LEFT_PARENTHESIS) //'(' 가 아닌 경우에만 우선순위가 높은 연산자에 대해 먼저 계산을 위해 후위 표현식으로 출력
					{
						if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, poppedNode->_data) != 0)
							throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));
					}

					LLS_DeallocateNode(&poppedNode);
				}
				else //현재 스택의 최상위 노드의 기호 타입에 대한 우선순위 < 현재 분리 된 토큰의 기호 타입에 대한 우선순위
				{
					break;
				}
			}

			LLS_Push(stack, LLS_CreateNode(token._str)); //현재 분리 된 토큰의 기호를 스택에 삽입
			break;
		}

		srcInfixExprNextReadIndex += token._readCount; //다음에 읽을 위치부터 다시 토큰 생성
	}

	while (!LLS_IsEmpty(stack)) //스택에 남은 노드의 기호들을 순차적으로 모두 변환 된 후위 표현식의 결과로서 출력
	{
		NODE* poppedNode = LLS_Pop(stack);

		if (CharToSymbolType(poppedNode->_data[0]) == SYMBOL_TYPE::LEFT_PARENTHESIS) //')'와 쌍이 맞지 않는 '(' 가 남아있는 경우
			throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong InfixExpr)"));

		if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, poppedNode->_data) != 0) //후위 표현식에 출력
			throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

		LLS_DeallocateNode(&poppedNode);
	}

	LLS_DeallocateLinkedListStack(&stack);
}

/// <summary>
/// 대상 후위 표현식으로부터 계산 결과 반환
/// </summary>
/// <param name="srcPostfixExpr">대상 후위 표현식</param>
/// <returns>대상 후위 표현식에 대한 계산 결과</returns>
double CalcPostfixExpr(const char* srcPostfixExpr)
{
	/***
		< 후위 표현식에 대한 처리 및 계산 >

		! 후위 표현식 : 중위 표현식으로부터 '(', ')' 및 연산자 우선순위에 따라 변환
		! '.'을 포함하는 실수의 경우 '.'과 피연산자를 포함한 실수를 하나의 피연산자 토큰으로 처리하였으므로, 2-2에서 처리

		ex)
		중위 표현식 : ((1 - 2 * 2) + 1) * 3
		변환 된 후위 표현식 : 122*-1+3*

		1) 후위 표현식의 왼쪽에서 오른쪽 (정방향)으로 순차적으로 토큰 분리 (피연산자 간 구분을 위한 공백, 피연산자, 연산자)

		2) 분리 된 토큰의 기호 타입에 따라,
			2-1) 피연산자 간 구분을 위한 공백인 경우
			: 계산 시에 사용하지 않으므로, 해당 토큰 무시

			2-2) 피연산자인 경우
			: 토큰을 스택에 삽입

			2-3) 연산자인 경우
			: 스택에 존재하는 피연산자를 2회 꺼낸 후 피연산자에 대해 역순으로 현재 토큰의 연산자와 계산 수행 및 계산 결과를 다시 스택에 삽입
			단, 올바른 후위 표현식은 분리 된 토큰이 연산자인 상황에서 계산을 위해 스택에서 2회 꺼냈을 시, 항상 피연산자가 2회 연속으로 나타나야 함
			이에 따라, 스택에서 노드를 2회 꺼낼 수 없을 경우 (ex : 1*+2), 잘못 된 후위 표현식 예외 발생

		3) 후위 표현식을 끝까지 읽었을 경우 현재 스택에 남아있는 최종 계산 결과 반환
	***/

	if (srcPostfixExpr == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	LINKED_LIST_STACK* stack = NULL;

	size_t srcPostfixExprLen = strlen(srcPostfixExpr); //대상 후위 표현식 길이
	size_t srcPostfixExprNextReadIndex = 0; //대상 후위 표현식의 다음에 읽을 위치

	TOKEN token;

	double retVal = 0.0; //최종 계산 결과

	LLS_CreateStack(&stack);

	while (srcPostfixExprNextReadIndex < srcPostfixExprLen) //후위 표현식을 다 읽을 때까지
	{
		GenNextToken(&srcPostfixExpr[srcPostfixExprNextReadIndex], &token, EXPR_READ_DIRECTION::LEFT_TO_RIGHT); //현재까지 읽은 위치부터 토큰 생성

		switch (token._symbolType)
		{
		case SYMBOL_TYPE::LEFT_PARENTHESIS:
		case SYMBOL_TYPE::RIGHT_PARENTHESIS:
		case SYMBOL_TYPE::DOT:
			throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcPostfixExpr)"));

		case SYMBOL_TYPE::SPACE:
			break;

		case SYMBOL_TYPE::OPERAND:
			LLS_Push(stack, LLS_CreateNode(token._str));
			break;

		default: //연산자인 경우
			if(LLS_GetTotalNodeCount(stack) < 2) //스택에서 노드를 2회 꺼낼 수 없을 경우
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcPostfixExpr)"));

			NODE* operandNode1 = LLS_Pop(stack); //연산자 뒤에 들어 갈 피연산자
			NODE* operandNode2 = LLS_Pop(stack); //연산자 앞에 들어 갈 피연산자
			double tmpResult = CalcOperation(atof(operandNode2->_data), token._symbolType, atof(operandNode1->_data)); //중간 계산 결과
			char tmpResultBuffer[_CVTBUFSIZE] = { '\0', }; //중간 계산 결과 변환 위한 버퍼

			_gcvt_s(tmpResultBuffer, _CVTBUFSIZE, tmpResult, 10); //실수를 문자열로 변환
			LLS_Push(stack, LLS_CreateNode(tmpResultBuffer));

			LLS_DeallocateNode(&operandNode1);
			LLS_DeallocateNode(&operandNode2);
			break;
		}

		srcPostfixExprNextReadIndex += token._readCount; //다음에 읽을 위치부터 다시 토큰 생성
	}

	NODE* resultNode = LLS_Pop(stack);
	retVal = atof(resultNode->_data);
	LLS_DeallocateNode(&resultNode);
	LLS_DeallocateLinkedListStack(&stack);

	return retVal;
}