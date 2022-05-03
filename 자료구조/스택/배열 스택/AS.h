#ifndef _AS_H_
#define _AS_H_

#define CAPACITY_REALLOC_RATIO_THRESHOLD 0.7 //기존 할당 크기에 대한 재 할당이 발생 될 임계 비율 (0.0 ~ 1.0)
#define CAPACITY_REALLOC_RATIO 0.5 //기존 할당 크기에 대해 증가 혹은 감소 시 재 할당 될 비율

typedef int DATA_TYPE; //노드의 데이터 타입
typedef int STACK_INDEX_TYPE; //스택 인덱스 타입

/// <summary>
/// 배열 스택 노드
/// </summary>
typedef struct NODE_TYPE
{
	DATA_TYPE _data; //노드의 데이터
}NODE;

/// <summary>
/// 배열 스택
/// </summary>
typedef struct ARRAY_STACK_TYPE
{
	STACK_INDEX_TYPE _capacity; //할당 크기
	STACK_INDEX_TYPE _top; //최상위 노드 인덱스

	NODE* _nodeArray; //노드 배열
}ARRAY_STACK;

void AS_CreateStack(ARRAY_STACK**, STACK_INDEX_TYPE);
void AS_DeallocateArrayStack(ARRAY_STACK**);

void AS_Push(ARRAY_STACK*, DATA_TYPE);
DATA_TYPE AS_Pop(ARRAY_STACK*);
DATA_TYPE AS_Peek(ARRAY_STACK*);

STACK_INDEX_TYPE AS_GetTotalNodeCount(ARRAY_STACK*);
bool AS_IsEmpty(ARRAY_STACK*);
bool AS_IsFull(ARRAY_STACK*);
#endif