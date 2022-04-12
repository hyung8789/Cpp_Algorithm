#include "HT_Core.h"

#define TABLE_INIT_SIZE 12345 //초기 테이블 크기
#define MAX_STR_LEN 256

const bool TEST_STATIC_KEY = false;
const bool TEST_RAND_KEY = true;

const int RAND_KEY_COUNT = 12000; //임의 길이의 키 개수
const int MIN_KEY_LENGTH = 5; //최소 키 길이
const int MAX_KEY_LENGTH = 10; //최대 키 길이

const bool TEST_CHAINING_HASH_TABLE = false;
const bool TEST_OPEN_ADDR_HASH_TABLE = true;

const bool DISP_NODE_LIST = false;
const bool DISP_INDEX_LIST = true;
const NODE_STATE DISP_INDEX_LIST_BY = NODE_STATE::OCCUPIED;

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
		CHAINING_HASH_TABLE* chainingHashTable = HT_Chaining_CreateHashTable(TABLE_INIT_SIZE);
		OPEN_ADDR_HASH_TABLE* openAddrHashTable = HT_OpenAddr_CreateHashTable(TABLE_INIT_SIZE);

		if (TEST_STATIC_KEY)
		{
			const char* inputData[][2] =
			{
				{"abcd", "foo data1"},
				{"abcd", "duplicate key test (overwrite)"},
				{"dbca", "foo data2"},
				{"foo key", "foo data3"},
				{"bar key", "foo data4"},
				{"a", "aaa"},
				{"b", "bbb"},
				{"ab", "ababab"},
				{"YZ", "YZ : ascii sum 179"}, //ascii sum 179
				{"Od", "Od : ascii sum 179"} //ascii sum 179
			}; //입력 데이터 (키, 값)

			for (int i = 0; i < LENGTH(inputData); i++)
			{
				if (TEST_CHAINING_HASH_TABLE)
					HT_Chaining_InsertData(chainingHashTable, (HT_KEY_TYPE)inputData[i][0], (HT_DATA_TYPE)inputData[i][1]);

				if (TEST_OPEN_ADDR_HASH_TABLE)
					HT_OpenAddr_InsertData(&openAddrHashTable, (HT_KEY_TYPE)inputData[i][0], (HT_DATA_TYPE)inputData[i][1]);
			}

			for (int i = 0; i < LENGTH(inputData); i++)
			{
				std::cout << "Key Search : " << inputData[i][0] << "\n";

				if (TEST_CHAINING_HASH_TABLE)
					std::cout << HT_Chaining_SearchData(chainingHashTable, (HT_KEY_TYPE)inputData[i][0]) << "\n";

				if (TEST_OPEN_ADDR_HASH_TABLE)
					std::cout << HT_OpenAddr_SearchData(openAddrHashTable, (HT_KEY_TYPE)inputData[i][0]) << "\n";
			}

			for (int i = 0; i < LENGTH(inputData); i++)
			{
				if (i == 1) //삭제 된 키
					continue;

				std::cout << "Remove : " << inputData[i][0] << "\n";

				if (TEST_CHAINING_HASH_TABLE)
					HT_Chaining_RemoveData(chainingHashTable, (HT_KEY_TYPE)inputData[i][0]);

				if (TEST_OPEN_ADDR_HASH_TABLE)
					HT_OpenAddr_RemoveData(openAddrHashTable, (HT_KEY_TYPE)inputData[i][0]);
			}
		}

		if (TEST_RAND_KEY)
		{
			char** randKeyList = new char* [RAND_KEY_COUNT]; //임의 길이의 키 목록

			for (int i = 0; i < RAND_KEY_COUNT; i++)
			{
				randKeyList[i] = new char[MAX_STR_LEN];

				utils::GenRandStr(randKeyList[i], MAX_STR_LEN, utils::GenUnsignedRandNum(MIN_KEY_LENGTH, MAX_KEY_LENGTH));
				//std::cout << randKeyList[i]<<"\n";
				if (TEST_CHAINING_HASH_TABLE)
					HT_Chaining_InsertData(chainingHashTable,
						(HT_KEY_TYPE)randKeyList[i],
						(HT_DATA_TYPE)"foo data");

				if (TEST_OPEN_ADDR_HASH_TABLE)
					HT_OpenAddr_InsertData(&openAddrHashTable,
						(HT_KEY_TYPE)randKeyList[i],
						(HT_DATA_TYPE)"foo data");
			}

			if (DISP_NODE_LIST)
			{
				if (TEST_CHAINING_HASH_TABLE)
					HT_Common_DispNodeList(chainingHashTable, HASH_TABLE_COLLISION_HANDLE_METHOD::CHAINING);

				if (TEST_OPEN_ADDR_HASH_TABLE)
					HT_Common_DispNodeList(openAddrHashTable, HASH_TABLE_COLLISION_HANDLE_METHOD::OPEN_ADDR);
			}

			if (DISP_INDEX_LIST)
			{
				if (TEST_CHAINING_HASH_TABLE)
				{
					std::cout << "--- Chaining Index List ---\n";
					HT_Common_DispIndexListBy(chainingHashTable, HASH_TABLE_COLLISION_HANDLE_METHOD::CHAINING,
						DISP_INDEX_LIST_BY);
					std::cout << "---------------------------\n";
				}

				if (TEST_OPEN_ADDR_HASH_TABLE)
				{
					std::cout << "--- OpenAddr Index List ---\n";
					HT_Common_DispIndexListBy(openAddrHashTable, HASH_TABLE_COLLISION_HANDLE_METHOD::OPEN_ADDR,
						DISP_INDEX_LIST_BY);
					std::cout << "---------------------------\n";
				}
			}
	
			for (int i = 0; i < RAND_KEY_COUNT; i++)
				delete randKeyList[i];
			delete[] randKeyList;
		}

#ifdef HT_DEBUG_MODE
		std::cout << "총 해시 충돌 발생 횟수 : " << HT_Common_GetHashCollisionCount() << "\n";
#endif

		HT_Chaining_DeallocateHashTable(&chainingHashTable);
		HT_OpenAddr_DeallocateHashTable(&openAddrHashTable);
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	_CrtMemCheckpoint(&newState); //할당 해제 후 상태
	_CrtDumpMemoryLeaks();
	if (_CrtMemDifference(&lastState, &oldState, &newState))
		_CrtMemDumpStatistics(&lastState);

	system("pause");
	return EXIT_SUCCESS;
}