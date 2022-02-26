#include "DS_Core.h"

/// <summary>
/// ���ο� ��� ���� �� ���� �� ��� ��ȯ
/// </summary>
/// <param name="srcData">����� ������</param>
/// <returns>���� �� ���</returns>
NODE* DS_CreateNode(void* srcData)
{
	NODE* retVal = (NODE*)malloc(sizeof(NODE));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_data = srcData;
	retVal->_parent = NULL;

	return retVal;
}

/// <summary>
/// ��� ��忡 �Ҵ� �� �޸� ����
/// </summary>
/// <param name="srcNode">��� ���</param>
void DS_DeallocateNode(NODE** srcNode)
{
	if ((*srcNode) != NULL)
	{
		free(*srcNode);
		(*srcNode) = NULL;
	}
}

/// <summary>
/// �� ��� ���� ��尡 ���� ���� ���� (������)
/// </summary>
/// <param name="originSetNode">���� �� ���� ���� ���</param>
/// <param name="targetSetNode">���� �� ���� ���տ� ���� �� ������ ���</param>
void DS_UnionSet(NODE** originSetNode, NODE* targetSetNode)
{
	if (targetSetNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((*originSetNode) == NULL)
	{
		(*originSetNode) = targetSetNode;
	}
	else
	{
		targetSetNode = DS_FindSet(targetSetNode); //���� �� ���� ���տ� ���� �� ������ �ֻ��� �θ� ���
		targetSetNode->_parent = DS_FindSet((*originSetNode));
	}
}

/// <summary>
/// ��� ���� ��尡 ���� ���� (�ֻ��� �θ� ���) ��ȯ
/// </summary>
/// <param name="targetSetNode">��� ���� ���</param>
/// <returns>��� ���� ��尡 ���� ���� (�ֻ��� �θ� ���)</returns>
NODE* DS_FindSet(NODE* targetSetNode)
{
	if (targetSetNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	while (targetSetNode->_parent != NULL) //�ֻ��� �θ� ��� Ž��
	{
		targetSetNode = targetSetNode->_parent;
	}

	return targetSetNode;
}