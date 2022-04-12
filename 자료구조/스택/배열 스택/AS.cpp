#include "AS_Core.h"

/// <summary>
/// 대상 배열 스택에 할당 크기만큼 배열 스택 생성
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <param name="capacity">할당 크기</param>
void AS_CreateStack(ARRAY_STACK** srcArrayStack, STACK_INDEX_TYPE capacity)
{
	if ((*srcArrayStack) != NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Memleak"));

	if (capacity <= 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	(*srcArrayStack) = (ARRAY_STACK*)malloc(sizeof(ARRAY_STACK));
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	(*srcArrayStack)->_nodeArray = (NODE*)malloc(sizeof(NODE) * capacity); //할당 크기만큼 생성
	if ((*srcArrayStack)->_nodeArray == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	(*srcArrayStack)->_capacity = capacity;
	(*srcArrayStack)->_top = 0;
}

/// <summary>
/// 대상 배열 스택에 할당 된 메모리 해제
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
void AS_DeallocateArrayStack(ARRAY_STACK** srcArrayStack)
{
	if ((*srcArrayStack) != NULL)
	{
		free((*srcArrayStack)->_nodeArray);
		(*srcArrayStack)->_nodeArray = NULL;

		free(*srcArrayStack);
		(*srcArrayStack) = NULL;
	}
}

/// <summary>
/// 대상 배열 스택의 순차적인 최상위 데이터 삽입
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <param name="srcData">삽입 할 데이터</param>
void AS_Push(ARRAY_STACK* srcArrayStack, DATA_TYPE srcData)
{
	if (srcArrayStack == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (AS_IsFull(srcArrayStack)) //가득 찼으면
	{
		STACK_INDEX_TYPE reallocCapacity = srcArrayStack->_capacity + ceil(srcArrayStack->_capacity * CAPACITY_REALLOC_RATIO); //재 할당 될 크기
		STACK_INDEX_TYPE reallocSizeInBytes = sizeof(NODE) * reallocCapacity; //재 할당 될 바이트 단위 크기

		void* reallocAddr = realloc(srcArrayStack->_nodeArray, reallocSizeInBytes);
		if (reallocAddr == NULL)
		{
			VAR_DUMP(reallocCapacity);
			VAR_DUMP(reallocSizeInBytes);

			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));
		}

		srcArrayStack->_nodeArray = (NODE*)reallocAddr;
		srcArrayStack->_capacity = reallocCapacity;
	}

	STACK_INDEX_TYPE index = srcArrayStack->_top++; //Push 할 인덱스
	srcArrayStack->_nodeArray[index]._data = srcData;
}

/// <summary>
/// 대상 배열 스택의 순차적인 최상위 데이터 제거 및 반환
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <returns>대상 배열 스택의 최상위 데이터</returns>
DATA_TYPE AS_Pop(ARRAY_STACK* srcArrayStack)
{
	if (srcArrayStack == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (AS_IsEmpty(srcArrayStack)) //비어있으면
		throw std::logic_error(std::string(__func__) + std::string(" : Empty Stack"));

	if (((static_cast<double>(AS_GetTotalNodeCount(srcArrayStack)) / static_cast<double>(srcArrayStack->_capacity))) <
		CAPACITY_REALLOC_RATIO_THRESHOLD)
	{
		STACK_INDEX_TYPE reallocCapacity = srcArrayStack->_capacity - floor(srcArrayStack->_capacity * CAPACITY_REALLOC_RATIO); //재 할당 될 크기
		STACK_INDEX_TYPE reallocSizeInBytes = sizeof(NODE) * reallocCapacity; //재 할당 될 바이트 단위 크기

		void* reallocAddr = realloc(srcArrayStack->_nodeArray, reallocSizeInBytes);
		if (reallocAddr == NULL)
		{
			VAR_DUMP(reallocCapacity);
			VAR_DUMP(reallocSizeInBytes);

			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));
		}

		srcArrayStack->_nodeArray = (NODE*)reallocAddr;
		srcArrayStack->_capacity = reallocCapacity;
	}

	STACK_INDEX_TYPE index = --(srcArrayStack->_top); //Pop 할 인덱스
	return srcArrayStack->_nodeArray[index]._data;
}

/// <summary>
/// 대상 배열 스택의 순차적인 최상위 데이터 제거없이 반환 
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <returns>대상 배열 스택의 최상위 데이터</returns>
DATA_TYPE AS_Peek(ARRAY_STACK* srcArrayStack)
{
	if (srcArrayStack == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (AS_IsEmpty(srcArrayStack)) //비어있으면
		throw std::logic_error(std::string(__func__) + std::string(" : Empty Stack"));

	STACK_INDEX_TYPE index = (srcArrayStack->_top) - 1; //Peek 할 인덱스
	return srcArrayStack->_nodeArray[index]._data;
}

/// <summary>
/// 대상 배열 스택의 전체 노드 개수 반환
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <returns>대상 배열 스택의 전체 노드 개수</returns>
STACK_INDEX_TYPE AS_GetTotalNodeCount(ARRAY_STACK* srcArrayStack)
{
	if (srcArrayStack == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return srcArrayStack->_top;
}

/// <summary>
/// 대상 배열 스택의 공백 여부 반환
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <returns>대상 배열 스택의 공백 여부</returns>
bool AS_IsEmpty(ARRAY_STACK* srcArrayStack)
{
	if (srcArrayStack == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return (srcArrayStack->_top == 0);
}

/// <summary>
/// 대상 배열 스택의 삽입 가능 여부 반환
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <returns>대상 배열 스택의 삽입 가능 여부</returns>
bool AS_IsFull(ARRAY_STACK* srcArrayStack)
{
	if (srcArrayStack == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return (srcArrayStack->_capacity == srcArrayStack->_top); //할당 크기와 동일해지는 시점부터 더 이상 삽입 불가능
}