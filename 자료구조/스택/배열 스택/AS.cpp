#include "AS_Core.h"

/// <summary>
/// 대상 배열 스택에 할당 크기만큼 배열 스택 생성
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <param name="capacity">할당 크기</param>
void AS_CreateStack(ArrayStack** srcArrayStack, StackIndexType capacity)
{
	if ((*srcArrayStack) != NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Memleak"));

	if (capacity <= 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	(*srcArrayStack) = (ArrayStack*)malloc(sizeof(ArrayStack));
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	(*srcArrayStack)->nodeArray = (Node*)malloc(sizeof(Node) * capacity); //할당 크기만큼 생성
	if ((*srcArrayStack)->nodeArray == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	(*srcArrayStack)->capacity = capacity;
	(*srcArrayStack)->top = 0;
}

/// <summary>
/// 대상 배열 스택에 할당 된 메모리 해제
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
void AS_DeallocateArrayStack(ArrayStack** srcArrayStack)
{
	if ((*srcArrayStack) != NULL)
	{
		free((*srcArrayStack)->nodeArray);
		(*srcArrayStack)->nodeArray = NULL;

		free(*srcArrayStack);
		(*srcArrayStack) = NULL;
	}
}

/// <summary>
/// 대상 배열 스택의 순차적인 최상위 데이터 삽입
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <param name="srcData">삽입 할 데이터</param>
void AS_Push(ArrayStack** srcArrayStack, DataType srcData)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (AS_IsFull(srcArrayStack)) //가득 찼으면
	{
		StackIndexType reallocCapacity = (*srcArrayStack)->capacity + ceil((*srcArrayStack)->capacity * CAPACITY_REALLOC_RATIO); //재 할당 될 크기
		StackIndexType reallocSizeInBytes = sizeof(Node) * reallocCapacity; //재 할당 될 바이트 단위 크기

		void* reallocAddr = realloc((*srcArrayStack)->nodeArray, reallocSizeInBytes);
		if (reallocAddr == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		(*srcArrayStack)->nodeArray = (Node*)reallocAddr;
		(*srcArrayStack)->capacity = reallocCapacity;
	}

	StackIndexType index = (*srcArrayStack)->top++; //Push 할 인덱스
	(*srcArrayStack)->nodeArray[index].data = srcData;
}

/// <summary>
/// 대상 배열 스택의 순차적인 최상위 데이터 제거 및 반환
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <returns>대상 배열 스택의 최상위 데이터</returns>
DataType AS_Pop(ArrayStack** srcArrayStack)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (AS_IsEmpty(srcArrayStack)) //비어있으면
		throw std::logic_error(std::string(__func__) + std::string(" : Empty Stack"));

	if ((AS_GetTotalNodeDataCount(srcArrayStack) / (*srcArrayStack)->capacity) <
		CAPACITY_REDUCE_RATIO_THRESHOLD) //사용량에 대한 비율이 기존 할당 크기에 대한 감소가 발생 될 빈 공간 임계 비율 미만일 경우
	{
		StackIndexType reallocCapacity = (*srcArrayStack)->capacity - ceil((*srcArrayStack)->capacity * CAPACITY_REALLOC_RATIO); //재 할당 될 크기
		StackIndexType reallocSizeInBytes = sizeof(Node) * reallocCapacity; //재 할당 될 바이트 단위 크기
		void* reallocAddr = realloc((*srcArrayStack)->nodeArray, reallocSizeInBytes);
		if (reallocAddr == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		(*srcArrayStack)->nodeArray = (Node*)reallocAddr;
		(*srcArrayStack)->capacity = reallocCapacity;
	}

	StackIndexType index = --((*srcArrayStack)->top); //Pop 할 인덱스
	return (*srcArrayStack)->nodeArray[index].data;
}

/// <summary>
/// 대상 배열 스택의 순차적인 최상위 데이터 제거없이 반환 
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <returns>대상 배열 스택의 최상위 데이터</returns>
DataType AS_Peek(ArrayStack** srcArrayStack)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (AS_IsEmpty(srcArrayStack)) //비어있으면
		throw std::logic_error(std::string(__func__) + std::string(" : Empty Stack"));

	StackIndexType index = ((*srcArrayStack)->top) - 1; //Peek 할 인덱스
	return (*srcArrayStack)->nodeArray[index].data;
}

/// <summary>
/// 대상 배열 스택의 전체 노드 데이터 개수 반환
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <returns>대상 배열 스택의 전체 노드 데이터 개수</returns>
StackIndexType AS_GetTotalNodeDataCount(ArrayStack** srcArrayStack)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return (*srcArrayStack)->top;
}

/// <summary>
/// 대상 배열 스택의 공백 여부 반환
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <returns>대상 배열 스택의 공백 여부</returns>
bool AS_IsEmpty(ArrayStack** srcArrayStack)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return ((*srcArrayStack)->top == 0);
}

/// <summary>
/// 대상 배열 스택의 삽입 가능 여부 반환
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <returns>대상 배열 스택의 삽입 가능 여부</returns>
bool AS_IsFull(ArrayStack** srcArrayStack)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return ((*srcArrayStack)->capacity == (*srcArrayStack)->top); //할당 크기와 동일해지는 시점부터 더 이상 삽입 불가능
}