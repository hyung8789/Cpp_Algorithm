#ifndef _RBT_H_
#define _RBT_H_

/***
	< 레드 블랙 트리 >

	DEF1) 루트 노드 및 단말 노드는 항상 검은색
	: 단말 노드가 항상 검은색임을 위반하지 않기 위해, 별도의 검은색 더미 노드를 단말 노드로 이용
	(검은색 더미 단말 노드 자신은 부모와 연결되어있지 않으며, 단말 노드에서 검은색 더미 단말 노드로만 연결을 허용)

	DEF2) 빨간 노드의 한 단계 하위 자식 노드는 항상 검은색

	DEF3) 검은 노드의 한 단계 하위 자식 노드는 빨간색 혹은 검은색

	DEF4) 루트 노드에서 검은색 더미 단말 노드 간의 각 연결 과정에 존재하는 검은 노드의 수는 모두 동일

	DEF5) 루트 노드 혹은 서브 트리 내의 루트 노드는 항상 중앙값을 가짐
	: 루트 노드의 왼쪽 서브 트리 < 루트 노드 < 루트 노드의 오른쪽 서브 트리

	DEF6) 위의 정의 및 구현의 단순화를 위해 중복 된 데이터를 허용하지 않음
***/

typedef int DataType; //노드의 데이터 타입
typedef int TreeDepthType; //트리 깊이 타입

enum class COLOR : const unsigned
{
	RED = (0xFF0000),
	BLACK = (0x000000)
};

typedef struct NodeType
{
	DataType _data; //노드의 데이터
	COLOR _color; //노드 색

	NodeType* _parent; //부모 노드
	NodeType* _left; //왼쪽 노드
	NodeType* _right; //오른쪽 노드
}Node;

enum class TRAVERSAL_MODE : const int
{
	PREORDER = 0, //전위 순회 (Root -> Left -> Right)
	INORDER, //중위 순회 (Left -> Root -> Right)
	POSTORDER //후위 순회 (Left -> Right -> Root)
};

enum class ROTATE_DIRECTION : const int
{
	RIGHT = 0, //우회전 (부모 노드의 왼쪽 자식과 부모의 위치 교환) 
	LEFT //좌회전 (부모 노드의 오른쪽 자식과 부모의 위치 교환)
};

static Node* dummyBlackTerminalNode = NULL; //검은색 더미 단말 노드

Node* RBT_CreateNode(DataType);

void RBT_InsertNode(Node**, Node*);
void RBT_InsertNodeHelper(Node**, Node*);
void RBT_PostProcAfterInsert(Node**, Node*);

void RBT_RotateTree(Node**, Node* , ROTATE_DIRECTION);
#endif