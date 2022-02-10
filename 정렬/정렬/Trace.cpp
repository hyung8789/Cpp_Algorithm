#include "Core.h"

/// <summary>
/// TRACE_RESULT ������
/// </summary>
TRACE_RESULT::TRACE_RESULT()
{
	this->accDuration = this->maxDuration = this->minDuration = std::chrono::nanoseconds::zero();
}

/// <summary>
/// ��ü Test Pass Ƚ���� ���� �ڽ��� ��� ���
/// </summary>
/// <param name="sortFuncNameStr">���� �Լ� �̸� ���ڿ�</param>
/// <param name="totalTestPassCount">��ü Test Pass Ƚ��</param>
void TRACE_RESULT::DispTotalTestPassTraceResult(const char* sortFuncNameStr, size_t totalTestPassCount)
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

/// <summary>
/// += ������ ������
/// </summary>
/// <param name="newResult">+= ������ ���� �� ���</param>
/// <returns>�ڽ��� ����</returns>
TRACE_RESULT& TRACE_RESULT::operator+=(const TRACE_RESULT& newResult)
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

/// <summary>
/// += ������ ������
/// </summary>
/// <param name="newDuration">+= ������ ���� �� ���</param>
/// <returns>�ڽ��� ����</returns>
TRACE_RESULT& TRACE_RESULT::operator+=(const std::chrono::nanoseconds& newDuration)
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

/// <summary>
/// = ������ ������
/// </summary>
/// <param name="newDuration">= ������ ���� �� ���</param>
/// <returns>�ڽ��� ����</returns>
TRACE_RESULT& TRACE_RESULT::operator=(const std::chrono::nanoseconds& newDuration)
{
	this->accDuration = newDuration;
	this->minDuration = newDuration;
	this->maxDuration = newDuration;

	return *this;
};

/// <summary>
/// ���� �� Ƚ�� �ʱ�ȭ
/// </summary>
void ClearCompareCount()
{
	memset(compareCount, 0, sizeof(size_t) * LENGTH(compareCount));
}

/// <summary>
/// ���� �Լ� �̸� ���ڿ��� ���� �� Ƚ�� ����
/// </summary>
/// <param name="sortFuncNameStr">���� �Լ� �̸� ���ڿ�</param>
void IncreaseCompareCount(const char* sortFuncNameStr)
{
	compareCount[SortFuncNameStrToUniqueMappedIndex(sortFuncNameStr)]++;
}

/// <summary>
/// ���� �Լ� �̸� ���ڿ��� ���� ���� �� Ƚ�� ���
/// </summary>
/// <param name="sortFuncNameStr">���� �Լ� �̸� ���ڿ�</param>
void DispCompareCount(const char* sortFuncNameStr)
{
	std::cout << ">> " << sortFuncNameStr << "�� �� �� Ƚ�� : " <<
		compareCount[SortFuncNameStrToUniqueMappedIndex(sortFuncNameStr)] << "ȸ" << std::endl;
}

/// <summary>
/// ���� �Լ� �̸� ���ڿ��� ���� ���� �� Ƚ�� ��ȯ
/// </summary>
/// <param name="sortFuncNameStr">���� �Լ� �̸� ���ڿ�</param>
/// <returns>���� �Լ� �̸� ���ڿ��� ���� ���� �� Ƚ��</returns>
size_t GetCompareCount(const char* sortFuncNameStr)
{
	return compareCount[SortFuncNameStrToUniqueMappedIndex(sortFuncNameStr)];
}