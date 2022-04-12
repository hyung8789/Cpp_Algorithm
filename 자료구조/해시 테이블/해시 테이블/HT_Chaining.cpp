#include "HT_Core.h"

/// <summary>
/// 할당 크기만큼 해시 테이블 생성 및 반환
/// </summary>
/// <param name="capacity">할당 크기</param>
/// <returns>생성 된 해시 테이블</returns>
CHAINING_HASH_TABLE* HT_Chaining_CreateHashTable(HASH_INDEX_TYPE capacity)
{
	CHAINING_HASH_TABLE* retVal = NULL;

	if (capacity <= 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	retVal = (CHAINING_HASH_TABLE*)malloc(sizeof(CHAINING_HASH_TABLE));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_table = (CHAINING_NODE_LIST*)malloc(sizeof(CHAINING_NODE_LIST) * capacity); //할당 크기만큼 생성
	if (retVal->_table == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	memset(retVal->_table, NULL, sizeof(CHAINING_NODE_LIST) * capacity);
	retVal->_capacity = capacity;

	return retVal;
}

/// <summary>
/// 대상 해시 테이블에 할당 된 메모리 해제
/// </summary>
/// <param name="srcHashTable">대상 해시 테이블</param>
void HT_Chaining_DeallocateHashTable(CHAINING_HASH_TABLE** srcHashTable)
{
	if ((*srcHashTable) != NULL)
	{
		for (HASH_INDEX_TYPE i = 0; i < (*srcHashTable)->_capacity; i++)
		{
			HT_Chaining_DeallocateNodeList(&((*srcHashTable)->_table[i]));
		}

		free((*srcHashTable)->_table);
		(*srcHashTable)->_table = NULL;

		free(*srcHashTable);
		(*srcHashTable) = NULL;
	}
}

/// <summary>
/// 대상 해시 테이블의 전체 노드에 대한 데이터 출력
/// </summary>
/// <param name="srcHashTable">대상 해시 테이블</param>
void HT_Chaining_DispNodeList(CHAINING_HASH_TABLE* srcHashTable)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	for (HASH_INDEX_TYPE i = 0; i < srcHashTable->_capacity; i++)
	{
		if (srcHashTable->_table[i] != NULL)
		{
			std::cout << "----------------------------------\n";
			std::cout << "Index [" << i << "]\n";

			CHAINING_NODE* currentNode = srcHashTable->_table[i];
			
			while (currentNode != NULL)
			{
				std::cout << "Key : " << currentNode->_key <<
					", Data : " << currentNode->_data << "\n";
				
				currentNode = currentNode->_next;
			}

			std::cout << "----------------------------------\n";
		}
	}
}

/// <summary>
/// 대상 해시 테이블의 비어있는 인덱스 출력
/// </summary>
/// <param name="srcHashTable">대상 해시 테이블</param>
void HT_Chaining_DispEmptyIndexList(CHAINING_HASH_TABLE* srcHashTable)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	for (HASH_INDEX_TYPE i = 0; i < srcHashTable->_capacity; i++)
	{
		if (srcHashTable->_table[i] == NULL)
		{
			std::cout << i <<" ";
		}
	}
	std::cout << std::endl;
}

/// <summary>
/// 대상 해시 테이블의 사용 중인 인덱스 출력
/// </summary>
/// <param name="srcHashTable">대상 해시 테이블</param>
void HT_Chaining_DispOccupiedIndexList(CHAINING_HASH_TABLE* srcHashTable)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	for (HASH_INDEX_TYPE i = 0; i < srcHashTable->_capacity; i++)
	{
		if (srcHashTable->_table[i] != NULL)
		{
			std::cout << i <<" ";
		}
	}

	std::cout << std::endl;
}

