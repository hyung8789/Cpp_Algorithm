#ifndef _BT_H_
#define _BT_H_

typedef char DataType; //����� ������ Ÿ��
typedef int TreeDepthType; //Ʈ�� ���� Ÿ��

typedef struct NodeType
{
	DataType data; //����� ������
	NodeType* left; //���� ���
	NodeType* right; //������ ���
}Node;

enum class TRAVERSAL_MODE : const int
{
	PREORDER = 0, //���� ��ȸ (Root -> Left -> Right)
	INORDER, //���� ��ȸ (Left -> Root -> Right)
	POSTORDER //���� ��ȸ (Left -> Right -> Root)
};

Node* BT_CreateNode(DataType);
void BT_DeallocateNode(Node**);
void BT_DeallocateTree(Node**);

void BT_DispTree(Node*, TRAVERSAL_MODE);
#endif