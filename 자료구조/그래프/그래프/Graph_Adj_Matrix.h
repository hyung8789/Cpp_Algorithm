#ifndef _GRAPH_ADJ_MATRIX_H_
#define _GRAPH_ADJ_MATRIX_H_

/// <summary>
/// 인접 행렬 정점
/// </summary>
struct ADJ_MATRIX_VERTEX
{
	GRAPH_INDEX_TYPE _data; //정점의 데이터
	bool _hasVisited; //정점의 방문 여부
};

/// <summary>
/// 인접 행렬 간선
/// </summary>
struct ADJ_MATRIX_EDGE
{
	bool _hasConnected; //간선의 연결 여부
};

/// <summary>
/// 인접 행렬 그래프
/// </summary>
struct ADJ_MATRIX_GRAPH
{
	ADJ_MATRIX_VERTEX* _vertexArray; //정점 배열
	ADJ_MATRIX_EDGE* _edgeArray; //정점 간의 연결을 관리하기 위한 간선 배열 (정점의 수 * 정점의 수)

	GRAPH_INDEX_TYPE _vertexCount; //정점의 수
};

ADJ_MATRIX_GRAPH* Graph_Adj_Matrix_CreateGraph();
ADJ_MATRIX_GRAPH* Graph_Adj_Matrix_CreateGraph(GRAPH_INDEX_TYPE);

void Graph_Adj_Matrix_DeallocateGraph(ADJ_MATRIX_GRAPH**);

void Graph_Adj_Matrix_AddVertex(ADJ_MATRIX_GRAPH*, GRAPH_INDEX_TYPE,
	GRAPH_INDEX_TYPE = NOT_ASSIGNED, GRAPH_INDEX_TYPE = NOT_ASSIGNED);
void Graph_Adj_Matrix_AddEdge(ADJ_MATRIX_GRAPH*, 
	GRAPH_INDEX_TYPE, GRAPH_INDEX_TYPE);
#endif