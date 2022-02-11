#include "Core.h"

/// <summary>
/// 자신의 고유 인스턴스 참조 반환
/// </summary>
/// <typeparam name="SortElementType">정렬 요소 타입</typeparam>
/// <returns>자신의 고유 인스턴스 참조</returns>
SORT_MAPPER& SORT_MAPPER::GetInstance()
{
	if (_instance == NULL)
		_instance = new SORT_MAPPER();

	return *_instance;
}

const char* SORT_MAPPER::UniqueMappedIndexToSortFuncNameStr(SORT_UNIQUE_MAPPED_INDEX index)
{
	return nullptr;
}

int SORT_MAPPER::SortFuncNameStrToUniqueMappedIndex(const char* sortFuncNameStr)
{
	return 0;
}

SortFuncType<MySortElementType> SORT_MAPPER::GetSortFuncAddr(SORT_UNIQUE_MAPPED_INDEX index)
{
	return SortFuncType<MySortElementType>();
}

SORT_METADATA SORT_MAPPER::GetSortMetaData(SORT_UNIQUE_MAPPED_INDEX index)
{
	// TODO: 여기에 return 문을 삽입합니다.
}

TRACE_RESULT SORT_MAPPER::GetTraceResult(SORT_UNIQUE_MAPPED_INDEX index)
{
	// TODO: 여기에 return 문을 삽입합니다.
}

SORT_MAPPER::SORT_MAPPER()
{
	//TODO : init
	{
		{BubbleSort<SortElementType>, "BubbleSort", 0, TRACE_RESULT()},
		{ InsertionSort<SortElementType>, "InsertionSort", 0, TRACE_RESULT() },
		{ static_cast<void(*)(SortElementType[], size_t, ORDER_BY)>(QuickSort<SortElementType>), "PartitioningProc", 0, TRACE_RESULT() }
	};
}
