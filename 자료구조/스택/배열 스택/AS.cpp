#include "AS_Core.h"

/// <summary>
/// 대상 배열 스택에 할당 크기만큼 배열 스택 생성
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <param name="capacity">할당 크기</param>
void AS_CreateStack(ArrayStack** srcArrayStack, StackIndexType capacity) throw(std::invalid_argument, std::runtime_error)
{
	if ((*srcArrayStack) != NULL)
<<<<<<< HEAD
		throw std::runtime_error("Memleak");

	if (capacity <= 0)
		throw std::invalid_argument("Invalid Args");

	(*srcArrayStack) = (ArrayStack*)malloc(sizeof(ArrayStack));
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error("Not enough Heap Memory");

	(*srcArrayStack)->nodeArray = (Node*)malloc(sizeof(Node) * capacity); //할당 크기만큼 생성
	if ((*srcArrayStack)->nodeArray == NULL)
		throw std::runtime_error("Not enough Heap Memory");
=======
		ThrowCriticalException(EX::MEM_LEAK);

	(*srcArrayStack) = (ArrayStack*)malloc(sizeof(ArrayStack));
	if ((*srcArrayStack) == NULL)
		ThrowCriticalException(EX::NOT_ENOUGH_HEAP_MEMORY);

	(*srcArrayStack)->nodeArray = (Node*)malloc(sizeof(Node) * capacity); //할당 크기만큼 생성
	if ((*srcArrayStack)->nodeArray == NULL)
		ThrowCriticalException(EX::NOT_ENOUGH_HEAP_MEMORY);
>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a

	//(*srcArrayStack)->nodeArray = (Node*)realloc((*srcArrayStack)->nodeArray, sizeof(Node) * capacity); //할당 크기만큼 재 할당
	//if ((*srcArrayStack)->nodeArray == NULL)
		//ThrowCriticalException(EX::NOT_ENOUGH_HEAP_MEMORY);

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
void AS_Push(ArrayStack** srcArrayStack, DataType srcData) throw(std::runtime_error)
{
	if ((*srcArrayStack) == NULL)
<<<<<<< HEAD
		throw std::runtime_error("Not initialized");
=======
		ThrowCriticalException(EX::NOT_ASSIGNED_STACK_ACCESS);
>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a

	if (!AS_IsFull(srcArrayStack)) //가득 차지 않았으면
	{
		StackIndexType index = (*srcArrayStack)->top++; //Push 할 인덱스
		(*srcArrayStack)->nodeArray[index].data = srcData;
		return;
	}
	else
	{
		//TODO : PUSH 할 경우 할당량이 임계값 초과 시 할당 된 크기 늘리기 (realloc)
<<<<<<< HEAD
=======
		ThrowCriticalException(EX::TRY_PUSH_ON_FULL_STACK);
>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a
	}
}

/// <summary>
/// 대상 배열 스택의 순차적인 최상위 데이터 제거 및 반환
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <returns>대상 배열 스택의 최상위 데이터</returns>
DataType AS_Pop(ArrayStack** srcArrayStack) throw(std::runtime_error)
{
	if ((*srcArrayStack) == NULL)
<<<<<<< HEAD
		throw std::runtime_error("Not initialized");
=======
		ThrowCriticalException(EX::NOT_ASSIGNED_STACK_ACCESS);
>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a

	if (!AS_IsEmpty(srcArrayStack)) //비어있지 않으면
	{
		StackIndexType index = --((*srcArrayStack)->top); //Pop 할 인덱스
		return (*srcArrayStack)->nodeArray[index].data;
	}
	else
	{
		//TODO : POP 할 경우 할당량이 임계값 이하 일 경우 할당 된 크기 줄이기 (realloc)
<<<<<<< HEAD
=======
		ThrowCriticalException(EX::TRY_POP_ON_EMPTY_STACK);
>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a
	}
}

/// <summary>
/// 대상 배열 스택의 순차적인 최상위 데이터 제거없이 반환 
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <returns>대상 배열 스택의 최상위 데이터</returns>
DataType AS_Peek(ArrayStack** srcArrayStack) throw(std::runtime_error)
{
	if ((*srcArrayStack) == NULL)
<<<<<<< HEAD
		throw std::runtime_error("Not initialized");
=======
		ThrowCriticalException(EX::NOT_ASSIGNED_STACK_ACCESS);
>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a

	if (!AS_IsEmpty(srcArrayStack)) //비어있지 않으면
	{
		StackIndexType index = ((*srcArrayStack)->top) - 1; //Peek 할 인덱스
		return (*srcArrayStack)->nodeArray[index].data;
	}
	else
	{
<<<<<<< HEAD

=======
		ThrowCriticalException(EX::TRY_PEEK_ON_EMPTY_STACK);
>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a
	}
}

/// <summary>
/// 대상 배열 스택의 전체 노드 데이터 개수 반환
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <returns>대상 배열 스택의 전체 노드 데이터 개수</returns>
StackIndexType AS_GetTotalNodeDataCount(ArrayStack** srcArrayStack) throw(std::runtime_error)
{
	if ((*srcArrayStack) == NULL)
<<<<<<< HEAD
		throw std::runtime_error("Not initialized");
=======
		ThrowCriticalException(EX::NOT_ASSIGNED_STACK_ACCESS);
>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a

	return (*srcArrayStack)->top;
}

/// <summary>
/// 대상 배열 스택의 공백 여부 반환
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <returns>대상 배열 스택의 공백 여부</returns>
bool AS_IsEmpty(ArrayStack** srcArrayStack) throw(std::runtime_error)
{
	if ((*srcArrayStack) == NULL)
<<<<<<< HEAD
		throw std::runtime_error("Not initialized");
=======
		ThrowCriticalException(EX::NOT_ASSIGNED_STACK_ACCESS);
>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a

	return ((*srcArrayStack)->top == 0);
}

/// <summary>
/// 대상 배열 스택의 삽입 가능 여부 반환
/// </summary>
/// <param name="srcArrayStack">대상 배열 스택</param>
/// <returns>대상 배열 스택의 삽입 가능 여부</returns>
bool AS_IsFull(ArrayStack** srcArrayStack) throw(std::runtime_error)
{
	if ((*srcArrayStack) == NULL)
<<<<<<< HEAD
		throw std::runtime_error("Not initialized");
=======
		ThrowCriticalException(EX::NOT_ASSIGNED_STACK_ACCESS);
>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a

	return ((*srcArrayStack)->capacity == (*srcArrayStack)->top); //할당 크기와 동일해지는 시점부터 더 이상 삽입 불가능
}