/// <summary>
/// 대상 키 및 데이터가 포함 된 노드 생성 및 반환
/// </summary>
/// <param name="srcKey">대상 키</param>
/// <param name="srcData">대상 데이터</param>
/// <returns>생성 된 노드</returns>
CHAINING_NODE* HT_Chaining_CreateNode(HT_KEY_TYPE srcKey, HT_DATA_TYPE srcData)
{
	CHAINING_NODE* retVal = NULL;

	retVal = (CHAINING_NODE*)malloc(sizeof(CHAINING_NODE));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_key = (HT_KEY_TYPE)malloc(sizeof(char) * (strlen(srcKey) + 1)); //'\0' 포함 길이
	if (retVal->_key == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	if (strcpy_s(retVal->_key, strlen(srcKey) + 1, srcKey) != 0)
		throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

	retVal->_data = (HT_KEY_TYPE)malloc(sizeof(char) * (strlen(srcData) + 1)); //'\0' 포함 길이
	if (retVal->_data == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	if (strcpy_s(retVal->_data, strlen(srcData) + 1, srcData) != 0)
		throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

	retVal->_next = NULL;

	return retVal;
}

/// <summary>
/// 대상 노드에 할당 된 메모리 해제
/// </summary>
/// <param name="srcNode">대상 노드</param>
void HT_Chaining_DeallocateNode(CHAINING_NODE** srcNode)
{
	if ((*srcNode) != NULL)
	{
		free((*srcNode)->_key);
		free((*srcNode)->_data);
		free(*srcNode);

		(*srcNode) = NULL;
	}
}

/// <summary>
/// 대상 노드 리스트에 할당 된 메모리 해제
/// </summary>
/// <param name="srcNodeList">대상 노드 리스트</param>
void HT_Chaining_DeallocateNodeList(CHAINING_NODE_LIST* srcNodeList)
{
	if ((*srcNodeList) != NULL)
	{
		while ((*srcNodeList) != NULL)
		{
			CHAINING_NODE* nextNode = (*srcNodeList)->_next;
			HT_Chaining_DeallocateNode(srcNodeList);
			(*srcNodeList) = nextNode;
		}
	}
}

/// <summary>
/// 대상 해시 테이블에 삽입하고자 하는 대상 키를 통해 사상 된 해시 인덱스에 따른 삽입하고자 하는 대상 데이터 삽입
/// </summary>
/// <param name="srcHashTable">대상 해시 테이블</param>
/// <param name="srcKey">삽입하고자 하는 대상 키</param>
/// <param name="srcData">삽입하고자 하는 대상 데이터</param>
void HT_Chaining_InsertData(CHAINING_HASH_TABLE* srcHashTable, HT_KEY_TYPE srcKey, HT_DATA_TYPE srcData)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (srcKey == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	HASH_INDEX_TYPE hashIndex = HT_Common_DigitFolding_Hash(srcHashTable->_capacity, srcKey);
	CHAINING_NODE* newNode = NULL;

	if (srcHashTable->_table[hashIndex] == NULL) //충돌이 발생하지 않았을 경우
	{
		newNode = HT_Chaining_CreateNode(srcKey, srcData);
		srcHashTable->_table[hashIndex] = newNode;
	}
	else //충돌이 발생했을 경우
	{
#ifdef HT_DEBUG_MODE
		HT_Common_IncreaseHashCollisionCount(srcKey);
#endif

		CHAINING_NODE_LIST currentNode = srcHashTable->_table[hashIndex];

		while (currentNode != NULL) //충돌 시 완전히 일치한 중복 키가 존재 시 덮어쓰기
		{
			if (strcmp(srcKey, currentNode->_key) == 0) //완전히 일치한 중복 키가 존재하면
			{
				size_t reallocSizeInBytes = sizeof(char) * (strlen(srcData) + 1); //재 할당 될 바이트 단위 크기 ('\0' 포함 길이)
				if (reallocSizeInBytes != strlen(currentNode->_data)) //기존 데이터의 크기가 재 할당 될 크기와 다를 경우
				{
					void* reallocAddr = realloc(currentNode->_data, reallocSizeInBytes);
					if (reallocAddr == NULL)
						throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

					currentNode->_data = (HT_DATA_TYPE)reallocAddr;
				}

				memset(currentNode->_data, '\0', strlen(currentNode->_data));

				if (strcpy_s(currentNode->_data, strlen(srcData) + 1, srcData) != 0)
					throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

				goto END_PROC;
			}

			currentNode = currentNode->_next;
		}

		newNode = HT_Chaining_CreateNode(srcKey, srcData);
		newNode->_next = srcHashTable->_table[hashIndex]; //새 노드를 기존 노드의 앞에 삽입
		srcHashTable->_table[hashIndex] = newNode;
	}

END_PROC:
	return;
}

/// <summary>
/// 대상 해시 테이블에 검색하고자 하는 대상 키를 통해 사상 된 해시 인덱스에 따른 데이터 반환
/// </summary>
/// <param name="srcHashTable">대상 해시 테이블</param>
/// <param name="targetKey">검색하고자 하는 대상 키</param>
/// <returns>검색하고자 하는 대상 키를 통해 사상 된 해시 인덱스에 따른 데이터</returns>
HT_DATA_TYPE HT_Chaining_SearchData(CHAINING_HASH_TABLE* srcHashTable, HT_KEY_TYPE targetKey)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (targetKey == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	HASH_INDEX_TYPE hashIndex = HT_Common_DigitFolding_Hash(srcHashTable->_capacity, targetKey);
	CHAINING_NODE_LIST currentNode = srcHashTable->_table[hashIndex];

	while (currentNode != NULL)
	{
		if (strcmp(targetKey, currentNode->_key) == 0) //키가 일치하면
			return currentNode->_data;

		currentNode = currentNode->_next;
	}

	throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));
}

