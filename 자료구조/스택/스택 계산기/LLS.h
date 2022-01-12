#ifndef _LLS_H_
#define _LLS_H_
//#pragma warning(disable : 6011) //NULL 역참조 경고 해제

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

void LLS_CreateStack(LinkedListStack**) throw(std::runtime_error);
void LLS_DeallocateLinkedListStack(LinkedListStack**);

Node* LLS_CreateNode(const char*) throw(std::invalid_argument, std::runtime_error);
void LLS_DeallocateNode(Node**);

void LLS_Push(LinkedListStack**, Node*) throw(std::invalid_argument, std::runtime_error);
Node* LLS_Pop(LinkedListStack**) throw(std::runtime_error);
Node* LLS_Peek(LinkedListStack**) throw(std::runtime_error);

StackIndexType LLS_GetTotalNodeCount(LinkedListStack**) throw(std::runtime_error);
bool LLS_IsEmpty(LinkedListStack**) throw(std::runtime_error);
#endif