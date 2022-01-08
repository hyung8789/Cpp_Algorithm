#ifndef _AS_H_
#define _AS_H_
#pragma warning(disable : 6011) //NULL 역참조 경고 해제

typedef int DataType; //노드의 데이터 타입
typedef int StackIndexType; //스택 인덱스 타입

typedef struct NodeType
{
	DataType data; //노드의 데이터
}Node;

typedef struct ArrayStackType
{
	StackIndexType capacity; //할당 크기
	StackIndexType top; //최상위 노드 인덱스
	Node* nodeArray; //노드 배열
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