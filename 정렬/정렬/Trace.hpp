#ifndef _TRACE_HPP_
#define _TRACE_HPP_

extern const int LOGGING_LEVEL;
static std::mutex mutex; //����׿� ��� ��� ���� ���ؽ�

struct TRACE_RESULT
{
	std::chrono::nanoseconds accDuration = std::chrono::nanoseconds::zero(); //���� �ҿ� �ð�
	std::chrono::nanoseconds minDuration = std::chrono::nanoseconds::zero(); //�ּ� �ҿ� �ð�
	std::chrono::nanoseconds maxDuration = std::chrono::nanoseconds::zero(); //�ִ� �ҿ� �ð�

	/// <summary>
	/// ��ü �׽�Ʈ Pass�� ���� �ڽ��� Trace Result ���
	/// </summary>
	/// <param sortFuncNameStr="sortFuncNameStr">���� �Լ� �̸� ���ڿ�</param>
	/// <param sortFuncNameStr="totalTestPassCount">��ü �׽�Ʈ Pass Ƚ��</param>
	void DispTotalTestPassTraceResult(const char* sortFuncNameStr, size_t totalTestPassCount)
	{
		std::cout << sortFuncNameStr << " ----------------------------------------------\n";
		std::cout << ">> ��ü Pass�� �� �ҿ� �ð� : " << this->accDuration.count() << "ns\n";
		std::cout << ">> ��� �ҿ� �ð� : " << this->accDuration.count() / totalTestPassCount << "ns\n";
		std::cout << ">> �ּ� �ҿ� �ð� : " << this->minDuration.count() << "ns\n";
		std::cout << ">> �ִ� �ҿ� �ð� : " << this->maxDuration.count() << "ns\n";

#ifdef LOGGING_COMPARE_COUNT
		DispCompareCount(sortFuncNameStr);
#endif
		std::cout << std::endl;
	}

	TRACE_RESULT& operator+=(const TRACE_RESULT& newResult)
	{
		this->accDuration += newResult.accDuration;

		if (this->minDuration == std::chrono::nanoseconds::zero())
			this->minDuration = newResult.minDuration;

		if (this->minDuration > newResult.minDuration)
			this->minDuration = newResult.minDuration;

		if (this->maxDuration < newResult.maxDuration)
			this->maxDuration = newResult.maxDuration;

		return *this;
	}

	TRACE_RESULT& operator+=(const std::chrono::nanoseconds& newDuration)
	{
		this->accDuration += newDuration;

		if (this->minDuration == std::chrono::nanoseconds::zero())
			this->minDuration = newDuration;

		if (this->minDuration > newDuration)
			this->minDuration = newDuration;

		if (this->maxDuration < newDuration)
			this->maxDuration = newDuration;

		return *this;
	};

	TRACE_RESULT& operator=(const std::chrono::nanoseconds& newDuration)
	{
		this->accDuration = newDuration;
		this->minDuration = newDuration;
		this->maxDuration = newDuration;

		return *this;
	};
};

/// <summary>
/// ���������� ���� ���� �� ��ҵ��� ������ �������� ��� ���
/// </summary>
/// <typeparam sortFuncNameStr="SortElementType">���� ��� Ÿ��</typeparam>
/// <param sortFuncNameStr="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param sortFuncNameStr="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
/// <param sortFuncNameStr="os">��� ��Ʈ��</param>
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
/// <typeparam sortFuncNameStr="SortElementType">���� ��� Ÿ��</typeparam>
/// <param sortFuncNameStr="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param sortFuncNameStr="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
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
/// <typeparam sortFuncNameStr="SortElementType">���� ��� Ÿ��</typeparam>
/// <param sortFuncNameStr="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param sortFuncNameStr="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
/// <param name="orderBy">���� ����</param>
template<typename SortElementType>
void GenSequentialPatternEnumerableSet(SortElementType targetEnumerableSet[], size_t elementCount, ORDER_BY orderBy = ORDER_BY::DESCENDING)
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
/// ���� Pass�� ���� �Լ��� ���� ���� ���� �� ���� �� ���� ��� ���
/// </summary>
/// <typeparam sortFuncNameStr="SortElementType">���� ��� Ÿ��</typeparam>
/// <param sortFuncNameStr="sortFuncNameStr">���� �Լ� �̸� ���ڿ�</param>
/// <param sortFuncNameStr="sortFunc">���� �Լ�</param>
/// <param sortFuncNameStr="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param sortFuncNameStr="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
/// <param sortFuncNameStr="traceResult">���� Pass�� Trace ���</param>
template<typename SortElementType>
void RunSinglePassSortTrace(const char* sortFuncNameStr,
	void(*sortFunc)(SortElementType[], size_t, ORDER_BY),
	SortElementType targetEnumerableSet[], size_t elementCount,
	std::promise<TRACE_RESULT>& traceResult)
{
	std::chrono::system_clock::time_point startTime; //���� �ð�
	std::chrono::system_clock::time_point endTime; //���� �ð�
	std::chrono::nanoseconds descDuration = std::chrono::milliseconds::zero(); //�������� ���� �ҿ� �ð�
	std::chrono::nanoseconds ascDuration = std::chrono::milliseconds::zero(); //�������� ���� �ҿ� �ð�

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

	if (LOGGING_LEVEL >= 1)
	{
		mutex.lock();

		if (LOGGING_LEVEL == 2)
		{
			std::cout << sortFuncNameStr << " : �������� ���� ��\n";
			DispEnumerableSet<SortElementType>(targetEnumerableSet, elementCount);
			std::cout << std::endl;
		}

		std::cout << ">> " << sortFuncNameStr << " (�������� ����) : " << ascDuration.count() << "ns �ҿ�" << std::endl;
		DispCompareCount(sortFuncNameStr);

		mutex.unlock();
	}

	startTime = std::chrono::system_clock::now();
	sortFunc(targetEnumerableSet, elementCount, ORDER_BY::DESCENDING); //�������� ����
	endTime = std::chrono::system_clock::now();
	descDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

	if (LOGGING_LEVEL >= 1)
	{
		mutex.lock();

		if (LOGGING_LEVEL == 2)
		{
			std::cout << sortFuncNameStr << " : �������� ���� �� �����Ϳ� ���� �������� ���� ��\n";
			DispEnumerableSet<SortElementType>(targetEnumerableSet, elementCount);
			std::cout << std::endl;
		}

		std::cout << ">> " << sortFuncNameStr << " (�������� ���� �� �����Ϳ� ���� �������� ����) : " << descDuration.count() << "ns �ҿ�" << std::endl;
		DispCompareCount(sortFuncNameStr);

		mutex.unlock();
	}

	TRACE_RESULT retVal;
	retVal = (descDuration + ascDuration);
	traceResult.set_value(retVal);
}
#endif