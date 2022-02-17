#include "DLL_Core.h"

/// <summary>
/// ���ο� ��� ���� �� ���� �� ��� ��ȯ
/// </summary>
/// <param name="srcData">����� ������</param>
/// <returns>���� �� ���</returns>
Node* DLL_CreateNode(DataType srcData)
{
	Node* retVal = (Node*)malloc(sizeof(Node));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->data = srcData;
	retVal->prev = retVal->next = NULL;

	return retVal;
}

/// <summary>
/// ��� ��忡 �Ҵ� �� �޸� ����
/// </summary>
/// <param name="srcNode">��� ���</param>
void DLL_DeallocateNode(Node** srcNode)
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
void DLL_DeallocateNodeList(Node** srcList)
{
	while ((*srcList) != NULL)
	{
		Node* tmp = (*srcList); //���� �� ���

		(*srcList) = (*srcList)->next; //��� ��带 ���� ���� �̵� �� ����
		DLL_DeallocateNode(&tmp);
	}

	(*srcList) = NULL;
}

/// <summary>
/// ��� ����Ʈ�� ���� �� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="srcNewNode">����Ʈ�� ���� �����ϰ��� �ϴ� �� ���</param>
void DLL_AppendNode(Node** srcList, Node* srcNewNode)
{
	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcList) == NULL) //������� ���
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

		DLL_InsertNodeAfter(tail, srcNewNode);
	}
}

