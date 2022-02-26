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
/// ��� ���� ǥ�������κ��� ���� ǥ���� ����
/// </summary>
/// <param name="srcInfixExpr">��� ���� ǥ����</param>
/// <param name="dstPostfixExpr">��� ���� ǥ�������κ��� ��ȯ�Ǿ� ��� �� ���� ǥ����</param>
void GenPostfixExpr(const char* srcInfixExpr, char* dstPostfixExpr)
{
	/***
		< ���� ǥ������ ���� ǥ�������� ��ȯ >

		! ���� ǥ���Ŀ��� ���� �켱������ ���� ��ȣ�� �ֿ켱������ ó���Ͽ��� ��
		! '(' �� ')'�� ��ȯ �� ���� ǥ������ ����μ� ������� ����

		1) ����ڷκ��� ���� ǥ���� �Է�

		2) ���� ǥ������ ���ʺ��� ���������� ��ū �и� (��ȣ, �ǿ����� �� ������ ���� ����, �ǿ�����, ������)
		: ��ū �и� �� '.'�� �����ϴ� �Ǽ��� ��� '.'�� �ǿ����ڸ� ������ �Ǽ��� �ϳ��� �ǿ����� ��ȣ Ÿ���� ��ū���� ó��

		3) ���� �и� �� ��ū�� ��ȣ Ÿ�Կ� ����,

			3-1) '(' �� ���
			: ���� �и� �� ��ū�� ��ȣ�� ���ÿ� ����

			3-2) ')' �� ���
			: ���� ���ÿ��� '(' �� �� ó������ ���� �� ���� ����� ��ȣ�� ������ ���������� ��ȯ �� ���� ǥ������ ����μ� ���
			('(' �� �� �� ������ ���� ��� �߸� �� ���� ǥ���� ���� �߻�)

			3-3) ������ ('+', '-', '*', '/') �� ���
			: �켱������ ���� �����ڿ� ���� ���� ����� ����, ���� ǥ������ ����μ� ��� ���� ������ �켱���� �Ǻ� ����
			�� �̻� ���ÿ� ���� ��ū�� ��ȣ Ÿ�Ժ��� ���� �켱������ ���� ��ȣ Ÿ���� �������� �ʰ� �Ǵ� ������ ���� �и� �� ��ū�� ��ȣ�� ���ÿ� ����

				3-3-1) ���� ������ �ֻ��� ����� ��ȣ Ÿ�Կ� ���� �켱���� >= ���� �и� �� ��ū�� ��ȣ Ÿ�Կ� ���� �켱����
				: �켱������ ���� �����ڿ� ���� ���� ����� ����, ���� ������ �ֻ��� ��带 ���ÿ��� ���� �ش� ��ȣ�� ��ȯ �� ���� ǥ������ ����μ� ���
				(��, ���� ������ �ֻ��� ����� ��ȣ�� '(' �� ���, ��ȯ �� ���� ǥ������ ����μ� ������� ����)

				3-3-2) ���� ������ �ֻ��� ����� ��ȣ Ÿ�Կ� ���� �켱���� < ���� �и� �� ��ū�� ��ȣ Ÿ�Կ� ���� �켱����
				: �켱������ ���� ��ȣ�� ���� ���� ����� ���� ���� ǥ������ ����μ� ��� ���� �Ǻ� ����

			3-4)  ' ' (�ǿ����� �� ������ ���� ����), �ǿ����� (���� Ȥ�� '.'�� ���� �� �Ǽ�) �� ���
			: ���� �и� �� ��ū�� ��ȣ�� ���ÿ� �������� �ʰ� ��� ��ȯ �� ���� ǥ������ ����μ� ���

			3-5) '.' (�Ǽ� ǥ���� ���� ��) �� ���
			: ' ', '.'�� ���� ��ū���μ� ó�� ���� ������, ���õǰų� '.'�� ���� �� �Ǽ��μ� �ϳ��� �ǿ����ڷ� ó���Ǿ�߸� ��
			�̿� ����, �� ���� �߻�

		4) ���� ���� ǥ���Ŀ� �� �̻� ��ū���� �и� �� ���� ������, ���ÿ� ���� ����� ��ȣ���� ���������� ��� ��ȯ �� ���� ǥ������ ����μ� ���
		: �ùٸ� ���� ǥ������ �׻� '('�� ')'�� ���� ��ġ (������ ��ġ)�ؾ߸� ����, ���� ǥ������ ���� ǥ�������� ��ȯ�ϴ� �������� ')'�� ���� ���, 
		'('�� ���� �� ���� ���� ǥ���Ŀ� ����ϹǷ�, ���� ���ÿ� ���� ���� �� '('�� ���� �� ���, �߸� �� ���� ǥ���� ���� �߻�
	***/

	if (srcInfixExpr == NULL || dstPostfixExpr == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	LINKED_LIST_STACK* stack = NULL;

	size_t srcInfixExprLen = strlen(srcInfixExpr); //��� ���� ǥ���� ����
	size_t srcInfixExprNextReadIndex = 0; //��� ���� ǥ������ ������ ���� �ε���

	bool isValidExpr = false; //��ȿ�� ǥ���� ����
	TOKEN token;

	LLS_CreateStack(&stack);

	while (srcInfixExprNextReadIndex < srcInfixExprLen) //���� ǥ������ �� ���� ������
	{
		GenNextToken(&srcInfixExpr[srcInfixExprNextReadIndex], &token, EXPR_READ_DIRECTION::LEFT_TO_RIGHT); //������� ���� ��ġ���� ��ū ����

		switch (token._symbolType)
		{
		case SYMBOL_TYPE::LEFT_PARENTHESIS:
			LLS_Push(&stack, LLS_CreateNode(token._str)); //���� �и� �� ��ū�� ��ȣ�� ���ÿ� ����
			break;

		case SYMBOL_TYPE::RIGHT_PARENTHESIS:
			isValidExpr = false;

			while (!LLS_IsEmpty(&stack)) //���� ���ÿ��� '(' �� ���� �� ���� ����� ��ȣ�� ������ ���������� ��ȯ �� ���� ǥ������ ����μ� ���
			{
				NODE* poppedNode = LLS_Pop(&stack);

				if (CharToSymbolType(poppedNode->_data[0]) == SYMBOL_TYPE::LEFT_PARENTHESIS) //'(' �� ���� ���
				{
					isValidExpr = true;
					LLS_DeallocateNode(&poppedNode);
					break;
				}
				else //�ǿ�����, �������� ���
				{
					if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, poppedNode->_data) != 0) //���� ǥ���Ŀ� ���
						throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

					LLS_DeallocateNode(&poppedNode);
				}
			}

			if (!isValidExpr) //'(' �� �� �� ������ ���� ���
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong InfixExpr)"));
			break;

		case SYMBOL_TYPE::OPERAND:
			if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, token._str) != 0) //���� �и� �� ��ū�� ��ȣ�� ���� ǥ���Ŀ� ���
				throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));
		case SYMBOL_TYPE::SPACE:
			if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, " ") != 0) //�ǿ����� �� ������ ���� ������ ���� ǥ���Ŀ� ���
				throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));
			break;

		case SYMBOL_TYPE::DOT:
			throw std::logic_error(std::string(__func__) + std::string(" : '.' is part of operand"));

		default: //�������� ���
			while (!LLS_IsEmpty(&stack)) //�켱������ ���� �����ڿ� ���� ���� ����� ����, ���� ǥ������ ����μ� ��� ���� ������ �켱���� �Ǻ�
			{
				SYMBOL_TYPE peekedNodeSymbolType = CharToSymbolType(LLS_Peek(&stack)->_data[0]); //���� ������ �ֻ��� ����� �����ڿ� ���� ��ȣ Ÿ��

				if (GetSymbolTypePriority(peekedNodeSymbolType) >=
					GetSymbolTypePriority(token._symbolType)) //���� ������ �ֻ��� ����� ��ȣ Ÿ�Կ� ���� �켱���� >= ���� �и� �� ��ū�� ��ȣ Ÿ�Կ� ���� �켱����
				{
					NODE* poppedNode = LLS_Pop(&stack);

					if (peekedNodeSymbolType != SYMBOL_TYPE::LEFT_PARENTHESIS) //'(' �� �ƴ� ��쿡�� �켱������ ���� �����ڿ� ���� ���� ����� ���� ���� ǥ�������� ���
					{
						if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, poppedNode->_data) != 0)
							throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));
					}

					LLS_DeallocateNode(&poppedNode);
				}
				else //���� ������ �ֻ��� ����� ��ȣ Ÿ�Կ� ���� �켱���� < ���� �и� �� ��ū�� ��ȣ Ÿ�Կ� ���� �켱����
				{
					break;
				}
			}

			LLS_Push(&stack, LLS_CreateNode(token._str)); //���� �и� �� ��ū�� ��ȣ�� ���ÿ� ����
			break;
		}

		srcInfixExprNextReadIndex += token._readCount; //������ ���� ��ġ���� �ٽ� ��ū ����
	}

	while (!LLS_IsEmpty(&stack)) //���ÿ� ���� ����� ��ȣ���� ���������� ��� ��ȯ �� ���� ǥ������ ����μ� ���
	{
		NODE* poppedNode = LLS_Pop(&stack);

		if (CharToSymbolType(poppedNode->_data[0]) == SYMBOL_TYPE::LEFT_PARENTHESIS) //')'�� ���� ���� �ʴ� '(' �� �����ִ� ���
			throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong InfixExpr)"));

		if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, poppedNode->_data) != 0) //���� ǥ���Ŀ� ���
			throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

		LLS_DeallocateNode(&poppedNode);
	}

	LLS_DeallocateLinkedListStack(&stack);
}

