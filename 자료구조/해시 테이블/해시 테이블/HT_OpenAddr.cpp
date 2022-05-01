#include "HT_Core.h"

/// <summary>
/// 할당 크기만큼 해시 테이블 생성 및 반환
/// </summary>
/// <param name="capacity">할당 크기</param>
/// <returns>생성 된 해시 테이블</returns>
OPENADDR_HASH_TABLE* HT_OpenAddr_CreateHashTable(HASH_INDEX_TYPE capacity)
{
	OPENADDR_HASH_TABLE* retVal = NULL;

	if (capacity <= 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	retVal = (OPENADDR_HASH_TABLE*)malloc(sizeof(OPENADDR_HASH_TABLE));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_table = (OPENADDR_NODE*)malloc(sizeof(OPENADDR_NODE) * capacity); //할당 크기만큼 생성
	if (retVal->_table == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	for (HASH_INDEX_TYPE i = 0; i < capacity; i++)
	{
		retVal->_table[i]._state = NODE_STATE::EMPTY;
		retVal->_table[i]._key = retVal->_table[i]._data = NULL;
	}

	retVal->_capacity = capacity;
	retVal->_usedSize = 0;

	return retVal;
}

/// <summary>
/// 대상 해시 테이블에 할당 된 메모리 해제
/// </summary>
/// <param name="srcHashTable">대상 해시 테이블</param>
void HT_OpenAddr_DeallocateHashTable(OPENADDR_HASH_TABLE** srcHashTable)
{
	if ((*srcHashTable) != NULL)
	{
		for (HASH_INDEX_TYPE i = 0; i < (*srcHashTable)->_capacity; i++)
		{
			HT_OpenAddr_DeallocateNode(&((*srcHashTable)->_table[i]));
		}

		free((*srcHashTable)->_table);
		(*srcHashTable)->_table = NULL;

		free(*srcHashTable);
		(*srcHashTable) = NULL;
	}
}

/// <summary>
/// 대상 노드에 할당 된 메모리 해제
/// </summary>
/// <param name="srcNode">대상 노드</param>
void HT_OpenAddr_DeallocateNode(OPENADDR_NODE* srcNode)
{
	if (srcNode != NULL)
	{
		free(srcNode->_key);
		free(srcNode->_data);

		srcNode->_key = NULL;
		srcNode->_data = NULL;
	}
}

/// <summary>
/// 대상 해시 테이블의 전체 노드에 대한 데이터 출력
/// </summary>
/// <param name="srcHashTable">대상 해시 테이블</param>
void HT_OpenAddr_DispNodeList(OPENADDR_HASH_TABLE* srcHashTable)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	std::cout << "--- OpenAddr Node List ---\n";

	for (HASH_INDEX_TYPE i = 0; i < srcHashTable->_capacity; i++)
	{
		if (srcHashTable->_table[i]._state == NODE_STATE::OCCUPIED)
		{
			std::cout << "----------------------------------\n";
			std::cout << "Index [" << i << "]\n";
			std::cout << "Key : " << srcHashTable->_table[i]._key <<
				", Data : " << srcHashTable->_table[i]._data << "\n";
			std::cout << "\n";
		}
	}
}

/// <summary>
/// 대상 해시 테이블의 대상 노드의 상태를 가진 노드의 인덱스 목록 출력
/// </summary>
/// <param name="srcHashTable">대상 해시 테이블</param>
/// <param name="targetNodeState">대상 노드의 상태</param>
void HT_OpenAddr_DispIndexListBy(OPENADDR_HASH_TABLE* srcHashTable, NODE_STATE targetNodeState)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	std::cout << "--- OpenAddr Index List ---\n";

	for (HASH_INDEX_TYPE i = 0; i < srcHashTable->_capacity; i++)
	{
		if (srcHashTable->_table[i]._state == targetNodeState)
		{
			std::cout << i << " ";
		}
	}

	std::cout << std::endl;
}

