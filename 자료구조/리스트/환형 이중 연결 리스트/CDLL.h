#ifndef _CDLL_H_
#define _CDLL_H_

typedef int DataType; //����� ������ Ÿ��
typedef int NodePositionType; //��� ��ġ Ÿ��

typedef struct NodeType 
{
	DataType _data; //����� ������

	NodeType* _prev; //���� ���
	NodeType* _next; //���� ���
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