#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <string>

/// <summary>
/// 연속적인 요소들에 대한 접근 방법
/// </summary>
enum class ELEMENT_ORDER : const int
{
	ROW_MAJOR = 0, //행 우선순위
	COL_MAJOR //열 우선순위
};

namespace utils
{
	void ReverseInplaceStr(char[]);
	double StrToDouble(const char*);
	int CharToDecAscii(char);
	int SingleNumToDecAscii(int);

	size_t TwoDimensionIndexToOneDimensionIndex(size_t, size_t,
		size_t, size_t, ELEMENT_ORDER = ELEMENT_ORDER::ROW_MAJOR);
}
#endif