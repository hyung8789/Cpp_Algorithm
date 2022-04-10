#include "HT_Core.h"

OPEN_ADDR_HASH_TABLE* HT_OpenAddr_CreateHashTable(HASH_INDEX_TYPE capacity)
{
	OPEN_ADDR_HASH_TABLE* retVal = NULL;

	if (capacity <= 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	retVal = (OPEN_ADDR_HASH_TABLE*)malloc(sizeof(OPEN_ADDR_HASH_TABLE));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_table = (OPEN_ADDR_NODE*)malloc(sizeof(OPEN_ADDR_NODE) * capacity); //할당 크기만큼 생성
	if (retVal->_table == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	for (HASH_INDEX_TYPE i = 0; i < capacity; i++)
	{
		retVal->_table[i]._state = OPEN_ADDR_NODE_STATE::EMPTY;
		retVal->_table[i]._key = retVal->_table[i]._data = NULL;
	}

	retVal->_capacity = capacity;

	return retVal;
}

void HT_OpenAddr_DeallocateHashTable(OPEN_ADDR_HASH_TABLE** srcHashTable)
{
	if ((*srcHashTable) != NULL)
	{
		for (HASH_INDEX_TYPE i = 0; i < (*srcHashTable)->_capacity; i++)
		{
			free((*srcHashTable)->_table[i]._key);
			free((*srcHashTable)->_table[i]._data);
		}

		free((*srcHashTable)->_table);
		(*srcHashTable)->_table = NULL;

		free(*srcHashTable);
		(*srcHashTable) = NULL;
	}
}

void HT_OpenAddr_DispNodeList(OPEN_ADDR_HASH_TABLE* srcHashTable)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	for (HASH_INDEX_TYPE i = 0; i < srcHashTable->_capacity; i++)
	{
		if (srcHashTable->_table[i]._state == OPEN_ADDR_NODE_STATE::OCCUPIED)
		{
			std::cout << "----------------------------------\n";
			std::cout << "Index [" << i << "]\n";
			std::cout << "Key : " << srcHashTable->_table[i]._key <<
				", Data : " << srcHashTable->_table[i]._data << "\n";
			std::cout << "----------------------------------\n";
		}
	}
}

void HT_OpenAddr_DispIndexListBy(OPEN_ADDR_HASH_TABLE* srcHashTable, OPEN_ADDR_NODE_STATE targetState)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	for (HASH_INDEX_TYPE i = 0; i < srcHashTable->_capacity; i++)
	{
		if (srcHashTable->_table[i]._state == targetState)
		{
			std::cout << "----------------------------------\n";
			std::cout << "Index [" << i << "]\n";
			std::cout << "Key : " << srcHashTable->_table[i]._key <<
				", Data : " << srcHashTable->_table[i]._data << "\n";
			std::cout << "----------------------------------\n";
		}
	}
}

