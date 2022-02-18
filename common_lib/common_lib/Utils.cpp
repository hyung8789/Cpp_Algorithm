#include "Common_LIB_Core.h"

/// <summary>
/// 대상 문자열 제자리 좌우반전
/// </summary>
/// <param name="targetStr">대상 문자열</param>
void ReverseInplaceStr(char targetStr[])
{
	if (targetStr == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	size_t targetStrLen = strlen(targetStr); //대상 문자열의 길이 ('\0' 제외 한 길이)

	for (size_t i = 0; i < (targetStrLen / 2); i++)
	{
		char tmp = targetStr[i];
		targetStr[i] = targetStr[targetStrLen - i - 1];
		targetStr[targetStrLen - i - 1] = tmp;
	}
}

/// <summary>
/// 부동 소수점으로 이루어진 대상 문자열을 부동 소수점으로 변환
/// </summary>
/// <param name="srcStrOfDouble">부동 소수점으로 이루어진 대상 문자열</param>
/// <returns>부동 소수점으로 이루어진 대상 문자열로부터 변환 된 부동 소수점</returns>
double StrToDouble(const char* srcStrOfDouble)
{
	double retVal = 0.0;
	char* endAddr; //대상 문자열로부터 성공적으로 변환 후 그 다음 문자의 주소

	retVal = strtod(srcStrOfDouble, &endAddr);

	/***
		1) 변환 완료 후 문자열이 끝나지 않을 경우, 대상 문자열의 뒤에 잘못 된 데이터 존재
		2) 대상 문자열이 숫자로 시작하지 않을 경우, 변환 불가
		3) strtod는 변환을 실패하여도 항상 0을 반환
	***/

	if ((*endAddr) != '\0' || endAddr == srcStrOfDouble)
		throw std::invalid_argument(std::string(__func__) +
			std::string(" : Invalid Args (junk data exists or not a numeric value)"));

	return retVal;
}

/// <summary>
/// 대상 문자를 10진 아스키 코드로 변환
/// </summary>
/// <param name="srcChar">대상 문자</param>
/// <returns>변환 된 10진 아스키 코드</returns>
int CharToDecAscii(char srcChar)
{
	return (int)srcChar;
}

/// <summary>
/// 대상 0~9 범위의 단일 숫자를 10진 아스키 코드로 변환
/// </summary>
/// <param name="srcSingleNum">대상 0~9 범위의 단일 숫자</param>
/// <returns>변환 된 10진 아스키 코드</returns>
int SingleNumToDecAscii(int srcSingleNum)
{
	if (srcSingleNum < 0 || srcSingleNum > 9)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	return '0' + srcSingleNum;
}