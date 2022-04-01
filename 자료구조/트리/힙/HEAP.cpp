#include "HEAP_Core.h"

/// <summary>
/// 할당 크기만큼 힙 생성
/// </summary>
/// <param name="capacity">할당 크기</param>
/// <returns>생성 된 힙</returns>
HEAP* HEAP_CreateHeap(HEAP_INDEX_TYPE capacity)
{
	HEAP* retVal = NULL;

	if (capacity <= 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	retVal = (HEAP*)malloc(sizeof(HEAP));
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
/// 대상 힙에 할당 된 메모리 해제
/// </summary>
/// <param name="srcHeap">대상 힙</param>
void HEAP_DeallocateHeap(HEAP** srcHeap)
{
	if ((*srcHeap) != NULL)
	{
		free((*srcHeap)->_nodeArray);
		(*srcHeap)->_nodeArray = NULL;

		free(*srcHeap);
		(*srcHeap) = NULL;
	}
}

/// <summary>
/// 대상 힙 출력
/// </summary>
/// <param name="srcHeap">대상 힙</param>
void HEAP_DispHeap(HEAP* srcHeap)
{
	for (HEAP_INDEX_TYPE i = 0; i < srcHeap->_usedSize; i++)
	{
		HEAP_INDEX_TYPE depth = HEAP_GetDepthFromIndex(i); //대상 인덱스에 해당하는 깊이

		for (HEAP_INDEX_TYPE j = 0; j < depth; j++)
		{
			std::cout << "|";
		}

		std::cout << "- " << srcHeap->_nodeArray[i]._data;
		
		if (i == 0)
			std::cout << " (Root)";
		else
			std::cout << " (Parent : " << srcHeap->_nodeArray[HEAP_GetParentIndex(i)]._data << ")";
		std::cout << "\n";
	}
}

/// <summary>
/// 대상 힙의 힙 순서 속성에 따른 데이터 삽입
/// </summary>
/// <param name="srcHeap">대상 힙</param>
/// <param name="srcData">삽입 할 데이터</param>
void HEAP_Push(HEAP* srcHeap, const DATA_TYPE& srcData)
{
	if (srcHeap == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (srcHeap->_usedSize == srcHeap->_capacity)
	{
		HEAP_INDEX_TYPE reallocCapacity = srcHeap->_capacity + ceil(srcHeap->_capacity * CAPACITY_REALLOC_RATIO); //재 할당 될 크기
		HEAP_INDEX_TYPE reallocSizeInBytes = sizeof(NODE) * reallocCapacity; //재 할당 될 바이트 단위 크기
		void* reallocAddr = realloc(srcHeap->_nodeArray, reallocSizeInBytes);
		if (reallocAddr == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		srcHeap->_nodeArray = (NODE*)reallocAddr;
		srcHeap->_capacity = reallocCapacity;
	}
	
	HEAP_INDEX_TYPE currentIndex = srcHeap->_usedSize; //삽입 될 데이터의 인덱스
	HEAP_INDEX_TYPE parentIndex = HEAP_GetParentIndex(currentIndex); //삽입 될 데이터의 인덱스의 부모 인덱스

	DATA_TYPE tmpData;

	srcHeap->_nodeArray[currentIndex]._data = srcData;

	while (currentIndex > 0) //삽입 된 데이터의 적절한 위치 판별 및 이동
	{
		switch (HEAP_ORDER_PROPERTY)
		{
		case MIN_HEAP:
			if (srcHeap->_nodeArray[currentIndex]._data < srcHeap->_nodeArray[parentIndex]._data)
				SWAP(srcHeap->_nodeArray[currentIndex]._data, srcHeap->_nodeArray[parentIndex]._data, tmpData);
			else
				goto END_PROC;
			break;

		case MAX_HEAP:
			if (srcHeap->_nodeArray[currentIndex]._data > srcHeap->_nodeArray[parentIndex]._data)
				SWAP(srcHeap->_nodeArray[currentIndex]._data, srcHeap->_nodeArray[parentIndex]._data, tmpData);
			else
				goto END_PROC;
			break;
		}

		currentIndex = parentIndex;
		parentIndex = HEAP_GetParentIndex(currentIndex);
	}

END_PROC:
	srcHeap->_usedSize++;
}

/// <summary>
/// 대상 힙의 힙 순서 속성에 따른 데이터 제거 및 반환
/// </summary>
/// <param name="srcHeap">대상 힙</param>
DATA_TYPE HEAP_Pop(HEAP* srcHeap)
{
	if (srcHeap == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (srcHeap->_usedSize == 0)
		throw std::logic_error(std::string(__func__) + std::string(" : Empty Heap"));

	DATA_TYPE retVal = srcHeap->_nodeArray[0]._data;
	DATA_TYPE tmpData;

	srcHeap->_nodeArray[0]._data = srcHeap->_nodeArray[--(srcHeap->_usedSize)]._data; //루트 노드로 마지막으로 삽입 된 노드의 데이터 이동
	memset(&(srcHeap->_nodeArray[srcHeap->_usedSize]), NULL, sizeof(NODE)); //마지막으로 삽입 된 노드 제거

	if ((srcHeap->_usedSize / srcHeap->_capacity) <
		CAPACITY_REALLOC_RATIO_THRESHOLD)
	{
		HEAP_INDEX_TYPE reallocCapacity = srcHeap->_capacity - ceil(srcHeap->_capacity * CAPACITY_REALLOC_RATIO); //재 할당 될 크기
		HEAP_INDEX_TYPE reallocSizeInBytes = sizeof(NODE) * reallocCapacity; //재 할당 될 바이트 단위 크기

		srcHeap->_nodeArray = (NODE*)realloc(srcHeap->_nodeArray, reallocSizeInBytes);
		srcHeap->_capacity = reallocCapacity;
	}

	HEAP_INDEX_TYPE currentIndex = 0; //루트 노드로 이동 된 마지막으로 삽입 된 노드 데이터의 인덱스

	while (1) //루트 노드로 이동 된 마지막으로 삽입 된 노드 데이터의 적절한 위치 판별 및 이동
	{
		HEAP_INDEX_TYPE leftChildIndex = HEAP_GetLeftChildIndex(currentIndex);
		HEAP_INDEX_TYPE rightChildIndex = leftChildIndex + 1;
		HEAP_INDEX_TYPE cmpTargetIndex = 0; //비교 대상 인덱스

		if (leftChildIndex >= srcHeap->_usedSize)
			break;

		switch (HEAP_ORDER_PROPERTY)
		{
		case MIN_HEAP:
			//왼쪽 자식과 오른쪽 자식 중 더 작은 데이터와 루트 노드로 이동 된 마지막으로 삽입 된 노드 데이터 비교
			cmpTargetIndex = 
				(srcHeap->_nodeArray[leftChildIndex]._data < srcHeap->_nodeArray[rightChildIndex]._data) ?
				leftChildIndex : rightChildIndex;

			if (srcHeap->_nodeArray[currentIndex]._data > srcHeap->_nodeArray[cmpTargetIndex]._data)
			{
				SWAP(srcHeap->_nodeArray[currentIndex]._data, srcHeap->_nodeArray[cmpTargetIndex]._data, tmpData);
				currentIndex = cmpTargetIndex; //다음 비교 위해 이동
			}
			else //루트 노드로 이동 된 마지막으로 삽입 된 노드 데이터의 현재 위치가 적절한 위치일 경우
			{
				goto END_PROC;
			}

			break;

		case MAX_HEAP:
			//왼쪽 자식과 오른쪽 자식 중 더 큰 데이터와 마지막으로 삽입 된 노드 데이터 비교
			cmpTargetIndex = 
				(srcHeap->_nodeArray[leftChildIndex]._data > srcHeap->_nodeArray[rightChildIndex]._data) ?
				leftChildIndex : rightChildIndex;

			if (srcHeap->_nodeArray[currentIndex]._data < srcHeap->_nodeArray[cmpTargetIndex]._data)
			{
				SWAP(srcHeap->_nodeArray[currentIndex]._data, srcHeap->_nodeArray[cmpTargetIndex]._data, tmpData);
				currentIndex = cmpTargetIndex; //다음 비교 위해 이동
			}
			else //루트 노드로 이동 된 마지막으로 삽입 된 노드 데이터의 현재 위치가 적절한 위치일 경우
			{
				goto END_PROC;
			}

			break;
		}
	}

END_PROC:
	return retVal;
}

/// <summary>
/// 대상 힙의 힙 순서 속성에 따른 읽기 전용 데이터 참조 반환
/// </summary>
/// <param name="srcHeap">대상 힙</param>
const DATA_TYPE& HEAP_Peek(HEAP* srcHeap)
{
	if (srcHeap == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (srcHeap->_usedSize == 0)
		throw std::logic_error(std::string(__func__) + std::string(" : Empty Heap"));

	return srcHeap->_nodeArray[0]._data;
}

/// <summary>
/// 대상 인덱스의 부모 인덱스 반환
/// </summary>
/// <param name="targetIndex">대상 인덱스</param>
/// <returns>대상 인덱스의 부모 인덱스</returns>
HEAP_INDEX_TYPE HEAP_GetParentIndex(HEAP_INDEX_TYPE targetIndex)
{
	if (targetIndex == 0)
		return 0;

	return (HEAP_INDEX_TYPE)((targetIndex - 1) / 2);
}

/// <summary>
/// 대상 인덱스의 왼쪽 자식 인덱스 반환
/// </summary>
/// <param name="targetIndex"대상 인덱스></param>
/// <returns>대상 인덱스의 왼쪽 자식 인덱스</returns>
HEAP_INDEX_TYPE HEAP_GetLeftChildIndex(HEAP_INDEX_TYPE targetIndex)
{
	return (2 * targetIndex) + 1;
}

/// <summary>
/// 대상 인덱스의 오른쪽 자식 인덱스 반환
/// </summary>
/// <param name="targetIndex">대상 인덱스</param>
/// <returns>대상 인덱스의 오른쪽 자식 인덱스</returns>
HEAP_INDEX_TYPE HEAP_GetRightChildIndex(HEAP_INDEX_TYPE targetIndex)
{
	return (2 * targetIndex) + 2;
}

/// <summary>
/// 대상 인덱스에 해당하는 깊이 반환
/// </summary>
/// <param name="targetIndex">대상 인덱스</param>
/// <returns>대상 인덱스에 해당하는 깊이</returns>
HEAP_INDEX_TYPE HEAP_GetDepthFromIndex(HEAP_INDEX_TYPE targetIndex)
{
	return (HEAP_INDEX_TYPE)floor(log2(targetIndex + 1)); //ex) index : 1 => depth : 1
}
