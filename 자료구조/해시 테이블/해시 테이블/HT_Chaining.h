#ifndef _HT_CHAINING_H_
#define _HT_CHAINING_H_

struct CHAINING_NODE
{
	HT_KEY_TYPE _key; //Hash Function에 키로 사용 할 데이터
	HT_DATA_TYPE _data; //노드의 데이터

	CHAINING_NODE* _next; //연결 된 다음 노드
};

typedef CHAINING_NODE* CHAINING_NODE_LIST;

struct CHAINING_HASH_TABLE
{
	HASH_INDEX_TYPE _capacity; //할당량

	CHAINING_NODE_LIST* _table; //연결 된 노드에 대한 테이블
};

CHAINING_HASH_TABLE* HT_Chaining_CreateHashTable(HASH_INDEX_TYPE);
void HT_Chaining_DeallocateHashTable(CHAINING_HASH_TABLE**);

void HT_Chaining_DispNodeList(CHAINING_HASH_TABLE*);
void HT_Chaining_DispEmptyIndexList(CHAINING_HASH_TABLE*);
void HT_Chaining_DispOccupiedIndexList(CHAINING_HASH_TABLE*);

CHAINING_NODE* HT_Chaining_CreateNode(HT_KEY_TYPE, HT_DATA_TYPE);
void HT_Chaining_DeallocateNode(CHAINING_NODE**);
void HT_Chaining_DeallocateNodeList(CHAINING_NODE_LIST*);

void HT_Chaining_InsertData(CHAINING_HASH_TABLE*, HT_KEY_TYPE, HT_DATA_TYPE);
HT_DATA_TYPE HT_Chaining_SearchData(CHAINING_HASH_TABLE*, HT_KEY_TYPE);
void HT_Chaining_RemoveData(CHAINING_HASH_TABLE*, HT_KEY_TYPE, bool = true);
#endif