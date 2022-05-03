#ifndef _TRACE_RESULT_H_
#define _TRACE_RESULT_H_

extern std::mutex;

class TRACE_RESULT
{
public:
	TRACE_RESULT();

	void ClearAll();

	void DispTotalTestPassTraceResult(const char*, size_t) const;
	void IncreaseCompareCount();
	size_t GetCompareCount() const;
	void DispCompareCount(const char*) const;

	TRACE_RESULT& operator+=(const TRACE_RESULT&);
	TRACE_RESULT& operator+=(const std::chrono::nanoseconds&);
	TRACE_RESULT& operator=(const std::chrono::nanoseconds&);

private:
	std::chrono::nanoseconds _accDuration; //누적 소요 시간
	std::chrono::nanoseconds _minDuration; //최소 소요 시간
	std::chrono::nanoseconds _maxDuration; //최대 소요 시간

	size_t _compareCount; //비교 횟수 카운트
};
#endif