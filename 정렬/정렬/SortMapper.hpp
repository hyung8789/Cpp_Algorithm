#pragma once
#define TOTAL_SORT_FUNC_COUNT 5 //��ü ���� �Լ� ����

//TODO : ���� ��� �ε����� ���� ���� �Լ� ��ȯ
//TODO : ���� �� ��� �ϵ��ڵ� �� �Լ� ���ڿ� ����, ���� ����
// https://www.google.com/search?q=c%2B%2B+funtion+pointer+enum&oq=c%2B%2B+funtion+pointer+enum&aqs=chrome..69i57.6428j0j1&sourceid=chrome&ie=UTF-8
//https://bytes.com/topic/c/answers/691849-how-use-enum-array-function-pointers
// https://handmade.network/forums/articles/t/2995-using_enum__with_function_pointers_in_c	
// https://codereview.stackexchange.com/questions/57765/mapping-enum-to-function-request-response-library-encapsulation
//TODO : https://en.cppreference.com/w/cpp/utility/source_location


/// <summary>
/// ���� �Լ� �̸� ���ڿ��� ���� ���� ��� �ε��� ��ȯ
/// </summary>
/// <param name="sortFuncNameStr">���� �Լ� �̸� ���ڿ�</param>
/// <returns>���� �Լ� �̸� ���ڿ��κ��� ��ȯ �� �� Ƚ�� ī��Ʈ�� �ε���</returns>
size_t SortFuncNameStrToUniqueMappedIndex(const char* sortFuncNameStr)
{
	return strcmp(sortFuncNameStr, "BubbleSort") == 0 ? 0 :
		strcmp(sortFuncNameStr, "InsertionSort") == 0 ? 1 :
		strcmp(sortFuncNameStr, "QuickSort") == 0 ? 2 :
		strcmp(sortFuncNameStr, "PartitioningProc") == 0 ? 2 : //�� ��Ʈ���� ȣ��
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));
}

/// <summary>
/// ���� ��� �ε����� ���� ���� �Լ� �̸� ���ڿ� ��ȯ
/// </summary>
/// <param name="sortFuncNameStr">���� ��� �ε���</param>
/// <returns>���� ��� �ε����� ���� ���� �Լ� �̸� ���ڿ�</returns>
const char* UniqueMappedIndexToSortFuncNameStr(size_t uniqueMappedIndex)
{
	switch (uniqueMappedIndex)
	{
	case 0:
		return "BubbleSort";

	case 1:
		return "InsertionSort";

	case 2:
		return "QuickSort";
	}
}

/// <summary>
/// ���� ��� �ε����� ���� ���� �Լ� ��ȯ
/// </summary>
/// <typeparam name="SortElementType"></typeparam>
/// <param name=""></param>
/// <param name=""></param>
/// <param name=""></param>
/// <returns></returns>
template<typename SortElementType>
void(*UniqueMappedIndexToSortFuncAddr(int thisArg1))(SortElementType[], size_t, ORDER_BY)
{
	return QuickSort;
}