/// <summary>
/// 대상 해시 테이블에 삽입하고자 하는 대상 키를 통해 사상 된 해시 인덱스에 따른 삽입하고자 하는 대상 데이터 삽입
/// </summary>
/// <param name="srcHashTable">대상 해시 테이블</param>
/// <param name="srcKey">삽입하고자 하는 대상 키</param>
/// <param name="srcData">삽입하고자 하는 대상 데이터</param>
void HT_OpenAddr_InsertData(OPENADDR_HASH_TABLE** srcHashTable, HT_KEY_TYPE srcKey, HT_DATA_TYPE srcData)
{
	if ((*srcHashTable) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (srcKey == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (((static_cast<double>((*srcHashTable)->_usedSize) / static_cast<double>((*srcHashTable)->_capacity))) >=
		CAPACITY_INCREASE_RATIO_THRESHOLD) //사용량이 기존 할당 크기에 대한 증가가 발생 될 임계 비율 이상일 경우
	{
#ifdef HT_DEBUG_MODE
		VAR_DUMP((*srcHashTable)->_usedSize);
		VAR_DUMP((*srcHashTable)->_capacity);
		VAR_DUMP((static_cast<double>((*srcHashTable)->_usedSize) / static_cast<double>((*srcHashTable)->_capacity)));
#endif

		HASH_INDEX_TYPE reallocCapacity = (*srcHashTable)->_capacity + ceil((*srcHashTable)->_capacity * CAPACITY_INCREASE_RATIO); //재 할당 될 크기
		HT_OpenAddr_RehashingProc(srcHashTable, reallocCapacity);
	}

	HASH_INDEX_TYPE hashIndex = HT_Common_DoubleHashing_Hash1((*srcHashTable)->_capacity, srcKey);
	HASH_INDEX_TYPE offset = HT_Common_DoubleHashing_Hash2((*srcHashTable)->_capacity, srcKey); //이동 단위
	const HASH_INDEX_TYPE hashIndexInitValue = hashIndex; //해시 인덱스의 초기 값

	size_t reallocSizeInBytes = 0; //재 할당 될 바이트 단위 크기 ('\0' 포함 길이)
	void* reallocAddr = NULL;

	do
	{
		switch ((*srcHashTable)->_table[hashIndex]._state)
		{
		case NODE_STATE::EMPTY:
			goto INSERT_PROC;

		case NODE_STATE::DELETED:
			if (strcmp(srcKey, (*srcHashTable)->_table[hashIndex]._key) == 0) //대상 키와 완전히 일치 한 키일 경우 덮어쓰기
				goto OVERWRITE_PROC;
			else //EMPTY 상태로 간주
				goto INSERT_PROC;

		case NODE_STATE::OCCUPIED:
#ifdef HT_DEBUG_MODE
			HT_Common_IncreaseHashCollisionCount(srcKey);
#endif
			if (strcmp(srcKey, (*srcHashTable)->_table[hashIndex]._key) == 0) //대상 키와 완전히 일치 한 키일 경우 덮어쓰기
				goto OVERWRITE_PROC;
			break;
		}

		hashIndex = (hashIndex + offset) % (*srcHashTable)->_capacity;
	} while (hashIndex != hashIndexInitValue); //최초 해시 인덱스로 돌아오기 전까지

	//RehashingProc에 의해, 빈 공간이 반드시 존재하여야 함
	throw myexception::MEM_CORRUPTION_EXCEPTION(std::string(__func__) + std::string(" : Mem corruption"));

INSERT_PROC: //삽입 처리 루틴
	if ((*srcHashTable)->_table[hashIndex]._key != NULL) //EMPTY 상태로 간주 된 기존 키의 길이에 따라 (DELETED)
	{
		reallocSizeInBytes = sizeof(char) * (strlen(srcKey) + 1);
		if (reallocSizeInBytes != strlen((*srcHashTable)->_table[hashIndex]._key)) //기존 키의 크기가 재 할당 될 크기와 다를 경우
		{
			reallocAddr = realloc((*srcHashTable)->_table[hashIndex]._key, reallocSizeInBytes);
			if (reallocAddr == NULL)
				throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

			(*srcHashTable)->_table[hashIndex]._key = (HT_KEY_TYPE)reallocAddr;
		}

		memset((*srcHashTable)->_table[hashIndex]._key, '\0', strlen((*srcHashTable)->_table[hashIndex]._key));
	}
	else //((*srcHashTable)->_table[hashIndex]._key == NULL)
	{
		(*srcHashTable)->_table[hashIndex]._key = (HT_KEY_TYPE)malloc(sizeof(char) * (strlen(srcKey) + 1)); //'\0' 포함 크기
		if ((*srcHashTable)->_table[hashIndex]._key == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));
	}

	if (strcpy_s((*srcHashTable)->_table[hashIndex]._key, strlen(srcKey) + 1, srcKey) != 0)
		throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

	(*srcHashTable)->_table[hashIndex]._data = (HT_DATA_TYPE)malloc(sizeof(char) * (strlen(srcData) + 1)); //'\0' 포함 크기
	if ((*srcHashTable)->_table[hashIndex]._data == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	if (strcpy_s((*srcHashTable)->_table[hashIndex]._data, strlen(srcData) + 1, srcData) != 0)
		throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

	goto END_PROC;

OVERWRITE_PROC: //덮어쓰기 처리 루틴
	reallocSizeInBytes = sizeof(char) * (strlen(srcData) + 1);
	if (reallocSizeInBytes != strlen((*srcHashTable)->_table[hashIndex]._data)) //기존 데이터의 크기가 재 할당 될 크기와 다를 경우
	{
		reallocAddr = realloc((*srcHashTable)->_table[hashIndex]._data, reallocSizeInBytes);
		if (reallocAddr == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		(*srcHashTable)->_table[hashIndex]._data = (HT_DATA_TYPE)reallocAddr;
	}

	memset((*srcHashTable)->_table[hashIndex]._data, '\0', strlen((*srcHashTable)->_table[hashIndex]._data));

	if (strcpy_s((*srcHashTable)->_table[hashIndex]._data, strlen(srcData) + 1, srcData) != 0)
		throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

	goto END_PROC;

END_PROC:
	(*srcHashTable)->_table[hashIndex]._state = NODE_STATE::OCCUPIED;
	(*srcHashTable)->_usedSize++;
}

/// <summary>
/// 대상 해시 테이블에 검색하고자 하는 대상 키를 통해 사상 된 해시 인덱스에 따른 데이터 반환
/// </summary>
/// <param name="srcHashTable">대상 해시 테이블</param>
/// <param name="targetKey">검색하고자 하는 대상 키</param>
/// <returns>검색하고자 하는 대상 키를 통해 사상 된 해시 인덱스에 따른 데이터</returns>
HT_DATA_TYPE HT_OpenAddr_SearchData(OPENADDR_HASH_TABLE* srcHashTable, HT_KEY_TYPE targetKey)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (targetKey == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	HASH_INDEX_TYPE hashIndex = HT_Common_DoubleHashing_Hash1(srcHashTable->_capacity, targetKey);
	HASH_INDEX_TYPE offset = HT_Common_DoubleHashing_Hash2(srcHashTable->_capacity, targetKey); //이동 단위
	const HASH_INDEX_TYPE hashIndexInitValue = hashIndex; //해시 인덱스의 초기 값

	do
	{
		switch (srcHashTable->_table[hashIndex]._state)
		{
		case NODE_STATE::EMPTY:
			goto THROW_NOT_FOUND_EXCEPTION; //대상 키에 해당하는 데이터는 실제로 존재하지 않음

		case NODE_STATE::DELETED:
			if (strcmp(targetKey, srcHashTable->_table[hashIndex]._key) == 0) //대상 키와 완전히 일치 한 키일 경우
				goto THROW_NOT_FOUND_EXCEPTION;
			break;

		case NODE_STATE::OCCUPIED:
			if (strcmp(targetKey, srcHashTable->_table[hashIndex]._key) == 0) //대상 키와 완전히 일치 한 키일 경우
				return srcHashTable->_table[hashIndex]._data;
			break;
		}

		hashIndex = (hashIndex + offset) % srcHashTable->_capacity;
	} while (hashIndex != hashIndexInitValue); //최초 해시 인덱스로 돌아오기 전까지

THROW_NOT_FOUND_EXCEPTION:
	throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));
}

/// <summary>
/// 대상 해시 테이블에 삭제하고자 하는 대상 키를 통해 사상 된 해시 인덱스에 따른 삭제 대상 노드 삭제
/// </summary>
/// <param name="srcHashTable">대상 해시 테이블</param>
/// <param name="targetKey">삭제하고자 하는 대상 키</param>
void HT_OpenAddr_RemoveData(OPENADDR_HASH_TABLE* srcHashTable, HT_KEY_TYPE targetKey)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (targetKey == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	HASH_INDEX_TYPE hashIndex = HT_Common_DoubleHashing_Hash1(srcHashTable->_capacity, targetKey);
	HASH_INDEX_TYPE offset = HT_Common_DoubleHashing_Hash2(srcHashTable->_capacity, targetKey); //이동 단위
	const HASH_INDEX_TYPE hashIndexInitValue = hashIndex; //해시 인덱스의 초기 값

	do
	{
		switch (srcHashTable->_table[hashIndex]._state)
		{
		case NODE_STATE::EMPTY:
			goto THROW_NOT_FOUND_EXCEPTION; //대상 키에 해당하는 데이터는 실제로 존재하지 않음

		case NODE_STATE::DELETED:
			if (strcmp(targetKey, srcHashTable->_table[hashIndex]._key) == 0) //대상 키와 완전히 일치 한 키일 경우
				goto THROW_NOT_FOUND_EXCEPTION;
			break;

		case NODE_STATE::OCCUPIED:
			if (strcmp(targetKey, srcHashTable->_table[hashIndex]._key) == 0) //대상 키와 완전히 일치 한 키일 경우
			{
				//탐색 수행 횟수를 줄이기 위해 기존 키 보존
				free(srcHashTable->_table[hashIndex]._data);
				srcHashTable->_table[hashIndex]._data = NULL;

				srcHashTable->_table[hashIndex]._state = NODE_STATE::DELETED;
				srcHashTable->_usedSize--;
				return;
			}

			break;
		}

		hashIndex = (hashIndex + offset) % srcHashTable->_capacity;
	} while (hashIndex != hashIndexInitValue); //최초 해시 인덱스로 돌아오기 전까지

THROW_NOT_FOUND_EXCEPTION:
	throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));
}

