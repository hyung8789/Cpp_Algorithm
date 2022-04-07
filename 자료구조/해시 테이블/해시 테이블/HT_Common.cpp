#include "HT_Core.h"

/// <summary>
/// 할당 크기 및 대상 키에 대해 자릿수 접기를 통해 사상 된 해시 인덱스 반환
/// </summary>
/// <param name="capacity">할당 크기</param>
/// <param name="srcKey">대상 키</param>
/// <returns>사상 된 해시 인덱스</returns>
HASH_INDEX_TYPE HT_Common_DigitFolding_Hash(HASH_INDEX_TYPE capacity, HT_KEY_TYPE srcKey)
{
	/***
		< 할당량에 대해 Hash Index의 충돌 최소화 및 사상 범위 확대 >

		- Digit Folding에 의해 사상 된 Hash Index의 최대 값 : 127 (아스키 코드에 의한 문자 개수) * 키의 자리 수
		- 테이블 할당량에 대해 Hash Index로 사상되지 않는 범위의 비트 개수 : 할당량의 비트 개수 - 키의 비트 개수

		---

		ex)
		- 할당량 : 1234567 (0001 0010 1101 0110 1000 0111)
		- 키의 자리 수 : 10
		- Digit Folding에 의해 사상 된 Hash Index의 최대 값 : 127 * 10 = 1270 (0000 0100 1111 0110)

		0001 0010 1101 0110 1000 0111
		0000 0000 0000 0100 1111 0110

		Digit Folding에 의해 사상 된 Hash Index의 할당량에 대해 2^11 부터 2^23까지 사상 범위를 확대하기 위해,
		Digit Folding에 의해 누적 연산 시마다 남는 자리수만큼 왼쪽 쉬프트 할 것
	***/

	HASH_INDEX_TYPE hashIndex = 0;

	size_t srcKeyLength = strlen(srcKey); //대상 키의 문자열 길이
	HASH_INDEX_TYPE leftShiftCount = utils::GetBitCountFrom(capacity) - utils::GetBitCountFrom(srcKeyLength);

	for (size_t i = 0; i < srcKeyLength; i++)
		hashIndex = (hashIndex << leftShiftCount) + utils::CharToDecAscii(srcKey[i]);

	return hashIndex % capacity;
}