﻿#include "CDLL_Core.h"

/// <summary>
/// 새로운 노드 생성 및 생성 된 노드 반환
/// </summary>
/// <param name="srcData">노드의 데이터</param>
/// <returns>생성 된 노드</returns>
NODE* CDLL_CreateNode(DATA_TYPE srcData)
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
void CDLL_DeallocateNode(NODE** srcNode)
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
void CDLL_DeallocateNodeList(NODE** srcList)
{
	if ((*srcList) != NULL)
	{
		NODE* oldHead = (*srcList); //기존 헤드 노드

		do
		{
			NODE* tmp = (*srcList); //삭제 할 노드

			(*srcList) = (*srcList)->_next; //헤드 노드를 다음 노드로 이동 후 삭제
			CDLL_DeallocateNode(&tmp);
		} while ((*srcList) != oldHead); //기존 헤드 노드로 돌아올 때 까지

		(*srcList) = NULL;
	}
}

/// <summary>
/// 대상 리스트의 끝에 새 노드 삽입
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <param name="srcNewNode">리스트의 끝에 삽입하고자 하는 새 노드</param>
void CDLL_AppendNode(NODE** srcList, NODE* srcNewNode)
{
	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcList) == NULL) //비어있을 경우
	{
		(*srcList) = srcNewNode; //헤드 노드 할당
		(*srcList)->_prev = (*srcList)->_next = (*srcList);
	}
	else //끝에 새 노드 삽입
	{
		NODE* tail = (*srcList)->_prev; //꼬리 노드

		CDLL_InsertNodeAfter(tail, srcNewNode);
	}
}

/// <summary>
/// 대상 리스트에서 헤드 노드 기준 상대적 특정 위치의 노드 반환
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <param name="position">헤드 노드 기준 상대적 특정 위치</param>
/// <returns>헤드 노드 기준 상대적 특정 위치의 노드</returns>
NODE* CDLL_GetNodeAt(NODE** srcList, NODE_POSITION_TYPE position)
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
void CDLL_RemoveNodeAt(NODE** srcList, NODE_POSITION_TYPE position, bool deallocateAfterRemove)
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

	CDLL_RemoveNode(srcList, current, deallocateAfterRemove);
}

/// <summary>
/// 대상 리스트에서 삭제 대상 노드 삭제
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <param name="srcTargetNode">삭제 대상 노드</param>
/// <param name="deallocateAfterRemove">삭제 대상 노드에 대한 메모리 해제 수행 여부</param>
void CDLL_RemoveNode(NODE** srcList, NODE* srcTargetNode, bool deallocateAfterRemove)
{
	/***
		단순 이중 연결 리스트와 비교하여, 헤드와 꼬리가 연결되어있으므로,
		삭제 대상 노드의 다음 노드 혹은 이전 노드의 연결 여부와 상관없이 처리
		---
		1) TH : 삭제 대상 노드의 헤드 노드 여부 (T : 헤드 노드, F : 중간 노드 혹은 꼬리 노드)
		2) D : 이에 따른 삭제 대상 노드 삭제 시 수행 작업

		TH | D
		T	1) 헤드 노드를 삭제 대상 노드의 다음 노드로 변경
			2) 삭제 대상 노드의 다음 노드의 이전을 현재 노드의 이전 노드로 연결
			3) 삭제 대상 노드의 이전 노드의 다음을 현재 노드의 다음 노드로 연결
			4) 삭제 대상 노드의 이전, 다음 연결 해제 및 삭제 대상 노드 삭제

		F	1) 삭제 대상 노드의 다음 노드의 이전을 삭제 대상 노드의 이전 노드로 연결
			2) 삭제 대상 노드의 이전 노드의 다음을 삭제 대상 노드의 다음 노드로 연결
			3) 삭제 대상 노드의 이전, 다음 연결 해제 및 삭제 대상 노드 삭제
	***/

	if (srcTargetNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (srcTargetNode == (*srcList)) //삭제 대상 노드가 헤드 노드일 경우
	{
		(*srcList) = srcTargetNode->_next; //헤드 노드를 삭제 대상 노드의 다음 노드로 변경
	}

	srcTargetNode->_next->_prev = srcTargetNode->_prev; //삭제 대상 노드의 다음 노드의 이전을 현재 노드의 이전 노드로 연결
	srcTargetNode->_prev->_next = srcTargetNode->_next; //삭제 대상 노드의 이전 노드의 다음을 현재 노드의 다음 노드로 연결
	srcTargetNode->_prev = NULL;
	srcTargetNode->_next = NULL;

	if (deallocateAfterRemove)
	{
		CDLL_DeallocateNode(&srcTargetNode);
	}
}

/// <summary>
/// 대상 리스트의 맨 앞에 새 노드 삽입
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <param name="srcNewNode">삽입하고자 하는 새 노드</param>
void CDLL_InsertNewHead(NODE** srcList, NODE* srcNewNode)
{
	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcList) == NULL)
	{
		(*srcList) = srcNewNode;
		(*srcList)->_prev = (*srcList)->_next = (*srcList);
	}
	else //헤드 노드의 앞에 삽입
	{
		CDLL_InsertNodeBefore(srcList, (*srcList), srcNewNode);
	}
}

