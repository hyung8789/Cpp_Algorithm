#ifndef _SORT_HPP_
#define _SORT_HPP_

#define LENGTH(array) ((sizeof(array)) / (sizeof(array[0])))
#define COMPARE(x, y) ((x) > (y) ? 1 : (x) == (y) ? 0 : -1) //x > y : 1, x == y : 0, x < y : -1
#define SWAP(x, y, tmp) ((tmp) = (x), (x) = (y), (y) = (tmp))

#define RECURSIVE_METHOD //����� ��� (�ּ� ó�� : �ݺ��� ��� ���)
#ifndef RECURSIVE_METHOD 
#define ITERATIVE_METHOD //�ݺ��� ���
#endif

#define LOGGING_COMPARE_COUNT //�� Ƚ�� ī��Ʈ ��� (�ּ� ó�� : ��Ȱ��ȭ)
#ifdef LOGGING_COMPARE_COUNT

extern const int LOGGING_LEVEL;
extern std::mutex mutex;

//TODO : XML �� �Է� �ּ� ����
//TODO : ���� �� ��� ���� �� Ƚ�� ī��Ʈ�� TRACE_RESULT�� ���� Ȥ�� ������ ��ü�� �и� �� ��
//TODO : �ּ����� ���� ���� : �� �Ȱ� depth�� �÷��� �ٹٲ� ���̱� (Partion ����)

static size_t compareCount[3] = { 0, }; //�� Ƚ�� ī��Ʈ

/// <summary>
/// ���� �Լ� �̸� ���ڿ��� �� Ƚ�� ī��Ʈ�� �ε����� ��ȯ
/// </summary>
/// <param name="sortFuncNameStr">���� �Լ� �̸� ���ڿ�</param>
/// <returns>���� �Լ� �̸� ���ڿ��κ��� ��ȯ �� �� Ƚ�� ī��Ʈ�� �ε���</returns>
size_t SortFuncNameStrToCompareCountIndex(const char* sortFuncNameStr)
{
	//TODO : promise future, traceResult �迭ȭ, ���� �Լ��� ���� ���� �ε��� ��� �Լ� �����
	return strcmp(sortFuncNameStr, "BubbleSort") == 0 ? 0 :
		strcmp(sortFuncNameStr, "InsertionSort") == 0 ? 1 :
		strcmp(sortFuncNameStr, "QuickSort") == 0 ? 2 :
		strcmp(sortFuncNameStr, "Partition") == 0 ? 2 : //�� ��Ʈ���� ȣ��
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));
}

