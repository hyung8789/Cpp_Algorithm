#ifndef _PQ_H_
#define _PQ_H_

/***
	- 우선순위 큐 : 우선순위를 갖는 요소들을 우선순위에 따라, 순차적 접근을 위한 큐
***/

#define CAPACITY_REALLOC_RATIO_THRESHOLD 0.5 //기존 할당 크기에 대한 재 할당이 발생 될 임계 비율 (0.0 ~ 1.0)
#define CAPACITY_REALLOC_RATIO 0.5 //기존 할당 크기에 대해 증가 혹은 감소 시 재 할당 될 비율

#define HIGH_PRIORITY_FIRST 0 //높은 우선순위 우선 (우선순위 0 > 우선순위 1)
#define LOW_PRIORITY_FIRST 1 //낮은 우선순위 우선 (우선순위 0 < 우선순위 1)

#define PRIORITY_ORDER_BY HIGH_PRIORITY_FIRST //우선순위 정렬 기준

typedef int QUEUE_INDEX_TYPE; //큐 인덱스 타입
typedef int PRIORITY_TYPE; //우선순위 타입

typedef struct NODE_TYPE
{
	PRIORITY_TYPE _priority; //우선순위
	void* _data; //노드의 데이터
}NODE;

struct PRIORITY_QUEUE
{
	QUEUE_INDEX_TYPE _capacity; //할당 크기
	QUEUE_INDEX_TYPE _usedSize; //사용량

	NODE* _nodeArray; //노드 배열
};

PRIORITY_QUEUE* PQ_CreateQueue(QUEUE_INDEX_TYPE);
void PQ_DeallocateQueue(PRIORITY_QUEUE**);

void PQ_Enqueue(PRIORITY_QUEUE*, NODE*);
void PQ_Dequeue(PRIORITY_QUEUE*, NODE*);
const NODE& PQ_Peek(PRIORITY_QUEUE*);

QUEUE_INDEX_TYPE PQ_GetTotalNodeCount(PRIORITY_QUEUE*);
bool PQ_IsEmpty(PRIORITY_QUEUE*);
bool PQ_IsFull(PRIORITY_QUEUE*);

QUEUE_INDEX_TYPE PQ_GetParentIndex(QUEUE_INDEX_TYPE);
QUEUE_INDEX_TYPE PQ_GetLeftChildIndex(QUEUE_INDEX_TYPE);
QUEUE_INDEX_TYPE PQ_GetRightChildIndex(QUEUE_INDEX_TYPE);
QUEUE_INDEX_TYPE PQ_GetDepthFromIndex(QUEUE_INDEX_TYPE);
#endif