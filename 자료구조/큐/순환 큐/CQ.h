#ifndef _CQ_H_
#define _CQ_H_

typedef int DATA_TYPE; //노드의 데이터 타입
typedef int QUEUE_INDEX_TYPE; //큐 인덱스 타입

typedef struct NODE_TYPE
{
	DATA_TYPE _data; //노드의 데이터
}NODE;

typedef struct CIRCULAR_QUEUE_TYPE
{
	QUEUE_INDEX_TYPE _capacity; //할당 크기
	QUEUE_INDEX_TYPE _front, _rear; //전단, 후단 인덱스

	NODE* _nodeArray; //노드 배열
}CIRCULAR_QUEUE;

void CQ_CreateQueue(CIRCULAR_QUEUE**, QUEUE_INDEX_TYPE);
void CQ_DeallocateQueue(CIRCULAR_QUEUE**);

void CQ_Enqueue(CIRCULAR_QUEUE*, DATA_TYPE);
DATA_TYPE CQ_Dequeue(CIRCULAR_QUEUE*);

QUEUE_INDEX_TYPE CQ_GetTotalNodeCount(CIRCULAR_QUEUE*);
bool CQ_IsEmpty(CIRCULAR_QUEUE*);
bool CQ_IsFull(CIRCULAR_QUEUE*);
#endif