#include "Core.h"

/// <summary>
/// ��� �ǿ����� 1 �� ��� �ǿ����� 2�� ���� ������ ������ ���� ��� ��� ��ȯ
/// </summary>
/// <param name="a">��� �ǿ����� 1</param>
/// <param name="srcChar">������ ����</param>
/// <param name="b">��� �ǿ����� 2</param>
/// <returns>��� �ǿ����� 1 �� ��� �ǿ����� 2�� ���� ������ ���ڿ� ���� ��� ���</returns>
double CalcOperation(double a, char srcChar, double b)
{
	return CalcOperation(a, CharToSymbolType(srcChar), b);
}

/// <summary>
/// ��� �ǿ����� 1 �� ��� �ǿ����� 2�� ���� ������ ��ȣ Ÿ�Կ� ���� ��� ��� ��ȯ
/// </summary>
/// <param name="a">��� �ǿ����� 1</param>
/// <param name="op">������ ��ȣ Ÿ��</param>
/// <param name="b">��� �ǿ����� 2</param>
/// <returns>��� �ǿ����� 1 �� ��� �ǿ����� 2�� ���� ������ ��ȣ Ÿ�Կ� ���� ��� ���</returns>
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
/// ���ο� ��� ���� �� ���� �� ��� ��ȯ
/// </summary>
/// <param name="srcData">����� ������</param>
/// <returns>���� �� ���</returns>
Node* EXPRT_CreateNode(DataType srcData)
{
	Node* retVal = (Node*)malloc(sizeof(Node));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	if (typeid(srcData) == typeid(char*)) //���ڿ� ���� �Ҵ� �� ���ڿ� ����
	{
		retVal->data = (char*)malloc(strlen(srcData) + 1); //'\0' ���� ũ��
		if (retVal->data == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		if (strcpy_s(retVal->data, strlen(srcData) + 1, srcData) != 0)
			throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));
	}
	else //�� �Ҵ�
	{
		retVal->data = srcData;
	}

	retVal->left = retVal->right = NULL;

	return retVal;
}

/// <summary>
/// ��� ��忡 �Ҵ� �� �޸� ����
/// </summary>
/// <param name="srcNode">��� ���</param>
void EXPRT_DeallocateNode(Node** srcNode)
{
	if ((*srcNode) != NULL)
	{
		if (typeid((*srcNode)->data) == typeid(char*) && (*srcNode)->data != NULL)
		{
			free((*srcNode)->data);
			(*srcNode)->data = NULL;
		}

		free(*srcNode);
		(*srcNode) = NULL;
	}
}

