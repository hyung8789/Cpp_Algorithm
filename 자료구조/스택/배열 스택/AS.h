#ifndef _AS_H_
#define _AS_H_
#pragma warning(disable : 6011) //NULL ������ ��� ����

typedef int DataType; //����� ������ Ÿ��
typedef int StackIndexType; //���� �ε��� Ÿ��

typedef struct NodeType
{
	DataType data; //����� ������
}Node;

typedef struct ArrayStackType
{
	StackIndexType capacity; //�Ҵ� ũ��
	StackIndexType top; //�ֻ��� ��� �ε���
	Node* nodeArray; //��� �迭
}ArrayStack;

void AS_CreateStack(ArrayStack**, StackIndexType) throw(std::invalid_argument, std::runtime_error);
void AS_DeallocateArrayStack(ArrayStack**);

void AS_Push(ArrayStack**, DataType) throw(std::runtime_error);
DataType AS_Pop(ArrayStack**) throw(std::runtime_error);
DataType AS_Peek(ArrayStack**) throw(std::runtime_error);

StackIndexType AS_GetTotalNodeDataCount(ArrayStack**) throw(std::runtime_error);
bool AS_IsEmpty(ArrayStack**) throw(std::runtime_error);
bool AS_IsFull(ArrayStack**) throw(std::runtime_error);
#endif