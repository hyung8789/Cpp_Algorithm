#include "BST_Core.h"

/// <summary>
/// ��� ���� Ž�� Ʈ���� �� ��� ����
/// </summary>
/// <param name="srcRootNode">��� ���� Ž�� Ʈ���� �ֻ��� ��Ʈ ���</param>
/// <param name="srcNewNode">�� ���</param>
void BST_InsertNode(Node** srcRootNode, Node* srcNewNode)
{
	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcRootNode) == NULL) //���� �� ������ ��ġ�� ���
	{
		(*srcRootNode) = srcNewNode;
	}
	else
	{
		if ((*srcRootNode)->data == srcNewNode->data) //�ߺ� �����Ͱ� ���� �� ���
			throw myexception::NOT_ALLOWED_DUPLICATE_DATA_EXCEPTION(std::string(__func__) + std::string(" : Not allowed duplicate data"));

		if ((*srcRootNode)->data > srcNewNode->data) //���� ����� ������ > �����ϰ��� �ϴ� ����� �������� ���
			BST_InsertNode(&((*srcRootNode)->left), srcNewNode); //���� ��� ���� �������� Ž�� ����
		else //���� ����� ������ < �����ϰ��� �ϴ� ����� �������� ���
			BST_InsertNode(&((*srcRootNode)->right), srcNewNode); //���� ��� ���� ���������� Ž�� ����
	}
}

