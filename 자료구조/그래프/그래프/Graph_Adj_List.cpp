#include "Graph_Core.h"

/// <summary>
/// 그래프 생성 및 반환
/// </summary>
/// <returns>생성 된 그래프</returns>
ADJ_LIST_GRAPH* Graph_Adj_List_CreateGraph()
{
	ADJ_LIST_GRAPH* retVal = (ADJ_LIST_GRAPH*)malloc(sizeof(ADJ_LIST_GRAPH));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_vertexList = NULL;
	retVal->_vertexCount = 0;

	return retVal;
}

/// <summary>
/// 새로운 정점 생성 및 생성 된 정점 반환
/// </summary>
/// <param name="srcData">정점의 데이터</param>
/// <returns>생성 된 정점</returns>
ADJ_LIST_VERTEX* Graph_Adj_List_CreateVertex(GRAPH_DATA_TYPE srcData)
{
	ADJ_LIST_VERTEX* retVal = (ADJ_LIST_VERTEX*)malloc(sizeof(ADJ_LIST_VERTEX));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_index = NOT_ASSIGNED;
	retVal->_data = srcData;
	retVal->_hasVisited = false;
	retVal->_next = NULL;
	retVal->_edgeList = NULL;

	return retVal;
}

/// <summary>
/// 새로운 간선 생성 및 생성 된 간선 반환
/// </summary>
/// <param name="weight">간선의 가중치</param>
/// <param name="from">간선의 시작 정점</param>
/// <param name="to">간선의 끝 정점</param>
/// <returns>생성 된 간선</returns>
ADJ_LIST_EDGE* Graph_Adj_List_CreateEdge(GRAPH_INDEX_TYPE weight, ADJ_LIST_VERTEX* from, ADJ_LIST_VERTEX* to)
{
	if (from == NULL || to == NULL) //정점 없이 간선 존재 불가
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (from == to)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (Not allowed self loop)"));

	ADJ_LIST_EDGE* retVal = (ADJ_LIST_EDGE*)malloc(sizeof(ADJ_LIST_EDGE));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_weight = weight;
	retVal->_next = NULL;
	retVal->_from = from;
	retVal->_to = to;

	return retVal;
}

/// <summary>
/// 대상 그래프에 할당 된 메모리 해제
/// </summary>
/// <param name="srcGraph">대상 그래프</param>
void Graph_Adj_List_DeallocateGraph(ADJ_LIST_GRAPH** srcGraph)
{
	if ((*srcGraph) != NULL)
	{
		while ((*srcGraph)->_vertexList != NULL)
		{
			ADJ_LIST_VERTEX* tmp = (*srcGraph)->_vertexList;
			(*srcGraph)->_vertexList = (*srcGraph)->_vertexList->_next;

			Graph_Adj_List_DeallocateVertex(&tmp);
		}

		free(*srcGraph);
		(*srcGraph) = NULL;
	}
}

/// <summary>
/// 대상 정점에 할당 된 메모리 해제
/// </summary>
/// <param name="srcVertex">대상 정점</param>
void Graph_Adj_List_DeallocateVertex(ADJ_LIST_VERTEX** srcVertex)
{
	if ((*srcVertex) != NULL)
	{
		while ((*srcVertex)->_edgeList != NULL)
		{
			ADJ_LIST_EDGE* tmp = (*srcVertex)->_edgeList;
			(*srcVertex)->_edgeList = (*srcVertex)->_edgeList->_next;

			Graph_Adj_List_DeallocateEdge(&tmp);
		}

		free(*srcVertex);
		(*srcVertex) = NULL;
	}
}

/// <summary>
/// 대상 간선에 할당 된 메모리 해제
/// </summary>
/// <param name="srcEdge">대상 간선</param>
void Graph_Adj_List_DeallocateEdge(ADJ_LIST_EDGE** srcEdge)
{
	if ((*srcEdge) != NULL)
	{
		free(*srcEdge);
		(*srcEdge) = NULL;
	}
}

