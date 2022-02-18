#ifndef _DS_H_
#define _DS_H_

typedef struct NodeType
{
	void* _data; //����� ������

	NodeType* _parent; //�� �ܰ� ���� �θ� ���
}Node;

Node* DS_CreateNode(void*);
void DS_DeallocateNode(Node**);

void DS_UnionSet(Node**, Node*);
Node* DS_FindSet(Node*);
#endif