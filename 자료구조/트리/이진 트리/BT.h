#ifndef _BT_H_
#define _BT_H_

typedef char DATA_TYPE; //노드의 데이터 타입
typedef int TREE_DEPTH_TYPE; //트리 깊이 타입

/// <summary>
/// 이진 트리의 노드
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

NODE* BT_CreateNode(DATA_TYPE);
void BT_DeallocateNode(NODE**);
void BT_DeallocateTree(NODE**);

void BT_DispOrderedTree(NODE*, TRAVERSAL_METHOD);
#endif