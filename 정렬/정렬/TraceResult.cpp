#include "Core.h"

/// <summary>
/// TRACE_RESULT ������
/// </summary>
TRACE_RESULT::TRACE_RESULT()
{
	this->ClearAll();
}

/// <summary>
/// ��ü Test Pass Ƚ���� ���� �ڽ��� ��� ���
/// </summary>
/// <param name="sortFuncNameStr">���� �Լ� �̸� ���ڿ�</param>
/// <param name="totalTestPassCount">��ü Test Pass Ƚ��</param>
void TRACE_RESULT::DispTotalTestPassTraceResult(const char* sortFuncNameStr, size_t totalTestPassCount)
{
	std::cout << sortFuncNameStr << " ----------------------------------------------\n";
	std::cout << ">> ��ü Pass�� �� �ҿ� �ð� : " << this->_accDuration.count() << "ns\n";
	std::cout << ">> ��� �ҿ� �ð� : " << this->_accDuration.count() / totalTestPassCount << "ns\n";
	std::cout << ">> �ּ� �ҿ� �ð� : " << this->_minDuration.count() << "ns\n";
	std::cout << ">> �ִ� �ҿ� �ð� : " << this->_maxDuration.count() << "ns\n";

#ifdef LOGGING_COMPARE_COUNT
	DispCompareCount(sortFuncNameStr);
#endif

	std::cout << std::endl;
}

/// <summary>
/// ��� ��� �ʱ�ȭ
/// </summary>
void TRACE_RESULT::ClearAll()
{
	this->_accDuration = this->_maxDuration = this->_minDuration = std::chrono::nanoseconds::zero();
	this->_compareCount = 0;
}

/// <summary>
/// += ������ ������
/// </summary>
/// <param name="newResult">+= ������ ���� �� ���</param>
/// <returns>�ڽ��� ����</returns>
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
/// += ������ ������
/// </summary>
/// <param name="newDuration">+= ������ ���� �� ���</param>
/// <returns>�ڽ��� ����</returns>
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
/// = ������ ������
/// </summary>
/// <param name="newDuration">= ������ ���� �� ���</param>
/// <returns>�ڽ��� ����</returns>
TRACE_RESULT& TRACE_RESULT::operator=(const std::chrono::nanoseconds& newDuration)
{
	this->_accDuration = newDuration;
	this->_minDuration = newDuration;
	this->_maxDuration = newDuration;

	return *this;
};

//TODO : ���� ����
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