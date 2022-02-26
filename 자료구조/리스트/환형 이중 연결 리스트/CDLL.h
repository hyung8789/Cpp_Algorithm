#ifndef _CDLL_H_
#define _CDLL_H_

typedef int DATA_TYPE; //����� ������ Ÿ��
typedef int NODE_POSITION_TYPE; //��� ��ġ Ÿ��

typedef struct NODE_TYPE 
{
	DATA_TYPE _data; //����� ������

	NODE_TYPE* _prev; //���� ���
	NODE_TYPE* _next; //���� ���
}NODE;

NODE* CDLL_CreateNode(DATA_TYPE);
void CDLL_DeallocateNode(NODE**);
void CDLL_DeallocateNodeList(NODE**);

void CDLL_AppendNode(NODE**, NODE*);
NODE* CDLL_GetNodeAt(NODE**, NODE_POSITION_TYPE);

void CDLL_RemoveNodeAt(NODE**, NODE_POSITION_TYPE, bool = true);
void CDLL_RemoveNode(NODE**, NODE*, bool = true);

void CDLL_InsertNewHead(NODE**, NODE*);
void CDLL_InsertNodeAfter(NODE*, NODE*);
void CDLL_InsertNodeBefore(NODE**, NODE*, NODE*);

NODE_POSITION_TYPE CDLL_GetTotalNodeCount(NODE**);
void CDLL_DispNodeList(NODE**);
#endif