#ifndef _DLL_H_
#define _DLL_H_

typedef int DATA_TYPE; //����� ������ Ÿ��
typedef int NODE_POSITION_TYPE; //��� ��ġ Ÿ��

typedef struct NODE_TYPE 
{
	DATA_TYPE _data; //����� ������

	NODE_TYPE* _prev; //���� ���
	NODE_TYPE* _next; //���� ���
}NODE;

NODE* DLL_CreateNode(DATA_TYPE);
void DLL_DeallocateNode(NODE**);
void DLL_DeallocateNodeList(NODE**);

void DLL_AppendNode(NODE**, NODE*);
NODE* DLL_GetNodeAt(NODE**, NODE_POSITION_TYPE);

void DLL_RemoveNodeAt(NODE**, NODE_POSITION_TYPE, bool = true);
void DLL_RemoveNode(NODE**, NODE*, bool = true);

void DLL_InsertNewHead(NODE**, NODE*);
void DLL_InsertNodeAfter(NODE*, NODE*);
void DLL_InsertNodeBefore(NODE**, NODE*, NODE*);

NODE_POSITION_TYPE DLL_GetTotalNodeCount(NODE**);
void DLL_DispNodeList(NODE**);
#endif