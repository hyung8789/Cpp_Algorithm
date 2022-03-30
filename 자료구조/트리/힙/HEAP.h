#ifndef _HEAP_H_
#define _HEAP_H_

/***
	< 연속적인 요소 (배열)로 구현 시 힙의 요소 접근 방법 >

	ex) MIN_HEAP_TYPE

						5 (root)
				7						30
		22			33				60		100
	19		30

	0 [5] : depth 0
	1 [7] : depth 1
	2 [30] : depth 1
	3 [22] : depth 2
	4 [33] : depth 2
	5 [60] : depth 2
	6 [100] : depth 2
	7 [19] : depth 3
	8 [30] : depth 3

	- i번 인덱스 요소의 왼쪽 자식 인덱스 : 2i + 1
	- i번 인덱스 요소의 오른쪽 자식 인덱스 : 2i + 2
	- i번 인덱스 요소의 부모 인덱스 : (i-1) / 2
***/

#define MIN_HEAP_TYPE 0 //root < left && root < right
#define MAX_HEAP_TYPE 1 //root > left && root > right

#define HEAP_TYPE MIN_HEAP_TYPE

typedef int DATA_TYPE; //노드의 데이터 타입
typedef int HEAP_INDEX_TYPE; //힙 인덱스 타입

typedef struct NODE_TYPE
{
	DATA_TYPE _data; //노드의 데이터
}NODE;

struct HEAP
{
	HEAP_INDEX_TYPE _capacity; //할당 크기
	HEAP_INDEX_TYPE _used; //사용량

	NODE* _nodeArray;
};

HEAP* HEAP_CreateHeap(HEAP_INDEX_TYPE);
void HEAP_DeallocateHeap(HEAP**);

void HEAP_Push(HEAP*, const DATA_TYPE&);
void HEAP_Pop(HEAP*);
DATA_TYPE HEAP_Peek(HEAP*);
#endif