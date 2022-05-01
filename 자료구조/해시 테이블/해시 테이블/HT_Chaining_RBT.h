#ifndef _HT_CHAINING_RBT_H_
#define _HT_CHAINING_RBT_H_

#ifndef STR_STORAGE_RBT_TYPE
#error Key, Data Type Mismatch (Must define STR_STORAGE_RBT_TYPE at RBT_Core.h)
#endif

typedef RBT_NODE* CHAINING_RBT_TREE;

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

void HT_Chaining_RBT_DispNodeList(CHAINING_RBT_HASH_TABLE*);
void HT_Chaining_RBT_DispIndexListBy(CHAINING_RBT_HASH_TABLE*, NODE_STATE);

void HT_Chaining_RBT_InsertData(CHAINING_RBT_HASH_TABLE*, HT_KEY_TYPE, HT_DATA_TYPE);
HT_DATA_TYPE HT_Chaining_RBT_SearchData(CHAINING_RBT_HASH_TABLE*, HT_KEY_TYPE);
void HT_Chaining_RBT_RemoveData(CHAINING_RBT_HASH_TABLE*, HT_KEY_TYPE);
#endif