#include "DLL_Core.h"

/// <summary>
/// ���ο� ��� ���� �� ���� �� ��� ��ȯ
/// </summary>
/// <param name="srcData">����� ������</param>
/// <returns>���� �� ���</returns>
NODE* DLL_CreateNode(DATA_TYPE srcData)
{
	NODE* retVal = (NODE*)malloc(sizeof(NODE));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_data = srcData;
	retVal->_prev = retVal->_next = NULL;

	return retVal;
}

/// <summary>
/// ��� ��忡 �Ҵ� �� �޸� ����
/// </summary>
/// <param name="srcNode">��� ���</param>
void DLL_DeallocateNode(NODE** srcNode)
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
void DLL_DeallocateNodeList(NODE** srcList)
{
	while ((*srcList) != NULL)
	{
		NODE* tmp = (*srcList); //���� �� ���

		(*srcList) = (*srcList)->_next; //��� ��带 ���� ���� �̵� �� ����
		DLL_DeallocateNode(&tmp);
	}

	(*srcList) = NULL;
}

/// <summary>
/// ��� ����Ʈ�� ���� �� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="srcNewNode">����Ʈ�� ���� �����ϰ��� �ϴ� �� ���</param>
void DLL_AppendNode(NODE** srcList, NODE* srcNewNode)
{
	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcList) == NULL) //������� ���
	{
		(*srcList) = srcNewNode; //��� ��� �Ҵ�
	}
	else //���� �� ��� ����
	{
		NODE* tail = (*srcList); //���� ���

		while (tail->_next != NULL) //������ �̵�
		{
			tail = tail->_next;
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
NODE* DLL_GetNodeAt(NODE** srcList, NODE_POSITION_TYPE position)
{
	if (position < 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	NODE* retVal = (*srcList); //���� ���

	while (retVal != NULL && (--position) >= 0) //��ȯ ��� ��� ��ġ���� �̵�
	{
		retVal = retVal->_next;
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
void DLL_RemoveNodeAt(NODE** srcList, NODE_POSITION_TYPE position, bool deallocateAfterRemove)
{
	if (position < 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	NODE* current = (*srcList); //���� ���

	while (current != NULL && (--position) >= 0) //���� ��� ��� ��ġ���� �̵�
	{
		current = current->_next;
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
void DLL_RemoveNode(NODE** srcList, NODE* srcTargetNode, bool deallocateAfterRemove)
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
		(*srcList) = srcTargetNode->_next; //��� ��带 ���� ��� ����� ���� ���� ����
	}

	if (srcTargetNode->_next != NULL) //- T
	{
		srcTargetNode->_next->_prev = srcTargetNode->_prev; //���� ��� ����� ���� ����� ������ ���� ��� ����� ���� ���� ����
	}

	if (srcTargetNode->_prev != NULL) //T -
	{
		srcTargetNode->_prev->_next = srcTargetNode->_next; //���� ��� ����� ���� ����� ������ ���� ��� ����� ���� ���� ����
	}

	srcTargetNode->_prev = NULL;
	srcTargetNode->_next = NULL;

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
void DLL_InsertNewHead(NODE** srcList, NODE* srcNewNode)
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
void DLL_InsertNodeAfter(NODE* srcTargetNode, NODE* srcNewNode)
{
	/***
		ȯ�� ���� ���� ����Ʈ�� ���Ͽ� ���� ������ ���� ������� �����Ƿ�,
		��� ����� �ڿ� ���� �� ��� ����� ���� ��尡 ���� �� ��츸,
		��� ����� ���� ����� ������ �� ���� ����
	***/

	if (srcTargetNode == NULL || srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	srcNewNode->_prev = srcTargetNode; //�� ����� ������ ��� ���� ����
	srcNewNode->_next = srcTargetNode->_next; //�� ����� ������ ��� ����� ���� ���� ����

	if (srcTargetNode->_next != NULL) //��� ����� ���� ��尡 ���� �� ���
	{
		srcTargetNode->_next->_prev = srcNewNode; //��� ����� ���� ����� ������ �� ���� ����
	}

	srcTargetNode->_next = srcNewNode; //��� ����� ������ �� ���� ����
}

/// <summary>
/// ��� ����Ʈ�� ��� ����� �տ� �� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="srcTargetNode">��� ���</param>
/// <param name="srcNewNode">�����ϰ��� �ϴ� �� ���</param>
void DLL_InsertNodeBefore(NODE** srcList, NODE* srcTargetNode, NODE* srcNewNode)
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

	srcNewNode->_next = srcTargetNode; //�� ����� ������ ��� ���� ����
	srcNewNode->_prev = srcTargetNode->_prev; //�� ����� ������ ��� ����� ���� ���� ����

	if (srcTargetNode == (*srcList)) //T
	{
		(*srcList) = srcNewNode; //��� ��� ����
	}
	else //F
	{
		srcTargetNode->_prev->_next = srcNewNode; //��� ����� ���� ����� ������ �� ���� ����
	}

	srcTargetNode->_prev = srcNewNode; //��� ����� ������ �� ���� ����
}

/// <summary>
/// ��� ����Ʈ�� ��ü ����� �� ��ȯ
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <returns>��� ����Ʈ�� ��ü ����� ��</returns>
NODE_POSITION_TYPE DLL_GetTotalNodeCount(NODE** srcList)
{
	NODE_POSITION_TYPE totalNodeCount = 0; //��ü ����� ��
	NODE* current = (*srcList); //���� ���

	while (current != NULL)
	{
		current = current->_next;
		totalNodeCount++;
	}

	return totalNodeCount;
}

/// <summary>
/// ��� ����Ʈ�� ��ü ��忡 ���� ������ ���
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
void DLL_DispNodeList(NODE** srcList)
{
	NODE_POSITION_TYPE currentPosition = 0; //���� ����� ��ġ
	NODE* current = (*srcList); //���� ���

	while (current != NULL)
	{
		std::cout << "List [" << currentPosition << "] : " << current->_data << std::endl;

		current = current->_next;
		currentPosition++;
	}
}