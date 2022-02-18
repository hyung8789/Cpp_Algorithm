#ifndef _BT_H_
#define _BT_H_

typedef int DataType; //����� ������ Ÿ��
typedef int TreeDepthType; //Ʈ�� ���� Ÿ��

typedef struct NodeType
{
	DataType _data; //����� ������

	NodeType* _left; //���� ���
	NodeType* _right; //������ ���
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

void BT_DispOrderedTree(Node*, TRAVERSAL_MODE);
#endif