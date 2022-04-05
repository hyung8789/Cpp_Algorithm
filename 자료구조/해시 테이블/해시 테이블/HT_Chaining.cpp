#include "HT_Core.h"

extern HASH_FUNC_ADDR_TYPE hashFunc;

CHAINING_HASH_TABLE* HT_Chaining_CreateHashTable(HASH_INDEX_TYPE capacity)
{
	CHAINING_HASH_TABLE* retVal = NULL;

	if (capacity <= 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	retVal = (CHAINING_HASH_TABLE*)malloc(sizeof(CHAINING_HASH_TABLE));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_table = (CHAINING_NODE**)malloc(sizeof(CHAINING_NODE) * capacity); //할당 크기만큼 생성
	if (retVal->_table == NULL)
	{
		HT_Chaining_DeallocateHashTable(&retVal);
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));
	}

	memset(retVal->_table, NULL, sizeof(CHAINING_NODE) * capacity);
	retVal->_capacity = capacity;

	return retVal;
}

void HT_Chaining_DeallocateHashTable(CHAINING_HASH_TABLE** srcHashTable)
{
	if ((*srcHashTable) != NULL)
	{
		if ((*srcHashTable)->_table != NULL)
		{
			for (HASH_INDEX_TYPE i = 0; i < (*srcHashTable)->_capacity; i++)
			{
				HT_Chaining_DeallocateNodeList(&((*srcHashTable)->_table[i]));
			}

			free((*srcHashTable)->_table);
			(*srcHashTable)->_table = NULL;
		}

		free(*srcHashTable);
		(*srcHashTable) = NULL;
	}
}

CHAINING_NODE* HT_Chaining_CreateNode(KEY_TYPE srcKey, DATA_TYPE srcData)
{
	CHAINING_NODE* retVal = NULL;

	try
	{
		retVal = (CHAINING_NODE*)malloc(sizeof(CHAINING_NODE));
		if (retVal == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		retVal->_key = (KEY_TYPE)malloc(sizeof(KEY_TYPE) * (strlen(srcKey) + 1)); //'\0' 포함 길이
		if (retVal->_key == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		if (strcpy_s(retVal->_key, strlen(srcKey) + 1, srcKey) != 0)
			throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

		retVal->_data = (KEY_TYPE)malloc(sizeof(DATA_TYPE) * (strlen(srcData) + 1)); //'\0' 포함 길이
		if (retVal->_data == NULL)
			throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

		if (strcpy_s(retVal->_data, strlen(srcData) + 1, srcData) != 0)
			throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

		retVal->_next = NULL;

	}
	catch (const std::exception& ex)
	{
		HT_Chaining_DeallocateNode(&retVal);
		throw ex;
	}

	return retVal;
}

void HT_Chaining_DeallocateNode(CHAINING_NODE** srcNode)
{
	if ((*srcNode) != NULL)
	{
		if ((*srcNode)->_key != NULL)
			free((*srcNode)->_key);

		if ((*srcNode)->_data != NULL)
			free((*srcNode)->_data);

		free(*srcNode);

		(*srcNode) = NULL;
	}
}

void HT_Chaining_DeallocateNodeList(CHAINING_NODE** srcNodeList)
{
	if ((*srcNodeList) != NULL)
	{
		while ((*srcNodeList) != NULL)
		{
			CHAINING_NODE* tmp = (*srcNodeList); //삭제 할 노드
			(*srcNodeList) = (*srcNodeList)->_next; //다음 노드로 이동 후 삭제
			HT_Chaining_DeallocateNode(&tmp);
		}
	}
}

void HT_Chaining_InsertData(CHAINING_HASH_TABLE* srcHashTable, KEY_TYPE srcKey, DATA_TYPE srcData)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	HASH_INDEX_TYPE hashIndex = hashFunc(srcHashTable->_capacity, srcKey);
	CHAINING_NODE* newNode = HT_Chaining_CreateNode(srcKey, srcData);

	//TODO : 할당 해제 연산을 없애기 위해, 지연 된 새 노드 생성 할 것 (충돌이 발생하지 않았거나, 완전히 일치한 중복키가 존재하지 않을 경우에 대해)

	if (srcHashTable->_table[hashIndex] == NULL) //충돌이 발생하지 않았을 경우
	{
		srcHashTable->_table[hashIndex] = newNode;
	}
	else //충돌이 발생했을 경우
	{
		CHAINING_NODE* nodeList = srcHashTable->_table[hashIndex];

		while (nodeList != NULL) ////충돌 시 완전히 일치한 중복 키에 대해 기존 데이터 갱신
		{
			if (strcmp(srcKey, nodeList->_key) == 0) //키가 일치하면
			{
				nodeList->_data = newNode->_data; //기존 데이터 갱신
				break;
			}

			nodeList = nodeList->_next;
		}

		//새 노드를 기존 노드의 앞에 삽입
		srcNewNode->_next = srcHashTable->_table[hashIndex];
		srcHashTable->_table[hashIndex] = srcNewNode;
	}

END_PROC:
	return;
}

DATA_TYPE HT_Chaining_SearchData(CHAINING_HASH_TABLE* srcHashTable, KEY_TYPE targetKey)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	HASH_INDEX_TYPE hashIndex = hashFunc(srcHashTable->_capacity, targetKey);
	CHAINING_NODE* nodeList = srcHashTable->_table[hashIndex];

	while (nodeList != NULL)
	{
		if (strcmp(targetKey, nodeList->_key) == 0) //키가 일치하면
			return nodeList->_data;
		
		nodeList = nodeList->_next;
	}

	throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));
}

void HT_Chaining_RemoveData(CHAINING_HASH_TABLE* srcHashTable, KEY_TYPE targetKey, bool deallocateAfterRemove)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	HASH_INDEX_TYPE hashIndex = hashFunc(srcHashTable->_capacity, targetKey);
	CHAINING_NODE* removeTargetNode = NULL; //삭제 대상 노드

	//TODO : 찾을 경우 이전 노드의 다음 변경 할 것

	if (removeTargetNode == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (deallocateAfterRemove)
		HT_Chaining_DeallocateNode(&removeTargetNode);
}