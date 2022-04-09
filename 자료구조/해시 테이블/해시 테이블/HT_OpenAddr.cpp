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

	memset(retVal->_table, NULL, sizeof(OPEN_ADDR_NODE) * capacity);
	retVal->_capacity = capacity;

	return retVal;
}

void HT_Chaining_DeallocateHashTable(OPEN_ADDR_HASH_TABLE** srcHashTable)
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

void HT_OpenAddr_InsertData(OPEN_ADDR_HASH_TABLE* srcHashTable, HT_KEY_TYPE srcKey, HT_DATA_TYPE srcData)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (srcKey == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	//TODO : 사용량에 대해 재해싱 수행

	HASH_INDEX_TYPE hashIndex = HT_Common_DoubleHashing_Hash1(srcHashTable->_capacity, srcKey);
	HASH_INDEX_TYPE offset = HT_Common_DoubleHashing_Hash2(srcHashTable->_capacity, srcKey); //이동 단위

	do
	{
		if ((srcHashTable->_table[hashIndex]._key == NULL) ||
			(srcHashTable->_table[hashIndex]._key != NULL && 
				strcmp(srcKey, srcHashTable->_table[hashIndex]._key) == 0))
			break;
			
#ifdef HT_DEBUG_MODE
		hashCollisionCount++;
		std::cout << "충돌 발생 : " << srcKey << " (" << hashCollisionCount << ")\n";
#endif

		hashIndex = (hashIndex + offset) % srcHashTable->_capacity;
	} while (1); //충돌이 발생하는 동안 탐색

	srcHashTable->_table[hashIndex]._key = (HT_KEY_TYPE)malloc(sizeof(char) * (strlen(srcKey) + 1)); //'\0' 포함 크기
	if (srcHashTable->_table[hashIndex]._key == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	if (strcpy_s(srcHashTable->_table[hashIndex]._key, strlen(srcKey) + 1, srcKey) != 0)
		throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

	srcHashTable->_table[hashIndex]._data = (HT_DATA_TYPE)malloc(sizeof(char) * (strlen(srcData) + 1)); //'\0' 포함 크기
	if (srcHashTable->_table[hashIndex]._data == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	if (strcpy_s(srcHashTable->_table[hashIndex]._data, strlen(srcData) + 1, srcData) != 0)
		throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

	srcHashTable->_usedSize++;
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
		if (srcHashTable->_table[hashIndex]._key != NULL && 
			strcmp(targetKey, srcHashTable->_table[hashIndex]._key) == 0)
		{
			//found
		}
		
		//TODO : 삽입 과정과 동일한 시퀸스로 이중 해싱을 수행하여, 데이터 삭제 시 순서와 상관없이 삭제 되므로 탐색 간
		//비어있는 키를 최초 만나더라도 실제 해당 데이터가 존재할 수 있음 => 
		//전체 테이블에 대해 키와 연관 된 전체 인덱스에 대한 조회를 항상 수행해야 하는가?
		//TODO : 노드에 대해 별도의 상태를 관리하여야 하는가?
		
		//if (srcHashTable->_table[hashIndex]._key == NULL)
			//break;


		hashIndex = (hashIndex + offset) % srcHashTable->_capacity;
	} while (hashIndex != hashIndexInitValue); //최초 해시 인덱스로 돌아오기 전까지



	throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));
}

void HT_OpenAddr_RemoveData(OPEN_ADDR_HASH_TABLE* srcHashTable, HT_KEY_TYPE targetKey)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (targetKey == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));


}

void HT_OpenAddr_Rehashing(OPEN_ADDR_HASH_TABLE** srcHashTable)
{
	if ((*srcHashTable) == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

}
