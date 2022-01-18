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

Node* LCRS_CreateNode(DataType);
void LCRS_DeallocateNode(Node**);
void LCRS_DeallocateTree(Node**);

void LCRS_AddChildNode(Node**, Node*);
void LCRS_DispTree(Node**, TreeIndexType);
#endif