#ifndef _LCRS_H_
#define _LCRS_H_

typedef int DataType; //����� ������ Ÿ��
typedef int TreeIndexType; //Ʈ�� �ε��� Ÿ��

typedef struct NodeType
{
	DataType data; //����� ������
	NodeType* leftChild; //���� �ڽ� ���
	NodeType* rightSibling; //������ ���� ���
}Node;

//#define RECURSIVE_METHOD //����� ��� (�ּ� ó�� : �ݺ��� ��� ���)
#ifndef RECURSIVE_METHOD 
#define ITERATIVE_METHOD //�ݺ��� ���

#include <queue> // https://www.cplusplus.com/reference/queue/queue/

typedef struct QueueElementType
{
	Node* node; //���
	TreeIndexType nodeDepth; //����� ����
}QueueElement; //ť�� ���
#endif

Node* LCRS_CreateNode(DataType);
void LCRS_DeallocateNode(Node**);
void LCRS_DeallocateTree(Node**);

void LCRS_AddChildNode(Node*, Node*);

void LCRS_DispTreeNodesAt(Node*, TreeIndexType);
void LCRS_DispTree(Node*, TreeIndexType = 0);
#endif