#ifndef _SLL_H_
#define _SLL_H_

typedef int DATA_TYPE; //노드의 데이터 타입
typedef int NODE_POSITION_TYPE; //노드 위치 타입

/// <summary>
/// 단일 연결 리스트 노드
/// </summary>
typedef struct NODE_TYPE 
{
	DATA_TYPE _data; //노드의 데이터

	NODE_TYPE* _next; //다음 노드
}NODE;

NODE* SLL_CreateNode(DATA_TYPE);
void SLL_DeallocateNode(NODE**);
void SLL_DeallocateNodeList(NODE**);

void SLL_AppendNode(NODE**, NODE*);
NODE* SLL_GetNodeAt(NODE**, NODE_POSITION_TYPE);

void SLL_RemoveNodeAt(NODE**, NODE_POSITION_TYPE, bool = true);
void SLL_RemoveNode(NODE**, NODE*, bool = true);

void SLL_InsertNewHead(NODE**, NODE*);
void SLL_InsertNodeAfter(NODE*, NODE*);
void SLL_InsertNodeBefore(NODE**, NODE*, NODE*);

NODE_POSITION_TYPE SLL_GetTotalNodeCount(NODE**);
void SLL_DispNodeList(NODE**);
#endif