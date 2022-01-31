#include "DS_Core.h"

/// <summary>
/// ���ο� ��� ���� �� ���� �� ��� ��ȯ
/// </summary>
/// <param name="srcData">����� ������</param>
/// <returns>���� �� ���</returns>
Node* DS_CreateNode(void* srcData)
{
	Node* retVal = (Node*)malloc(sizeof(Node));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->data = srcData;
	retVal->parent = NULL;

	return retVal;
}

/// <summary>
/// ��� ��忡 �Ҵ� �� �޸� ����
/// </summary>
/// <param name="srcNode">��� ���</param>
void DS_DeallocateNode(Node** srcNode)
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
void DS_UnionSet(Node** originSetNode, Node* targetSetNode)
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
		targetSetNode->parent = DS_FindSet((*originSetNode));
	}
}

/// <summary>
/// ��� ���� ��尡 ���� ���� (�ֻ��� �θ� ���) ��ȯ
/// </summary>
/// <param name="targetSetNode">��� ���� ���</param>
/// <returns>��� ���� ��尡 ���� ���� (�ֻ��� �θ� ���)</returns>
Node* DS_FindSet(Node* targetSetNode)
{
	if (targetSetNode == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	while (targetSetNode->parent != NULL) //�ֻ��� �θ� ��� Ž��
	{
		targetSetNode = targetSetNode->parent;
	}

	return targetSetNode;
}