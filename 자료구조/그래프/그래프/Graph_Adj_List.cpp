#include "Graph_Core.h"

ADJ_LIST_GRAPH* Graph_Adj_List_CreateGraph()
{
	return nullptr;
}

void Graph_Adj_List_DeallocateGraph(ADJ_LIST_GRAPH**)
{
}

ADJ_LIST_VERTEX* Graph_Adj_List_CreateVertex(GRAPH_DATA_TYPE)
{
	return nullptr;
}

void Graph_Adj_List_DeallocateVertex(ADJ_LIST_VERTEX**)
{
}

ADJ_LIST_EDGE* Graph_Adj_List_CreateEdge(ADJ_LIST_VERTEX*, ADJ_LIST_VERTEX, GRAPH_INDEX_TYPE)
{
	return nullptr;
}

void Graph_Adj_List_DeallocateEdge(ADJ_LIST_EDGE**)
{
}

void Graph_Adj_AddVertex(ADJ_LIST_GRAPH*, ADJ_LIST_VERTEX*)
{
}

void Graph_Adj_AddEdge(ADJ_LIST_VERTEX*, ADJ_LIST_EDGE*)
{
}

void Graph_Adj_DispGraph(ADJ_LIST_GRAPH*)
{
}
