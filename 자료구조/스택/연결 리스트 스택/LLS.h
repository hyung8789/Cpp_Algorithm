#ifndef _LLS_H_
#define _LLS_H_
#pragma warning(disable : 6011) //NULL 역참조 경고 해제

typedef int StackIndexType; //스택 인덱스 타입

typedef struct NodeType
{
	char* data; //노드의 데이터
	NodeType* next; //다음 노드
}Node;

typedef struct LinkedListStackType
{
	Node* top; //최상위 노드 (tail)
	Node* head; //헤드 노드

	StackIndexType totalNodeCount; //전체 노드 수
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