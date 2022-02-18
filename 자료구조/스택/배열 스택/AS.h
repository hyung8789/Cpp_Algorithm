#ifndef _AS_H_
#define _AS_H_

#define CAPACITY_REDUCE_RATIO_THRESHOLD 0.7 //기존 할당 크기에 대한 감소가 발생 될 빈 공간 임계 비율 (0.0 ~ 1.0)
#define CAPACITY_REALLOC_RATIO (1.0 - CAPACITY_REDUCE_RATIO_THRESHOLD) //기존 할당 크기에 대해 증가 혹은 감소 시 재 할당 될 비율

typedef int DataType; //노드의 데이터 타입
typedef int StackIndexType; //스택 인덱스 타입

typedef struct NodeType
{
	DataType _data; //노드의 데이터
}Node;

typedef struct ArrayStackType
{
	StackIndexType _capacity; //할당 크기
	StackIndexType _top; //최상위 노드 인덱스

	Node* _nodeArray; //노드 배열
}ArrayStack;

void AS_CreateStack(ArrayStack**, StackIndexType);
void AS_DeallocateArrayStack(ArrayStack**);

void AS_Push(ArrayStack**, DataType);
DataType AS_Pop(ArrayStack**);
DataType AS_Peek(ArrayStack**);

StackIndexType AS_GetTotalNodeDataCount(ArrayStack**);
bool AS_IsEmpty(ArrayStack**);
bool AS_IsFull(ArrayStack**);
#endif