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

	retVal->data = srcData;
	retVal->left = retVal->right = NULL;

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
		if ((*srcRootNode)->left != NULL)
			BT_DeallocateTree(&((*srcRootNode)->left));

		if((*srcRootNode)->right != NULL)
			BT_DeallocateTree(&((*srcRootNode)->right));

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
		std::cout << srcRootNode->data << " ";

		if(srcRootNode->left != NULL)
			BT_DispOrderedTree(srcRootNode->left, traversalMode);

		if (srcRootNode->right != NULL)
			BT_DispOrderedTree(srcRootNode->right, traversalMode);
		break;

	case TRAVERSAL_MODE::INORDER:
		if (srcRootNode->left != NULL)
			BT_DispOrderedTree(srcRootNode->left, traversalMode);

		std::cout << srcRootNode->data << " ";

		if (srcRootNode->right != NULL)
			BT_DispOrderedTree(srcRootNode->right, traversalMode);
		break;

	case TRAVERSAL_MODE::POSTORDER:
		if (srcRootNode->left != NULL)
			BT_DispOrderedTree(srcRootNode->left, traversalMode);

		if (srcRootNode->right != NULL)
			BT_DispOrderedTree(srcRootNode->right, traversalMode);

		std::cout << srcRootNode->data << " ";
		break;
	}
}