/// <summary>
/// ��� Ʈ���� �Ҵ� �� ��� ����� �޸� ����
/// </summary>
/// <param name="srcRootNode">��� Ʈ���� �ֻ��� ��Ʈ ���</param>
void EXPRT_DeallocateTree(Node** srcRootNode)
{
	if ((*srcRootNode) != NULL) //���� ��ȸ�� ���� �� ������ ���� 
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
/// ��� ���� ǥ�������κ��� Ʈ�� ����
/// </summary>
/// <param name="srcRootNode">��� Ʈ���� �ֻ��� ��Ʈ ���</param>
/// <param name="srcPostfixExpr">��� ���� ǥ����</param>
void EXPRT_BulidTreeFromPostfixExpr(Node** srcRootNode, char srcPostfixExpr[])
{
	if ((*srcRootNode) != NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Memleak"));

	if (srcPostfixExpr == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	/***
		< ���� ǥ�������κ��� ���� Ʈ�� ���� >

		! ���� ǥ���� : ���� ǥ�������κ��� '(', ')' �� ������ �켱������ ���� ��ȯ
		! '.'�� �����ϴ� �Ǽ��� ��� '.'�� �ǿ����ڸ� ������ �Ǽ��� �ϳ��� �ǿ����� ��ū���� ó���Ͽ����Ƿ�, 2-2���� ó��
		! �ǿ����ڴ� �� ���, �����ڴ� �ʱ� ��Ʈ ��� Ȥ�� ���� ���
		! �ùٸ� ���� ǥ������ �׻� ������ ���� �����ڰ� �����ϸ�, �ش� �����ڴ� ���� Ʈ���� ��Ʈ ���
		! ���� �� ���� Ʈ���� ���� ��ȸ�� ���� ����� ���� ��, ���ʺ��� ���ǹǷ�,
		���� ǥ������ �����ʿ��� ���� (������)���� �������� ��ū �и� �� ���� Ʈ�� ���� ���� �� ������ �ڽ� ���� ���� �ڽ� ��� ������ �Ҵ�
		! ���� ǥ���Ŀ� ' ' (�ǿ����� �� ������ ���� ����)�� ���� �� ���, �̴� Ʈ�� ���࿡ �̿����� �����Ƿ�,
		��ū�� ��ȣ Ÿ���� ' ' (�ǿ����� �� ������ ���� ����)�� �ƴ� ������ �� ����

		1) ���� ǥ������ �����ʿ��� ���� (������)���� �������� ��ū �и� (�ǿ����� �� ������ ���� ����, �ǿ�����, ������)

		2) �и� �� ��ū�� ��ȣ Ÿ�Կ� ����,

			2-1) �ǿ������� ���
			: �ش� ��ū�� �� ��尡 �Ǹ�, ���� ��忡 �ش� ��ū���� ����

			2-2) �������� ���
			: �ش� ��ū�� �ʱ� ��Ʈ ��� Ȥ�� ���� ��尡 �Ǹ�, ���� ��忡 �ش� ��ū���� �����ϰ�,
			���� ����� ������ ���� Ʈ��, ���� ���� Ʈ���� ���� ��������� �̵� (1�� �̵��Ͽ� ��ū �� ����)

			ex)
			- ���� ����� ���� �� ��ū�� ���� �ǿ������� ���, �ش� ��ū�� ���� ����� ���� ������ �ڽ� ���� ���� �ڽ� ���
			- ���� ����� ���� ���� ��ū�� �ٽ� �������� ���, �ش� ��ū�� ���� ���

		---

		���� ǥ���� : 1.0 * 2.0 + (7.0 - 8.0)
		���� ǥ���� : 1.0 2.0 * 7.0 8.0 -+

					+
			*				-
		1.0		2.0		7.0		8.0
	***/

	Token token;

	do
	{
		GenNextToken(srcPostfixExpr, &token, EXPR_READ_DIRECTION::RIGHT_TO_LEFT);
		memset(&srcPostfixExpr[strlen(srcPostfixExpr) - token.readCount], '\0', token.readCount); //��� ���� ǥ������ ��ū���� ���� ���� ������ŭ ����
	} while (token.symbolType == SYMBOL_TYPE::SPACE); //��ū�� ��ȣ Ÿ���� ' ' (�ǿ����� �� ������ ���� ����)�� �ƴ� ������ �� ����

	switch (token.symbolType)
	{
	case SYMBOL_TYPE::LEFT_PARENTHESIS:
	case SYMBOL_TYPE::RIGHT_PARENTHESIS:
	case SYMBOL_TYPE::DOT:
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcPostfixExpr)"));

	case SYMBOL_TYPE::SPACE:
		throw std::runtime_error(std::string(__func__) + std::string(" : Mem corruption"));

	case SYMBOL_TYPE::PLUS:
	case SYMBOL_TYPE::MINUS:
	case SYMBOL_TYPE::MULTIPLY:
	case SYMBOL_TYPE::DIVIDE:
		(*srcRootNode) = EXPRT_CreateNode(token.str); //�������� ��� �ش� ��ū�� �ʱ� ��Ʈ ��� Ȥ�� ���� ���

		EXPRT_BulidTreeFromPostfixExpr(&((*srcRootNode)->right), srcPostfixExpr); //���� ����� ������ ���� Ʈ�� ����
		EXPRT_BulidTreeFromPostfixExpr(&((*srcRootNode)->left), srcPostfixExpr); //���� ����� ���� ���� Ʈ�� ����
		break;

	default: //�ǿ������� ���
		(*srcRootNode) = EXPRT_CreateNode(token.str); //�ǿ������� ��� �ش� ��ū�� �� ���
		break;
	}
}

/// <summary>
/// ��� Ʈ���� ���� ���� ��� ��ȯ
/// </summary>
/// <param name="srcRootNode">��� Ʈ���� �ֻ��� ��Ʈ ���</param>
double EXPRT_EvaluateTree(Node* srcRootNode)
{
	/***
		< ���� �� ���� Ʈ���κ��� ���� ���� >

		1) ��Ʈ ��忡�� ����, ���� ����� ��ȣ Ÿ�Կ� ����,

			1-1) �������� ���
			: ���� ����� ���� ���� Ʈ�� �� ������ ���� Ʈ���� ��������� �̵�
			�� ���� Ʈ���� ���� ����� �����Ͽ� ��Ʈ ��� (������)���� ���� ��� ��� ��ȯ (���� ��ȸ)
			��, ���� ��尡 �������� ���, ���� ���� Ʈ�� Ȥ�� ������ ���� Ʈ���� �ϳ��� �������� ���� ��� �߸� �� ���� Ʈ�� ���� �߻�

			1-2) �ǿ������� ���
			: ���� ����� ���� ���� ���� ����� �� ����
	***/

	double retVal = 0.0;
	double leftTreeOpResult = 0.0; //���� ����� ���� ���� Ʈ���� ���� ���
	double rightTreeOpResult = 0.0; //���� ����� ������ ���� Ʈ���� ���� ���

	switch (StrToSymbolType(srcRootNode->data))
	{
	case SYMBOL_TYPE::LEFT_PARENTHESIS:
	case SYMBOL_TYPE::RIGHT_PARENTHESIS:
	case SYMBOL_TYPE::DOT:
	case SYMBOL_TYPE::SPACE:
		throw std::runtime_error(std::string(__func__) + std::string(" : Invalid Expression Tree"));

	case SYMBOL_TYPE::OPERAND:
		retVal = StrToDouble(srcRootNode->data); //�ǿ������� ���, ���� ����� ���� ���� ���� ����� �� ����
		break;

	default: //�������� ���
		if (srcRootNode->left == NULL || srcRootNode->right == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Invalid Expression Tree"));

		leftTreeOpResult = EXPRT_EvaluateTree(srcRootNode->left); //���� ����� ���� ���� Ʈ���� �̵�
		rightTreeOpResult = EXPRT_EvaluateTree(srcRootNode->right); //���� ����� ������ ���� Ʈ���� �̵�

		retVal = CalcOperation(leftTreeOpResult, StrToSymbolType(srcRootNode->data), rightTreeOpResult);
		break;
	}

	return retVal;
}

/// <summary>
/// ��ȸ ��忡 ���� ��� Ʈ�� ���
/// </summary>
/// <param name="srcRootNode">��� Ʈ���� �ֻ��� ��Ʈ ���</param>
/// <param name="traversalMode">��ȸ ���</param>
/// <param name="os">��� ��Ʈ��</param>
void EXPRT_DispOrderedTree(Node* srcRootNode, TRAVERSAL_MODE traversalMode, std::ostream& os)
{
	if (srcRootNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	switch (traversalMode)
	{
	case TRAVERSAL_MODE::PREORDER:
		os << srcRootNode->data << " ";

		if (srcRootNode->left != NULL)
			EXPRT_DispOrderedTree(srcRootNode->left, traversalMode, os);

		if (srcRootNode->right != NULL)
			EXPRT_DispOrderedTree(srcRootNode->right, traversalMode, os);
		break;

	case TRAVERSAL_MODE::INORDER:
		if (srcRootNode->left != NULL)
			EXPRT_DispOrderedTree(srcRootNode->left, traversalMode, os);

		os << " '" << srcRootNode->data << "' ";

		if (srcRootNode->right != NULL)
			EXPRT_DispOrderedTree(srcRootNode->right, traversalMode, os);
		break;

	case TRAVERSAL_MODE::POSTORDER:
		if (srcRootNode->left != NULL)
			EXPRT_DispOrderedTree(srcRootNode->left, traversalMode, os);

		if (srcRootNode->right != NULL)
			EXPRT_DispOrderedTree(srcRootNode->right, traversalMode, os);

		os << srcRootNode->data << " ";
		break;
	}
}