/// <summary>
/// ��� ����Ʈ���� ��� ��� ���� ����� Ư�� ��ġ�� ��� ��ȯ
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="position">��� ��� ���� ����� Ư�� ��ġ</param>
/// <returns>��� ��� ���� ����� Ư�� ��ġ�� ���</returns>
Node* DLL_GetNodeAt(Node** srcList, NodePositionType position)
{
	if (position < 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	Node* retVal = (*srcList); //���� ���

	while (retVal != NULL && (--position) >= 0) //��ȯ ��� ��� ��ġ���� �̵�
	{
		retVal = retVal->next;
	}

	if (retVal == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	return retVal;
}

/// <summary>
/// ��� ����Ʈ���� ��� ��� ���� ���� �� ����� Ư�� ��ġ�� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="position">��� ��� ���� ���� �� ����� Ư�� ��ġ</param>
/// <param name="deallocateAfterRemove">���� ��� ��忡 ���� �޸� ���� ���� ����</param>
void DLL_RemoveNodeAt(Node** srcList, NodePositionType position, bool deallocateAfterRemove)
{
	if (position < 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	Node* current = (*srcList); //���� ���

	while (current != NULL && (--position) >= 0) //���� ��� ��� ��ġ���� �̵�
	{
		current = current->next;
	}

	if (current == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	DLL_RemoveNode(srcList, current, deallocateAfterRemove);
}

/// <summary>
/// ��� ����Ʈ���� ���� ��� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="srcTargetNode">���� ��� ���</param>
/// <param name="deallocateAfterRemove">���� ��� ��忡 ���� �޸� ���� ���� ����</param>
void DLL_RemoveNode(Node** srcList, Node* srcTargetNode, bool deallocateAfterRemove)
{
	/***
		1) TPE : ���� ��� ����� ���� ��� ���� ���� (T : ����, F : �� ����)
		2) TNE : ���� ��� ����� ���� ��� ���� ���� (T : ����, F : �� ����)
		3) D : �̿� ���� ���� ��� ��� ���� �� ���� �۾�

		TPE | TNE | D
		T	 T		1) ���� ��� ����� ���� ����� ������ ���� ��� ����� ���� ���� ����
					2) ���� ��� ����� ���� ����� ������ ���� ��� ����� ���� ���� ����
					3) ���� ��� ����� ����, ���� ���� ���� �� ���� ��� ��� ����

		T	 F		1) ���� ��� ����� ���� ����� ������ ���� ��� ����� ���� ���� ���� (NULL)
					2) ���� ��� ����� ����, ���� (NULL) ���� ���� �� ���� ��� ��� ����

		F	 T		1) ��� ��带 ���� ��� ����� ���� ���� ����
					2) ���� ��� ����� ���� ����� ������ ���� ��� ����� ���� ���� ���� (NULL)
					3) ���� ��� ����� ���� (NULL), ���� ���� ���� �� ���� ��� ��� ����

		F	 F		1) ���� ��� ����� ���� (NULL), ���� (NULL) ���� ���� �� ���� ��� ��� ����
	***/

	if (srcTargetNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (srcTargetNode == (*srcList)) //F T (���� ��� ��尡 ��� ����� ���)
	{
		(*srcList) = srcTargetNode->next; //��� ��带 ���� ��� ����� ���� ���� ����
	}

	if (srcTargetNode->next != NULL) //- T
	{
		srcTargetNode->next->prev = srcTargetNode->prev; //���� ��� ����� ���� ����� ������ ���� ��� ����� ���� ���� ����
	}

	if (srcTargetNode->prev != NULL) //T -
	{
		srcTargetNode->prev->next = srcTargetNode->next; //���� ��� ����� ���� ����� ������ ���� ��� ����� ���� ���� ����
	}

	srcTargetNode->prev = NULL;
	srcTargetNode->next = NULL;

	if (deallocateAfterRemove)
	{
		DLL_DeallocateNode(&srcTargetNode);
	}
}

/// <summary>
/// ��� ����Ʈ�� �� �տ� �� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="srcNewNode">�����ϰ��� �ϴ� �� ���</param>
void DLL_InsertNewHead(Node** srcList, Node* srcNewNode)
{
	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcList) == NULL)
	{
		(*srcList) = srcNewNode;
	}
	else //��� ����� �տ� ����
	{
		DLL_InsertNodeBefore(srcList, (*srcList), srcNewNode);
	}
}

/// <summary>
/// ��� ����� �ڿ� �� ��� ����
/// </summary>
/// <param name="srcTargetNode">��� ���</param>
/// <param name="srcNewNode">�����ϰ��� �ϴ� �� ���</param>
void DLL_InsertNodeAfter(Node* srcTargetNode, Node* srcNewNode)
{
	/***
		ȯ�� ���� ���� ����Ʈ�� ���Ͽ� ���� ������ ���� ������� �����Ƿ�,
		��� ����� �ڿ� ���� �� ��� ����� ���� ��尡 ���� �� ��츸,
		��� ����� ���� ����� ������ �� ���� ����
	***/

	if (srcTargetNode == NULL || srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	srcNewNode->prev = srcTargetNode; //�� ����� ������ ��� ���� ����
	srcNewNode->next = srcTargetNode->next; //�� ����� ������ ��� ����� ���� ���� ����

	if (srcTargetNode->next != NULL) //��� ����� ���� ��尡 ���� �� ���
	{
		srcTargetNode->next->prev = srcNewNode; //��� ����� ���� ����� ������ �� ���� ����
	}

	srcTargetNode->next = srcNewNode; //��� ����� ������ �� ���� ����
}

/// <summary>
/// ��� ����Ʈ�� ��� ����� �տ� �� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="srcTargetNode">��� ���</param>
/// <param name="srcNewNode">�����ϰ��� �ϴ� �� ���</param>
void DLL_InsertNodeBefore(Node** srcList, Node* srcTargetNode, Node* srcNewNode)
{
	/***
		ȯ�� ���� ���� ����Ʈ�� ���Ͽ� ���� ������ ���� ������� �����Ƿ�,
		��� ����� �տ� ���� �� ��� ����� ���� ��尡 ���� �� ��츸 (��� ��尡 ��� ��尡 �ƴ� ���)
		��� ����� ���� ����� ������ �� ���� ����
		---
		1) TH : ��� ����� ��� ��� ���� (T : ��� ���, F : �߰� ��� Ȥ�� ���� ���)
		2) D : �̿� ���� �����ϰ��� �ϴ� �� ��� ���� �� ���� �۾�

		TH | D
		T	1) �� ����� ������ ��� ���� ����
			2) �� ����� ������ ��� ����� ���� ���� ����
			3) �� ��带 ��� ���� ����
			4) ��� ����� ������ �� ���� ����

		F	1) �� ����� ������ ��� ���� ����
			2) �� ����� ������ ��� ����� ���� ���� ����
			3) ��� ����� ���� ����� ������ �� ���� ����
			4) ��� ����� ������ �� ���� ����
	***/

	if (srcTargetNode == NULL || srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	srcNewNode->next = srcTargetNode; //�� ����� ������ ��� ���� ����
	srcNewNode->prev = srcTargetNode->prev; //�� ����� ������ ��� ����� ���� ���� ����

	if (srcTargetNode == (*srcList)) //T
	{
		(*srcList) = srcNewNode; //��� ��� ����
	}
	else //F
	{
		srcTargetNode->prev->next = srcNewNode; //��� ����� ���� ����� ������ �� ���� ����
	}

	srcTargetNode->prev = srcNewNode; //��� ����� ������ �� ���� ����
}

/// <summary>
/// ��� ����Ʈ�� ��ü ����� �� ��ȯ
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <returns>��� ����Ʈ�� ��ü ����� ��</returns>
NodePositionType DLL_GetTotalNodeCount(Node** srcList)
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
void DLL_DispNodeList(Node** srcList)
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