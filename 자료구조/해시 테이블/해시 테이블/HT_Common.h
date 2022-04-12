#ifndef _HT_COMMON_H_
#define _HT_COMMON_H_

typedef unsigned int HASH_INDEX_TYPE;

typedef char* HT_KEY_TYPE; //노드의 키 타입
typedef char* HT_DATA_TYPE; //노드의 데이터 타입

/// <summary>
/// 해시 테이블의 충돌 처리 방법
/// </summary>
enum class HASH_TABLE_COLLISION_HANDLE_METHOD : const char
{
	CHAINING = 0,
	//CHAINING_RBT,
	OPEN_ADDR
};

/// <summary>
/// 노드의 상태
/// </summary>
enum class NODE_STATE : const char
{
	/***
		< 개방 주소법 노드의 상태 관리 >

		DELETED 상태를 사용하지 않고, 삭제 시 EMPTY 상태로 전이 할 경우,
		탐색 과정도 삽입 과정과 동일한 시퀸스로 이중 해싱을 수행하기 때문에,

		M1) 최초 비어있는 위치가 EMPTY 상태를 만날 경우 탐색을 중지
		: 삭제 시 해당 해시 인덱스의 해시 함수에 의해 연관 된 인덱스들간의 정렬 된 순서를 유지하지 않으므로,
		실제 존재하는 데이터이지만 찾을 수 없는 경우가 발생

		M2) 최초 비어있는 위치가 EMPTY 상태가 아닌 최초 해시 인덱스로 돌아올 때까지 탐색 수행
		: 삭제 시 해당 해시 인덱스의 해시 함수에 의해 연관 된 인덱스들간의 정렬 된 순서를 유지하지 않으므로,
		최악의 경우, 해당 해시 인덱스의 해시 함수에 의해 연관 된 전체 인덱스들을 모두 탐색하여야 함

		---

		따라서, 탐색 수행 횟수를 줄이기 위해 별도의 DELETED 상태를 두어 다음과 같이 처리

		1) Insert 과정에서 만난 노드의 상태에 따라,

			1-1) EMPTY
			: 해당 위치에 삽입 및 OCCUPIED 상태로 전이

			1-2) OCCUPIED
			: 대상 키와 완전히 일치 한 중복 키일 경우 덮어쓰기,
			아닐 경우 계속 탐색

			1-3) DELETED
			: 탐색 수행 횟수를 줄이기 위해 기존 키를 보존하였으므로,
			대상 키와 완전히 일치 한 중복 키일 경우 덮어쓰기 및 OCCUPIED 상태로 전이,
			아닐 경우 EMPTY 상태로 간주하여	해당 위치에 삽입 및 OCCUPIED 상태로 전이 (기존 키 변경)

		2) Search 과정에서 만난 노드의 상태에 따라,

			2-1) EMPTY
			: 대상 키에 해당하는 데이터는 실제로 존재하지 않으므로 탐색 중지

			2-2) OCCUPIED
			: 대상 키와 완전히 일치 한 키일 경우 해당 데이터 반환,
			아닐 경우 계속 탐색

			2-3) DELETED
			: 대상 키와 삭제 된 키가 완전히 일치 한 키일 경우 탐색 중지,
			아닐 경우 계속 탐색

		3) Remove 과정에서 만난 노드의 상태에 따라,

			3-1) EMPTY
			: 대상 키에 해당하는 데이터는 실제로 존재하지 않으므로 탐색 중지

			3-2) OCCUPIED
			: 대상 키와 완전히 일치 한 키일 경우, 해당 데이터 삭제 및 DELETED 상태로 전이,
			아닐 경우 계속 탐색 (탐색 수행 횟수를 줄이기 위해 기존 키 보존)

			3-3) DELETED
			: 대상 키와 완전히 일치 한 키일 경우, 대상 키에 해당하는 데이터는 이미 삭제 된 데이터이므로 탐색 중지,
			아닐 경우 계속 탐색
	***/

	EMPTY = 0, //비어있음
	OCCUPIED, //사용 중
	DELETED, //삭제 됨
};

#ifdef HT_DEBUG_MODE
static size_t hashCollisionCount = 0; //해시 충돌 횟수 카운트

void HT_Common_InitHashCollisionCount();
void HT_Common_IncreaseHashCollisionCount(HT_KEY_TYPE = NULL);
size_t HT_Common_GetHashCollisionCount();
#endif

void HT_Common_DispNodeList(void*, HASH_TABLE_COLLISION_HANDLE_METHOD);
void HT_Common_DispIndexListBy(void*, HASH_TABLE_COLLISION_HANDLE_METHOD,
	NODE_STATE);

HASH_INDEX_TYPE HT_Common_DigitFolding_BaseProc(HASH_INDEX_TYPE, HT_KEY_TYPE);
HASH_INDEX_TYPE HT_Common_DigitFolding_Hash(HASH_INDEX_TYPE, HT_KEY_TYPE);
HASH_INDEX_TYPE HT_Common_DoubleHashing_Hash1(HASH_INDEX_TYPE, HT_KEY_TYPE);
HASH_INDEX_TYPE HT_Common_DoubleHashing_Hash2(HASH_INDEX_TYPE, HT_KEY_TYPE);
#endif