#include "HT_Core.h"

/// <summary>
/// 할당 크기만큼 해시 테이블 생성 및 반환
/// </summary>
/// <param name="capacity">할당 크기</param>
/// <returns>생성 된 해시 테이블</returns>
CHAINING_RBT_HASH_TABLE* HT_Chaining_RBT_CreateHashTable(HASH_INDEX_TYPE capacity)
{
	CHAINING_RBT_HASH_TABLE* retVal = NULL;

	if (capacity <= 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	retVal = (CHAINING_RBT_HASH_TABLE*)malloc(sizeof(CHAINING_RBT_HASH_TABLE));
	if (retVal == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	retVal->_table = (CHAINING_RBT_TREE*)malloc(sizeof(CHAINING_RBT_TREE) * capacity); //할당 크기만큼 생성
	if (retVal->_table == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

	memset(retVal->_table, NULL, sizeof(CHAINING_RBT_TREE) * capacity);
	retVal->_capacity = capacity;

	return retVal;
}

/// <summary>
/// 대상 해시 테이블에 할당 된 메모리 해제
/// </summary>
/// <param name="srcHashTable">대상 해시 테이블</param>
void HT_Chaining_RBT_DeallocateHashTable(CHAINING_RBT_HASH_TABLE** srcHashTable)
{
	if ((*srcHashTable) != NULL)
	{
		for (HASH_INDEX_TYPE i = 0; i < (*srcHashTable)->_capacity; i++)
		{
			RBT_DeallocateTree(&(*srcHashTable)->_table[i], true);
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
void HT_Chaining_RBT_DispNodeList(CHAINING_RBT_HASH_TABLE* srcHashTable)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	std::cout << "--- Chaining RBT Node List ---\n";

	for (HASH_INDEX_TYPE i = 0; i < srcHashTable->_capacity; i++)
	{
		if (srcHashTable->_table[i] != NULL)
		{
			std::cout << "----------------------------------\n";
			std::cout << "Index [" << i << "]\n";

			RBT_NODE* currentRootNode = srcHashTable->_table[i];

			if(currentRootNode != NULL)
				RBT_DispOrderedTree(currentRootNode, TRAVERSAL_METHOD::INORDER);
			std::cout << "\n";
		}
	}
}

/// <summary>
/// 대상 해시 테이블의 대상 노드의 상태를 가진 노드의 인덱스 목록 출력
/// </summary>
/// <param name="srcHashTable">대상 해시 테이블</param>
/// <param name="targetNodeState">대상 노드의 상태</param>
void HT_Chaining_RBT_DispIndexListBy(CHAINING_RBT_HASH_TABLE* srcHashTable, NODE_STATE targetNodeState)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (targetNodeState == NODE_STATE::DELETED)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	std::cout << "--- Chaining RBT Index List ---\n";

	for (HASH_INDEX_TYPE i = 0; i < srcHashTable->_capacity; i++)
	{
		if ((targetNodeState == NODE_STATE::EMPTY) ?
			srcHashTable->_table[i] == NULL : srcHashTable->_table[i] != NULL)
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
void HT_Chaining_RBT_InsertData(CHAINING_RBT_HASH_TABLE* srcHashTable, HT_KEY_TYPE srcKey, HT_DATA_TYPE srcData)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (srcKey == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	HASH_INDEX_TYPE hashIndex = HT_Common_DigitFolding_Hash(srcHashTable->_capacity, srcKey);

	if (srcHashTable->_table[hashIndex] == NULL) //충돌이 발생하지 않았을 경우
	{
		RBT_InsertNode(&(srcHashTable->_table[hashIndex]),
			RBT_CreateNode(static_cast<RBT_KEY_TYPE>(srcKey), static_cast<RBT_DATA_TYPE>(srcData)));
	}
	else //충돌이 발생했을 경우
	{
#ifdef HT_DEBUG_MODE
		HT_Common_IncreaseHashCollisionCount(srcKey);
#endif

		try //충돌 시 완전히 일치 한 중복 키가 존재 시 덮어쓰기
		{
			RBT_NODE* duplicateKeyNode =
				RBT_SearchNode(srcHashTable->_table[hashIndex], 
					static_cast<RBT_KEY_TYPE>(srcKey)); //중복 키 노드

			size_t reallocSizeInBytes = sizeof(char) * (strlen(srcData) + 1); //재 할당 될 바이트 단위 크기 ('\0' 포함 길이)
			if (reallocSizeInBytes != strlen(duplicateKeyNode->_data)) //기존 데이터의 크기가 재 할당 될 크기와 다를 경우
			{
				void* reallocAddr = realloc(duplicateKeyNode->_data, reallocSizeInBytes);
				if (reallocAddr == NULL)
					throw std::runtime_error(std::string(__func__) + std::string(" : Not enough Heap Memory"));

				duplicateKeyNode->_data = static_cast<RBT_DATA_TYPE>(reallocAddr);
			}
			
			memset(duplicateKeyNode->_data, '\0', strlen(duplicateKeyNode->_data));

			if (strcpy_s(duplicateKeyNode->_data, strlen(srcData) + 1, srcData) != 0)
				throw std::runtime_error(std::string(__func__) + std::string(" : src, dst is null or wrong size"));

			return;
		}
		catch (const myexception::NOT_FOUND_EXCEPTION& ex) //완전히 일치 한 중복 키가 존재하지 않을 경우
		{
			RBT_InsertNode(&(srcHashTable->_table[hashIndex]), 
				RBT_CreateNode(static_cast<RBT_KEY_TYPE>(srcKey), static_cast<RBT_DATA_TYPE>(srcData)));
		}
	}
}

/// <summary>
/// 대상 해시 테이블에 검색하고자 하는 대상 키를 통해 사상 된 해시 인덱스에 따른 데이터 반환
/// </summary>
/// <param name="srcHashTable">대상 해시 테이블</param>
/// <param name="targetKey">검색하고자 하는 대상 키</param>
/// <returns>검색하고자 하는 대상 키를 통해 사상 된 해시 인덱스에 따른 데이터</returns>
HT_DATA_TYPE HT_Chaining_RBT_SearchData(CHAINING_RBT_HASH_TABLE* srcHashTable, HT_KEY_TYPE targetKey)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (targetKey == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	HASH_INDEX_TYPE hashIndex = HT_Common_DigitFolding_Hash(srcHashTable->_capacity, targetKey);
	CHAINING_RBT_TREE rootNode = srcHashTable->_table[hashIndex];

	try
	{
		return static_cast<HT_DATA_TYPE>(RBT_SearchNode(rootNode, static_cast<RBT_KEY_TYPE>(targetKey))->_data);
	}
	catch (const myexception::NOT_FOUND_EXCEPTION& ex)
	{
		throw ex;
	}
}

/// <summary>
/// 대상 해시 테이블에 삭제하고자 하는 대상 키를 통해 사상 된 해시 인덱스에 따른 삭제 대상 노드 삭제
/// </summary>
/// <param name="srcHashTable">대상 해시 테이블</param>
/// <param name="targetKey">삭제하고자 하는 대상 키</param>
void HT_Chaining_RBT_RemoveData(CHAINING_RBT_HASH_TABLE* srcHashTable, HT_KEY_TYPE targetKey)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (targetKey == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	HASH_INDEX_TYPE hashIndex = HT_Common_DigitFolding_Hash(srcHashTable->_capacity, targetKey);

	if (srcHashTable->_table[hashIndex] != NULL)
	{
		try
		{
			RBT_RemoveNode(&(srcHashTable->_table[hashIndex]), static_cast<RBT_KEY_TYPE>(targetKey));
			return;
		}
		catch (const myexception::NOT_FOUND_EXCEPTION& ex)
		{
			throw ex;
		}
	}

	throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));
}
