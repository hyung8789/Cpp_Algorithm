#include "Utils.h"

/// <summary>
/// 대상 문자열 제자리 좌우반전
/// </summary>
/// <param name="srcTargetStr">대상 문자열</param>
void utils::ReverseInplaceStr(char srcTargetStr[])
{
	if (srcTargetStr == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	size_t targetStrLen = strlen(srcTargetStr); //대상 문자열의 길이 ('\0' 제외 한 길이)

	for (size_t i = 0; i < (targetStrLen / 2); i++)
	{
		char tmp = srcTargetStr[i];
		srcTargetStr[i] = srcTargetStr[targetStrLen - i - 1];
		srcTargetStr[targetStrLen - i - 1] = tmp;
	}
}

/// <summary>
/// 부동 소수점으로 이루어진 대상 문자열을 부동 소수점으로 변환
/// </summary>
/// <param name="srcStrOfDouble">부동 소수점으로 이루어진 대상 문자열</param>
/// <returns>부동 소수점으로 이루어진 대상 문자열로부터 변환 된 부동 소수점</returns>
double utils::StrToDouble(const char* srcStrOfDouble)
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
int utils::CharToDecAscii(char srcChar)
{
	return (int)srcChar;
}

/// <summary>
/// 대상 0~9 범위의 단일 숫자를 10진 아스키 코드로 변환
/// </summary>
/// <param name="srcSingleNum">대상 0~9 범위의 단일 숫자</param>
/// <returns>변환 된 10진 아스키 코드</returns>
int utils::SingleNumToDecAscii(int srcSingleNum)
{
	if (srcSingleNum < 0 || srcSingleNum > 9)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	return '0' + srcSingleNum;
}

/// <summary>
/// 대상 숫자의 비트 개수 반환
/// </summary>
/// <param name="srcNum">대상 숫자</param>
/// <returns>대상 숫자의 비트 개수</returns>
size_t utils::GetBitCountFrom(size_t srcNum)
{
	return (1 + log2(srcNum)); //<< : *2, >> : /2 이므로, 2^0 자리를 포함하여 계산
}
/// <summary>
/// 임의의 문자열 생성
/// </summary>
/// <param name="dstStr">출력 문자열</param>
/// <param name="dstStrSize">출력 문자열 버퍼의 바이트 단위 크기</param>
/// <param name="randStrLength">출력 문자열 버퍼의 바이트 단위의 크기보다 작은 바이트 단위의 생성 될 문자열의 길이</param>
void utils::GenRandStr(char dstStr[], size_t dstStrSize, size_t randStrLength)
{
	if (dstStrSize == 0 || randStrLength == 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (randStrLength >= dstStrSize)
		throw std::out_of_range(std::string(__func__) + std::string(" : out of range"));

	// https://www.asciitable.com/
	const char decAsciiMinRangeValue = utils::CharToDecAscii(' '); //32 (space)
	const char decAsciiMaxRangeValue = utils::CharToDecAscii('~'); //126 (~)

	std::random_device rand_device; //비결정적 생성기
	std::mt19937 gen(rand_device()); //메르센 트위스터에 시드 할당
	std::uniform_int_distribution<short> dist(decAsciiMinRangeValue, decAsciiMaxRangeValue); //균일 이산 분포

	for (size_t i = 0; i < randStrLength; i++)
		dstStr[i] = dist(gen);

	dstStr[randStrLength] = '\0';
}

/// <summary>
/// 2차원 요소 인덱스를 1차원 요소 인덱스로 변환
/// </summary>
/// <param name="srcRowElementCount">행에 대한 전체 요소 개수</param>
/// <param name="srcColElementCount">열에 대한 전체 요소 개수</param>
/// <param name="targetRowIndex">대상 2차원 행에 대한 인덱스</param>
/// <param name="targetColIndex">대상 2차원 열에 대한 인덱스</param>
/// <param name="elementOrder">연속적인 요소에 대한 접근 방법</param>
/// <returns>변환 된 1차원 요소 인덱스</returns>
size_t utils::TwoDimensionIndexToOneDimensionIndex(size_t srcRowElementCount, size_t srcColElementCount,
	size_t targetRowIndex, size_t targetColIndex, ELEMENT_ORDER elementOrder)
{
	// https://www.geeksforgeeks.org/emulating-a-2-d-array-using-1-d-array/
	// https://en.wikipedia.org/wiki/Row-_and_column-major_order

	/***
		< Row-Major Order >

		1 [0]	2 [1]	3 [2]	4 [3]	5 [4]	6 [5]

		<= 실제 저장 된 요소
		=> 2차원 요소로 취급하여 접근 (1-D Index : targetColIndex + (targetRowIndex * srcColElementCount))

		1 [0, 0]	2 [0, 1]	3 [0, 2]
		4 [1, 0]	5 [1, 1]	6 [1, 2]

		---

		< Col-Major Order >

		1 [0]	4 [1]	2 [2]	5 [3]	3 [4]	6 [5]

		<= 실제 저장 된 요소
		=> 2차원 요소로 취급하여 접근 (1-D Index : targetRowIndex + (targetColIndex * srcRowElementCount))

		1 [0, 0]	2 [0, 1]	3 [0, 2]
		4 [1, 0]	5 [1, 1]	6 [1, 2]
	***/

	if (targetRowIndex >= srcRowElementCount || targetColIndex >= srcColElementCount)
		throw std::out_of_range(std::string(__func__) + std::string(" : out of range (") +
			std::string(std::to_string(targetRowIndex) + ", " + std::to_string(srcRowElementCount) + ", " +
				std::to_string(targetColIndex) + ", " + std::to_string(srcColElementCount) + ")"));

	switch (elementOrder)
	{
	case ELEMENT_ORDER::ROW_MAJOR:
		return targetColIndex + (targetRowIndex * srcColElementCount);

	case ELEMENT_ORDER::COL_MAJOR:
		return targetRowIndex + (targetColIndex * srcRowElementCount);

	default:
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));
	}
}