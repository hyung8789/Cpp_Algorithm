#include "SLL_Core.h"

/// <summary>
/// ���ο� ��� ���� �� ���� �� ��� ��ȯ
/// </summary>
/// <param name="srcData">����� ������</param>
/// <returns>���� �� ���</returns>
Node* SLL_CreateNode(DataType srcData)
{
	Node* retVal = (Node*)malloc(sizeof(Node));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->data = srcData;
	retVal->next = NULL;

	return retVal;
}

/// <summary>
/// ��� ��忡 �Ҵ� �� �޸� ����
/// </summary>
/// <param name="srcNode">��� ���</param>
void SLL_DeallocateNode(Node** srcNode)
{
	if ((*srcNode) != NULL)
	{
		free(*srcNode);
		(*srcNode) = NULL;
	}
}

/// <summary>
/// ��� ����Ʈ�� �Ҵ� �� �޸� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
void SLL_DeallocateNodeList(Node** srcList)
{
	while ((*srcList) != NULL)
	{
		Node* tmp = (*srcList); //���� �� ���

		(*srcList) = (*srcList)->next; //��� ��带 ���� ���� �̵� �� ����
		SLL_DeallocateNode(&tmp);
	}

	(*srcList) = NULL;
}

/// <summary>
/// ��� ����Ʈ�� ���� �� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="srcNewNode">����Ʈ�� ���� �����ϰ��� �ϴ� �� ���</param>
void SLL_AppendNode(Node** srcList, Node* srcNewNode)
{
	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcList) == NULL)
	{
		(*srcList) = srcNewNode; //��� ��� �Ҵ�
	}
	else //���� �� ��� ����
	{
		Node* tail = (*srcList); //���� ���

		while (tail->next != NULL) //������ �̵�
		{
			tail = tail->next;
		}

		SLL_InsertNodeAfter(tail, srcNewNode);
	}
}

