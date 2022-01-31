#include "DS_Core.h"

/// <summary>
/// 새로운 노드 생성 및 생성 된 노드 반환
/// </summary>
/// <param name="srcData">노드의 데이터</param>
/// <returns>생성 된 노드</returns>
Node* DS_CreateNode(void* srcData)
{
	Node* retVal = (Node*)malloc(sizeof(Node));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->data = srcData;
	retVal->parent = NULL;

	return retVal;
}

/// <summary>
/// 대상 노드에 할당 된 메모리 해제
/// </summary>
/// <param name="srcNode">대상 노드</param>
void DS_DeallocateNode(Node** srcNode)
{
	if ((*srcNode) != NULL)
	{
		free(*srcNode);
		(*srcNode) = NULL;
	}
}

/// <summary>
/// 두 대상 집합 노드가 속한 집합 통합 (합집합)
/// </summary>
/// <param name="originSetNode">통합 될 원본 집합 노드</param>
/// <param name="targetSetNode">통합 될 원본 집합에 통합 할 집합의 노드</param>
void DS_UnionSet(Node** originSetNode, Node* targetSetNode)
{
	if (targetSetNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*originSetNode) == NULL)
	{
		(*originSetNode) = targetSetNode;
	}
	else
	{
		targetSetNode = DS_FindSet(targetSetNode); //통합 될 원본 집합에 통합 할 집합의 최상위 부모 노드
		targetSetNode->parent = DS_FindSet((*originSetNode));
	}
}

/// <summary>
/// 대상 집합 노드가 속한 집합 (최상위 부모 노드) 반환
/// </summary>
/// <param name="targetSetNode">대상 집합 노드</param>
/// <returns>대상 집합 노드가 속한 집합 (최상위 부모 노드)</returns>
Node* DS_FindSet(Node* targetSetNode)
{
	if (targetSetNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	while (targetSetNode->parent != NULL) //최상위 부모 노드 탐색
	{
		targetSetNode = targetSetNode->parent;
	}

	return targetSetNode;
}