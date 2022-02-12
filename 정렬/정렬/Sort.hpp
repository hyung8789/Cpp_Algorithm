#ifndef _SORT_HPP_
#define _SORT_HPP_

#define LENGTH(array) ((sizeof(array)) / (sizeof(array[0])))
#define COMPARE(x, y) ((x) > (y) ? 1 : (x) == (y) ? 0 : -1) //x > y : 1, x == y : 0, x < y : -1
#define SWAP(x, y, tmp) ((tmp) = (x), (x) = (y), (y) = (tmp))

//#define RECURSIVE_METHOD //����� ��� (�ּ� ó�� : �ݺ��� ��� ���)
#ifndef RECURSIVE_METHOD 
#define ITERATIVE_METHOD //�ݺ��� ���
#endif

#define LOGGING_COMPARE_COUNT //�� Ƚ�� ī��Ʈ ��� �� ��� (�ּ� ó�� : ��Ȱ��ȭ)
#ifdef LOGGING_COMPARE_COUNT
extern class TRACE_RESULT;
extern enum class SORT_UNIQUE_MAPPED_INDEX;
extern struct SORT_METADATA;
extern class SORT_MAPPER;

#define ALTERNATIVE_COMPARE(x, y) SORT_MAPPER::GetInstance().\
GetRefSortMetaData(SORT_MAPPER::GetInstance().SortFuncNameStrToUniqueMappedIndex(__func__)).\
_traceResult.IncreaseCompareCount(), \
((x) > (y) ? 1 : (x) == (y) ? 0 : -1) //x > y : 1, x == y : 0, x < y : -1
#define COMPARE(x, y) ALTERNATIVE_COMPARE(x, y) //�� Ƚ�� ī��Ʈ ���� ȣ�� �� �Լ��� �̿�
#endif

