#include "HT_Core.h"

#define TABLE_INIT_SIZE 111 //초기 테이블 크기
#define MAX_STR_LEN 256

const bool TEST_CHAINING_HASH_TABLE = false;
const bool TEST_OPENADDR_HASH_TABLE = false;
const bool TEST_CHAINING_RBT_HASH_TABLE = true;

const bool TEST_STATIC_KEY = false;
const bool TEST_RAND_KEY = true;
const int RAND_KEY_COUNT = 2000; //임의 길이의 키 개수
const int MIN_KEY_LENGTH = 5; //최소 키 길이
const int MAX_KEY_LENGTH = 10; //최대 키 길이

const bool DISP_NODE_LIST = true;
const bool DISP_INDEX_LIST = false;
const NODE_STATE DISP_CHAINING_INDEX_LIST_BY = NODE_STATE::OCCUPIED;
const NODE_STATE DISP_OPENADDR_INDEX_LIST_BY = NODE_STATE::OCCUPIED;
const NODE_STATE DISP_CHAINING_RBT_INDEX_LIST_BY = NODE_STATE::OCCUPIED;

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태
	//_CrtSetBreakAlloc(245);

	try
	{
		const char* staticInputKeyData[][2] =
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
		}; //정적 입력 데이터 (키, 값)

		char** randKeyList = new char* [RAND_KEY_COUNT]; //임의 길이의 키 목록
		const char* fooDataForRandKey = "foo data12312312";

		if (TEST_RAND_KEY)
		{
			for (int i = 0; i < RAND_KEY_COUNT; i++)
			{
				randKeyList[i] = new char[MAX_STR_LEN];

				utils::GenRandStr(randKeyList[i], MAX_STR_LEN, utils::GenUnsignedRandNum(MIN_KEY_LENGTH, MAX_KEY_LENGTH));
				//std::cout << randKeyList[i]<<"\n";
			}
		}

		if (TEST_CHAINING_HASH_TABLE)
		{
			std::cout << "=================== Chaining Hash Table ===================\n";
			CHAINING_HASH_TABLE* chainingHashTable = HT_Chaining_CreateHashTable(TABLE_INIT_SIZE);

			if (TEST_STATIC_KEY)
			{
				for (int i = 0; i < LENGTH(staticInputKeyData); i++)
				{
					HT_Chaining_InsertData(chainingHashTable,
						(HT_KEY_TYPE)staticInputKeyData[i][0],
						(HT_DATA_TYPE)staticInputKeyData[i][1]);
				}

				for (int i = 0; i < LENGTH(staticInputKeyData); i++)
				{
					std::cout << "- Key Search : " << staticInputKeyData[i][0] << "\n";
					std::cout << HT_Chaining_SearchData(chainingHashTable,
						(HT_KEY_TYPE)staticInputKeyData[i][0]) << "\n";
				}

				if (DISP_NODE_LIST)
				{
					HT_Chaining_DispNodeList(chainingHashTable);
				}

				if (DISP_INDEX_LIST)
				{
					HT_Chaining_DispIndexListBy(chainingHashTable, DISP_CHAINING_INDEX_LIST_BY);
				}

				for (int i = 0; i < LENGTH(staticInputKeyData); i++)
				{
					try
					{
						std::cout << "- Remove : " << staticInputKeyData[i][0] << "\n";
						HT_Chaining_RemoveData(chainingHashTable,
							(HT_KEY_TYPE)staticInputKeyData[i][0]);
					}
					catch (const myexception::NOT_FOUND_EXCEPTION& ex)
					{
						std::cout << ex.what() << std::endl;
						system("pause");
						continue;
					}
				}
			}

			if (TEST_RAND_KEY)
			{
				for (int i = 0; i < RAND_KEY_COUNT; i++)
				{
					HT_Chaining_InsertData(chainingHashTable,
						(HT_KEY_TYPE)randKeyList[i],
						(HT_DATA_TYPE)fooDataForRandKey);
				}

				if (DISP_NODE_LIST)
				{
					HT_Chaining_DispNodeList(chainingHashTable);
				}

				for (int i = 0; i < RAND_KEY_COUNT; i++)
				{
					try
					{
						std::cout << "- Remove : " << randKeyList[i] << "\n";
						HT_Chaining_RemoveData(chainingHashTable,
							(HT_KEY_TYPE)randKeyList[i]);
					}
					catch (const myexception::NOT_FOUND_EXCEPTION& ex)
					{
						std::cout << ex.what() << std::endl;
						system("pause");
						continue;
					}
				}
			}

			HT_Chaining_DeallocateHashTable(&chainingHashTable);

#ifdef HT_DEBUG_MODE
			std::cout << "총 해시 충돌 발생 횟수 : " << HT_Common_GetHashCollisionCount() << "\n";
			HT_Common_InitHashCollisionCount();
#endif
		}

		std::cout << "\n";

		if (TEST_OPENADDR_HASH_TABLE)
		{
			std::cout << "=================== OpenAddr Hash Table ===================\n";
			OPENADDR_HASH_TABLE* openAddrHashTable = HT_OpenAddr_CreateHashTable(TABLE_INIT_SIZE);

			if (TEST_STATIC_KEY)
			{
				for (int i = 0; i < LENGTH(staticInputKeyData); i++)
				{
					HT_OpenAddr_InsertData(&openAddrHashTable,
						(HT_KEY_TYPE)staticInputKeyData[i][0],
						(HT_DATA_TYPE)staticInputKeyData[i][1]);
				}

				for (int i = 0; i < LENGTH(staticInputKeyData); i++)
				{
					std::cout << "- Key Search : " << staticInputKeyData[i][0] << "\n";
					std::cout << HT_OpenAddr_SearchData(openAddrHashTable,
						(HT_KEY_TYPE)staticInputKeyData[i][0]) << "\n";
				}

				if (DISP_NODE_LIST)
				{
					HT_OpenAddr_DispNodeList(openAddrHashTable);
				}

				if (DISP_INDEX_LIST)
				{
					HT_OpenAddr_DispIndexListBy(openAddrHashTable, DISP_OPENADDR_INDEX_LIST_BY);
				}

				for (int i = 0; i < LENGTH(staticInputKeyData); i++)
				{
					try
					{
						std::cout << "- Remove : " << staticInputKeyData[i][0] << "\n";
						HT_OpenAddr_RemoveData(openAddrHashTable,
							(HT_KEY_TYPE)staticInputKeyData[i][0]);
					}
					catch (const myexception::NOT_FOUND_EXCEPTION& ex)
					{
						std::cout << ex.what() << std::endl;
						system("pause");
						continue;
					}
				}
			}

			if (TEST_RAND_KEY)
			{
				for (int i = 0; i < RAND_KEY_COUNT; i++)
				{
					HT_OpenAddr_InsertData(&openAddrHashTable,
						(HT_KEY_TYPE)randKeyList[i],
						(HT_DATA_TYPE)fooDataForRandKey);
				}

				if (DISP_NODE_LIST)
				{
					HT_OpenAddr_DispNodeList(openAddrHashTable);
				}

				for (int i = 0; i < RAND_KEY_COUNT; i++)
				{
					try
					{
						std::cout << "- Remove : " << randKeyList[i] << "\n";
						HT_OpenAddr_RemoveData(openAddrHashTable,
							(HT_KEY_TYPE)randKeyList[i]);
					}
					catch (const myexception::NOT_FOUND_EXCEPTION& ex)
					{
						std::cout << ex.what() << std::endl;
						system("pause");
						continue;
					}
				}
			}

			HT_OpenAddr_DeallocateHashTable(&openAddrHashTable);
#ifdef HT_DEBUG_MODE
			std::cout << "총 해시 충돌 발생 횟수 : " << HT_Common_GetHashCollisionCount() << "\n";
			HT_Common_InitHashCollisionCount();
#endif
		}

		std::cout << "\n";

		if (TEST_CHAINING_RBT_HASH_TABLE)
		{
			std::cout << "=================== Chaining RBT Hash Table ===================\n";
			CHAINING_RBT_HASH_TABLE* chainingRBTHashTable = HT_Chaining_RBT_CreateHashTable(TABLE_INIT_SIZE);

			if (TEST_STATIC_KEY)
			{
				for (int i = 0; i < LENGTH(staticInputKeyData); i++)
				{
					HT_Chaining_RBT_InsertData(chainingRBTHashTable,
						(HT_KEY_TYPE)staticInputKeyData[i][0],
						(HT_DATA_TYPE)staticInputKeyData[i][1]);
				}

				for (int i = 0; i < LENGTH(staticInputKeyData); i++)
				{
					std::cout << "- Key Search : " << staticInputKeyData[i][0] << "\n";
					std::cout << HT_Chaining_RBT_SearchData(chainingRBTHashTable,
						(HT_KEY_TYPE)staticInputKeyData[i][0]) << "\n";
				}

				if (DISP_NODE_LIST)
				{
					HT_Chaining_RBT_DispNodeList(chainingRBTHashTable);
				}

				if (DISP_INDEX_LIST)
				{
					HT_Chaining_RBT_DispIndexListBy(chainingRBTHashTable, DISP_CHAINING_INDEX_LIST_BY);
				}

				for (int i = 0; i < LENGTH(staticInputKeyData); i++)
				{
					try
					{
						std::cout << "- Remove : " << staticInputKeyData[i][0] << "\n";
						HT_Chaining_RBT_RemoveData(chainingRBTHashTable,
							(HT_KEY_TYPE)staticInputKeyData[i][0]);
					}
					catch (const myexception::NOT_FOUND_EXCEPTION& ex)
					{
						std::cout << ex.what() << std::endl;
						system("pause");
						continue;
					}
				}
			}

			if (TEST_RAND_KEY)
			{
				for (int i = 0; i < RAND_KEY_COUNT; i++)
				{
					HT_Chaining_RBT_InsertData(chainingRBTHashTable,
						(HT_KEY_TYPE)randKeyList[i],
						(HT_DATA_TYPE)fooDataForRandKey);
				}

				if (DISP_NODE_LIST)
				{
					HT_Chaining_RBT_DispNodeList(chainingRBTHashTable);
				}

				for (int i = 0; i < RAND_KEY_COUNT; i++)
				{
					try
					{
						std::cout << "- Remove : " << randKeyList[i] << "\n";
						HT_Chaining_RBT_RemoveData(chainingRBTHashTable,
							(HT_KEY_TYPE)randKeyList[i]);
					}
					catch (const myexception::NOT_FOUND_EXCEPTION& ex)
					{
						std::cout << ex.what() << std::endl;
						system("pause");
						continue;
					}
				}
			}

			HT_Chaining_RBT_DeallocateHashTable(&chainingRBTHashTable);
#ifdef COLOR_VISUALIZATION
			CONSOLE_SCREEN_MANAGER::GetInstance().Dispose();
#endif
#ifdef HT_DEBUG_MODE
			std::cout << "총 해시 충돌 발생 횟수 : " << HT_Common_GetHashCollisionCount() << "\n";
			HT_Common_InitHashCollisionCount();
#endif
		}

		std::cout << "\n";

		if (TEST_RAND_KEY)
		{
			for (int i = 0; i < RAND_KEY_COUNT; i++)
				delete randKeyList[i];
			delete[] randKeyList;
		}
		else
		{
			delete[] randKeyList;
		}
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