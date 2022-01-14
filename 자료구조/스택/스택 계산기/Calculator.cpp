#include "Core.h"

/// <summary>
/// ��� �ǿ����� 1 �� ��� �ǿ����� 2�� ���� ������ ������ ���� ��� ��� ��ȯ
/// </summary>
/// <param name="a">��� �ǿ����� 1</param>
/// <param name="srcChar">������ ����</param>
/// <param name="b">��� �ǿ����� 2</param>
/// <returns>��� �ǿ����� 1 �� ��� �ǿ����� 2�� ���� ������ ���ڿ� ���� ��� ���</returns>
inline double CalcOperation(double a, char srcChar, double b) throw(std::invalid_argument, std::overflow_error, std::underflow_error)
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
inline int SingleNumToDecAscii(int srcSingleNum) throw(std::invalid_argument)
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
SYMBOL_TYPE CharToSymbolType(char srcChar) throw(std::invalid_argument)
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
/// <param name="isInStack">��� ��ȣ Ÿ���� ���� �� ���� ���� ('(' ����)</param>
/// <returns>�켱���� (Ŭ ���� ���� ���� �켱����)</returns>
int GetSymbolTypePriority(SYMBOL_TYPE srcSymbol, bool isInStack)
{
	int priority = -1; //�켱���� (Ŭ ���� ���� ���� �켱����)

	switch (srcSymbol)
	{
	case SYMBOL_TYPE::LEFT_PARENTHESIS:
		/***
			���� ���ÿ� '('�� �����ϴ� ��Ȳ��, ���ο� '('�� �Է� �Ǿ��� ���, 
			���ο� '('�� ���� '(' ���� �� ���� �켱������ ������.
			'('�� ���� ���ÿ� �������� ���� ���, �ٸ� ��� ��ȣ Ÿ�Ժ��ٵ� �ֿ켱���� �켱������ ������.
		***/

		if (isInStack) //���� ���ÿ� �����ϴ� '('�� ���
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

	default: //')', �ǿ�����, �ǿ����� �� ������ ���� ����, �Ҽ� ǥ���� ���� ��
		/***
			')', �ǿ�����, �ǿ����� �� ������ ���� ����, �Ҽ� ǥ���� ���� ���� �켱������ �������� ���� (��� ó���� ���Ͽ� ������ �켱����)
			---
			- ��� ó�� : �ش� ��ȣ Ÿ�Կ� ���� �켱���� �� �񱳸� �������� �ʰ�, ���� ǥ������� ��� ����ϰų�, ���� Ȥ�� ��Ÿ ó�� ����
		***/

		break;
	}

	return priority;
}