/***
	< ���� ���� ��ҵ� �� �� Ƚ�� >

	n : ���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����

	ex1) ��Ұ� 6���� ��, ù ��° �������� �� ���� ���� ��� ��ҵ��� 5�� ��
	�� ���� �Ϸ� �� �� ���� 1�� ����

	�� ���� �Ϸ� �� �� �ں��� ���������� ���� �� ��Ҹ� �����Ͽ� �� �� ��� ���� �ϳ��� ����
	5 + 4 + 3 + 2 + 1
	= 6 (���� ������ ��, ���� ��� ���� ��) * 2 (���� ������ ��, ���� ��� ���� ����) + 3 (���� ���� ���� ������)
	= 6 (���� ������ ��, ���� ��� ���� ��) * 2.5 (���� ������ ��, ���� ��� ���� ���� (n�� ¦�� �� ���, ���� ���� ���� �������� ����))

	ex2) ��Ұ� 7���� ��,  ù ��° �������� �� ���� ���� ��� ��ҵ��� 6�� ��
	�� ���� �Ϸ� �� �� ���� 1�� ����
	6 + 5 + 4 + 3 + 2 + 1
	= 7 (���� ������ ��, ���� ��� ���� ��) * 3 (���� ������ ��, ���� ��� ���� ���� (n�� ¦�� �� ���, ���� ���� ���� �������� ����))

	���� ������ ��, ���� ��� ���� �� = n
	���� ������ ��, ���� ��� ���� ���� (n�� ¦�� �� ���, ���� ���� ���� �������� ����) = (n-1) / 2
	�̹Ƿ�,
	(n-1) + (n-2) + (n-3) + ... + (n-(n-2) + (n-(n-1)) = (n-1) + (n-2) + (n-3) + ... + 2 + 1 = n * ((n-1) / 2)

	---

	< ���� ���� ��ҵ� �� �� Ƚ�� >

	n : ���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����

	ex) ��Ұ� 6���� ��, ù ��° �������� �� ���� ���� �� ��Ҹ� 1�� ��
	�� ���� �Ϸ� �� �� ���� 1�� ����
	1 + 2 + 3 + 4 + 5
	= 6 (���� ������ ��, ���� ��� ���� ��) * 2 (���� ������ ��, ���� ��� ���� ����) + 3 (���� ���� ���� ������)
	= 6 (���� ������ ��, ���� ��� ���� ��) * 2.5 (���� ������ ��, ���� ��� ���� ���� (n�� ¦�� �� ���, ���� ���� ���� �������� ����))

	1 + 2 + 3 + ... + (n-2) + (n-1)
	���� ������ ��, ���� ��� ���� �� = n
	���� ������ ��, ���� ��� ���� ���� (n�� ¦�� �� ���, ���� ���� ���� �������� ����) = (n-1) / 2 �̹Ƿ�,
	1 + 2 + 3 + ... + (n-2) + (n-1) = (n-1) + (n-2) + ... + 2 + 1 = n * ((n-1) / 2)

	---

	< ���� ���İ� ���� ���� �� >

	�̷������� ���� ���� �� ���� ������ n * ((n-1) / 2) �� �� Ƚ���� ����
	�׷��� ����� ������ Ŀ������ Best Case ������ �׻� �����ϳ�,
	Worst Case, Average Case �� �ش������� ���� ������ �� ���� �� Ƚ���� ����

	���� ������ ���� ��κ� ��Ȳ���� n * ((n-1) / 2) �̰ų� ���� ����� ��ġ
	(���� ���� �� ���� �� ��� ������ ���� SWAP �� �� ���� ������� ���� ���
	��, �̹� ���� ��Ұ� ��� ���� �Ǿ� ���� ���
	�� �̻� �񱳸� �����ϹǷ� n * ((n-1) / 2) ���� ���� ���� �� ����)

	���� ������ ���� �� ��� ���� ������ �� ���� ���� ��Ұ� ���� �� ������ ��ġ��
	���� �� ��� ������ ó������ ���������� Ž�� ��������
	���� ������ �߻��� �������� �׻� ���� �� ��� ������ ��� ��ҵ��� ���� �� ������ �����ϰ�,
	���� �� �� �� ��� ������ �����ϳ�, �� ��� ���� ���� ���� ��� ���� ��ҵ��� �������� ���� �� ������ �����ϹǷ�,
	������ ������ ���Ͽ� ���� ���ĵǾ� �ִ� ���� �� ��� ���� ���� ��ҵ鿡 ���� ���� �� ��ġ�� ��� ã�� �� �ְų�,
	��������� (n-1) + n * ((n-1) / 2) / 2

	---

	< �� ������ ��ҵ� �� �� Ƚ�� >

	https://www.interviewbit.com/tutorial/quicksort-algorithm/#h827ske1ht0ea2fcemd1dxxl7u1ndu40q

	n : ���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����

	ex) ��Ұ� 8���� ��, ��� ���Ҹ��� ���� �� ������ �������� �и� �� ��� (Best Case)
	�ִ� ���� Ƚ�� = 4ȸ (1, 2, 4, 8) = log2(n)
	�� ���� ���� �߻� �ø��� �� Ƚ���� �� = nȸ
	�� ���� �������� �� Ƚ���� �� * �ִ� ���� Ƚ�� = n * log2(n)

	�� ���Ҹ��� �ϳ��� ��ҿ� �ش� ��Ҹ� ������ ��ҵ��� ���� (1 : n-1) ���� �и� �� ��� (Worst Case)
	�̹� ���� �� ��ҵ鿡 ���� ù ��° Ȥ�� ������ ��Ҹ� ���� (pivot)���� ���� �� ��Ȳ���� ������ �õ� �� ��� �߻��ϸ�,

	�� Ƚ�� = 7 + 6 + ... 2 + 1 = (n-1) + (n-2) + ... + 2 + 1 = n * ((n-1) / 2)

	�߾Ӱ��� ���� (pivot)���� �����ϰų� ������ �����Ͽ� ������ ���� (pivot)�� ���� �� ���,
	�� ���� �� �ʿ� �� ��� SWAP ���꿡 �־� ��� �ð���ŭ �� �ҿ������,
	O(n^2)���� O(nlog2(n)) ���� ���� �� ����
***/

