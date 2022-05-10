#include "Graph_Core.h"

/// <summary>
/// 가변 크기 그래프 생성 및 반환
/// </summary>
/// <param name="isSymmetric">그래프의 대칭 여부</param>
/// <returns>생성 된 가변 크기 그래프</returns>
ADJ_MATRIX_GRAPH* Graph_Adj_Matrix_CreateGraph(bool isSymmetric)
{
	ADJ_MATRIX_GRAPH* retVal = (ADJ_MATRIX_GRAPH*)malloc(sizeof(ADJ_MATRIX_GRAPH));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_vertexArray = NULL;
	retVal->_edgeArray = NULL;
	retVal->_vertexCount = 0;
	retVal->_isSymmetric = isSymmetric;
	retVal->_isFixedSize = false;
	retVal->_lastMallocEdgeIndex = 0;

	return retVal;
}

/// <summary>
/// 정점 개수에 따라 고정 크기 그래프 생성 및 반환
/// </summary>
/// <param name="isSymmetric">그래프의 대칭 여부</param>
/// <returns>정점 개수에 따라 생성 된 고정 크기 그래프</returns>
ADJ_MATRIX_GRAPH* Graph_Adj_Matrix_CreateGraph(bool isSymmetric, GRAPH_INDEX_TYPE vertexCount)
{
	if (vertexCount <= 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	ADJ_MATRIX_GRAPH* retVal = (ADJ_MATRIX_GRAPH*)malloc(sizeof(ADJ_MATRIX_GRAPH));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_vertexArray = NULL;
	retVal->_edgeArray = NULL;
	retVal->_vertexCount = vertexCount;
	retVal->_isSymmetric = isSymmetric;
	retVal->_isFixedSize = true;
	retVal->_lastMallocEdgeIndex = 0;

	Graph_Adj_Matrix_ResizeGraphProc(retVal);
	Graph_Adj_Matrix_InitGraphProc(retVal, 0, (vertexCount - 1));

	return retVal;
}

/// <summary>
/// 대상 그래프에 할당 된 메모리 해제
/// </summary>
/// <param name="srcGraph">대상 그래프</param>
void Graph_Adj_Matrix_DeallocateGraph(ADJ_MATRIX_GRAPH** srcGraph)
{
	if ((*srcGraph) != NULL)
	{
		free((*srcGraph)->_vertexArray);
		(*srcGraph)->_vertexArray = NULL;

		for (GRAPH_INDEX_TYPE i = 0; i < (*srcGraph)->_vertexCount; i++)
		{
			free((*srcGraph)->_edgeArray[i]);
			(*srcGraph)->_edgeArray[i] = NULL;
		}

		free((*srcGraph)->_edgeArray);
		(*srcGraph)->_edgeArray = NULL;

		free(*srcGraph);
		(*srcGraph) = NULL;
	}
}

/// <summary>
/// 대상 그래프 출력
/// </summary>
/// <param name="srcGraph">대상 그래프</param>
void Graph_Adj_Matrix_DispGraph(ADJ_MATRIX_GRAPH* srcGraph)
{
	if (srcGraph == NULL || srcGraph->_vertexArray == NULL || srcGraph->_edgeArray == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	std::cout << "\n---------------------------------------------------------------------------------------------\n";
	std::cout << "[정점 인덱스] 정점 데이터 (정점 방문 여부) : 인접 한 정점 데이터 (인접 한 정점으로의 가중치)\n";
	std::cout << "- 정점 데이터 : X - Not Assigned\n";
	std::cout << "- 정점 방문 여부 : 0 - 방문되지 않음, 1 - 방문 됨\n";
	std::cout << "- 인접 한 정점 데이터 : X - Not Assigned\n";
	std::cout << "- 인접 한 정점으로의 가중치 : X - Not Assigned\n";
	std::cout << "---------------------------------------------------------------------------------------------\n";

	for (GRAPH_INDEX_TYPE i = 0; i < srcGraph->_vertexCount; i++)
	{
		std::cout << "[" << i << "] ";
		if (srcGraph->_vertexArray[i]._data == NOT_ASSIGNED)
			std::cout << "X";
		else
			std::cout << srcGraph->_vertexArray[i]._data;

		std::cout << " (" << srcGraph->_vertexArray[i]._hasVisited << ") : ";

		/***
			< 현재 정점과 인접 한 정점 목록 출력 >

			ex)
			i : 1
			Vertex Count : 3
			Vertex Array : [0], [1], [2]
			Edge Array (x - adj check target, num - adj check sequence) :
			[]		[x1]	[]
			[x4]	[x2]	[x5]
			[]		[x3]	[]
		***/

		for (GRAPH_INDEX_TYPE row = 0; row < srcGraph->_vertexCount; row++) //열 고정
		{
			if (srcGraph->_edgeArray[row][i]._hasConnected)
			{
				if (srcGraph->_vertexArray[row]._data == NOT_ASSIGNED)
					std::cout << "X";
				else
					std::cout << srcGraph->_vertexArray[row]._data;

				std::cout << " (";
				if (srcGraph->_edgeArray[row][i]._weight == NOT_ASSIGNED)
					std::cout << "X) ";
				else
					std::cout << srcGraph->_edgeArray[row][i]._weight << ") ";
			}
		}

		if (srcGraph->_isSymmetric)
			break;

		for (GRAPH_INDEX_TYPE col = 0; col < srcGraph->_vertexCount; col++) //행 고정
		{
			if (col != i) //row, col가 만나는 중복 위치 제외
			{
				if (srcGraph->_edgeArray[i][col]._hasConnected)
				{
					if (srcGraph->_vertexArray[col]._data == NOT_ASSIGNED)
						std::cout << "X";
					else
						std::cout << srcGraph->_vertexArray[col]._data;

					std::cout << " (";
					if (srcGraph->_edgeArray[i][col]._weight == NOT_ASSIGNED)
						std::cout << "X) ";
					else
						std::cout << srcGraph->_edgeArray[i][col]._weight << ") ";
				}
			}
		}

		std::cout << "\n";
	}
}

/// <summary>
/// 대상 그래프의 정점 배열 출력
/// </summary>
/// <param name="srcGraph">대상 그래프</param>
void Graph_Adj_Matrix_DispVertexArray(ADJ_MATRIX_GRAPH* srcGraph)
{
	if (srcGraph == NULL || srcGraph->_vertexArray == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	std::cout << "\n--------------------------------------------------------------------------\n";
	std::cout << "[정점 인덱스] 정점 데이터 (정점 방문 여부)\n";
	std::cout << "- 정점 데이터 : X - Not Assigned\n";
	std::cout << "- 정점 방문 여부 : 0 - 방문되지 않음, 1 - 방문 됨\n";
	std::cout << "--------------------------------------------------------------------------\n";

	for (GRAPH_INDEX_TYPE i = 0; i < srcGraph->_vertexCount; i++)
	{
		std::cout << "[" << i << "] ";
		if (srcGraph->_vertexArray[i]._data == NOT_ASSIGNED)
			std::cout << "X";
		else
			std::cout << srcGraph->_vertexArray[i]._data;

		std::cout << " (" << srcGraph->_vertexArray[i]._hasVisited << ")\n";
	}
}

/// <summary>
/// 대상 그래프의 간선 배열 출력
/// </summary>
/// <param name="srcGraph">대상 그래프</param>
void Graph_Adj_Matrix_DispEdgeArray(ADJ_MATRIX_GRAPH* srcGraph)
{
	if (srcGraph == NULL || srcGraph->_edgeArray == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	std::cout << "\n--------------------------------------------------------------------------\n";
	std::cout << "간선의 연결 여부 (가중치)\n";
	std::cout << "- 간선의 연결 여부 : 0 - 정점 간에 연결되지 않음, 1 - 정점 간에 연결 됨\n";
	std::cout << "- 가중치 : X - Not Assigned\n";
	std::cout << "--------------------------------------------------------------------------\n";

	for (GRAPH_INDEX_TYPE row = 0; row < srcGraph->_vertexCount; row++)
	{
		for (GRAPH_INDEX_TYPE col = 0; col < srcGraph->_vertexCount; col++)
		{
			std::cout << srcGraph->_edgeArray[row][col]._hasConnected << "(";

			if (srcGraph->_edgeArray[row][col]._weight == NOT_ASSIGNED)
				std::cout << "X";
			else
				std::cout << srcGraph->_edgeArray[row][col]._weight;

			std::cout << ") ";
		}

		std::cout << "\n";
	}
}

/// <summary>
/// 대상 그래프에 깊이 우선 탐색 (Depth First Search)에 따른 방문 경로 출력
/// </summary>
/// <param name="srcGraph">대상 그래프</param>
void Graph_Adj_Matrix_DispDFSPath(ADJ_MATRIX_GRAPH* srcGraph)
{
	if (srcGraph == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));


}

/// <summary>
/// 대상 그래프에 너비 우선 탐색 (Breadth First Search)에 따른 방문 경로 출력
/// </summary>
/// <param name="srcGraph">대상 그래프</param>
void Graph_Adj_Matrix_DispBFSPath(ADJ_MATRIX_GRAPH* srcGraph)
{
	if (srcGraph == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));
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

	if (isPushBackProc && srcGraph->_isFixedSize) //고정 크기에 대해 끝에 정점 추가 수행 불가
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (Fixed Size)"));

	targetVertexIndex =
		(isPushBackProc) ? srcGraph->_vertexCount : targetVertexIndex; //끝에 정점 추가 수행 여부에 따른 추가 할 정점의 인덱스 재할당

	if (isPushBackProc)
	{
		srcGraph->_vertexCount++;
		Graph_Adj_Matrix_ResizeGraphProc(srcGraph);
		Graph_Adj_Matrix_InitGraphProc(srcGraph, (srcGraph->_vertexCount - 1), (srcGraph->_vertexCount - 1)); //크기 재 조절 된 영역에 대해 초기화
	}

	if (srcGraph->_vertexArray[targetVertexIndex]._data != NOT_ASSIGNED)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (Already exists vertex)"));

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

	if (srcGraph->_vertexArray[from]._data == NOT_ASSIGNED || srcGraph->_vertexArray[to]._data == NOT_ASSIGNED) //정점 없이 간선 존재불가
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (from == to)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (Not allowed self loop)"));

	if (srcGraph->_edgeArray[from][to]._hasConnected) //이미 해당 간선이 연결되었으면
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args (already connected)"));

	srcGraph->_edgeArray[from][to]._weight = weight;
	srcGraph->_edgeArray[from][to]._hasConnected = true;

	if (srcGraph->_isSymmetric && !(srcGraph->_edgeArray[to][from]._hasConnected)) //대칭 그래프인 경우
		Graph_Adj_Matrix_AddEdge(srcGraph, weight, to, from);
}

/// <summary>
/// 대상 그래프의 초기화 작업 수행
/// </summary>
/// <param name="srcGraph">대상 그래프</param>
/// <param name="startInitVertexIndex">초기화 작업이 시작 될 정점의 기준 인덱스</param>
/// <param name="endInitVertexIndex">초기화 작업이 끝날 정점의 기준 인덱스</param>
void Graph_Adj_Matrix_InitGraphProc(ADJ_MATRIX_GRAPH* srcGraph,
	GRAPH_INDEX_TYPE startInitVertexIndex, GRAPH_INDEX_TYPE endInitVertexIndex)
{
	if (srcGraph == NULL || srcGraph->_vertexArray == NULL || srcGraph->_edgeArray == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (startInitVertexIndex < 0 || endInitVertexIndex < 0 ||
		endInitVertexIndex < startInitVertexIndex || endInitVertexIndex >= srcGraph->_vertexCount)
		throw std::out_of_range(std::string(__func__) + std::string(" : out of range"));

	for (GRAPH_INDEX_TYPE i = startInitVertexIndex; i <= endInitVertexIndex; i++)
		Graph_Adj_Matrix_InitVertexProc(&(srcGraph->_vertexArray[i]));

	/***
		ex)
		Vertex Count : 3
		Vertex Array : [0], [1], [2]
		startInitVertexIndex : 1, endInitVertexIndex : 2
		Edge Array (x - init target, num - init sequence) :
		[]		[x8]	[x5]
		[x7]	[x6]	[x3]
		[x4]	[x2]	[x1]
	***/

	while (endInitVertexIndex >= startInitVertexIndex) //초기화 작업이 시작 될 정점의 기준 인덱스까지
	{
		GRAPH_INDEX_TYPE rowEdgeIndex = endInitVertexIndex;
		GRAPH_INDEX_TYPE colEdgeIndex = endInitVertexIndex;

		//row, col가 만나는 공통 영역 초기화
		Graph_Adj_Matrix_InitEdgeProc(&(srcGraph->_edgeArray[endInitVertexIndex][endInitVertexIndex]));

		while ((--colEdgeIndex) >= 0) //초기화 시퀸스에 따라
		{
			Graph_Adj_Matrix_InitEdgeProc(&(srcGraph->_edgeArray[rowEdgeIndex][colEdgeIndex]));
			Graph_Adj_Matrix_InitEdgeProc(&(srcGraph->_edgeArray[colEdgeIndex][rowEdgeIndex]));
		}

		endInitVertexIndex--; //row, col가 만나는 공통 영역 축소
	}
}

/// <summary>
/// 대상 그래프의 크기 재조절 작업 수행
/// </summary>
/// <param name="srcGraph">대상 그래프</param>
void Graph_Adj_Matrix_ResizeGraphProc(ADJ_MATRIX_GRAPH* srcGraph)
{
	if (srcGraph == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (srcGraph->_vertexArray == NULL && srcGraph->_edgeArray == NULL && srcGraph->_vertexCount > 0)
	{
		srcGraph->_vertexArray = (ADJ_MATRIX_VERTEX*)malloc(sizeof(ADJ_MATRIX_VERTEX) * srcGraph->_vertexCount);
		if (srcGraph->_vertexArray == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		srcGraph->_edgeArray = (ADJ_MATRIX_EDGE**)malloc(sizeof(ADJ_MATRIX_EDGE*) * srcGraph->_vertexCount); //row
		if (srcGraph->_edgeArray == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		for (GRAPH_INDEX_TYPE i = 0; i < srcGraph->_vertexCount; i++) //col
		{
			srcGraph->_edgeArray[i] = (ADJ_MATRIX_EDGE*)malloc(sizeof(ADJ_MATRIX_EDGE) * srcGraph->_vertexCount);
			if (srcGraph->_edgeArray[i] == NULL)
				throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));
		}
	}
	else if (srcGraph->_vertexArray != NULL && srcGraph->_edgeArray != NULL && srcGraph->_vertexCount > 0)
	{
		//재 할당 될 바이트 단위 크기
		size_t reallocVertexArraySizeInBytes = sizeof(ADJ_MATRIX_VERTEX) * srcGraph->_vertexCount;
		size_t reallocEdgePtrArraySizeInBytes = sizeof(ADJ_MATRIX_EDGE*) * srcGraph->_vertexCount;
		size_t reallocEdgeElementArraySizeInBytes = sizeof(ADJ_MATRIX_EDGE) * srcGraph->_vertexCount;

		void* reallocVertexArrayAddr = realloc(srcGraph->_vertexArray, reallocVertexArraySizeInBytes);
		if (reallocVertexArrayAddr == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		void* reallocEdgePtrArrayAddr = realloc(srcGraph->_edgeArray, reallocEdgePtrArraySizeInBytes); //row
		if (reallocEdgePtrArrayAddr == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		srcGraph->_vertexArray = (ADJ_MATRIX_VERTEX*)reallocVertexArrayAddr;
		srcGraph->_edgeArray = (ADJ_MATRIX_EDGE**)reallocEdgePtrArrayAddr;

		for (GRAPH_INDEX_TYPE i = 0; i <= srcGraph->_lastMallocEdgeIndex; i++) //col (마지막으로 힙 메모리 할당 된 정점 인덱스까지 재할당)
		{
			void* reallocEdgeElementArrayAddr = realloc(srcGraph->_edgeArray[i], reallocEdgeElementArraySizeInBytes);
			if (reallocEdgeElementArrayAddr == NULL)
				throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

			srcGraph->_edgeArray[i] = (ADJ_MATRIX_EDGE*)reallocEdgeElementArrayAddr;
		}

		for (GRAPH_INDEX_TYPE i = srcGraph->_lastMallocEdgeIndex + 1; i < srcGraph->_vertexCount; i++) //col
		{
			srcGraph->_edgeArray[i] = (ADJ_MATRIX_EDGE*)malloc(reallocEdgeElementArraySizeInBytes);
			if (srcGraph->_edgeArray[i] == NULL)
				throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));
		}
	}
	else
	{
		throw myexception::MEM_CORRUPTION_EXCEPTION(std::string(__func__) + std::string(" : Mem corruption"));
	}

	srcGraph->_lastMallocEdgeIndex = srcGraph->_vertexCount - 1;
}

/// <summary>
/// 대상 정점 초기화 작업 수행
/// </summary>
/// <param name="targetVertex">대상 정점</param>
inline void Graph_Adj_Matrix_InitVertexProc(ADJ_MATRIX_VERTEX* targetVertex)
{
	targetVertex->_data = NOT_ASSIGNED;
	targetVertex->_hasVisited = false;
}

/// <summary>
/// 대상 간선 초기화 작업 수행
/// </summary>
/// <param name="targetEdge">대상 간선</param>
inline void Graph_Adj_Matrix_InitEdgeProc(ADJ_MATRIX_EDGE* targetEdge)
{
	targetEdge->_weight = NOT_ASSIGNED;
	targetEdge->_hasConnected = false;
}