void HT_OpenAddr_InsertData(OPEN_ADDR_HASH_TABLE** srcHashTable, HT_KEY_TYPE srcKey, HT_DATA_TYPE srcData)
{
	if ((*srcHashTable) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (srcKey == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if ((static_cast<double>((*srcHashTable)->_usedSize) / static_cast<double>((*srcHashTable)->_capacity)) <
		CAPACITY_REALLOC_RATIO_THRESHOLD)
	{
		HASH_INDEX_TYPE reallocCapacity = (*srcHashTable)->_capacity + ceil((*srcHashTable)->_capacity * CAPACITY_REALLOC_RATIO); //재 할당 될 크기
		HT_OpenAddr_RehashProc(srcHashTable, reallocCapacity);
	}

	HASH_INDEX_TYPE hashIndex = HT_Common_DoubleHashing_Hash1((*srcHashTable)->_capacity, srcKey);
	HASH_INDEX_TYPE offset = HT_Common_DoubleHashing_Hash2((*srcHashTable)->_capacity, srcKey); //이동 단위

	do
	{
		switch ((*srcHashTable)->_table[hashIndex]._state)
		{
		case OPEN_ADDR_NODE_STATE::EMPTY:
		case OPEN_ADDR_NODE_STATE::DELETED:
			goto INSERT_PROC; //기존 키가 비어있거나, 삭제 된 상태일 경우 중지
		
		case OPEN_ADDR_NODE_STATE::OCCUPIED:
			if (strcmp(srcKey, (*srcHashTable)->_table[hashIndex]._key) == 0)
				goto INSERT_PROC; //기존 키가 사용중이며, 완전히 일치 한 중복 키일 경우 덮어쓰기 위해 중지
			break;
		}

#ifdef HT_DEBUG_MODE
		HT_Common_IncreaseHashCollisionCount(srcKey);
#endif

		hashIndex = (hashIndex + offset) % (*srcHashTable)->_capacity;
	} while (1); //충돌이 발생하는 동안

INSERT_PROC: //삽입 처리 루틴
	switch ((*srcHashTable)->_table[hashIndex]._state)
	{
	case OPEN_ADDR_NODE_STATE::EMPTY:
	case OPEN_ADDR_NODE_STATE::DELETED:
		(*srcHashTable)->_table[hashIndex]._key = (HT_KEY_TYPE)malloc(sizeof(char) * (strlen(srcKey) + 1)); //'\0' 포함 크기
		if ((*srcHashTable)->_table[hashIndex]._key == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		if (strcpy_s((*srcHashTable)->_table[hashIndex]._key, strlen(srcKey) + 1, srcKey) != 0)
			throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

		(*srcHashTable)->_table[hashIndex]._data = (HT_DATA_TYPE)malloc(sizeof(char) * (strlen(srcData) + 1)); //'\0' 포함 크기
		if ((*srcHashTable)->_table[hashIndex]._data == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		break;

	case OPEN_ADDR_NODE_STATE::OCCUPIED: //기존 키가 사용중이며, 완전히 일치 한 중복 키일 경우
		memset((*srcHashTable)->_table[hashIndex]._data, '\0', strlen((*srcHashTable)->_table[hashIndex]._data));

		size_t reallocSizeInBytes = sizeof(char) * (strlen(srcData) + 1); //재 할당 될 바이트 단위 크기 ('\0' 포함 길이)
		if (reallocSizeInBytes != strlen((*srcHashTable)->_table[hashIndex]._data)) //기존 데이터의 크기가 재 할당 될 크기와 다를 경우
		{
			void* reallocAddr = realloc((*srcHashTable)->_table[hashIndex]._data, reallocSizeInBytes);
			if (reallocAddr == NULL)
				throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

			(*srcHashTable)->_table[hashIndex]._data = (HT_DATA_TYPE)reallocAddr;
		}

		break;
	}

	if (strcpy_s((*srcHashTable)->_table[hashIndex]._data, strlen(srcData) + 1, srcData) != 0)
		throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

	(*srcHashTable)->_table[hashIndex]._state = OPEN_ADDR_NODE_STATE::OCCUPIED;
	(*srcHashTable)->_usedSize++;
}

HT_DATA_TYPE HT_OpenAddr_SearchData(OPEN_ADDR_HASH_TABLE* srcHashTable, HT_KEY_TYPE targetKey)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (targetKey == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	HASH_INDEX_TYPE hashIndex = HT_Common_DoubleHashing_Hash1(srcHashTable->_capacity, targetKey);
	HASH_INDEX_TYPE offset = HT_Common_DoubleHashing_Hash2(srcHashTable->_capacity, targetKey); //이동 단위

	HASH_INDEX_TYPE hashIndexInitValue = hashIndex; //해시 인덱스의 초기 값

	do
	{
		switch (srcHashTable->_table[hashIndex]._state)
		{
		case OPEN_ADDR_NODE_STATE::EMPTY:
			goto THROW_NOT_FOUND_EXCEPTION; //최초 EMPTY 상태를 만날 경우 해당 데이터는 실제로 존재하지 않음

		case OPEN_ADDR_NODE_STATE::DELETED:
			break;

		case OPEN_ADDR_NODE_STATE::OCCUPIED:
			if (strcmp(targetKey, srcHashTable->_table[hashIndex]._key) == 0) //기존 키가 사용중이며, 완전히 일치 한 키일 경우
				return srcHashTable->_table[hashIndex]._data;
			break;
		}

		hashIndex = (hashIndex + offset) % srcHashTable->_capacity;
	} while (hashIndex != hashIndexInitValue); //최초 해시 인덱스로 돌아오기 전까지

THROW_NOT_FOUND_EXCEPTION:
	throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));
}

void HT_OpenAddr_RemoveData(OPEN_ADDR_HASH_TABLE* srcHashTable, HT_KEY_TYPE targetKey)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (targetKey == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	HASH_INDEX_TYPE hashIndex = HT_Common_DoubleHashing_Hash1(srcHashTable->_capacity, targetKey);
	HASH_INDEX_TYPE offset = HT_Common_DoubleHashing_Hash2(srcHashTable->_capacity, targetKey); //이동 단위

	HASH_INDEX_TYPE hashIndexInitValue = hashIndex; //해시 인덱스의 초기 값

	do
	{
		switch (srcHashTable->_table[hashIndex]._state)
		{
		case OPEN_ADDR_NODE_STATE::EMPTY:
			break;

		case OPEN_ADDR_NODE_STATE::DELETED:
			//TODO : 현재 위치가 삭제 된 상태일 경우, 찾고자 하는 데이터가 이미 삭제 되었는가? 
			// (최초 해시 인덱스로 돌아올떄까지 탐색을 항상 수행하여야 하는가 ? )

			break;

		case OPEN_ADDR_NODE_STATE::OCCUPIED:
			if (strcmp(targetKey, srcHashTable->_table[hashIndex]._key) == 0) //기존 키가 사용중이며, 완전히 일치 한 키일 경우
			{
				free(srcHashTable->_table[hashIndex]._key);
				srcHashTable->_table[hashIndex]._key = NULL;
				free(srcHashTable->_table[hashIndex]._data);
				srcHashTable->_table[hashIndex]._data = NULL;

				srcHashTable->_table[hashIndex]._state = OPEN_ADDR_NODE_STATE::DELETED;
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

void HT_OpenAddr_RehashProc(OPEN_ADDR_HASH_TABLE** srcHashTable, HASH_INDEX_TYPE reallocCapacity)
{
	if ((*srcHashTable) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (reallocCapacity <= 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	OPEN_ADDR_HASH_TABLE* newHashTable = HT_OpenAddr_CreateHashTable(reallocCapacity);

	for (HASH_INDEX_TYPE i = 0; i < (*srcHashTable)->_capacity; i++)
	{
		switch ((*srcHashTable)->_table[i]._state)
		{
		case OPEN_ADDR_NODE_STATE::EMPTY:
		case OPEN_ADDR_NODE_STATE::DELETED:
			//do nothing
			break;

		case OPEN_ADDR_NODE_STATE::OCCUPIED:
			//사용 중인 기존 데이터에 대해 새 테이블로 이동
			HT_OpenAddr_InsertData(&newHashTable, (*srcHashTable)->_table[i]._key, (*srcHashTable)->_table[i]._data);
			break;
		}
	}

	HT_OpenAddr_DeallocateHashTable(srcHashTable);
	(*srcHashTable) = newHashTable;
}