/// <summary>
/// ���� �� Ƚ�� �ʱ�ȭ
/// </summary>
void ClearCompareCount()
{
	memset(compareCount, 0, sizeof(size_t) * LENGTH(compareCount));
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
void BubbleSort(SortElementType targetEnumerableSet[], 
	size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	/***
		< ���� ���� - �������� >

		1) ���� �� ��� ������ ���������� ���� ���� �� ��ҵ��� ���� ���� ��� ��ҵ鿡 ���� ����

		2) ���� �� ��� ���� ���� ��� ��ҵ��� �̿� ��ҳ��� ���������� �� (���� ���⿡ ���� �� ���� ����)
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
			: ���� �� ��� ������ ���� �� ���� SWAP�� �߻����� �ʾ��� ��� (!swapPerformed),
			�̴� �̹� ���� �����Ͱ� ��� ���ĵǾ������� �ǹ��ϰ�, �񱳸� �ߴ��ϹǷ� (n-1) �� �� Ƚ��

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

		�̷������� ���� ���� �� ���� ������ n * ((n-1) / 2) �� �� Ƚ���� ����
		�׷��� ����� ������ Ŀ������ Best Case ������ �׻� �����ϳ�, Worst Case, Average Case �� �ش������� ���� ������ �� ���� �� Ƚ���� ����

		���� ������ ���� ��κ� ��Ȳ���� n * ((n-1) / 2) �̰ų� ���� ����� ��ġ
		(���� ���� �� ���� �� ��� ������ ���� SWAP �� �� ���� ������� ���� ��� (�̹� ���� ��Ұ� ��� ���� �Ǿ� ���� ���)
		�� �̻� �񱳸� �����ϹǷ� n * ((n-1) / 2) ���� ���� ���� �� ����)

		���� ������ ���� �� ��� ���� ������ �� ���� ���� ��Ұ� ���� �� ������ ��ġ�� ���� �� ��� ������ ó������ ���������� Ž�� ��������
		���� ������ �߻��� �������� �׻� ���� �� ��� ������ ��� ��ҵ��� ���� �� ������ �����ϰ�,
		���� �� �� �� ��� ������ �����ϳ�, �� ��� ���� ���� ���� ��� ���� ��ҵ��� �������� ���� �� ������ �����ϹǷ�,
		������ ������ ���Ͽ� ���� ���ĵǾ� �ִ� ���� �� ��� ���� ���� ��ҵ鿡 ���� ���� �� ��ġ�� ��� ã�� �� �ְų�,
		��������� (n-1) + n * ((n-1) / 2) / 2
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
/// ���������� ���� ���� �� ��ҵ��� ���տ� ���� ���� �� �̵� ó��
/// </summary>
/// <typeparam name="SortElementType">���� ��� Ÿ��</typeparam>
/// <param name="targetEnumereableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="partitionLeftIndex">���������� ���� ���� �� ��ҵ��� ������ ���� �� ���� �ε���</param>
/// <param name="partitionRightIndex">���������� ���� ���� �� ��ҵ��� ������ ���� �� ������ �ε���</param>
/// <param name="orderBy"></param>
/// <returns></returns>
template<typename SortElementType>
size_t Partition(SortElementType targetEnumereableSet[], 
	size_t partitionLeftIndex, size_t partitionRightIndex, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	/***
		< Partitioning - �������� >

		1) ���� ȣ�� ������ partitionLeftIndex �� ���������� ���� ���� �� ��ҵ��� ������ ó������ ����
		
		2) ���� ȣ�� ������ partitionRightIndex �� ���������� ���� ���� �� ��ҵ��� ������ ���������� ����

		3) ���� (pivot)�� ���� ���� �� ���������� ���� ���� �� ��ҵ��� ������ ù ��° ��� (partitionLeftIndex)�� ����

		4) ���� (pivot)�� �Ǵ� ��Ҹ� ������ ������ ��ҵ鿡 ���� partitionLeftIndex �� partitionRightIndex �� �����Ǵ� �������� ���� �ݺ�

			4-1) partitionLeftIndex �� ���� (pivot)�� �Ǵ� ��Ұ� ���� �� ���� ��ġ (partitionLeftIndex + 1)���� ����

		5) ���� partitionLeftIndex �� ��ҿ� ���� (pivot)�� �Ǵ� ��� �� (���� ���⿡ ���� �� ���� ����)

			5-1) ���� partitionLeftIndex �� ��Ұ� ���� (pivot)���� Ŭ ���

				2-1-1) ���� partitionLeftIndex �� ��Ҵ� ���� (pivot)�� �Ǵ� ����� ���������� �̵��ؾ��ϹǷ�,
				���� partitionRightIndex �� ��ҿ� ���� partitionLeftIndex �� ��� SWAP

				2-1-2) ���� partitionLeftIndex �� ��ҿ� ���� partitionRightIndex �� ����� SWAP �� �߻� �� ������,
				���� partitionRightIndex �� ��Ҵ� ���� (pivot)�� �Ǵ� ��Һ��� ū ����̹Ƿ� 
				���� partitionRightIndex �� ��ġ�� �̵� (partitionRightIndex - 1)

			2-2) ���� partitionLeftIndex �� ��Ұ� ���� (pivot)���� �۰ų� ���� ���
			: do nothing

		6) ���� partitionLeftIndex �� ��Ҵ� ���� (pivot)���� �۰ų� �����Ƿ�, ���� partitionLeftIndex �� ��ġ�� �̵� (partitionLeftIndex + 1)

		7) partitionLeftIndex �� partitionRightIndex �� �����Ǵ� ������ ����,

			7-1) partitionLeftIndex �� �����Ͽ� partitionLeftIndex �� ���ʿ��� ���� (pivot)���� �۰ų� ���� ��ҵ鸸 ����

			7-2) partitionRightIndex �� �����ʿ��� ���� (pivot)���� ū ��ҵ鸸 ����

			7-3) ���� ���� ������ �����ϱ� ����, ���� (pivot)�� partitionLeftIndex �� ��� SWAP

			7-4) �̿� ����, ���� (pivot)�� ���ʿ��� ���� (pivot)���� �۰ų� ���� ��ҵ鸸 ����, ���� (pivot)�� �����ʿ��� ���غ��� ū ��ҵ鸸 ����

		8) ���� (pivot)�� ������ �����ִ� ��ҵ鿡 ���� ���� Partitioning ó���� ���� ���� partitionRightIndex ��ȯ
		
			8-1) �ش� ��ȯ ���� ���� �Է� �� ���� (pivot)�� ������ �����ִ� ���� ��ҿ� ���� ó���� ���� ������ ���� �Է� �� ��
			: partitionLeftIndex (Arg) = Caller Left Index, partitionRightIndex (Arg) = partitionRightIndex (retVal) - 1

			8-2) �ش� ��ȯ ���� ���� �Է� �� ���� (pivot)�� ������ �����ִ� ������ ��ҿ� ���� ó���� ���� ������ ���� �Է� �� ��
			: partitionLeftIndex (Arg) = partitionRightIndex (retVal) + 1, partitionRightIndex (Arg) = Caller Right Index
	***/

	if(partitionLeftIndex < 0 || partitionRightIndex < 0 || (partitionLeftIndex > partitionRightIndex))
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));
	
	size_t pivotIndex = partitionLeftIndex++; //���� (pivot)�� �Ǵ� ��� �ε���
	size_t tmp;

	while (partitionLeftIndex < partitionRightIndex) //���� (pivot)�� �Ǵ� ��Ҹ� ������ ������ ��ҵ鿡 ����
	{
		switch (orderBy)
		{
		case ORDER_BY::ASCENDING:
			if (COMPARE(targetEnumereableSet[partitionLeftIndex], targetEnumereableSet[pivotIndex]) == 1) //a > b
			{
				SWAP(targetEnumereableSet[partitionLeftIndex],
					targetEnumereableSet[partitionRightIndex],
					tmp); //���� partitionLeftIndex �� ��Ҵ� ���� (pivot)�� �Ǵ� ����� ���������� �̵�

				partitionRightIndex--;
			}

			partitionLeftIndex++;
			break;

		case ORDER_BY::DESCENDING:
			if (COMPARE(targetEnumereableSet[partitionLeftIndex], targetEnumereableSet[pivotIndex]) == -1) //a < b
			{
				SWAP(targetEnumereableSet[partitionLeftIndex],
					targetEnumereableSet[partitionRightIndex],
					tmp); //���� partitionLeftIndex �� ��Ҵ� ���� (pivot)�� �Ǵ� ����� ���������� �̵�

				partitionRightIndex--;
			}

			partitionLeftIndex++;
			break;
		}
	}

	SWAP(targetEnumereableSet[pivotIndex], 
		targetEnumereableSet[partitionRightIndex], 
		tmp); //���� ���� ������ �����ϱ� ����, ���� (pivot)�� partitionLeftIndex �� ��� SWAP
	return partitionRightIndex;
}

