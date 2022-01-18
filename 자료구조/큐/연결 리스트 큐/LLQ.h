#ifndef _LLQ_H_
#define _LLQ_H_

typedef int DataType; //����� ������ Ÿ��
typedef int QueueIndexType; //ť �ε��� Ÿ��

typedef struct NodeType
{
	DataType data; //����� ������
	NodeType* next; //���� ���
}Node;

typedef struct LinkedListQueueType
{
	Node* front; //���� ���
	Node* rear; //�Ĵ� ���
	QueueIndexType totalNodeCount; //��ü ��� ��
}LinkedListQueue;

void LLQ_CreateQueue(LinkedListQueue**);
void LLQ_DeallocateQueue(LinkedListQueue**);

Node* LLQ_CreateNode(DataType);
void LLQ_DeallocateNode(Node**);

void LLQ_Enqueue(LinkedListQueue**, Node*);
Node* LLQ_Dequeue(LinkedListQueue**);

QueueIndexType LLQ_GetTotalNodeCount(LinkedListQueue**);
bool LLQ_IsEmpty(LinkedListQueue**);
#endif