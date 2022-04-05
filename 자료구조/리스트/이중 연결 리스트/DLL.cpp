#include "DLL_Core.h"

/// <summary>
/// 새로운 노드 생성 및 생성 된 노드 반환
/// </summary>
/// <param name="srcData">노드의 데이터</param>
/// <returns>생성 된 노드</returns>
NODE* DLL_CreateNode(DATA_TYPE srcData)
{
	NODE* retVal = (NODE*)malloc(sizeof(NODE));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_data = srcData;
	retVal->_prev = retVal->_next = NULL;

	return retVal;
}

/// <summary>
/// 대상 노드에 할당 된 메모리 해제
/// </summary>
/// <param name="srcNode">대상 노드</param>
void DLL_DeallocateNode(NODE** srcNode)
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
void DLL_DeallocateNodeList(NODE** srcList)
{
	while ((*srcList) != NULL)
	{
		NODE* tmp = (*srcList); //삭제 할 노드

		(*srcList) = (*srcList)->_next; //헤드 노드를 다음 노드로 이동 후 삭제
		DLL_DeallocateNode(&tmp);
	}
}

/// <summary>
/// 대상 리스트의 끝에 새 노드 삽입
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <param name="srcNewNode">리스트의 끝에 삽입하고자 하는 새 노드</param>
void DLL_AppendNode(NODE** srcList, NODE* srcNewNode)
{
	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcList) == NULL) //비어있을 경우
	{
		(*srcList) = srcNewNode; //헤드 노드 할당
	}
	else //끝에 새 노드 삽입
	{
		NODE* tail = (*srcList); //꼬리 노드

		while (tail->_next != NULL) //끝으로 이동
		{
			tail = tail->_next;
		}

		DLL_InsertNodeAfter(tail, srcNewNode);
	}
}

