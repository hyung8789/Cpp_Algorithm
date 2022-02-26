#ifndef _LLQ_H_
#define _LLQ_H_

typedef int DATA_TYPE; //노드의 데이터 타입
typedef int QUEUE_INDEX_TYPE; //큐 인덱스 타입

typedef struct NODE_TYPE
{
	DATA_TYPE _data; //노드의 데이터

	NODE_TYPE* _next; //다음 노드
}NODE;

typedef struct LINKED_LIST_QUEUE_TYPE
{
	NODE* _front; //전단 노드
	NODE* _rear; //후단 노드

	QUEUE_INDEX_TYPE _totalNodeCount; //전체 노드 수
}LINKED_LIST_QUEUE;

void LLQ_CreateQueue(LINKED_LIST_QUEUE**);
void LLQ_DeallocateQueue(LINKED_LIST_QUEUE**);

NODE* LLQ_CreateNode(DATA_TYPE);
void LLQ_DeallocateNode(NODE**);

void LLQ_Enqueue(LINKED_LIST_QUEUE**, NODE*);
NODE* LLQ_Dequeue(LINKED_LIST_QUEUE**);

QUEUE_INDEX_TYPE LLQ_GetTotalNodeCount(LINKED_LIST_QUEUE**);
bool LLQ_IsEmpty(LINKED_LIST_QUEUE**);
#endif