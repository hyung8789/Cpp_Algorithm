#ifndef _HT_COMMON_H_
#define _HT_COMMON_H_

#define DIGIT_FOLDING 0
#define DOUBLE_HASHING 1
#define REHASHING 2
//TODO : add

#define HASH_METHOD DIGIT_FOLDING

typedef unsigned int HASH_INDEX_TYPE;

typedef char* HT_KEY_TYPE; //노드의 키 타입
typedef char* HT_DATA_TYPE; //노드의 데이터 타입

typedef HASH_INDEX_TYPE(*HASH_FUNC_ADDR_TYPE)(HASH_INDEX_TYPE, HT_KEY_TYPE);

HASH_INDEX_TYPE HT_Common_DigitFolding_Hash(HASH_INDEX_TYPE, HT_KEY_TYPE);
//TODO : add

#if HASH_METHOD == DIGIT_FOLDING
static HASH_FUNC_ADDR_TYPE hashFunc = &HT_Common_DigitFolding_Hash;
#elif HASH_METHOD == DOUBLE_HASHING
#endif
#endif