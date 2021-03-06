#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <string>
#include <math.h>
#include <random>

typedef signed long long ssize_t;

/// <summary>
/// 연속적인 요소들에 대한 접근 방법
/// </summary>
enum class ELEMENT_ORDER : const int
{
	ROW_MAJOR = 0, //행 우선순위
	COL_MAJOR //열 우선순위
};

/// <summary>
/// 기타 유틸 함수
/// </summary>
namespace utils
{
	void ReverseInplaceStr(char[]);
	double StrToDouble(const char*);
	int CharToDecAscii(char);
	int SingleNumToDecAscii(int);
	int StrCompareByNaturalSortOrder(const char*, const char*);

	size_t GetBitCountFrom(size_t);

	void GenRandStr(char[], size_t, size_t);
	ssize_t GenSignedRandNum(ssize_t, ssize_t);
	size_t GenUnsignedRandNum(size_t, size_t);

	size_t TwoDimensionIndexToOneDimensionIndex(size_t, size_t,
		size_t, size_t, ELEMENT_ORDER = ELEMENT_ORDER::ROW_MAJOR);
}
#endif