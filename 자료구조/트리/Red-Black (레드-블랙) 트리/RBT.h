#ifndef _RBT_H_
#define _RBT_H_

/***
	< 레드 블랙 트리의 정의 >

	DEF1) 루트 노드 및 단말 노드는 항상 검은색이며, 모든 노드는 빨간색 혹은 검은색
	: 단말 노드가 항상 검은색임을 위반하지 않기 위해, 별도의 검은색 더미 노드를 단말 노드로 이용
	(검은색 더미 단말 노드 자신은 부모와 연결되어있지 않으며, 단말 노드에서 검은색 더미 단말 노드로만 연결을 허용)

	DEF2) 빨간 노드의 한 단계 하위 자식 노드는 항상 검은색
	: 연속해서 빨간 노드가 두 개 존재할 수 없음

	DEF3) 검은 노드의 한 단계 하위 자식 노드는 빨간색 혹은 검은색

	DEF4) 루트 노드에서 검은색 더미 단말 노드 간의 각 연결 과정에 존재하는 검은 노드의 수는 모두 동일
	: 루트 노드 및 검은색 더미 단말 노드 포함

	DEF5) 루트 노드 혹은 서브 트리 내의 루트 노드는 항상 중앙값을 가짐
	: 루트 노드의 왼쪽 서브 트리 < 루트 노드 < 루트 노드의 오른쪽 서브 트리

	DEF6) 위의 정의 및 구현의 단순화를 위해 중복 된 키를 허용하지 않음
***/

#ifdef STR_STORAGE_TREE_TYPE
typedef char* RBT_KEY_TYPE; //노드의 키 타입
typedef char* RBT_DATA_TYPE; //노드의 데이터 타입
#else
typedef int RBT_KEY_TYPE; //노드의 키 타입
typedef int RBT_DATA_TYPE; //노드의 데이터 타입
#endif
typedef size_t TREE_DEPTH_TYPE; //트리 깊이 타입

#ifdef COLOR_VISUALIZATION
/// <summary>
/// 색상 가시화를 위한 노드의 색상
/// </summary>
enum class COLOR : const unsigned
{
    RED = ((const unsigned)BG_COLOR::BG_RED | (const unsigned)FG_COLOR::FG_WHITE |
        FOREGROUND_INTENSITY),
    BLACK = ((const unsigned)BG_COLOR::BG_GRAY | (const unsigned)FG_COLOR::FG_WHITE |
		FOREGROUND_INTENSITY)
};
#else
/// <summary>
/// 노드의 색상
/// </summary>
enum class COLOR : const char
{
    RED = 0,
    BLACK
};
#endif

/// <summary>
/// 레드 블랙 트리의 노드
/// </summary>
typedef struct RBT_NODE_TYPE
{
	RBT_KEY_TYPE _key; //노드의 키
	RBT_DATA_TYPE _data; //노드의 데이터
	COLOR _color; //노드 색

	RBT_NODE_TYPE* _parent; //부모 노드
	RBT_NODE_TYPE* _left; //왼쪽 노드
	RBT_NODE_TYPE* _right; //오른쪽 노드
}RBT_NODE;

/// <summary>
/// 순회 방법
/// </summary>
enum class TRAVERSAL_METHOD : const int
{
	PREORDER = 0, //전위 순회 (Root -> Left -> Right)
	INORDER, //중위 순회 (Left -> Root -> Right)
	POSTORDER //후위 순회 (Left -> Right -> Root)
};

/// <summary>
/// 회전 방향
/// </summary>
enum class ROTATE_DIRECTION : const int
{
	RIGHT = 0, //우회전 (부모 노드의 왼쪽 자식과 부모의 위치 교환) 
	LEFT //좌회전 (부모 노드의 오른쪽 자식과 부모의 위치 교환)
};

/// <summary>
/// 경로 방향
/// </summary>
enum class PATH_DIRECTION : const int
{
	RIGHT = 0, //오른쪽 경로
	LEFT, //왼쪽 경로
	BOTH //양쪽 경로 (오른쪽 및 왼쪽)
};

extern RBT_NODE* dummyBlackTerminalNode;

RBT_NODE* RBT_CreateNode(RBT_KEY_TYPE, RBT_DATA_TYPE, bool = false);
void RBT_DeallocateNode(RBT_NODE**);
void RBT_DeallocateTree(RBT_NODE**);

void RBT_DispOrderedTree(RBT_NODE*, TRAVERSAL_METHOD, TREE_DEPTH_TYPE = 0);

void RBT_InsertNode(RBT_NODE**, RBT_NODE*);
void RBT_RemoveNode(RBT_NODE**, RBT_KEY_TYPE, bool = true);
RBT_NODE* RBT_SearchNode(RBT_NODE*, RBT_KEY_TYPE);
RBT_NODE* RBT_SearchMaxKeyNode(RBT_NODE*);
RBT_NODE* RBT_SearchMinKeyNode(RBT_NODE*);

inline int RBT_CompareKey(RBT_KEY_TYPE, RBT_KEY_TYPE);
void RBT_AssignKeyAndData(RBT_NODE*, RBT_KEY_TYPE, RBT_DATA_TYPE, bool = false);
void RBT_CopyKeyAndData(RBT_NODE*, RBT_KEY_TYPE, RBT_DATA_TYPE);
void RBT_MoveKeyAndData(RBT_NODE*, RBT_KEY_TYPE*, RBT_DATA_TYPE*);

void RBT_InsertNodeHelper(RBT_NODE**, RBT_NODE*);
void RBT_RestructureAfterInsert(RBT_NODE**, RBT_NODE*);
void RBT_RotateTree(RBT_NODE**, RBT_NODE*, ROTATE_DIRECTION);
size_t RBT_GetColorCount(RBT_NODE*, COLOR, PATH_DIRECTION);

#ifdef RBT_DEBUG_MODE
void RBT_ValidateTree(RBT_NODE*);
#endif
#endif