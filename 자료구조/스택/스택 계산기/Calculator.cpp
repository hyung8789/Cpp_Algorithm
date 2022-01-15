#include "Core.h"

/// <summary>
/// ��� �ǿ����� 1 �� ��� �ǿ����� 2�� ���� ������ ������ ���� ��� ��� ��ȯ
/// </summary>
/// <param name="a">��� �ǿ����� 1</param>
/// <param name="srcChar">������ ����</param>
/// <param name="b">��� �ǿ����� 2</param>
/// <returns>��� �ǿ����� 1 �� ��� �ǿ����� 2�� ���� ������ ���ڿ� ���� ��� ���</returns>
inline double CalcOperation(double a, char srcChar, double b)
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
/// ��� ���ڸ� 10�� �ƽ�Ű �ڵ�� ��ȯ
/// </summary>
/// <param name="srcChar">��� ����</param>
/// <returns>��ȯ �� 10�� �ƽ�Ű �ڵ�</returns>
inline int CharToDecAscii(char srcChar)
{
	return (int)srcChar;
}

/// <summary>
/// ��� 0~9 ������ ���� ���ڸ� 10�� �ƽ�Ű �ڵ�� ��ȯ
/// </summary>
/// <param name="srcSingleNum">��� 0~9 ������ ���� ����</param>
/// <returns>��ȯ �� 10�� �ƽ�Ű �ڵ�</returns>
inline int SingleNumToDecAscii(int srcSingleNum)
{
	if (srcSingleNum < 0 || srcSingleNum > 9)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	return '0' + srcSingleNum;
}

/// <summary>
/// ��� ���ڸ� ��ȣ Ÿ������ ��ȯ
/// </summary>
/// <param name="srcChar">��� ����</param>
/// <returns>��� ������ ��ȣ Ÿ��</returns>
SYMBOL_TYPE CharToSymbolType(char srcChar)
{
	int srcCharDecAscii = CharToDecAscii(srcChar); //��� ������ 10�� �ƽ�Ű �ڵ�

	switch (srcChar) //��� ���ڿ� ����
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

	default: //�ǿ����� Ȥ�� �߸� �� �Է�
		if (!(srcCharDecAscii >= SingleNumToDecAscii(0) &&
			srcCharDecAscii <= SingleNumToDecAscii(9))) //0 (dec ascii : 48) ~ 9 (dec ascii : 57) �� �ƴ� ��� (�ǿ����ڰ� �ƴ� ���)
			throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

		return SYMBOL_TYPE::OPERAND;
	}

	return (SYMBOL_TYPE)srcChar;
}

/// <summary>
/// ��� ��ȣ Ÿ�Կ� ���� ������ �켱���� ��ȯ
/// </summary>
/// <param name="srcSymbol">��� ��ȣ Ÿ��</param>
/// <returns>�켱���� (Ŭ ���� ���� ���� �켱����)</returns>
int GetSymbolTypePriority(SYMBOL_TYPE srcSymbol)
{
	int priority = -1; //�켱���� (Ŭ ���� ���� ���� �켱����)

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

	default: //')', �ǿ�����, �ǿ����� �� ������ ���� ����, �Ǽ� ǥ���� ���� ���� �켱������ �������� ����
		throw std::logic_error(std::string(__func__) + std::string(" : unmanaged priority"));
	}

	return priority;
}

