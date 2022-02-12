#ifndef _TRACE_HPP_
#define _TRACE_HPP_

extern const int LOGGING_LEVEL;
extern const bool VALIDATE_AFTER_SORT;

static std::mutex mutex;

/// <summary>
/// ���������� ���� ���� �� ��ҵ��� ������ �������� ��� ���
/// </summary>
/// <typeparam name="SortElementType">���� ��� Ÿ��</typeparam>
/// <param name="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
/// <param name="os">��� ��Ʈ��</param>
template<typename SortElementType>
void DispEnumerableSet(SortElementType targetEnumerableSet[], size_t elementCount, std::ostream& os = std::cout)
{
	os << ">> ";
	for (size_t i = 0; i < elementCount; i++)
		os << targetEnumerableSet[i] << " ";
	os << "\n";
}

/// <summary>
/// ���������� ���� ���� �� ��ҵ��� ������ ���� ������ ��� ���� �� �Ҵ�
/// </summary>
/// <typeparam name="SortElementType">���� ��� Ÿ��</typeparam>
/// <param name="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
template<typename SortElementType>
void GenRandPatternEnumerableSet(SortElementType targetEnumerableSet[], size_t elementCount)
{
	static std::random_device rand_device; //������� ������
	static std::mt19937 gen(rand_device()); //�޸��� Ʈ�����Ϳ� �õ� �Ҵ�
	static std::uniform_int_distribution<SortElementType> dist(0, elementCount); //���� �̻� ����

	for (size_t i = 0; i < elementCount; i++)
	{
		targetEnumerableSet[i] = dist(gen);
	}
}

/// <summary>
/// ���������� ���� ���� �� ��ҵ��� ������ ���� ���⿡ ���� ������ ������ ���� ��� ���� �� �Ҵ�
/// </summary>
/// <typeparam name="SortElementType">���� ��� Ÿ��</typeparam>
/// <param name="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
/// <param name="orderBy">���� ����</param>
template<typename SortElementType>
void GenSequentialPatternEnumerableSet(SortElementType targetEnumerableSet[], size_t elementCount, ORDER_BY orderBy)
{
	for (size_t i = 0; i < elementCount; i++)
	{
		switch (orderBy)
		{
		case ORDER_BY::ASCENDING:
			targetEnumerableSet[i] = i;
			break;

		case ORDER_BY::DESCENDING:
			targetEnumerableSet[i] = ((elementCount - 1) - i);
			break;
		}
	}
}

/// <summary>
/// ���� �� ���������� ���� ���� �� ��ҵ��� ���տ� ���� ���� ���⿡ ���� ��ȿ�� �˻� ����
/// </summary>
/// <typeparam name="SortElementType">���� ��� Ÿ��</typeparam>
/// <param name="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
/// <param name="orderBy">���� ����</param>
template<typename SortElementType>
void ValidateSortedEnumerableSet(SortElementType targetEnumerableSet[], size_t elementCount, ORDER_BY orderBy)
{
	for (size_t i = 0; i < elementCount - 1; i++) //������ ��� ���� ����
	{
		switch (orderBy)
		{
		case ORDER_BY::ASCENDING:
			if (targetEnumerableSet[i] > targetEnumerableSet[i + 1])
				throw std::logic_error(std::string(__func__) + std::string(" : Sort logic error (Asc)"));
			break;

		case ORDER_BY::DESCENDING:
			if (targetEnumerableSet[i] < targetEnumerableSet[i + 1])
				throw std::logic_error(std::string(__func__) + std::string(" : Sort logic error (Desc)"));
			break;
		}
	}
}

/// <summary>
/// ���� Pass�� ���� �Լ��� ���� ���� ���� �� ���� �� ���� ��� ���
/// </summary>
/// <typeparam name="SortElementType">���� ��� Ÿ��</typeparam>
/// <param name="sortFuncNameStr">���� �Լ� �̸� ���ڿ�</param>
/// <param name="sortFunc">���� �Լ�</param>
/// <param name="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param name="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
/// <param name="traceResult">���� Pass�� Trace ���</param>
template<typename SortElementType>
void RunSinglePassSortTrace(const char* sortFuncNameStr,
	void(*sortFunc)(SortElementType[], size_t, ORDER_BY),
	SortElementType targetEnumerableSet[], size_t elementCount,
	std::promise<TRACE_RESULT>& traceResult)
{
	std::chrono::system_clock::time_point startTime; //���� �ð�
	std::chrono::system_clock::time_point endTime; //���� �ð�
	std::chrono::nanoseconds descDuration = std::chrono::nanoseconds::zero(); //�������� ���� �ҿ� �ð�
	std::chrono::nanoseconds ascDuration = std::chrono::nanoseconds::zero(); //�������� ���� �ҿ� �ð�

	if (LOGGING_LEVEL == 2)
	{
		mutex.lock();

		std::cout << sortFuncNameStr << " : ���� �� �ʱ� ����\n";
		DispEnumerableSet<SortElementType>(targetEnumerableSet, elementCount);
		std::cout << std::endl;

		mutex.unlock();
	}

	startTime = std::chrono::system_clock::now();
	sortFunc(targetEnumerableSet, elementCount, ORDER_BY::ASCENDING); //�������� ����
	endTime = std::chrono::system_clock::now();
	ascDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

	if (VALIDATE_AFTER_SORT)
		ValidateSortedEnumerableSet<SortElementType>(targetEnumerableSet, elementCount, ORDER_BY::ASCENDING);

	if (LOGGING_LEVEL >= 1)
	{
		mutex.lock();

		if (LOGGING_LEVEL == 2)
		{
			std::cout << sortFuncNameStr << " (�������� ���� ��)\n";
			DispEnumerableSet<SortElementType>(targetEnumerableSet, elementCount);
			std::cout << std::endl;
		}

		std::cout << sortFuncNameStr << " (�������� ����) : " << ascDuration.count() << "ns �ҿ�\n" << std::endl;
		mutex.unlock();
	}

	startTime = std::chrono::system_clock::now();
	sortFunc(targetEnumerableSet, elementCount, ORDER_BY::DESCENDING); //�������� ����
	endTime = std::chrono::system_clock::now();
	descDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

	if (VALIDATE_AFTER_SORT)
		ValidateSortedEnumerableSet<SortElementType>(targetEnumerableSet, elementCount, ORDER_BY::DESCENDING);

	if (LOGGING_LEVEL >= 1)
	{
		mutex.lock();

		if (LOGGING_LEVEL == 2)
		{
			std::cout << sortFuncNameStr << " (�������� ���� �� �����Ϳ� ���� �������� ���� ��)\n";
			DispEnumerableSet<SortElementType>(targetEnumerableSet, elementCount);
			std::cout << std::endl;
		}

		std::cout << sortFuncNameStr << " (�������� ���� �� �����Ϳ� ���� �������� ����) : " << descDuration.count() << "ns �ҿ�\n" << std::endl;
		mutex.unlock();
	}

	TRACE_RESULT retVal;
	retVal = (descDuration + ascDuration);
	traceResult.set_value(retVal);
}
#endif