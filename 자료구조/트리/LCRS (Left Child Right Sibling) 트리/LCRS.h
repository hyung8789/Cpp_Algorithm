#ifndef _LCRS_H_
#define _LCRS_H_

typedef int DataType; //노드의 데이터 타입
typedef int TreeIndexType; //트리 인덱스 타입

typedef struct NodeType
{
	DataType data; //노드의 데이터
	NodeType* leftChild; //왼쪽 자식 노드
	NodeType* rightSibling; //오른쪽 형제 노드
}Node;

Node* LCRS_CreateNode(DataType);
void LCRS_DeallocateNode(Node**);
void LCRS_DeallocateTree(Node**);

void LCRS_AddChildNode(Node**, Node*);
void LCRS_DispTree(Node**, TreeIndexType);
#endif