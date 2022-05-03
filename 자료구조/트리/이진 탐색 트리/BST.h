#ifndef _BST_H_
#define _BST_H_

/***
	< 이진 탐색 트리의 정의 >

	DEF1) 루트 노드 혹은 서브 트리 내의 루트 노드는 항상 중앙값을 가짐
	: 루트 노드의 왼쪽 서브 트리 < 루트 노드 < 루트 노드의 오른쪽 서브 트리

	DEF2) 위의 정의 및 구현의 단순화를 위해 중복 된 데이터를 허용하지 않음
***/

typedef int DATA_TYPE; //노드의 데이터 타입
typedef int TREE_DEPTH_TYPE; //트리 깊이 타입

/// <summary>
/// 이진 탐색 트리의 노드
/// </summary>
typedef struct NODE_TYPE
{
	DATA_TYPE _data; //노드의 데이터

	NODE_TYPE* _left; //왼쪽 노드
	NODE_TYPE* _right; //오른쪽 노드
}NODE;

/// <summary>
/// 순회 방법
/// </summary>
enum class TRAVERSAL_METHOD : const int
{
	PREORDER = 0, //전위 순회 (Root -> Left -> Right)
	INORDER, //중위 순회 (Left -> Root -> Right)
	POSTORDER //후위 순회 (Left -> Right -> Root)
};

NODE* BST_CreateNode(DATA_TYPE);
void BST_DeallocateNode(NODE**);
void BST_DeallocateTree(NODE**);

void BST_DispOrderedTree(NODE*, TRAVERSAL_METHOD);

void BST_InsertNode(NODE**, NODE*);
void BST_RemoveNode(NODE**, DATA_TYPE, bool = true);

NODE* BST_SearchNode(NODE*, DATA_TYPE);
std::tuple<NODE*, NODE*, NODE**> BST_SearchNodeWithParentNode(NODE*, DATA_TYPE, NODE* = NULL);

NODE* BST_SearchMinNode(NODE*);
std::tuple<NODE*, NODE*, NODE**> BST_SearchMinNodeWithParentNode(NODE*, NODE* = NULL);
#endif