/// <summary>
/// ��� ����Ʈ���� ��� ��� ���� ����� Ư�� ��ġ�� ��� ��ȯ
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="position">��� ��� ���� ����� Ư�� ��ġ</param>
/// <returns>��� ��� ���� ����� Ư�� ��ġ�� ���</returns>
Node* SLL_GetNodeAt(Node** srcList, NodePositionType position)
{
	if (position < 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	Node* retVal = (*srcList); //���� ���

	while (retVal != NULL && (--position) >= 0) //��ȯ ��� ��� ��ġ���� �̵�
	{
		retVal = retVal->next;
	}

	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not found"));

	return retVal;
}

/// <summary>
/// ��� ����Ʈ���� ��� ��� ���� ���� �� ����� Ư�� ��ġ�� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="position">��� ��� ���� ���� �� ����� Ư�� ��ġ</param>
/// <param name="deallocateAfterRemove">���� ��� ��忡 ���� �޸� ���� ���� ����</param>
void SLL_RemoveNodeAt(Node** srcList, NodePositionType position, bool deallocateAfterRemove)
{
	/***
		���� ���� ����Ʈ���� ���� ��� ��忡�� ���� ���� ��� ������ �Ұ����ϹǷ�,
		���� ��� ����� ���� ������ �̵��Ͽ�, ���� ��� ����� ���� ���� ������ �����Ͽ��� �Ѵ�.
		---
		�̿� ����, �ܼ� ���� ���� ����Ʈ Ȥ�� ȯ�� ���� ���� ����Ʈ�� �ٸ���
		���� �������� RemoveNodeAt�� RemoveNode ���� �ߺ� �ּ�ȭ�� ���� ���� ���踦 ���� �� �� ����.
	***/

	if (position < 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (position == 0) //��� ��尡 ���� ����� ���
	{
		Node* tmp = (*srcList); //���� ��� ���
		(*srcList) = (*srcList)->next; //��� ����� ���� ���� ��� ��� ����
		tmp->next = NULL;

		if (deallocateAfterRemove)
		{
			SLL_DeallocateNode(&tmp);
		}
	}
	else //���� ����� ���� ��尡 ���� ��� ����� ������ Ž��
	{
		Node* current = (*srcList); //���� ���

		while (current != NULL && (--position) >= 1) //���� ��� ����� ���� ��� ��ġ���� �̵�
		{
			current = current->next;
		}

		if (current == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not found"));

		Node* tmp = current->next; //���� ��� ���

		current->next = tmp->next; //���� ����� ������ ���� ��� ����� ���� ���� ����
		tmp->next = NULL;

		if (deallocateAfterRemove)
		{
			SLL_DeallocateNode(&tmp);
		}
	}
}

/// <summary>
/// ��� ����Ʈ���� ���� ��� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="srcTargetNode">���� ��� ���</param>
/// <param name="deallocateAfterRemove">���� ��� ��忡 ���� �޸� ���� ���� ����</param>
void SLL_RemoveNode(Node** srcList, Node* srcTargetNode, bool deallocateAfterRemove)
{
	if (srcTargetNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (srcTargetNode == (*srcList)) //���� ��� ��尡 ��� ����� ���
	{
		(*srcList) = srcTargetNode->next; //���� ��� ����� ���� ���� ��� ��� ����
		srcTargetNode->next = NULL;

		if (deallocateAfterRemove)
		{
			SLL_DeallocateNode(&srcTargetNode);
		}
	}
	else //���� ����� ���� ��尡 ���� ��� ����϶����� Ž��
	{
		Node* current = (*srcList); //���� ���

		while (current != NULL && current->next != srcTargetNode) //���� ��� ����� ���� ��� ��ġ���� �̵�
		{
			current = current->next;
		}

		if (current == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not found"));

		current->next = srcTargetNode->next; //���� ����� ������ ���� ��� ����� ���� ���� ����
		srcTargetNode->next = NULL;

		if (deallocateAfterRemove)
		{
			SLL_DeallocateNode(&srcTargetNode);
		}
	}
}

/// <summary>
/// ��� ����Ʈ�� �� �տ� �� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="srcNewNode">�����ϰ��� �ϴ� �� ���</param>
void SLL_InsertNewHead(Node** srcList, Node* srcNewNode)
{
	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcList) == NULL)
	{
		(*srcList) = srcNewNode;
	}
	else //��� ����� �տ� ����
	{
		SLL_InsertNodeBefore(srcList, (*srcList), srcNewNode);
	}
}

/// <summary>
/// ��� ����� �ڿ� �� ��� ����
/// </summary>
/// <param name="srcTargetNode">��� ���</param>
/// <param name="srcNewNode">�����ϰ��� �ϴ� �� ���</param>
void SLL_InsertNodeAfter(Node* srcTargetNode, Node* srcNewNode)
{
	if (srcTargetNode == NULL || srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	srcNewNode->next = srcTargetNode->next; //�� ����� ������ ��� ����� ���� ���� ����
	srcTargetNode->next = srcNewNode; //��� ����� ������ �� ���� ����
}

/// <summary>
/// ��� ����Ʈ�� ��� ����� �տ� �� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="srcTargetNode">��� ���</param>
/// <param name="srcNewNode">�����ϰ��� �ϴ� �� ���</param>
void SLL_InsertNodeBefore(Node** srcList, Node* srcTargetNode, Node* srcNewNode)
{
	/***
		1) TH : ��� ����� ��� ��� ���� (T : ��� ���, F : �߰� ��� Ȥ�� ���� ���)
		2) D : �̿� ���� �����ϰ��� �ϴ� �� ��� ���� �� ���� �۾�

		TH | D
		T	1) �� ����� ������ ���� ��� ���� ����
			2) �� ��带 ��� ���� ����

		F	1) ��� ��忡�� ���� ���� ��� ������ �Ұ����ϹǷ�, ��� ����� ���� ������ �̵�
			2) ��� ����� ���� ����� ������ �� ���� ����
			3) �� ����� ������ ��� ���� ����
	***/

	if (srcTargetNode == NULL || srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (srcTargetNode == (*srcList)) //T
	{
		srcNewNode->next = (*srcList); //�� ����� ������ ���� ��� ���� ����
		(*srcList) = srcNewNode; //��� ��� ����
	}
	else //F
	{
		Node* current = (*srcList); //���� ���

		while (current != NULL && current->next != srcTargetNode) //���� ����� ���� ��尡 ���� ��� ����϶����� Ž��
		{
			current = current->next;
		}

		if (current == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not found"));

		current->next = srcNewNode; //���� ����� ������ �� ���� ����
		srcNewNode->next = srcTargetNode; //�� ����� ������ ��� ���� ����
	}
}

/// <summary>
/// ��� ����Ʈ�� ��ü ����� �� ��ȯ
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <returns>��� ����Ʈ�� ��ü ����� ��</returns>
NodePositionType SLL_GetTotalNodeCount(Node** srcList)
{
	NodePositionType totalNodeCount = 0; //��ü ����� ��
	Node* current = (*srcList); //���� ���

	while (current != NULL)
	{
		current = current->next;
		totalNodeCount++;
	}

	return totalNodeCount;
}

/// <summary>
/// ��� ����Ʈ�� ��ü ��忡 ���� ������ ���
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
void SLL_DispNodeList(Node** srcList)
{
	NodePositionType currentPosition = 0; //���� ����� ��ġ
	Node* current = (*srcList); //���� ���

	while (current != NULL)
	{
		std::cout << "List [" << currentPosition << "] : " << current->data << std::endl;

		current = current->next;
		currentPosition++;
	}
}