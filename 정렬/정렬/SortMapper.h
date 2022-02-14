#ifndef _SORT_MAPPER_H_
#define _SORT_MAPPER_H_

template<typename SortElementType>
using SortFuncAddrType = void(*)(SortElementType[], size_t, ORDER_BY); // https://en.cppreference.com/w/cpp/language/type_alias

typedef int MySortElementType; //������ ���� ��� Ÿ�� (���� ��� Ÿ�� ��üȭ)

enum class SORT_UNIQUE_MAPPED_INDEX : const int
{
	BUBBLE_SORT = 0,
	INSERTION_SORT,
	QUICK_SORT,
//	MERGE_SORT,
	SELECTION_SORT,
	TOTAL_SORT_FUNC_COUNT //��ü ���� �Լ� ����
}; //������ ���� ��� �ε���

struct SORT_METADATA
{
	SORT_METADATA();
	SORT_METADATA(SortFuncAddrType<MySortElementType>, const char*, const char* = NULL);

	SortFuncAddrType<MySortElementType> _sortFuncAddr; //���� �Լ� �ּ�
	const char* _sortFuncName; //���� �Լ� �̸�
	const char* _optionalAlternativeSortFuncName; //��ü ���� �Լ� �̸� (Optional)
	TRACE_RESULT _traceResult;
}; //���Ŀ� ���� ��Ÿ������

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
	SORT_METADATA _sortMetaDataTable[(const int)SORT_UNIQUE_MAPPED_INDEX::TOTAL_SORT_FUNC_COUNT];
};
#endif