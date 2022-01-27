#ifndef _EXPRT_H_
#define _EXPRT_H_

/***
	! ����, ���� ǥ�������κ��� ���� Ʈ���� �����ϴ� ������ ��� ���� �з��� ����,

		F1) ���� ǥ���Ŀ� ���� �������� ��ū ����
		F2) ���� �� ��ū���κ��� �������� ���� Ʈ�� ����
		F3) ���� �� ���� Ʈ���κ��� ��� ����

	=> ���� �� ���� Ʈ���κ��� ����� ����Ǵ� ����� ���� Ʈ���� �������̹Ƿ�,
	���� Ʈ�� ����� ����� ����Ǵ� ���� ���� ǥ�������κ��� �������� ��ū�� �����Ǵ� ����� ������ �и� �� ��
***/

typedef char* DataType; //����� ������ Ÿ��
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

Node* EXPRT_CreateNode(DataType);
void EXPRT_DeallocateNode(Node**);
void EXPRT_DeallocateTree(Node**);

void EXPRT_DispTree(Node*, TRAVERSAL_MODE);
#endif