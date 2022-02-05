#ifndef _SORT_HPP_
#define _SORT_HPP_

#define LENGTH(array) ((sizeof(array)) / (sizeof(array[0])))
#define COMPARE(x, y) ((x) > (y) ? 1 : (x) == (y) ? 0 : -1) //x > y : 1, x == y : 0, x < y : -1
#define SWAP(x, y, tmp) ((tmp) = (x), (x) = (y), (y) = (tmp))

#define LOGGING_COMPARE_COUNT //�� Ƚ�� ī��Ʈ ��� (�ּ� ó�� : ��Ȱ��ȭ)
#ifdef LOGGING_COMPARE_COUNT

extern const int LOGGING_LEVEL;
extern std::mutex mutex;

static size_t compareCount[3] = { 0, }; //�� Ƚ�� ī��Ʈ

/// <summary>
/// ���� �Լ� �̸� ���ڿ��� �� Ƚ�� ī��Ʈ�� �ε����� ��ȯ
/// </summary>
/// <param name="sortFuncNameStr">���� �Լ� �̸� ���ڿ�</param>
/// <returns>���� �Լ� �̸� ���ڿ��κ��� ��ȯ �� �� Ƚ�� ī��Ʈ�� �ε���</returns>
size_t SortFuncNameStrToCompareCountIndex(const char* sortFuncNameStr)
{
	return strcmp(sortFuncNameStr, "BubbleSort") == 0 ? 0 :
		strcmp(sortFuncNameStr, "InsertionSort") == 0 ? 1 :
		strcmp(sortFuncNameStr, "QuickSort") == 0 ? 2 :
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));
}

/// <summary>
/// ���� �� Ƚ�� �ʱ�ȭ
/// </summary>
void ClearCompareCount()
{
	memset(compareCount, 0, LENGTH(compareCount));
}

/// <summary>
/// ���Ŀ� ���� ���� �� Ƚ�� ���
/// </summary>
/// <param name="sortFuncNameStr">���� �Լ� �̸� ���ڿ�</param>
void DispCompareCount(const char* sortFuncNameStr)
{
	std::cout << ">> " << sortFuncNameStr << "�� �� �� Ƚ�� : " << \
		compareCount[SortFuncNameStrToCompareCountIndex(sortFuncNameStr)] << "ȸ" << std::endl;
}

/// <summary>
/// ���Ŀ� ���� ���� �� Ƚ�� ��ȯ
/// </summary>
/// <param name="sortFuncNameStr"></param>
/// <returns></returns>
size_t GetCompareCount(const char* sortFuncNameStr)
{
	return compareCount[SortFuncNameStrToCompareCountIndex(sortFuncNameStr)];
}

/// <summary>
/// ���Ŀ� ���� �� Ƚ�� ���� �� �� ��� ��ȯ
/// </summary>
/// <param name="sortFuncNameStr">���� �Լ� �̸� ���ڿ�</param>
template<typename SortElementType>
int CompareAndLoggingCompareCount(SortElementType x, SortElementType y, const char* sortFuncNameStr)
{
	compareCount[SortFuncNameStrToCompareCountIndex(sortFuncNameStr)]++;

	if (LOGGING_LEVEL == 2)
	{
		mutex.lock();

		std::cout << sortFuncNameStr << " : " << x << "��(��) " << y << " �� �߻�\n";

		mutex.unlock();
	}

	return COMPARE(x, y);
}

#define COMPARE(x, y) CompareAndLoggingCompareCount(x,y, __func__) //�� Ƚ�� ī��Ʈ ���� ȣ�� �� �Լ��� �̿�
#endif

enum class ORDER_BY : const int
{
	ASCENDING, //��������
	DESCENDING //��������
}; //���� ����

