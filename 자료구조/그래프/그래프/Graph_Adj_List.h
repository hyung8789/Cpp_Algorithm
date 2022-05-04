#ifndef _GRAPH_ADJ_LIST_H_
#define _GRAPH_ADJ_LIST_H_

/// <summary>
/// 인접 리스트 정점
/// </summary>
struct ADJ_LIST_VERTEX
{
	GRAPH_INDEX_TYPE _index; //그래프 상의 정점 인덱스
	GRAPH_DATA_TYPE _data; //정점의 데이터
	bool _hasVisited; //정점의 방문 여부

	ADJ_LIST_VERTEX* _next; //다음 정점
	struct ADJ_LIST_EDGE* _edgeList; //정점과 연결 된 간선의 목록 (헤드 간선)
};

/// <summary>
/// 인접 리스트 간선
/// </summary>
struct ADJ_LIST_EDGE
{
	GRAPH_INDEX_TYPE _weight; //간선의 가중치

	ADJ_LIST_EDGE* _next; //다음 간선
	ADJ_LIST_VERTEX* _from; //간선의 시작 정점
	ADJ_LIST_VERTEX* _to; //간선의 끝 정점
};

/// <summary>
/// 인접 리스트 그래프
/// </summary>
struct ADJ_LIST_GRAPH
{
	ADJ_LIST_VERTEX* _vertexList; //정점 목록 (헤드 정점)
	GRAPH_INDEX_TYPE _vertexCount; //정점의 수
};

ADJ_LIST_GRAPH* Graph_Adj_List_CreateGraph();
ADJ_LIST_VERTEX* Graph_Adj_List_CreateVertex(GRAPH_DATA_TYPE);
ADJ_LIST_EDGE* Graph_Adj_List_CreateEdge(GRAPH_INDEX_TYPE, ADJ_LIST_VERTEX*, ADJ_LIST_VERTEX*);

void Graph_Adj_List_DeallocateGraph(ADJ_LIST_GRAPH**);
void Graph_Adj_List_DeallocateVertex(ADJ_LIST_VERTEX**);
void Graph_Adj_List_DeallocateEdge(ADJ_LIST_EDGE**);

void Graph_Adj_List_DispGraph(ADJ_LIST_GRAPH*);

void Graph_Adj_List_AddVertex(ADJ_LIST_GRAPH*, ADJ_LIST_VERTEX*);
void Graph_Adj_List_AddEdge(ADJ_LIST_VERTEX*, ADJ_LIST_EDGE*);
#endif,