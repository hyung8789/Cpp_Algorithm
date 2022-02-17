#ifndef _BST_H_
#define _BST_H_

/***
	< ���� Ž�� Ʈ�� >

	1) ��Ʈ ��� Ȥ�� ���� Ʈ�� ���� ��Ʈ ���� �׻� �߾Ӱ��� ����
	: ��Ʈ ����� ���� ���� Ʈ�� < ��Ʈ ��� < ��Ʈ ����� ������ ���� Ʈ��

	2) ���� Ž�� Ʈ���� ���ǿ� ����, �ߺ� �� �����͸� ������� ����
***/

void BST_InsertNode(Node**, Node*);
void BST_RemoveNode(Node**, const DataType&, bool = true);

Node* BST_SearchNode(Node*, const DataType&);
Node* BST_SearchTargetParentNode(Node*, const DataType&, Node* = NULL);
Node* BST_SearchMinNode(Node*);
#endif