#include "AS_Core.h"

/// <summary>
/// ��� �迭 ���ÿ� �Ҵ� ũ�⸸ŭ �迭 ���� ����
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
/// <param name="capacity">�Ҵ� ũ��</param>
void AS_CreateStack(ArrayStack** srcArrayStack, StackIndexType capacity)
{
	if ((*srcArrayStack) != NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Memleak"));

	if (capacity <= 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	(*srcArrayStack) = (ArrayStack*)malloc(sizeof(ArrayStack));
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	(*srcArrayStack)->_nodeArray = (Node*)malloc(sizeof(Node) * capacity); //�Ҵ� ũ�⸸ŭ ����
	if ((*srcArrayStack)->_nodeArray == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	(*srcArrayStack)->_capacity = capacity;
	(*srcArrayStack)->_top = 0;
}

/// <summary>
/// ��� �迭 ���ÿ� �Ҵ� �� �޸� ����
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
void AS_DeallocateArrayStack(ArrayStack** srcArrayStack)
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
void AS_Push(ArrayStack** srcArrayStack, DataType srcData)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (AS_IsFull(srcArrayStack)) //���� á����
	{
		StackIndexType reallocCapacity = (*srcArrayStack)->_capacity + ceil((*srcArrayStack)->_capacity * CAPACITY_REALLOC_RATIO); //�� �Ҵ� �� ũ��
		StackIndexType reallocSizeInBytes = sizeof(Node) * reallocCapacity; //�� �Ҵ� �� ����Ʈ ���� ũ��

		void* reallocAddr = realloc((*srcArrayStack)->_nodeArray, reallocSizeInBytes);
		if (reallocAddr == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		(*srcArrayStack)->_nodeArray = (Node*)reallocAddr;
		(*srcArrayStack)->_capacity = reallocCapacity;
	}

	StackIndexType index = (*srcArrayStack)->_top++; //Push �� �ε���
	(*srcArrayStack)->_nodeArray[index]._data = srcData;
}

/// <summary>
/// ��� �迭 ������ �������� �ֻ��� ������ ���� �� ��ȯ
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
/// <returns>��� �迭 ������ �ֻ��� ������</returns>
DataType AS_Pop(ArrayStack** srcArrayStack)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (AS_IsEmpty(srcArrayStack)) //���������
		throw std::logic_error(std::string(__func__) + std::string(" : Empty Stack"));

	if ((AS_GetTotalNodeDataCount(srcArrayStack) / (*srcArrayStack)->_capacity) <
		CAPACITY_REDUCE_RATIO_THRESHOLD) //��뷮�� ���� ������ ���� �Ҵ� ũ�⿡ ���� ���Ұ� �߻� �� �� ���� �Ӱ� ���� �̸��� ���
	{
		StackIndexType reallocCapacity = (*srcArrayStack)->_capacity - ceil((*srcArrayStack)->_capacity * CAPACITY_REALLOC_RATIO); //�� �Ҵ� �� ũ��
		StackIndexType reallocSizeInBytes = sizeof(Node) * reallocCapacity; //�� �Ҵ� �� ����Ʈ ���� ũ��
		void* reallocAddr = realloc((*srcArrayStack)->_nodeArray, reallocSizeInBytes);
		if (reallocAddr == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		(*srcArrayStack)->_nodeArray = (Node*)reallocAddr;
		(*srcArrayStack)->_capacity = reallocCapacity;
	}

	StackIndexType index = --((*srcArrayStack)->_top); //Pop �� �ε���
	return (*srcArrayStack)->_nodeArray[index]._data;
}

/// <summary>
/// ��� �迭 ������ �������� �ֻ��� ������ ���ž��� ��ȯ 
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
/// <returns>��� �迭 ������ �ֻ��� ������</returns>
DataType AS_Peek(ArrayStack** srcArrayStack)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (AS_IsEmpty(srcArrayStack)) //���������
		throw std::logic_error(std::string(__func__) + std::string(" : Empty Stack"));

	StackIndexType index = ((*srcArrayStack)->_top) - 1; //Peek �� �ε���
	return (*srcArrayStack)->_nodeArray[index]._data;
}

/// <summary>
/// ��� �迭 ������ ��ü ��� ������ ���� ��ȯ
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
/// <returns>��� �迭 ������ ��ü ��� ������ ����</returns>
StackIndexType AS_GetTotalNodeDataCount(ArrayStack** srcArrayStack)
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
bool AS_IsEmpty(ArrayStack** srcArrayStack)
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
bool AS_IsFull(ArrayStack** srcArrayStack)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return ((*srcArrayStack)->_capacity == (*srcArrayStack)->_top); //�Ҵ� ũ��� ���������� �������� �� �̻� ���� �Ұ���
}