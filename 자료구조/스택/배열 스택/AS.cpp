#include "AS_Core.h"

/// <summary>
/// ��� �迭 ���ÿ� �Ҵ� ũ�⸸ŭ �迭 ���� ����
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
/// <param name="capacity">�Ҵ� ũ��</param>
void AS_CreateStack(ARRAY_STACK** srcArrayStack, STACK_INDEX_TYPE capacity)
{
	if ((*srcArrayStack) != NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Memleak"));

	if (capacity <= 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	(*srcArrayStack) = (ARRAY_STACK*)malloc(sizeof(ARRAY_STACK));
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	(*srcArrayStack)->_nodeArray = (NODE*)malloc(sizeof(NODE) * capacity); //�Ҵ� ũ�⸸ŭ ����
	if ((*srcArrayStack)->_nodeArray == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	(*srcArrayStack)->_capacity = capacity;
	(*srcArrayStack)->_top = 0;
}

/// <summary>
/// ��� �迭 ���ÿ� �Ҵ� �� �޸� ����
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
void AS_DeallocateArrayStack(ARRAY_STACK** srcArrayStack)
{
	if ((*srcArrayStack) != NULL)
	{
		free((*srcArrayStack)->_nodeArray);
		(*srcArrayStack)->_nodeArray = NULL;

		free(*srcArrayStack);
		(*srcArrayStack) = NULL;
	}
}

/// <summary>
/// ��� �迭 ������ �������� �ֻ��� ������ ����
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
/// <param name="srcData">���� �� ������</param>
void AS_Push(ARRAY_STACK** srcArrayStack, DATA_TYPE srcData)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (AS_IsFull(srcArrayStack)) //���� á����
	{
		STACK_INDEX_TYPE reallocCapacity = (*srcArrayStack)->_capacity + ceil((*srcArrayStack)->_capacity * CAPACITY_REALLOC_RATIO); //�� �Ҵ� �� ũ��
		STACK_INDEX_TYPE reallocSizeInBytes = sizeof(NODE) * reallocCapacity; //�� �Ҵ� �� ����Ʈ ���� ũ��

		void* reallocAddr = realloc((*srcArrayStack)->_nodeArray, reallocSizeInBytes);
		if (reallocAddr == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		(*srcArrayStack)->_nodeArray = (NODE*)reallocAddr;
		(*srcArrayStack)->_capacity = reallocCapacity;
	}

	STACK_INDEX_TYPE index = (*srcArrayStack)->_top++; //Push �� �ε���
	(*srcArrayStack)->_nodeArray[index]._data = srcData;
}

/// <summary>
/// ��� �迭 ������ �������� �ֻ��� ������ ���� �� ��ȯ
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
/// <returns>��� �迭 ������ �ֻ��� ������</returns>
DATA_TYPE AS_Pop(ARRAY_STACK** srcArrayStack)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (AS_IsEmpty(srcArrayStack)) //���������
		throw std::logic_error(std::string(__func__) + std::string(" : Empty Stack"));

	if ((AS_GetTotalNodeDataCount(srcArrayStack) / (*srcArrayStack)->_capacity) <
		CAPACITY_REDUCE_RATIO_THRESHOLD) //��뷮�� ���� ������ ���� �Ҵ� ũ�⿡ ���� ���Ұ� �߻� �� �� ���� �Ӱ� ���� �̸��� ���
	{
		STACK_INDEX_TYPE reallocCapacity = (*srcArrayStack)->_capacity - ceil((*srcArrayStack)->_capacity * CAPACITY_REALLOC_RATIO); //�� �Ҵ� �� ũ��
		STACK_INDEX_TYPE reallocSizeInBytes = sizeof(NODE) * reallocCapacity; //�� �Ҵ� �� ����Ʈ ���� ũ��
		void* reallocAddr = realloc((*srcArrayStack)->_nodeArray, reallocSizeInBytes);
		if (reallocAddr == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		(*srcArrayStack)->_nodeArray = (NODE*)reallocAddr;
		(*srcArrayStack)->_capacity = reallocCapacity;
	}

	STACK_INDEX_TYPE index = --((*srcArrayStack)->_top); //Pop �� �ε���
	return (*srcArrayStack)->_nodeArray[index]._data;
}

/// <summary>
/// ��� �迭 ������ �������� �ֻ��� ������ ���ž��� ��ȯ 
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
/// <returns>��� �迭 ������ �ֻ��� ������</returns>
DATA_TYPE AS_Peek(ARRAY_STACK** srcArrayStack)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (AS_IsEmpty(srcArrayStack)) //���������
		throw std::logic_error(std::string(__func__) + std::string(" : Empty Stack"));

	STACK_INDEX_TYPE index = ((*srcArrayStack)->_top) - 1; //Peek �� �ε���
	return (*srcArrayStack)->_nodeArray[index]._data;
}

/// <summary>
/// ��� �迭 ������ ��ü ��� ������ ���� ��ȯ
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
/// <returns>��� �迭 ������ ��ü ��� ������ ����</returns>
STACK_INDEX_TYPE AS_GetTotalNodeDataCount(ARRAY_STACK** srcArrayStack)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return (*srcArrayStack)->_top;
}

/// <summary>
/// ��� �迭 ������ ���� ���� ��ȯ
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
/// <returns>��� �迭 ������ ���� ����</returns>
bool AS_IsEmpty(ARRAY_STACK** srcArrayStack)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return ((*srcArrayStack)->_top == 0);
}

/// <summary>
/// ��� �迭 ������ ���� ���� ���� ��ȯ
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
/// <returns>��� �迭 ������ ���� ���� ����</returns>
bool AS_IsFull(ARRAY_STACK** srcArrayStack)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return ((*srcArrayStack)->_capacity == (*srcArrayStack)->_top); //�Ҵ� ũ��� ���������� �������� �� �̻� ���� �Ұ���
}