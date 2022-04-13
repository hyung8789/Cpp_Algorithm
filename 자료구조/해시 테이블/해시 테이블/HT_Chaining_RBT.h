#ifndef _HT_CHAINING_RBT_H_
#define _HT_CHAINING_RBT_H_

//TODO : key를 같이 저장해야 하는데 기존 RBT를 끌어다쓰기 위해 구조를 바꿔야 하는가? base (parent)에서 key 접근 불가

/// <summary>
/// 노드의 색상
/// </summary>
enum class COLOR : const char
{
	RED = 0,
	BLACK
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
/// 체이닝 레드 블랙 트리 노드
/// </summary>
struct CHAINING_RBT_NODE
{
	HT_KEY_TYPE _key; //Hash Function에 키로 사용 할 데이터
	HT_DATA_TYPE _data; //노드의 데이터
	COLOR _color; //노드 색

	CHAINING_RBT_NODE* _parent; //부모 노드
	CHAINING_RBT_NODE* _left; //왼쪽 노드
	CHAINING_RBT_NODE* _right; //오른쪽 노드
	//RBT_NODE* _base;
};

typedef CHAINING_RBT_NODE* CHAINING_RBT_TREE;

/// <summary>
/// 체이닝 레드 블랙 트리 해시 테이블
/// </summary>
struct CHAINING_RBT_HASH_TABLE
{
	HASH_INDEX_TYPE _capacity; //할당량

	CHAINING_RBT_TREE* _table; //RBT 트리에 대한 테이블
};

CHAINING_RBT_HASH_TABLE* HT_Chaining_RBT_CreateHashTable(HASH_INDEX_TYPE);
void HT_Chaining_RBT_DeallocateHashTable(CHAINING_RBT_HASH_TABLE**);

CHAINING_RBT_NODE* HT_Chaining_RBT_CreateNode(HT_KEY_TYPE, HT_DATA_TYPE);
void HT_Chaining_RBT_DeallocateNode(CHAINING_RBT_NODE**);
void HT_Chaining_RBT_DeallocateTree(CHAINING_RBT_TREE*);

void HT_Chaining_RBT_DispNodeList(CHAINING_RBT_HASH_TABLE*);
void HT_Chaining_RBT_DispIndexListBy(CHAINING_RBT_HASH_TABLE*, NODE_STATE);

void HT_Chaining_RBT_InsertData(CHAINING_RBT_HASH_TABLE*, HT_KEY_TYPE, HT_DATA_TYPE);
HT_DATA_TYPE HT_Chaining_RBT_SearchData(CHAINING_RBT_HASH_TABLE*, HT_KEY_TYPE);
void HT_Chaining_RBT_RemoveData(CHAINING_RBT_HASH_TABLE*, HT_KEY_TYPE, bool = true);

//TODO : 삽입, 삭제 연산 시 key에도 접근해야 함
//TODO : RBT와 chaining RBT 간의 상속 관계로 변경 할 경우, 삽입 삭제 시에 key에 접근하여야 하는데, 
//TODO : base가 되는 부모 측에서 이에 대해 접근이 불가능, 따라서 끌어다 쓸수가 없다
#endif