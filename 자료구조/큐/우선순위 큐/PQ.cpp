#include "PQ_Core.h"

/// <summary>
/// 할당 크기만큼 큐 생성 및 반환
/// </summary>
/// <param name="capacity">할당 크기</param>
/// <returns>생성 된 큐</returns>
PRIORITY_QUEUE* PQ_CreateQueue(QUEUE_INDEX_TYPE capacity)
{
	PRIORITY_QUEUE* retVal = NULL;

	if (capacity <= 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	retVal = (PRIORITY_QUEUE*)malloc(sizeof(PRIORITY_QUEUE));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_nodeArray = (NODE*)malloc(sizeof(NODE) * capacity); //할당 크기만큼 생성
	if (retVal->_nodeArray == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_capacity = capacity;
	retVal->_usedSize = 0;

	return retVal;
}

/// <summary>
/// 대상 큐에 할당 된 메모리 해제
/// </summary>
/// <param name="srcPriorityQueue">대상 큐</param>
void PQ_DeallocateQueue(PRIORITY_QUEUE** srcPriorityQueue)
{
	if ((*srcPriorityQueue) != NULL)
	{
		free((*srcPriorityQueue)->_nodeArray);
		(*srcPriorityQueue)->_nodeArray = NULL;

		free(*srcPriorityQueue);
		(*srcPriorityQueue) = NULL;
	}
}

/// <summary>
/// 대상 큐의 우선순위에 따른 새 노드 삽입
/// </summary>
/// <param name="srcPriorityQueue">대상 큐</param>
/// <param name="srcNewNode">삽입 할 새 노드</param>
void PQ_Enqueue(PRIORITY_QUEUE* srcPriorityQueue, NODE* srcNewNode)
{
	if (srcPriorityQueue == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (PQ_IsFull(srcPriorityQueue))
	{
		QUEUE_INDEX_TYPE reallocCapacity = srcPriorityQueue->_capacity + ceil(srcPriorityQueue->_capacity * CAPACITY_REALLOC_RATIO); //재 할당 될 크기
		QUEUE_INDEX_TYPE reallocSizeInBytes = sizeof(NODE) * reallocCapacity; //재 할당 될 바이트 단위 크기

		void* reallocAddr = realloc(srcPriorityQueue->_nodeArray, reallocSizeInBytes);
		if (reallocAddr == NULL)
		{
			VAR_DUMP(srcPriorityQueue->_capacity);
			VAR_DUMP(reallocCapacity);
			VAR_DUMP(reallocSizeInBytes);

			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));
		}

		srcPriorityQueue->_nodeArray = (NODE*)reallocAddr;
		srcPriorityQueue->_capacity = reallocCapacity;
	}

	QUEUE_INDEX_TYPE currentIndex = srcPriorityQueue->_usedSize; //삽입 될 노드의 인덱스
	QUEUE_INDEX_TYPE parentIndex = PQ_GetParentIndex(currentIndex); //삽입 될 노드의 부모 인덱스

	NODE tmpNode;

	srcPriorityQueue->_nodeArray[currentIndex] = (*srcNewNode);

	while (currentIndex > 0) //삽입 된 노드의 우선순위에 따른 적절한 위치 판별 및 이동
	{
		switch (PRIORITY_ORDER_BY)
		{
		case HIGH_PRIORITY_FIRST:
			if (srcPriorityQueue->_nodeArray[currentIndex]._priority < srcPriorityQueue->_nodeArray[parentIndex]._priority)
				SWAP(srcPriorityQueue->_nodeArray[currentIndex], srcPriorityQueue->_nodeArray[parentIndex], tmpNode);
			else
				goto END_PROC;
			break;

		case LOW_PRIORITY_FIRST:
			if (srcPriorityQueue->_nodeArray[currentIndex]._priority > srcPriorityQueue->_nodeArray[parentIndex]._priority)
				SWAP(srcPriorityQueue->_nodeArray[currentIndex], srcPriorityQueue->_nodeArray[parentIndex], tmpNode);
			else
				goto END_PROC;
			break;
		}

		currentIndex = parentIndex;
		parentIndex = PQ_GetParentIndex(currentIndex);
	}

END_PROC:
	srcPriorityQueue->_usedSize++;
}

/// <summary>
/// 대상 큐의 우선순위에 따른 노드 제거 및 출력
/// </summary>
/// <param name="srcPriorityQueue">대상 큐</param>
/// <param name="dstNode">출력 될 우선순위에 따른 노드</param>
void PQ_Dequeue(PRIORITY_QUEUE* srcPriorityQueue, NODE* dstNode)
{
	if (srcPriorityQueue == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (PQ_IsEmpty(srcPriorityQueue))
		throw std::logic_error(std::string(__func__) + std::string(" : Empty Queue"));

	memcpy(dstNode, &(srcPriorityQueue->_nodeArray[0]), sizeof(NODE));

	NODE tmpNode;

	--(srcPriorityQueue->_usedSize);
	srcPriorityQueue->_nodeArray[0] = srcPriorityQueue->_nodeArray[srcPriorityQueue->_usedSize]; //루트 노드로 마지막으로 삽입 된 노드 이동

	if ((static_cast<double>(srcPriorityQueue->_usedSize) / static_cast<double>(srcPriorityQueue->_capacity)) <
		CAPACITY_REALLOC_RATIO_THRESHOLD)
	{
		QUEUE_INDEX_TYPE reallocCapacity = srcPriorityQueue->_capacity - floor(srcPriorityQueue->_capacity * CAPACITY_REALLOC_RATIO); //재 할당 될 크기
		QUEUE_INDEX_TYPE reallocSizeInBytes = sizeof(NODE) * reallocCapacity; //재 할당 될 바이트 단위 크기

		void* reallocAddr = realloc(srcPriorityQueue->_nodeArray, reallocSizeInBytes);
		if (reallocAddr == NULL)
		{
			VAR_DUMP(srcPriorityQueue->_capacity);
			VAR_DUMP(reallocCapacity);
			VAR_DUMP(reallocSizeInBytes);

			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));
		}

		srcPriorityQueue->_nodeArray = (NODE*)reallocAddr;
		srcPriorityQueue->_capacity = reallocCapacity;
	}

	QUEUE_INDEX_TYPE currentIndex = 0; //루트 노드로 이동 된 마지막으로 삽입 된 노드의 인덱스

	while (1) //루트 노드로 이동 된 마지막으로 삽입 된 노드의 우선순위에 따른 적절한 위치 판별 및 이동
	{
		QUEUE_INDEX_TYPE leftChildIndex = PQ_GetLeftChildIndex(currentIndex);
		QUEUE_INDEX_TYPE rightChildIndex = leftChildIndex + 1;
		QUEUE_INDEX_TYPE cmpTargetIndex = 0; //비교 대상 인덱스

		if (leftChildIndex >= srcPriorityQueue->_usedSize)
			break;

		switch (PRIORITY_ORDER_BY)
		{
		case HIGH_PRIORITY_FIRST:
			//왼쪽 자식과 오른쪽 자식 중 더 높은 우선순위와 루트 노드로 이동 된 마지막으로 삽입 된 노드의 우선순위 비교
			cmpTargetIndex =
				(srcPriorityQueue->_nodeArray[leftChildIndex]._priority < srcPriorityQueue->_nodeArray[rightChildIndex]._priority) ?
				leftChildIndex : rightChildIndex;

			if (srcPriorityQueue->_nodeArray[currentIndex]._priority > srcPriorityQueue->_nodeArray[cmpTargetIndex]._priority)
			{
				SWAP(srcPriorityQueue->_nodeArray[currentIndex], srcPriorityQueue->_nodeArray[cmpTargetIndex], tmpNode);
				currentIndex = cmpTargetIndex; //다음 비교 위해 이동
			}
			else //루트 노드로 이동 된 마지막으로 삽입 된 노드의 현재 위치가 적절한 위치일 경우
			{
				goto END_PROC;
			}

			break;

		case LOW_PRIORITY_FIRST:
			//왼쪽 자식과 오른쪽 자식 중 더 낮은 우선순위와 루트 노드로 이동 된 마지막으로 삽입 된 노드의 우선순위 비교
			cmpTargetIndex =
				(srcPriorityQueue->_nodeArray[leftChildIndex]._priority > srcPriorityQueue->_nodeArray[rightChildIndex]._priority) ?
				leftChildIndex : rightChildIndex;

			if (srcPriorityQueue->_nodeArray[currentIndex]._priority < srcPriorityQueue->_nodeArray[cmpTargetIndex]._priority)
			{
				SWAP(srcPriorityQueue->_nodeArray[currentIndex], srcPriorityQueue->_nodeArray[cmpTargetIndex], tmpNode);
				currentIndex = cmpTargetIndex; //다음 비교 위해 이동
			}
			else //루트 노드로 이동 된 마지막으로 삽입 된 노드의 현재 위치가 적절한 위치일 경우
			{
				goto END_PROC;
			}

			break;
		}
	}

END_PROC:
	return;
}

