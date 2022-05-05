#include "Graph_Core.h"

ADJ_MATRIX_GRAPH* Graph_Adj_Matrix_CreateGraph()
{
	ADJ_MATRIX_GRAPH* retVal = (ADJ_MATRIX_GRAPH*)malloc(sizeof(ADJ_MATRIX_GRAPH));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_vertexArray = NULL;
	retVal->_edgeArray = NULL;
	retVal->_vertexCount = 0;

	return retVal;
}

ADJ_MATRIX_GRAPH* Graph_Adj_Matrix_CreateGraph(GRAPH_INDEX_TYPE vertexCount)
{
	if (vertexCount <= 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	ADJ_MATRIX_GRAPH* retVal = (ADJ_MATRIX_GRAPH*)malloc(sizeof(ADJ_MATRIX_GRAPH));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_vertexArray = (ADJ_MATRIX_VERTEX*)malloc(sizeof(ADJ_MATRIX_VERTEX) * vertexCount);
	if (retVal->_vertexArray == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_edgeArray = (ADJ_MATRIX_EDGE*)malloc(sizeof(ADJ_MATRIX_EDGE) * (vertexCount * vertexCount));
	if (retVal->_vertexArray == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_vertexCount = vertexCount;
	Graph_Adj_Matrix_ClearProc(retVal, 0, (vertexCount - 1));

	return retVal;
}

void Graph_Adj_Matrix_DeallocateGraph(ADJ_MATRIX_GRAPH** srcGraph)
{
	if ((*srcGraph) != NULL)
	{
		free((*srcGraph)->_vertexArray);
		free((*srcGraph)->_edgeArray);

		(*srcGraph) = NULL;
	}
}

void Graph_Adj_Matrix_DispGraph(ADJ_MATRIX_GRAPH* srcGraph)
{
	if (srcGraph == NULL || srcGraph->_vertexArray == NULL || srcGraph->_edgeArray == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));




}

void Graph_Adj_Matrix_DispVertexArray(ADJ_MATRIX_GRAPH* srcGraph)
{
	if (srcGraph == NULL || srcGraph->_vertexArray == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	std::cout << "---------------------------------------------------------------------------------------------\n";
	std::cout << "[정점의 인덱스] 정점의 데이터 (정점의 방문 여부)\n";
	std::cout << "정점의 방문 여부 : 0 - 방문되지 않음, 1 - 방문 됨\n";
	std::cout << "---------------------------------------------------------------------------------------------\n";

	for (GRAPH_INDEX_TYPE i = 0; i < srcGraph->_vertexCount; i++)
	{
		std::cout << "[" << i << "] ";
		if (srcGraph->_vertexArray[i]._data == NOT_ASSIGNED)
			std::cout << "Not Assigned";
		else
			std::cout << srcGraph->_vertexArray[i]._data;

		std::cout << " (" << srcGraph->_vertexArray[i]._hasVisited << ")\n";
	}
}

void Graph_Adj_Matrix_DispEdgeArray(ADJ_MATRIX_GRAPH* srcGraph)
{
	if (srcGraph == NULL || srcGraph->_edgeArray == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	std::cout << "---------------------------------------------------------------------------------------------\n";
	std::cout << "간선의 연결 여부 (가중치)\n";
	std::cout << "간선의 연결 여부 : 0 - 정점 간에 연결되지 않음, 1 - 정점 간에 연결 됨\n";
	std::cout << "---------------------------------------------------------------------------------------------\n";

	for (GRAPH_INDEX_TYPE row = 0; row < srcGraph->_vertexCount; row++)
	{
		for (GRAPH_INDEX_TYPE col = 0; col < srcGraph->_vertexCount; col++)
		{
			size_t edgeIndex = 
				utils::TwoDimensionIndexToOneDimensionIndex(srcGraph->_vertexCount, srcGraph->_vertexCount, row, col);
			std::cout << srcGraph->_edgeArray[edgeIndex]._hasConnected << "(" <<
				srcGraph->_edgeArray[edgeIndex]._weight << ") ";
		}

		std::cout << "\n";
	}
}

/// <summary>
/// 대상 그래프에 정점 추가
/// </summary>
/// <param name="srcGraph">대상 그래프</param>
/// <param name="srcData">추가 할 정점의 데이터</param>
/// <param name="from">추가 할 정점의 인덱스
/// <para>(지정되지 않을 시 그래프의 가변적 크기 조절 및 끝에 추가 수행)</para></param>
void Graph_Adj_Matrix_AddVertex(ADJ_MATRIX_GRAPH* srcGraph,
	GRAPH_DATA_TYPE srcData, GRAPH_INDEX_TYPE targetVertexIndex)
{
	if (srcGraph == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	bool isPushBackProc =
		(targetVertexIndex == NOT_ASSIGNED) ? true : false; //끝에 정점 추가 수행 여부

	targetVertexIndex =
		(isPushBackProc) ? srcGraph->_vertexCount : targetVertexIndex; //끝에 정점 추가 수행 여부에 따른 추가 할 정점의 인덱스 재 할당
	srcGraph->_vertexCount++;

	if (isPushBackProc)
	{
		Graph_Adj_Matrix_ResizeProc(srcGraph, srcGraph->_vertexCount);
		Graph_Adj_Matrix_ClearProc(srcGraph, (srcGraph->_vertexCount - 1), (srcGraph->_vertexCount - 1)); //크기 재 조절 된 영역에 대해 초기화
	}

	if (srcGraph->_vertexArray[targetVertexIndex]._data != NOT_ASSIGNED)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (already exists data)"));

	srcGraph->_vertexArray[targetVertexIndex]._data = srcData;
}

/// <summary>
/// 대상 그래프에 정점과 연결 될 간선 추가
/// </summary>
/// <param name="srcGraph">대상 그래프</param>
/// <param name="weight">간선의 가중치</param>
/// <param name="from">간선의 시작 정점</param>
/// <param name="to">간선의 끝 정점</param>
void Graph_Adj_Matrix_AddEdge(ADJ_MATRIX_GRAPH* srcGraph,
	GRAPH_INDEX_TYPE weight, GRAPH_INDEX_TYPE from, GRAPH_INDEX_TYPE to)
{
	if (srcGraph == NULL || srcGraph->_vertexArray == NULL || srcGraph->_edgeArray == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if ((from < 0 || to < 0) ||
		(from >= srcGraph->_vertexCount || to >= srcGraph->_vertexCount))
		throw std::out_of_range(std::string(__func__) + std::string(" : out of range"));

	if (from == to)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (Not allowed self loop)"));

	if (srcGraph->_vertexArray[from]._data == NOT_ASSIGNED || srcGraph->_vertexArray[to]._data == NOT_ASSIGNED) //정점 없이 간선 존재불가
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	GRAPH_INDEX_TYPE edgeIndex = 
		utils::TwoDimensionIndexToOneDimensionIndex(srcGraph->_vertexCount, srcGraph->_vertexCount, from, to);

	if (srcGraph->_edgeArray[edgeIndex]._hasConnected) //이미 해당 간선이 연결되었으면
	{
		VAR_DUMP(from);
		VAR_DUMP(to);
		VAR_DUMP(edgeIndex);

		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (already connected)"));
	}
		
	srcGraph->_edgeArray[edgeIndex]._weight = weight;
	srcGraph->_edgeArray[edgeIndex]._hasConnected = true;
}

/// <summary>
/// 대상 그래프의 초기화 작업 수행
/// </summary>
/// <param name="srcGraph">대상 그래프</param>
/// <param name="startClearVertexIndex">초기화 작업이 시작 될 정점의 인덱스</param>
/// <param name="endClearVertexIndex">초기화 작업이 끝날 정점의 인덱스</param>
void Graph_Adj_Matrix_ClearProc(ADJ_MATRIX_GRAPH* srcGraph,
	GRAPH_INDEX_TYPE startClearVertexIndex, GRAPH_INDEX_TYPE endClearVertexIndex)
{
	if (srcGraph == NULL || srcGraph->_vertexArray == NULL || srcGraph->_edgeArray == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));
	
	if (startClearVertexIndex < 0 || (endClearVertexIndex < 0 || endClearVertexIndex >= srcGraph->_vertexCount))
		throw std::out_of_range(std::string(__func__) + std::string(" : out of range"));

	for (GRAPH_INDEX_TYPE i = startClearVertexIndex; i <= endClearVertexIndex; i++)
	{
		srcGraph->_vertexArray[i]._data = NOT_ASSIGNED;
		srcGraph->_vertexArray[i]._hasVisited = false;
	}

	for (GRAPH_INDEX_TYPE row = startClearVertexIndex; row <= endClearVertexIndex; row++)
	{
		for (GRAPH_INDEX_TYPE col = startClearVertexIndex; col <= endClearVertexIndex; col++)
		{
			/***
				ex)
				Vertex Array : 0 1 2
				startClearVertexIndex : 1
				clearEndVertexInde : 2
				edgeIndex : [1,1], [1,2], [2,1], [2,2]
			***/

			GRAPH_INDEX_TYPE edgeIndex = 
				utils::TwoDimensionIndexToOneDimensionIndex(srcGraph->_vertexCount, srcGraph->_vertexCount, row, col);
			srcGraph->_edgeArray[edgeIndex]._weight = NOT_ASSIGNED;
			srcGraph->_edgeArray[edgeIndex]._hasConnected = false;
		}
	}
}

/// <summary>
/// 대상 그래프의 크기 재 조절 작업 수행
/// </summary>
/// <param name="srcGraph">대상 그래프</param>
/// <param name="newVertexCount">크기 재 조절 작업 수행을 위한 새 정점의 수</param>
void Graph_Adj_Matrix_ResizeProc(ADJ_MATRIX_GRAPH* srcGraph,
	GRAPH_INDEX_TYPE newVertexCount)
{
	if (srcGraph == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (srcGraph->_vertexArray == NULL && srcGraph->_edgeArray == NULL && newVertexCount > 0)
	{
		srcGraph->_vertexArray = (ADJ_MATRIX_VERTEX*)malloc(sizeof(ADJ_MATRIX_VERTEX) * newVertexCount);
		if (srcGraph->_vertexArray == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		srcGraph->_edgeArray = (ADJ_MATRIX_EDGE*)malloc(sizeof(ADJ_MATRIX_EDGE) * (newVertexCount * newVertexCount));
		if (srcGraph->_vertexArray == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));
	}
	else if (srcGraph->_vertexArray != NULL && srcGraph->_edgeArray != NULL && newVertexCount > 0)
	{
		size_t reallocVertexArraySizeInBytes = sizeof(ADJ_MATRIX_VERTEX) * newVertexCount; //재 할당 될 바이트 단위 크기
		size_t reallocEdgeArraySizeInBytes = sizeof(ADJ_MATRIX_EDGE) * (newVertexCount * newVertexCount); //재 할당 될 바이트 단위 크기

		void* reallocVertexArrayAddr = realloc(srcGraph->_vertexArray, reallocVertexArraySizeInBytes);
		if (reallocVertexArrayAddr == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		void* reallocEdgeArrayAddr = realloc(srcGraph->_edgeArray, reallocEdgeArraySizeInBytes);
		if (reallocEdgeArrayAddr == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		srcGraph->_vertexArray = (ADJ_MATRIX_VERTEX*)reallocVertexArrayAddr;
		srcGraph->_edgeArray = (ADJ_MATRIX_EDGE*)reallocEdgeArrayAddr;
	}
	else
	{
		throw myexception::MEM_CORRUPTION_EXCEPTION(std::string(__func__) + std::string(" : Mem corruption"));
	}
}