/// <summary>
/// ���� ����
/// </summary>
/// <typeparam name="SortElementType">���� ��� Ÿ��</typeparam>
/// <param name="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
/// <param name="orderBy">���� ����</param>
template<typename SortElementType>
void BubbleSort(SortElementType targetEnumerableSet[], size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	/***
		< ���� ���� - �������� >

		1) ���� �� ��� ������ ���������� ���� ���� �� ��ҵ��� ���� ���� ��� ��ҵ鿡 ���� ����

		2) ���� �� ��� ���� ���� ��� ��ҵ��� �̿� ��ҳ��� ���������� ��
		: �ε��� 0�� �ε��� 1�� ���, �ε��� 1�� �ε��� 2�� ���...

			2-1) �� ���� ���� ��Ұ� �̿� �� �ٷ� ���� ��Һ��� ���� ���
			: �� ��Ҹ� SWAP

			2-2) �� ���� ���� ��Ұ� �̿� �� �ٷ� ���� ��Һ��� ũ�ų� ���� ���
			: �� ��Ҹ� SWAP ���� �ʰ�, ���� �̿� �� ��ҵ鿡 ���� ��� ��

		3) ���� �� ��� ���� ���� ������ ��Ҵ� ���� �� ������ �����ϹǷ�, ���� �� ��� �������� ���� (�� ��� ���� 1 ����)

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
/// <param name="orderBy">���� ����</param>
template<typename SortElementType>
void InsertionSort(SortElementType targetEnumerableSet[], size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	/***
		< ���� ���� - �������� >

		1) ���� �� ��� ������ ���������� ���� ���� �� ��ҵ��� ���� ���� ù ��° ��� ~ �� ��° ����� �������� ����

		2) ���� �� ��� ���� ������ ������ ��Ҹ� �̿� �� �ٷ� ���� ��ҿ� ��

			2-1) �� ���� ���� ��Ұ� �̿� �� �ٷ� ���� ��Һ��� ���� ���
			: ���� �� ��� ���� ������ �� ���� ���� ��Ұ� ���� �� ������ ��ġ�� ���� �� ��� ������ ó������ ���������� Ž���Ͽ�,
			���� �� ��ġ�� ���� ��Һ��� �� ���� ���� ��Ұ� �̿� �� �ٷ� ���� ��� ���� ������ ��ҵ��� �ڷ� �� ĭ�� �̵��ϰ�
			�� ���� ���� ��Ҹ� �� ��ġ�� ����

			2-2) �� ���� ���� ��Ұ� �̿� �� �ٷ� ���� ��Һ��� Ŭ ���
			: ���� �� ��� ���� ���� ������ ��Ҵ� ���� �� ������ �����ϹǷ�, ���� �� ��� ���� 1 ����

		---

		< ��ҵ� �� �� Ƚ�� >

		n : ���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����

		ex) ��Ұ� 6���� ��, ù ��° �������� �� ���� ���� �� ��Ҹ� 1�� ��
		�� ���� �Ϸ� �� �� ���� 1�� ����
		1 + 2 + 3 + 4 + 5
		== 6 (���� ������ ��, ���� ��� ���� ��) * 2 (���� ������ ��, ���� ��� ���� ����) + 3 (���� ���� ���� ������)
		== 6 (���� ������ ��, ���� ��� ���� ��) * 2.5 (���� ������ ��, ���� ��� ���� ���� (n�� ¦�� �� ���, ���� ���� ���� �������� ����))

		1 + 2 + 3 + ... + (n-2) + (n-1)
		���� ������ ��, ���� ��� ���� �� == n
		���� ������ ��, ���� ��� ���� ���� (n�� ¦�� �� ���, ���� ���� ���� �������� ����) == (n-1) / 2 �̹Ƿ�,
		1 + 2 + 3 + ... + (n-2) + (n-1) == (n-1) + (n-2) + ... + 2 + 1 == n * ((n-1) / 2)

		---

		< ���� ���İ� ���� ���� �� >

		1) �̹� ���� �Ǿ� �ִ� ��Ȳ (Best Case)
		: 0, 1, 2 �� ���� �������� ����

			1-1) ���� ����
			: ����, ������ �� �������� �� ���� ������ �߻����� �ʾ��� ��� (!sortPerformed),
			�̴� �̹� ���� �����Ͱ� ��� ���ĵǾ������� �ǹ��ϰ�,
			�̿� ����, �񱳸� �ߴ��ϹǷ� (n-1) �� �� Ƚ��

			< ���� ���� ��� >

			BubbleSort : 0��(��) 1 �� �߻�
			BubbleSort : 1��(��) 2 �� �߻�

			1-2) ���� ����
			: (n-1) �� �� Ƚ��

			< ���� ���� ��� >

			InsertionSort : 0��(��) 1 �� �߻�
			InsertionSort : 1��(��) 2 �� �߻�

		2) �����ϰ��� �ϴ� ����� �ݴ�� ���� �Ǿ� �ִ� ��Ȳ (Worst Case)
		: 2 1 0 �� ���� �������� ����

			2-1) ���� ����
			: 2 1 0 => 2��(��) 1�� ���Ͽ� �� 1ȸ ��
				1 2 0 => 2��(��) 0�� ���Ͽ� �� 1ȸ ��
					1 0 2 => 1��(��) 0�� ���Ͽ� �� 1ȸ ��
						0 1 2

			< ���� ���� ��� >

			BubbleSort : 2��(��) 1 �� �߻�
			BubbleSort : 2��(��) 0 �� �߻�
			BubbleSort : 1��(��) 0 �� �߻�

			2-2) ���� ����
			: 2 1 0 => 2��(��) 1�� ���ϰ�, 1�� ������ ��ġ Ž���� ���� 2��(��) ���Ͽ� �� 2ȸ ��
				1 2 0 => 0��(��) 2�� ���ϰ�, 0�� ������ ��ġ Ž���� ���� 1��(��) ���Ͽ� �� 2ȸ ��
					0 1 2

			< ���� ���� ��� >

			InsertionSort : 2��(��) 1 �� �߻�
			InsertionSort : 2��(��) 1 �� �߻�
			InsertionSort : 2��(��) 0 �� �߻�
			InsertionSort : 1��(��) 0 �� �߻�

		�̷������� ���� ���� �� ���� ������ n * ((n-1) / 2) �� �� Ƚ���� ������.
		�׷��� ����� ������ Ŀ������ Best Case ������ �׻� �����ϳ�, Worst Case, Average Case �� �ش������� ���� ������ �� ���� �� Ƚ���� ������.
		
		���� ������ SWAP�� ���� �� ���� �� SWAP ������ �Ϸ� �� ������ ���� �� ��� ������ ������ ��Ҵ� �ùٸ� ���� �� ������ �����ϳ�,
		���� �� �� �� ��� ������ �پ���, ���� �� ������ �����ϴ� ������ ��Ұ� ���ܵǾ����Ƿ�,
		������ ������ ���Ͽ� ���� ���ĵǾ� ���� ���� ���� �� ��� ���� ���� ��ҵ鿡 ���� �� Ƚ���� n * ((n-1) / 2) �̴�.
		(������ 100���� �����Ϳ� ���� ���� ������ Worst Case�� ���, n * ((n-1) / 2)�� 4950ȸ�� �񱳰� �߻�)

		���� ������ ���� �� ��� ���� ������ �� ���� ���� ��Ұ� ���� �� ������ ��ġ�� ���� �� ��� ������ ó������ ���������� Ž�� ��������
		���� ������ �߻��� �������� �׻� ���� �� ��� ������ ��� ��ҵ��� ���� �� ������ �����ϰ�,
		���� �� �� �� ��� ������ �����ϳ�, �� ��� ���� ���� ���� ��� ���� ��ҵ��� �������� ���� �� ������ �����ϹǷ�,
		������ ������ ���Ͽ� ���� ���ĵǾ� �ִ� ���� �� ��� ���� ���� ��ҵ鿡 ���� ���� �� ��ġ�� ��� ã�� �� �ְų�, 
		��������� (n-1) + n * ((n-1) / 2) / 2 �̴�.
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
						memmove_s(
							&targetEnumerableSet[j + 1],
							sizeof(targetEnumerableSet[0]) * (i - j),
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
/// <param name="orderBy">���� ����</param>
template<typename SortElementType>
void QuickSort(SortElementType targetEnumerableSet[], size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{

}
#endif