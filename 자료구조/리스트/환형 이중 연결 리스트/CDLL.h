#ifndef _CDLL_H_
#define _CDLL_H_

typedef int DATA_TYPE; //노드의 데이터 타입
typedef int NODE_POSITION_TYPE; //노드 위치 타입

/// <summary>
/// 환형 이중 연결 리스트 노드
/// </summary>
typedef struct NODE_TYPE 
{
	DATA_TYPE _data; //노드의 데이터

	NODE_TYPE* _prev; //이전 노드
	NODE_TYPE* _next; //다음 노드
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

NODE_POSITION_TYPE CDLL_GetTotalNodeCount(NODE*);
void CDLL_DispNodeList(NODE*);
#endif