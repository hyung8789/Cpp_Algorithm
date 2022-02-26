#include "BT_Core.h"

/// <summary>
/// 새로운 노드 생성 및 생성 된 노드 반환
/// </summary>
/// <param name="srcData">노드의 데이터</param>
/// <returns>생성 된 노드</returns>
NODE* BT_CreateNode(DATA_TYPE srcData)
{
	NODE* retVal = (NODE*)malloc(sizeof(NODE));
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
void BT_DeallocateNode(NODE** srcNode)
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
void BT_DeallocateTree(NODE** srcRootNode)
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
/// 순회 방법에 따른 대상 트리 출력
/// </summary>
/// <param name="srcRootNode">대상 트리의 최상위 루트 노드</param>
/// <param name="traversalMethod">순회 방법</param>
void BT_DispOrderedTree(NODE* srcRootNode, TRAVERSAL_METHOD traversalMethod)
{
	if (srcRootNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	switch (traversalMethod)
	{
	case TRAVERSAL_METHOD::PREORDER:
		std::cout << srcRootNode->_data << " ";

		if(srcRootNode->_left != NULL)
			BT_DispOrderedTree(srcRootNode->_left, traversalMethod);

		if (srcRootNode->_right != NULL)
			BT_DispOrderedTree(srcRootNode->_right, traversalMethod);
		break;

	case TRAVERSAL_METHOD::INORDER:
		if (srcRootNode->_left != NULL)
			BT_DispOrderedTree(srcRootNode->_left, traversalMethod);

		std::cout << srcRootNode->_data << " ";

		if (srcRootNode->_right != NULL)
			BT_DispOrderedTree(srcRootNode->_right, traversalMethod);
		break;

	case TRAVERSAL_METHOD::POSTORDER:
		if (srcRootNode->_left != NULL)
			BT_DispOrderedTree(srcRootNode->_left, traversalMethod);

		if (srcRootNode->_right != NULL)
			BT_DispOrderedTree(srcRootNode->_right, traversalMethod);

		std::cout << srcRootNode->_data << " ";
		break;
	}
}