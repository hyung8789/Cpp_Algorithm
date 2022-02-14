#include "Core.h"

/// <summary>
/// SORT_METADATA ������
/// </summary>
SORT_METADATA::SORT_METADATA()
{
	this->_sortFuncAddr = NULL;
	this->_sortFuncName = NULL;
	this->_optionalAlternativeSortFuncName = NULL;
	this->_traceResult = TRACE_RESULT();
}

/// <summary>
/// SORT_METADATA ������
/// </summary>
/// <param name="sortFuncAddr">���� �Լ� �ּ�</param>
/// <param name="sortFuncName">���� �Լ� �̸�</param>
/// <param name="optionalAlternativeSortFuncName">��ü ���� �Լ� �̸� (Optional)</param>
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

SORT_MAPPER* SORT_MAPPER::_instance = NULL; //SORT_MAPPER ���� �ν��Ͻ� �ʱ�ȭ

/// <summary>
/// �ڽ��� ���� �ν��Ͻ� ���� ��ȯ
/// </summary>
/// <typeparam name="SortElementType">���� ��� Ÿ��</typeparam>
/// <returns>�ڽ��� ���� �ν��Ͻ� ����</returns>
SORT_MAPPER& SORT_MAPPER::GetInstance()
{
	if (_instance == NULL)
		_instance = new SORT_MAPPER();

	return *_instance;
}

/// <summary>
/// �ڽ��� ���� �ν��Ͻ� �޸� �Ҵ� ����
/// </summary>
void SORT_MAPPER::Dispose()
{
	if (_instance != NULL)
		delete _instance;

	_instance = NULL;
}

/// <summary>
/// ������ ���� ��� �ε����� ���� ���� �Լ� �̸� ���ڿ� ��ȯ
/// </summary>
/// <param name="index">������ ���� ��� �ε���</param>
/// <returns>���� �Լ� �̸� ���ڿ�</returns>
const char* SORT_MAPPER::SortUniqueMappedIndexToSortFuncNameStr(SORT_UNIQUE_MAPPED_INDEX index) const
{
	return this->_sortMetaDataTable[(const int)index]._sortFuncName;
}

/// <summary>
/// ���� �Լ� �̸� ���ڿ��� ���� ������ ���� ��� �ε��� ��ȯ
/// </summary>
/// <param name="sortFuncNameStr">���� �Լ� �̸� ���ڿ�</param>
/// <returns>������ ���� ��� �ε���</returns>
SORT_UNIQUE_MAPPED_INDEX SORT_MAPPER::SortFuncNameStrToSortUniqueMappedIndex(const char* sortFuncNameStr) const
{
	for (int i = 0; i < (const int)SORT_UNIQUE_MAPPED_INDEX::TOTAL_SORT_FUNC_COUNT; i++)
	{
		if (strcmp(sortFuncNameStr, this->_sortMetaDataTable[i]._sortFuncName) == 0) //���� �Լ� �̸� ��
			return (SORT_UNIQUE_MAPPED_INDEX)i;

		if (this->_sortMetaDataTable[i]._optionalAlternativeSortFuncName != NULL) //��ü ���� �Լ� �̸� ���� ��
			if (strcmp(sortFuncNameStr, this->_sortMetaDataTable[i]._optionalAlternativeSortFuncName) == 0) //��ü ���� �Լ� �̸� ��
				return (SORT_UNIQUE_MAPPED_INDEX)i;
	}

	throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));
}

/// <summary>
/// ������ ���� ��� �ε����� ���� ���Ŀ� ���� �б� ���� ��Ÿ������ ���� ��ȯ
/// </summary>
/// <param name="index">������ ���� ��� �ε���</param>
/// <returns>������ ���� ��� �ε����� ���� ���Ŀ� ���� �б� ���� ��Ÿ������ ����</returns>
const SORT_METADATA& SORT_MAPPER::GetSortMetaData(SORT_UNIQUE_MAPPED_INDEX index) const
{
	return this->_sortMetaDataTable[(const int)index];
}

/// <summary>
/// ������ ���� ��� �ε����� ���� ���Ŀ� ���� Trace Result ���� ��ȯ
/// </summary>
/// <param name="index">������ ���� ��� �ε���</param>
/// <returns>������ ���� ��� �ε����� ���� ���Ŀ� ���� Trace Result ����</returns>
TRACE_RESULT& SORT_MAPPER::GetRefTraceResult(SORT_UNIQUE_MAPPED_INDEX index)
{
	return this->_sortMetaDataTable[(const int)index]._traceResult;
}

/// <summary>
/// SORT_MAPPER ������
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
	//�����ε� �� ���ø� �Լ��� thread ���� �� �����Ϸ��� �߷� �� �� �����Ƿ�, ������ Ÿ�ӿ� ���� ĳ��Ʈ
	this->_sortMetaDataTable[(const int)SORT_UNIQUE_MAPPED_INDEX::QUICK_SORT]
		= SORT_METADATA(static_cast<void(*)(MySortElementType[], size_t, ORDER_BY)>(QuickSort<MySortElementType>), "QuickSort", "PartitioningProc");
}