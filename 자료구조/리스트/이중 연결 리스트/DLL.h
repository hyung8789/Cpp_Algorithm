#ifndef _DLL_H_
#define _DLL_H_

typedef int DataType; //����� ������ Ÿ��
typedef int NodePositionType; //��� ��ġ Ÿ��

typedef struct NodeType 
{
	DataType _data; //����� ������

	NodeType* _prev; //���� ���
	NodeType* _next; //���� ���
}Node;

Node* DLL_CreateNode(DataType);
void DLL_DeallocateNode(Node**);
void DLL_DeallocateNodeList(Node**);

void DLL_AppendNode(Node**, Node*);
Node* DLL_GetNodeAt(Node**, NodePositionType);

void DLL_RemoveNodeAt(Node**, NodePositionType, bool = true);
void DLL_RemoveNode(Node**, Node*, bool = true);

void DLL_InsertNewHead(Node**, Node*);
void DLL_InsertNodeAfter(Node*, Node*);
void DLL_InsertNodeBefore(Node**, Node*, Node*);

NodePositionType DLL_GetTotalNodeCount(Node**);
void DLL_DispNodeList(Node**);
#endif