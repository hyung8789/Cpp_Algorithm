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
/// 새로운 노드 생성 및 생성 된 노드 반환
/// </summary>
/// <param name="srcData">노드의 데이터</param>
/// <returns>생성 된 노드</returns>
Node* EXPRT_CreateNode(DataType srcData)
{
	Node* retVal = (Node*)malloc(sizeof(Node));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	if (typeid(srcData) == typeid(char*)) //문자열 공간 할당 및 문자열 복사
	{
		retVal->data = (char*)malloc(strlen(srcData) + 1); //'\0' 포함 크기
		if (retVal->data == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		if (strcpy_s(retVal->data, strlen(srcData) + 1, srcData) != 0)
			throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));
	}
	else //값 할당
	{
		retVal->data = srcData;
	}

	retVal->left = retVal->right = NULL;

	return retVal;
}

/// <summary>
/// 대상 노드에 할당 된 메모리 해제
/// </summary>
/// <param name="srcNode">대상 노드</param>
void EXPRT_DeallocateNode(Node** srcNode)
{
	if ((*srcNode) != NULL)
	{
		free(*srcNode);
		(*srcNode) = NULL;
	}
}

/// <summary>
/// 대상 트리에 할당 된 모든 노드의 메모리 해제
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
void EXPRT_DeallocateTree(Node** srcRootNode)
{
	if ((*srcRootNode) != NULL) //후위 순회로 왼쪽 끝 노드부터 해제 
	{
		if ((*srcRootNode)->left != NULL)
			EXPRT_DeallocateTree(&((*srcRootNode)->left));

		if ((*srcRootNode)->right != NULL)
			EXPRT_DeallocateTree(&((*srcRootNode)->right));

		EXPRT_DeallocateNode(srcRootNode);
		(*srcRootNode) = NULL;
	}
}

/// <summary>
/// 대상 후위 표현식으로부터 트리 구축
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="srcPostfixExpr">대상 후위 표현식</param>
void EXPRT_BulidTreeFromPostfixExpr(Node** srcRootNode, char* srcPostfixExpr)
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

		1) 후위 표현식의 오른쪽에서 왼쪽 (역방향)으로 순차적인 토큰 분리 (피연산자 간 구분을 위한 공백, 피연산자, 연산자)

		2) 분리 된 토큰의 기호 타입에 따라,
			2-1) 피연산자 간 구분을 위한 공백인 경우
			: 계산 시에 사용하지 않으므로, 해당 토큰 무시

			2-2) 피연산자인 경우
			: 해당 토큰은 잎 노드

			2-3) 연산자인 경우
			: 해당 토큰은 초기 루트 노드 혹은 가지 노드

				2-3-1) 이 다음 두 토큰이 각각 피연산자일 경우
				: 해당 토큰은 각각 오른쪽 자식 노드와 왼쪽 자식 노드

				2-3-2) 이 다음 단일 토큰이 다시 연산자인 경우
				: 해당 토큰은 가지 노드
		---

		ex :
		중위 표현식 : 1.0 * 2.0 + (7.0 - 8.0)
		후위 표현식 : 1.0 2.0 * 7.0 8.0 -+

					+
			*				-
		1.0		2.0		7.0		8.0
	***/

	Token token;
	GenNextToken(srcPostfixExpr, &token, EXPR_READ_DIRECTION::RIGHT_TO_LEFT);
	srcPostfixExpr[strlen(srcPostfixExpr) - token.readCount] = '\0'; //대상 후위 표현식의 토큰에서 읽은 문자 개수만큼 제거

	switch (token.symbolType)
	{
	case SYMBOL_TYPE::LEFT_PARENTHESIS:
	case SYMBOL_TYPE::RIGHT_PARENTHESIS:
	case SYMBOL_TYPE::DOT:
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcPostfixExpr)"));

	case SYMBOL_TYPE::SPACE:
		break;

	case SYMBOL_TYPE::PLUS:
	case SYMBOL_TYPE::MINUS:
	case SYMBOL_TYPE::MULTIPLY:
	case SYMBOL_TYPE::DIVIDE:
		(*srcRootNode) = EXPRT_CreateNode(token.str);

		EXPRT_BulidTreeFromPostfixExpr(&((*srcRootNode)->right), srcPostfixExpr); //현재 노드의 오른쪽 자식 노드 생성
		EXPRT_BulidTreeFromPostfixExpr(&((*srcRootNode)->left), srcPostfixExpr); //현재 노드의 왼쪽 자식 노드 생성
		break;

	default: //피연산자인 경우
		(*srcRootNode) = EXPRT_CreateNode(token.str); //해당 토큰은 잎 노드
		break;
	}
}

/// <summary>
/// 대상 트리에 대한 연산 결과 반환
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
double EXPRT_EvaluateTree(Node* srcRootNode)
{
	//구축 된 수식 트리의 후위 순회를 통해 맨 왼쪽 하단 노드부터 병합하여 계산 수행

	if (srcRootNode == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	double retVal = 0.0;



	return 0.0;
}

/// <summary>
/// 순회 모드에 따른 대상 트리 출력
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="traversalMode">순회 모드</param>
void EXPRT_DispOrderedTree(Node* srcRootNode, TRAVERSAL_MODE traversalMode)
{
	if (srcRootNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	switch (traversalMode)
	{
	case TRAVERSAL_MODE::PREORDER:
		std::cout << srcRootNode->data << " ";

		if (srcRootNode->left != NULL)
			EXPRT_DispOrderedTree(srcRootNode->left, traversalMode);

		if (srcRootNode->right != NULL)
			EXPRT_DispOrderedTree(srcRootNode->right, traversalMode);
		break;

	case TRAVERSAL_MODE::INORDER:
		if (srcRootNode->left != NULL)
			EXPRT_DispOrderedTree(srcRootNode->left, traversalMode);

		std::cout << srcRootNode->data << " ";

		if (srcRootNode->right != NULL)
			EXPRT_DispOrderedTree(srcRootNode->right, traversalMode);
		break;

	case TRAVERSAL_MODE::POSTORDER:
		if (srcRootNode->left != NULL)
			EXPRT_DispOrderedTree(srcRootNode->left, traversalMode);

		if (srcRootNode->right != NULL)
			EXPRT_DispOrderedTree(srcRootNode->right, traversalMode);

		std::cout << srcRootNode->data << " ";
		break;
	}
}
