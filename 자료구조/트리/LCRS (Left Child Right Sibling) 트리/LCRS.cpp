#include "LCRS_Core.h"

/// <summary>
/// 새로운 노드 생성 및 생성 된 노드 반환
/// </summary>
/// <param name="srcData">노드의 데이터</param>
/// <returns>생성 된 노드</returns>
Node* LCRS_CreateNode(DataType srcData)
{
	Node* retVal = (Node*)malloc(sizeof(Node));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->data = srcData;
	retVal->leftChild = retVal->rightSibling = NULL;

	return retVal;
}

/// <summary>
/// 대상 노드에 할당 된 메모리 해제
/// </summary>
/// <param name="srcNode">대상 노드</param>
void LCRS_DeallocateNode(Node** srcNode)
{
	if ((*srcNode) != NULL)
	{
		free(*srcNode);
		(*srcNode) = NULL;
	}
}

/// <summary>
/// 대상 트리에 할당 된 모든 노드의 메모리 해제
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
void LCRS_DeallocateTree(Node** srcRootNode)
{

}

/// <summary>
/// 대상 노드에 자식 노드 연결
/// </summary>
/// <param name="srcTargetNode">대상 노드</param>
/// <param name="srcNewNode">대상 노드에 연결 될 자식 노드</param>
void LCRS_AddChildNode(Node** srcTargetNode, Node* srcNewNode)
{
}

/// <summary>
/// 대상 트리의 모든 노드 출력
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="depth">현재 노드의 깊이</param>
void LCRS_DispTree(Node** srcRootNode, TreeIndexType depth)
{
}