/// <summary>
/// �� ����
/// </summary>
/// <typeparam name="SortElementType">���� ��� Ÿ��</typeparam>
/// <param name="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="leftIndex">���������� ���� ���� �� ��ҵ��� ������ ���� �ε��� (���� 0)</param>
/// <param name="rightIndex">���������� ���� ���� �� ��ҵ��� ������ ������ �ε��� (���� ��ҵ��� ���� - 1)</param>
/// <param name="orderBy">���� ����</param>
template<typename SortElementType>
void QuickSort(SortElementType targetEnumerableSet[], 
	size_t leftIndex, size_t rightIndex, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	if (leftIndex < 0 || rightIndex < 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (leftIndex < rightIndex) //���������� ���� ���� �� ��ҵ��� ���տ� ���� ���� �� �� �ִ� ���
	{
		size_t partitionRightIndex = Partition(targetEnumerableSet, leftIndex, rightIndex); //���� �� ������ �ε���

		QuickSort<SortElementType>(targetEnumerableSet, leftIndex, partitionRightIndex - 1, orderBy); //���� �� ������ �ε��� ���� �����ִ� ���ʿ� ���� ó��
		QuickSort<SortElementType>(targetEnumerableSet, partitionRightIndex + 1, rightIndex, orderBy); //���� �� ������ �ε��� ���� �����ִ� �����ʿ� ���� ó��
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
void QuickSort(SortElementType targetEnumerableSet[], 
	size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	/***
	//TODO : �ּ� ����

		< �� ���� - �������� >

		1) ���������� ���� ���� �� ��ҵ��� ���� ���� ��� ��ҵ鿡 ���� ���� (pivot)�� �Ǵ� ������ ��Ҹ� ����

			M1) ���� ��ҵ� �� ù ��° ��Ҹ� ���� (pivot)���� �����ϴ� ���

			M2) ���� ��ҵ� �� ������ ��Ҹ� ���� (pivot)���� �����ϴ� ���

			M3) ���� ��ҵ� �� ���õ��� ���� ���� (pivot)�� ���� ������ ������ ������ ���� �� ���� (pivot)���� �����ϴ� ���

			M4) ���� ��ҵ� �� �߾Ӱ� (median)�� ���� (pivot)���� �����ϴ� ���

		2) ���������� ���� ���� �� ��ҵ��� ���տ� ���� �� �̻� ���� �� �� ���� ������, ���� �ݺ�

		3) ���� ���� (pivot)�� �Ǵ� ��Ҹ� ������ ������ ��ҵ鿡 ����

			3-1) ���� ���� (pivot)�� �Ǵ� ��Ҹ� ������ ������ ��� < ���� ���� (pivot)�� �Ǵ� ���
			: ���� (pivot)�� �Ǵ� ��Ҹ� ������ ������ ��Ҹ� ���� (pivot)�� �Ǵ� ����� �������� �̵�

			3-2) ���� ���� (pivot)�� �Ǵ� ��Ҹ� ������ ������ ��� >= ���� ���� (pivot)�� �Ǵ� ���
			: ���� (pivot)�� �Ǵ� ��Ҹ� ������ ������ ��Ҹ� ���� (pivot)�� �Ǵ� ����� ���������� �̵�
	***/

	

//	size_t partitionLeftIndex; //���� �� ���� �ε���
	//size_t partitionRightIndex; //���� �� ������ �ε���
//	size_t partitionPivotIndex; //���� �� ���� (pivot)�� �Ǵ� ��� �ε���
//	size_t tmp;

#ifdef RECURSIVE_METHOD
	QuickSort<SortElementType>(targetEnumerableSet, 0, elementCount - 1, orderBy);

#elif defined ITERATIVE_METHOD

#endif
}

#endif