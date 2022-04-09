#ifndef _HT_COMMON_H_
#define _HT_COMMON_H_

typedef unsigned int HASH_INDEX_TYPE;

typedef char* HT_KEY_TYPE; //노드의 키 타입
typedef char* HT_DATA_TYPE; //노드의 데이터 타입

HASH_INDEX_TYPE HT_Common_DigitFolding_BaseProc(HASH_INDEX_TYPE, HT_KEY_TYPE);
HASH_INDEX_TYPE HT_Common_DigitFolding_Hash(HASH_INDEX_TYPE, HT_KEY_TYPE);
HASH_INDEX_TYPE HT_Common_DoubleHashing_Hash1(HASH_INDEX_TYPE, HT_KEY_TYPE);
HASH_INDEX_TYPE HT_Common_DoubleHashing_Hash2(HASH_INDEX_TYPE, HT_KEY_TYPE);
#endif