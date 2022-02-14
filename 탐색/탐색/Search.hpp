#ifndef _SEARCH_HPP_
#define _SEARCH_HPP_

#ifdef _DLL_H_
/// <summary>
/// 대상 리스트에서 순차탐색 - 전진 이동법 (Move To Front)으로 대상 데이터가 포함 된 최초 노드 반환
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <param name="targetData">찾고자 하는 대상 데이터</param>
/// <returns>대상 데이터가 포함 된 최초 노드</returns>
Node* DLL_MTF_SequentialSearch(Node** srcList, DataType targetData)
{
	Node* current = (*srcList); //현재 노드
	Node* retVal = NULL; //찾은 노드

	while (current != NULL)
	{
		if (current->data == targetData) //찾고자 하는 대상 데이터를 발견 시
		{
			retVal = current;
			break;
		}

		current = current->next;
	}

	if (current == NULL || current->data != targetData)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not found"));

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
Node* DLL_Transpose_SequentialSearch(Node** srcList, DataType targetData)
{
	Node* current = (*srcList); //현재 노드
	Node* retVal = NULL; //찾은 노드
	Node* insertTarget = NULL; //삽입 대상 노드

	while (current != NULL)
	{
		if (current->data == targetData) //찾고자 하는 대상 데이터를 발견 시
		{
			retVal = current;
			break;
		}

		current = current->next;
	}

	if (current == NULL || current->data != targetData)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not found"));

	if (retVal->prev != NULL) //찾은 노드의 앞 노드가 존재하면
	{
		insertTarget = retVal->prev; //삽입 대상 노드는 찾은 노드의 앞 노드
		DLL_RemoveNode(srcList, retVal, false); //찾은 노드를 리스트에서 분리
		DLL_InsertNodeBefore(srcList, insertTarget, retVal); //삽입 대상 노드의 앞에 삽입
	}
	else //찾은 노드의 앞 노드가 존재하지 않으면
	{
		//do nothing (찾은 노드는 헤드 노드)
	}

	return retVal;
}
#endif

template<typename SearchElementType>
SearchElementType MTF_SequentialSearch(SearchElementType targetEnumerableSet[], 
	size_t elementCount)
{

}

template<typename SearchElementType>
SearchElementType Transpose_SequentialSearch(SearchElementType targetEnumerableSet[],
	size_t elementCount)
{

}

template<typename SearchElementType>
SearchElementType BinarySearch(SearchElementType targetEnumerableSet[],
	size_t elementCount)
{
}
#endif