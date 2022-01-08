#include "LLS_Core.h"

using namespace exceptionhandler;

/// <summary>
/// 대상 연결 리스트 스택 생성
/// </summary>
/// <param name="srcLinkedListStack">대상 연결 리스트 스택</param>
void LLS_CreateStack(LinkedListStack** srcLinkedListStack)
{
	if ((*srcLinkedListStack) == NULL)
	{
		(*srcLinkedListStack) = (LinkedListStack*)malloc(sizeof(LinkedListStack));
		if ((*srcLinkedListStack) == NULL)
			ThrowException(EX::NOT_ENOUGH_HEAP_MEMORY, true);

		(*srcLinkedListStack)->head = (*srcLinkedListStack)->top = NULL; \
			(*srcLinkedListStack)->totalNodeCount = 0;
	}
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

	if ((*srcData) != NULL)
	{
		retVal = (Node*)malloc(sizeof(Node));
		if (retVal == NULL)
			ThrowException(EX::NOT_ENOUGH_HEAP_MEMORY, true);

		retVal->data = (char*)malloc(sizeof(srcData) + 1); //'\0' 포함 크기
		if (retVal->data == NULL)
			ThrowException(EX::NOT_ENOUGH_HEAP_MEMORY, true);

		if (strcpy_s(retVal->data, sizeof(retVal->data), srcData) != 0)
			ThrowException(EX::FAILED_TO_STRCPY, true);

		retVal->next = NULL;
	}

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
	if ((*srcLinkedListStack) != NULL)
	{
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
	else
	{
		ThrowException(EX::NOT_ASSIGNED_STACK_ACCESS, true);
	}
}

/// <summary>
/// 대상 연결 리스트 스택의 순차적인 최상위 데이터 제거 및 반환
/// </summary>
/// <param name="srcLinkedListStack">대상 연결 리스트 스택</param>
/// <returns>대상 연결 리스트 스택의 최상위 데이터</returns>
Node* LLS_Pop(LinkedListStack** srcLinkedListStack)
{
	Node* retVal = NULL;

	if ((*srcLinkedListStack) != NULL)
	{
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

				if (current != NULL)
				{
					(*srcLinkedListStack)->top = current; //최상위 노드를 최상위 노드의 이전 노드로 변경
					current->next = NULL;
				}
				else
				{
					ThrowException(EX::UNKNOWN_ERR, true);
				}
			}
		}
		else
		{
			ThrowException(EX::TRY_POP_ON_EMPTY_STACK, true);
		}
	}
	else
	{
		ThrowException(EX::NOT_ASSIGNED_STACK_ACCESS, true);
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
	if ((*srcLinkedListStack) != NULL)
	{
		if (!LLS_IsEmpty(srcLinkedListStack))
		{
			return (*srcLinkedListStack)->top;
		}
		else
		{
			ThrowException(EX::TRY_PEEK_ON_EMPTY_STACK, true);
		}
	}
	else
	{
		ThrowException(EX::NOT_ASSIGNED_STACK_ACCESS, true);
	}
}

/// <summary>
/// 대상 연결 리스트 스택의 전체 노드 개수 반환
/// </summary>
/// <param name="srcLinkedListStack">대상 연결 리스트 스택의 전체 노드 개수</param>
/// <returns></returns>
StackIndexType LLS_GetTotalNodeCount(LinkedListStack** srcLinkedListStack)
{
	if ((*srcLinkedListStack) != NULL)
	{
		return (*srcLinkedListStack)->totalNodeCount;
	}
	else
	{
		ThrowException(EX::NOT_ASSIGNED_STACK_ACCESS, true);
	}
}

/// <summary>
/// 대상 연결 리스트 스택의 공백 여부 반환
/// </summary>
/// <param name="srcLinkedListStack">대상 연결 리스트 스택</param>
/// <returns>대상 연결 리스트 스택의 공백 여부</returns>
bool LLS_IsEmpty(LinkedListStack** srcLinkedListStack)
{
	if ((*srcLinkedListStack) != NULL)
	{
		return ((*srcLinkedListStack)->totalNodeCount == 0);
	}
	else
	{
		ThrowException(EX::NOT_ASSIGNED_STACK_ACCESS, true);
	}
}