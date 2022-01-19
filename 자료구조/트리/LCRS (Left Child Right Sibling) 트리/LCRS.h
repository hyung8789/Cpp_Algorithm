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

//#define RECURSIVE_METHOD //재귀적 방법 (주석 처리 : 반복적 방법 사용)
#ifndef RECURSIVE_METHOD 
#define ITERATIVE_METHOD //반복적 방법

#include <queue> // https://www.cplusplus.com/reference/queue/queue/

typedef struct QueueElementType
{
	Node* node; //노드
	TreeIndexType nodeDepth; //노드의 깊이
}QueueElement; //큐의 요소
#endif

Node* LCRS_CreateNode(DataType);
void LCRS_DeallocateNode(Node**);
void LCRS_DeallocateTree(Node**);

void LCRS_AddChildNode(Node*, Node*);

void LCRS_DispTreeNodesAt(Node*, TreeIndexType);
void LCRS_DispTree(Node*, TreeIndexType = 0);
#endif