enum class ORDER_BY : const int
{
	ASCENDING, //��������
	DESCENDING //��������
}; //���� ����

/// <summary>
/// ���� ���� (Best Case : O(n), Worst, Average Case : O(n^2))
/// </summary>
/// <typeparam name="SortElementType">���� ��� Ÿ��</typeparam>
/// <param name="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
/// <param name="orderBy">���� ����</param>
template<typename SortElementType>
void BubbleSort(SortElementType targetEnumerableSet[],
	size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	/***
		< ���� ���� - �������� >

		1) ���� �� ��� ������ ���������� ���� ���� �� ��ҵ��� ���� ���� ��� ��ҵ鿡 ���� ����

		2) ���� �� ��� ���� ���� ��� ��ҵ��� �̿� ��ҳ��� ���������� �� (���� ���⿡ ���� �� ���� ����)
		: �ε��� 0�� �ε��� 1�� ���, �ε��� 1�� �ε��� 2�� ���...

			2-1) �� ���� ���� ��Ұ� �̿� �� �ٷ� ���� ��Һ��� ���� ���

				2-1-1) �� ��Ҹ� SWAP

			2-2) �� ���� ���� ��Ұ� �̿� �� �ٷ� ���� ��Һ��� ũ�ų� ���� ���

				2-2-1) �� ��Ҹ� SWAP ���� �ʰ�, ���� �̿� �� ��ҵ鿡 ���� ��� ��

		3) ���� �� ��� ���� ���� ������ ��Ҵ� ���� �� ������ �����ϹǷ�, ���� �� ��� �������� ���� (�� ��� ���� 1 ����)
	***/

	SortElementType tmp;
	bool swapPerformed = false; //���� �� ��� ������ ���� SWAP �߻� ����

	for (size_t i = 0; i < (elementCount - 1); i++) //������ ��Ҵ� ���� ���� ���� ������, �� �ÿ��� ���� ����
	{
		swapPerformed = false;

		for (size_t j = 0; j < ((elementCount - 1) - i); j++) //���� �� ��� ���� ���� ������ ��Ҵ� ���� �� ������ �����ϹǷ�, ���� �� ��� �������� ����
		{
			switch (orderBy)
			{
			case ORDER_BY::ASCENDING:
				if (COMPARE(targetEnumerableSet[j], targetEnumerableSet[j + 1]) == 1) //a > b
				{
					SWAP(targetEnumerableSet[j], targetEnumerableSet[j + 1], tmp);
					swapPerformed = true;
				}

				break;

			case ORDER_BY::DESCENDING:
				if (COMPARE(targetEnumerableSet[j], targetEnumerableSet[j + 1]) == -1) //a < b
				{
					SWAP(targetEnumerableSet[j], targetEnumerableSet[j + 1], tmp);
					swapPerformed = true;
				}

				break;
			}
		}

		if (!swapPerformed) //���� �� ��� ������ ���� SWAP�� �� ���� ������� ���� ��� (�̹� ���� ��Ұ� ��� ���� �Ǿ� ���� ���)
			break;
	}
}

/// <summary>
/// ���� ����
/// </summary>
/// <typeparam name="SortElementType"></typeparam>
/// <param name="targetEnumerableSet"></param>
/// <param name="elementCount"></param>
/// <param name="orderBy"></param>
template<typename SortElementType>
void SelectionSort(SortElementType targetEnumerableSet[],
	size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	//TODO : ���� ����
}

