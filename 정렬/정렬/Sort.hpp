#ifndef _SORT_HPP_
#define _SORT_HPP_

#define LENGTH(array) ((sizeof(array)) / (sizeof(array[0])))
#define COMPARE(x, y) ((x) > (y) ? 1 : (x) == (y) ? 0 : -1) //x > y : 1, x == y : 0, x < y : -1
#define SWAP(x, y, tmp) ((tmp) = (x), (x) = (y), (y) = (tmp))

enum class ORDER_BY : const int
{
	ASCENDING, //��������
	DESCENDING //��������
}; //���� ���

/// <summary>
/// ���� ����
/// </summary>
/// <typeparam name="SortElementType">���� ��� Ÿ��</typeparam>
/// <param name="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
/// <param name="orderBy">���� ���</param>
template<typename SortElementType>
void BubbleSort(SortElementType targetEnumerableSet[], size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	/***
		< ���� ���� - �������� >

		1) ���� �� ��� ������ ���������� ���� ���� �� ��ҵ��� ���� ���� ��� ��ҵ鿡 ���� ����

		2) ���� �� ��� ���� ������ �̿� ��ҳ��� ���������� ��
		: �ε��� 0�� �ε��� 1�� ���, �ε��� 1�� �ε��� 2�� ���...

			2-1) �� ���� ���� ��Ұ� �̿� �� �ٷ� ���� ��Һ��� ���� ���
			: �� ��Ҹ� SWAP

		3) ���� �� ��� ���� ������ �̿� ��ҵ� �� �� �� SWAP �� ������ ���
		: ���� �� ��� ���� ���� ������ ��Ҵ� ���� �� ������ �����ϹǷ�, ���� �� ��� �������� ����

		---

		< ��ҵ� �� �� Ƚ�� >

		n : ���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����

		ex1) ��Ұ� 6���� ��, ù ��° �������� �� ���� ���� ��� ��ҵ��� 5�� ��
		�� ���� �Ϸ� �� �� ���� 1�� ����

		�� ���� �Ϸ� �� �� �ں��� ���������� ���� �� ��Ҹ� �����Ͽ� �� �� ��� ���� �ϳ��� ����
		5 + 4 + 3 + 2 + 1
		== 6 (���� ������ ��, ���� ��� ���� ��) * 2 (���� ������ ��, ���� ��� ���� ����) + 3 (���� ���� ���� ������)
		== 6 (���� ������ ��, ���� ��� ���� ��) * 2.5 (���� ������ ��, ���� ��� ���� ���� (n�� ¦�� �� ���, ���� ���� ���� �������� ����))

		ex2) ��Ұ� 7���� ��,  ù ��° �������� �� ���� ���� ��� ��ҵ��� 6�� ��
		�� ���� �Ϸ� �� �� ���� 1�� ����
		6 + 5 + 4 + 3 + 2 + 1
		== 7 (���� ������ ��, ���� ��� ���� ��) * 3 (���� ������ ��, ���� ��� ���� ���� (n�� ¦�� �� ���, ���� ���� ���� �������� ����))

		---

		���� ������ ��, ���� ��� ���� �� == n
		���� ������ ��, ���� ��� ���� ���� (n�� ¦�� �� ���, ���� ���� ���� �������� ����) == (n-1) / 2
		�̹Ƿ�,
		(n-1) + (n-2) + (n-3) + ... + (n-(n-2) + (n-(n-1)) == (n-1) + (n-2) + (n-3) + ... + 2 + 1 == n * ((n-1) / 2)
	***/

	SortElementType tmp;
	bool sortPerformed = false; //���� ���� ����

	for (size_t i = 0; i < (elementCount - 1); i++) //������ ��Ҵ� ���� ���� ���� ������, �� �ÿ��� ���� ����
	{
		sortPerformed = false;

		for (size_t j = 0; j < ((elementCount - 1) - i); j++) //���� �� ��� ���� ���� ������ ��Ҵ� ���� �� ������ �����ϹǷ�, ���� �� ��� �������� ����
		{
			switch (orderBy)
			{
			case ORDER_BY::ASCENDING:
				if (COMPARE(targetEnumerableSet[j], targetEnumerableSet[j + 1]) == 1) //a > b
				{
					SWAP(targetEnumerableSet[j], targetEnumerableSet[j + 1], tmp);
					sortPerformed = true;
				}

				break;

			case ORDER_BY::DESCENDING:
				if (COMPARE(targetEnumerableSet[j], targetEnumerableSet[j + 1]) == -1) //a < b
				{
					SWAP(targetEnumerableSet[j], targetEnumerableSet[j + 1], tmp);
					sortPerformed = true;
				}

				break;
			}
		}

		if (!sortPerformed) //������ �� ���� ������� ���� ��� (�̹� ���� ��Ұ� ��� ���� �Ǿ� ���� ���)
			break;
	}
}

