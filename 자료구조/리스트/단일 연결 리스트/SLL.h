#ifndef _SLL_H_
#define _SLL_H_

typedef int DataType; //노드의 데이터 타입
typedef int NodePositionType; //노드 위치 타입

typedef struct NodeType 
{
	DataType data; //노드의 데이터
	NodeType* next; //다음 노드
}Node;

Node* SLL_CreateNode(DataType) throw(std::runtime_error);
void SLL_DeallocateNode(Node**);
void SLL_DeallocateNodeList(Node**);

void SLL_AppendNode(Node**, Node*) throw(std::invalid_argument);
Node* SLL_GetNodeAt(Node**, NodePositionType) throw(std::invalid_argument, std::runtime_error);

void SLL_RemoveNodeAt(Node**, NodePositionType, bool = true) throw(std::invalid_argument, std::runtime_error);
void SLL_RemoveNode(Node**, Node*, bool = true) throw(std::invalid_argument, std::runtime_error);

void SLL_InsertNewHead(Node**, Node*) throw(std::invalid_argument);
void SLL_InsertNodeAfter(Node*, Node*) throw(std::invalid_argument);
void SLL_InsertNodeBefore(Node**, Node*, Node*) throw(std::invalid_argument, std::runtime_error);

NodePositionType SLL_GetTotalNodeCount(Node**);
void SLL_DispNodeList(Node**);
#endif