#ifndef _LCRS_H_
#define _LCRS_H_

typedef char DATA_TYPE; //노드의 데이터 타입
typedef int TREE_DEPTH_TYPE; //트리 깊이 타입

/// <summary>
/// LCRS 트리의 노드
/// </summary>
typedef struct NODE_TYPE
{
	DATA_TYPE _data; //노드의 데이터

	NODE_TYPE* _leftChild; //왼쪽 자식 노드
	NODE_TYPE* _rightSibling; //오른쪽 형제 노드
}NODE;

//#define RECURSIVE_METHOD //재귀적 방법 (주석 처리 : 반복적 방법 사용)
#ifndef RECURSIVE_METHOD 
#define ITERATIVE_METHOD //반복적 방법

#include <stack> // https://www.cplusplus.com/reference/stack/stack/
#include <utility> // https://www.cplusplus.com/reference/utility/
#include <tuple> // https://www.cplusplus.com/reference/tuple/
#endif

NODE* LCRS_CreateNode(DATA_TYPE);
void LCRS_DeallocateNode(NODE**);
void LCRS_DeallocateTree(NODE**);

void LCRS_AppendNode(NODE*, NODE*);

void LCRS_DispTreeNodesAt(NODE*, TREE_DEPTH_TYPE);
void LCRS_DispTree(NODE*, TREE_DEPTH_TYPE = 0);
#endif