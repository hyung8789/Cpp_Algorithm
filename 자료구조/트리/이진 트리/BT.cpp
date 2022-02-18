#include "BT_Core.h"

/// <summary>
/// 새로운 노드 생성 및 생성 된 노드 반환
/// </summary>
/// <param name="srcData">노드의 데이터</param>
/// <returns>생성 된 노드</returns>
Node* BT_CreateNode(DataType srcData)
{
	Node* retVal = (Node*)malloc(sizeof(Node));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_data = srcData;
	retVal->_left = retVal->_right = NULL;

	return retVal;
}

/// <summary>
/// 대상 노드에 할당 된 메모리 해제
/// </summary>
/// <param name="srcNode">대상 노드</param>
void BT_DeallocateNode(Node** srcNode)
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
void BT_DeallocateTree(Node** srcRootNode)
{
	if ((*srcRootNode) != NULL) //후위 순회로 왼쪽 끝 노드부터 해제 
	{
		if ((*srcRootNode)->_left != NULL)
			BT_DeallocateTree(&((*srcRootNode)->_left));

		if((*srcRootNode)->_right != NULL)
			BT_DeallocateTree(&((*srcRootNode)->_right));

		BT_DeallocateNode(srcRootNode);
		(*srcRootNode) = NULL;
	}
}

/// <summary>
/// 순회 모드에 따른 대상 트리 출력
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="traversalMode">순회 모드</param>
void BT_DispOrderedTree(Node* srcRootNode, TRAVERSAL_MODE traversalMode)
{
	if (srcRootNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	switch (traversalMode)
	{
	case TRAVERSAL_MODE::PREORDER:
		std::cout << srcRootNode->_data << " ";

		if(srcRootNode->_left != NULL)
			BT_DispOrderedTree(srcRootNode->_left, traversalMode);

		if (srcRootNode->_right != NULL)
			BT_DispOrderedTree(srcRootNode->_right, traversalMode);
		break;

	case TRAVERSAL_MODE::INORDER:
		if (srcRootNode->_left != NULL)
			BT_DispOrderedTree(srcRootNode->_left, traversalMode);

		std::cout << srcRootNode->_data << " ";

		if (srcRootNode->_right != NULL)
			BT_DispOrderedTree(srcRootNode->_right, traversalMode);
		break;

	case TRAVERSAL_MODE::POSTORDER:
		if (srcRootNode->_left != NULL)
			BT_DispOrderedTree(srcRootNode->_left, traversalMode);

		if (srcRootNode->_right != NULL)
			BT_DispOrderedTree(srcRootNode->_right, traversalMode);

		std::cout << srcRootNode->_data << " ";
		break;
	}
}