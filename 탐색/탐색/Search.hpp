#ifndef _SEARCH_HPP_
#define _SEARCH_HPP_

#ifdef _DLL_H_
/// <summary>
/// ��� ����Ʈ���� ����Ž�� - ���� �̵��� (Move To Front)���� ��� �����Ͱ� ���� �� ���� ��� ��ȯ
/// </summary>
/// <param name="srcList">��� ����Ʈ</param>
/// <param name="targetData">ã���� �ϴ� ��� ������</param>
/// <returns>��� �����Ͱ� ���� �� ���� ���</returns>
Node* DLL_SequentialSearch_MTF(Node** srcList, const DataType& targetData)
{
	Node* currentNode = (*srcList); //���� ���
	Node* retVal = NULL; //ã�� ���

	while (currentNode != NULL)
	{
		if (COMPARE(currentNode->_data, targetData) == 0) //ã���� �ϴ� ��� �����͸� �߰� ��
		{
			retVal = currentNode;
			break;
		}

		currentNode = currentNode->_next;
	}

	if (currentNode == NULL || COMPARE(currentNode->_data, targetData) != 0)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

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
Node* DLL_SequentialSearch_Transpose(Node** srcList, const DataType& targetData)
{
	Node* currentNode = (*srcList); //���� ���
	Node* retVal = NULL; //ã�� ���
	Node* insertTargetNode = NULL; //���� ��� ���

	while (currentNode != NULL)
	{
		if (COMPARE(currentNode->_data, targetData) == 0) //ã���� �ϴ� ��� �����͸� �߰� ��
		{
			retVal = currentNode;
			break;
		}

		currentNode = currentNode->_next;
	}

	if (currentNode == NULL || COMPARE(currentNode->_data, targetData) != 0)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (retVal->_prev != NULL) //ã�� ����� �� ��尡 �����ϸ�
	{
		insertTargetNode = retVal->_prev; //���� ��� ���� ã�� ����� �� ���
		DLL_RemoveNode(srcList, retVal, false); //ã�� ��带 ����Ʈ���� �и�
		DLL_InsertNodeBefore(srcList, insertTargetNode, retVal); //���� ��� ����� �տ� ����
	}
	else //ã�� ����� �� ��尡 �������� ������
	{
		//do nothing (ã�� ���� �̹� ��� ���)
	}

	return retVal;
}
#endif

/// <summary>
/// ����Ž�� - ���� �̵��� (Move To Front) (Best Case : O(1), Average, Worst Case : O(n))
/// </summary>
/// <typeparam name="SearchElementType">�˻� ��� Ÿ��</typeparam>
/// <param name="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
/// <param name="targetData">ã���� �ϴ� ��� ������</param>
/// <returns>ã���� �ϴ� ��� �����Ϳ� ��ġ�ϴ� ���ʷ� ã�� ������</returns>
template<typename SearchElementType>
SearchElementType SequentialSearch_MTF(SearchElementType targetEnumerableSet[],
	size_t elementCount, const SearchElementType& targetData)
{
	SearchElementType tmp;

	for (size_t i = 0; i < elementCount; i++)
	{
		if (COMPARE(targetEnumerableSet[i], targetData) == 0) //ã���� �ϴ� ��� �����͸� �߰� ��
		{
			tmp = targetEnumerableSet[i];

			memmove_s(&targetEnumerableSet[1],
				sizeof(targetEnumerableSet[0]) * i,
				&targetEnumerableSet[0],
				sizeof(targetEnumerableSet[0]) * i
			); //ã�� �������� �ٷ� �ձ����� ��� ���� ������ ��ҵ��� �ڷ� �� ĭ�� �̵�

			targetEnumerableSet[0] = tmp; //ã�� �����͸� �� ������ ����
			return targetEnumerableSet[0];
		}
	}

	throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));
}

/// <summary>
/// ����Ž�� - ������ (Transpose) (Best Case : O(1), Average, Worst Case : O(n))
/// </summary>
/// <typeparam name="SearchElementType">�˻� ��� Ÿ��</typeparam>
/// <param name="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
/// <param name="targetData">ã���� �ϴ� ��� ������</param>
/// <returns>ã���� �ϴ� ��� �����Ϳ� ��ġ�ϴ� ���ʷ� ã�� ������</returns>
template<typename SearchElementType>
SearchElementType SequentialSearch_Transpose(SearchElementType targetEnumerableSet[],
	size_t elementCount, const SearchElementType& targetData)
{
	SearchElementType tmp;

	for (size_t i = 0; i < elementCount; i++)
	{
		if (COMPARE(targetEnumerableSet[i], targetData) == 0) //ã���� �ϴ� ��� �����͸� �߰� ��
		{
			if (i > 0) //ã�� �������� �տ� �����Ͱ� �����ϸ�
			{
				SWAP(targetEnumerableSet[i], targetEnumerableSet[i - 1], tmp); //ã�� �����͸� �ٷ� ���� ��ҿ� ��ȯ
				return targetEnumerableSet[i - 1];
			}

			return targetEnumerableSet[i]; //ã�� �����Ͱ� �̹� �� ���� ����� ���
		}
	}

	throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));
}

