#include "CDLL_Core.h"

/// <summary>
/// ���ο� ��� ���� �� ���� �� ��� ��ȯ
/// </summary>
/// <param name="srcData">����� ������</param>
/// <returns>���� �� ���</returns>
NODE* CDLL_CreateNode(DATA_TYPE srcData)
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
void CDLL_DeallocateNode(NODE** srcNode)
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
void CDLL_DeallocateNodeList(NODE** srcList)
{
	NODE* oldHead = (*srcList); //���� ��� ���

	while ((*srcList) != NULL)
	{
		NODE* tmp = (*srcList); //���� �� ���

		(*srcList) = (*srcList)->_next; //��� ��带 ���� ���� �̵� �� ����
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
void CDLL_AppendNode(NODE** srcList, NODE* srcNewNode)
{
	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcList) == NULL) //������� ���
	{
		(*srcList) = srcNewNode; //��� ��� �Ҵ�
		(*srcList)->_prev = (*srcList)->_next = (*srcList);
	}
	else //���� �� ��� ����
	{
		NODE* tail = (*srcList)->_prev; //���� ���

		CDLL_InsertNodeAfter(tail, srcNewNode);
	}
}

/// <summary>
/// ��� ����Ʈ���� ��� ��� ���� ����� Ư�� ��ġ�� ��� ��ȯ
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="position">��� ��� ���� ����� Ư�� ��ġ</param>
/// <returns>��� ��� ���� ����� Ư�� ��ġ�� ���</returns>
NODE* CDLL_GetNodeAt(NODE** srcList, NODE_POSITION_TYPE position)
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
void CDLL_RemoveNodeAt(NODE** srcList, NODE_POSITION_TYPE position, bool deallocateAfterRemove)
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

	CDLL_RemoveNode(srcList, current, deallocateAfterRemove);
}

/// <summary>
/// ��� ����Ʈ���� ���� ��� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="srcTargetNode">���� ��� ���</param>
/// <param name="deallocateAfterRemove">���� ��� ��忡 ���� �޸� ���� ���� ����</param>
void CDLL_RemoveNode(NODE** srcList, NODE* srcTargetNode, bool deallocateAfterRemove)
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

	if (srcTargetNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (srcTargetNode == (*srcList)) //���� ��� ��尡 ��� ����� ���
	{
		(*srcList) = srcTargetNode->_next; //��� ��带 ���� ��� ����� ���� ���� ����
	}

	srcTargetNode->_next->_prev = srcTargetNode->_prev; //���� ��� ����� ���� ����� ������ ���� ����� ���� ���� ����
	srcTargetNode->_prev->_next = srcTargetNode->_next; //���� ��� ����� ���� ����� ������ ���� ����� ���� ���� ����
	srcTargetNode->_prev = NULL;
	srcTargetNode->_next = NULL;

	if (deallocateAfterRemove)
	{
		CDLL_DeallocateNode(&srcTargetNode);
	}
}

/// <summary>
/// ��� ����Ʈ�� �� �տ� �� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="srcNewNode">�����ϰ��� �ϴ� �� ���</param>
void CDLL_InsertNewHead(NODE** srcList, NODE* srcNewNode)
{
	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcList) == NULL)
	{
		(*srcList) = srcNewNode;
		(*srcList)->_prev = (*srcList)->_next = (*srcList);
	}
	else //��� ����� �տ� ����
	{
		CDLL_InsertNodeBefore(srcList, (*srcList), srcNewNode);
	}
}

/// <summary>
/// ��� ����� �ڿ� �� ��� ����
/// </summary>
/// <param name="srcTargetNode">��� ���</param>
/// <param name="srcNewNode">�����ϰ��� �ϴ� �� ���</param>
void CDLL_InsertNodeAfter(NODE* srcTargetNode, NODE* srcNewNode)
{
	if (srcTargetNode == NULL || srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	srcNewNode->_prev = srcTargetNode; //�� ����� ������ ��� ���� ����
	srcNewNode->_next = srcTargetNode->_next; //�� ����� ������ ��� ����� ���� ���� ����
	srcTargetNode->_next->_prev = srcNewNode; //��� ����� ���� ����� ������ �� ���� ����
	srcTargetNode->_next = srcNewNode; //��� ����� ������ �� ���� ����
}

/// <summary>
/// ��� ����Ʈ�� ��� ����� �տ� �� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="srcTargetNode">��� ���</param>
/// <param name="srcNewNode">�����ϰ��� �ϴ� �� ���</param>
void CDLL_InsertNodeBefore(NODE** srcList, NODE* srcTargetNode, NODE* srcNewNode)
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

	if (srcTargetNode == NULL || srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	srcNewNode->_next = srcTargetNode; //�� ����� ������ ��� ���� ����
	srcNewNode->_prev = srcTargetNode->_prev; //�� ����� ������ ��� ����� ���� ���� ����
	srcTargetNode->_prev->_next = srcNewNode; //��� ����� ���� ����� ������ �� ���� ����
	srcTargetNode->_prev = srcNewNode; //��� ����� ������ �� ���� ����

	if (srcTargetNode == (*srcList)) //T
	{
		(*srcList) = srcNewNode; //��� ��� ����
	}
}

/// <summary>
/// ��� ����Ʈ�� ��ü ����� �� ��ȯ
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <returns>��� ����Ʈ�� ��ü ����� ��</returns>
NODE_POSITION_TYPE CDLL_GetTotalNodeCount(NODE** srcList)
{
	NODE_POSITION_TYPE totalNodeCount = 0; //��ü ����� ��
	NODE* current = (*srcList); //���� ���

	while (current != NULL)
	{
		current = current->_next;
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
void CDLL_DispNodeList(NODE** srcList)
{
	NODE_POSITION_TYPE currentPosition = 0; //���� ����� ��ġ
	NODE* current = (*srcList); //���� ���

	while (current != NULL)
	{
		std::cout << "List [" << currentPosition << "] : " << current->_data << std::endl;

		current = current->_next;
		currentPosition++;

		if (current == (*srcList)) //��� ���� ���ƿ� ���
			break;
	}
}