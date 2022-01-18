#ifndef _CQ_H_
#define _CQ_H_

typedef int DataType; //노드의 데이터 타입
typedef int QueueIndexType; //큐 인덱스 타입

typedef struct NodeType
{
	DataType data; //노드의 데이터
}Node;

typedef struct CircularQueueType
{
	QueueIndexType capacity; //할당 크기
	QueueIndexType front, rear; //전단, 후단 인덱스
	Node* nodeArray; //노드 배열
}CircularQueue;

void CQ_CreateQueue(CircularQueue**, QueueIndexType);
void CQ_DeallocateQueue(CircularQueue**);

void CQ_Enqueue(CircularQueue**, DataType);
DataType CQ_Dequeue(CircularQueue**);

QueueIndexType CQ_GetTotalNodeDataCount(CircularQueue**);
bool CQ_IsEmpty(CircularQueue**);
bool CQ_IsFull(CircularQueue**);
#endif