/// <summary>
/// 대상 해시 테이블에 삭제하고자 하는 대상 키를 통해 사상 된 해시 인덱스에 따른 삭제 대상 노드 삭제
/// </summary>
/// <param name="srcHashTable">대상 해시 테이블</param>
/// <param name="targetKey">삭제하고자 하는 대상 키</param>
/// <param name="deallocateAfterRemove">삭제 대상 노드에 대한 메모리 해제 수행 여부</param>
void HT_Chaining_RemoveData(CHAINING_HASH_TABLE* srcHashTable, HT_KEY_TYPE targetKey, bool deallocateAfterRemove)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (targetKey == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	HASH_INDEX_TYPE hashIndex = HT_Common_DigitFolding_Hash(srcHashTable->_capacity, targetKey);
	CHAINING_NODE* removeTargetNode = NULL; //삭제 대상 노드
	CHAINING_NODE* removeTargetPrevNode = NULL; //삭제 대상 노드의 이전 노드

	if (srcHashTable->_table[hashIndex] != NULL)
	{
		CHAINING_NODE_LIST currentNode = srcHashTable->_table[hashIndex];

		while (currentNode != NULL)
		{
			if (strcmp(targetKey, currentNode->_key) == 0) //삭제 대상 키와 일치 할 경우
			{
				removeTargetNode = currentNode;

				if (removeTargetPrevNode != NULL) //삭제 대상 노드의 이전 노드가 존재 할 경우
					removeTargetPrevNode->_next = removeTargetNode->_next;
				else //삭제 대상 노드의 이전 노드가 존재하지 않을 경우
					srcHashTable->_table[hashIndex] = removeTargetPrevNode; //삭제 대상 노드가 헤드 노드이므로, 현재 헤드 노드 변경
				break;
			}

			removeTargetPrevNode = currentNode;
			currentNode = currentNode->_next;
		}
	}

	if (removeTargetNode == NULL)
		throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));

	if (deallocateAfterRemove)
		HT_Chaining_DeallocateNode(&removeTargetNode);
}