/// <summary>
/// ���� ����
/// </summary>
/// <typeparam name="SortElementType">���� ��� Ÿ��</typeparam>
/// <param name="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
/// <param name="orderBy">���� ���</param>
template<typename SortElementType>
void InsertionSort(SortElementType targetEnumerableSet[], size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	/***
	* TODO : �ּ� ����
		< ���� ���� - �������� >

		1) ���� �� ��� ������ ���������� ���� ���� �� ��ҵ��� ���� ���� ù ��° ��� ~ �� ��° ����� �������� ����

		2) ���� �� ��� ���� ������ ������ ��Ҹ� �̿� �� �ٷ� ���� ��ҿ� ��

			2-1) �� ���� ���� ��Ұ� �̿� �� �ٷ� ���� ��Һ��� ���� ���
			: ���� �� ��� ���� ������ �� ���� ���� ��Ұ� ���� �� ������ ��ġ Ž��

				2-1-1) �� ���� ���� ��Ұ� ���� �� �� ������ ���� �ش� ��ġ�� ����
				: ���� �� ��ġ�� ���� ��Һ��� �� ���� ���� ��Ұ� �̿� �� �ٷ� ���� ��� ���� ������ ��ҵ��� �ڷ� �� ĭ�� �̵�

			2-2) �� ���� ���� ��Ұ� �̿� �� �ٷ� ���� ��Һ��� Ŭ ���
			: do nothing

		3) ���� �� ��� ���� 1 ���� �� 2)�� �̵�

		---

		< ��ҵ� �� �� Ƚ�� >

		n : ���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����

		ex) ��Ұ� 6���� ��, ù ��° �������� �� ���� ���� �� ��Ҹ� 1�� ��
		�� ���� �Ϸ� �� �� ���� 1�� ����
		1 + 2 + 3 + 4 + 5
		== 6 (���� ������ ��, ���� ��� ���� ��) * 2 (���� ������ ��, ���� ��� ���� ����) + 3 (���� ���� ���� ������)
		== 6 (���� ������ ��, ���� ��� ���� ��) * 2.5 (���� ������ ��, ���� ��� ���� ���� (n�� ¦�� �� ���, ���� ���� ���� �������� ����))

		---

		1 + 2 + 3 + ... + (n-2) + (n-1)
		���� ������ ��, ���� ��� ���� �� == n
		���� ������ ��, ���� ��� ���� ���� (n�� ¦�� �� ���, ���� ���� ���� �������� ����) == (n-1) / 2
		�̹Ƿ�,
		1 + 2 + 3 + ... + (n-2) + (n-1) == (n-1) + (n-2) + ... + 2 + 1 == n * ((n-1) / 2)

		---

		���� ���İ� ���Ͽ� ���� ������ �̷� �� ���� �� �� Ƚ���� ������,

		1) ��� �����Ͱ� �̹� ���� �Ǿ� �ִ� ��Ȳ (Best Case)

			1-1) ���� ����
			: ����, ������ �� �������� �� ���� ������ �߻����� �ʾ��� ��� (!sortPerformed),
			�̴� �̹� ���� �����Ͱ� ��� ���ĵǾ������� �ǹ��ϰ�,
			�̿� ����, �񱳸� �ߴ��ϹǷ� (n-1) �� �� Ƚ��

			1-2) ���� ����
			:


		2) ��� �����Ͱ� �����ϰ��� �ϴ� ����� �ݴ�� ���� �Ǿ� �ִ� ��Ȳ (Worst Case)

	***/

	SortElementType tmp;

	for (size_t i = 1; i < elementCount; i++)
	{
		switch (orderBy)
		{
		case ORDER_BY::ASCENDING:
			if (COMPARE(targetEnumerableSet[i - 1], targetEnumerableSet[i]) == 1) //a > b
			{
				tmp = targetEnumerableSet[i]; //�� ���� ���� ���

				for (size_t j = 0; j < i; j++) //���� �� ��� ���� ������ �� ���� ���� ��Ұ� ���� �� ������ ��ġ Ž��
				{
					if (COMPARE(targetEnumerableSet[j], tmp) == 1) //���� ��� > �� ���� ���� ���
					{
						memmove(
							&targetEnumerableSet[j + 1],
							&targetEnumerableSet[j],
							sizeof(targetEnumerableSet[0]) * (i-j)
						);//���� �� ��ġ�� ���� ��Һ��� �� ���� ���� ��Ұ� �̿� �� �ٷ� ���� ��� ���� ������ ��ҵ��� �ڷ� �� ĭ�� �̵�
						targetEnumerableSet[j] = tmp;
						break;
					}
				}
			}

			break;

		case ORDER_BY::DESCENDING:
			if (COMPARE(targetEnumerableSet[i - 1], targetEnumerableSet[i]) == -1) //a < b
			{
				tmp = targetEnumerableSet[i]; //�� ���� ���� ���

				for (size_t j = 0; j < i; j++) //���� �� ��� ���� ������ �� ���� ���� ��Ұ� ���� �� ������ ��ġ Ž��
				{
					if (COMPARE(targetEnumerableSet[j], tmp) == -1) //���� ��� < �� ���� ���� ���
					{
						memmove(
							&targetEnumerableSet[j + 1],
							&targetEnumerableSet[j],
							sizeof(targetEnumerableSet[0]) * (i - j)
						);//���� �� ��ġ�� ���� ��Һ��� �� ���� ���� ��Ұ� �̿� �� �ٷ� ���� ��� ���� ������ ��ҵ��� �ڷ� �� ĭ�� �̵�
						targetEnumerableSet[j] = tmp;
						break;
					}
				}
			}

			break;
		}
	}
}

/// <summary>
/// �� ����
/// </summary>
/// <typeparam name="SortElementType">���� ��� Ÿ��</typeparam>
/// <param name="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
/// <param name="orderBy">���� ���</param>
template<typename SortElementType>
void QuickSort(SortElementType targetEnumerableSet[], size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{

}
#endif