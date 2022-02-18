#ifndef _LLS_H_
#define _LLS_H_

typedef int StackIndexType; //스택 인덱스 타입

typedef struct NodeType
{
	char* _data; //노드의 데이터

	NodeType* _next; //다음 노드
}Node;

typedef struct LinkedListStackType
{
	Node* _top; //최상위 노드 (tail)
	Node* _head; //헤드 노드

	StackIndexType _totalNodeCount; //전체 노드 수
}LinkedListStack;

void LLS_CreateStack(LinkedListStack**);
void LLS_DeallocateLinkedListStack(LinkedListStack**);

Node* LLS_CreateNode(const char*);
void LLS_DeallocateNode(Node**);

void LLS_Push(LinkedListStack**, Node*);
Node* LLS_Pop(LinkedListStack**);
Node* LLS_Peek(LinkedListStack**);

StackIndexType LLS_GetTotalNodeCount(LinkedListStack**);
bool LLS_IsEmpty(LinkedListStack**);
#endif