/// <summary>
/// ��� ���� Ž�� Ʈ���� �����ϰ��� �ϴ� ��� �����Ͱ� ���� �� ��� ����
/// </summary>
/// <param name="srcRootNode">��� ���� Ž�� Ʈ���� �ֻ��� ��Ʈ ���</param>
/// <param name="targetData">�����ϰ��� �ϴ� ��� ������</param>
/// <param name="deallocateAfterRemove">���� ��� ��忡 ���� �޸� ���� ���� ����</param>
/// <summary>
void BST_RemoveNode(Node** srcRootNode, const DataType& targetData, bool deallocateAfterRemove)
{
	/***
		< �����ϰ��� �ϴ� ��� �����Ͱ� ���� �� ��� (���� ��� ���)�� �ڽ� ��� ������ ���� ó�� >

		1) ���� ��� ��尡 �ڽ� ��带 ���� ���� ���� ���

			1-1) ���� ��� ����� �θ� ���κ��� ���� ����

			1-2) ���� ��� ��� ����

		2) ���� ��� ��尡 ����, ������ �ڽ� ��� �� �ϳ��� ���� ���� ���

			2-1) ���� ��� ����� �θ� ���κ��� ���� ����

			2-2) ���� ��� ��尡 ������ �ִ� �ڽ� ��带 ���� ��� ��尡 ���� �Ǿ��� ���� �θ� ���� ����

			2-3) ���� ��� ��� ����

		3) ���� ��� ��尡 ����, ������ �ڽ� ��带 ��� ���� ���� ���

			3-1) ���� ��� ����� �θ� ���κ��� ���� ����

			3-2) ���� ��� ����� ������ ���� Ʈ���� ��� �� ���� ���� ��� (����, moveTarget) �� ���� ��� ��尡 ���� �Ǿ��� ���� �θ� ���� ����
			: moveTarget�� �ش� ���� Ʈ������ ���� ���� ����̹Ƿ�, ���� �ڽ� ��尡 �������� ����

				3-2-1) moveTarget�� ������ ���� Ʈ���� ���� �� ���
				: moveTarget�� ������ ���� Ʈ���� ��Ʈ ��带 moveTarget�� �̵� �߻� �� �θ� ���� ����

				3-2-2) moveTarget�� ������ ���� Ʈ���� �������� ���� ���
				: do nothing
			
			3-3) ���� ��� ��� ����

		---

		ex)
										23 (Root)
					10 (Remove Target)					100
			1				16					50
		0		9		12		20

		10�� ���� ��� ��� �� ���, 20�� �θ� ����� 50�� ���ʿ��� 50���� ���� ��尡 �;��ϸ�,
		���� Ʈ���� ���� Ž�� Ʈ���� ���Ǹ� �����ؾ� ��

		1) ���� ��� ��带 ���� �� ���� Ʈ���� ��� �� ���� ���� ����� 0�� ���� ��� ����� ��ġ�� �ű� ���

					0
			1				16
				9		12		20

		=> ���� Ž�� Ʈ���� ���Ǹ� �������� ����

		2) ���� ��� ��带 ���� �� ���� Ʈ���� ��� �� ���� ū ����� 20�� ���� ��� ����� ��ġ�� �ű� ���

					20
			1				16
		0		9		12

		=> ���� Ž�� Ʈ���� ���Ǹ� �������� ����

		3) ���� ��� ��带 ���� �� ���� ���� Ʈ���� ��� �� ���� ū ����� 9�� ���� ��� ����� ��ġ�� �ű� ���

					9
			1				16
		0				12		20

		=> ���� Ž�� Ʈ���� ���Ǹ� ����

		4) ���� ��� ��带 ���� �� ������ ���� Ʈ���� ��� �� ���� ���� ����� 12�� ���� ��� ����� ��ġ�� �ű� ���


					12
			1				16
		0		9				20

		=> ���� Ž�� Ʈ���� ���Ǹ� ����
	***/

	if ((*srcRootNode) == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	Node* removeTargetParentNode = BST_SearchTargetParentNode((*srcRootNode), targetData); //���� ��� ����� �θ� ���
	Node* removeTargetNode = 
		(removeTargetParentNode->left != NULL && removeTargetParentNode->left->data == targetData) ? removeTargetParentNode->left :
		(removeTargetParentNode->right != NULL && removeTargetParentNode->right->data == targetData) ? removeTargetParentNode->right : 
		throw std::logic_error(std::string(__func__) + std::string(" : Unknown err")); //���� ��� ���
	Node* moveTarget = BST_SearchMinNode(removeTargetNode->right); //���� ��� ����� ������ ���� Ʈ���� ��� �� ���� ���� ���


	//TODO : ���� ���� Ȥ�� ���� �� ��
	if ((*srcRootNode)->data > targetData) //���� ����� ������ > �����ϰ��� �ϴ� ��� �������� ���
	{ 
		BST_RemoveNode(&((*srcRootNode)->left), targetData, (*srcRootNode), deallocateAfterRemove); //���� ��� ���� �������� Ž�� ����
	} 
	else if((*srcRootNode)->data < targetData) //���� ����� ������ < �����ϰ��� �ϴ� ��� �������� ���
	{
		BST_RemoveNode(&((*srcRootNode)->right), targetData, (*srcRootNode), deallocateAfterRemove); //���� ��� ���� ���������� Ž�� ����
	}
	else //���� ����� �����Ͱ� �����ϰ��� �ϴ� ��� �����Ϳ� ��ġ�� ���
	{
		Node* removeTargetNode = (*srcRootNode); //���� ��� ���
		//Node* removeTargetNodePrevParentChildNode = NULL; //���� ��� ����� ������ �θ� ��忡�� ���� �Ǿ��� �ڽ� ���
		//TODO : ���� �߻� �� Ʈ���� ��Ʈ ��� ���� �� ���
		// 
		//���� ��� ����� �θ� ���κ��� ���� ����
		removeTargetNodePrevParentChildNode = 
			(removeTargetParentNode->left == removeTargetNode) ? removeTargetParentNode->left : removeTargetParentNode->right;

		if (removeTargetParentNode->left == removeTargetNode)
			removeTargetParentNode->left = NULL;
		else //right == removeTargetNode
			removeTargetParentNode->right = NULL;

		if (removeTargetNode->left != NULL && removeTargetNode->right != NULL) //���� ��� ��尡 ����, ������ �ڽ� ��带 ��� ���� ���� ���
		{
			Node* moveTarget = BST_SearchMinNode(removeTargetNode); //���� ��� ����� ������ ���� Ʈ���� ��� �� ���� ���� ���
			//���� ��� ��尡 ���� �Ǿ��� ���� �θ� ���� ����
				

		}

		//���� ��� ��� ����
		if (deallocateAfterRemove)
			BT_DeallocateNode(&removeTargetNode);
	}
}

/// <summary>
/// ��� ���� Ž�� Ʈ���� ã���� �ϴ� ��� �����Ͱ� ���� �� ��� ��ȯ
/// </summary>
/// <param name="srcRootNode">��� ���� Ž�� Ʈ���� �ֻ��� ��Ʈ ���</param>
/// <param name="targetData">ã���� �ϴ� ��� ������</param>
/// <returns>ã���� �ϴ� ��� �����Ͱ� ���� �� ���</returns>
Node* BST_SearchNode(Node* srcRootNode, const DataType& targetData)
{
	if (srcRootNode == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (srcRootNode->data == targetData) //���� ��尡 ã���� �ϴ� ��� �����Ϳ� ��ġ�� ���
		return srcRootNode;
	else if (srcRootNode->data > targetData) //���� ��� > ã���� �ϴ� ��� �������� ���
		return BST_SearchNode(srcRootNode->left, targetData); //���� ��� ���� �������� Ž�� ����
	else //���� ��� < ã���� �ϴ� ��� �������� ���
		return BST_SearchNode(srcRootNode->right, targetData); //���� ��� ���� ���������� Ž�� ����
}

/// <summary>
/// ��� ���� Ž�� Ʈ���� ã���� �ϴ� ��� �����Ͱ� ���� �� ����� ���� �θ� ��� ��ȯ
/// </summary>
/// <param name="srcRootNode">��� ���� Ž�� Ʈ���� �ֻ��� ��Ʈ ���</param>
/// <param name="targetData">ã���� �ϴ� ��� ������</param>
/// <param name="targetParentNode">ã���� �ϴ� ��� �����Ͱ� ���� �� ����� ���� �θ� ���</param>
/// <returns>ã���� �ϴ� ��� �����Ͱ� ���� �� ����� ���� �θ� ���</returns>
Node* BST_SearchTargetParentNode(Node* srcRootNode, const DataType& targetData, Node* targetParentNode)
{
	if (srcRootNode == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (srcRootNode->data == targetData) //���� ��尡 ã���� �ϴ� ��� �����Ϳ� ��ġ�� ���
		return targetParentNode;
	else if (srcRootNode->data > targetData) //���� ��� > ã���� �ϴ� ��� �������� ���
		return BST_SearchTargetParentNode(srcRootNode->left, targetData, srcRootNode); //���� ��� ���� �������� Ž�� ����
	else //���� ��� < ã���� �ϴ� ��� �������� ���
		return BST_SearchTargetParentNode(srcRootNode->right, targetData, srcRootNode); //���� ��� ���� ���������� Ž�� ����
}

/// <summary>
/// ��� ���� Ž�� Ʈ���� �ּҰ��� �����Ͱ� ���� �� ��� ��ȯ
/// </summary>
/// <param name="srcRootNode">��� ���� Ž�� Ʈ���� �ֻ��� ��Ʈ ���</param>
/// <returns>�ּҰ��� �����Ͱ� ���� �� ���</returns>
Node* BST_SearchMinNode(Node* srcRootNode)
{
	if (srcRootNode == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (srcRootNode->left != NULL) //���� ����� ���� ���� Ʈ���� ���� �� ���
		return BST_SearchMinNode(srcRootNode->left);
	else //���� ����� ���� ���� Ʈ���� �������� ���� ���
		return srcRootNode;
}