/// <summary>
/// 대상 해시 테이블에 재 할당 크기로 기존 데이터에 대한 재해싱 수행
/// </summary>
/// <param name="srcHashTable">대상 해시 테이블</param>
/// <param name="reallocCapacity">재 할당 크기</param>
void HT_OpenAddr_RehashingProc(OPENADDR_HASH_TABLE** srcHashTable, HASH_INDEX_TYPE reallocCapacity)
{
	if ((*srcHashTable) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if ((reallocCapacity <= 0) || (*srcHashTable)->_usedSize > reallocCapacity)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	OPENADDR_HASH_TABLE* newHashTable = HT_OpenAddr_CreateHashTable(reallocCapacity);

	for (HASH_INDEX_TYPE i = 0; i < (*srcHashTable)->_capacity; i++)
	{
		switch ((*srcHashTable)->_table[i]._state)
		{
		case NODE_STATE::EMPTY:
			//do nothing
			break;

		case NODE_STATE::DELETED:
			//삽입 과정을 위한 힙 메모리 확보
			HT_OpenAddr_DeallocateNode(&((*srcHashTable)->_table[i]));
			break;

		case NODE_STATE::OCCUPIED:
			//사용 중인 기존 데이터에 대해 새 테이블로 이동
			HT_OpenAddr_InsertData(&newHashTable, (*srcHashTable)->_table[i]._key, (*srcHashTable)->_table[i]._data);
			
			//삽입 과정을 위한 힙 메모리 확보
			HT_OpenAddr_DeallocateNode(&((*srcHashTable)->_table[i]));
			break;
		}
	}

	HT_OpenAddr_DeallocateHashTable(srcHashTable);
	(*srcHashTable) = newHashTable;
}