/// <summary>
/// ��� ���� ǥ�������κ��� ��� ��� ��ȯ
/// </summary>
/// <param name="srcPostfixExpr">��� ���� ǥ����</param>
/// <returns>��� ���� ǥ���Ŀ� ���� ��� ���</returns>
double CalcPostfixExpr(const char* srcPostfixExpr)
{
	/***
		< ���� ǥ���Ŀ� ���� ó�� �� ��� >

		! ���� ǥ���� : ���� ǥ�������κ��� '(', ')' �� ������ �켱������ ���� ��ȯ
		! '.'�� �����ϴ� �Ǽ��� ��� '.'�� �ǿ����ڸ� ������ �Ǽ��� �ϳ��� �ǿ����� ��ū���� ó���Ͽ����Ƿ�, 2-2���� ó��

		ex)
		���� ǥ���� : ((1 - 2 * 2) + 1) * 3
		��ȯ �� ���� ǥ���� : 122*-1+3*

		1) ���� ǥ������ ���ʿ��� ������ (������)���� ���������� ��ū �и� (�ǿ����� �� ������ ���� ����, �ǿ�����, ������)

		2) �и� �� ��ū�� ��ȣ Ÿ�Կ� ����,
			2-1) �ǿ����� �� ������ ���� ������ ���
			: ��� �ÿ� ������� �����Ƿ�, �ش� ��ū ����

			2-2) �ǿ������� ���
			: ��ū�� ���ÿ� ����

			2-3) �������� ���
			: ���ÿ� �����ϴ� �ǿ����ڸ� 2ȸ ���� �� �ǿ����ڿ� ���� �������� ���� ��ū�� �����ڿ� ��� ���� �� ��� ����� �ٽ� ���ÿ� ����
			��, �ùٸ� ���� ǥ������ �и� �� ��ū�� �������� ��Ȳ���� ����� ���� ���ÿ��� 2ȸ ������ ��, �׻� �ǿ����ڰ� 2ȸ �������� ��Ÿ���� ��
			�̿� ����, ���ÿ��� ��带 2ȸ ���� �� ���� ��� (ex : 1*+2), �߸� �� ���� ǥ���� ���� �߻�

		3) ���� ǥ������ ������ �о��� ��� ���� ���ÿ� �����ִ� ���� ��� ��� ��ȯ
	***/

	if (srcPostfixExpr == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	LINKED_LIST_STACK* stack = NULL;

	size_t srcPostfixExprLen = strlen(srcPostfixExpr); //��� ���� ǥ���� ����
	size_t srcPostfixExprNextReadIndex = 0; //��� ���� ǥ������ ������ ���� ��ġ

	TOKEN token;

	double retVal = 0.0; //���� ��� ���

	LLS_CreateStack(&stack);

	while (srcPostfixExprNextReadIndex < srcPostfixExprLen) //���� ǥ������ �� ���� ������
	{
		GenNextToken(&srcPostfixExpr[srcPostfixExprNextReadIndex], &token, EXPR_READ_DIRECTION::LEFT_TO_RIGHT); //������� ���� ��ġ���� ��ū ����

		switch (token._symbolType)
		{
		case SYMBOL_TYPE::LEFT_PARENTHESIS:
		case SYMBOL_TYPE::RIGHT_PARENTHESIS:
		case SYMBOL_TYPE::DOT:
			throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcPostfixExpr)"));

		case SYMBOL_TYPE::SPACE:
			break;

		case SYMBOL_TYPE::OPERAND:
			LLS_Push(&stack, LLS_CreateNode(token._str));
			break;

		default: //�������� ���
			if(LLS_GetTotalNodeCount(&stack) < 2) //���ÿ��� ��带 2ȸ ���� �� ���� ���
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcPostfixExpr)"));

			NODE* operandNode1 = LLS_Pop(&stack); //������ �ڿ� ��� �� �ǿ�����
			NODE* operandNode2 = LLS_Pop(&stack); //������ �տ� ��� �� �ǿ�����
			double tmpResult = CalcOperation(atof(operandNode2->_data), token._symbolType, atof(operandNode1->_data)); //�߰� ��� ���
			char tmpResultBuffer[_CVTBUFSIZE] = { '\0', }; //�߰� ��� ��� ��ȯ ���� ����

			_gcvt_s(tmpResultBuffer, _CVTBUFSIZE, tmpResult, 10); //�Ǽ��� ���ڿ��� ��ȯ
			LLS_Push(&stack, LLS_CreateNode(tmpResultBuffer));

			LLS_DeallocateNode(&operandNode1);
			LLS_DeallocateNode(&operandNode2);
			break;
		}

		srcPostfixExprNextReadIndex += token._readCount; //������ ���� ��ġ���� �ٽ� ��ū ����
	}

	NODE* resultNode = LLS_Pop(&stack);
	retVal = atof(resultNode->_data);
	LLS_DeallocateNode(&resultNode);
	LLS_DeallocateLinkedListStack(&stack);

	return retVal;
}