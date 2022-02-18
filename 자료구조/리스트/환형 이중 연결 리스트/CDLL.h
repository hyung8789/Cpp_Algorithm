#ifndef _CDLL_H_
#define _CDLL_H_

typedef int DataType; //노드의 데이터 타입
typedef int NodePositionType; //노드 위치 타입

typedef struct NodeType 
{
	DataType _data; //노드의 데이터

	NodeType* _prev; //이전 노드
	NodeType* _next; //다음 노드
}Node;

Node* CDLL_CreateNode(DataType);
void CDLL_DeallocateNode(Node**);
void CDLL_DeallocateNodeList(Node**);

void CDLL_AppendNode(Node**, Node*);
Node* CDLL_GetNodeAt(Node**, NodePositionType);

void CDLL_RemoveNodeAt(Node**, NodePositionType, bool = true);
void CDLL_RemoveNode(Node**, Node*, bool = true);

void CDLL_InsertNewHead(Node**, Node*);
void CDLL_InsertNodeAfter(Node*, Node*);
void CDLL_InsertNodeBefore(Node**, Node*, Node*);

NodePositionType CDLL_GetTotalNodeCount(Node**);
void CDLL_DispNodeList(Node**);
#endif