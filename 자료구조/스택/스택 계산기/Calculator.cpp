#include "Core.h"

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
/// ��� 0~9 ���ڸ� 10�� �ƽ�Ű �ڵ�� ��ȯ
/// </summary>
/// <param name="srcNum">��� 0~9 ����</param>
/// <returns>��ȯ �� 10�� �ƽ�Ű �ڵ�</returns>
inline int IntToDecAscii(int srcNum) throw(std::invalid_argument)
{
	if (srcNum < 0 || srcNum > 9)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	return '0' + srcNum;
}

/// <summary>
/// ��� ������ ��ȣ Ÿ�� ��ȯ
/// </summary>
/// <param name="srcChar"></param>
/// <returns></returns>
SYMBOL_TYPE GetSymbolTypeFromChar(char srcChar) throw(std::invalid_argument)
{
	int decAscii = CharToDecAscii(srcChar); //��� ������ 10�� �ƽ�Ű �ڵ�

	switch (srcChar) //��� ���ڿ� ����
	{
	case (const char)SYMBOL_TYPE::LEFT_PARENTHESIS:
	case (const char)SYMBOL_TYPE::RIGHT_PARENTHESIS:
	case (const char)SYMBOL_TYPE::PLUS:
	case (const char)SYMBOL_TYPE::MINUS:
	case (const char)SYMBOL_TYPE::MULTIPLY:
	case (const char)SYMBOL_TYPE::DIVIDE:
	case (const char)SYMBOL_TYPE::SPACE:
		break;

	default: //�ǿ����� Ȥ�� �߸� �� �Է�
		if (!((decAscii >= IntToDecAscii(0) && decAscii <= IntToDecAscii(9)) ||
			(decAscii == CharToDecAscii('.')))) //0(48) ~ 9(57) �� '.'(46)�� �ƴ� ��� (�ǿ����ڰ� �ƴ� ���)
			throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

		return SYMBOL_TYPE::OPERAND;
	}

	return (SYMBOL_TYPE)srcChar;
}

