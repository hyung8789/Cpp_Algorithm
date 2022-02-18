#include "CQ_Core.h"

/// <summary>
/// 대상 큐에 할당 크기만큼 큐 생성
/// </summary>
/// <param name="srcCircularQueue">대상 큐</param>
/// <param name="capacity">할당 크기</param>
void CQ_CreateQueue(CircularQueue** srcCircularQueue, QueueIndexType capacity)
{
	if ((*srcCircularQueue) != NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Memleak"));

	if (capacity <= 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	(*srcCircularQueue) = (CircularQueue*)malloc(sizeof(CircularQueue));
	if ((*srcCircularQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	(*srcCircularQueue)->_nodeArray = (Node*)malloc(sizeof(Node) * (capacity + 1)); //할당 크기만큼 삽입 위해 할당 크기 + 1로 할당
	if ((*srcCircularQueue)->_nodeArray == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	(*srcCircularQueue)->_front = (*srcCircularQueue)->_rear = 0;
	(*srcCircularQueue)->_capacity = capacity + 1; //할당 크기만큼 삽입 위해 할당 크기 + 1로 할당
}

/// <summary>
/// 대상 큐에 할당 된 메모리 해제
/// </summary>
/// <param name="srcCircularQueue">대상 큐</param>
void CQ_DeallocateQueue(CircularQueue** srcCircularQueue)
{
	if ((*srcCircularQueue) != NULL)
	{
		free((*srcCircularQueue)->_nodeArray);
		(*srcCircularQueue)->_nodeArray = NULL;

		free(*srcCircularQueue);
		(*srcCircularQueue) = NULL;
	}
}

/// <summary>
/// 대상 큐의 순차적인 데이터 삽입
/// </summary>
/// <param name="srcCircularQueue">대상 큐</param>
/// <param name="srcData">삽입 할 데이터</param>
void CQ_Enqueue(CircularQueue** srcCircularQueue, DataType srcData)
{
	if ((*srcCircularQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (CQ_IsFull(srcCircularQueue)) //포화 상태일 경우
		throw std::logic_error(std::string(__func__) + std::string(" : Queue is Full"));

	(*srcCircularQueue)->_rear = ((*srcCircularQueue)->_rear + 1) % (*srcCircularQueue)->_capacity;
	(*srcCircularQueue)->_nodeArray[(*srcCircularQueue)->_rear]._data = srcData;
}

/// <summary>
/// 대상 큐의 순차적인 데이터 제거 및 반환
/// </summary>
/// <param name="srcCircularQueue">대상 큐</param>
/// <returns>대상 큐의 순차적인 데이터</returns>
DataType CQ_Dequeue(CircularQueue** srcCircularQueue)
{
	if ((*srcCircularQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (CQ_IsEmpty(srcCircularQueue)) //공백 상태일 경우
		throw std::logic_error(std::string(__func__) + std::string(" : Queue is Empty"));

	(*srcCircularQueue)->_front = ((*srcCircularQueue)->_front + 1) % (*srcCircularQueue)->_capacity;
	return (*srcCircularQueue)->_nodeArray[(*srcCircularQueue)->_front]._data;
}

/// <summary>
/// 대상 큐의 전체 노드 데이터 개수 반환
/// </summary>
/// <param name="srcCircularQueue">대상 큐</param>
/// <returns>대상 큐의 전체 노드 데이터 개수</returns>
QueueIndexType CQ_GetTotalNodeDataCount(CircularQueue** srcCircularQueue)
{
	if ((*srcCircularQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return ((*srcCircularQueue)->_rear - (*srcCircularQueue)->_front);
}

/// <summary>
/// 대상 큐의 공백 여부 반환
/// </summary>
/// <param name="srcCircularQueue">대상 큐</param>
/// <returns>대상 큐의 공백 여부</returns>
bool CQ_IsEmpty(CircularQueue** srcCircularQueue)
{
	if ((*srcCircularQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return ((*srcCircularQueue)->_front == (*srcCircularQueue)->_rear); //전단과 후단이 만나는 시점에 공백 상태
}

/// <summary>
/// 대상 큐의 포화 여부 반환
/// </summary>
/// <param name="srcCircularQueue">대상 큐</param>
/// <returns>대상 큐의 포화 여부</returns>
bool CQ_IsFull(CircularQueue** srcCircularQueue)
{
	if ((*srcCircularQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return ((*srcCircularQueue)->_rear + 1) % (*srcCircularQueue)->_capacity == (*srcCircularQueue)->_front; //다음에 삽입 될 후단의 위치에서 전단을 만나면 포화 상태
}
