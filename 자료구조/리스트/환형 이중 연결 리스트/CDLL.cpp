#include "CDLL_Core.h"

/// <summary>
/// ���ο� ��� ���� �� ���� �� ��� ��ȯ
/// </summary>
/// <param name="srcData">����� ������</param>
/// <returns>���� �� ���</returns>
Node* CDLL_CreateNode(DataType srcData)
{
	Node* retVal = (Node*)malloc(sizeof(Node));

	if (retVal != NULL)
	{
		retVal->data = srcData;
		retVal->prev = retVal->next = NULL;
	}

	return retVal;
}

/// <summary>
/// ��� ��忡 �Ҵ� �� �޸� ����
/// </summary>
/// <param name="srcNode">��� ���</param>
void CDLL_DeallocateNode(Node** srcNode)
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
void CDLL_DeallocateNodeList(Node** srcList)
{
	Node* oldHead = (*srcList); //���� ��� ���

	while ((*srcList) != NULL)
	{
		Node* tmp = (*srcList); //���� �� ���

		(*srcList) = (*srcList)->next; //��� ��带 ���� ���� �̵� �� ����
		CDLL_DeallocateNode(&tmp);

		if ((*srcList) == oldHead) //���� ��� ���� ���ƿ� ���
			break;
	}

	(*srcList) = NULL;
}

/// <summary>
/// ��� ����Ʈ�� ���� �� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="srcNewNode">����Ʈ�� ���� �����ϰ��� �ϴ� �� ���</param>
void CDLL_AppendNode(Node** srcList, Node* srcNewNode)
{
	if (srcNewNode != NULL)
	{
		if ((*srcList) == NULL) //������� ���
		{
			(*srcList) = srcNewNode; //��� ��� �Ҵ�
			(*srcList)->prev = (*srcList)->next = (*srcList);
		}
		else //���� �� ��� ����
		{
			Node* tail = (*srcList)->prev; //���� ���

			CDLL_InsertNodeAfter(tail, srcNewNode);
		}
	}
}

/// <summary>
/// ��� ����Ʈ���� ��� ��� ���� ����� Ư�� ��ġ�� ��� ��ȯ
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="position">��� ��� ���� ����� Ư�� ��ġ</param>
/// <returns>��� ��� ���� ����� Ư�� ��ġ�� ���</returns>
Node* CDLL_GetNodeAt(Node** srcList, NodePositionType position)
{
	Node* retVal = (*srcList); //���� ���

	while (retVal != NULL && (--position) >= 0) //��ȯ ��� ��� ��ġ���� �̵�
	{
		retVal = retVal->next;
	}

	return retVal;
}

/// <summary>
/// ��� ����Ʈ���� ��� ��� ���� ���� �� ����� Ư�� ��ġ�� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="position">��� ��� ���� ���� �� ����� Ư�� ��ġ</param>
/// <param name="deallocateAfterRemove">���� ��� ��忡 ���� �޸� ���� ���� ����</param>
void CDLL_RemoveNodeAt(Node** srcList, NodePositionType position, bool deallocateAfterRemove)
{
	Node* current = (*srcList); //���� ���

	while (current != NULL && (--position) >= 0) //���� ��� ��� ��ġ���� �̵�
	{
		current = current->next;
	}

	CDLL_RemoveNode(srcList, current, deallocateAfterRemove);
}

/// <summary>
/// ��� ����Ʈ���� ���� ��� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="srcTargetNode">���� ��� ���</param>
/// <param name="deallocateAfterRemove">���� ��� ��忡 ���� �޸� ���� ���� ����</param>
void CDLL_RemoveNode(Node** srcList, Node* srcTargetNode, bool deallocateAfterRemove)
{
	/***
		�ܼ� ���� ���� ����Ʈ�� ���Ͽ�, ���� ������ ����Ǿ������Ƿ�,
		���� ��� ����� ���� ��� Ȥ�� ���� ����� ���� ���ο� ������� ó��
		---
		1) TH : ���� ��� ����� ��� ��� ���� (T : ��� ���, F : �߰� ��� Ȥ�� ���� ���)
		2) D : �̿� ���� ���� ��� ��� ���� �� ���� �۾�

		TH | D
		T	1) ��� ��带 ���� ��� ����� ���� ���� ����
			2) ���� ��� ����� ���� ����� ������ ���� ����� ���� ���� ����
			3) ���� ��� ����� ���� ����� ������ ���� ����� ���� ���� ����
			4) ���� ��� ����� ����, ���� ���� ���� �� ���� ��� ��� ����

		F	1) ���� ��� ����� ���� ����� ������ ���� ��� ����� ���� ���� ����
			2) ���� ��� ����� ���� ����� ������ ���� ��� ����� ���� ���� ����
			3) ���� ��� ����� ����, ���� ���� ���� �� ���� ��� ��� ����
	***/

	if (srcTargetNode != NULL)
	{
		if (srcTargetNode == (*srcList)) //���� ��� ��尡 ��� ����� ���
		{
			(*srcList) = srcTargetNode->next; //��� ��带 ���� ��� ����� ���� ���� ����
		}

		srcTargetNode->next->prev = srcTargetNode->prev; //���� ��� ����� ���� ����� ������ ���� ����� ���� ���� ����
		srcTargetNode->prev->next = srcTargetNode->next; //���� ��� ����� ���� ����� ������ ���� ����� ���� ���� ����
		srcTargetNode->prev = NULL;
		srcTargetNode->next = NULL;

		if (deallocateAfterRemove)
		{
			CDLL_DeallocateNode(&srcTargetNode);
		}
	}
}

