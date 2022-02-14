#ifndef _SEARCH_HPP_
#define _SEARCH_HPP_

#ifdef _DLL_H_
/// <summary>
/// ��� ����Ʈ���� ����Ž�� - ���� �̵��� (Move To Front)���� ��� �����Ͱ� ���� �� ���� ��� ��ȯ
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="targetData">ã���� �ϴ� ��� ������</param>
/// <returns>��� �����Ͱ� ���� �� ���� ���</returns>
Node* DLL_MTF_SequentialSearch(Node** srcList, DataType targetData)
{
	Node* current = (*srcList); //���� ���
	Node* retVal = NULL; //ã�� ���

	while (current != NULL)
	{
		if (current->data == targetData) //ã���� �ϴ� ��� �����͸� �߰� ��
		{
			retVal = current;
			break;
		}

		current = current->next;
	}

	if (current == NULL || current->data != targetData)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not found"));

	if (retVal != (*srcList)) //ã�� ��尡 ��� ��尡 �ƴ� ���
	{
		DLL_RemoveNode(srcList, retVal, false); //ã�� ��带 ����Ʈ���� �и�
		DLL_InsertNewHead(srcList, retVal); //ã�� ��带 ����Ʈ�� �� �տ� ����
	}
	else //ã�� ��尡 ��� ����� ���
	{
		//do nothing 
	}

	return retVal;
}

/// <summary>
/// ��� ����Ʈ���� ����Ž�� - ������ (Transpose)���� ��� �����Ͱ� ���� �� ���� ��� ��ȯ
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="targetData">ã���� �ϴ� ��� ������</param>
/// <returns>��� �����Ͱ� ���� �� ���� ���</returns>
Node* DLL_Transpose_SequentialSearch(Node** srcList, DataType targetData)
{
	Node* current = (*srcList); //���� ���
	Node* retVal = NULL; //ã�� ���
	Node* insertTarget = NULL; //���� ��� ���

	while (current != NULL)
	{
		if (current->data == targetData) //ã���� �ϴ� ��� �����͸� �߰� ��
		{
			retVal = current;
			break;
		}

		current = current->next;
	}

	if (current == NULL || current->data != targetData)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not found"));

	if (retVal->prev != NULL) //ã�� ����� �� ��尡 �����ϸ�
	{
		insertTarget = retVal->prev; //���� ��� ���� ã�� ����� �� ���
		DLL_RemoveNode(srcList, retVal, false); //ã�� ��带 ����Ʈ���� �и�
		DLL_InsertNodeBefore(srcList, insertTarget, retVal); //���� ��� ����� �տ� ����
	}
	else //ã�� ����� �� ��尡 �������� ������
	{
		//do nothing (ã�� ���� ��� ���)
	}

	return retVal;
}
#endif

template<typename SearchElementType>
SearchElementType MTF_SequentialSearch(SearchElementType targetEnumerableSet[], 
	size_t elementCount)
{

}

template<typename SearchElementType>
SearchElementType Transpose_SequentialSearch(SearchElementType targetEnumerableSet[],
	size_t elementCount)
{

}

template<typename SearchElementType>
SearchElementType BinarySearch(SearchElementType targetEnumerableSet[],
	size_t elementCount)
{
}
#endif