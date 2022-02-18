#include "LLQ_Core.h"

/// <summary>
/// ��� ť�� ť ����
/// </summary>
/// <param name="srcLinkedListQueue">��� ť</param>
void LLQ_CreateQueue(LinkedListQueue** srcLinkedListQueue)
{
	if ((*srcLinkedListQueue) != NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Memleak"));

	(*srcLinkedListQueue) = (LinkedListQueue*)malloc(sizeof(LinkedListQueue));
	if ((*srcLinkedListQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	(*srcLinkedListQueue)->_front = (*srcLinkedListQueue)->_rear = NULL;
	(*srcLinkedListQueue)->_totalNodeCount = 0;
}

/// <summary>
/// ��� ť�� �Ҵ� �� �޸� ����
/// </summary>
/// <param name="srcLinkedListQueue">��� ť</param>
void LLQ_DeallocateQueue(LinkedListQueue** srcLinkedListQueue)
{
	if ((*srcLinkedListQueue) != NULL)
	{
		while (!LLQ_IsEmpty(srcLinkedListQueue)) //��� ť�� �Ҵ� �� ��ü ��忡 ����
		{
			Node* tmp = LLQ_Dequeue(srcLinkedListQueue);
			LLQ_DeallocateNode(&tmp);
		}

		free(*srcLinkedListQueue);
		(*srcLinkedListQueue) = NULL;
	}
}

/// <summary>
/// ���ο� ��� ���� �� ���� �� ��� ��ȯ
/// </summary>
/// <param name="srcData">����� ������</param>
/// <returns>���� �� ���</returns>
Node* LLQ_CreateNode(DataType srcData)
{
	Node* retVal = (Node*)malloc(sizeof(Node));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_data = srcData;
	retVal->_next = NULL;

	return retVal;
}

/// <summary>
/// ��� ��忡 �Ҵ� �� �޸� ����
/// </summary>
/// <param name="srcNode">��� ���</param>
void LLQ_DeallocateNode(Node** srcNode)
{
	if ((*srcNode) != NULL)
	{
		free(*srcNode);
		(*srcNode) = NULL;
	}
}

/// <summary>
/// ��� ť�� �������� ��� ����
/// </summary>
/// <param name="srcLinkedListQueue">��� ť</param>
/// <param name="srcNewNode">���� �� ���</param>
void LLQ_Enqueue(LinkedListQueue** srcLinkedListQueue, Node* srcNewNode)
{
	if ((*srcLinkedListQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (LLQ_IsEmpty(srcLinkedListQueue)) //ť�� ���������
	{
		(*srcLinkedListQueue)->_front = (*srcLinkedListQueue)->_rear = srcNewNode;
	}
	else
	{
		(*srcLinkedListQueue)->_rear->_next = srcNewNode; //�Ĵ� ����� ������ �� ���� ����
		(*srcLinkedListQueue)->_rear = srcNewNode; //�Ĵ� ��带 �� ���� ����
	}

	(*srcLinkedListQueue)->_totalNodeCount++;
}

/// <summary>
/// ��� ť�� �������� ��� ���� �� ��ȯ
/// </summary>
/// <param name="srcLinkedListQueue">��� ť</param>
/// <returns>��� ť�� �������� ���</returns>
Node* LLQ_Dequeue(LinkedListQueue** srcLinkedListQueue)
{
	if ((*srcLinkedListQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (LLQ_IsEmpty(srcLinkedListQueue)) //���� ������ ���
		throw std::logic_error(std::string(__func__) + std::string(" : Queue is Empty"));

	Node* retVal = (*srcLinkedListQueue)->_front;
	(*srcLinkedListQueue)->_front = (*srcLinkedListQueue)->_front->_next;
	(*srcLinkedListQueue)->_totalNodeCount--;

	if (LLQ_IsEmpty(srcLinkedListQueue)) //���ſ� ���� ���� ���°� �� ���
		(*srcLinkedListQueue)->_rear = NULL; //���� �� ��忡 ���� �������� ����

	return retVal;
}

/// <summary>
/// ��� ť�� ��ü ��� ���� ��ȯ
/// </summary>
/// <param name="srcCircularQueue">��� ť</param>
/// <returns>��� ť�� ��ü ��� ����</returns>
QueueIndexType LLQ_GetTotalNodeCount(LinkedListQueue** srcLinkedListQueue)
{
	if ((*srcLinkedListQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return (*srcLinkedListQueue)->_totalNodeCount;
}

/// <summary>
/// ��� ť�� ���� ���� ��ȯ
/// </summary>
/// <param name="srcLinkedListQueue">��� ť</param>
/// <returns>��� ť�� ���� ����</returns>
bool LLQ_IsEmpty(LinkedListQueue** srcLinkedListQueue)
{
	if ((*srcLinkedListQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return ((*srcLinkedListQueue)->_totalNodeCount == 0);
}