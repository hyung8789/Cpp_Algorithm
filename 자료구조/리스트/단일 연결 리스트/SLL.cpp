#include "SLL_Core.h"

/// <summary>
/// 새로운 노드 생성 및 생성 된 노드 반환
/// </summary>
/// <param name="srcData">노드의 데이터</param>
/// <returns>생성 된 노드</returns>
Node* SLL_CreateNode(DataType srcData)
{
	Node* retVal = (Node*)malloc(sizeof(Node));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_data = srcData;
	retVal->_next = NULL;

	return retVal;
}

/// <summary>
/// 대상 노드에 할당 된 메모리 해제
/// </summary>
/// <param name="srcNode">대상 노드</param>
void SLL_DeallocateNode(Node** srcNode)
{
	if ((*srcNode) != NULL)
	{
		free(*srcNode);
		(*srcNode) = NULL;
	}
}

/// <summary>
/// 대상 리스트에 할당 된 메모리 해제
/// </summary>
/// <param name="srcList">대상 리스트</param>
void SLL_DeallocateNodeList(Node** srcList)
{
	while ((*srcList) != NULL)
	{
		Node* tmp = (*srcList); //삭제 할 노드

		(*srcList) = (*srcList)->_next; //헤드 노드를 다음 노드로 이동 후 삭제
		SLL_DeallocateNode(&tmp);
	}

	(*srcList) = NULL;
}

/// <summary>
/// 대상 리스트의 끝에 새 노드 삽입
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <param name="srcNewNode">리스트의 끝에 삽입하고자 하는 새 노드</param>
void SLL_AppendNode(Node** srcList, Node* srcNewNode)
{
	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcList) == NULL)
	{
		(*srcList) = srcNewNode; //헤드 노드 할당
	}
	else //끝에 새 노드 삽입
	{
		Node* tail = (*srcList); //꼬리 노드

		while (tail->_next != NULL) //끝으로 이동
		{
			tail = tail->_next;
		}

		SLL_InsertNodeAfter(tail, srcNewNode);
	}
}

