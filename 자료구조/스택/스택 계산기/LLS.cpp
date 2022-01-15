#include "Core.h"

/// <summary>
/// 대상 연결 리스트 스택 생성
/// </summary>
/// <param name="srcLinkedListStack">대상 연결 리스트 스택</param>
void LLS_CreateStack(LinkedListStack** srcLinkedListStack)
{
	if ((*srcLinkedListStack) != NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Memleak"));

	(*srcLinkedListStack) = (LinkedListStack*)malloc(sizeof(LinkedListStack));
	if ((*srcLinkedListStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	(*srcLinkedListStack)->head = (*srcLinkedListStack)->top = NULL;
	(*srcLinkedListStack)->totalNodeCount = 0;
}

/// <summary>
/// 대상 연결 리스트에 할당 된 메모리 해제
/// </summary>
/// <param name="srcLinkedListStack">대상 연결 리스트</param>
void LLS_DeallocateLinkedListStack(LinkedListStack** srcLinkedListStack)
{
	if ((*srcLinkedListStack) != NULL)
	{
		while (!LLS_IsEmpty(srcLinkedListStack)) //대상 연결 리스트에 할당 된 전체 노드에 대해
		{
			Node* tmp = LLS_Pop(srcLinkedListStack);
			LLS_DeallocateNode(&tmp);
		}
	}

	free(*srcLinkedListStack);
	(*srcLinkedListStack) = NULL;
}

/// <summary>
/// 새로운 노드 생성 및 생성 된 노드 반환
/// </summary>
/// <param name="srcData">노드의 데이터</param>
/// <returns>생성 된 노드</returns>
Node* LLS_CreateNode(const char* srcData)
{
	Node* retVal = NULL;
	
	if (srcData == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	retVal = (Node*)malloc(sizeof(Node));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->data = (char*)malloc(strlen(srcData) + 1); //'\0' 포함 크기
	if (retVal->data == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	if (strcpy_s(retVal->data, strlen(srcData) + 1, srcData) != 0)
		throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

	retVal->next = NULL;

	return retVal;
}

/// <summary>
/// 대상 노드에 할당 된 메모리 해제
/// </summary>
/// <param name="srcNode">대상 노드</param>
void LLS_DeallocateNode(Node** srcNode)
{
	if ((*srcNode) != NULL)
	{
		if ((*srcNode)->data != NULL)
		{
			free((*srcNode)->data);
			(*srcNode)->data = NULL;
		}

		free(*srcNode);
		(*srcNode) = NULL;
	}
}

/// <summary>
/// 대상 연결 리스트 스택의 순차적인 최상위 노드 삽입
/// </summary>
/// <param name="srcLinkedListStack">대상 연결 리스트 스택</param>
/// <param name="srcNewNode">삽입 할 노드</param>
void LLS_Push(LinkedListStack** srcLinkedListStack, Node* srcNewNode)
{
	if ((*srcLinkedListStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcLinkedListStack)->head == NULL)
	{
		(*srcLinkedListStack)->head = (*srcLinkedListStack)->top = srcNewNode;
	}
	else //최상위 노드 다음에 새 노드 연결 및 최상위 노드 변경
	{
		(*srcLinkedListStack)->top->next = srcNewNode;
		(*srcLinkedListStack)->top = srcNewNode; //최상위 노드 변경
	}

	(*srcLinkedListStack)->totalNodeCount++; //전체 노드 수 증가

}

/// <summary>
/// 대상 연결 리스트 스택의 순차적인 최상위 데이터 제거 및 반환
/// </summary>
/// <param name="srcLinkedListStack">대상 연결 리스트 스택</param>
/// <returns>대상 연결 리스트 스택의 최상위 데이터</returns>
Node* LLS_Pop(LinkedListStack** srcLinkedListStack)
{
	Node* retVal = NULL;

	if ((*srcLinkedListStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (!LLS_IsEmpty(srcLinkedListStack))
	{
		retVal = (*srcLinkedListStack)->top;
		(*srcLinkedListStack)->totalNodeCount--; //전체 노드 수 감소

		if (retVal == (*srcLinkedListStack)->head) //최상위 노드가 헤드 노드일 경우
		{
			(*srcLinkedListStack)->top = (*srcLinkedListStack)->head = NULL;
		}
		else //단일 연결 리스트이므로, 순차적으로 최상위 노드의 이전을 탐색
		{
			Node* current = (*srcLinkedListStack)->head;

			while (current != NULL && current->next != retVal)
			{
				current = current->next;
			}

			if (current == NULL)
				throw std::runtime_error(std::string(__func__) + std::string(" : Unknown Err"));

			(*srcLinkedListStack)->top = current; //최상위 노드를 최상위 노드의 이전 노드로 변경
			current->next = NULL;
		}
	}

	return retVal;
}

/// <summary>
/// 대상 연결 리스트 스택의 순차적인 최상위 데이터 제거없이 반환 
/// </summary>
/// <param name="srcLinkedListStack">대상 연결 리스트 스택</param>
/// <returns>대상 연결 리스트 스택의 최상위 데이터</returns>
Node* LLS_Peek(LinkedListStack** srcLinkedListStack)
{
	Node* retVal = NULL;

	if ((*srcLinkedListStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (!LLS_IsEmpty(srcLinkedListStack))
	{
		retVal = (*srcLinkedListStack)->top;
	}

	return retVal;
}

/// <summary>
/// 대상 연결 리스트 스택의 전체 노드 개수 반환
/// </summary>
/// <param name="srcLinkedListStack">대상 연결 리스트 스택의 전체 노드 개수</param>
/// <returns></returns>
StackIndexType LLS_GetTotalNodeCount(LinkedListStack** srcLinkedListStack)
{
	if ((*srcLinkedListStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return (*srcLinkedListStack)->totalNodeCount;
}

/// <summary>
/// 대상 연결 리스트 스택의 피연산자 간 구분을 위한 공백 여부 반환
/// </summary>
/// <param name="srcLinkedListStack">대상 연결 리스트 스택</param>
/// <returns>대상 연결 리스트 스택의 피연산자 간 구분을 위한 공백 여부</returns>
bool LLS_IsEmpty(LinkedListStack** srcLinkedListStack)
{
	if ((*srcLinkedListStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return ((*srcLinkedListStack)->totalNodeCount == 0);
}