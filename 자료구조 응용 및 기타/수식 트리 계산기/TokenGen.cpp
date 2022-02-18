#include "Core.h"

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
			srcCharDecAscii <= SingleNumToDecAscii(9))) //0 (dec ascii : 48) ~ 9 (dec ascii : 57) �� �ƴ� ���
			throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

		return SYMBOL_TYPE::OPERAND;
	}

	return (SYMBOL_TYPE)srcChar;
}

/// <summary>
/// ��� ���ڿ��� ��ȣ Ÿ������ ��ȯ
/// </summary>
/// <param name="srcStr">��� ���ڿ�</param>
/// <returns>��� ���ڿ��� ��ȣ Ÿ��</returns>
SYMBOL_TYPE StrToSymbolType(const char* srcStr)
{
	if (srcStr == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	size_t srcStrLen = strlen(srcStr); //��� ���ڿ��� ���� ('\0' ���� �� ����)
	if (srcStrLen == 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (0 size str)"));
	if (srcStrLen == 1) //���� ������ ���
		return CharToSymbolType((const char)srcStr[0]); //���� ���ڿ� ���� ��ȣ Ÿ�� ��ȯ ��� ��ȯ

	/***
		< ���� ���ڰ� �ƴ� �ǿ����� (�� �ڸ��� �ʰ��ϴ� ���� Ȥ�� '.'�� �����ϴ� �Ǽ�) �Ǻ� >

		1) GenNextToken �� '.'�� �����ϴ� �Ǽ��� ������ ����
	***/

	bool isDotAlreadyExists = false; //�Ǽ� ǥ���� ���� '.' �� ���� �� �� ���� ����

	for (size_t i = 0; i < srcStrLen; i++)
	{
		switch (CharToDecAscii((const char)srcStr[i])) //��� ���ڿ��� �� �ڸ��� ���� ���ڿ� ���� 10�� �ƽ�Ű �ڵ忡 ����
		{
		case (const int)SYMBOL_TYPE::DOT:
			if (isDotAlreadyExists ||
				!(i >= 1 && (i + 1) < srcStrLen)) //�̹� '.'�� �����ϰų�, ���� ������ ����, ���� ���ڰ� �������� ���� ���
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args ('.' already exists or invalid floating point)"));

			isDotAlreadyExists = true;
			break;

		default:
			if (!(CharToDecAscii((const char)srcStr[i]) >= SingleNumToDecAscii(0) &&
				CharToDecAscii((const char)srcStr[i]) <= SingleNumToDecAscii(9))) //0 (dec ascii : 48) ~ 9 (dec ascii : 57) �� �ƴ� ���
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));
			break;
		}
	}

	return SYMBOL_TYPE::OPERAND;
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
/// <param name="srcExpr">��� ǥ���� (��ü ���ڿ� Ȥ�� Slice �� ���ڿ�)</param>
/// <param name="dstToken">��� ǥ�������κ��� �����Ǿ� ��� �� ��ū</param>
/// <param name="exprReadDirection">��� ǥ���� �д� ����</param>
void GenNextToken(const char* srcExpr, Token* dstToken, EXPR_READ_DIRECTION exprReadDirection)
{
	if (srcExpr == NULL || dstToken == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (strlen(srcExpr) > sizeof(dstToken->_str) - 1) //��� ǥ������ ��� ������ '\0' ���� �� ũ�⸦ �ʰ� �� ���
		throw std::out_of_range(std::string(__func__) + std::string(" : out of range"));

	memset(dstToken->_str, '\0', sizeof(dstToken->_str));
	dstToken->_readCount = 0;

	/***
		< '.'�� �����ϴ� �Ǽ��� ���� >

		1) '.'�� '.'�� �����ϴ� �Ǽ����� �� ó���� �ܵ����� ���� �� �� ������,
		��� �ǿ����ڰ� �ּ� �� ���� ���� �� �������� ����

		2) '.'�� '.'�� �����ϴ� �Ǽ����� �ݵ�� �� ���� ����

		3) '.'�� �����ϴ� �Ǽ����� '.'�� ������ ��, �ǿ����ڰ� �ּ� �� ���� �ݵ�� �����Ͽ��� �ϰ�,
		�ּ� �� �� �̻��� �ǿ����ڰ� ���� �� �� �̻� �ǿ����ڰ� �������� ���� ���,
		�� ��ü�� �ϳ��� �Ǽ��� �����Ͽ� '.' �� �ǿ����ڸ� ���� �� �ϳ��� �ǿ����� ��ū���� ó��

		---

		< ��� ǥ�������κ��� ��ū ���� >

		! '.'�� �����ϴ� �Ǽ��� ��� '.'�� �ǿ����ڸ� ������ �Ǽ��� �ϳ��� �ǿ����� ��ū���� ó��

		1) ��� ǥ���Ŀ� ���� �� �аų�, ������� ���� ���ڿ� ���� ó���� ���Ͽ� �б� ������ �߻��ϴ� �������� �ݺ�

		2) ���� ���� ������ ��ȣ Ÿ�Կ� ����,

			2-1) ���� ���� ������ ��ȣ Ÿ���� �ǿ������� ���

				2-1-1) ���� ������ ���� ���ڰ� �����ϸ�, ���� ������ ���� ������ ��ȣ Ÿ���� �ǿ�����, �Ǽ� ǥ���� ���� '.'�� ���
				: ��� �б�

				2-1-2) ���� ������ ���� ���ڰ� �����ϸ�, ���� ������ ���� ������ ��ȣ Ÿ���� �ǿ�����, �Ǽ� ǥ���� ���� '.'�� �ƴ� ���
				: ������� ���� ���ڿ� ���� ó���� ���Ͽ� �б� ���� (�����ʿ��� �������� �о��� ���, ������� ���� ���� �¿����)

			2-2) ���� ���� ������ ��ȣ Ÿ���� �Ǽ� ǥ���� ���� '.'�� ���

				2-2-1) ���� ������ ����, ���� ���ڰ� �����ϸ�, ���� ������ ����, ���� ���ڰ� �ǿ������̸�
				: ��� �б�

				2-2-2) �̹� '.'�� �����ϰų�, ���� ������ ����, ���� ���ڰ� �������� �ʰų�, ���� ������ ����, ���� ���ڰ� �ǿ����ڰ� �ƴ� ���
				: �߸� �� ǥ���� ���� �߻�

			2-3) ���� ���� ������ ��ȣ Ÿ���� ' ' (�ǿ����� �� ������ ���� ����), '(', ')', �������� ���
			: ���� ���� ���ڿ� ���� ó���� ���Ͽ� �б� ����
	***/

	size_t srcExprLen = strlen(srcExpr); //��� ǥ������ ���� ('\0' ���� �� ����)
	size_t srcExprNextReadIndex =
		(exprReadDirection == EXPR_READ_DIRECTION::LEFT_TO_RIGHT) ? 0 : //���� ��ġ : ��� ���ڿ��� ����
		(exprReadDirection == EXPR_READ_DIRECTION::RIGHT_TO_LEFT) ? (srcExprLen - 1) : //���� ��ġ : ��� ���ڿ��� ��
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args")); //��� ǥ������ ������ ���� �ε���
	bool isDotAlreadyExists = false; //�Ǽ� ǥ���� ���� '.' �� ���� �� �� ���� ����

	while (dstToken->_readCount < srcExprLen) //��� ǥ������ �� ���� �� ����
	{
		dstToken->_str[dstToken->_readCount] = srcExpr[srcExprNextReadIndex]; //�� ���ھ� ó��
		dstToken->_symbolType = CharToSymbolType(srcExpr[srcExprNextReadIndex]); //���� ���ڿ� ���� ��ȣ Ÿ�� �Ҵ�
		dstToken->_readCount++; //���� ���� ���� ����

		srcExprNextReadIndex =
			(exprReadDirection == EXPR_READ_DIRECTION::LEFT_TO_RIGHT) ? srcExprNextReadIndex + 1 :
			(exprReadDirection == EXPR_READ_DIRECTION::RIGHT_TO_LEFT) ? srcExprNextReadIndex - 1 :
			throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args")); //��� ǥ������ ������ ���� ��ġ �Ҵ�

		switch (dstToken->_symbolType)
		{
		case SYMBOL_TYPE::OPERAND:
			if (dstToken->_readCount < srcExprLen) //���� ������ ���� ���ڰ� ���� �� ���
			{
				switch (CharToSymbolType(srcExpr[srcExprNextReadIndex]))
				{
				case SYMBOL_TYPE::OPERAND:
				case SYMBOL_TYPE::DOT:
					break; //���� ������ ���� ������ ��ȣ Ÿ���� �ǿ�����, �Ǽ� ǥ���� ���� '.'�̸� ��� �б�

				default: //���� ������ ���� ������ ��ȣ Ÿ���� '(', ')', ������, �ǿ����� �� ������ ���� ������ ���
					if (exprReadDirection == EXPR_READ_DIRECTION::RIGHT_TO_LEFT) //�����ʿ��� �������� �о��� ���
						ReverseInplaceStr(dstToken->_str); //������� ���� ���ڿ��� ���� �¿����

					goto END_PROC; //������� ���� ���ڿ� ���� ó���� ���Ͽ� �б� ����
				}
			}

			break;

		case SYMBOL_TYPE::DOT:
			if (isDotAlreadyExists ||
				!(dstToken->_readCount >= 2 && dstToken->_readCount < srcExprLen)) //�̹� '.'�� �����ϰų�, ���� ������ ����, ���� ���ڰ� �������� ���� ���
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcExpr)"));

			isDotAlreadyExists = true;

			if (!(CharToSymbolType(dstToken->_str[dstToken->_readCount - 2]) == SYMBOL_TYPE::OPERAND &&
				CharToSymbolType(srcExpr[srcExprNextReadIndex]) == SYMBOL_TYPE::OPERAND)) //���� ������ ����, ���� ������ ��ȣ Ÿ���� �ǿ����ڰ� �ƴ� ���
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcExpr)"));

			break; //���� ������ ����, ���� ���ڰ� �����ϸ�, ���� ������ ����, ���� ���ڰ� �ǿ������̸� ��� �б�

		default: //' ' (�ǿ����� �� ������ ���� ����), '(', ')', �������� ���
			goto END_PROC; //���� ���� ���ڿ� ���� ó���� ���Ͽ� �б� ����
		}
	}

END_PROC: //do nothing
	return;
}