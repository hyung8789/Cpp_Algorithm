#ifndef _LLS_H_
#define _LLS_H_

typedef int STACK_INDEX_TYPE; //스택 인덱스 타입

/// <summary>
/// 연결 리스트 스택 노드
/// </summary>
typedef struct NODE_TYPE
{
	char* _data; //노드의 데이터

	NODE_TYPE* _next; //다음 노드
}NODE;

/// <summary>
/// 연결 리스트 스택
/// </summary>
typedef struct LINKED_LIST_STACK_TYPE
{
	NODE* _top; //최상위 노드 (tail)
	NODE* _head; //헤드 노드

	STACK_INDEX_TYPE _totalNodeCount; //전체 노드 수
}LINKED_LIST_STACK;

void LLS_CreateStack(LINKED_LIST_STACK**);
void LLS_DeallocateLinkedListStack(LINKED_LIST_STACK**);

NODE* LLS_CreateNode(const char*);
void LLS_DeallocateNode(NODE**);

void LLS_Push(LINKED_LIST_STACK*, NODE*);
NODE* LLS_Pop(LINKED_LIST_STACK*);
NODE* LLS_Peek(LINKED_LIST_STACK*);

STACK_INDEX_TYPE LLS_GetTotalNodeCount(LINKED_LIST_STACK*);
bool LLS_IsEmpty(LINKED_LIST_STACK*);
#endif