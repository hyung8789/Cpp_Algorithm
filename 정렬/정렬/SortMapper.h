#ifndef _SORT_MAPPER_H_
#define _SORT_MAPPER_H_

extern class TRACE_RESULT;
typedef int MySortElementType; //������ ���� ��� Ÿ��

enum class SORT_UNIQUE_MAPPED_INDEX : const int
{
	BUBBLE_SORT = 0,
	INSERTION_SORT,
	QUICK_SORT,

	TOTAL_SORT_FUNC_COUNT //��ü ���� �Լ� ����
}; //������ ���� ��� �ε���

struct SORT_METADATA
{
	SortFuncType<MySortElementType> _sortFuncAddr; //���� �Լ� �ּ�
	const char* _sortFuncName; //���� �Լ� �̸�
	TRACE_RESULT _traceResult;
}; //���Ŀ� ���� ��Ÿ������

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