#ifndef _SORT_MAPPER_H_
#define _SORT_MAPPER_H_

extern class TRACE_RESULT;
typedef int MySortElementType; //임의의 정렬 요소 타입

enum class SORT_UNIQUE_MAPPED_INDEX : const int
{
	BUBBLE_SORT = 0,
	INSERTION_SORT,
	QUICK_SORT,

	TOTAL_SORT_FUNC_COUNT //전체 정렬 함수 개수
}; //정렬의 고유 사상 인덱스

struct SORT_METADATA
{
	SortFuncType<MySortElementType> _sortFuncAddr; //정렬 함수 주소
	const char* _sortFuncName; //정렬 함수 이름
	TRACE_RESULT _traceResult;
}; //정렬에 대한 메타데이터

class SORT_MAPPER
{
public:
	static SORT_MAPPER& GetInstance();

	const char* UniqueMappedIndexToSortFuncNameStr(SORT_UNIQUE_MAPPED_INDEX);
	int SortFuncNameStrToUniqueMappedIndex(const char*);
	SortFuncType<MySortElementType> GetSortFuncAddr(SORT_UNIQUE_MAPPED_INDEX);
	SORT_METADATA GetSortMetaData(SORT_UNIQUE_MAPPED_INDEX);
	TRACE_RESULT GetTraceResult(SORT_UNIQUE_MAPPED_INDEX);

private:
	SORT_MAPPER();

	static SORT_MAPPER* _instance;
	SORT_METADATA _sortMetaData[(const int)SORT_UNIQUE_MAPPED_INDEX::TOTAL_SORT_FUNC_COUNT];
};
#endif