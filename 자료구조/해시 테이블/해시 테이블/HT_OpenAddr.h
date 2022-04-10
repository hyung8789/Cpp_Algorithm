#ifndef _HT_OPEN_ADDR_H_
#define _HT_OPEN_ADDR_H_

#define CAPACITY_REALLOC_RATIO_THRESHOLD 0.4 //기존 할당 크기에 대한 재 할당이 발생 될 빈 공간 임계 비율 (0.0 ~ 1.0)
#define CAPACITY_REALLOC_RATIO 0.5 //기존 할당 크기에 대해 증가 혹은 감소 시 재 할당 될 비율

//TODO : 체이닝에서 DELETED 상태는 사용하지 않지만 GetIndexList에서 공통적으로 사용하기 위해 묶기 
enum class OPEN_ADDR_NODE_STATE : const char
{
	/***
		< 노드의 상태 관리 >

		DELETED 상태를 사용하지 않고, 삭제 시 EMPTY 상태로 전이 할 경우,
		탐색 과정도 삽입 과정과 동일한 시퀸스로 이중 해싱을 수행하기 때문에, 
		
		- 최초 비어있는 위치가 EMPTY 상태를 만날 경우 탐색을 중지
		: 삭제 시 해당 해시 인덱스의 해시 함수에 의해 연관 된 인덱스의 데이터들에 대해 정렬 된 순서를 유지하지 않으므로,
		실제 존재하는 데이터이지만 찾을 수 없는 경우가 발생

		- 최초 비어있는 위치가 EMPTY 상태가 아닌 최초 해시 인덱스로 돌아올 때까지 탐색 수행
		: 삭제 시 해당 해시 인덱스의 해시 함수에 의해 연관 된 인덱스의 데이터들에 대해 정렬 된 순서를 유지하지 않으므로,
		최악의 경우, 해당 해시 인덱스의 해시 함수에 의해 연관 된 전체 인덱스의 데이터들을 모두 탐색하여야 함

		따라서, 별도의 DELETED 상태를 두어 탐색 과정에서 DELETED 상태에 대해서는 무시하며,
		EMPTY 상태를 만날때까지 탐색을 수행 할 것
	***/

	EMPTY = 0, //비어있음
	OCCUPIED, //사용 중
	DELETED //삭제 됨
};

struct OPEN_ADDR_NODE
{
	HT_KEY_TYPE _key; //Hash Function에 키로 사용 할 데이터
	HT_DATA_TYPE _data; //노드의 데이터

	OPEN_ADDR_NODE_STATE _state; //노드의 상태
};

struct OPEN_ADDR_HASH_TABLE
{
	HASH_INDEX_TYPE _capacity; //할당량
	HASH_INDEX_TYPE _usedSize; //사용량

	OPEN_ADDR_NODE* _table; //노드에 대한 테이블
};

OPEN_ADDR_HASH_TABLE* HT_OpenAddr_CreateHashTable(HASH_INDEX_TYPE);
void HT_OpenAddr_DeallocateHashTable(OPEN_ADDR_HASH_TABLE**);

void HT_OpenAddr_DispNodeList(OPEN_ADDR_HASH_TABLE*);
void HT_OpenAddr_DispIndexListBy(OPEN_ADDR_HASH_TABLE*, OPEN_ADDR_NODE_STATE);

void HT_OpenAddr_InsertData(OPEN_ADDR_HASH_TABLE**, HT_KEY_TYPE, HT_DATA_TYPE);
HT_DATA_TYPE HT_OpenAddr_SearchData(OPEN_ADDR_HASH_TABLE*, HT_KEY_TYPE);
void HT_OpenAddr_RemoveData(OPEN_ADDR_HASH_TABLE*, HT_KEY_TYPE);

void HT_OpenAddr_RehashProc(OPEN_ADDR_HASH_TABLE**, HASH_INDEX_TYPE);
#endif