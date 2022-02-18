#ifndef _LLQ_H_
#define _LLQ_H_

typedef int DataType; //노드의 데이터 타입
typedef int QueueIndexType; //큐 인덱스 타입

typedef struct NodeType
{
	DataType _data; //노드의 데이터

	NodeType* _next; //다음 노드
}Node;

typedef struct LinkedListQueueType
{
	Node* _front; //전단 노드
	Node* _rear; //후단 노드

	QueueIndexType _totalNodeCount; //전체 노드 수
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