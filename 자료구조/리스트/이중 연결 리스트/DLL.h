#ifndef _DLL_H_
#define _DLL_H_

typedef int DATA_TYPE; //노드의 데이터 타입
typedef int NODE_POSITION_TYPE; //노드 위치 타입

/// <summary>
/// 이중 연결 리스트 노드
/// </summary>
typedef struct NODE_TYPE 
{
	DATA_TYPE _data; //노드의 데이터

	NODE_TYPE* _prev; //이전 노드
	NODE_TYPE* _next; //다음 노드
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