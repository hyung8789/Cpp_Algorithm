#ifndef _HT_OPENADDR_H_
#define _HT_OPENADDR_H_

#define CAPACITY_INCREASE_RATIO_THRESHOLD 0.5 //기존 할당 크기에 대한 증가가 발생 될 임계 비율 (0.0 ~ 1.0)
#define CAPACITY_INCREASE_RATIO 2.0 //기존 할당 크기에 대해 증가 시 재 할당 될 비율

/// <summary>
/// 개방 주소법 노드
/// </summary>
struct OPENADDR_NODE
{
	HT_KEY_TYPE _key; //Hash Function에 키로 사용 할 데이터
	HT_DATA_TYPE _data; //노드의 데이터

	NODE_STATE _state; //노드의 상태
};

/// <summary>
/// 개방 주소법 해시 테이블
/// </summary>
struct OPENADDR_HASH_TABLE
{
	HASH_INDEX_TYPE _capacity; //할당량
	HASH_INDEX_TYPE _usedSize; //사용량

	OPENADDR_NODE* _table; //노드에 대한 테이블
};

OPENADDR_HASH_TABLE* HT_OpenAddr_CreateHashTable(HASH_INDEX_TYPE);
void HT_OpenAddr_DeallocateHashTable(OPENADDR_HASH_TABLE**);

void HT_OpenAddr_DeallocateNode(OPENADDR_NODE*);

void HT_OpenAddr_DispNodeList(OPENADDR_HASH_TABLE*);
void HT_OpenAddr_DispIndexListBy(OPENADDR_HASH_TABLE*, NODE_STATE);

void HT_OpenAddr_InsertData(OPENADDR_HASH_TABLE**, HT_KEY_TYPE, HT_DATA_TYPE);
HT_DATA_TYPE HT_OpenAddr_SearchData(OPENADDR_HASH_TABLE*, HT_KEY_TYPE);
void HT_OpenAddr_RemoveData(OPENADDR_HASH_TABLE*, HT_KEY_TYPE);

void HT_OpenAddr_RehashProc(OPENADDR_HASH_TABLE**, HASH_INDEX_TYPE);
#endif