/// <summary>
/// 대상 리스트에서 헤드 노드 기준 상대적 특정 위치의 노드 반환
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <param name="position">헤드 노드 기준 상대적 특정 위치</param>
/// <returns>헤드 노드 기준 상대적 특정 위치의 노드</returns>
Node* SLL_GetNodeAt(Node** srcList, NodePositionType position)
{
	if (position < 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	Node* retVal = (*srcList); //현재 노드

	while (retVal != NULL && (--position) >= 0) //반환 대상 노드 위치까지 이동
	{
		retVal = retVal->_next;
	}

	if (retVal == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	return retVal;
}

/// <summary>
/// 대상 리스트에서 헤드 노드 기준 삭제 할 상대적 특정 위치의 노드 삭제
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <param name="position">헤드 노드 기준 삭제 할 상대적 특정 위치</param>
/// <param name="deallocateAfterRemove">삭제 대상 노드에 대한 메모리 해제 수행 여부</param>
void SLL_RemoveNodeAt(Node** srcList, NodePositionType position, bool deallocateAfterRemove)
{
	/***
		단일 연결 리스트에서 삭제 대상 노드에서 이전 노드로 즉시 접근이 불가능하므로,
		삭제 대상 노드의 이전 노드까지 이동하여, 삭제 대상 노드의 다음 연결 해제를 수행하여야 한다.
		---
		이에 따라, 단순 이중 연결 리스트 혹은 환형 이중 연결 리스트와 다르게
		현재 구조에서 RemoveNodeAt과 RemoveNode 간의 중복 최소화를 위한 종속 관계를 연결 할 수 없다.
	***/

	if (position < 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (position == 0) //헤드 노드가 삭제 대상일 경우
	{
		Node* tmp = (*srcList); //삭제 대상 노드
		(*srcList) = (*srcList)->_next; //헤드 노드의 다음 노드로 헤드 노드 변경
		tmp->_next = NULL;

		if (deallocateAfterRemove)
		{
			SLL_DeallocateNode(&tmp);
		}
	}
	else //현재 노드의 다음 노드가 삭제 대상 노드일 때까지 탐색
	{
		Node* current = (*srcList); //현재 노드

		while (current != NULL && (--position) >= 1) //삭제 대상 노드의 이전 노드 위치까지 이동
		{
			current = current->_next;
		}

		if (current == NULL)
			throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

		Node* tmp = current->_next; //삭제 대상 노드

		current->_next = tmp->_next; //현재 노드의 다음을 삭제 대상 노드의 다음 노드로 연결
		tmp->_next = NULL;

		if (deallocateAfterRemove)
		{
			SLL_DeallocateNode(&tmp);
		}
	}
}

/// <summary>
/// 대상 리스트에서 삭제 대상 노드 삭제
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <param name="srcTargetNode">삭제 대상 노드</param>
/// <param name="deallocateAfterRemove">삭제 대상 노드에 대한 메모리 해제 수행 여부</param>
void SLL_RemoveNode(Node** srcList, Node* srcTargetNode, bool deallocateAfterRemove)
{
	if (srcTargetNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (srcTargetNode == (*srcList)) //삭제 대상 노드가 헤드 노드일 경우
	{
		(*srcList) = srcTargetNode->_next; //삭제 대상 노드의 다음 노드로 헤드 노드 변경
		srcTargetNode->_next = NULL;

		if (deallocateAfterRemove)
		{
			SLL_DeallocateNode(&srcTargetNode);
		}
	}
	else //현재 노드의 다음 노드가 삭제 대상 노드일때까지 탐색
	{
		Node* current = (*srcList); //현재 노드

		while (current != NULL && current->_next != srcTargetNode) //삭제 대상 노드의 이전 노드 위치까지 이동
		{
			current = current->_next;
		}

		if (current == NULL)
			throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

		current->_next = srcTargetNode->_next; //현재 노드의 다음을 삭제 대상 노드의 다음 노드로 연결
		srcTargetNode->_next = NULL;

		if (deallocateAfterRemove)
		{
			SLL_DeallocateNode(&srcTargetNode);
		}
	}
}

/// <summary>
/// 대상 리스트의 맨 앞에 새 노드 삽입
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <param name="srcNewNode">삽입하고자 하는 새 노드</param>
void SLL_InsertNewHead(Node** srcList, Node* srcNewNode)
{
	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcList) == NULL)
	{
		(*srcList) = srcNewNode;
	}
	else //헤드 노드의 앞에 삽입
	{
		SLL_InsertNodeBefore(srcList, (*srcList), srcNewNode);
	}
}

/// <summary>
/// 대상 노드의 뒤에 새 노드 삽입
/// </summary>
/// <param name="srcTargetNode">대상 노드</param>
/// <param name="srcNewNode">삽입하고자 하는 새 노드</param>
void SLL_InsertNodeAfter(Node* srcTargetNode, Node* srcNewNode)
{
	if (srcTargetNode == NULL || srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	srcNewNode->_next = srcTargetNode->_next; //새 노드의 다음을 대상 노드의 다음 노드로 연결
	srcTargetNode->_next = srcNewNode; //대상 노드의 다음을 새 노드로 연결
}

/// <summary>
/// 대상 리스트의 대상 노드의 앞에 새 노드 삽입
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <param name="srcTargetNode">대상 노드</param>
/// <param name="srcNewNode">삽입하고자 하는 새 노드</param>
void SLL_InsertNodeBefore(Node** srcList, Node* srcTargetNode, Node* srcNewNode)
{
	/***
		1) TH : 대상 노드의 헤드 노드 여부 (T : 헤드 노드, F : 중간 노드 혹은 꼬리 노드)
		2) D : 이에 따른 삽입하고자 하는 새 노드 삽입 시 수행 작업

		TH | D
		T	1) 새 노드의 다음을 기존 헤드 노드로 연결
			2) 새 노드를 헤드 노드로 변경

		F	1) 대상 노드에서 이전 노드로 즉시 접근이 불가능하므로, 대상 노드의 이전 노드까지 이동
			2) 대상 노드의 이전 노드의 다음을 새 노드로 연결
			3) 새 노드의 다음을 대상 노드로 연결
	***/

	if (srcTargetNode == NULL || srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (srcTargetNode == (*srcList)) //T
	{
		srcNewNode->_next = (*srcList); //새 노드의 다음을 기존 헤드 노드로 연결
		(*srcList) = srcNewNode; //헤드 노드 변경
	}
	else //F
	{
		Node* current = (*srcList); //현재 노드

		while (current != NULL && current->_next != srcTargetNode) //현재 노드의 다음 노드가 삽입 대상 노드일때까지 탐색
		{
			current = current->_next;
		}

		if (current == NULL)
			throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

		current->_next = srcNewNode; //현재 노드의 다음을 새 노드로 연결
		srcNewNode->_next = srcTargetNode; //새 노드의 다음을 대상 노드로 연결
	}
}

/// <summary>
/// 대상 리스트의 전체 노드의 수 반환
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <returns>대상 리스트의 전체 노드의 수</returns>
NodePositionType SLL_GetTotalNodeCount(Node** srcList)
{
	NodePositionType totalNodeCount = 0; //전체 노드의 수
	Node* current = (*srcList); //현재 노드

	while (current != NULL)
	{
		current = current->_next;
		totalNodeCount++;
	}

	return totalNodeCount;
}

/// <summary>
/// 대상 리스트의 전체 노드에 대한 데이터 출력
/// </summary>
/// <param name="srcList">대상 리스트</param>
void SLL_DispNodeList(Node** srcList)
{
	NodePositionType currentPosition = 0; //현재 노드의 위치
	Node* current = (*srcList); //현재 노드

	while (current != NULL)
	{
		std::cout << "List [" << currentPosition << "] : " << current->_data << std::endl;

		current = current->_next;
		currentPosition++;
	}
}