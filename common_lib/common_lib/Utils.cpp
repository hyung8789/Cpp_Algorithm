#include "Common_LIB_Core.h"

/// <summary>
/// ��� ���ڿ� ���ڸ� �¿����
/// </summary>
/// <param name="targetStr">��� ���ڿ�</param>
void ReverseInplaceStr(char targetStr[])
{
	if (targetStr == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	size_t targetStrLen = strlen(targetStr); //��� ���ڿ��� ���� ('\0' ���� �� ����)

	for (size_t i = 0; i < (targetStrLen / 2); i++)
	{
		char tmp = targetStr[i];
		targetStr[i] = targetStr[targetStrLen - i - 1];
		targetStr[targetStrLen - i - 1] = tmp;
	}
}

/// <summary>
/// �ε� �Ҽ������� �̷���� ��� ���ڿ��� �ε� �Ҽ������� ��ȯ
/// </summary>
/// <param name="srcStrOfDouble">�ε� �Ҽ������� �̷���� ��� ���ڿ�</param>
/// <returns>�ε� �Ҽ������� �̷���� ��� ���ڿ��κ��� ��ȯ �� �ε� �Ҽ���</returns>
double StrToDouble(const char* srcStrOfDouble)
{
	double retVal = 0.0;
	char* endAddr; //��� ���ڿ��κ��� ���������� ��ȯ �� �� ���� ������ �ּ�

	retVal = strtod(srcStrOfDouble, &endAddr);

	/***
		1) ��ȯ �Ϸ� �� ���ڿ��� ������ ���� ���, ��� ���ڿ��� �ڿ� �߸� �� ������ ����
		2) ��� ���ڿ��� ���ڷ� �������� ���� ���, ��ȯ �Ұ�
		3) strtod�� ��ȯ�� �����Ͽ��� �׻� 0�� ��ȯ
	***/

	if ((*endAddr) != '\0' || endAddr == srcStrOfDouble)
		throw std::invalid_argument(std::string(__func__) +
			std::string(" : Invalid Args (junk data exists or not a numeric value)"));

	return retVal;
}

/// <summary>
/// ��� ���ڸ� 10�� �ƽ�Ű �ڵ�� ��ȯ
/// </summary>
/// <param name="srcChar">��� ����</param>
/// <returns>��ȯ �� 10�� �ƽ�Ű �ڵ�</returns>
int CharToDecAscii(char srcChar)
{
	return (int)srcChar;
}

/// <summary>
/// ��� 0~9 ������ ���� ���ڸ� 10�� �ƽ�Ű �ڵ�� ��ȯ
/// </summary>
/// <param name="srcSingleNum">��� 0~9 ������ ���� ����</param>
/// <returns>��ȯ �� 10�� �ƽ�Ű �ڵ�</returns>
int SingleNumToDecAscii(int srcSingleNum)
{
	if (srcSingleNum < 0 || srcSingleNum > 9)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	return '0' + srcSingleNum;
}