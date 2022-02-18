#ifndef _SEARCH_HPP_
#define _SEARCH_HPP_

#ifdef _DLL_H_
/// <summary>
/// 대상 리스트에서 순차탐색 - 전진 이동법 (Move To Front)으로 대상 데이터가 포함 된 최초 노드 반환
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <param name="targetData">찾고자 하는 대상 데이터</param>
/// <returns>대상 데이터가 포함 된 최초 노드</returns>
Node* DLL_SequentialSearch_MTF(Node** srcList, const DataType& targetData)
{
	Node* currentNode = (*srcList); //현재 노드
	Node* retVal = NULL; //찾은 노드

	while (currentNode != NULL)
	{
		if (COMPARE(currentNode->_data, targetData) == 0) //찾고자 하는 대상 데이터를 발견 시
		{
			retVal = currentNode;
			break;
		}

		currentNode = currentNode->_next;
	}

	if (currentNode == NULL || COMPARE(currentNode->_data, targetData) != 0)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (retVal != (*srcList)) //찾은 노드가 헤드 노드가 아닌 경우
	{
		DLL_RemoveNode(srcList, retVal, false); //찾은 노드를 리스트에서 분리
		DLL_InsertNewHead(srcList, retVal); //찾은 노드를 리스트의 맨 앞에 삽입
	}
	else //찾은 노드가 헤드 노드인 경우
	{
		//do nothing 
	}

	return retVal;
}

/// <summary>
/// 대상 리스트에서 순차탐색 - 전위법 (Transpose)으로 대상 데이터가 포함 된 최초 노드 반환
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <param name="targetData">찾고자 하는 대상 데이터</param>
/// <returns>대상 데이터가 포함 된 최초 노드</returns>
Node* DLL_SequentialSearch_Transpose(Node** srcList, const DataType& targetData)
{
	Node* currentNode = (*srcList); //현재 노드
	Node* retVal = NULL; //찾은 노드
	Node* insertTargetNode = NULL; //삽입 대상 노드

	while (currentNode != NULL)
	{
		if (COMPARE(currentNode->_data, targetData) == 0) //찾고자 하는 대상 데이터를 발견 시
		{
			retVal = currentNode;
			break;
		}

		currentNode = currentNode->_next;
	}

	if (currentNode == NULL || COMPARE(currentNode->_data, targetData) != 0)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (retVal->_prev != NULL) //찾은 노드의 앞 노드가 존재하면
	{
		insertTargetNode = retVal->_prev; //삽입 대상 노드는 찾은 노드의 앞 노드
		DLL_RemoveNode(srcList, retVal, false); //찾은 노드를 리스트에서 분리
		DLL_InsertNodeBefore(srcList, insertTargetNode, retVal); //삽입 대상 노드의 앞에 삽입
	}
	else //찾은 노드의 앞 노드가 존재하지 않으면
	{
		//do nothing (찾은 노드는 이미 헤드 노드)
	}

	return retVal;
}
#endif

/// <summary>
/// 순차탐색 - 전진 이동법 (Move To Front) (Best Case : O(1), Average, Worst Case : O(n))
/// </summary>
/// <typeparam name="SearchElementType">검색 요소 타입</typeparam>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param name="targetData">찾고자 하는 대상 데이터</param>
/// <returns>찾고자 하는 대상 데이터와 일치하는 최초로 찾은 데이터</returns>
template<typename SearchElementType>
SearchElementType SequentialSearch_MTF(SearchElementType targetEnumerableSet[],
	size_t elementCount, const SearchElementType& targetData)
{
	SearchElementType tmp;

	for (size_t i = 0; i < elementCount; i++)
	{
		if (COMPARE(targetEnumerableSet[i], targetData) == 0) //찾고자 하는 대상 데이터를 발견 시
		{
			tmp = targetEnumerableSet[i];

			memmove_s(&targetEnumerableSet[1],
				sizeof(targetEnumerableSet[0]) * i,
				&targetEnumerableSet[0],
				sizeof(targetEnumerableSet[0]) * i
			); //찾은 데이터의 바로 앞까지의 요소 내의 범위의 요소들을 뒤로 한 칸씩 이동

			targetEnumerableSet[0] = tmp; //찾은 데이터를 맨 앞으로 삽입
			return targetEnumerableSet[0];
		}
	}

	throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));
}

/// <summary>
/// 순차탐색 - 전위법 (Transpose) (Best Case : O(1), Average, Worst Case : O(n))
/// </summary>
/// <typeparam name="SearchElementType">검색 요소 타입</typeparam>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param name="targetData">찾고자 하는 대상 데이터</param>
/// <returns>찾고자 하는 대상 데이터와 일치하는 최초로 찾은 데이터</returns>
template<typename SearchElementType>
SearchElementType SequentialSearch_Transpose(SearchElementType targetEnumerableSet[],
	size_t elementCount, const SearchElementType& targetData)
{
	SearchElementType tmp;

	for (size_t i = 0; i < elementCount; i++)
	{
		if (COMPARE(targetEnumerableSet[i], targetData) == 0) //찾고자 하는 대상 데이터를 발견 시
		{
			if (i > 0) //찾은 데이터의 앞에 데이터가 존재하면
			{
				SWAP(targetEnumerableSet[i], targetEnumerableSet[i - 1], tmp); //찾은 데이터를 바로 앞의 요소와 교환
				return targetEnumerableSet[i - 1];
			}

			return targetEnumerableSet[i]; //찾은 데이터가 이미 맨 앞의 요소인 경우
		}
	}

	throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));
}

