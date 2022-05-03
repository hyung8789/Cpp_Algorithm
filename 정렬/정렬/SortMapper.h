#ifndef _SORT_MAPPER_H_
#define _SORT_MAPPER_H_

template<typename SORT_ELEMENT_TYPE>
using SORT_FUNC_ADDR_TYPE = void(*)(SORT_ELEMENT_TYPE[], size_t, ORDER_BY); // https://en.cppreference.com/w/cpp/language/type_alias

typedef int MY_SORT_ELEMENT_TYPE; //임의의 정렬 요소 타입 (정렬 요소 타입 구체화)

/// <summary>
/// 정렬의 고유 사상 인덱스
/// </summary>
enum class SORT_UNIQUE_MAPPED_INDEX : const int
{
	BUBBLE_SORT = 0,
	INSERTION_SORT,
	QUICK_SORT,
	SELECTION_SORT,
	//	MERGE_SORT,
//COCKTAIL_SORT,
//CYCLE_SORT,
	TOTAL_SORT_FUNC_COUNT //전체 정렬 함수 개수
};

/// <summary>
/// 정렬 메타데이터
/// </summary>
struct SORT_METADATA
{
	SORT_METADATA();
	SORT_METADATA(SORT_FUNC_ADDR_TYPE<MY_SORT_ELEMENT_TYPE>, const char*, const char* = NULL);

	SORT_FUNC_ADDR_TYPE<MY_SORT_ELEMENT_TYPE> _sortFuncAddr; //정렬 함수 주소
	const char* _sortFuncName; //정렬 함수 이름
	const char* _optionalAlternativeSortFuncName; //대체 정렬 함수 이름 (Optional)
	TRACE_RESULT _traceResult;
};

class SORT_MAPPER
{
public:
	static SORT_MAPPER& GetInstance();
	void Dispose();

	const char* SortUniqueMappedIndexToSortFuncNameStr(SORT_UNIQUE_MAPPED_INDEX) const;
	SORT_UNIQUE_MAPPED_INDEX SortFuncNameStrToSortUniqueMappedIndex(const char*) const;

	const SORT_METADATA& GetSortMetaData(SORT_UNIQUE_MAPPED_INDEX) const;
	TRACE_RESULT& GetRefTraceResult(SORT_UNIQUE_MAPPED_INDEX);

private:
	SORT_MAPPER();
	static SORT_MAPPER* _instance;

private:
	SORT_METADATA _sortMetaDataTable[static_cast<const int>(SORT_UNIQUE_MAPPED_INDEX::TOTAL_SORT_FUNC_COUNT)];
};
#endif