/// <summary>
/// ��� ��ȣ�� ���� ������ �켱���� ��ȯ
/// </summary>
/// <param name="srcSymbol">��� ��ȣ</param>
/// <param name="isInStack">��� ��ȣ�� ���� �� ���� ����</param>
/// <returns>�켱���� (Ŭ ���� ���� ���� �켱����)</returns>
int GetSymbolTypePriority(SYMBOL_TYPE srcSymbol, bool isInStack)
{
	int priority = -1; //�켱���� (Ŭ ���� ���� ���� �켱����)

	switch (srcSymbol)
	{
	case SYMBOL_TYPE::LEFT_PARENTHESIS:
		/***
			���� ���ÿ� '('�� �����ϴ� ��Ȳ��, ���ο� '('�� �Է� �Ǿ��� ���, ���ο� '('�� �� ���� �켱������ ������.
			'('�� ���� ���ÿ� �������� ���� ���, �ٸ� ��� �����ں��ٵ� �ֿ켱���� �켱������ ������.
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

	default: //')' �� ���鿡 ���� ��� ó���� ���Ͽ� ������ �켱����
		break;
	}

	return priority;
}

/// <summary>
/// ��� ǥ�������κ��� ��� ��ū�� ��ū ����
/// </summary>
/// <param name="srcExpr">��� ǥ����</param>
/// <returns>��� ǥ�������κ��� ���� �� ��ū</returns>
void CreateNextToken(const char* srcExpr, Token* dstToken) throw(std::invalid_argument, std::out_of_range)
{
	if (srcExpr == NULL || dstToken == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (strlen(srcExpr) > sizeof(dstToken->str) - 1) //'\0' ���� �� ũ�⸦ �ʰ� �� ���
		throw std::out_of_range(std::string(__func__) + std::string(" : out of range"));

	memset(dstToken->str, '\0', sizeof(dstToken->str));
	dstToken->readCount = 0;

	/***
		1) ���� ���� ���ڰ� �ǿ����� (���� Ȥ�� �Ҽ� ǥ���� ���� '.' ��ȣ)�� ���, ���� ������ ���� ���ڰ� �����ϸ� ���� ������ ���� ���ڰ� 
		�ǿ����ڰ� �ƴ� ��� ���ÿ� ���� ���Ͽ� �б� ����
		
		2) ���� ���� ���ڰ� �ǿ����� (���� Ȥ�� �Ҽ� ǥ���� ���� '.' ��ȣ)�� �ƴ� ���, ���ÿ� ���� ���Ͽ� �б� ����
		---
		ex) (1.0 * 2.0) + 3.0
		
		1) '(' �� ')' �� ���� ���ÿ� ���� ���� �б� ����
		
		2) �������� * �� + �� ���� ���ÿ� ���� ���� �б� ����
		
		3) �ǿ����ڿ� ���� �ǿ����ڰ� �ƴ� ��ȣ Ȥ�� �����ڸ� ���� ������ �б�
	***/

	while (dstToken->readCount < strlen(srcExpr))
	{
		dstToken->str[dstToken->readCount] = srcExpr[dstToken->readCount]; //���ʺ��� �� ���ھ� ó��
		dstToken->symbolType = GetSymbolTypeFromChar(srcExpr[dstToken->readCount]); //���� ���ڿ� ���� ��ȣ Ÿ�� �Ҵ�
		dstToken->readCount++;

		if (dstToken->symbolType == SYMBOL_TYPE::OPERAND) //���� ���� ���ڰ� �ǿ����� (���� Ȥ�� �Ҽ� ǥ���� ���� '.' ��ȣ)�� ���
		{
			if (dstToken->readCount < strlen(srcExpr) && 
				GetSymbolTypeFromChar(srcExpr[dstToken->readCount]) != SYMBOL_TYPE::OPERAND) //���� ������ ���� ���ڰ� �����ϸ�, ���� ������ ���� ���ڰ� �ǿ����ڰ� �ƴ� ���
				break; //���ÿ� ���� ���Ͽ� �б� ����
		}
		else //���� ���� ���ڰ� �ǿ����� (���� Ȥ�� �Ҽ� ǥ���� ���� '.' ��ȣ)�� �ƴ� ���
		{
			break; //���ÿ� ���� ���Ͽ� �б� ����
		}	
	}
}

/// <summary>
/// ��� ���� ǥ������κ��� ���� ǥ��� ����
/// </summary>
/// <param name="srcInfixExpr">��� ���� ǥ���</param>
/// <param name="dstPostfixExpr">��� ���� ǥ������κ��� ��ȯ �Ǿ� ��� �� ���� ǥ���</param>
void GenPostfixExpr(const char* srcInfixExpr, char* dstPostfixExpr) throw(std::invalid_argument)
{
	/***
		< ���� ǥ����� ���� ǥ������� ��ȯ >
		! ���� ǥ��Ŀ��� ���� �켱������ ���� ��ȣ�� �ֿ켱������ ó���Ͽ��� ��
		! '(' �� ')'�� ��ȯ �� ���� ǥ����� ����μ� ������� ����

		1) ����ڷκ��� ���� ǥ��� �Է�

		2) ���ʺ��� ���������� ��ū �и� (��ȣ, ����, �ǿ����� (���� Ȥ�� �Ҽ� ǥ���� ���� '.' ��ȣ), ������)

		3) �и� �� ��ū�� ��ȣ Ÿ�Կ� ����,
			3-1) �ǿ����� (���� Ȥ�� �Ҽ� ǥ���� ���� '.' ��ȣ)�� ���
			: ���ÿ� �������� �ʰ� ��ȯ �� ���� ǥ����� ����μ� ���

			3-2) '(' Ȥ�� �������� ���
			: ���� ǥ����� ����μ� ��� ���� �Ǻ� ����
			���� ������ �ֻ��� ����� ������ �켱���� < ��ū�� �켱���� Ȥ�� ������ �� ������,
			���� ������ �ֻ��� ��忡 �ִ� ������ �켱������ ��ū�� �켱���� ��

			ex : ((1 - 2 * 2) + 1) * 3
			=> ���ÿ� '('�� �����ϰ�, �ٽ� '('�� ���� ó�� : ���� ���ÿ� �����ϴ� '('�� �켱���� < ���ο� '('�� �켱���� => 3-2-2

				3-2-1) ���� ������ �ֻ��� ����� ������ �켱���� >= ��ū�� �켱����
				: ���� ������ �ֻ��� ��带 ���ÿ��� ���� ��ȯ �� ���� ǥ����� ����μ� ��� �� ��ū�� ���ÿ� ����

				3-2-2) ���� ������ �ֻ��� ����� ������ �켱���� < ��ū�� �켱����
				: ��ū�� ���ÿ� ���� �� ���� ǥ����� ����μ� ��� ���� �Ǻ� ����
			
			3-3) ')' ��ȣ�� ���
			: ���ÿ��� '(' �� ���� �� ���� ������ ���������� ��ȯ �� ���� ǥ����� ����μ� ���

		4) ���ÿ� ���� ��带 ���������� ��� ��ȯ �� ���� ǥ����� ����μ� ���
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
		CreateNextToken(&srcInfixExpr[currentReadPos], &token); //������� ���� ��ġ���� ��ū ����

		switch (token.symbolType)
		{
		case SYMBOL_TYPE::OPERAND: //�ǿ����� (���� Ȥ�� �Ҽ� ǥ���� ���� '.' ��ȣ)�� ���
			if(strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, token.str) != 0) //���� ǥ��Ŀ� ���
				throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));
			break;

		case SYMBOL_TYPE::RIGHT_PARENTHESIS: //')' �� ���
			while (!LLS_IsEmpty(&stack)) //���ÿ��� '(' �� ���������� ������ ���� ǥ��Ŀ� ���
			{
				Node* poppedNode = LLS_Pop(&stack);

				if (GetSymbolTypeFromChar(poppedNode->data[0]) == SYMBOL_TYPE::LEFT_PARENTHESIS) //'(' �� ���� ���
				{
					LLS_DeallocateNode(&poppedNode);
					break;
				}
				else //�ǿ����� (���� Ȥ�� �Ҽ� ǥ���� ���� '.' ��ȣ) Ȥ�� �������� ���
				{
					if(strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, poppedNode->data) != 0) //���� ǥ��Ŀ� ���
						throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

					LLS_DeallocateNode(&poppedNode);
				}
			}
			break;

		default: //'(', ������, ������ ���
			while (!LLS_IsEmpty(&stack))
			{
				SYMBOL_TYPE peekedNodeSymbolType = GetSymbolTypeFromChar(LLS_Peek(&stack)->data[0]); //���� ������ �ֻ��� ����� ������

				if (GetSymbolTypePriority(peekedNodeSymbolType, true) >=
					GetSymbolTypePriority(token.symbolType, false)) //���� ������ �ֻ��� ����� ������ �켱���� >= ��ū�� �켱����
				{
					if (peekedNodeSymbolType != SYMBOL_TYPE::LEFT_PARENTHESIS) //'(' �� �ƴ� ���
					{
						Node* poppedNode = LLS_Pop(&stack);

						if(strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, poppedNode->data) != 0) //���� ǥ��Ŀ� ���
							throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

						LLS_DeallocateNode(&poppedNode);
					}
				}
				else //���� ������ �ֻ��� ����� ������ �켱���� < ��ū�� �켱����
				{
					break;
				}
			}

			LLS_Push(&stack, LLS_CreateNode(token.str)); //��ū�� ���ÿ� ����
			break;
		}

		currentReadPos += token.readCount; //������ ���� ��ġ���� �ٽ� ��ū ����
	}

	while (!LLS_IsEmpty(&stack)) //���ÿ� ���� ��带 ���������� ��� ��ȯ �� ���� ǥ����� ����μ� ���
	{
		Node* poppedNode = LLS_Pop(&stack);

		if (GetSymbolTypeFromChar(poppedNode->data[0]) != SYMBOL_TYPE::LEFT_PARENTHESIS) //'(' �� �ƴ� ���
			if(strcat_s(dstPostfixExpr, MAX_STR_LEN - 1, poppedNode->data) != 0) //���� ǥ��Ŀ� ���
				throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

		LLS_DeallocateNode(&poppedNode);
	}

	LLS_DeallocateLinkedListStack(&stack);

	//TODO :����Ʈ���� ���� ó�� �߰�
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
		1) ���ʺ��� ���������� ��ū �и� (�ǿ�����, ������)
		ex: 1 3 - 2 *

		2) ��ū Ÿ�Կ� ����,
			2-1) �ǿ������� ���
			: ���ÿ� ����

			2-2) �������� ���
			: ���ÿ��� 2ȸ ���� �� ��� ���� �� ��� ����� �ٽ� ���ÿ� ����

		3) ������ ������ �о��� ��� ���� ���ÿ� �����ִ� ��� ��� ��ȯ

	***/


	return 0.0;
}