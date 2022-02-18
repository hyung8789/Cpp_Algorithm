#include "LLQ_Core.h"

/// <summary>
/// 대상 큐에 큐 생성
/// </summary>
/// <param name="srcLinkedListQueue">대상 큐</param>
void LLQ_CreateQueue(LinkedListQueue** srcLinkedListQueue)
{
	if ((*srcLinkedListQueue) != NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Memleak"));

	(*srcLinkedListQueue) = (LinkedListQueue*)malloc(sizeof(LinkedListQueue));
	if ((*srcLinkedListQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	(*srcLinkedListQueue)->_front = (*srcLinkedListQueue)->_rear = NULL;
	(*srcLinkedListQueue)->_totalNodeCount = 0;
}

/// <summary>
/// 대상 큐에 할당 된 메모리 해제
/// </summary>
/// <param name="srcLinkedListQueue">대상 큐</param>
void LLQ_DeallocateQueue(LinkedListQueue** srcLinkedListQueue)
{
	if ((*srcLinkedListQueue) != NULL)
	{
		while (!LLQ_IsEmpty(srcLinkedListQueue)) //대상 큐에 할당 된 전체 노드에 대해
		{
			Node* tmp = LLQ_Dequeue(srcLinkedListQueue);
			LLQ_DeallocateNode(&tmp);
		}

		free(*srcLinkedListQueue);
		(*srcLinkedListQueue) = NULL;
	}
}

/// <summary>
/// 새로운 노드 생성 및 생성 된 노드 반환
/// </summary>
/// <param name="srcData">노드의 데이터</param>
/// <returns>생성 된 노드</returns>
Node* LLQ_CreateNode(DataType srcData)
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
void LLQ_DeallocateNode(Node** srcNode)
{
	if ((*srcNode) != NULL)
	{
		free(*srcNode);
		(*srcNode) = NULL;
	}
}

/// <summary>
/// 대상 큐의 순차적인 노드 삽입
/// </summary>
/// <param name="srcLinkedListQueue">대상 큐</param>
/// <param name="srcNewNode">삽입 할 노드</param>
void LLQ_Enqueue(LinkedListQueue** srcLinkedListQueue, Node* srcNewNode)
{
	if ((*srcLinkedListQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (LLQ_IsEmpty(srcLinkedListQueue)) //큐가 비어있으면
	{
		(*srcLinkedListQueue)->_front = (*srcLinkedListQueue)->_rear = srcNewNode;
	}
	else
	{
		(*srcLinkedListQueue)->_rear->_next = srcNewNode; //후단 노드의 다음을 새 노드로 연결
		(*srcLinkedListQueue)->_rear = srcNewNode; //후단 노드를 새 노드로 변경
	}

	(*srcLinkedListQueue)->_totalNodeCount++;
}

/// <summary>
/// 대상 큐의 순차적인 노드 제거 및 반환
/// </summary>
/// <param name="srcLinkedListQueue">대상 큐</param>
/// <returns>대상 큐의 순차적인 노드</returns>
Node* LLQ_Dequeue(LinkedListQueue** srcLinkedListQueue)
{
	if ((*srcLinkedListQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (LLQ_IsEmpty(srcLinkedListQueue)) //공백 상태일 경우
		throw std::logic_error(std::string(__func__) + std::string(" : Queue is Empty"));

	Node* retVal = (*srcLinkedListQueue)->_front;
	(*srcLinkedListQueue)->_front = (*srcLinkedListQueue)->_front->_next;
	(*srcLinkedListQueue)->_totalNodeCount--;

	if (LLQ_IsEmpty(srcLinkedListQueue)) //제거에 의한 공백 상태가 될 경우
		(*srcLinkedListQueue)->_rear = NULL; //제거 된 노드에 대해 관리하지 않음

	return retVal;
}

/// <summary>
/// 대상 큐의 전체 노드 개수 반환
/// </summary>
/// <param name="srcCircularQueue">대상 큐</param>
/// <returns>대상 큐의 전체 노드 개수</returns>
QueueIndexType LLQ_GetTotalNodeCount(LinkedListQueue** srcLinkedListQueue)
{
	if ((*srcLinkedListQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return (*srcLinkedListQueue)->_totalNodeCount;
}

/// <summary>
/// 대상 큐의 공백 여부 반환
/// </summary>
/// <param name="srcLinkedListQueue">대상 큐</param>
/// <returns>대상 큐의 공백 여부</returns>
bool LLQ_IsEmpty(LinkedListQueue** srcLinkedListQueue)
{
	if ((*srcLinkedListQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return ((*srcLinkedListQueue)->_totalNodeCount == 0);
}