/// <summary>
/// 대상 노드의 뒤에 새 노드 삽입
/// </summary>
/// <param name="srcTargetNode">대상 노드</param>
/// <param name="srcNewNode">삽입하고자 하는 새 노드</param>
void CDLL_InsertNodeAfter(NODE* srcTargetNode, NODE* srcNewNode)
{
	if (srcTargetNode == NULL || srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	srcNewNode->_prev = srcTargetNode; //새 노드의 이전을 대상 노드로 연결
	srcNewNode->_next = srcTargetNode->_next; //새 노드의 다음을 대상 노드의 다음 노드로 연결
	srcTargetNode->_next->_prev = srcNewNode; //대상 노드의 다음 노드의 이전을 새 노드로 연결
	srcTargetNode->_next = srcNewNode; //대상 노드의 다음을 새 노드로 연결
}

/// <summary>
/// 대상 리스트의 대상 노드의 앞에 새 노드 삽입
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <param name="srcTargetNode">대상 노드</param>
/// <param name="srcNewNode">삽입하고자 하는 새 노드</param>
void CDLL_InsertNodeBefore(NODE** srcList, NODE* srcTargetNode, NODE* srcNewNode)
{
	/***
		1) TH : 대상 노드의 헤드 노드 여부 (T : 헤드 노드, F : 중간 노드 혹은 꼬리 노드)
		2) D : 이에 따른 삽입하고자 하는 새 노드 삽입 시 수행 작업

		TH | D
		T	1) 새 노드의 다음을 대상 노드로 연결
			2) 새 노드의 이전을 대상 노드의 이전 노드로 연결
			3) 대상 노드의 이전 노드의 다음을 새 노드로 연결
			4) 대상 노드의 이전을 새 노드로 연결
			5) 새 노드를 헤드 노드로 변경

		F	1) 새 노드의 다음을 대상 노드로 연결
			2) 새 노드의 이전을 대상 노드의 이전 노드로 연결
			3) 대상 노드의 이전 노드의 다음을 새 노드로 연결
			4) 대상 노드의 이전을 새 노드로 연결
	***/

	if (srcTargetNode == NULL || srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	srcNewNode->_next = srcTargetNode; //새 노드의 다음을 대상 노드로 연결
	srcNewNode->_prev = srcTargetNode->_prev; //새 노드의 이전을 대상 노드의 이전 노드로 연결
	srcTargetNode->_prev->_next = srcNewNode; //대상 노드의 이전 노드의 다음을 새 노드로 연결
	srcTargetNode->_prev = srcNewNode; //대상 노드의 이전을 새 노드로 연결

	if (srcTargetNode == (*srcList)) //T
	{
		(*srcList) = srcNewNode; //헤드 노드 변경
	}
}

/// <summary>
/// 대상 리스트의 전체 노드의 수 반환
/// </summary>
/// <param name="srcList">대상 리스트</param>
/// <returns>대상 리스트의 전체 노드의 수</returns>
NODE_POSITION_TYPE CDLL_GetTotalNodeCount(NODE** srcList)
{
	NODE_POSITION_TYPE totalNodeCount = 0; //전체 노드의 수
	NODE* current = (*srcList); //현재 노드

	while (current != NULL)
	{
		current = current->_next;
		totalNodeCount++;

		if (current == (*srcList)) //헤드 노드로 돌아올 경우
			break;
	}

	return totalNodeCount;
}

/// <summary>
/// 대상 리스트의 전체 노드에 대한 데이터 출력
/// </summary>
/// <param name="srcList">대상 리스트</param>
void CDLL_DispNodeList(NODE** srcList)
{
	NODE_POSITION_TYPE currentPosition = 0; //현재 노드의 위치
	NODE* current = (*srcList); //현재 노드

	while (current != NULL)
	{
		std::cout << "List [" << currentPosition << "] : " << current->_data << std::endl;

		current = current->_next;
		currentPosition++;

		if (current == (*srcList)) //헤드 노드로 돌아올 경우
			break;
	}
}