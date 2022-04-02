#ifndef _HEAP_H_
#define _HEAP_H_

/***
	- 힙 : 힙 순서 속성 (트리 내의 모든 노드가 부모 노드보다 크거나 작음)을 만족하는 완전 이진 트리
	- 완전 이진 트리 : 왼쪽에서 오른쪽 순으로 노드가 채워져 있으며, 포화 상태가 아닌 트리

	< 연속적인 요소 (배열)로 구현 시 힙의 요소 접근 방법 >

	ex) MIN_HEAP

						5 (root)
				7						30
		18			33				60		100
	19		30

	0 [5] : depth 0
	1 [7] : depth 1
	2 [30] : depth 1
	3 [18] : depth 2
	4 [33] : depth 2
	5 [60] : depth 2
	6 [100] : depth 2
	7 [19] : depth 3
	8 [30] : depth 3

	- i번 인덱스 요소의 왼쪽 자식 인덱스 : 2i + 1
	- i번 인덱스 요소의 오른쪽 자식 인덱스 : 2i + 2
	- i번 인덱스 요소의 부모 인덱스 : (i-1) / 2
***/

#define CAPACITY_REALLOC_RATIO_THRESHOLD 0.5 //기존 할당 크기에 대한 재 할당이 발생 될 빈 공간 임계 비율 (0.0 ~ 1.0)
#define CAPACITY_REALLOC_RATIO (1.0 - CAPACITY_REALLOC_RATIO_THRESHOLD) //기존 할당 크기에 대해 증가 혹은 감소 시 재 할당 될 비율

#define MIN_HEAP 0 //parent < left child && parent < right child
#define MAX_HEAP 1 //parent > left child && parent > right child

#define HEAP_ORDER_PROPERTY MAX_HEAP //힙 순서 속성

typedef int DATA_TYPE; //노드의 데이터 타입
typedef int HEAP_INDEX_TYPE; //힙 인덱스 타입

typedef struct NODE_TYPE
{
	DATA_TYPE _data; //노드의 데이터
}NODE;

struct HEAP
{
	HEAP_INDEX_TYPE _capacity; //할당 크기
	HEAP_INDEX_TYPE _usedSize; //사용량

	NODE* _nodeArray; //노드 배열
};

HEAP* HEAP_CreateHeap(HEAP_INDEX_TYPE);
void HEAP_DeallocateHeap(HEAP**);

void HEAP_DispTotalNode(HEAP*);

void HEAP_Push(HEAP*, const DATA_TYPE&);
DATA_TYPE HEAP_Pop(HEAP*);
const DATA_TYPE& HEAP_Peek(HEAP*);

HEAP_INDEX_TYPE HEAP_GetParentIndex(HEAP_INDEX_TYPE);
HEAP_INDEX_TYPE HEAP_GetLeftChildIndex(HEAP_INDEX_TYPE);
HEAP_INDEX_TYPE HEAP_GetRightChildIndex(HEAP_INDEX_TYPE);
HEAP_INDEX_TYPE HEAP_GetDepthFromIndex(HEAP_INDEX_TYPE);
#endif