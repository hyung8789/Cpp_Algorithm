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

	for (GRAPH_INDEX_TYPE i = 0; i < vertexCount; i++)
	{
		retVal->_vertexArray[i]._data = NOT_ASSIGNED;
		retVal->_vertexArray[i]._hasVisited = false;
		
		for (GRAPH_INDEX_TYPE j = 0; j < vertexCount; j++)
		{
			retVal->_edgeArray[utils::TwoDimensionIndexToOneDimensionIndex(vertexCount, vertexCount, i, j)]._hasConnected = false;
		}
	}

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

void Graph_Adj_Matrix_AddVertex(ADJ_MATRIX_GRAPH*, GRAPH_INDEX_TYPE, GRAPH_INDEX_TYPE, GRAPH_INDEX_TYPE)
{
	//TODO : row col 지정되지 않을 시 추가되는 정점의 수에 따라 가변적으로 할당
}

void Graph_Adj_Matrix_AddEdge(ADJ_MATRIX_GRAPH* srcGraph,
	GRAPH_INDEX_TYPE targetVertexRowIndex, GRAPH_INDEX_TYPE targetVertexColIndex)
{
	if (srcGraph == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if ((targetVertexRowIndex < 0 || targetVertexColIndex < 0) ||
		(targetVertexRowIndex >= srcGraph->_vertexCount || targetVertexColIndex >= srcGraph->_vertexCount))
		throw std::out_of_range(std::string(__func__) + std::string(" : out of range"));


}
