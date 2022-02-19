#ifndef _RBT_H_
#define _RBT_H_

/***
	< 레드 블랙 트리 >

	1) 루트 노드 및 단말 노드는 항상 검은색

	2) 빨간 노드의 한 단계 하위 자식 노드는 항상 검은색

	3) 검은 노드의 한 단계 하위 자식 노드는 빨간색 혹은 검은색

	4) 루트 노드에서 각 단말 노드 간의 연결 과정에 존재하는 검은 노드의 수는 모두 동일 

	5) 루트 노드 혹은 서브 트리 내의 루트 노드는 항상 중앙값을 가짐
	: 루트 노드의 왼쪽 서브 트리 < 루트 노드 < 루트 노드의 오른쪽 서브 트리

	6) 위의 정의에 따라, 중복 된 데이터를 허용하지 않음
***/

typedef int DataType; //노드의 데이터 타입
typedef int TreeDepthType; //트리 깊이 타입

enum class COLOR : const int
{
	RED = 0,
	BLACK
};

typedef struct NodeType
{
	COLOR color;
	DataType _data; //노드의 데이터

	Node* _parent; //부모 노드
	NodeType* _left; //왼쪽 노드
	NodeType* _right; //오른쪽 노드
}Node;

enum class TRAVERSAL_MODE : const int
{
	PREORDER = 0, //전위 순회 (Root -> Left -> Right)
	INORDER, //중위 순회 (Left -> Root -> Right)
	POSTORDER //후위 순회 (Left -> Right -> Root)
};
#endif