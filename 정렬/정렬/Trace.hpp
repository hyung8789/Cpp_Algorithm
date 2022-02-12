#ifndef _TRACE_HPP_
#define _TRACE_HPP_

extern const int LOGGING_LEVEL;
extern const bool VALIDATE_AFTER_SORT;

static std::mutex mutex;

/// <summary>
/// 순차적으로 열거 가능 한 요소들의 집합의 순차적인 요소 출력
/// </summary>
/// <typeparam name="SortElementType">정렬 요소 타입</typeparam>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param name="os">출력 스트림</param>
template<typename SortElementType>
void DispEnumerableSet(SortElementType targetEnumerableSet[], size_t elementCount, std::ostream& os = std::cout)
{
	os << ">> ";
	for (size_t i = 0; i < elementCount; i++)
		os << targetEnumerableSet[i] << " ";
	os << "\n";
}

/// <summary>
/// 순차적으로 열거 가능 한 요소들의 집합의 임의 패턴의 요소 생성 및 할당
/// </summary>
/// <typeparam name="SortElementType">정렬 요소 타입</typeparam>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
template<typename SortElementType>
void GenRandPatternEnumerableSet(SortElementType targetEnumerableSet[], size_t elementCount)
{
	static std::random_device rand_device; //비결정적 생성기
	static std::mt19937 gen(rand_device()); //메르센 트위스터에 시드 할당
	static std::uniform_int_distribution<SortElementType> dist(0, elementCount); //균일 이산 분포

	for (size_t i = 0; i < elementCount; i++)
	{
		targetEnumerableSet[i] = dist(gen);
	}
}

/// <summary>
/// 순차적으로 열거 가능 한 요소들의 집합의 정렬 방향에 따른 순차적 패턴을 가진 요소 생성 및 할당
/// </summary>
/// <typeparam name="SortElementType">정렬 요소 타입</typeparam>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param name="orderBy">정렬 방향</param>
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
/// 정렬 된 순차적으로 열거 가능 한 요소들의 집합에 대해 정렬 방향에 따른 유효성 검사 수행
/// </summary>
/// <typeparam name="SortElementType">정렬 요소 타입</typeparam>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param name="orderBy">정렬 방향</param>
template<typename SortElementType>
void ValidateSortedEnumerableSet(SortElementType targetEnumerableSet[], size_t elementCount, ORDER_BY orderBy)
{
	for (size_t i = 0; i < elementCount - 1; i++) //마지막 요소 간접 접근
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
/// 단일 Pass의 정렬 함수에 따른 정렬 수행 및 성능 비교 위한 결과 출력
/// </summary>
/// <typeparam name="SortElementType">정렬 요소 타입</typeparam>
/// <param name="sortFuncNameStr">정렬 함수 이름 문자열</param>
/// <param name="sortFunc">정렬 함수</param>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param name="traceResult">단일 Pass의 Trace 결과</param>
template<typename SortElementType>
void RunSinglePassSortTrace(const char* sortFuncNameStr,
	void(*sortFunc)(SortElementType[], size_t, ORDER_BY),
	SortElementType targetEnumerableSet[], size_t elementCount,
	std::promise<TRACE_RESULT>& traceResult)
{
	std::chrono::system_clock::time_point startTime; //시작 시간
	std::chrono::system_clock::time_point endTime; //종료 시간
	std::chrono::nanoseconds descDuration = std::chrono::nanoseconds::zero(); //내림차순 정렬 소요 시간
	std::chrono::nanoseconds ascDuration = std::chrono::nanoseconds::zero(); //오름차순 정렬 소요 시간

	if (LOGGING_LEVEL == 2)
	{
		mutex.lock();

		std::cout << sortFuncNameStr << " : 정렬 전 초기 상태\n";
		DispEnumerableSet<SortElementType>(targetEnumerableSet, elementCount);
		std::cout << std::endl;

		mutex.unlock();
	}

	startTime = std::chrono::system_clock::now();
	sortFunc(targetEnumerableSet, elementCount, ORDER_BY::ASCENDING); //오름차순 정렬
	endTime = std::chrono::system_clock::now();
	ascDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

	if (VALIDATE_AFTER_SORT)
		ValidateSortedEnumerableSet<SortElementType>(targetEnumerableSet, elementCount, ORDER_BY::ASCENDING);

	if (LOGGING_LEVEL >= 1)
	{
		mutex.lock();

		if (LOGGING_LEVEL == 2)
		{
			std::cout << sortFuncNameStr << " (오름차순 정렬 후)\n";
			DispEnumerableSet<SortElementType>(targetEnumerableSet, elementCount);
			std::cout << std::endl;
		}

		std::cout << sortFuncNameStr << " (오름차순 정렬) : " << ascDuration.count() << "ns 소요\n" << std::endl;
		mutex.unlock();
	}

	startTime = std::chrono::system_clock::now();
	sortFunc(targetEnumerableSet, elementCount, ORDER_BY::DESCENDING); //내림차순 정렬
	endTime = std::chrono::system_clock::now();
	descDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

	if (VALIDATE_AFTER_SORT)
		ValidateSortedEnumerableSet<SortElementType>(targetEnumerableSet, elementCount, ORDER_BY::DESCENDING);

	if (LOGGING_LEVEL >= 1)
	{
		mutex.lock();

		if (LOGGING_LEVEL == 2)
		{
			std::cout << sortFuncNameStr << " (오름차순 정렬 된 데이터에 대한 내림차순 정렬 후)\n";
			DispEnumerableSet<SortElementType>(targetEnumerableSet, elementCount);
			std::cout << std::endl;
		}

		std::cout << sortFuncNameStr << " (오름차순 정렬 된 데이터에 대한 내림차순 정렬) : " << descDuration.count() << "ns 소요\n" << std::endl;
		mutex.unlock();
	}

	TRACE_RESULT retVal;
	retVal = (descDuration + ascDuration);
	traceResult.set_value(retVal);
}
#endif