/// <summary>
/// ���� Ž�� (Best Case : O(1), Average, Worst Case : O(log2(n)))
/// </summary>
/// <typeparam name="SearchElementType">Ž�� ��� Ÿ��</typeparam>
/// <param name="srcOrderedEnumerableSet">�̹� ���� �� ���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="elementCount">�̹� ���� �� ���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
/// <param name="targetData">ã���� �ϴ� ��� ������</param>
/// <returns>ã���� �ϴ� ��� �����Ϳ� ��ġ�ϴ� ���ʷ� ã�� ������</returns>
template<typename SearchElementType>
SearchElementType BinarySearch(const SearchElementType srcOrderedEnumerableSet[],
	size_t elementCount, SearchElementType targetData)
{
	/***
		< ���� Ž���� ���� ���� �Ǻ� >

		M1) ���� ������ �Ķ���ͷ� �޴� ���

			M1-1) ����ڰ� �̹� ���� �� ���������� ���� ���� �� ��ҵ��� ������ ��� �������� ���ĵǾ��ִ��� �˰� ���� ���
			: ���� Ž�� �� Ž�� ������ �ٿ����� ��ġ�� �Ǻ� ����

			M1-2) ����ڰ� �̹� ���� �� ���������� ���� ���� �� ��ҵ��� ������ ��� �������� ���ĵǾ��ִ��� �� ���
			: ���� Ž���� ���� �� �� ������, �̴� ������ ���� ���� ����� ������

		M2) �̹� ���� �� ���������� ���� ���� �� ��ҵ��� ������ ó�� ��ҿ� ������ ��Ҹ� ���� �Ǻ�

			M2-1) ó�� ��� > ������ ���
			: �̹� ���� �� ���������� ���� ���� �� ��ҵ��� ������ ������������ ���ĵǾ������� ����

			M2-2) ó�� ��� == ������ ���
			: �ش� ������ ��� ��Ұ� ������ ���� ������ �ְų�, �ش� ������ ���ĵǾ� ���� ����
			(ex) 3 1 3 Ȥ�� 1 1 �� �� ���տ� ����, ���� ������ ���ĵǾ� ���� ������, ���� ������ ��� ��Ұ� ������ ���� ����)
			�̿� ����, ���ĵǾ� ���� ���� ���տ� ����, ã���� �ϴ� ��ҿ� ó�� ��Ұ� ��ġ���� �Ǻ�

				M2-2-1) ã���� �ϴ� ��ҿ� ó�� ��Ұ� ��ġ
				: Ž�� ����, ã�� ��� ��ȯ

				M2-2-2) ã���� �ϴ� ��ҿ� ó�� ��Ұ� ����ġ
				: �ش� ������ ��� ��Ұ� ������ ���� ������ �������� ã���� �ϴ� ��ҿ� ����ġ�ϰų�,
				�ش� ������ ���ĵǾ� ���� �����Ƿ�, ���� Ž�� ���� �Ұ�

			M2-3) ó�� ��� < ������ ���
			: �̹� ���� �� ���������� ���� ���� �� ��ҵ��� ������ ������������ ���ĵǾ������� ����

		---

		< Ž�� Ƚ�� >

		n : ���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����
		Ž�� �������� ��ҵ��� ���� (Ž�� ����) 1/2 �� ����

		1 (���� Ž�� ����) = n * (1 / 2)^Ž�� Ƚ��
		1 = n * 1 / 2^Ž�� Ƚ��
		2^Ž�� Ƚ�� = n
		Ž�� Ƚ�� = log2(n)
	***/

	size_t leftIndex = 0; //���� �ε���
	size_t rightIndex = elementCount - 1; //������ �ε���
	size_t midIndex; //��� �ε���

	/***
		< �̹� ���� �� ���������� ���� ���� �� ��ҵ��� ������ �������� ���� ���� >

		1: �������� ���� (right > left)
		-1 : �������� ���� (right < left)
		0 : ��� ��Ұ� ������ ���� ������ �ְų�, �ش� ������ ���ĵǾ� ���� ���� (left == right)
	***/

	char isOrderByAscending = COMPARE(srcOrderedEnumerableSet[rightIndex], srcOrderedEnumerableSet[leftIndex]);

	switch (isOrderByAscending)
	{
	case 0: //��� ��Ұ� ������ ���� ������ �ְų�, �ش� ������ ���ĵǾ� ���� ���� 
		if (COMPARE(srcOrderedEnumerableSet[leftIndex], targetData) == 0) //ã���� �ϴ� ��ҿ� ó�� ��Ұ� ��ġ
			return srcOrderedEnumerableSet[leftIndex];

		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Unordered EnumerableSet or Not found"));

	default:
		break;
	}

	while (leftIndex <= rightIndex) //Ž�� ������ �����ϴ� ����
	{
		midIndex = leftIndex + ((rightIndex - leftIndex) >> 1); //base : leftIndex, offset : rightIndex�� leftIndex�� �߰���ŭ ���� (overflow ����)

		if (COMPARE(srcOrderedEnumerableSet[midIndex], targetData) == 0)
			return srcOrderedEnumerableSet[midIndex];

		switch (isOrderByAscending)
		{
		case 1: //�������� ����
			if (COMPARE(srcOrderedEnumerableSet[midIndex], targetData) == -1) //mid < target
				leftIndex = midIndex + 1; //���� ��� �ε��� ���� ���������� Ž��
			else //mid > target
				rightIndex = midIndex - 1; //���� ��� �ε��� ���� �������� Ž��
			break;

		case -1: //�������� ����
			if (COMPARE(srcOrderedEnumerableSet[midIndex], targetData) == -1) //mid < target
				rightIndex = midIndex - 1; //���� ��� �ε��� ���� �������� Ž��
			else //mid > target
				leftIndex = midIndex + 1; //���� ��� �ε��� ���� ���������� Ž��
			break;

		default:
			throw myexception::MEM_CORRUPTION_EXCEPTION(std::string(__func__) + std::string(" : Mem corruption"));
		}
	}

	throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));
}
#endif