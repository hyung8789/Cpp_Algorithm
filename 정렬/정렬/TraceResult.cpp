#include "Core.h"

/// <summary>
/// TRACE_RESULT 생성자
/// </summary>
TRACE_RESULT::TRACE_RESULT()
{
	this->ClearAll();
}

/// <summary>
/// 전체 Test Pass 횟수에 대한 자신의 결과 출력
/// </summary>
/// <param name="sortFuncNameStr">정렬 함수 이름 문자열</param>
/// <param name="totalTestPassCount">전체 Test Pass 횟수</param>
void TRACE_RESULT::DispTotalTestPassTraceResult(const char* sortFuncNameStr, size_t totalTestPassCount)
{
	std::cout << sortFuncNameStr << " ----------------------------------------------\n";
	std::cout << ">> 전체 Pass의 총 소요 시간 : " << this->_accDuration.count() << "ns\n";
	std::cout << ">> 평균 소요 시간 : " << this->_accDuration.count() / totalTestPassCount << "ns\n";
	std::cout << ">> 최소 소요 시간 : " << this->_minDuration.count() << "ns\n";
	std::cout << ">> 최대 소요 시간 : " << this->_maxDuration.count() << "ns\n";

#ifdef LOGGING_COMPARE_COUNT
	DispCompareCount(sortFuncNameStr);
#endif

	std::cout << std::endl;
}

/// <summary>
/// 모든 결과 초기화
/// </summary>
void TRACE_RESULT::ClearAll()
{
	this->_accDuration = this->_maxDuration = this->_minDuration = std::chrono::nanoseconds::zero();
	this->_compareCount = 0;
}

/// <summary>
/// += 연산자 재정의
/// </summary>
/// <param name="newResult">+= 연산을 수행 할 대상</param>
/// <returns>자신의 참조</returns>
TRACE_RESULT& TRACE_RESULT::operator+=(const TRACE_RESULT& newResult)
{
	this->_accDuration += newResult._accDuration;

	if (this->_minDuration == std::chrono::nanoseconds::zero())
		this->_minDuration = newResult._minDuration;

	if (this->_minDuration > newResult._minDuration)
		this->_minDuration = newResult._minDuration;

	if (this->_maxDuration < newResult._maxDuration)
		this->_maxDuration = newResult._maxDuration;

	return *this;
}

/// <summary>
/// += 연산자 재정의
/// </summary>
/// <param name="newDuration">+= 연산을 수행 할 대상</param>
/// <returns>자신의 참조</returns>
TRACE_RESULT& TRACE_RESULT::operator+=(const std::chrono::nanoseconds& newDuration)
{
	this->_accDuration += newDuration;

	if (this->_minDuration == std::chrono::nanoseconds::zero())
		this->_minDuration = newDuration;

	if (this->_minDuration > newDuration)
		this->_minDuration = newDuration;

	if (this->_maxDuration < newDuration)
		this->_maxDuration = newDuration;

	return *this;
};

/// <summary>
/// = 연산자 재정의
/// </summary>
/// <param name="newDuration">= 연산을 수행 할 대상</param>
/// <returns>자신의 참조</returns>
TRACE_RESULT& TRACE_RESULT::operator=(const std::chrono::nanoseconds& newDuration)
{
	this->_accDuration = newDuration;
	this->_minDuration = newDuration;
	this->_maxDuration = newDuration;

	return *this;
};

//TODO : 이하 삭제
/// <summary>
/// 현재 비교 횟수 초기화
/// </summary>
void ClearCompareCount()
{
	memset(compareCount, 0, sizeof(size_t) * LENGTH(compareCount));
}

/// <summary>
/// 정렬 함수 이름 문자열에 따른 비교 횟수 증가
/// </summary>
/// <param name="sortFuncNameStr">정렬 함수 이름 문자열</param>
void IncreaseCompareCount(const char* sortFuncNameStr)
{
	compareCount[SortFuncNameStrToUniqueMappedIndex(sortFuncNameStr)]++;
}

/// <summary>
/// 정렬 함수 이름 문자열에 따른 현재 비교 횟수 출력
/// </summary>
/// <param name="sortFuncNameStr">정렬 함수 이름 문자열</param>
void DispCompareCount(const char* sortFuncNameStr)
{
	std::cout << ">> " << sortFuncNameStr << "의 총 비교 횟수 : " <<
		compareCount[SortFuncNameStrToUniqueMappedIndex(sortFuncNameStr)] << "회" << std::endl;
}

/// <summary>
/// 정렬 함수 이름 문자열에 따른 현재 비교 횟수 반환
/// </summary>
/// <param name="sortFuncNameStr">정렬 함수 이름 문자열</param>
/// <returns>정렬 함수 이름 문자열에 따른 현재 비교 횟수</returns>
size_t GetCompareCount(const char* sortFuncNameStr)
{
	return compareCount[SortFuncNameStrToUniqueMappedIndex(sortFuncNameStr)];
}