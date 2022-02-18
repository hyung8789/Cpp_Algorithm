#include "CQ_Core.h"

/// <summary>
/// ��� ť�� �Ҵ� ũ�⸸ŭ ť ����
/// </summary>
/// <param name="srcCircularQueue">��� ť</param>
/// <param name="capacity">�Ҵ� ũ��</param>
void CQ_CreateQueue(CircularQueue** srcCircularQueue, QueueIndexType capacity)
{
	if ((*srcCircularQueue) != NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Memleak"));

	if (capacity <= 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	(*srcCircularQueue) = (CircularQueue*)malloc(sizeof(CircularQueue));
	if ((*srcCircularQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	(*srcCircularQueue)->_nodeArray = (Node*)malloc(sizeof(Node) * (capacity + 1)); //�Ҵ� ũ�⸸ŭ ���� ���� �Ҵ� ũ�� + 1�� �Ҵ�
	if ((*srcCircularQueue)->_nodeArray == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	(*srcCircularQueue)->_front = (*srcCircularQueue)->_rear = 0;
	(*srcCircularQueue)->_capacity = capacity + 1; //�Ҵ� ũ�⸸ŭ ���� ���� �Ҵ� ũ�� + 1�� �Ҵ�
}

/// <summary>
/// ��� ť�� �Ҵ� �� �޸� ����
/// </summary>
/// <param name="srcCircularQueue">��� ť</param>
void CQ_DeallocateQueue(CircularQueue** srcCircularQueue)
{
	if ((*srcCircularQueue) != NULL)
	{
		free((*srcCircularQueue)->_nodeArray);
		(*srcCircularQueue)->_nodeArray = NULL;

		free(*srcCircularQueue);
		(*srcCircularQueue) = NULL;
	}
}

/// <summary>
/// ��� ť�� �������� ������ ����
/// </summary>
/// <param name="srcCircularQueue">��� ť</param>
/// <param name="srcData">���� �� ������</param>
void CQ_Enqueue(CircularQueue** srcCircularQueue, DataType srcData)
{
	if ((*srcCircularQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (CQ_IsFull(srcCircularQueue)) //��ȭ ������ ���
		throw std::logic_error(std::string(__func__) + std::string(" : Queue is Full"));

	(*srcCircularQueue)->_rear = ((*srcCircularQueue)->_rear + 1) % (*srcCircularQueue)->_capacity;
	(*srcCircularQueue)->_nodeArray[(*srcCircularQueue)->_rear]._data = srcData;
}

/// <summary>
/// ��� ť�� �������� ������ ���� �� ��ȯ
/// </summary>
/// <param name="srcCircularQueue">��� ť</param>
/// <returns>��� ť�� �������� ������</returns>
DataType CQ_Dequeue(CircularQueue** srcCircularQueue)
{
	if ((*srcCircularQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (CQ_IsEmpty(srcCircularQueue)) //���� ������ ���
		throw std::logic_error(std::string(__func__) + std::string(" : Queue is Empty"));

	(*srcCircularQueue)->_front = ((*srcCircularQueue)->_front + 1) % (*srcCircularQueue)->_capacity;
	return (*srcCircularQueue)->_nodeArray[(*srcCircularQueue)->_front]._data;
}

/// <summary>
/// ��� ť�� ��ü ��� ������ ���� ��ȯ
/// </summary>
/// <param name="srcCircularQueue">��� ť</param>
/// <returns>��� ť�� ��ü ��� ������ ����</returns>
QueueIndexType CQ_GetTotalNodeDataCount(CircularQueue** srcCircularQueue)
{
	if ((*srcCircularQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return ((*srcCircularQueue)->_rear - (*srcCircularQueue)->_front);
}

/// <summary>
/// ��� ť�� ���� ���� ��ȯ
/// </summary>
/// <param name="srcCircularQueue">��� ť</param>
/// <returns>��� ť�� ���� ����</returns>
bool CQ_IsEmpty(CircularQueue** srcCircularQueue)
{
	if ((*srcCircularQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return ((*srcCircularQueue)->_front == (*srcCircularQueue)->_rear); //���ܰ� �Ĵ��� ������ ������ ���� ����
}

/// <summary>
/// ��� ť�� ��ȭ ���� ��ȯ
/// </summary>
/// <param name="srcCircularQueue">��� ť</param>
/// <returns>��� ť�� ��ȭ ����</returns>
bool CQ_IsFull(CircularQueue** srcCircularQueue)
{
	if ((*srcCircularQueue) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return ((*srcCircularQueue)->_rear + 1) % (*srcCircularQueue)->_capacity == (*srcCircularQueue)->_front; //������ ���� �� �Ĵ��� ��ġ���� ������ ������ ��ȭ ����
}
