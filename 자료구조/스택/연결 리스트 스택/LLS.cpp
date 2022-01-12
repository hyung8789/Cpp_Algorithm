#include "LLS_Core.h"

/// <summary>
/// ��� ���� ����Ʈ ���� ����
/// </summary>
/// <param name="srcLinkedListStack">��� ���� ����Ʈ ����</param>
void LLS_CreateStack(LinkedListStack** srcLinkedListStack) throw(std::runtime_error)
{
	if ((*srcLinkedListStack) != NULL)
<<<<<<< HEAD
		throw std::runtime_error("Memleak");

	(*srcLinkedListStack) = (LinkedListStack*)malloc(sizeof(LinkedListStack));
	if ((*srcLinkedListStack) == NULL)
		throw std::runtime_error("Not enough Heap Memory");
=======
		ThrowCriticalException(EX::MEM_LEAK);

	(*srcLinkedListStack) = (LinkedListStack*)malloc(sizeof(LinkedListStack));
	if ((*srcLinkedListStack) == NULL)
		ThrowCriticalException(EX::NOT_ENOUGH_HEAP_MEMORY);
>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a

	(*srcLinkedListStack)->head = (*srcLinkedListStack)->top = NULL;
	(*srcLinkedListStack)->totalNodeCount = 0;
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
Node* LLS_CreateNode(const char* srcData) throw(std::invalid_argument, std::runtime_error)
{
	Node* retVal = NULL;

<<<<<<< HEAD
	if (srcData == NULL)
		throw std::invalid_argument("Invalid Args");

	retVal = (Node*)malloc(sizeof(Node));
	if (retVal == NULL)
		throw std::runtime_error("Not enough Heap Memory");

	retVal->data = (char*)malloc(strlen(srcData) + 1); //'\0' ���� ũ��
	if (retVal->data == NULL)
		throw std::runtime_error("Not enough Heap Memory");

	if (strcpy_s(retVal->data, strlen(srcData) + 1, srcData) != 0)
		throw std::runtime_error("dst is not null or wrong size");
=======
	if ((*srcData) == NULL)
		ThrowCriticalException(EX::INVALID_DATA);

	retVal = (Node*)malloc(sizeof(Node));
	if (retVal == NULL)
		ThrowCriticalException(EX::NOT_ENOUGH_HEAP_MEMORY);

	retVal->data = (char*)malloc(sizeof(srcData) + 1); //'\0' ���� ũ��
	if (retVal->data == NULL)
		ThrowCriticalException(EX::NOT_ENOUGH_HEAP_MEMORY);

	if (strcpy_s(retVal->data, sizeof(retVal->data), srcData) != 0)
		ThrowCriticalException(EX::FAILED_TO_STRCPY);
>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a

	retVal->next = NULL;

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
void LLS_Push(LinkedListStack** srcLinkedListStack, Node* srcNewNode) throw(std::invalid_argument, std::runtime_error)
{
	if ((*srcLinkedListStack) == NULL)
<<<<<<< HEAD
		throw std::runtime_error("Not initialized");

	if (srcNewNode == NULL)
		throw std::invalid_argument("Invalid Args");

=======
		ThrowCriticalException(EX::NOT_ASSIGNED_STACK_ACCESS);

>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a
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

/// <summary>
/// ��� ���� ����Ʈ ������ �������� �ֻ��� ������ ���� �� ��ȯ
/// </summary>
/// <param name="srcLinkedListStack">��� ���� ����Ʈ ����</param>
/// <returns>��� ���� ����Ʈ ������ �ֻ��� ������</returns>
Node* LLS_Pop(LinkedListStack** srcLinkedListStack) throw(std::runtime_error)
{
	Node* retVal = NULL;

	if ((*srcLinkedListStack) == NULL)
<<<<<<< HEAD
		throw std::runtime_error("Not initialized");
=======
		ThrowCriticalException(EX::NOT_ASSIGNED_STACK_ACCESS);
>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a

	if (!LLS_IsEmpty(srcLinkedListStack))
	{
		retVal = (*srcLinkedListStack)->top;
		(*srcLinkedListStack)->totalNodeCount--; //��ü ��� �� ����

		if (retVal == (*srcLinkedListStack)->head) //�ֻ��� ��尡 ��� ����� ���
<<<<<<< HEAD
		{
			(*srcLinkedListStack)->top = (*srcLinkedListStack)->head = NULL;
		}
		else //���� ���� ����Ʈ�̹Ƿ�, ���������� �ֻ��� ����� ������ Ž��
		{
=======
		{
			(*srcLinkedListStack)->top = (*srcLinkedListStack)->head = NULL;
		}
		else //���� ���� ����Ʈ�̹Ƿ�, ���������� �ֻ��� ����� ������ Ž��
		{
>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a
			Node* current = (*srcLinkedListStack)->head;

			while (current != NULL && current->next != retVal)
			{
				current = current->next;
			}

			if (current == NULL)
<<<<<<< HEAD
				throw std::runtime_error("Unknown Err");
=======
				ThrowCriticalException(EX::UNKNOWN_ERR);
>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a

			(*srcLinkedListStack)->top = current; //�ֻ��� ��带 �ֻ��� ����� ���� ���� ����
			current->next = NULL;
		}
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
	Node* retVal = NULL;

	if ((*srcLinkedListStack) == NULL)
<<<<<<< HEAD
		throw std::runtime_error("Not initialized");
=======
		ThrowCriticalException(EX::NOT_ASSIGNED_STACK_ACCESS);
>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a

	if (!LLS_IsEmpty(srcLinkedListStack))
	{
		retVal = (*srcLinkedListStack)->top;
	}

	return retVal;
}

/// <summary>
/// ��� ���� ����Ʈ ������ ��ü ��� ���� ��ȯ
/// </summary>
/// <param name="srcLinkedListStack">��� ���� ����Ʈ ������ ��ü ��� ����</param>
/// <returns></returns>
StackIndexType LLS_GetTotalNodeCount(LinkedListStack** srcLinkedListStack) throw(std::runtime_error)
{
	if ((*srcLinkedListStack) == NULL)
<<<<<<< HEAD
		throw std::runtime_error("Not initialized");
=======
		ThrowCriticalException(EX::NOT_ASSIGNED_STACK_ACCESS);
>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a

	return (*srcLinkedListStack)->totalNodeCount;
}

/// <summary>
/// ��� ���� ����Ʈ ������ ���� ���� ��ȯ
/// </summary>
/// <param name="srcLinkedListStack">��� ���� ����Ʈ ����</param>
/// <returns>��� ���� ����Ʈ ������ ���� ����</returns>
bool LLS_IsEmpty(LinkedListStack** srcLinkedListStack) throw(std::runtime_error)
{
	if ((*srcLinkedListStack) == NULL)
<<<<<<< HEAD
		throw std::runtime_error("Not initialized");
=======
		ThrowCriticalException(EX::NOT_ASSIGNED_STACK_ACCESS);
>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a

	return ((*srcLinkedListStack)->totalNodeCount == 0);
}