#ifndef _AS_H_
#define _AS_H_

#define CAPACITY_REALLOC_RATIO 0.3 //���� �Ҵ� ũ�⿡ ���� ���� Ȥ�� ���� �� �� �Ҵ� �� ���� (0.0 ~ 1.0)
#define CAPACITY_REDUCE_RATIO_THRESHOLD 0.7 //���� �Ҵ� ũ�⿡ ���� ���Ұ� �߻� �� �� ���� �Ӱ� ���� (0.0 ~ 1.0)

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

void AS_CreateStack(ArrayStack**, StackIndexType);
void AS_DeallocateArrayStack(ArrayStack**);

void AS_Push(ArrayStack**, DataType);
DataType AS_Pop(ArrayStack**);
DataType AS_Peek(ArrayStack**);

StackIndexType AS_GetTotalNodeDataCount(ArrayStack**);
bool AS_IsEmpty(ArrayStack**);
bool AS_IsFull(ArrayStack**);
#endif