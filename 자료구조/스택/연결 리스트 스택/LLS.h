#ifndef _LLS_H_
#define _LLS_H_
#pragma warning(disable : 6011) //NULL ������ ��� ����

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

void LLS_CreateStack(LinkedListStack**);
void LLS_DeallocateLinkedListStack(LinkedListStack**);

Node* LLS_CreateNode(const char*);
void LLS_DeallocateNode(Node**);

void LLS_Push(LinkedListStack**, Node*);
Node* LLS_Pop(LinkedListStack**);
Node* LLS_Peek(LinkedListStack**);

StackIndexType LLS_GetTotalNodeCount(LinkedListStack**);
bool LLS_IsEmpty(LinkedListStack**);
#endif