/// <summary>
/// ��� ǥ�������κ��� ��ū ����
/// </summary>
/// <param name="srcExpr">��� ǥ����</param>
/// <param name="dstToken">��� ǥ�������κ��� �����Ǿ� ��� �� ��ū</param>
void GenNextToken(const char* srcExpr, Token* dstToken) throw(std::invalid_argument, std::out_of_range)
{
	if (srcExpr == NULL || dstToken == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (strlen(srcExpr) > sizeof(dstToken->str) - 1) //'\0' ���� �� ũ�⸦ �ʰ� �� ���
		throw std::out_of_range(std::string(__func__) + std::string(" : out of range"));

	memset(dstToken->str, '\0', sizeof(dstToken->str));
	dstToken->readCount = 0;

	/***
		1) ��� ǥ���Ŀ� ���� �� �аų�, ���ÿ� ���� ���Ͽ� �б� ���� �� ���� �ݺ�

		2) ���� ���� ������ ��ȣ Ÿ���� �ǿ����� Ȥ�� �Ҽ� ǥ���� ���� ���� ���,
		���� ������ ���� ���ڰ� �����ϸ� ���� ������ ���� ������ ��ȣ Ÿ���� �ǿ����ڰ� �ƴϸ� ���ÿ� ���� ���Ͽ� �б� ����
		('.'�� �����ϴ� �Ҽ��� ��� '.'�� �ǿ����ڸ� ������ �Ҽ��� �ϳ��� �ǿ����� ��ū���� ó��)

		3) ���� ���� ������ ��ȣ Ÿ���� �ǿ����ڰ� �ƴ� ��ȣ, �ǿ����� �� ������ ���� ����, �������� ���, ���ÿ� ���� ���Ͽ� �б� ����
		---
		< '.'�� �����ϴ� �Ҽ��� ���� ��ū ó�� >

		1) '.'�� '.'�� �����ϴ� �Ҽ����� �� ó���� �ܵ����� ���� �� �� ������, ��� �ǿ����ڰ� ���� �� �������� �����ؾ߸� �Ѵ�.

		2) '.'�� '.'�� �����ϴ� �Ҽ����� �� ���� �����Ѵ�.

		3) '.'�� �����ϴ� �Ҽ����� '.'�� ������ ��, �ǿ����ڰ� �ּ� �� ���� �����ؾ� �Ѵ�.

		4) '.'�� �����ϴ� �Ҽ����� '.'�� ������ ��, �ǿ����ڰ� �ּ� �� ���� �����ϰ� �� ������ �ǿ����ڰ� �������� ���� ���,
		�̸� �ϳ��� �Ҽ��� �����Ͽ� '.' �� �ǿ����ڸ� ���� �� �ϳ��� �ǿ����� ��ū���� ó���Ѵ�.
		---
		=> ���� ���� ���ڰ� '.' �� ���, ���� '.'�� �������� �ʾ�����, '.'�� ���� �� �� ���������� �˸���, 
		���� ���� ������ ����, ������ �� ���ڸ� Ȯ���Ͽ� �ǿ����ڰ� �ƴ� ��� ���� �߻�
	***/

	bool isDotAlreadyExists = false; //�Ҽ� ǥ���� ���� '.' �� ���� �� �� ���� ����

	while (dstToken->readCount < strlen(srcExpr))
	{
		dstToken->str[dstToken->readCount] = srcExpr[dstToken->readCount]; //���ʺ��� �� ���ھ� ó��
		dstToken->symbolType = CharToSymbolType(srcExpr[dstToken->readCount]); //���� ���ڿ� ���� ��ȣ Ÿ�� �Ҵ�
		dstToken->readCount++; //���� ���� ���� ����

		switch (dstToken->symbolType)
		{
		case SYMBOL_TYPE::DOT:
			if (!isDotAlreadyExists) //���� '.'�� �������� ���� ���
			{
				isDotAlreadyExists = true;
			
				if (dstToken->readCount >= 2 && dstToken->readCount < strlen(srcExpr)) //���� ������ ����, ���� ���ڰ� ���� �� ���
				{
					SYMBOL_TYPE prevSymbolType = CharToSymbolType(dstToken->str[dstToken->readCount - 2]); //���� ������ ���� ��ȣ ����
					SYMBOL_TYPE nextSymbolType = CharToSymbolType(srcExpr[dstToken->readCount]); //���� ������ ���� ��ȣ ����

					if(!(prevSymbolType == SYMBOL_TYPE::OPERAND && nextSymbolType == SYMBOL_TYPE::OPERAND)) //���� ������ ����, ���� ������ ��ȣ Ÿ���� �ǿ����ڰ� �ƴ� ���
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
		
		case SYMBOL_TYPE::OPERAND:
			if (dstToken->readCount < strlen(srcExpr)) ////���� ������ ���� ���ڰ� ���� �� ���
			{
				switch (CharToSymbolType(srcExpr[dstToken->readCount]))
				{
				case SYMBOL_TYPE::OPERAND:
				case SYMBOL_TYPE::DOT:
					break;

				default: //���ÿ� ���� ���Ͽ� �б� ����
					goto END_PROC;
				}
			}


			//if (dstToken->readCount < strlen(srcExpr) &&
			//	CharToSymbolType(srcExpr[dstToken->readCount]) != (SYMBOL_TYPE::OPERAND | SYMBOL_TYPE::DOT)) ��, ���� ������ ���� ������ ��ȣ Ÿ���� �ǿ����� Ȥ�� ���� �ƴ� ���
			//	goto END_PROC; //���ÿ� ���� ���Ͽ� �б� ����
			break;

		default: //���� ������ ��ȣ Ÿ���� �ǿ����� �� ������ ���� ����, ��ȣ, �������� ���
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
void GenPostfixExpr(const char* srcInfixExpr, char* dstPostfixExpr) throw(std::invalid_argument, std::runtime_error, std::logic_error)
{
	/***
		< ���� ǥ����� ���� ǥ������� ��ȯ >

		! ���� ǥ��Ŀ��� ���� �켱������ ���� ��ȣ�� �ֿ켱������ ó���Ͽ��� ��
		! '(' �� ')'�� ��ȯ �� ���� ǥ����� ����μ� ������� ����

		1) ����ڷκ��� ���� ǥ��� �Է�

		2) ���� ǥ����� ���ʺ��� ���������� ��ū �и� (��ȣ, �ǿ����� �� ������ ���� ����, �ǿ�����, ������)
		: ��ū �и� �� '.'�� �����ϴ� �Ҽ��� ��� '.'�� �ǿ����ڸ� ������ �Ҽ��� �ϳ��� �ǿ����� ��ȣ Ÿ���� ��ū���� ó��

		3) �и� �� ��ū�� ��ȣ Ÿ�Կ� ����,
			3-1) �ǿ�����, �ǿ����� �� ������ ���� ������ ���
			: ���ÿ� �������� �ʰ� ��ȯ �� ���� ǥ����� ����μ� ���

			3-2) '(' Ȥ�� �������� ���
			: �켱������ ���� ��ȣ�� ���� ���� ����� ����, ���� ǥ����� ����μ� ��� ���� �Ǻ� ����
			���� ������ �ֻ��� ����� ��ȣ Ÿ�Կ� ���� �켱���� < ���� �и� �� ��ū�� ��ȣ Ÿ�Կ� ���� �켱���� Ȥ�� ������ �� ������,
			���� ������ �ֻ��� ����� ��ȣ Ÿ�Կ� ���� �켱������ ���� �и� �� ��ū�� ��ȣ Ÿ�Կ� ���� �켱������ ��

			ex : ((1 - 2 * 2) + 1) * 3
			=> ���ÿ� '('�� �����ϰ�, �ٽ� '('�� ���� ó�� : ���� ���ÿ� �����ϴ� '('�� �켱���� < ���ο� '('�� �켱���� => 3-2-2

				3-2-1) ���� ������ �ֻ��� ����� ��ȣ Ÿ�Կ� ���� �켱���� >= ���� �и� �� ��ū�� ��ȣ Ÿ�Կ� ���� �켱����
				: ���� ������ �ֻ��� ����� ��ȣ�� ���� ���Ǿ�� �ϹǷ�,
				���� ������ �ֻ��� ��带 ���ÿ��� ���� �ش� ��ȣ�� ��ȯ �� ���� ǥ����� ����μ� ���,
				���� �и� �� ��ū�� ��ȣ�� ���ÿ� ����

				3-2-2) ���� ������ �ֻ��� ����� ��ȣ Ÿ�Կ� ���� �켱���� < ���� �и� �� ��ū�� ��ȣ Ÿ�Կ� ���� �켱����
				: ���� �и� �� ��ū�� ��ȣ�� ���ÿ� ����,
				�켱������ ���� ��ȣ�� ���� ���� ����� ���� ���� ǥ����� ����μ� ��� ���� �Ǻ� ����

			3-3) ')' �� ���
			: ���� ���ÿ��� '(' �� ���� �� ���� ����� ��ȣ�� ������ ���������� ��ȯ �� ���� ǥ����� ����μ� ���

		4) ���ÿ� ���� ����� ��ȣ���� ���������� ��� ��ȯ �� ���� ǥ����� ����μ� ���
	***/

	if (srcInfixExpr == NULL || dstPostfixExpr == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	LinkedListStack* stack = NULL;

	unsigned int currentReadPos = 0; //������� ���� ��ġ
	unsigned int infixExprLength = strlen(srcInfixExpr); //���� ǥ��� ����
	Token token;

	LLS_CreateStack(&stack);

	while (currentReadPos < infixExprLength) //���� ǥ����� �� ���� ������
	{
		GenNextToken(&srcInfixExpr[currentReadPos], &token); //������� ���� ��ġ���� ��ū ����

		switch (token.symbolType)
		{
		case SYMBOL_TYPE::DOT:
			throw std::logic_error(std::string(__func__) + std::string(" : dot is part of operand"));

		case SYMBOL_TYPE::OPERAND:
			if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, token.str) != 0) //���� ǥ��Ŀ� ���
				throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

		case SYMBOL_TYPE::SPACE:
			if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, " ") != 0) //�ǿ����� �� ������ ���� ������ ���� ǥ��Ŀ� ���
				throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));
			break;

		case SYMBOL_TYPE::RIGHT_PARENTHESIS:
			while (!LLS_IsEmpty(&stack)) //���� ���ÿ��� '(' �� ���� �� ���� ����� ��ȣ�� ������ ���������� ��ȯ �� ���� ǥ����� ����μ� ���
			{
				Node* poppedNode = LLS_Pop(&stack);

				if (CharToSymbolType(poppedNode->data[0]) == SYMBOL_TYPE::LEFT_PARENTHESIS) //'(' �� ���� ���
				{
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
			break;

		default: //'(', �������� ���
			while (!LLS_IsEmpty(&stack))
			{
				SYMBOL_TYPE peekedNodeSymbolType = CharToSymbolType(LLS_Peek(&stack)->data[0]); //���� ������ �ֻ��� ����� �����ڿ� ���� ��ȣ Ÿ��

				if (GetSymbolTypePriority(peekedNodeSymbolType, true) >=
					GetSymbolTypePriority(token.symbolType, false)) //���� ������ �ֻ��� ����� ��ȣ Ÿ�Կ� ���� �켱���� >= ���� �и� �� ��ū�� ��ȣ Ÿ�Կ� ���� �켱����
				{
					Node* poppedNode = LLS_Pop(&stack);

					if (peekedNodeSymbolType != SYMBOL_TYPE::LEFT_PARENTHESIS) //'(' �� �ƴ� ���
					{
						if (strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, poppedNode->data) != 0) //���� ������ �ֻ��� ����� ��ȣ�� ���� ���Ǿ�� �ϹǷ�, ���� ǥ��Ŀ� ���
							throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));
					}

					LLS_DeallocateNode(&poppedNode);
				}
				else //���� ������ �ֻ��� ����� ��ȣ Ÿ�Կ� ���� �켱���� < ���� �и� �� ��ū�� ��ȣ Ÿ�Կ� ���� �켱����
				{
					break;
				}
			}

			LLS_Push(&stack, LLS_CreateNode(token.str)); //��ū�� ���ÿ� ����
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

	//TODO : ����Ʈ���� ���� ó�� �߰�
	//TODO : ���� ���⿡ ���� ���� �׽�Ʈ �ۼ�
}

/// <summary>
/// ��� ���� ǥ������κ��� ��� ��� ��ȯ
/// </summary>
/// <param name="srcPostfixExpr">��� ���� ǥ���</param>
/// <returns>��� ���� ǥ��Ŀ� ���� ��� ���</returns>
double CalcPostfixExpr(const char* srcPostfixExpr) throw(std::invalid_argument)
{
	/***
		< ���� ǥ��Ŀ� ���� ó�� �� ��� >

		! ���� ǥ��� : ���� ǥ������κ��� '(', ')' �� ������ �켱������ ���� ��ȯ
		! '.'�� �����ϴ� �Ҽ��� ��� '.'�� �ǿ����ڸ� ������ �Ҽ��� �ϳ��� �ǿ����� ��ū���� ó���Ͽ����Ƿ�, 2-2���� ó��

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
			Node* operandNode1 = LLS_Pop(&stack);
			Node* operandNode2 = LLS_Pop(&stack);
			double tmpResult = CalcOperation(atof(operandNode2->data), token.symbolType, atof(operandNode1->data)); //�߰� ��� ���
			char tmpResultBuffer[_CVTBUFSIZE] = { '\0', }; //�߰� ��� ��� ��ȯ ���� ����

			_gcvt_s(tmpResultBuffer, _CVTBUFSIZE, tmpResult, 10); //�ε� �Ҽ����� ���ڿ��� ��ȯ
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