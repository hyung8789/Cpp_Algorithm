#ifndef _DS_H_
#define _DS_H_

typedef struct NodeType
{
	void* data; //노드의 데이터
	NodeType* parent; //한 단계 상위 부모 노드
}Node;

Node* DS_CreateNode(void*);
void DS_DeallocateNode(Node**);

void DS_UnionSet(Node**, Node*);
Node* DS_FindSet(Node*);
#endif