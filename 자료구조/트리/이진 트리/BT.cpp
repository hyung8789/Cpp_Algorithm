#include "BT_Core.h"

/// <summary>
/// ���ο� ��� ���� �� ���� �� ��� ��ȯ
/// </summary>
/// <param name="srcData">����� ������</param>
/// <returns>���� �� ���</returns>
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
/// ��� ��忡 �Ҵ� �� �޸� ����
/// </summary>
/// <param name="srcNode">��� ���</param>
void BT_DeallocateNode(Node** srcNode)
{
	if ((*srcNode) != NULL)
	{
		free(*srcNode);
		(*srcNode) = NULL;
	}
}

/// <summary>
/// ��� Ʈ���� �Ҵ� �� ��� ����� �޸� ����
/// </summary>
/// <param name="srcRootNode">��� Ʈ���� �ֻ��� ��Ʈ ���</param>
void BT_DeallocateTree(Node** srcRootNode)
{
	if ((*srcRootNode) != NULL) //���� ��ȸ�� ���� �� ������ ���� 
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
/// ��ȸ ��忡 ���� ��� Ʈ�� ���
/// </summary>
/// <param name="srcRootNode">��� Ʈ���� �ֻ��� ��Ʈ ���</param>
/// <param name="traversalMode">��ȸ ���</param>
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