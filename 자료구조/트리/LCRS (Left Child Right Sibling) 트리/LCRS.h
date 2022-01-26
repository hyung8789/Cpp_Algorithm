#ifndef _LCRS_H_
#define _LCRS_H_

typedef char DataType; //노드의 데이터 타입
typedef int TreeDepthType; //트리 깊이 타입

typedef struct NodeType
{
	DataType data; //노드의 데이터
	NodeType* leftChild; //왼쪽 자식 노드
	NodeType* rightSibling; //오른쪽 형제 노드
}Node;

//#define RECURSIVE_METHOD //재귀적 방법 (주석 처리 : 반복적 방법 사용)
#ifndef RECURSIVE_METHOD 
#define ITERATIVE_METHOD //반복적 방법

#include <stack> // https://www.cplusplus.com/reference/stack/stack/
#include <utility> // https://www.cplusplus.com/reference/utility/
#include <tuple> // https://www.cplusplus.com/reference/tuple/
#endif

Node* LCRS_CreateNode(DataType);
void LCRS_DeallocateNode(Node**);
void LCRS_DeallocateTree(Node**);

void LCRS_AddChildNode(Node*, Node*);

void LCRS_DispTreeNodesAt(Node*, TreeDepthType);
void LCRS_DispTree(Node*, TreeDepthType = 0);
#endif