/// <summary>
/// ��� ǥ�������κ��� ��ū ����
/// </summary>
/// <param name="srcExpr">��� ǥ����</param>
/// <param name="dstToken">��� ǥ�������κ��� �����Ǿ� ��� �� ��ū</param>
void GenNextToken(const char* srcExpr, Token* dstToken)
{
	if (srcExpr == NULL || dstToken == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (strlen(srcExpr) > sizeof(dstToken->str) - 1) //'\0' ���� �� ũ�⸦ �ʰ� �� ���
		throw std::out_of_range(std::string(__func__) + std::string(" : out of range"));

	memset(dstToken->str, '\0', sizeof(dstToken->str));
	dstToken->readCount = 0;

	/***
		< ��� ǥ�������κ��� ��ū ���� >

		1) ��� ǥ���Ŀ� ���� �� �аų�, ���ÿ� ���� ���Ͽ� �б� ���� �� ���� �ݺ�

		2) ���� ���� ������ ��ȣ Ÿ�Կ� ����,

			2-1) ���� ���� ������ ��ȣ Ÿ���� �ǿ������� ���
			: ���� ������ ���� ���ڰ� �����ϸ�, ���� ������ ���� ������ ��ȣ Ÿ���� �ǿ�����, �Ǽ� ǥ���� ���� '.'�̸� ��� �б�
			���� ������ ���� ���ڰ� �����ϸ�, ���� ������ ���� ������ ��ȣ Ÿ���� �ǿ�����, �Ǽ� ǥ���� ���� '.'�� �ƴ� ��� ���ÿ� ���� ���Ͽ� �б� ����
			('.'�� �����ϴ� �Ǽ��� ��� '.'�� �ǿ����ڸ� ������ �Ǽ��� �ϳ��� �ǿ����� ��ū���� ó��)

			2-2) ���� ���� ������ ��ȣ Ÿ���� �Ǽ� ǥ���� ���� '.'�� ���
			: ���� ������ ����, ���� ���ڰ� �����ϸ�, ���� ������ ����, ���� ���ڰ� �ǿ������̸� ��� �б�
			���� ������ ����, ���� ���ڰ� �������� �ʰų�, ���� ������ ����, ���� ���ڰ� �ǿ����ڰ� �ƴ� ��� �߸� �� ǥ���� ���� �߻�
			'.'�� �̹� ������ �����Ͽ��� ��� �߸� �� ǥ���� ���� �߻�

			2-3) ���� ���� ������ ��ȣ Ÿ���� ('(', ')'), ������, �ǿ����� �� ������ ���� ������ ���
			: ���ÿ� ���� Ȥ�� ��Ÿ ó���� ���Ͽ� �б� ����
		
		---

		< '.'�� �����ϴ� �Ǽ��� ���� >

		1) '.'�� '.'�� �����ϴ� �Ǽ����� �� ó���� �ܵ����� ���� �� �� ������, ��� �ǿ����ڰ� �ּ� �� ���� ���� �� �������� ����
		2) '.'�� '.'�� �����ϴ� �Ǽ����� �ݵ�� �� ���� ����
		3) '.'�� �����ϴ� �Ǽ����� '.'�� ������ ��, �ǿ����ڰ� �ּ� �� ���� �ݵ�� �����Ͽ��� �ϰ�, 
		�ּ� �� �� �̻��� �ǿ����ڰ� ���� �� �� �̻� �ǿ����ڰ� �������� ���� ���, 
		�� ��ü�� �ϳ��� �Ǽ��� �����Ͽ� '.' �� �ǿ����ڸ� ���� �� �ϳ��� �ǿ����� ��ū���� ó��
	***/

	bool isDotAlreadyExists = false; //�Ǽ� ǥ���� ���� '.' �� ���� �� �� ���� ����

	while (dstToken->readCount < strlen(srcExpr))
	{
		dstToken->str[dstToken->readCount] = srcExpr[dstToken->readCount]; //���ʺ��� �� ���ھ� ó��
		dstToken->symbolType = CharToSymbolType(srcExpr[dstToken->readCount]); //���� ���ڿ� ���� ��ȣ Ÿ�� �Ҵ�
		dstToken->readCount++; //���� ���� ���� ����

		switch (dstToken->symbolType)
		{
		case SYMBOL_TYPE::OPERAND:
			if (dstToken->readCount < strlen(srcExpr)) //���� ������ ���� ���ڰ� ���� �� ���
			{
				switch (CharToSymbolType(srcExpr[dstToken->readCount]))
				{
				case SYMBOL_TYPE::OPERAND:
				case SYMBOL_TYPE::DOT:
					break; //���� ������ ���� ������ ��ȣ Ÿ���� �ǿ�����, �Ǽ� ǥ���� ���� '.'�̸� ��� �б�

				default:
					goto END_PROC; //���ÿ� ���� ���Ͽ� �б� ����
				}
			}

			break;

		case SYMBOL_TYPE::DOT:
			if (!isDotAlreadyExists) //���� '.'�� �������� ���� ���
			{
				isDotAlreadyExists = true;

				if (dstToken->readCount >= 2 && dstToken->readCount < strlen(srcExpr)) //���� ������ ����, ���� ���ڰ� ���� �� ���
				{
					SYMBOL_TYPE prevSymbolType = CharToSymbolType(dstToken->str[dstToken->readCount - 2]); //���� ������ ���� ��ȣ ����
					SYMBOL_TYPE nextSymbolType = CharToSymbolType(srcExpr[dstToken->readCount]); //���� ������ ���� ��ȣ ����

					if (!(prevSymbolType == SYMBOL_TYPE::OPERAND && nextSymbolType == SYMBOL_TYPE::OPERAND)) //���� ������ ����, ���� ������ ��ȣ Ÿ���� �ǿ����ڰ� �ƴ� ���
						throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcExpr)"));
				}
				else
				{
					throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcExpr)"));
				}
			}
			else //�̹� '.'�� ���� �� ���
			{
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcExpr)"));
			}
			break;

		default: //'(', ')', ������, �ǿ����� �� ������ ���� ������ ���
			goto END_PROC; //���ÿ� ���� ���Ͽ� �б� ����
		}
	}

