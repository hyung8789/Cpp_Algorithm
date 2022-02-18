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
		if ((*srcRootNode)->_data == srcNewNode->_data) //�ߺ� �����Ͱ� ���� �� ���
			throw myexception::NOT_ALLOWED_DUPLICATE_DATA_EXCEPTION(std::string(__func__) + std::string(" : Not allowed duplicate data"));

		if ((*srcRootNode)->_data > srcNewNode->_data) //���� ����� ������ > �����ϰ��� �ϴ� ����� �������� ���
			BST_InsertNode(&((*srcRootNode)->_left), srcNewNode); //���� ��� ���� �������� Ž�� ����
		else //���� ����� ������ < �����ϰ��� �ϴ� ����� �������� ���
			BST_InsertNode(&((*srcRootNode)->_right), srcNewNode); //���� ��� ���� ���������� Ž�� ����
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

		! ��Ʈ ��忡 ���� ������ �߻� �� ���, ������ ó�� �䱸

		1) ���� ��� ��尡 �ڽ� ��带 ���� ���� ���� ���

			1-1) ���� ��� ����� �θ� ���κ��� ���� ����

			1-2) ���� ��� ��� ����

		2) ���� ��� ��尡 ����, ������ �ڽ� ��� �� �ϳ��� ���� ���� ���

			2-1) ���� ��� ��尡 ��Ʈ ����� ���
			: ���� ��� ��尡 ������ �ִ� �ڽ� ��带 �ֻ��� ��Ʈ ���� ����

			2-2) ���� ��� ��尡 ��Ʈ ��尡 �ƴ� ���
			: ���� ��� ��尡 ������ �ִ� �ڽ� ��带 ���� ��� ��尡 ���� �Ǿ��� ���� �θ� ���� ����

			2-3) ���� ��� ��� ����

		3) ���� ��� ��尡 ����, ������ �ڽ� ��带 ��� ���� ���� ���
			
			3-1) ���� ��� ��尡 ��Ʈ ����� ���

				3-1-1) �̵� ��� ��� (���� ��� ����� ������ ���� Ʈ���� ��� �� ���� ���� ���) �� �ֻ��� ��Ʈ ���� ����
				: �̵� ��� ���� �ش� ���� Ʈ������ ���� ���� ����̹Ƿ�, ���� �ڽ� ��尡 �������� ����

			3-2) ���� ��� ��尡 ��Ʈ ��尡 �ƴ� ���

				3-2-1) �̵� ��� ��� (���� ��� ����� ������ ���� Ʈ���� ��� �� ���� ���� ���) �� ���� ��� ��尡 ���� �Ǿ��� ���� �θ� ���� ����
				: �̵� ��� ���� �ش� ���� Ʈ������ ���� ���� ����̹Ƿ�, ���� �ڽ� ��尡 �������� ����

			3-3) �̵� ��� ����� �����ִ� ������ ���� Ʈ�� ���� ���ο� ����,

				3-3-1) �̵� ��� ����� ������ ���� Ʈ���� ���� �� ���
				: �̵� ��� ����� ������ ���� Ʈ���� ��Ʈ ��带 �̵� ��� ��尡 �̵��� �߻��ϱ� �� ����Ǿ��� ���� �θ� ���� ����

				3-3=2) �̵� ��� ����� ������ ���� Ʈ���� �������� ���� ���
				: �̵� ��� ��尡 �̵��� �߻��ϱ� �� ���� �Ǿ��� ���� �θ� ���κ����� ���� ����

			3-4) �̵� ��� ����� ���� �� ������ �ڽ� ��带 ���� ��� ����� ���� �� ������ �ڽ� ���� ����

			3-5) ���� ��� ��� ����
	***/
	
	if ((*srcRootNode) == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	std::tuple<Node*, Node*> removeTargetNodeWithParentNodeTuple = 
		BST_SearchNodeWithParentNode((*srcRootNode), targetData); //���� ��� ���� ���� ��� ����� ���� �θ� ��尡 ���� �� Ʃ��
	Node* removeTargetNode = std::get<0>(removeTargetNodeWithParentNodeTuple); //���� ��� ���
	Node* removeTargetParentNode = std::get<1>(removeTargetNodeWithParentNodeTuple); //���� ��� ����� �θ� ���

	std::tuple<Node*, Node*> moveTargetNodeWithParentNodeTuple =
		BST_SearchMinNodeWithParentNode(removeTargetNode->_right); //�̵� ��� ���� �̵� ��� ����� ���� �θ� ��尡 ���� �� Ʃ��
	Node* moveTargetNode = std::get<0>(moveTargetNodeWithParentNodeTuple); //�̵� ��� ��� (���� ��� ����� ������ ���� Ʈ���� ��� �� ���� ���� ���)
	Node* moveTargetParentNode = std::get<1>(moveTargetNodeWithParentNodeTuple); //�̵� ��� ����� �θ� ���

	const unsigned char REMOVE_TARGET_NODE_LEFT_CHILD_EXISTS = (0x1) << 0; //���� ��� ����� ���� �ڽ� ��� ���� ���� (0001)
	const unsigned char REMOVE_TARGET_NODE_RIGHT_CHILD_EXISTS = (0x1) << 1; //���� ��� ����� ������ �ڽ� ��� ���� ���� (0010)
	const unsigned char REMOVE_TARGET_NODE_STATE =
		((removeTargetNode->_left != NULL) ? REMOVE_TARGET_NODE_LEFT_CHILD_EXISTS : FALSE) |
		((removeTargetNode->_right != NULL) ? REMOVE_TARGET_NODE_RIGHT_CHILD_EXISTS : FALSE); //���� ��� ��� ���� (���� �ڽ� �� ������ �ڽ� ���� ���ο� ����)

	switch (REMOVE_TARGET_NODE_STATE & (REMOVE_TARGET_NODE_LEFT_CHILD_EXISTS | REMOVE_TARGET_NODE_RIGHT_CHILD_EXISTS)) //���� ��� ��� ���¿� ����
	{
	case FALSE: //���� ��� ��尡 �ڽ� ��带 ���� ���� ���� ���
		(removeTargetParentNode->_left == removeTargetNode) ?
			removeTargetParentNode->_left = NULL : 
			removeTargetParentNode->_right = NULL; //���� ��� ����� �θ� ���κ��� ���� ����

		break;

	case (REMOVE_TARGET_NODE_LEFT_CHILD_EXISTS | REMOVE_TARGET_NODE_RIGHT_CHILD_EXISTS): //���� ��� ��尡 ����, ������ �ڽ� ��带 ��� ���� ���� ���
		if (removeTargetNode == (*srcRootNode)) //���� ��� ��尡 ��Ʈ ����� ���
			(*srcRootNode) = moveTargetNode; //�̵� ��� ��带 �ֻ��� ��Ʈ ���� ����
		else //���� ��� ��尡 ��Ʈ ��尡 �ƴ� ���
			(removeTargetParentNode->_left == removeTargetNode) ?
				removeTargetParentNode->_left = moveTargetNode :
				removeTargetParentNode->_right = moveTargetNode; //�̵� ��� ��带 ���� ��� ��尡 ���� �Ǿ��� ���� �θ� ���� ����

		if (moveTargetNode->_right != NULL) //�̵� ��� ����� ������ ���� Ʈ���� ���� �� ���
			(moveTargetParentNode->_left == moveTargetNode) ?
				moveTargetParentNode->_left = moveTargetNode->_right :
				moveTargetParentNode->_right = moveTargetNode->_right; //�̵� ��� ����� ������ ���� Ʈ���� ��Ʈ ��带 �̵� ��� ��尡 �̵��� �߻��ϱ� �� ����Ǿ��� ���� �θ� ���� ����
		else //�̵� ��� ����� ������ ���� Ʈ���� �������� ���� ��� 
			(moveTargetParentNode->_left == moveTargetNode) ?
				moveTargetParentNode->_left = NULL :
				moveTargetParentNode->_right = NULL; //�̵� ��� ��尡 �̵��� �߻��ϱ� �� ���� �Ǿ��� ���� �θ� ���κ����� ���� ����

		//�̵� ��� ����� ���� �� ������ �ڽ� ��带 ���� ��� ����� ���� �� ������ �ڽ� ���� ����
		moveTargetNode->_left = removeTargetNode->_left;
		moveTargetNode->_right = removeTargetNode->_right;

		break;

	default: //���� ��� ��尡 ����, ������ �ڽ� ��� �� �ϳ��� ���� ���� ���
		if (removeTargetNode == (*srcRootNode)) //���� ��� ��尡 ��Ʈ ����� ���
			(*srcRootNode) = (removeTargetNode->_left != NULL) ? 
				removeTargetNode->_left :
				removeTargetNode->_right; //���� ��� ��尡 ������ �ִ� �ڽ� ��带 �ֻ��� ��Ʈ ���� ����
		else //���� ��� ��尡 ��Ʈ ��尡 �ƴ� ���
		{
			//���� ��� ��尡 ������ �ִ� �ڽ� ��带 ���� ��� ��尡 ���� �Ǿ��� ���� �θ� ���� ����
			if (REMOVE_TARGET_NODE_STATE & REMOVE_TARGET_NODE_LEFT_CHILD_EXISTS) //���� ��� ��尡 ���� �ڽ��� ���� �� ���
				(removeTargetParentNode->_left == removeTargetNode) ?
					removeTargetParentNode->_left = removeTargetNode->_left :
					removeTargetParentNode->_right = removeTargetNode->_left;
			else if (REMOVE_TARGET_NODE_STATE & REMOVE_TARGET_NODE_RIGHT_CHILD_EXISTS) //���� ��� ��尡 ������ �ڽ��� ���� �� ���
				(removeTargetParentNode->_left == removeTargetNode) ?
					removeTargetParentNode->_left = removeTargetNode->_right :
					removeTargetParentNode->_right = removeTargetNode->_right;
			else
				throw std::logic_error(std::string(__func__) + std::string(" : Invalid State"));
		}

		break;
	}

	if (deallocateAfterRemove)
		BT_DeallocateNode(&removeTargetNode);
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

	if (srcRootNode->_data == targetData) //���� ��尡 ã���� �ϴ� ��� �����Ϳ� ��ġ�� ���
		return srcRootNode;
	else if (srcRootNode->_data > targetData) //���� ��� > ã���� �ϴ� ��� �������� ���
		return BST_SearchNode(srcRootNode->_left, targetData); //���� ��� ���� �������� Ž�� ����
	else //���� ��� < ã���� �ϴ� ��� �������� ���
		return BST_SearchNode(srcRootNode->_right, targetData); //���� ��� ���� ���������� Ž�� ����
}

