#ifndef _SORT_HPP_
#define _SORT_HPP_

enum class ORDER_BY :const int
{
	ASCENDING, //오름차순
	DESCENDING //내림차순
}; //정렬 모드

/// <summary>
/// 버블 정렬
/// </summary>
/// <typeparam name="SortElementType">정렬 요소 타입</typeparam>
/// <param name="srcEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="srcEnumerableSetSize">순차적으로 열거 가능 한 요소들의 집합의 크기</param>
/// <param name="orderBy">정렬 모드</param>
template<typename SortElementType>
void BubbleSort(SortElementType srcEnumerableSet[], size_t srcEnumerableSetSize, ORDER_BY orderBy)
{
}
#endif