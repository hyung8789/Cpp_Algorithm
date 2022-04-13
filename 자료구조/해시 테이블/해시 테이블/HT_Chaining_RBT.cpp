#include "HT_Core.h"

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

void HT_Chaining_RBT_DeallocateHashTable(CHAINING_RBT_HASH_TABLE** srcHashTable)
{
	if ((*srcHashTable) != NULL)
	{
		for (HASH_INDEX_TYPE i = 0; i < (*srcHashTable)->_capacity; i++)
		{
			HT_Chaining_RBT_DeallocateTree(&((*srcHashTable)->_table[i]));
		}

		free((*srcHashTable)->_table);
		(*srcHashTable)->_table = NULL;

		free(*srcHashTable);
		(*srcHashTable) = NULL;
	}
}

CHAINING_RBT_NODE* HT_Chaining_RBT_CreateNode(HT_KEY_TYPE srcKey, HT_DATA_TYPE srcData)
{
	CHAINING_RBT_NODE* retVal = NULL;

	retVal = (CHAINING_RBT_NODE*)malloc(sizeof(CHAINING_RBT_NODE));
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

	retVal->_color = COLOR::RED; //항상 새 노드는 빨간색
	retVal->_parent = retVal->_left = retVal->_right = NULL;

	return retVal;
}

void HT_Chaining_RBT_DeallocateNode(CHAINING_RBT_NODE** srcNode)
{
	if ((*srcNode) != NULL)
	{
		free((*srcNode)->_key);
		free((*srcNode)->_data);
		free(*srcNode);

		(*srcNode) = NULL;
	}
}

void HT_Chaining_RBT_DeallocateTree(CHAINING_RBT_TREE* srcNodeList)
{
	//TODO : RBT Tree dealloc
}

void HT_Chaining_RBT_DispNodeList(CHAINING_RBT_HASH_TABLE* srcHashTable)
{
	//TODO
}

void HT_Chaining_RBT_DispIndexListBy(CHAINING_RBT_HASH_TABLE* srcHashTable, NODE_STATE targetNodeState)
{
	//TODO
}

void HT_Chaining_RBT_InsertData(CHAINING_RBT_HASH_TABLE* srcHashTable, HT_KEY_TYPE srcKey, HT_DATA_TYPE srcData)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (srcKey == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	HASH_INDEX_TYPE hashIndex = HT_Common_DigitFolding_Hash(srcHashTable->_capacity, srcKey);
	CHAINING_RBT_NODE* newNode = NULL;

	if (srcHashTable->_table[hashIndex] == NULL) //충돌이 발생하지 않았을 경우
	{
		newNode = HT_Chaining_RBT_CreateNode(srcKey, srcData);
		srcHashTable->_table[hashIndex] = newNode;
	}
	else //충돌이 발생했을 경우
	{
#ifdef HT_DEBUG_MODE
		HT_Common_IncreaseHashCollisionCount(srcKey);
#endif

		//TODO : balance tree
	}

END_PROC:
	return;
}

HT_DATA_TYPE HT_Chaining_RBT_SearchData(CHAINING_RBT_HASH_TABLE* srcHashTable, HT_KEY_TYPE targetKey)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (targetKey == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	HASH_INDEX_TYPE hashIndex = HT_Common_DigitFolding_Hash(srcHashTable->_capacity, targetKey);
	CHAINING_RBT_TREE currentNode = srcHashTable->_table[hashIndex];

//TODO : tree search

	throw myexception::NOT_FOUND_EXCEPTION(std::string(__func__) + std::string(" : Not found"));
}

void HT_Chaining_RBT_RemoveData(CHAINING_RBT_HASH_TABLE* srcHashTable, HT_KEY_TYPE targetKey, bool deallocateAfterRemove)
{
	if (srcHashTable == NULL)
		throw std::runtime_error(std::string(__func__) + std::string(" : Not initialized"));

	if (targetKey == NULL)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	HASH_INDEX_TYPE hashIndex = HT_Common_DigitFolding_Hash(srcHashTable->_capacity, targetKey);
	
	//TODO : balance tree
}