END_PROC: //do nothing
	return;
}

/// <summary>
/// ��� ���� ǥ������κ��� ���� ǥ��� ����
/// </summary>
/// <param name="srcInfixExpr">��� ���� ǥ���</param>
/// <param name="dstPostfixExpr">��� ���� ǥ������κ��� ��ȯ�Ǿ� ��� �� ���� ǥ���</param>
void GenPostfixExpr(const char* srcInfixExpr, char* dstPostfixExpr)
{
	/***
		< ���� ǥ����� ���� ǥ������� ��ȯ >

		! ���� ǥ��Ŀ��� ���� �켱������ ���� ��ȣ�� �ֿ켱������ ó���Ͽ��� ��
		! '(' �� ')'�� ��ȯ �� ���� ǥ����� ����μ� ������� ����

		1) ����ڷκ��� ���� ǥ��� �Է�

		2) ���� ǥ����� ���ʺ��� ���������� ��ū �и� (��ȣ, �ǿ����� �� ������ ���� ����, �ǿ�����, ������)
		: ��ū �и� �� '.'�� �����ϴ� �Ǽ��� ��� '.'�� �ǿ����ڸ� ������ �Ǽ��� �ϳ��� �ǿ����� ��ȣ Ÿ���� ��ū���� ó��

		3) ���� �и� �� ��ū�� ��ȣ Ÿ�Կ� ����,

			3-1) '(' �� ���
			: ���� �и� �� ��ū�� ��ȣ�� ���ÿ� ����

			3-2) ')' �� ���
			: ���� ���ÿ��� '(' �� �� ó������ ���� �� ���� ����� ��ȣ�� ������ ���������� ��ȯ �� ���� ǥ����� ����μ� ���
			('(' �� �� �� ������ ���� ��� �߸� �� ���� ǥ��� ���� �߻�)

			3-3) ������ ('+', '-', '*', '/') �� ���
			: �켱������ ���� �����ڿ� ���� ���� ����� ����, ���� ǥ����� ����μ� ��� ���� ������ �켱���� �Ǻ� ����
			�� �̻� ���ÿ� ���� ��ū�� ��ȣ Ÿ�Ժ��� ���� �켱������ ���� ��ȣ Ÿ���� �������� �ʰ� �Ǵ� ������ ���� �и� �� ��ū�� ��ȣ�� ���ÿ� ����

				3-3-1) ���� ������ �ֻ��� ����� ��ȣ Ÿ�Կ� ���� �켱���� >= ���� �и� �� ��ū�� ��ȣ Ÿ�Կ� ���� �켱����
				: �켱������ ���� �����ڿ� ���� ���� ����� ����, ���� ������ �ֻ��� ��带 ���ÿ��� ���� �ش� ��ȣ�� ��ȯ �� ���� ǥ����� ����μ� ���
				(��, ���� ������ �ֻ��� ����� ��ȣ�� '(' �� ���, ��ȯ �� ���� ǥ����� ����μ� ������� ����)

				3-3-2) ���� ������ �ֻ��� ����� ��ȣ Ÿ�Կ� ���� �켱���� < ���� �и� �� ��ū�� ��ȣ Ÿ�Կ� ���� �켱����
				: �켱������ ���� ��ȣ�� ���� ���� ����� ���� ���� ǥ����� ����μ� ��� ���� �Ǻ� ����

			3-4) �ǿ����� (���� Ȥ�� '.'�� ���� �� �Ǽ�), ' ' (�ǿ����� �� ������ ���� ����) �� ���
			: ���� �и� �� ��ū�� ��ȣ�� ���ÿ� �������� �ʰ� ��� ��ȯ �� ���� ǥ����� ����μ� ���

			3-5) '.' (�Ǽ� ǥ���� ���� ��) �� ���
			: '.'�� ���� ��ū���μ� ó�� ���� ������, '.'�� ���� �� �Ǽ��μ� �ϳ��� �ǿ����ڷ� ó���Ǿ�߸� ��
			�̿� ����, �� ���� �߻�

		4) ���� ���� ǥ��Ŀ� �� �̻� ��ū���� �и� �� ���� ������,
		���ÿ� ���� ����� ��ȣ���� ���������� ��� ��ȯ �� ���� ǥ����� ����μ� ���
	***/

	if (srcInfixExpr == NULL || dstPostfixExpr == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	LinkedListStack* stack = NULL;

	unsigned int currentReadPos = 0; //������� ���� ��ġ
	unsigned int infixExprLength = strlen(srcInfixExpr); //���� ǥ��� ����
	bool isValidExpr = false; //��ȿ�� ǥ���� ����
	Token token;

	LLS_CreateStack(&stack);

	while (currentReadPos < infixExprLength) //���� ǥ����� �� ���� ������
	{
		GenNextToken(&srcInfixExpr[currentReadPos], &token); //������� ���� ��ġ���� ��ū ����

		switch (token.symbolType)
		{
		case SYMBOL_TYPE::LEFT_PARENTHESIS:
			LLS_Push(&stack, LLS_CreateNode(token.str)); //���� �и� �� ��ū�� ��ȣ�� ���ÿ� ����
			break;

		case SYMBOL_TYPE::RIGHT_PARENTHESIS:
			isValidExpr = false;

			while (!LLS_IsEmpty(&stack)) //���� ���ÿ��� '(' �� ���� �� ���� ����� ��ȣ�� ������ ���������� ��ȯ �� ���� ǥ����� ����μ� ���
			{
				Node* poppedNode = LLS_Pop(&stack);

				if (CharToSymbolType(poppedNode->data[0]) == SYMBOL_TYPE::LEFT_PARENTHESIS) //'(' �� ���� ���
				{
					isValidExpr = true;
					LLS_DeallocateNode(&poppedNode);
					break;
				}
				else //�ǿ�����, �������� ���
				{
					if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, poppedNode->data) != 0) //���� ǥ��Ŀ� ���
						throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

					LLS_DeallocateNode(&poppedNode);
				}
			}

			if (!isValidExpr) //'(' �� �� �� ������ ���� ���
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong InfixExpr)"));
			break;

		case SYMBOL_TYPE::OPERAND:
			if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, token.str) != 0) //���� �и� �� ��ū�� ��ȣ�� ���� ǥ��Ŀ� ���
				throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));
		case SYMBOL_TYPE::SPACE:
			if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, " ") != 0) //�ǿ����� �� ������ ���� ������ ���� ǥ��Ŀ� ���
				throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));
			break;

		case SYMBOL_TYPE::DOT:
			throw std::logic_error(std::string(__func__) + std::string(" : '.' is part of operand"));

		default: //�������� ���
			while (!LLS_IsEmpty(&stack)) //�켱������ ���� �����ڿ� ���� ���� ����� ����, ���� ǥ����� ����μ� ��� ���� ������ �켱���� �Ǻ�
			{
				SYMBOL_TYPE peekedNodeSymbolType = CharToSymbolType(LLS_Peek(&stack)->data[0]); //���� ������ �ֻ��� ����� �����ڿ� ���� ��ȣ Ÿ��

				if (GetSymbolTypePriority(peekedNodeSymbolType) >=
					GetSymbolTypePriority(token.symbolType)) //���� ������ �ֻ��� ����� ��ȣ Ÿ�Կ� ���� �켱���� >= ���� �и� �� ��ū�� ��ȣ Ÿ�Կ� ���� �켱����
				{
					Node* poppedNode = LLS_Pop(&stack);

					if (peekedNodeSymbolType != SYMBOL_TYPE::LEFT_PARENTHESIS) //'(' �� �ƴ� ��쿡�� �켱������ ���� �����ڿ� ���� ���� ����� ���� ���� ǥ������� ���
					{
						if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, poppedNode->data) != 0)
							throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));
					}

					LLS_DeallocateNode(&poppedNode);
				}
				else //���� ������ �ֻ��� ����� ��ȣ Ÿ�Կ� ���� �켱���� < ���� �и� �� ��ū�� ��ȣ Ÿ�Կ� ���� �켱����
				{
					break;
				}
			}

			LLS_Push(&stack, LLS_CreateNode(token.str)); //���� �и� �� ��ū�� ��ȣ�� ���ÿ� ����
			break;
		}

		currentReadPos += token.readCount; //������ ���� ��ġ���� �ٽ� ��ū ����
	}

	while (!LLS_IsEmpty(&stack)) //���ÿ� ���� ����� ��ȣ���� ���������� ��� ��ȯ �� ���� ǥ����� ����μ� ���
	{
		Node* poppedNode = LLS_Pop(&stack);

		if (CharToSymbolType(poppedNode->data[0]) != SYMBOL_TYPE::LEFT_PARENTHESIS) //'(' �� �ƴ� ���
			if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, poppedNode->data) != 0) //���� ǥ��Ŀ� ���
				throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

		LLS_DeallocateNode(&poppedNode);
	}

	LLS_DeallocateLinkedListStack(&stack);
}

