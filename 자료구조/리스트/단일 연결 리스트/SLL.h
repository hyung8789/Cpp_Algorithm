#ifndef _SLL_H_
#define _SLL_H_

typedef int DataType; //����� ������ Ÿ��
typedef int NodePositionType; //��� ��ġ Ÿ��

typedef struct NodeType 
{
	DataType data; //����� ������
	NodeType* next; //���� ���
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