/// <summary>
/// ��� ����Ʈ�� �� �տ� �� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="srcNewNode">�����ϰ��� �ϴ� �� ���</param>
void CDLL_InsertNewHead(Node** srcList, Node* srcNewNode)
{
	if (srcNewNode != NULL)
	{
		if ((*srcList) == NULL)
		{
			(*srcList) = srcNewNode;
			(*srcList)->prev = (*srcList)->next = (*srcList);
		}
		else //��� ����� �տ� ����
		{
			CDLL_InsertNodeBefore(srcList, (*srcList), srcNewNode);
		}
	}
}

/// <summary>
/// ��� ����� �ڿ� �� ��� ����
/// </summary>
/// <param name="srcTargetNode">��� ���</param>
/// <param name="srcNewNode">�����ϰ��� �ϴ� �� ���</param>
void CDLL_InsertNodeAfter(Node* srcTargetNode, Node* srcNewNode)
{
	if (srcTargetNode != NULL && srcNewNode != NULL)
	{
		srcNewNode->prev = srcTargetNode; //�� ����� ������ ��� ���� ����
		srcNewNode->next = srcTargetNode->next; //�� ����� ������ ��� ����� ���� ���� ����
		srcTargetNode->next->prev = srcNewNode; //��� ����� ���� ����� ������ �� ���� ����
		srcTargetNode->next = srcNewNode; //��� ����� ������ �� ���� ����
	}
}

/// <summary>
/// ��� ����Ʈ�� ��� ����� �տ� �� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="srcTargetNode">��� ���</param>
/// <param name="srcNewNode">�����ϰ��� �ϴ� �� ���</param>
void CDLL_InsertNodeBefore(Node** srcList, Node* srcTargetNode, Node* srcNewNode)
{
	/***
		1) TH : ��� ����� ��� ��� ���� (T : ��� ���, F : �߰� ��� Ȥ�� ���� ���)
		2) D : �̿� ���� �����ϰ��� �ϴ� �� ��� ���� �� ���� �۾�

		TH | D
		T	1) �� ����� ������ ��� ���� ����
			2) �� ����� ������ ��� ����� ���� ���� ����
			3) ��� ����� ���� ����� ������ �� ���� ����
			4) ��� ����� ������ �� ���� ����
			5) �� ��带 ��� ���� ����

		F	1) �� ����� ������ ��� ���� ����
			2) �� ����� ������ ��� ����� ���� ���� ����
			3) ��� ����� ���� ����� ������ �� ���� ����
			4) ��� ����� ������ �� ���� ����
	***/

	if (srcTargetNode != NULL && srcNewNode != NULL)
	{
		srcNewNode->next = srcTargetNode; //�� ����� ������ ��� ���� ����
		srcNewNode->prev = srcTargetNode->prev; //�� ����� ������ ��� ����� ���� ���� ����
		srcTargetNode->prev->next = srcNewNode; //��� ����� ���� ����� ������ �� ���� ����
		srcTargetNode->prev = srcNewNode; //��� ����� ������ �� ���� ����

		if (srcTargetNode == (*srcList)) //T
		{
			(*srcList) = srcNewNode; //��� ��� ����
		}
	}
}

/// <summary>
/// ��� ����Ʈ�� ��ü ����� �� ��ȯ
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <returns>��� ����Ʈ�� ��ü ����� ��</returns>
NodePositionType CDLL_GetTotalNodeCount(Node** srcList)
{
	NodePositionType totalNodeCount = 0; //��ü ����� ��
	Node* current = (*srcList); //���� ���

	while (current != NULL)
	{
		current = current->next;
		totalNodeCount++;

		if (current == (*srcList)) //��� ���� ���ƿ� ���
			break;
	}

	return totalNodeCount;
}

/// <summary>
/// ��� ����Ʈ�� ��ü ��忡 ���� ������ ���
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
void CDLL_DispNodeList(Node** srcList)
{
	NodePositionType currentPosition = 0; //���� ����� ��ġ
	Node* current = (*srcList); //���� ���

	while (current != NULL)
	{
		std::cout << "List [" << currentPosition << "] : " << current->data << std::endl;

		current = current->next;
		currentPosition++;

		if (current == (*srcList)) //��� ���� ���ƿ� ���
			break;
	}
}