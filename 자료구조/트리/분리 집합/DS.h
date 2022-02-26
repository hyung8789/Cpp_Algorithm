#ifndef _DS_H_
#define _DS_H_

typedef struct NODE_TYPE
{
	void* _data; //����� ������

	NODE_TYPE* _parent; //�� �ܰ� ���� �θ� ���
}NODE;

NODE* DS_CreateNode(void*);
void DS_DeallocateNode(NODE**);

void DS_UnionSet(NODE**, NODE*);
NODE* DS_FindSet(NODE*);
#endif