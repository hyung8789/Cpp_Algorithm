#include "LLS_Core.h"

using namespace exceptionhandler;

/// <summary>
/// ��� ���� ����Ʈ ���� ����
/// </summary>
/// <param name="srcLinkedListStack">��� ���� ����Ʈ ����</param>
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
/// ��� ���� ����Ʈ�� �Ҵ� �� �޸� ����
/// </summary>
/// <param name="srcLinkedListStack">��� ���� ����Ʈ</param>
void LLS_DeallocateLinkedListStack(LinkedListStack** srcLinkedListStack)
{
	if ((*srcLinkedListStack) != NULL)
	{
		while (!LLS_IsEmpty(srcLinkedListStack)) //��� ���� ����Ʈ�� �Ҵ� �� ��ü ��忡 ����
		{
			Node* tmp = LLS_Pop(srcLinkedListStack);
			LLS_DeallocateNode(&tmp);
		}
	}

	free(*srcLinkedListStack);
	(*srcLinkedListStack) = NULL;
}

/// <summary>
/// ���ο� ��� ���� �� ���� �� ��� ��ȯ
/// </summary>
/// <param name="srcData">����� ������</param>
/// <returns>���� �� ���</returns>
Node* LLS_CreateNode(const char* srcData)
{
	Node* retVal = NULL;

	if ((*srcData) != NULL)
	{
		retVal = (Node*)malloc(sizeof(Node));
		if (retVal == NULL)
			ThrowException(EX::NOT_ENOUGH_HEAP_MEMORY, true);

		retVal->data = (char*)malloc(sizeof(srcData) + 1); //'\0' ���� ũ��
		if (retVal->data == NULL)
			ThrowException(EX::NOT_ENOUGH_HEAP_MEMORY, true);

		if (strcpy_s(retVal->data, sizeof(retVal->data), srcData) != 0)
			ThrowException(EX::FAILED_TO_STRCPY, true);

		retVal->next = NULL;
	}

	return retVal;
}

/// <summary>
/// ��� ��忡 �Ҵ� �� �޸� ����
/// </summary>
/// <param name="srcNode">��� ���</param>
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
/// ��� ���� ����Ʈ ������ �������� �ֻ��� ��� ����
/// </summary>
/// <param name="srcLinkedListStack">��� ���� ����Ʈ ����</param>
/// <param name="srcNewNode">���� �� ���</param>
void LLS_Push(LinkedListStack** srcLinkedListStack, Node* srcNewNode)
{
	if ((*srcLinkedListStack) != NULL)
	{
		if ((*srcLinkedListStack)->head == NULL)
		{
			(*srcLinkedListStack)->head = (*srcLinkedListStack)->top = srcNewNode;
		}
		else //�ֻ��� ��� ������ �� ��� ���� �� �ֻ��� ��� ����
		{
			(*srcLinkedListStack)->top->next = srcNewNode;
			(*srcLinkedListStack)->top = srcNewNode; //�ֻ��� ��� ����
		}

		(*srcLinkedListStack)->totalNodeCount++; //��ü ��� �� ����
	}
	else
	{
		ThrowException(EX::NOT_ASSIGNED_STACK_ACCESS, true);
	}
}

/// <summary>
/// ��� ���� ����Ʈ ������ �������� �ֻ��� ������ ���� �� ��ȯ
/// </summary>
/// <param name="srcLinkedListStack">��� ���� ����Ʈ ����</param>
/// <returns>��� ���� ����Ʈ ������ �ֻ��� ������</returns>
Node* LLS_Pop(LinkedListStack** srcLinkedListStack)
{
	Node* retVal = NULL;

	if ((*srcLinkedListStack) != NULL)
	{
		if (!LLS_IsEmpty(srcLinkedListStack))
		{
			retVal = (*srcLinkedListStack)->top;
			(*srcLinkedListStack)->totalNodeCount--; //��ü ��� �� ����

			if (retVal == (*srcLinkedListStack)->head) //�ֻ��� ��尡 ��� ����� ���
			{
				(*srcLinkedListStack)->top = (*srcLinkedListStack)->head = NULL;
			}
			else //���� ���� ����Ʈ�̹Ƿ�, ���������� �ֻ��� ����� ������ Ž��
			{
				Node* current = (*srcLinkedListStack)->head;

				while (current != NULL && current->next != retVal)
				{
					current = current->next;
				}

				if (current != NULL)
				{
					(*srcLinkedListStack)->top = current; //�ֻ��� ��带 �ֻ��� ����� ���� ���� ����
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
/// ��� ���� ����Ʈ ������ �������� �ֻ��� ������ ���ž��� ��ȯ 
/// </summary>
/// <param name="srcLinkedListStack">��� ���� ����Ʈ ����</param>
/// <returns>��� ���� ����Ʈ ������ �ֻ��� ������</returns>
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
/// ��� ���� ����Ʈ ������ ��ü ��� ���� ��ȯ
/// </summary>
/// <param name="srcLinkedListStack">��� ���� ����Ʈ ������ ��ü ��� ����</param>
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
/// ��� ���� ����Ʈ ������ ���� ���� ��ȯ
/// </summary>
/// <param name="srcLinkedListStack">��� ���� ����Ʈ ����</param>
/// <returns>��� ���� ����Ʈ ������ ���� ����</returns>
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