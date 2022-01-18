#ifndef _CQ_H_
#define _CQ_H_

typedef int DataType; //����� ������ Ÿ��
typedef int QueueIndexType; //ť �ε��� Ÿ��

typedef struct NodeType
{
	DataType data; //����� ������
}Node;

typedef struct CircularQueueType
{
	QueueIndexType capacity; //�Ҵ� ũ��
	QueueIndexType front, rear; //����, �Ĵ� �ε���
	Node* nodeArray; //��� �迭
}CircularQueue;

void CQ_CreateQueue(CircularQueue**, QueueIndexType);
void CQ_DeallocateQueue(CircularQueue**);

void CQ_Enqueue(CircularQueue**, DataType);
DataType CQ_Dequeue(CircularQueue**);

QueueIndexType CQ_GetTotalNodeDataCount(CircularQueue**);
bool CQ_IsEmpty(CircularQueue**);
bool CQ_IsFull(CircularQueue**);
#endif