/// <summary>
/// 이진 탐색 (Best Case : O(1), Average, Worst Case : O(log2(n)))
/// </summary>
/// <typeparam name="SearchElementType">탐색 요소 타입</typeparam>
/// <param name="srcOrderedEnumerableSet">이미 정렬 된 순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="elementCount">이미 정렬 된 순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param name="targetData">찾고자 하는 대상 데이터</param>
/// <returns>찾고자 하는 대상 데이터와 일치하는 최초로 찾은 데이터</returns>
template<typename SearchElementType>
SearchElementType BinarySearch(const SearchElementType srcOrderedEnumerableSet[],
	size_t elementCount, SearchElementType targetData)
{
	/***
		< 이진 탐색의 정렬 방향 판별 >

		M1) 정렬 방향을 파라미터로 받는 방법

			M1-1) 사용자가 이미 정렬 된 순차적으로 열거 가능 한 요소들의 집합이 어떠한 방향으로 정렬되어있는지 알고 있을 경우
			: 이진 탐색 시 탐색 범위를 줄여나갈 위치를 판별 가능

			M1-2) 사용자가 이미 정렬 된 순차적으로 열거 가능 한 요소들의 집합이 어떠한 방향으로 정렬되어있는지 모를 경우
			: 이진 탐색을 수행 할 수 없으며, 이는 이전의 정렬 수행 방법에 의존적

		M2) 이미 정렬 된 순차적으로 열거 가능 한 요소들의 집합의 처음 요소와 마지막 요소를 통한 판별

			M2-1) 처음 요소 > 마지막 요소
			: 이미 정렬 된 순차적으로 열거 가능 한 요소들의 집합은 내림차순으로 정렬되어있음을 가정

			M2-2) 처음 요소 == 마지막 요소
			: 해당 집합의 모든 요소가 동일한 값을 가지고 있거나, 해당 집합은 정렬되어 있지 않음
			(ex) 3 1 3 혹은 1 1 의 두 집합에 대해, 앞의 집합은 정렬되어 있지 않으며, 뒤의 집합은 모든 요소가 동일한 값을 가짐)
			이에 따라, 정렬되어 있지 않은 집합에 대해, 찾고자 하는 요소와 처음 요소가 일치한지 판별

				M2-2-1) 찾고자 하는 요소와 처음 요소가 일치
				: 탐색 성공, 찾은 요소 반환

				M2-2-2) 찾고자 하는 요소와 처음 요소가 불일치
				: 해당 집합의 모든 요소가 동일한 값을 가지고 있음에도 찾고자 하는 요소와 불일치하거나,
				해당 집합은 정렬되어 있지 않으므로, 이진 탐색 수행 불가

			M2-3) 처음 요소 < 마지막 요소
			: 이미 정렬 된 순차적으로 열거 가능 한 요소들의 집합은 오름차순으로 정렬되어있음을 가정

		---

		< 탐색 횟수 >

		n : 순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수
		탐색 과정에서 요소들의 개수 (탐색 범위) 1/2 씩 감소

		1 (최종 탐색 범위) = n * (1 / 2)^탐색 횟수
		1 = n * 1 / 2^탐색 횟수
		2^탐색 횟수 = n
		탐색 횟수 = log2(n)
	***/

	size_t leftIndex = 0; //왼쪽 인덱스
	size_t rightIndex = elementCount - 1; //오른쪽 인덱스
	size_t midIndex; //가운데 인덱스

	/***
		< 이미 정렬 된 순차적으로 열거 가능 한 요소들의 집합의 오름차순 정렬 여부 >

		1: 오름차순 정렬 (right > left)
		-1 : 내림차순 정렬 (right < left)
		0 : 모든 요소가 동일한 값을 가지고 있거나, 해당 집합은 정렬되어 있지 않음 (left == right)
	***/

	char isOrderByAscending = COMPARE(srcOrderedEnumerableSet[rightIndex], srcOrderedEnumerableSet[leftIndex]);

	switch (isOrderByAscending)
	{
	case 0: //모든 요소가 동일한 값을 가지고 있거나, 해당 집합은 정렬되어 있지 않음 
		if (COMPARE(srcOrderedEnumerableSet[leftIndex], targetData) == 0) //찾고자 하는 요소와 처음 요소가 일치
			return srcOrderedEnumerableSet[leftIndex];

		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Unordered EnumerableSet or Not found"));

	default:
		break;
	}

	while (leftIndex <= rightIndex) //탐색 범위가 존재하는 동안
	{
		midIndex = leftIndex + ((rightIndex - leftIndex) >> 1); //base : leftIndex, offset : rightIndex와 leftIndex의 중간만큼 증가 (overflow 방지)

		if (COMPARE(srcOrderedEnumerableSet[midIndex], targetData) == 0)
			return srcOrderedEnumerableSet[midIndex];

		switch (isOrderByAscending)
		{
		case 1: //오름차순 정렬
			if (COMPARE(srcOrderedEnumerableSet[midIndex], targetData) == -1) //mid < target
				leftIndex = midIndex + 1; //현재 가운데 인덱스 기준 오른쪽으로 탐색
			else //mid > target
				rightIndex = midIndex - 1; //현재 가운데 인덱스 기준 왼쪽으로 탐색
			break;

		case -1: //내림차순 정렬
			if (COMPARE(srcOrderedEnumerableSet[midIndex], targetData) == -1) //mid < target
				rightIndex = midIndex - 1; //현재 가운데 인덱스 기준 왼쪽으로 탐색
			else //mid > target
				leftIndex = midIndex + 1; //현재 가운데 인덱스 기준 오른쪽으로 탐색
			break;

		default:
			throw myexception::MEM_CORRUPTION_EXCEPTION(std::string(__func__) + std::string(" : Mem corruption"));
		}
	}

	throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));
}
#endif