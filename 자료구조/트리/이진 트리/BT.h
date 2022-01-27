#ifndef _BT_H_
#define _BT_H_

typedef char DataType; //노드의 데이터 타입
typedef int TreeDepthType; //트리 깊이 타입

typedef struct NodeType
{
	DataType data; //노드의 데이터
	NodeType* left; //왼쪽 노드
	NodeType* right; //오른쪽 노드
}Node;

enum class TRAVERSAL_MODE : const int
{
	PREORDER = 0, //전위 순회 (Root -> Left -> Right)
	INORDER, //중위 순회 (Left -> Root -> Right)
	POSTORDER //후위 순회 (Left -> Right -> Root)
};

Node* BT_CreateNode(DataType);
void BT_DeallocateNode(Node**);
void BT_DeallocateTree(Node**);

void BT_DispTree(Node*, TRAVERSAL_MODE);
#endif