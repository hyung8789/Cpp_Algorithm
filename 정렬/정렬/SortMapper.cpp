#include "Core.h"

/// <summary>
/// SORT_METADATA 생성자
/// </summary>
SORT_METADATA::SORT_METADATA()
{
	this->_sortFuncAddr = NULL;
	this->_sortFuncName = NULL;
	this->_optionalAlternativeSortFuncName = NULL;
	this->_traceResult = TRACE_RESULT();
}

/// <summary>
/// SORT_METADATA 생성자
/// </summary>
/// <param name="sortFuncAddr">정렬 함수 주소</param>
/// <param name="sortFuncName">정렬 함수 이름</param>
/// <param name="optionalAlternativeSortFuncName">대체 정렬 함수 이름 (Optional)</param>
SORT_METADATA::SORT_METADATA(SortFuncAddrType<MySortElementType> sortFuncAddr, const char* sortFuncName, const char* optionalAlternativeSortFuncName)
{
	if (sortFuncAddr == NULL || sortFuncName == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	this->_sortFuncAddr = sortFuncAddr;
	this->_sortFuncName = sortFuncName;

	if (optionalAlternativeSortFuncName != NULL)
		this->_optionalAlternativeSortFuncName = optionalAlternativeSortFuncName;
	else
		this->_optionalAlternativeSortFuncName = NULL;

	this->_traceResult = TRACE_RESULT();
}

SORT_MAPPER* SORT_MAPPER::_instance = NULL; //SORT_MAPPER 고유 인스턴스 초기화

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

/// <summary>
/// 자신의 고유 인스턴스 메모리 할당 해제
/// </summary>
void SORT_MAPPER::Dispose()
{
	if (_instance != NULL)
		delete _instance;

	_instance = NULL;
}

/// <summary>
/// 정렬의 고유 사상 인덱스에 대한 정렬 함수 이름 문자열 반환
/// </summary>
/// <param name="index">정렬의 고유 사상 인덱스</param>
/// <returns>정렬 함수 이름 문자열</returns>
const char* SORT_MAPPER::SortUniqueMappedIndexToSortFuncNameStr(SORT_UNIQUE_MAPPED_INDEX index) const
{
	return this->_sortMetaDataTable[(const int)index]._sortFuncName;
}

/// <summary>
/// 정렬 함수 이름 문자열에 대한 정렬의 고유 사상 인덱스 반환
/// </summary>
/// <param name="sortFuncNameStr">정렬 함수 이름 문자열</param>
/// <returns>정렬의 고유 사상 인덱스</returns>
SORT_UNIQUE_MAPPED_INDEX SORT_MAPPER::SortFuncNameStrToSortUniqueMappedIndex(const char* sortFuncNameStr) const
{
	for (int i = 0; i < (const int)SORT_UNIQUE_MAPPED_INDEX::TOTAL_SORT_FUNC_COUNT; i++)
	{
		if (strcmp(sortFuncNameStr, this->_sortMetaDataTable[i]._sortFuncName) == 0) //정렬 함수 이름 비교
			return (SORT_UNIQUE_MAPPED_INDEX)i;

		if (this->_sortMetaDataTable[i]._optionalAlternativeSortFuncName != NULL) //대체 정렬 함수 이름 존재 시
			if (strcmp(sortFuncNameStr, this->_sortMetaDataTable[i]._optionalAlternativeSortFuncName) == 0) //대체 정렬 함수 이름 비교
				return (SORT_UNIQUE_MAPPED_INDEX)i;
	}

	throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));
}

/// <summary>
/// 정렬의 고유 사상 인덱스에 대한 정렬에 대한 읽기 전용 메타데이터 참조 반환
/// </summary>
/// <param name="index">정렬의 고유 사상 인덱스</param>
/// <returns>정렬의 고유 사상 인덱스에 대한 정렬에 대한 읽기 전용 메타데이터 참조</returns>
const SORT_METADATA& SORT_MAPPER::GetSortMetaData(SORT_UNIQUE_MAPPED_INDEX index) const
{
	return this->_sortMetaDataTable[(const int)index];
}

/// <summary>
/// 정렬의 고유 사상 인덱스에 대한 정렬에 대한 Trace Result 참조 반환
/// </summary>
/// <param name="index">정렬의 고유 사상 인덱스</param>
/// <returns>정렬의 고유 사상 인덱스에 대한 정렬에 대한 Trace Result 참조</returns>
TRACE_RESULT& SORT_MAPPER::GetRefTraceResult(SORT_UNIQUE_MAPPED_INDEX index)
{
	return this->_sortMetaDataTable[(const int)index]._traceResult;
}

/// <summary>
/// SORT_MAPPER 생성자
/// </summary>
SORT_MAPPER::SORT_MAPPER()
{
	this->_sortMetaDataTable[(const int)SORT_UNIQUE_MAPPED_INDEX::BUBBLE_SORT]
		= SORT_METADATA(BubbleSort<MySortElementType>, "BubbleSort");

	this->_sortMetaDataTable[(const int)SORT_UNIQUE_MAPPED_INDEX::INSERTION_SORT]
		= SORT_METADATA(InsertionSort<MySortElementType>, "InsertionSort");

	this->_sortMetaDataTable[(const int)SORT_UNIQUE_MAPPED_INDEX::SELECTION_SORT]
		= SORT_METADATA(SelectionSort<MySortElementType>, "SelectionSort");

	// https://stackoverflow.com/questions/44049407/c-compilation-fails-on-calling-overloaded-function-in-stdthread
	//오버로딩 된 템플릿 함수를 thread 생성 시 컴파일러가 추론 할 수 없으므로, 컴파일 타임에 정적 캐스트
	this->_sortMetaDataTable[(const int)SORT_UNIQUE_MAPPED_INDEX::QUICK_SORT]
		= SORT_METADATA(static_cast<void(*)(MySortElementType[], size_t, ORDER_BY)>(QuickSort<MySortElementType>), "QuickSort", "PartitioningProc");
}