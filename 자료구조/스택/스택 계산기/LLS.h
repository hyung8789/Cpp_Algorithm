#ifndef _LLS_H_
#define _LLS_H_
//#pragma warning(disable : 6011) //NULL ������ ��� ����

typedef int StackIndexType; //���� �ε��� Ÿ��

typedef struct NodeType
{
	char* data; //����� ������
	NodeType* next; //���� ���
}Node;

typedef struct LinkedListStackType
{
	Node* top; //�ֻ��� ��� (tail)
	Node* head; //��� ���

	StackIndexType totalNodeCount; //��ü ��� ��
}LinkedListStack;

void LLS_CreateStack(LinkedListStack**) throw(std::runtime_error);
void LLS_DeallocateLinkedListStack(LinkedListStack**);

Node* LLS_CreateNode(const char*) throw(std::invalid_argument, std::runtime_error);
void LLS_DeallocateNode(Node**);

void LLS_Push(LinkedListStack**, Node*) throw(std::invalid_argument, std::runtime_error);
Node* LLS_Pop(LinkedListStack**) throw(std::runtime_error);
Node* LLS_Peek(LinkedListStack**) throw(std::runtime_error);

StackIndexType LLS_GetTotalNodeCount(LinkedListStack**) throw(std::runtime_error);
bool LLS_IsEmpty(LinkedListStack**) throw(std::runtime_error);
#endif