#ifndef _TRACE_HPP_
#define _TRACE_HPP_

extern const int LOGGING_LEVEL;
static std::mutex mutex; //디버그용 결과 출력 위한 뮤텍스

struct TRACE_RESULT
{
	std::chrono::nanoseconds accDuration = std::chrono::nanoseconds::zero(); //누적 소요 시간
	std::chrono::nanoseconds minDuration = std::chrono::nanoseconds::zero(); //최소 소요 시간
	std::chrono::nanoseconds maxDuration = std::chrono::nanoseconds::zero(); //최대 소요 시간

	/// <summary>
	/// 전체 테스트 Pass에 대한 자신의 Trace Result 출력
	/// </summary>
	/// <param sortFuncNameStr="sortFuncNameStr">정렬 함수 이름 문자열</param>
	/// <param sortFuncNameStr="totalTestPassCount">전체 테스트 Pass 횟수</param>
	void DispTotalTestPassTraceResult(const char* sortFuncNameStr, size_t totalTestPassCount)
	{
		std::cout << sortFuncNameStr << " ----------------------------------------------\n";
		std::cout << ">> 전체 Pass의 총 소요 시간 : " << this->accDuration.count() << "ns\n";
		std::cout << ">> 평균 소요 시간 : " << this->accDuration.count() / totalTestPassCount << "ns\n";
		std::cout << ">> 최소 소요 시간 : " << this->minDuration.count() << "ns\n";
		std::cout << ">> 최대 소요 시간 : " << this->maxDuration.count() << "ns\n";

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
/// 순차적으로 열거 가능 한 요소들의 집합의 순차적인 요소 출력
/// </summary>
/// <typeparam sortFuncNameStr="SortElementType">정렬 요소 타입</typeparam>
/// <param sortFuncNameStr="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param sortFuncNameStr="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param sortFuncNameStr="os">출력 스트림</param>
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
/// <typeparam sortFuncNameStr="SortElementType">정렬 요소 타입</typeparam>
/// <param sortFuncNameStr="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param sortFuncNameStr="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
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
/// <typeparam sortFuncNameStr="SortElementType">정렬 요소 타입</typeparam>
/// <param sortFuncNameStr="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param sortFuncNameStr="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param name="orderBy">정렬 방향</param>
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
/// 단일 Pass의 정렬 함수에 따른 정렬 수행 및 성능 비교 위한 결과 출력
/// </summary>
/// <typeparam sortFuncNameStr="SortElementType">정렬 요소 타입</typeparam>
/// <param sortFuncNameStr="sortFuncNameStr">정렬 함수 이름 문자열</param>
/// <param sortFuncNameStr="sortFunc">정렬 함수</param>
/// <param sortFuncNameStr="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param sortFuncNameStr="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param sortFuncNameStr="traceResult">단일 Pass의 Trace 결과</param>
template<typename SortElementType>
void RunSinglePassSortTrace(const char* sortFuncNameStr,
	void(*sortFunc)(SortElementType[], size_t, ORDER_BY),
	SortElementType targetEnumerableSet[], size_t elementCount,
	std::promise<TRACE_RESULT>& traceResult)
{
	std::chrono::system_clock::time_point startTime; //시작 시간
	std::chrono::system_clock::time_point endTime; //종료 시간
	std::chrono::nanoseconds descDuration = std::chrono::milliseconds::zero(); //내림차순 정렬 소요 시간
	std::chrono::nanoseconds ascDuration = std::chrono::milliseconds::zero(); //오름차순 정렬 소요 시간

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

	if (LOGGING_LEVEL >= 1)
	{
		mutex.lock();

		if (LOGGING_LEVEL == 2)
		{
			std::cout << sortFuncNameStr << " : 오름차순 정렬 후\n";
			DispEnumerableSet<SortElementType>(targetEnumerableSet, elementCount);
			std::cout << std::endl;
		}

		std::cout << ">> " << sortFuncNameStr << " (오름차순 정렬) : " << ascDuration.count() << "ns 소요" << std::endl;
		DispCompareCount(sortFuncNameStr);

		mutex.unlock();
	}

	startTime = std::chrono::system_clock::now();
	sortFunc(targetEnumerableSet, elementCount, ORDER_BY::DESCENDING); //내림차순 정렬
	endTime = std::chrono::system_clock::now();
	descDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

	if (LOGGING_LEVEL >= 1)
	{
		mutex.lock();

		if (LOGGING_LEVEL == 2)
		{
			std::cout << sortFuncNameStr << " : 오름차순 정렬 된 데이터에 대한 내림차순 정렬 후\n";
			DispEnumerableSet<SortElementType>(targetEnumerableSet, elementCount);
			std::cout << std::endl;
		}

		std::cout << ">> " << sortFuncNameStr << " (오름차순 정렬 된 데이터에 대한 내림차순 정렬) : " << descDuration.count() << "ns 소요" << std::endl;
		DispCompareCount(sortFuncNameStr);

		mutex.unlock();
	}

	TRACE_RESULT retVal;
	retVal = (descDuration + ascDuration);
	traceResult.set_value(retVal);
}
#endif