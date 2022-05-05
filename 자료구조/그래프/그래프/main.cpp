#include "Graph_Core.h"

#define VERTEX_COUNT 5 //정점의 개수
//#define TEST_ADJ_LIST
#define TEST_ADJ_MATRIX

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
#ifdef TEST_ADJ_LIST
		ADJ_LIST_GRAPH* adjListGraph = Graph_Adj_List_CreateGraph();
		ADJ_LIST_VERTEX* adjListVertex[VERTEX_COUNT] ={NULL,}; //정점

		for (int i = 0; i < VERTEX_COUNT; i++)
		{
			adjListVertex[i] = Graph_Adj_List_CreateVertex(i);
			Graph_Adj_List_AddVertex(adjListGraph, adjListVertex[i]);
		}

		//정점과 정점 간 간선으로 연결
		//0 => 1,2,3,4
		Graph_Adj_List_AddEdge(adjListVertex[0], 
			Graph_Adj_List_CreateEdge(0, adjListVertex[0], adjListVertex[1]));
		Graph_Adj_List_AddEdge(adjListVertex[0],
			Graph_Adj_List_CreateEdge(0, adjListVertex[0], adjListVertex[2]));
		Graph_Adj_List_AddEdge(adjListVertex[0],
			Graph_Adj_List_CreateEdge(0, adjListVertex[0], adjListVertex[3]));
		Graph_Adj_List_AddEdge(adjListVertex[0],
			Graph_Adj_List_CreateEdge(0, adjListVertex[0], adjListVertex[4]));

		//1 => 0, 2, 4
		Graph_Adj_List_AddEdge(adjListVertex[1],
			Graph_Adj_List_CreateEdge(0, adjListVertex[1], adjListVertex[0]));
		Graph_Adj_List_AddEdge(adjListVertex[1],
			Graph_Adj_List_CreateEdge(0, adjListVertex[1], adjListVertex[2]));
		Graph_Adj_List_AddEdge(adjListVertex[1],
			Graph_Adj_List_CreateEdge(0, adjListVertex[1], adjListVertex[4]));

		//2 => 0, 1
		Graph_Adj_List_AddEdge(adjListVertex[2],
			Graph_Adj_List_CreateEdge(0, adjListVertex[2], adjListVertex[0]));
		Graph_Adj_List_AddEdge(adjListVertex[2],
			Graph_Adj_List_CreateEdge(0, adjListVertex[2], adjListVertex[1]));

		//3 => 0, 4
		Graph_Adj_List_AddEdge(adjListVertex[3],
			Graph_Adj_List_CreateEdge(0, adjListVertex[3], adjListVertex[0]));
		Graph_Adj_List_AddEdge(adjListVertex[3],
			Graph_Adj_List_CreateEdge(0, adjListVertex[3], adjListVertex[4]));

		//4 => 0, 1, 3
		Graph_Adj_List_AddEdge(adjListVertex[4],
			Graph_Adj_List_CreateEdge(0, adjListVertex[4], adjListVertex[0]));
		Graph_Adj_List_AddEdge(adjListVertex[4],
			Graph_Adj_List_CreateEdge(0, adjListVertex[4], adjListVertex[1]));
		Graph_Adj_List_AddEdge(adjListVertex[4],
			Graph_Adj_List_CreateEdge(0, adjListVertex[4], adjListVertex[3]));

		Graph_Adj_List_DispGraph(adjListGraph);
		Graph_Adj_List_DeallocateGraph(&adjListGraph);
#endif

#ifdef TEST_ADJ_MATRIX
		ADJ_MATRIX_GRAPH* adjMatrixGraph = Graph_Adj_Matrix_CreateGraph();

		for (GRAPH_INDEX_TYPE i = 0; i < VERTEX_COUNT; i++) //push back
		{
			Graph_Adj_Matrix_AddVertex(adjMatrixGraph, i);
		}
		
		Graph_Adj_Matrix_DispVertexArray(adjMatrixGraph);

		//정점과 정점 간 간선으로 연결
		//0 => 1,2,3,4
		Graph_Adj_Matrix_AddEdge(adjMatrixGraph, 0, 0, 1);
		Graph_Adj_Matrix_AddEdge(adjMatrixGraph, 0, 0, 2);
		Graph_Adj_Matrix_AddEdge(adjMatrixGraph, 0, 0, 3);
		Graph_Adj_Matrix_AddEdge(adjMatrixGraph, 0, 0, 4);

		//1 => 0, 2, 4
		Graph_Adj_Matrix_AddEdge(adjMatrixGraph, 0, 1, 0);
		Graph_Adj_Matrix_AddEdge(adjMatrixGraph, 0, 1, 2);
		Graph_Adj_Matrix_AddEdge(adjMatrixGraph, 0, 1, 4);

		//2 => 0, 1
		Graph_Adj_Matrix_AddEdge(adjMatrixGraph, 0, 2, 0);
		Graph_Adj_Matrix_AddEdge(adjMatrixGraph, 0, 2, 1);

		//3 => 0, 4
		Graph_Adj_Matrix_AddEdge(adjMatrixGraph, 0, 3, 0);
		Graph_Adj_Matrix_AddEdge(adjMatrixGraph, 0, 3, 4);

		//4 => 0, 1, 3
		Graph_Adj_Matrix_AddEdge(adjMatrixGraph, 0, 4, 0);
		Graph_Adj_Matrix_AddEdge(adjMatrixGraph, 0, 4, 1);
		Graph_Adj_Matrix_AddEdge(adjMatrixGraph, 0, 4, 3);

		Graph_Adj_Matrix_DispEdgeArray(adjMatrixGraph);
#endif
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	_CrtMemCheckpoint(&newState); //할당 해제 후 상태
	_CrtDumpMemoryLeaks();
	if (_CrtMemDifference(&lastState, &oldState, &newState))
		_CrtMemDumpStatistics(&lastState);

	system("pause");
	return EXIT_SUCCESS;
}