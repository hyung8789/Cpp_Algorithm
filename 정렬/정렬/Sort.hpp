#ifndef _SORT_HPP_
#define _SORT_HPP_

enum class ORDER_BY :const int
{
	ASCENDING, //��������
	DESCENDING //��������
}; //���� ���

/// <summary>
/// ���� ����
/// </summary>
/// <typeparam name="SortElementType">���� ��� Ÿ��</typeparam>
/// <param name="srcEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="srcEnumerableSetSize">���������� ���� ���� �� ��ҵ��� ������ ũ��</param>
/// <param name="orderBy">���� ���</param>
template<typename SortElementType>
void BubbleSort(SortElementType srcEnumerableSet[], size_t srcEnumerableSetSize, ORDER_BY orderBy)
{
}
#endif