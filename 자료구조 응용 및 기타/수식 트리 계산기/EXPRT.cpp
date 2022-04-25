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
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (not operand)"));
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
/// 새로운 노드 생성 및 생성 된 노드 반환
/// </summary>
/// <param name="srcData">노드의 데이터</param>
/// <returns>생성 된 노드</returns>
NODE* EXPRT_CreateNode(DATA_TYPE srcData)
{
	NODE* retVal = (NODE*)malloc(sizeof(NODE));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_data = (char*)malloc(strlen(srcData) + 1); //'\0' 포함 크기
	if (retVal->_data == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	if (strcpy_s(retVal->_data, strlen(srcData) + 1, srcData) != 0)
		throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

	retVal->_left = retVal->_right = NULL;

	return retVal;
}

/// <summary>
/// 대상 노드에 할당 된 메모리 해제
/// </summary>
/// <param name="srcNode">대상 노드</param>
void EXPRT_DeallocateNode(NODE** srcNode)
{
	if ((*srcNode) != NULL)
	{
		if ((*srcNode)->_data != NULL)
		{
			free((*srcNode)->_data);
			(*srcNode)->_data = NULL;
		}

		free(*srcNode);
		(*srcNode) = NULL;
	}
}

/// <summary>
/// 대상 트리에 할당 된 모든 노드의 메모리 해제
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
void EXPRT_DeallocateTree(NODE** srcRootNode)
{
	if ((*srcRootNode) != NULL) //후위 순회로 왼쪽 끝 노드부터 해제 
	{
		if ((*srcRootNode)->_left != NULL)
			EXPRT_DeallocateTree(&((*srcRootNode)->_left));

		if ((*srcRootNode)->_right != NULL)
			EXPRT_DeallocateTree(&((*srcRootNode)->_right));

		EXPRT_DeallocateNode(srcRootNode);
		(*srcRootNode) = NULL;
	}
}

/// <summary>
/// 대상 후위 표현식으로부터 트리 구축
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="srcPostfixExpr">대상 후위 표현식</param>
void EXPRT_BulidTreeFromPostfixExpr(NODE** srcRootNode, char srcPostfixExpr[])
{
	if ((*srcRootNode) != NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Memleak"));

	if (srcPostfixExpr == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	/***
		< 후위 표현식으로부터 수식 트리 구축 >

		! 후위 표현식 : 중위 표현식으로부터 '(', ')' 및 연산자 우선순위에 따라 변환
		! '.'을 포함하는 실수의 경우 '.'과 피연산자를 포함한 실수를 하나의 피연산자 토큰으로 처리하였으므로, 2-2에서 처리
		! 피연산자는 잎 노드, 연산자는 초기 루트 노드 혹은 가지 노드
		! 올바른 후위 표현식은 항상 오른쪽 끝에 연산자가 존재하며, 해당 연산자는 수식 트리의 루트 노드
		! 구축 된 수식 트리를 후위 순회를 통해 계산을 수행 시, 왼쪽부터 계산되므로,
		후위 표현식의 오른쪽에서 왼쪽 (역방향)으로 순차적인 토큰 분리 및 수식 트리 구축 과정 간 오른쪽 자식 노드와 왼쪽 자식 노드 순으로 할당
		! 후위 표현식에 ' ' (피연산자 간 구분을 위한 공백)이 존재 할 경우, 이는 트리 구축에 이용하지 않으므로,
		토큰의 기호 타입이 ' ' (피연산자 간 구분을 위한 공백)가 아닐 때까지 재 생성

		1) 후위 표현식의 오른쪽에서 왼쪽 (역방향)으로 순차적인 토큰 분리 (피연산자 간 구분을 위한 공백, 피연산자, 연산자)

		2) 분리 된 토큰의 기호 타입에 따라,

			2-1) 피연산자인 경우
			: 해당 토큰은 잎 노드가 되며, 현재 노드에 해당 토큰으로 생성

			2-2) 연산자인 경우
			: 해당 토큰은 초기 루트 노드 혹은 가지 노드가 되며, 현재 노드에 해당 토큰으로 생성하고,
			현재 노드의 오른쪽 하위 트리, 왼쪽 하위 트리에 대해 재귀적으로 이동 (1로 이동하여 토큰 재 생성)

			ex)
			- 현재 노드의 다음 두 토큰이 각각 피연산자일 경우, 해당 토큰은 현재 노드의 각각 오른쪽 자식 노드와 왼쪽 자식 노드
			- 현재 노드의 다음 단일 토큰이 다시 연산자인 경우, 해당 토큰은 가지 노드

		---

		중위 표현식 : 1.0 * 2.0 + (7.0 - 8.0)
		후위 표현식 : 1.0 2.0 * 7.0 8.0 -+

					+
			*				-
		1.0		2.0		7.0		8.0
	***/

	TOKEN token;

	do
	{
		GenNextToken(srcPostfixExpr, &token, EXPR_READ_DIRECTION::RIGHT_TO_LEFT);
		memset(&srcPostfixExpr[strlen(srcPostfixExpr) - token._readCount], '\0', token._readCount); //대상 후위 표현식의 토큰에서 읽은 문자 개수만큼 제거
	} while (token._symbolType == SYMBOL_TYPE::SPACE); //토큰의 기호 타입이 ' ' (피연산자 간 구분을 위한 공백)가 아닐 때까지 재 생성

	switch (token._symbolType)
	{
	case SYMBOL_TYPE::LEFT_PARENTHESIS:
	case SYMBOL_TYPE::RIGHT_PARENTHESIS:
	case SYMBOL_TYPE::DOT:
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcPostfixExpr)"));

	case SYMBOL_TYPE::SPACE:
		throw myexception::MEM_CORRUPTION_EXCEPTION(std::string(__func__) + std::string(" : Mem corruption"));

	case SYMBOL_TYPE::PLUS:
	case SYMBOL_TYPE::MINUS:
	case SYMBOL_TYPE::MULTIPLY:
	case SYMBOL_TYPE::DIVIDE:
		(*srcRootNode) = EXPRT_CreateNode(token._str); //연산자인 경우 해당 토큰은 초기 루트 노드 혹은 가지 노드

		EXPRT_BulidTreeFromPostfixExpr(&((*srcRootNode)->_right), srcPostfixExpr); //현재 노드의 오른쪽 하위 트리 생성
		EXPRT_BulidTreeFromPostfixExpr(&((*srcRootNode)->_left), srcPostfixExpr); //현재 노드의 왼쪽 하위 트리 생성
		break;

	default: //피연산자인 경우
		(*srcRootNode) = EXPRT_CreateNode(token._str); //피연산자인 경우 해당 토큰은 잎 노드
		break;
	}
}

/// <summary>
/// 대상 트리에 대한 연산 결과 반환
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
double EXPRT_EvaluateTree(NODE* srcRootNode)
{
	/***
		< 구축 된 수식 트리로부터 연산 수행 >

		1) 루트 노드에서 시작, 현재 노드의 기호 타입에 따라,

			1-1) 연산자인 경우
			: 현재 노드의 왼쪽 하위 트리 및 오른쪽 하위 트리로 재귀적으로 이동
			각 하위 트리의 연산 결과를 병합하여 루트 노드 (연산자)에서 최종 계산 결과 반환 (후위 순회)
			단, 현재 노드가 연산자인 경우, 왼쪽 하위 트리 혹은 오른쪽 하위 트리가 하나라도 존재하지 않을 경우 잘못 된 수식 트리 예외 발생

			1-2) 피연산자인 경우
			: 현재 노드의 상위 노드로 현재 노드의 값 전달
	***/

	double retVal = 0.0;
	double leftTreeOpResult = 0.0; //현재 노드의 왼쪽 하위 트리의 연산 결과
	double rightTreeOpResult = 0.0; //현재 노드의 오른쪽 하위 트리의 연산 결과

	switch (StrToSymbolType(srcRootNode->_data))
	{
	case SYMBOL_TYPE::LEFT_PARENTHESIS:
	case SYMBOL_TYPE::RIGHT_PARENTHESIS:
	case SYMBOL_TYPE::DOT:
	case SYMBOL_TYPE::SPACE:
		throw std::runtime_error(std::string(__func__) + std::string(" : Invalid Expression Tree"));

	case SYMBOL_TYPE::OPERAND:
		retVal = utils::StrToDouble(srcRootNode->_data); //피연산자인 경우, 현재 노드의 상위 노드로 현재 노드의 값 전달
		break;

	default: //연산자인 경우
		if (srcRootNode->_left == NULL || srcRootNode->_right == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Invalid Expression Tree"));

		leftTreeOpResult = EXPRT_EvaluateTree(srcRootNode->_left); //현재 노드의 왼쪽 하위 트리로 이동
		rightTreeOpResult = EXPRT_EvaluateTree(srcRootNode->_right); //현재 노드의 오른쪽 하위 트리로 이동

		retVal = CalcOperation(leftTreeOpResult, StrToSymbolType(srcRootNode->_data), rightTreeOpResult);
		break;
	}

	return retVal;
}

/// <summary>
/// 순회 방법에 따른 대상 트리 출력
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="traversalMethod">순회 방법</param>
/// <param name="os">출력 스트림</param>
void EXPRT_DispOrderedTree(NODE* srcRootNode, TRAVERSAL_METHOD traversalMethod, std::ostream& os)
{
	if (srcRootNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	switch (traversalMethod)
	{
	case TRAVERSAL_METHOD::PREORDER:
		os << srcRootNode->_data << " ";

		if (srcRootNode->_left != NULL)
			EXPRT_DispOrderedTree(srcRootNode->_left, traversalMethod, os);

		if (srcRootNode->_right != NULL)
			EXPRT_DispOrderedTree(srcRootNode->_right, traversalMethod, os);
		break;

	case TRAVERSAL_METHOD::INORDER:
		if (srcRootNode->_left != NULL)
			EXPRT_DispOrderedTree(srcRootNode->_left, traversalMethod, os);

		os << " '" << srcRootNode->_data << "' ";

		if (srcRootNode->_right != NULL)
			EXPRT_DispOrderedTree(srcRootNode->_right, traversalMethod, os);
		break;

	case TRAVERSAL_METHOD::POSTORDER:
		if (srcRootNode->_left != NULL)
			EXPRT_DispOrderedTree(srcRootNode->_left, traversalMethod, os);

		if (srcRootNode->_right != NULL)
			EXPRT_DispOrderedTree(srcRootNode->_right, traversalMethod, os);

		os << srcRootNode->_data << " ";
		break;
	}
}
