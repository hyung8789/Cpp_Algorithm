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

	(*srcLinkedListQueue)->front = (*srcLinkedListQueue)->rear = NULL;
	(*srcLinkedListQueue)->totalNodeCount = 0;
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

	retVal->data = srcData;
	retVal->next = NULL;

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
		(*srcLinkedListQueue)->front = (*srcLinkedListQueue)->rear = srcNewNode;
	}
	else
	{
		(*srcLinkedListQueue)->rear->next = srcNewNode; //�Ĵ� ����� ������ �� ���� ����
		(*srcLinkedListQueue)->rear = srcNewNode; //�Ĵ� ��带 �� ���� ����
	}

	(*srcLinkedListQueue)->totalNodeCount++;
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

	Node* retVal = (*srcLinkedListQueue)->front;
	(*srcLinkedListQueue)->front = (*srcLinkedListQueue)->front->next;
	(*srcLinkedListQueue)->totalNodeCount--;

	if (LLQ_IsEmpty(srcLinkedListQueue)) //���ſ� ���� ���� ���°� �� ���
		(*srcLinkedListQueue)->rear = NULL; //���� �� ��忡 ���� �������� ����

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

	return (*srcLinkedListQueue)->totalNodeCount;
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

	return ((*srcLinkedListQueue)->totalNodeCount == 0);
}