/// <summary>
/// ���� ���� (Best Case : O(n), Worst, Average Case : O(n^2))
/// </summary>
/// <typeparam name="SortElementType">���� ��� Ÿ��</typeparam>
/// <param name="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
/// <param name="orderBy">���� ����</param>
template<typename SortElementType>
void InsertionSort(SortElementType targetEnumerableSet[],
	size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	/***
		< ���� ���� - �������� >

		1) ���� �� ��� ������ ���������� ���� ���� �� ��ҵ��� ���� ���� ù ��° ��� ~ �� ��° ����� �������� ����

		2) ���� �� ��� ���� ������ ������ ��Ҹ� �̿� �� �ٷ� ���� ��ҿ� �� (���� ���⿡ ���� �� ���� ����)

			2-1) �� ���� ���� ��Ұ� �̿� �� �ٷ� ���� ��Һ��� ���� ���

				2-1-1) ���� �� ��� ���� ������ �� ���� ���� ��Ұ� ���� �� ������ ��ġ�� ���� �� ��� ������ ó������ ���������� Ž��

				2-1-2) ���� �� ��ġ�� ���� ��Һ��� �� ���� ���� ��Ұ� �̿� �� �ٷ� ���� ��� ���� ������ ��ҵ��� �ڷ� �� ĭ�� �̵�

				2-1-3) �� ���� ���� ��Ҹ� �� ��ġ�� ����

			2-2) �� ���� ���� ��Ұ� �̿� �� �ٷ� ���� ��Һ��� Ŭ ���

				2-2-1) ���� �� ��� ���� ���� ������ ��Ҵ� ���� �� ������ �����ϹǷ�, ���� �� ��� ���� 1 ����
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
						memmove_s(
							&targetEnumerableSet[j + 1],
							sizeof(targetEnumerableSet[0]) * (i - j),
							&targetEnumerableSet[j],
							sizeof(targetEnumerableSet[0]) * (i - j)
						); //���� �� ��ġ�� ���� ��Һ��� �� ���� ���� ��Ұ� �̿� �� �ٷ� ���� ��� ���� ������ ��ҵ��� �ڷ� �� ĭ�� �̵�
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
						memmove_s(
							&targetEnumerableSet[j + 1],
							sizeof(targetEnumerableSet[0]) * (i - j),
							&targetEnumerableSet[j],
							sizeof(targetEnumerableSet[0]) * (i - j)
						); //���� �� ��ġ�� ���� ��Һ��� �� ���� ���� ��Ұ� �̿� �� �ٷ� ���� ��� ���� ������ ��ҵ��� �ڷ� �� ĭ�� �̵�
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
/// ���������� ���� ���� �� ��ҵ��� ���տ� ���� ���� (pivot) ����, ���� ���⿡ ���� ���� ���� �� ���� (pivot)�� ���� ���� �� ��ġ ��ȯ
/// </summary>
/// <typeparam name="SortElementType">���� ��� Ÿ��</typeparam>
/// <param name="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="srcLeftIndex">���������� ���� ���� �� ��ҵ��� ������ ���� �� ���� �ε��� (���� 0)</param>
/// <param name="srcRightIndex">���������� ���� ���� �� ��ҵ��� ������ ���� �� ������ �ε��� (���� ��ҵ��� ���� - 1)</param>
/// <param name="orderBy">���� ����</param>
/// <returns>���� (pivot)�� ���� ���� �� ��ġ</returns>
template<typename SortElementType>
size_t PartitioningProc(SortElementType targetEnumerableSet[],
	size_t srcLeftIndex, size_t srcRightIndex, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	/***
		< �� ���� - �������� >

		! SWAP ��ũ�� ġȯ �ÿ� SWAP ��� �ε����� �������� �ʰ�, ������ ���� ���� �� ��

		1) ������ ����� ���� ���� (pivot) ����
		: �߾Ӱ� ���� � ���� �ʿ� �� ��� ���� (pivot)�� ��ġ SWAP �Ͽ� �� �������� ������ ��

			1-1) ���� ���� (pivot)�� ���� ���� �� ��ġ�� ���� (pivot)���� �Ҵ�

		2) ���� (pivot)�� �Ǵ� ��Ҹ� ������ ������ ��ҵ鿡 ���� ���� �ε����� ������ �ε����� ���� �����Ǵ� �������� ����, �ݺ�

		3) ���� ���� �ε����� ��ҿ� ���� (pivot)�� �Ǵ� ��� �� (���� ���⿡ ���� �� ���� ����)

			3-1) ���� ���� �ε����� ��Ұ� ���� (pivot)���� ���� ���
			: ���� ���� �ε����� ��Ҵ� ���� (pivot)�� �Ǵ� ����� �������� �̵��ؾ� ��

				3-1-1) ���� (pivot)�� ���� ���� �� ��ġ ����

				3-1-2) ���� ���� �ε����� ��ҿ� ���� (pivot)�� ���� ���� �� ��ġ�� ��� SWAP
				: �̿� ����, ���� ���� (pivot)�� ���� ���� �� ��ġ�� �ε����� ����� ������ �׻� ���� (pivot)���� ���� ��Ҹ� ����

			3-2) ���� ���� �ε����� ��Ұ� ���� (pivot)���� ũ�ų� ���� ���
			: do nothing

		4) ���� �ε����� ������ �ε����� ���� �����Ǵ� ������ ����,
		: ���� ���� (pivot)�� ���� ���� �� ��ġ�� �ε����� ����� ������ �׻� ���� (pivot)���� ���� ��Ҹ� ����

			4-1) ���� (pivot)�� ���� ���� (pivot)�� ���� ���� �� ��ġ�� ��� SWAP

			4-2) ���� ���� (pivot)�� ���� ���� �� ��ġ�� �������� �ٽ� �����ϱ� ���� ���� ���� (pivot)�� ���� ���� �� ��ġ ��ȯ
	***/

SELECTION_MEDIAN_PROC: //�߾Ӱ����� ���� (pivot) ���� ó�� ��ƾ
	/***
		< �� ���� �߾Ӱ� ���� - �������� >

		1) ����, ����� ���� ���� (pivot) ���� (���� ���⿡ ���� �߾Ӱ� ���� ���� �� ���� ����)

		2) ���� �ε����� ���, ��� �ε����� ���, ������ �ε����� ��ҿ� ����,

		3) ���� �ε��� ��ҿ� ��� �ε��� ��� ��

			3-1) ���� �ε��� ��� > ��� �ε��� ���
			: ��� �ε����� ��Ұ� ���� �ε����� ����� ���ʿ� ��ġ�ϴ� ���� �ùٸ� ���� �� ��ġ�Ƿ�,
			��� �ε��� ��ҿ� ���� �ε��� ��� SWAP

			3-2) ���� �ε��� ��� <= ��� �ε��� ���
			: do nothing

		4) ���� �ε��� ��ҿ� ������ �ε��� ��� ��

			4-1) ���� �ε��� ��� > ������ �ε��� ���
			: ������ �ε����� ��Ұ� ���� �ε����� ����� ���ʿ� ��ġ�ϴ� ���� �ùٸ� ���� �� ��ġ�Ƿ�,
			���� �ε��� ��ҿ� ������ �ε��� ��� SWAP

			4-2) ���� �ε��� ��� <= ������ �ε��� ���
			: do nothing

		5) ��� �ε��� ��ҿ� ������ �ε��� ��� ��

			5-1) ��� �ε��� ��� > ������ �ε��� ���
			: ������ �ε����� ��Ұ� ��� �ε��� ����� ���ʿ� ��ġ�ϴ� ���� �ùٸ� ���� �� ��ġ�Ƿ�,
			������ ��ҿ� ��� �ε��� ��� SWAP

			5-2) ��� �ε��� ��� <= ������ �ε��� ���
			: do nothing

		6) ���� ��� ������ ����,

			6-1) ���� ��Ұ� 3�� ������ ���
			: �߾Ӱ��� ���� �� ���� �ε���, ��� �ε���, ������ �ε����� ��ҵ��� ���ĵǹǷ�,
			���� ���� ó�� ��ƾ ���� �� ��� �ε��� ��ȯ

			6-2) ���� ��Ұ� 3�� �ʰ��� ���
			: ���� ���� �ε����� ��Ҹ� ���� (pivot)���� �Ͽ�, ���� ���� ó�� ��ƾ ����

		---

		�߾Ӱ����� ���� (pivot) ���� ó�� ��ƾ�� ����, ��� �ð���ŭ �� Ƚ�� �� SWAP Ƚ���� ����
		�ش� �߾Ӱ��� ���������� ���� ���� �� ��ҵ��� ���� ������ ������ �߾Ӱ��̶�� ���� �� �� ������, Worst Case �߻� Ȯ���� ���� �� ����
	***/

	size_t midIndex = (size_t)((srcLeftIndex + srcRightIndex) >> 1); //��� �ε���
	SortElementType tmp;

	switch (orderBy)
	{
	case ORDER_BY::ASCENDING:
		if (COMPARE(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[midIndex]) == 1) //left > mid
			SWAP(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[midIndex], tmp);

		if (COMPARE(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[srcRightIndex]) == 1) //left > right
			SWAP(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[srcRightIndex], tmp);

		if (COMPARE(targetEnumerableSet[midIndex], targetEnumerableSet[srcRightIndex]) == 1) //mid > right
			SWAP(targetEnumerableSet[midIndex], targetEnumerableSet[srcRightIndex], tmp);

		break;

	case ORDER_BY::DESCENDING:
		if (COMPARE(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[midIndex]) == -1) //left < mid
			SWAP(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[midIndex], tmp);

		if (COMPARE(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[srcRightIndex]) == -1) //left < right
			SWAP(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[srcRightIndex], tmp);

		if (COMPARE(targetEnumerableSet[midIndex], targetEnumerableSet[srcRightIndex]) == -1) //mid < right
			SWAP(targetEnumerableSet[midIndex], targetEnumerableSet[srcRightIndex], tmp);

		break;
	}

	if ((midIndex + 1) == srcRightIndex) //���� ��Ұ� 3�� ������ ���
		return midIndex;
	else //���� ��Ұ� 3�� �ʰ��� ���
		goto SORT_PROC;

SORT_PROC: //���� ó�� ��ƾ
	size_t pivotIndex = srcLeftIndex; //���� (pivot) �ε���
	size_t orderedPivotIndex = pivotIndex; //���� (pivot)�� ���� ���� �� ��ġ

	for (srcLeftIndex += 1; srcLeftIndex <= srcRightIndex; srcLeftIndex++)
	{
		switch (orderBy)
		{
		case ORDER_BY::ASCENDING:
			if (COMPARE(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[pivotIndex]) == -1) //left < pivot
			{
				orderedPivotIndex++;
				SWAP(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[orderedPivotIndex], tmp);
			}

			break;

		case ORDER_BY::DESCENDING:
			if (COMPARE(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[pivotIndex]) == 1) //left > pivot
			{
				orderedPivotIndex++;
				SWAP(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[orderedPivotIndex], tmp);
			}

			break;
		}
	}

	SWAP(targetEnumerableSet[pivotIndex], targetEnumerableSet[orderedPivotIndex], tmp);
	return orderedPivotIndex;
}

