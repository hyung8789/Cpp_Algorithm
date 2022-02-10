#pragma once
#define TOTAL_SORT_FUNC_COUNT 5 //전체 정렬 함수 개수

//TODO : 고유 사상 인덱스에 따른 정렬 함수 반환
//TODO : 가능 할 경우 하드코딩 된 함수 문자열 변경, 구조 개선
// https://www.google.com/search?q=c%2B%2B+funtion+pointer+enum&oq=c%2B%2B+funtion+pointer+enum&aqs=chrome..69i57.6428j0j1&sourceid=chrome&ie=UTF-8
//https://bytes.com/topic/c/answers/691849-how-use-enum-array-function-pointers
// https://handmade.network/forums/articles/t/2995-using_enum__with_function_pointers_in_c	
// https://codereview.stackexchange.com/questions/57765/mapping-enum-to-function-request-response-library-encapsulation
//TODO : https://en.cppreference.com/w/cpp/utility/source_location


/// <summary>
/// 정렬 함수 이름 문자열에 대한 고유 사상 인덱스 반환
/// </summary>
/// <param name="sortFuncNameStr">정렬 함수 이름 문자열</param>
/// <returns>정렬 함수 이름 문자열로부터 변환 된 비교 횟수 카운트의 인덱스</returns>
size_t SortFuncNameStrToUniqueMappedIndex(const char* sortFuncNameStr)
{
	return strcmp(sortFuncNameStr, "BubbleSort") == 0 ? 0 :
		strcmp(sortFuncNameStr, "InsertionSort") == 0 ? 1 :
		strcmp(sortFuncNameStr, "QuickSort") == 0 ? 2 :
		strcmp(sortFuncNameStr, "PartitioningProc") == 0 ? 2 : //퀵 소트에서 호출
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));
}

/// <summary>
/// 고유 사상 인덱스에 대한 정렬 함수 이름 문자열 반환
/// </summary>
/// <param name="sortFuncNameStr">고유 사상 인덱스</param>
/// <returns>고유 사상 인덱스에 대한 정렬 함수 이름 문자열</returns>
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
/// 고유 사상 인덱스에 대한 정렬 함수 반환
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