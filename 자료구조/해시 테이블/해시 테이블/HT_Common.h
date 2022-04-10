#ifndef _HT_COMMON_H_
#define _HT_COMMON_H_

typedef unsigned int HASH_INDEX_TYPE;

typedef char* HT_KEY_TYPE; //노드의 키 타입
typedef char* HT_DATA_TYPE; //노드의 데이터 타입

#ifdef HT_DEBUG_MODE
static size_t hashCollisionCount = 0; //해시 충돌 횟수 카운트

void HT_Common_InitHashCollisionCount();
void HT_Common_IncreaseHashCollisionCount(HT_KEY_TYPE = NULL);
size_t HT_Common_GetHashCollisionCount();
#endif

HASH_INDEX_TYPE HT_Common_DigitFolding_BaseProc(HASH_INDEX_TYPE, HT_KEY_TYPE);
HASH_INDEX_TYPE HT_Common_DigitFolding_Hash(HASH_INDEX_TYPE, HT_KEY_TYPE);
HASH_INDEX_TYPE HT_Common_DoubleHashing_Hash1(HASH_INDEX_TYPE, HT_KEY_TYPE);
HASH_INDEX_TYPE HT_Common_DoubleHashing_Hash2(HASH_INDEX_TYPE, HT_KEY_TYPE);
#endif