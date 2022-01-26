#ifndef _LCRS_H_
#define _LCRS_H_

typedef char DataType; //����� ������ Ÿ��
typedef int TreeDepthType; //Ʈ�� ���� Ÿ��

typedef struct NodeType
{
	DataType data; //����� ������
	NodeType* leftChild; //���� �ڽ� ���
	NodeType* rightSibling; //������ ���� ���
}Node;

//#define RECURSIVE_METHOD //����� ��� (�ּ� ó�� : �ݺ��� ��� ���)
#ifndef RECURSIVE_METHOD 
#define ITERATIVE_METHOD //�ݺ��� ���

#include <stack> // https://www.cplusplus.com/reference/stack/stack/
#include <utility> // https://www.cplusplus.com/reference/utility/
#include <tuple> // https://www.cplusplus.com/reference/tuple/
#endif

Node* LCRS_CreateNode(DataType);
void LCRS_DeallocateNode(Node**);
void LCRS_DeallocateTree(Node**);

void LCRS_AddChildNode(Node*, Node*);

void LCRS_DispTreeNodesAt(Node*, TreeDepthType);
void LCRS_DispTree(Node*, TreeDepthType = 0);
#endif