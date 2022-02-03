#ifndef _TRACE_HPP_
#define _TRACE_HPP_

extern const int ELEMENT_COUNT;
extern const int TEST_PASSES;
extern const bool LOGGING_DEBUG_RESULT;

struct TRACE_RESULT
{
	std::chrono::nanoseconds accDuration = std::chrono::milliseconds::zero(); //���� �ҿ� �ð�
	std::chrono::nanoseconds minDuration = std::chrono::milliseconds::zero(); //�ּ� �ҿ� �ð�
	std::chrono::nanoseconds maxDuration = std::chrono::milliseconds::zero(); //�ִ� �ҿ� �ð�

	/// <summary>
	/// �ڽ��� Trace Result ���
	/// </summary>
	/// <param sortNameStr="sortNameStr">���� �̸� ���ڿ�</param>
	/// <param sortNameStr="totalTestPassCount">��ü �׽�Ʈ Pass Ƚ��</param>
	void dispTraceResult(const char* sortNameStr, size_t totalTestPassCount)
	{
		std::cout << "========================== " << sortNameStr << " ==========================\n";
		std::cout << "��ü Pass �� �� �ҿ� �ð� : " << this->accDuration.count() << "ns\n";
		std::cout << "��ü Pass �� ��� �ҿ� �ð� : " << this->accDuration.count() / TEST_PASSES << "ns\n";
		std::cout << "��ü Pass �� �ּ� �ҿ� �ð� : " << this->minDuration.count() << "ns\n";
		std::cout << "��ü Pass �� �ִ� �ҿ� �ð� : " << this->maxDuration.count() << "ns\n";
		std::cout << "===========================================================================\n";
	}

	/// <summary>
	/// ���� ������ ������
	/// </summary>
	/// <param name="newResult">�� Trace Result</param>
	void operator+=(TRACE_RESULT newResult)
	{
		this->accDuration += newResult.accDuration;

		if (this->minDuration == std::chrono::milliseconds::zero())
			this->minDuration = newResult.minDuration;

		if (this->minDuration > newResult.minDuration)
			this->minDuration = newResult.minDuration;

		if (this->maxDuration < newResult.maxDuration)
			this->maxDuration = newResult.maxDuration;
	}

	/// <summary>
	/// ���� ������ ������
	/// </summary>
	/// <param name="newDuration">�� �ҿ� �ð�</param>
	void operator+=(std::chrono::nanoseconds newDuration)
	{
		this->accDuration += newDuration;

		if (this->minDuration == std::chrono::milliseconds::zero())
			this->minDuration = newDuration;

		if (this->minDuration > newDuration)
			this->minDuration = newDuration;

		if (this->maxDuration < newDuration)
			this->maxDuration = newDuration;
	};

	/// <summary>
	/// ���� ������ ������
	/// </summary>
	/// <param name="newDuration">�� �ҿ� �ð�</param>
	void operator=(std::chrono::nanoseconds newDuration)
	{
		this->accDuration = newDuration;
		this->minDuration = newDuration;
		this->minDuration = newDuration;
		this->maxDuration = newDuration;
	};
};

/// <summary>
/// ���������� ���� ���� �� ��ҵ��� ������ �������� ��� ���
/// </summary>
/// <typeparam sortNameStr="SortElementType">���� ��� Ÿ��</typeparam>
/// <param sortNameStr="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param sortNameStr="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
/// <param sortNameStr="os">��� ��Ʈ��</param>
template<typename SortElementType>
void DispEnumerableSet(SortElementType targetEnumerableSet[], size_t elementCount, std::ostream& os = std::cout)
{
	for (size_t i = 0; i < elementCount; i++)
		os << targetEnumerableSet[i] << " ";
	os << "\n";
}

/// <summary>
/// ���������� ���� ���� �� ��ҵ��� ������ ������ ��� ���� �� �Ҵ�
/// </summary>
/// <typeparam sortNameStr="SortElementType">���� ��� Ÿ��</typeparam>
/// <param sortNameStr="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param sortNameStr="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
template<typename SortElementType>
void RandGenEnumerableSet(SortElementType targetEnumerableSet[], size_t elementCount)
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
/// ���� Pass�� ���� �Լ��� ���� ���� ���� �� ���� �� ���� ��� ���
/// </summary>
/// <typeparam sortNameStr="SortElementType">���� ��� Ÿ��</typeparam>
/// <param sortNameStr="sortNameStr">���� �̸� ���ڿ�</param>
/// <param sortNameStr="sortFunc">���� �Լ�</param>
/// <param sortNameStr="targetEnumerableSet">���������� ���� ���� �� ��ҵ��� ����</param>
/// <param sortNameStr="elementCount">���������� ���� ���� �� ��ҵ��� ������ ��ҵ��� ����</param>
/// <param sortNameStr="traceResult">���� Pass�� Trace ���</param>
/// <param sortNameStr="mutex">���ؽ�</param>
template<typename SortElementType>
void RunSinglePassSortTrace(const char* sortNameStr, 
	void(*sortFunc)(SortElementType[], size_t, ORDER_BY),
	SortElementType targetEnumerableSet[], size_t elementCount, 
	std::promise<TRACE_RESULT>& traceResult, std::mutex& mutex)
{
	std::chrono::system_clock::time_point startTime; //���� �ð�
	std::chrono::system_clock::time_point endTime; //���� �ð�
	std::chrono::nanoseconds descDuration = std::chrono::milliseconds::zero(); //�������� ���� �ҿ� �ð�
	std::chrono::nanoseconds ascDuration = std::chrono::milliseconds::zero(); //�������� ���� �ҿ� �ð�

	if (LOGGING_DEBUG_RESULT)
	{
		mutex.lock();
		std::cout << "--- " << sortNameStr << " : ���� �� �ʱ� ���� ---\n";
		DispEnumerableSet<SortElementType>(targetEnumerableSet, elementCount);
		mutex.unlock();
	}

	startTime = std::chrono::system_clock::now();
	sortFunc(targetEnumerableSet, elementCount, ORDER_BY::DESCENDING); //�������� ����
	endTime = std::chrono::system_clock::now();
	descDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

	if (LOGGING_DEBUG_RESULT)
	{
		mutex.lock();
		std::cout << "--- " << sortNameStr << " : �������� ���� �� ---\n";
		DispEnumerableSet<SortElementType>(targetEnumerableSet, elementCount);
		std::cout << ">> �������� ���� : " << descDuration.count() << "ns �ҿ�\n" << std::endl;
		mutex.unlock();
	}

	startTime = std::chrono::system_clock::now();
	sortFunc(targetEnumerableSet, elementCount, ORDER_BY::ASCENDING); //�������� ����
	endTime = std::chrono::system_clock::now();
	ascDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

	if (LOGGING_DEBUG_RESULT)
	{
		mutex.lock();
		std::cout << "--- " << sortNameStr << " :  Worst Case �� ���� �������� ���� �� ---\n";
		DispEnumerableSet<SortElementType>(targetEnumerableSet, elementCount);
		std::cout << ">> �������� ���� �� �����Ϳ� ���� �������� ���� (Worst Case) : " << ascDuration.count() << "ns �ҿ�\n" << std::endl;
		mutex.unlock();
	}

	TRACE_RESULT retVal;
	retVal = (descDuration + ascDuration);
	traceResult.set_value(retVal);
}
#endif