/// <summary>
/// ��� ���� Ž�� Ʈ���� ã���� �ϴ� ��� �����Ͱ� ���� �� ���� �ش� ����� ���� �θ� ��尡 ���� �� Ʃ�� ��ȯ
/// </summary>
/// <param name="srcRootNode">��� ���� Ž�� Ʈ���� �ֻ��� ��Ʈ ���</param>
/// <param name="targetData">ã���� �ϴ� ��� ������</param>
/// <param name="targetParentNode">ã���� �ϴ� ��� �����Ͱ� ���� �� ����� ���� �θ� ���</param>
/// <returns>��� �����Ͱ� ���� �� ���� �ش� ����� ���� �θ� ��尡 ���� �� Ʃ��
/// (pos 0 : ��� �����Ͱ� ���� �� ���, pos 1 : �ش� ����� ���� �θ� ��� (�������� ���� ��� pos 0�� ����)</returns>
std::tuple<Node*, Node*> BST_SearchNodeWithParentNode(Node* srcRootNode, const DataType& targetData, Node* targetParentNode)
{
	if (srcRootNode == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (srcRootNode->_data == targetData) //���� ��尡 ã���� �ϴ� ��� �����Ϳ� ��ġ�� ���
	{
		if (targetParentNode != NULL) //���� ����� ���� �θ� ��尡 ���� �� ���
			return std::make_tuple(srcRootNode, targetParentNode);
		else //���� ����� ���� �θ� ��尡 �������� ���� ���
			return std::make_tuple(srcRootNode, srcRootNode);
	}
	if (srcRootNode->_data > targetData) //���� ��� > ã���� �ϴ� ��� �������� ���
		return BST_SearchNodeWithParentNode(srcRootNode->_left, targetData, srcRootNode); //���� ��� ���� �������� Ž�� ����
	else //���� ��� < ã���� �ϴ� ��� �������� ���
		return BST_SearchNodeWithParentNode(srcRootNode->_right, targetData, srcRootNode); //���� ��� ���� ���������� Ž�� ����
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

	if (srcRootNode->_left != NULL) //���� ����� ���� ���� Ʈ���� ���� �� ���
		return BST_SearchMinNode(srcRootNode->_left);
	else //���� ����� ���� ���� Ʈ���� �������� ���� ���
		return srcRootNode;
}

/// <summary>
///	��� ���� Ž�� Ʈ���� �ּҰ��� �����Ͱ� ���� �� ���� �ش� ����� ���� �θ� ��尡 ���� �� Ʃ�� ��ȯ
/// </summary>
/// <param name="srcRootNode">��� ���� Ž�� Ʈ���� �ֻ��� ��Ʈ ���</param>
/// <param name="targetParentNode">��� ���� Ž�� Ʈ���� �ּҰ��� �����Ͱ� ���� �� ����� ���� �θ� ���</param>
/// <returns>�ּҰ��� �����Ͱ� ���� �� ���� �ش� ����� ���� �θ� ��尡 ���� �� Ʃ��
/// (pos 0 : �ּҰ��� �����Ͱ� ���� �� ���, pos 1 : �ش� ����� ���� �θ� ��� (�������� ���� ��� pos 0�� ����)</returns>
std::tuple<Node*, Node*> BST_SearchMinNodeWithParentNode(Node* srcRootNode, Node* targetParentNode)
{
	if (srcRootNode == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (srcRootNode->_left != NULL) //���� ����� ���� ���� Ʈ���� ���� �� ���
	{
		return BST_SearchMinNodeWithParentNode(srcRootNode->_left, srcRootNode);
	}
	else //���� ����� ���� ���� Ʈ���� �������� ���� ���
	{
		if (targetParentNode != NULL) //���� ����� ���� �θ� ��尡 ���� �� ���
			return std::make_tuple(srcRootNode, targetParentNode);
		else //���� ����� ���� �θ� ��尡 �������� ���� ���
			return std::make_tuple(srcRootNode, srcRootNode);
	}
}