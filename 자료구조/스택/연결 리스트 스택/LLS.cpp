#include "LLS_Core.h"

/// <summary>
/// ��� ���� ����Ʈ ���� ����
/// </summary>
/// <param name="srcLinkedListStack">��� ���� ����Ʈ ����</param>
void LLS_CreateStack(LinkedListStack** srcLinkedListStack)
{
	if ((*srcLinkedListStack) != NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Memleak"));

	(*srcLinkedListStack) = (LinkedListStack*)malloc(sizeof(LinkedListStack));
	if ((*srcLinkedListStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	(*srcLinkedListStack)->_head = (*srcLinkedListStack)->_top = NULL;
	(*srcLinkedListStack)->_totalNodeCount = 0;
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

		free(*srcLinkedListStack);
		(*srcLinkedListStack) = NULL;
	}
}

/// <summary>
/// ���ο� ��� ���� �� ���� �� ��� ��ȯ
/// </summary>
/// <param name="srcData">����� ������</param>
/// <returns>���� �� ���</returns>
Node* LLS_CreateNode(const char* srcData)
{
	if (srcData == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	Node* retVal = (Node*)malloc(sizeof(Node));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_data = (char*)malloc(strlen(srcData) + 1); //'\0' ���� ũ��
	if (retVal->_data == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	if (strcpy_s(retVal->_data, strlen(srcData) + 1, srcData) != 0)
		throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

	retVal->_next = NULL;

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
		if ((*srcNode)->_data != NULL)
		{
			free((*srcNode)->_data);
			(*srcNode)->_data = NULL;
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
	if ((*srcLinkedListStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcLinkedListStack)->_head == NULL)
	{
		(*srcLinkedListStack)->_head = (*srcLinkedListStack)->_top = srcNewNode;
	}
	else //�ֻ��� ��� ������ �� ��� ���� �� �ֻ��� ��� ����
	{
		(*srcLinkedListStack)->_top->_next = srcNewNode;
		(*srcLinkedListStack)->_top = srcNewNode; //�ֻ��� ��� ����
	}

	(*srcLinkedListStack)->_totalNodeCount++; //��ü ��� �� ����
}

/// <summary>
/// ��� ���� ����Ʈ ������ �������� �ֻ��� ������ ���� �� ��ȯ
/// </summary>
/// <param name="srcLinkedListStack">��� ���� ����Ʈ ����</param>
/// <returns>��� ���� ����Ʈ ������ �ֻ��� ������</returns>
Node* LLS_Pop(LinkedListStack** srcLinkedListStack)
{
	Node* retVal = NULL;

	if ((*srcLinkedListStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (LLS_IsEmpty(srcLinkedListStack)) //���������
		throw std::logic_error(std::string(__func__) + std::string(" : Empty Stack"));

	retVal = (*srcLinkedListStack)->_top;
	(*srcLinkedListStack)->_totalNodeCount--; //��ü ��� �� ����

	if (retVal == (*srcLinkedListStack)->_head) //�ֻ��� ��尡 ��� ����� ���
	{
		(*srcLinkedListStack)->_top = (*srcLinkedListStack)->_head = NULL;
	}
	else //���� ���� ����Ʈ�̹Ƿ�, ���������� �ֻ��� ����� ������ Ž��
	{
		Node* current = (*srcLinkedListStack)->_head;

		while (current != NULL && current->_next != retVal)
		{
			current = current->_next;
		}

		if (current == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Unknown Err"));

		(*srcLinkedListStack)->_top = current; //�ֻ��� ��带 �ֻ��� ����� ���� ���� ����
		current->_next = NULL;
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
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (LLS_IsEmpty(srcLinkedListStack)) //���������
		throw std::logic_error(std::string(__func__) + std::string(" : Empty Stack"));

	retVal = (*srcLinkedListStack)->_top;

	return retVal;
}

/// <summary>
/// ��� ���� ����Ʈ ������ ��ü ��� ���� ��ȯ
/// </summary>
/// <param name="srcLinkedListStack">��� ���� ����Ʈ ������ ��ü ��� ����</param>
/// <returns></returns>
StackIndexType LLS_GetTotalNodeCount(LinkedListStack** srcLinkedListStack)
{
	if ((*srcLinkedListStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return (*srcLinkedListStack)->_totalNodeCount;
}

/// <summary>
/// ��� ���� ����Ʈ ������ ���� ���� ��ȯ
/// </summary>
/// <param name="srcLinkedListStack">��� ���� ����Ʈ ����</param>
/// <returns>��� ���� ����Ʈ ������ ���� ����</returns>
bool LLS_IsEmpty(LinkedListStack** srcLinkedListStack)
{
	if ((*srcLinkedListStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return ((*srcLinkedListStack)->_totalNodeCount == 0);
}