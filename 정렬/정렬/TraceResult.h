#ifndef _TRACE_RESULT_H_
#define _TRACE_RESULT_H_

extern std::mutex;

class TRACE_RESULT
{
public:
	TRACE_RESULT();

	void ClearAll();

	void DispTotalTestPassTraceResult(const char*, size_t);
	void IncreaseCompareCount();
	size_t GetCompareCount();
	void DispCompareCount(const char*);

	TRACE_RESULT& operator+=(const TRACE_RESULT&);
	TRACE_RESULT& operator+=(const std::chrono::nanoseconds&);
	TRACE_RESULT& operator=(const std::chrono::nanoseconds&);

private:
	std::chrono::nanoseconds _accDuration; //���� �ҿ� �ð�
	std::chrono::nanoseconds _minDuration; //�ּ� �ҿ� �ð�
	std::chrono::nanoseconds _maxDuration; //�ִ� �ҿ� �ð�

	size_t _compareCount; //�� Ƚ�� ī��Ʈ
};
#endif