/// <summary>
/// �� ���� (Best Case : O(nlog2(n)), Worst Case : O(n^2), Average Case : O(nlog2(n)))
/// </summary>
/// <typeparam name="SortElementType">���� ��� Ÿ��</typeparam>
/// <param name="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="srcLeftIndex">���������� ���� ���� �� ��ҵ��� ������ ���� �ε��� (���� 0)</param>
/// <param name="srcRightIndex">���������� ���� ���� �� ��ҵ��� ������ ������ �ε��� (���� ��ҵ��� ���� - 1)</param>
/// <param name="orderBy">���� ����</param>
template<typename SortElementType>
void QuickSort(SortElementType targetEnumerableSet[],
	size_t srcLeftIndex, size_t srcRightIndex, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
#ifdef RECURSIVE_METHOD
	if (srcLeftIndex >= srcRightIndex) //base case
		return;

	size_t orderedPivotIndex = 
		PartitioningProc<SortElementType>(targetEnumerableSet, srcLeftIndex, srcRightIndex, orderBy); //���� �� ���� (pivot)�� �ε���

	if (orderedPivotIndex > 0) //underflow ����
		QuickSort<SortElementType>(targetEnumerableSet,
			srcLeftIndex, orderedPivotIndex - 1, orderBy); //���� �� ���� (pivot)�� ���ʿ� ���� �ٽ� ����

	QuickSort<SortElementType>(targetEnumerableSet,
		orderedPivotIndex + 1, srcRightIndex, orderBy); //���� �� ���� (pivot)�� �����ʿ� ���� �ٽ� ����

#elif defined ITERATIVE_METHOD
	/***
		����, ���������� RECURSIVE_METHOD�� ���� ������ �������� ����
		RECURSIVE_METHOD�� ���� �� ���� ������ LCRS Ʈ���� < RECURSIVE_METHOD�� Call Stack ���� > �� ���� �� ��
	***/

	size_t* stack = new size_t[(srcRightIndex - srcLeftIndex) + 1]; //��ü ��� ������ŭ�� ����
	size_t top = 0; //������ �ֻ��� ��� �ε���

	//���� �ε���, ������ �ε��� ������ push, ������ �ε���, ���� �ε��� ������ pop
	stack[top++] = srcLeftIndex;
	stack[top++] = srcRightIndex;
	
	while (top > 0) //pop, PartitioningProc
	{
		srcRightIndex = stack[--top];
		srcLeftIndex = stack[--top];

		size_t orderedPivotIndex = 
			PartitioningProc<SortElementType>(targetEnumerableSet, srcLeftIndex, srcRightIndex, orderBy); //���� �� ���� (pivot)�� �ε���

		if (orderedPivotIndex > 0) //underflow ����
			if (orderedPivotIndex - 1 > srcLeftIndex) //���� �� ���� (pivot)�� ���ʿ� ���� ��Ұ� ���� �� ��� �ٽ� ����
			{
				stack[top++] = srcLeftIndex;
				stack[top++] = orderedPivotIndex - 1;
			}

		if (orderedPivotIndex + 1 < srcRightIndex) //���� �� ���� (pivot)�� �����ʿ� ���� ��Ұ� ���� �� ��� �ٽ� ����
		{
			stack[top++] = orderedPivotIndex + 1;
			stack[top++] = srcRightIndex;
		}
	}

	delete[] stack;
#endif
}

/// <summary>
/// �� ���� (Best Case : O(nlog2(n)), Worst Case : O(n^2), Average Case : O(nlog2(n)))
/// </summary>
/// <typeparam name="SortElementType">���� ��� Ÿ��</typeparam>
/// <param name="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
/// <param name="orderBy">���� ����</param>
template<typename SortElementType>
void QuickSort(SortElementType targetEnumerableSet[],
	size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	if (elementCount <= 1)
		return;

	QuickSort<SortElementType>(targetEnumerableSet,
		0, elementCount - 1, orderBy);
}

/// <summary>
/// ���� ����
/// </summary>
/// <typeparam name="SortElementType"></typeparam>
/// <param name="targetEnumerableSet"></param>
/// <param name="elementCount"></param>
/// <param name="orderBy"></param>
template<typename SortElementType>
void MergeSort(SortElementType targetEnumerableSet[],
	size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	//TODO : ���� ����
}
#endif