/// <summary>
/// ��� ���� ǥ������κ��� ��� ��� ��ȯ
/// </summary>
/// <param name="srcPostfixExpr">��� ���� ǥ���</param>
/// <returns>��� ���� ǥ��Ŀ� ���� ��� ���</returns>
double CalcPostfixExpr(const char* srcPostfixExpr)
{
	/***
		< ���� ǥ��Ŀ� ���� ó�� �� ��� >

		! ���� ǥ��� : ���� ǥ������κ��� '(', ')' �� ������ �켱������ ���� ��ȯ
		! '.'�� �����ϴ� �Ǽ��� ��� '.'�� �ǿ����ڸ� ������ �Ǽ��� �ϳ��� �ǿ����� ��ū���� ó���Ͽ����Ƿ�, 2-2���� ó��

		ex :
		���� ǥ��� : ((1 - 2 * 2) + 1) * 3
		��ȯ �� ���� ǥ��� : 122*-1+3*

		1) ���ʺ��� ���������� ��ū �и� (�ǿ����� �� ������ ���� ����, �ǿ�����, ������)

		2) �и� �� ��ū�� ��ȣ Ÿ�Կ� ����,
			2-1) �ǿ����� �� ������ ���� ������ ���
			: ����� ���Ͽ� �ش� ��ū ����

			2-2) �ǿ������� ���
			: ��ū�� ���ÿ� ����

			2-3) �������� ���
			: ���ÿ� �����ϴ� �ǿ����ڸ� 2ȸ ���� �� �ǿ����ڿ� ���� �������� ���� ��ū�� �����ڿ� ��� ���� �� ��� ����� �ٽ� ���ÿ� ����

		3) ���� ǥ����� ������ �о��� ��� ���� ���ÿ� �����ִ� ���� ��� ��� ��ȯ
	***/

	//TODO : ���� ǥ���Ŀ� ���� ��ȿ�� �˻縦 �ܼ�ȭ �ϱ� ���� ��ȯ �� ���� ǥ������ ���� ��ȿ�� �˻� ����
	// (ex : 1*+2 �Է� �� ���ÿ��� 2ȸ ���� �� �� ���� ���)
	//���� ǥ���Ŀ��� ��ȿ�� �˻縦 ������ ��� �Ľ� Ʈ���� �ۼ��ؾ� ��

	if (srcPostfixExpr == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	LinkedListStack* stack = NULL;

	unsigned int currentReadPos = 0; //������� ���� ��ġ
	unsigned int postfixExprLength = strlen(srcPostfixExpr); //���� ǥ��� ����
	Token token;

	double retVal = 0.0; //���� ��� ���

	LLS_CreateStack(&stack);

	while (currentReadPos < postfixExprLength) //���� ǥ����� �� ���� ������
	{
		GenNextToken(&srcPostfixExpr[currentReadPos], &token); //������� ���� ��ġ���� ��ū ����

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

		default: //�������� ���
			Node* operandNode1 = LLS_Pop(&stack); //������ �ڿ� ��� �� �ǿ�����
			Node* operandNode2 = LLS_Pop(&stack); //������ �տ� ��� �� �ǿ�����
			double tmpResult = CalcOperation(atof(operandNode2->data), token.symbolType, atof(operandNode1->data)); //�߰� ��� ���
			char tmpResultBuffer[_CVTBUFSIZE] = { '\0', }; //�߰� ��� ��� ��ȯ ���� ����

			_gcvt_s(tmpResultBuffer, _CVTBUFSIZE, tmpResult, 10); //�ε� �Ǽ����� ���ڿ��� ��ȯ
			LLS_Push(&stack, LLS_CreateNode(tmpResultBuffer));

			LLS_DeallocateNode(&operandNode1);
			LLS_DeallocateNode(&operandNode2);
			break;
		}

		currentReadPos += token.readCount; //������ ���� ��ġ���� �ٽ� ��ū ����
	}

	Node* resultNode = LLS_Pop(&stack);
	retVal = atof(resultNode->data);
	LLS_DeallocateNode(&resultNode);
	LLS_DeallocateLinkedListStack(&stack);

	return retVal;
}