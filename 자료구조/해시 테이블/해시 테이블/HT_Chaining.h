#ifndef _HT_CHAINING_H_
#define _HT_CHAINING_H_

struct CHAINING_NODE
{
	KEY_TYPE _key; //Hash Function에 키로 사용 할 데이터
	DATA_TYPE _data; //노드의 데이터

	CHAINING_NODE* _next; //연결 된 다음 노드
};

struct CHAINING_HASH_TABLE
{
	HASH_INDEX_TYPE _capacity; //할당량
	CHAINING_NODE** _table;
};

CHAINING_HASH_TABLE* HT_Chaining_CreateHashTable(HASH_INDEX_TYPE);
void HT_Chaining_DeallocateHashTable(CHAINING_HASH_TABLE**);

CHAINING_NODE* HT_Chaining_CreateNode(KEY_TYPE, DATA_TYPE);
void HT_Chaining_DeallocateNode(CHAINING_NODE**);
void HT_Chaining_DeallocateNodeList(CHAINING_NODE**);

void HT_Chaining_InsertData(CHAINING_HASH_TABLE*, KEY_TYPE, DATA_TYPE);
DATA_TYPE HT_Chaining_SearchData(CHAINING_HASH_TABLE*, KEY_TYPE);
void HT_Chaining_RemoveData(CHAINING_HASH_TABLE*, KEY_TYPE, bool = true);
#endif