/// <summary>
/// 대상 큐의 우선순위에 따른 읽기 전용 데이터 참조 반환
/// </summary>
/// <param name="srcPriorityQueue">대상 큐</param>
/// <returns>우선순위에 따른 읽기 전용 데이터 참조</returns>
const NODE& PQ_Peek(PRIORITY_QUEUE* srcPriorityQueue)
{
	if (srcPriorityQueue == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (PQ_IsEmpty(srcPriorityQueue))
		throw std::logic_error(std::string(__func__) + std::string(" : Empty Queue"));

	return srcPriorityQueue->_nodeArray[0];
}

/// <summary>
/// 대상 큐의 전체 노드 개수 반환
/// </summary>
/// <param name="srcPriorityQueue">대상 큐</param>
/// <returns>대상 큐의 전체 노드 개수</returns>
QUEUE_INDEX_TYPE PQ_GetTotalNodeCount(PRIORITY_QUEUE* srcPriorityQueue)
{
	if (srcPriorityQueue == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return srcPriorityQueue->_usedSize;
}

/// <summary>
/// 대상 큐의 공백 여부 반환
/// </summary>
/// <param name="srcPriorityQueue">대상 큐</param>
/// <returns>대상 큐의 공백 여부</returns>
bool PQ_IsEmpty(PRIORITY_QUEUE* srcPriorityQueue)
{
	if (srcPriorityQueue == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return (srcPriorityQueue->_usedSize == 0);
}

/// <summary>
/// 대상 큐의 포화 여부 반환
/// </summary>
/// <param name="srcPriorityQueue">대상 큐</param>
/// <returns>대상 큐의 포화 여부</returns>
bool PQ_IsFull(PRIORITY_QUEUE* srcPriorityQueue)
{
	if (srcPriorityQueue == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return (srcPriorityQueue->_usedSize == srcPriorityQueue->_capacity);
}

/// <summary>
/// 대상 인덱스의 부모 인덱스 반환
/// </summary>
/// <param name="targetIndex">대상 인덱스</param>
/// <returns>대상 인덱스의 부모 인덱스</returns>
QUEUE_INDEX_TYPE PQ_GetParentIndex(QUEUE_INDEX_TYPE targetIndex)
{
	if (targetIndex == 0)
		return 0;

	return (QUEUE_INDEX_TYPE)((targetIndex - 1) / 2);
}

/// <summary>
/// 대상 인덱스의 왼쪽 자식 인덱스 반환
/// </summary>
/// <param name="targetIndex"대상 인덱스></param>
/// <returns>대상 인덱스의 왼쪽 자식 인덱스</returns>
QUEUE_INDEX_TYPE PQ_GetLeftChildIndex(QUEUE_INDEX_TYPE targetIndex)
{
	return (2 * targetIndex) + 1;
}

/// <summary>
/// 대상 인덱스의 오른쪽 자식 인덱스 반환
/// </summary>
/// <param name="targetIndex">대상 인덱스</param>
/// <returns>대상 인덱스의 오른쪽 자식 인덱스</returns>
QUEUE_INDEX_TYPE PQ_GetRightChildIndex(QUEUE_INDEX_TYPE targetIndex)
{
	return (2 * targetIndex) + 2;
}

/// <summary>
/// 대상 인덱스에 해당하는 깊이 반환
/// </summary>
/// <param name="targetIndex">대상 인덱스</param>
/// <returns>대상 인덱스에 해당하는 깊이</returns>
QUEUE_INDEX_TYPE PQ_GetDepthFromIndex(QUEUE_INDEX_TYPE targetIndex)
{
	return (QUEUE_INDEX_TYPE)floor(log2(targetIndex + 1)); //ex) index : 1 => depth : 1
}
