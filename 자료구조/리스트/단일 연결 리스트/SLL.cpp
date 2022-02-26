#include "SLL_Core.h"

/// <summary>
/// ���ο� ��� ���� �� ���� �� ��� ��ȯ
/// </summary>
/// <param name="srcData">����� ������</param>
/// <returns>���� �� ���</returns>
NODE* SLL_CreateNode(DATA_TYPE srcData)
{
	NODE* retVal = (NODE*)malloc(sizeof(NODE));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_data = srcData;
	retVal->_next = NULL;

	return retVal;
}

/// <summary>
/// ��� ��忡 �Ҵ� �� �޸� ����
/// </summary>
/// <param name="srcNode">��� ���</param>
void SLL_DeallocateNode(NODE** srcNode)
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
void SLL_DeallocateNodeList(NODE** srcList)
{
	while ((*srcList) != NULL)
	{
		NODE* tmp = (*srcList); //���� �� ���

		(*srcList) = (*srcList)->_next; //��� ��带 ���� ���� �̵� �� ����
		SLL_DeallocateNode(&tmp);
	}

	(*srcList) = NULL;
}

/// <summary>
/// ��� ����Ʈ�� ���� �� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="srcNewNode">����Ʈ�� ���� �����ϰ��� �ϴ� �� ���</param>
void SLL_AppendNode(NODE** srcList, NODE* srcNewNode)
{
	if (srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*srcList) == NULL)
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

		SLL_InsertNodeAfter(tail, srcNewNode);
	}
}

/// <summary>
/// ��� ����Ʈ���� ��� ��� ���� ����� Ư�� ��ġ�� ��� ��ȯ
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="position">��� ��� ���� ����� Ư�� ��ġ</param>
/// <returns>��� ��� ���� ����� Ư�� ��ġ�� ���</returns>
NODE* SLL_GetNodeAt(NODE** srcList, NODE_POSITION_TYPE position)
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
void SLL_RemoveNodeAt(NODE** srcList, NODE_POSITION_TYPE position, bool deallocateAfterRemove)
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
		NODE* tmp = (*srcList); //���� ��� ���
		(*srcList) = (*srcList)->_next; //��� ����� ���� ���� ��� ��� ����
		tmp->_next = NULL;

		if (deallocateAfterRemove)
		{
			SLL_DeallocateNode(&tmp);
		}
	}
	else //���� ����� ���� ��尡 ���� ��� ����� ������ Ž��
	{
		NODE* current = (*srcList); //���� ���

		while (current != NULL && (--position) >= 1) //���� ��� ����� ���� ��� ��ġ���� �̵�
		{
			current = current->_next;
		}

		if (current == NULL)
			throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

		NODE* tmp = current->_next; //���� ��� ���

		current->_next = tmp->_next; //���� ����� ������ ���� ��� ����� ���� ���� ����
		tmp->_next = NULL;

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
void SLL_RemoveNode(NODE** srcList, NODE* srcTargetNode, bool deallocateAfterRemove)
{
	if (srcTargetNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (srcTargetNode == (*srcList)) //���� ��� ��尡 ��� ����� ���
	{
		(*srcList) = srcTargetNode->_next; //���� ��� ����� ���� ���� ��� ��� ����
		srcTargetNode->_next = NULL;

		if (deallocateAfterRemove)
		{
			SLL_DeallocateNode(&srcTargetNode);
		}
	}
	else //���� ����� ���� ��尡 ���� ��� ����϶����� Ž��
	{
		NODE* current = (*srcList); //���� ���

		while (current != NULL && current->_next != srcTargetNode) //���� ��� ����� ���� ��� ��ġ���� �̵�
		{
			current = current->_next;
		}

		if (current == NULL)
			throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

		current->_next = srcTargetNode->_next; //���� ����� ������ ���� ��� ����� ���� ���� ����
		srcTargetNode->_next = NULL;

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
void SLL_InsertNewHead(NODE** srcList, NODE* srcNewNode)
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
void SLL_InsertNodeAfter(NODE* srcTargetNode, NODE* srcNewNode)
{
	if (srcTargetNode == NULL || srcNewNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	srcNewNode->_next = srcTargetNode->_next; //�� ����� ������ ��� ����� ���� ���� ����
	srcTargetNode->_next = srcNewNode; //��� ����� ������ �� ���� ����
}

/// <summary>
/// ��� ����Ʈ�� ��� ����� �տ� �� ��� ����
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="srcTargetNode">��� ���</param>
/// <param name="srcNewNode">�����ϰ��� �ϴ� �� ���</param>
void SLL_InsertNodeBefore(NODE** srcList, NODE* srcTargetNode, NODE* srcNewNode)
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
		srcNewNode->_next = (*srcList); //�� ����� ������ ���� ��� ���� ����
		(*srcList) = srcNewNode; //��� ��� ����
	}
	else //F
	{
		NODE* current = (*srcList); //���� ���

		while (current != NULL && current->_next != srcTargetNode) //���� ����� ���� ��尡 ���� ��� ����϶����� Ž��
		{
			current = current->_next;
		}

		if (current == NULL)
			throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

		current->_next = srcNewNode; //���� ����� ������ �� ���� ����
		srcNewNode->_next = srcTargetNode; //�� ����� ������ ��� ���� ����
	}
}

/// <summary>
/// ��� ����Ʈ�� ��ü ����� �� ��ȯ
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <returns>��� ����Ʈ�� ��ü ����� ��</returns>
NODE_POSITION_TYPE SLL_GetTotalNodeCount(NODE** srcList)
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
void SLL_DispNodeList(NODE** srcList)
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