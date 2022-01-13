#include "AS_Core.h"

/// <summary>
/// ��� �迭 ���ÿ� �Ҵ� ũ�⸸ŭ �迭 ���� ����
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
/// <param name="capacity">�Ҵ� ũ��</param>
void AS_CreateStack(ArrayStack** srcArrayStack, StackIndexType capacity) throw(std::invalid_argument, std::runtime_error)
{
	if ((*srcArrayStack) != NULL)
		throw std::runtime_error("Memleak");

	if (capacity <= 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	(*srcArrayStack) = (ArrayStack*)malloc(sizeof(ArrayStack));
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	(*srcArrayStack)->nodeArray = (Node*)malloc(sizeof(Node) * capacity); //�Ҵ� ũ�⸸ŭ ����
	if ((*srcArrayStack)->nodeArray == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	//(*srcArrayStack)->nodeArray = (Node*)realloc((*srcArrayStack)->nodeArray, sizeof(Node) * capacity); //�Ҵ� ũ�⸸ŭ �� �Ҵ�
	//if ((*srcArrayStack)->nodeArray == NULL)
		//ThrowCriticalException(EX::NOT_ENOUGH_HEAP_MEMORY);

	(*srcArrayStack)->capacity = capacity;
	(*srcArrayStack)->top = 0;
}

/// <summary>
/// ��� �迭 ���ÿ� �Ҵ� �� �޸� ����
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
void AS_DeallocateArrayStack(ArrayStack** srcArrayStack)
{
	if ((*srcArrayStack) != NULL)
	{
		free((*srcArrayStack)->nodeArray);
		(*srcArrayStack)->nodeArray = NULL;

		free(*srcArrayStack);
		(*srcArrayStack) = NULL;
	}
}

/// <summary>
/// ��� �迭 ������ �������� �ֻ��� ������ ����
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
/// <param name="srcData">���� �� ������</param>
void AS_Push(ArrayStack** srcArrayStack, DataType srcData) throw(std::runtime_error)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (!AS_IsFull(srcArrayStack)) //���� ���� �ʾ�����
	{
		StackIndexType index = (*srcArrayStack)->top++; //Push �� �ε���
		(*srcArrayStack)->nodeArray[index].data = srcData;
		return;
	}
	else
	{
		//TODO : PUSH �� ��� �Ҵ緮�� �Ӱ谪 �ʰ� �� �Ҵ� �� ũ�� �ø��� (realloc)
	}
}

/// <summary>
/// ��� �迭 ������ �������� �ֻ��� ������ ���� �� ��ȯ
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
/// <returns>��� �迭 ������ �ֻ��� ������</returns>
DataType AS_Pop(ArrayStack** srcArrayStack) throw(std::runtime_error)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (!AS_IsEmpty(srcArrayStack)) //������� ������
	{
		StackIndexType index = --((*srcArrayStack)->top); //Pop �� �ε���
		return (*srcArrayStack)->nodeArray[index].data;
	}
	else
	{
		//TODO : POP �� ��� �Ҵ緮�� �Ӱ谪 ���� �� ��� �Ҵ� �� ũ�� ���̱� (realloc)
	}
}

/// <summary>
/// ��� �迭 ������ �������� �ֻ��� ������ ���ž��� ��ȯ 
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
/// <returns>��� �迭 ������ �ֻ��� ������</returns>
DataType AS_Peek(ArrayStack** srcArrayStack) throw(std::runtime_error)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (!AS_IsEmpty(srcArrayStack)) //������� ������
	{
		StackIndexType index = ((*srcArrayStack)->top) - 1; //Peek �� �ε���
		return (*srcArrayStack)->nodeArray[index].data;
	}
	else
	{

	}
}

/// <summary>
/// ��� �迭 ������ ��ü ��� ������ ���� ��ȯ
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
/// <returns>��� �迭 ������ ��ü ��� ������ ����</returns>
StackIndexType AS_GetTotalNodeDataCount(ArrayStack** srcArrayStack) throw(std::runtime_error)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return (*srcArrayStack)->top;
}

/// <summary>
/// ��� �迭 ������ ���� ���� ��ȯ
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
/// <returns>��� �迭 ������ ���� ����</returns>
bool AS_IsEmpty(ArrayStack** srcArrayStack) throw(std::runtime_error)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return ((*srcArrayStack)->top == 0);
}

/// <summary>
/// ��� �迭 ������ ���� ���� ���� ��ȯ
/// </summary>
/// <param name="srcArrayStack">��� �迭 ����</param>
/// <returns>��� �迭 ������ ���� ���� ����</returns>
bool AS_IsFull(ArrayStack** srcArrayStack) throw(std::runtime_error)
{
	if ((*srcArrayStack) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	return ((*srcArrayStack)->capacity == (*srcArrayStack)->top); //�Ҵ� ũ��� ���������� �������� �� �̻� ���� �Ұ���
}