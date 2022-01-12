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

void AS_CreateStack(ArrayStack**, StackIndexType) throw(std::invalid_argument, std::runtime_error);
void AS_DeallocateArrayStack(ArrayStack**);

void AS_Push(ArrayStack**, DataType) throw(std::runtime_error);
DataType AS_Pop(ArrayStack**) throw(std::runtime_error);
DataType AS_Peek(ArrayStack**) throw(std::runtime_error);

StackIndexType AS_GetTotalNodeDataCount(ArrayStack**) throw(std::runtime_error);
bool AS_IsEmpty(ArrayStack**) throw(std::runtime_error);
bool AS_IsFull(ArrayStack**) throw(std::runtime_error);
#endif