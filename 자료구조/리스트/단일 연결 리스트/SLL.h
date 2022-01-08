#ifndef _SLL_H_
#define _SLL_H_

typedef int DataType; //노드의 데이터 타입
typedef int NodePositionType; //노드 위치 타입

typedef struct NodeType 
{
	DataType data; //노드의 데이터
	NodeType* next; //다음 노드
}Node;

Node* SLL_CreateNode(DataType);
void SLL_DeallocateNode(Node**);
void SLL_DeallocateNodeList(Node**);

void SLL_AppendNode(Node**, Node*);
Node* SLL_GetNodeAt(Node**, NodePositionType);

void SLL_RemoveNodeAt(Node**, NodePositionType, bool = true);
void SLL_RemoveNode(Node**, Node*, bool = true);

void SLL_InsertNewHead(Node**, Node*);
void SLL_InsertNodeAfter(Node*, Node*);
void SLL_InsertNodeBefore(Node**, Node*, Node*);

NodePositionType SLL_GetTotalNodeCount(Node**);
void SLL_DispNodeList(Node**);
#endif