/// <summary>
/// 대상 그래프 출력
/// </summary>
/// <param name="srcGraph">대상 그래프</param>
void Graph_Adj_List_DispGraph(ADJ_LIST_GRAPH* srcGraph)
{
	if (srcGraph == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	std::cout << "---------------------------------------------------------------------------------------------\n";
	std::cout << "[정점 인덱스] 정점 데이터 (정점 방문 여부) : 인접 한 정점 데이터 (인접 한 정점으로의 가중치)\n";
	std::cout << "- 정점 인덱스 : X - Not Assigned\n";
	std::cout << "- 정점 방문 여부 : 0 - 방문되지 않음, 1 - 방문 됨\n";
	std::cout << "---------------------------------------------------------------------------------------------\n";

	ADJ_LIST_VERTEX* vertexList = srcGraph->_vertexList; //정점 목록
	ADJ_LIST_EDGE* edgeList = NULL; //정점과 연결 된 간선 목록

	while (vertexList != NULL)
	{
		edgeList = vertexList->_edgeList;

		std::cout << "[";
		if (vertexList->_index == NOT_ASSIGNED)
			std::cout << "X";
		else
			std::cout << vertexList->_index;
		std::cout << "] " << vertexList->_data << " ("
			<< vertexList->_hasVisited << ") : ";

		while (edgeList != NULL) //현재 정점과 인접 한 정점 목록 출력
		{
			std::cout << edgeList->_to->_data
				<< "(" << edgeList->_weight << ") ";

			edgeList = edgeList->_next;
		}

		std::cout << "\n";
		vertexList = vertexList->_next;
	}

	std::cout << "\n";
}

/// <summary>
/// 대상 그래프에 깊이 우선 탐색 (Depth First Search)에 따른 방문 경로 출력
/// </summary>
/// <param name="srcGraph">대상 그래프</param>
/// <param name="currentVertex">방문 할 다음 정점</param>
void Graph_Adj_List_DispDFSPath(ADJ_LIST_GRAPH* srcGraph, ADJ_LIST_VERTEX* nextVertex)
{
	if (srcGraph == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	ADJ_LIST_VERTEX* currentVertex; //현재 정점
	ADJ_LIST_EDGE* currentVertexAdjEdgeList; //현재 정점의 인접 간선 목록

	if (nextVertex == NULL) //최초 호출 시
	{
		std::cout << "-------------- DFS Path --------------\n";
		std::cout << "방문 순서에 따른 [정점 인덱스] 정점 데이터\n";
		std::cout << "- 정점 인덱스 : X - Not Assigned\n";
		std::cout << "--------------------------------------\n";

		currentVertex = srcGraph->_vertexList;
	}
	else
	{
		currentVertex = nextVertex;
	}

	currentVertexAdjEdgeList = currentVertex->_edgeList;

	std::cout << "[";
	if (currentVertex->_index == NOT_ASSIGNED)
		std::cout << "X";
	else
		std::cout << currentVertex->_index;
	std::cout << "] " << currentVertex->_data << std::endl;

	currentVertex->_hasVisited = true;

	while (currentVertexAdjEdgeList != NULL) //현재 정점과 인접한 모든 정점에 대해
	{
		nextVertex = currentVertexAdjEdgeList->_to; //다음에 방문 할 인접 정점

		if (nextVertex == NULL)
			throw myexception::MEM_CORRUPTION_EXCEPTION(std::string(__func__) + std::string(" : Mem corruption"));

		if (!nextVertex->_hasVisited) //아직 방문하지 않은 인접 정점이 존재 할 경우
			Graph_Adj_List_DispDFSPath(srcGraph, nextVertex);

		currentVertexAdjEdgeList = currentVertexAdjEdgeList->_next;
	}
}

/// <summary>
/// 대상 그래프에 너비 우선 탐색 (Breadth First Search)에 따른 방문 경로 출력
/// </summary>
/// <param name="srcGraph">대상 그래프</param>
void Graph_Adj_List_DispBFSPath(ADJ_LIST_GRAPH* srcGraph)
{
	if (srcGraph == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	ADJ_LIST_VERTEX* currentVertex = srcGraph->_vertexList; //현재 정점
	ADJ_LIST_EDGE* currentVertexAdjEdgeList = currentVertex->_edgeList; //현재 정점의 인접 간선 목록
	std::queue<ADJ_LIST_VERTEX*> vertexQueue;

	vertexQueue.push(currentVertex);

	std::cout << "-------------- BFS Path --------------\n";
	std::cout << "방문 순서에 따른 [정점 인덱스] 정점 데이터\n";
	std::cout << "- 정점 인덱스 : X - Not Assigned\n";
	std::cout << "--------------------------------------\n";
	std::cout << "[";
	if (currentVertex->_index == NOT_ASSIGNED)
		std::cout << "X";
	else
		std::cout << currentVertex->_index;
	std::cout << "] " << currentVertex->_data << std::endl;

	currentVertex->_hasVisited = true;

	while (!vertexQueue.empty()) //모든 정점을 방문 할 때까지
	{
		currentVertex = vertexQueue.front();
		currentVertexAdjEdgeList = currentVertex->_edgeList;

		while (currentVertexAdjEdgeList != NULL) //현재 정점의 모든 인접 정점에 대해 동일 시퀸스 내에 방문
		{
			currentVertex = currentVertexAdjEdgeList->_to;

			if (currentVertex == NULL)
				throw myexception::MEM_CORRUPTION_EXCEPTION(std::string(__func__) + std::string(" : Mem corruption"));

			if (!currentVertex->_hasVisited) //현재 정점에 아직 방문하지 않은 인접 정점이 존재 할 경우
			{
				std::cout << "[";
				if (currentVertex->_index == NOT_ASSIGNED)
					std::cout << "X";
				else
					std::cout << currentVertex->_index;
				std::cout << "] " << currentVertex->_data << std::endl;

				currentVertex->_hasVisited = true;
				vertexQueue.push(currentVertex);
			}

			currentVertexAdjEdgeList = currentVertexAdjEdgeList->_next;
		}

		vertexQueue.pop();
	}
}

/// <summary>
/// 대상 그래프에 정점 추가
/// </summary>
/// <param name="srcGraph">대상 그래프</param>
/// <param name="srcVertex">추가 할 정점</param>
void Graph_Adj_List_AddVertex(ADJ_LIST_GRAPH* srcGraph, ADJ_LIST_VERTEX* srcVertex)
{
	if (srcGraph == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (srcVertex == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	ADJ_LIST_VERTEX* vertexList = srcGraph->_vertexList; //대상 그래프의 정점 목록

	if (vertexList == NULL)
	{
		srcGraph->_vertexList = srcVertex;
	}
	else
	{
		while (vertexList->_next != NULL) //마지막 정점으로 이동
			vertexList = vertexList->_next;

		vertexList->_next = srcVertex;
	}

	srcGraph->_vertexCount++;
}

/// <summary>
/// 대상 정점에 대상 정점과 연결 될 간선 추가
/// </summary>
/// <param name="targetVertex">대상 정점</param>
/// <param name="srcEdge">대상 정점과 연결 될 간선</param>
void Graph_Adj_List_AddEdge(ADJ_LIST_VERTEX* targetVertex, ADJ_LIST_EDGE* srcEdge)
{
	if (targetVertex == NULL || srcEdge == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	ADJ_LIST_EDGE* edgeList = targetVertex->_edgeList; //대상 정점의 간선 목록

	if (edgeList == NULL) //대상 정점과 연결 된 간선이 존재하지 않으면
	{
		targetVertex->_edgeList = srcEdge;
	}
	else
	{
		while (edgeList->_next != NULL) //마지막 간선으로 이동
			edgeList = edgeList->_next;

		edgeList->_next = srcEdge;
	}
}