/// <summary>
/// 대상 리스트에서 헤드 노드 기준 상대적 특정 위치의 노드 반환
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <param name="position">헤드 노드 기준 상대적 특정 위치</param>
/// <returns>헤드 노드 기준 상대적 특정 위치의 노드</returns>
NODE* DLL_GetNodeAt(NODE** srcList, NODE_POSITION_TYPE position)
{
	if (position < 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	NODE* retVal = (*srcList); //현재 노드

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
void DLL_RemoveNodeAt(NODE** srcList, NODE_POSITION_TYPE position, bool deallocateAfterRemove)
{
	if (position < 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	NODE* current = (*srcList); //현재 노드

	while (current != NULL && (--position) >= 0) //삭제 대상 노드 위치까지 이동
	{
		current = current->_next;
	}

	if (current == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	DLL_RemoveNode(srcList, current, deallocateAfterRemove);
}

/// <summary>
/// 대상 리스트에서 삭제 대상 노드 삭제
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <param name="srcTargetNode">삭제 대상 노드</param>
/// <param name="deallocateAfterRemove">삭제 대상 노드에 대한 메모리 해제 수행 여부</param>
void DLL_RemoveNode(NODE** srcList, NODE* srcTargetNode, bool deallocateAfterRemove)
{
	/***
		1) TPE : 삭제 대상 노드의 이전 노드 존재 여부 (T : 존재, F : 미 존재)
		2) TNE : 삭제 대상 노드의 다음 노드 존재 여부 (T : 존재, F : 미 존재)
		3) D : 이에 따른 삭제 대상 노드 삭제 시 수행 작업

		TPE | TNE | D
		T	 T		1) 삭제 대상 노드의 다음 노드의 이전을 삭제 대상 노드의 이전 노드로 연결
					2) 삭제 대상 노드의 이전 노드의 다음을 삭제 대상 노드의 다음 노드로 연결
					3) 삭제 대상 노드의 이전, 다음 연결 해제 및 삭제 대상 노드 삭제

		T	 F		1) 삭제 대상 노드의 이전 노드의 다음을 삭제 대상 노드의 다음 노드로 연결 (NULL)
					2) 삭제 대상 노드의 이전, 다음 (NULL) 연결 해제 및 삭제 대상 노드 삭제

		F	 T		1) 헤드 노드를 삭제 대상 노드의 다음 노드로 변경
					2) 삭제 대상 노드의 다음 노드의 이전을 삭제 대상 노드의 이전 노드로 연결 (NULL)
					3) 삭제 대상 노드의 이전 (NULL), 다음 연결 해제 및 삭제 대상 노드 삭제

		F	 F		1) 삭제 대상 노드의 이전 (NULL), 다음 (NULL) 연결 해제 및 삭제 대상 노드 삭제
	***/

	if (srcTargetNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (srcTargetNode == (*srcList)) //F T (삭제 대상 노드가 헤드 노드인 경우)
	{
		(*srcList) = srcTargetNode->_next; //헤드 노드를 삭제 대상 노드의 다음 노드로 변경
	}

	if (srcTargetNode->_next != NULL) //- T
	{
		srcTargetNode->_next->_prev = srcTargetNode->_prev; //삭제 대상 노드의 다음 노드의 이전을 삭제 대상 노드의 이전 노드로 연결
	}

	if (srcTargetNode->_prev != NULL) //T -
	{
		srcTargetNode->_prev->_next = srcTargetNode->_next; //삭제 대상 노드의 이전 노드의 다음을 삭제 대상 노드의 다음 노드로 연결
	}

	srcTargetNode->_prev = NULL;
	srcTargetNode->_next = NULL;

	if (deallocateAfterRemove)
	{
		DLL_DeallocateNode(&srcTargetNode);
	}
}

/// <summary>
/// 대상 리스트의 맨 앞에 새 노드 삽입
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <param name="srcNewNode">삽입하고자 하는 새 노드</param>
void DLL_InsertNewHead(NODE** srcList, NODE* srcNewNode)
{
	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcList) == NULL)
	{
		(*srcList) = srcNewNode;
	}
	else //헤드 노드의 앞에 삽입
	{
		DLL_InsertNodeBefore(srcList, (*srcList), srcNewNode);
	}
}

/// <summary>
/// 대상 노드의 뒤에 새 노드 삽입
/// </summary>
/// <param name="srcTargetNode">대상 노드</param>
/// <param name="srcNewNode">삽입하고자 하는 새 노드</param>
void DLL_InsertNodeAfter(NODE* srcTargetNode, NODE* srcNewNode)
{
	/***
		환형 이중 연결 리스트와 비교하여 헤드와 꼬리가 서로 연결되지 않으므로,
		대상 노드의 뒤에 삽입 시 대상 노드의 다음 노드가 존재 할 경우만,
		대상 노드의 다음 노드의 이전을 새 노드로 연결
	***/

	if (srcTargetNode == NULL || srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	srcNewNode->_prev = srcTargetNode; //새 노드의 이전을 대상 노드로 연결
	srcNewNode->_next = srcTargetNode->_next; //새 노드의 다음을 대상 노드의 다음 노드로 연결

	if (srcTargetNode->_next != NULL) //대상 노드의 다음 노드가 존재 할 경우
	{
		srcTargetNode->_next->_prev = srcNewNode; //대상 노드의 다음 노드의 이전을 새 노드로 연결
	}

	srcTargetNode->_next = srcNewNode; //대상 노드의 다음을 새 노드로 연결
}

/// <summary>
/// 대상 리스트의 대상 노드의 앞에 새 노드 삽입
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <param name="srcTargetNode">대상 노드</param>
/// <param name="srcNewNode">삽입하고자 하는 새 노드</param>
void DLL_InsertNodeBefore(NODE** srcList, NODE* srcTargetNode, NODE* srcNewNode)
{
	/***
		환형 이중 연결 리스트와 비교하여 헤드와 꼬리가 서로 연결되지 않으므로,
		대상 노드의 앞에 삽입 시 대상 노드의 이전 노드가 존재 할 경우만 (대상 노드가 헤드 노드가 아닌 경우)
		대상 노드의 이전 노드의 다음을 새 노드로 연결
		---
		1) TH : 대상 노드의 헤드 노드 여부 (T : 헤드 노드, F : 중간 노드 혹은 꼬리 노드)
		2) D : 이에 따른 삽입하고자 하는 새 노드 삽입 시 수행 작업

		TH | D
		T	1) 새 노드의 다음을 대상 노드로 연결
			2) 새 노드의 이전을 대상 노드의 이전 노드로 연결
			3) 새 노드를 헤드 노드로 변경
			4) 대상 노드의 이전을 새 노드로 연결

		F	1) 새 노드의 다음을 대상 노드로 연결
			2) 새 노드의 이전을 대상 노드의 이전 노드로 연결
			3) 대상 노드의 이전 노드의 다음을 새 노드로 연결
			4) 대상 노드의 이전을 새 노드로 연결
	***/

	if (srcTargetNode == NULL || srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	srcNewNode->_next = srcTargetNode; //새 노드의 다음을 대상 노드로 연결
	srcNewNode->_prev = srcTargetNode->_prev; //새 노드의 이전을 대상 노드의 이전 노드로 연결

	if (srcTargetNode == (*srcList)) //T
	{
		(*srcList) = srcNewNode; //헤드 노드 변경
	}
	else //F
	{
		srcTargetNode->_prev->_next = srcNewNode; //대상 노드의 이전 노드의 다음을 새 노드로 연결
	}

	srcTargetNode->_prev = srcNewNode; //대상 노드의 이전을 새 노드로 연결
}

/// <summary>
/// 대상 리스트의 전체 노드의 수 반환
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <returns>대상 리스트의 전체 노드의 수</returns>
NODE_POSITION_TYPE DLL_GetTotalNodeCount(NODE** srcList)
{
	NODE_POSITION_TYPE totalNodeCount = 0; //전체 노드의 수
	NODE* current = (*srcList); //현재 노드

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
void DLL_DispNodeList(NODE** srcList)
{
	NODE_POSITION_TYPE currentPosition = 0; //현재 노드의 위치
	NODE* current = (*srcList); //현재 노드

	while (current != NULL)
	{
		std::cout << "List [" << currentPosition << "] : " << current->_data << std::endl;

		current = current->_next;
		currentPosition++;
	}
}