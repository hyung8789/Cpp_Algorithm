#include "Core.h"

/// <summary>
/// ��� ���ڿ� ���ڸ� �¿����
/// </summary>
/// <param name="targetStr">��� ���ڿ�</param>
/// <param name="targetStrLen">��� ���ڿ��� ���� ('\0' ���� �� ����)</param>
void ReverseInplaceStr(char* targetStr, size_t targetStrLen)
{
	if (targetStr == NULL || targetStrLen == 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	for (size_t i = 0; i < (targetStrLen / 2); i++)
	{
		char tmp = targetStr[i];
		targetStr[i] = targetStr[targetStrLen - i - 1];
		targetStr[targetStrLen - i - 1] = tmp;
	}
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
void GenNextToken(const char* srcExpr, Token* dstToken, EXPR_READ_DIRECTION exprReadDirection)
{
	if (srcExpr == NULL || dstToken == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (strlen(srcExpr) > sizeof(dstToken->str) - 1) //'\0' ���� �� ũ�⸦ �ʰ� �� ���
		throw std::out_of_range(std::string(__func__) + std::string(" : out of range"));

	memset(dstToken->str, '\0', sizeof(dstToken->str));
	dstToken->readCount = 0;

	//TODO : �д� ���� ����, �д� ������ ������ ��� 
	//2-1) ���� ���� ������ ��ȣ Ÿ���� �ǿ������� ��쿡��
	//���� ������ ���� ���ڰ� �����ϸ�, ���� ������ ���� ������ ��ȣ Ÿ���� �ǿ�����, �Ǽ� ǥ���� ���� '.'�� �ƴ� ���
	//������� ���� ���ڿ� ���� ó���� ���Ͽ� �б� ���� ������ ���ڿ� �¿���� ����
	//�� ��, ������ ó�������� ������ ������ ��� ����

	/***
		< ��� ǥ�������κ��� ��ū ���� >

		1) ��� ǥ���Ŀ� ���� �� �аų�, ������� ���� ���ڿ� ���� ó���� ���Ͽ� �б� ������ �߻��ϴ� ���� ������ �ݺ�

		2) ���� ���� ������ ��ȣ Ÿ�Կ� ����,

			2-1) ���� ���� ������ ��ȣ Ÿ���� �ǿ������� ���
			: ���� ������ ���� ���ڰ� �����ϸ�, ���� ������ ���� ������ ��ȣ Ÿ���� �ǿ�����, �Ǽ� ǥ���� ���� '.'�̸� ��� �б�
			���� ������ ���� ���ڰ� �����ϸ�, ���� ������ ���� ������ ��ȣ Ÿ���� �ǿ�����, �Ǽ� ǥ���� ���� '.'�� �ƴ� ��� ������� ���� ���ڿ� ���� ó���� ���Ͽ� �б� ����
			('.'�� �����ϴ� �Ǽ��� ��� '.'�� �ǿ����ڸ� ������ �Ǽ��� �ϳ��� �ǿ����� ��ū���� ó��)

			2-2) ���� ���� ������ ��ȣ Ÿ���� �Ǽ� ǥ���� ���� '.'�� ���
			: ���� ������ ����, ���� ���ڰ� �����ϸ�, ���� ������ ����, ���� ���ڰ� �ǿ������̸� ��� �б�
			���� ������ ����, ���� ���ڰ� �������� �ʰų�, ���� ������ ����, ���� ���ڰ� �ǿ����ڰ� �ƴ� ��� �߸� �� ǥ���� ���� �߻�
			'.'�� �̹� ������ �����Ͽ��� ��� �߸� �� ǥ���� ���� �߻�

			2-3) ���� ���� ������ ��ȣ Ÿ���� '(', ')', ������, �ǿ����� �� ������ ���� ������ ���
			: ���� ���� ���ڿ� ���� ó���� ���Ͽ� �б� ����

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
			if (isDotAlreadyExists ||
				!(dstToken->readCount >= 2 && dstToken->readCount < strlen(srcExpr))) //�̹� '.'�� �����ϰų�, ���� ������ ����, ���� ���ڰ� �������� ���� ���
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcExpr)"));

			isDotAlreadyExists = true;

			if (!(CharToSymbolType(dstToken->str[dstToken->readCount - 2]) == SYMBOL_TYPE::OPERAND &&
				CharToSymbolType(srcExpr[dstToken->readCount]) == SYMBOL_TYPE::OPERAND)) //���� ������ ����, ���� ������ ��ȣ Ÿ���� �ǿ����ڰ� �ƴ� ���
				throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (wrong srcExpr)"));

			break; //���� ������ ����, ���� ���ڰ� �����ϸ�, ���� ������ ����, ���� ���ڰ� �ǿ������̸� ��� �б�

		default: //'(', ')', ������, �ǿ����� �� ������ ���� ������ ���
			goto END_PROC; //���ÿ� ���� ���Ͽ� �б� ����
		}
	}

END_PROC: //do nothing
	return;
}