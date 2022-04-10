#include "HT_Core.h"

#define TABLE_INIT_SIZE 1233 //초기 테이블 크기
#define MAX_STR_LEN 256

const bool TEST_CHAINING_HASH_TABLE = false;
const bool TEST_RAND_PATTERN_DATA = true;

const bool DISP_NODE_LIST = false;
const bool DISP_OCCUPIED_INDEX_LIST = true;
const bool DISP_EMPTY_INDEX_LIST = true;

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
		CHAINING_HASH_TABLE* hashTable = HT_Chaining_CreateHashTable(TABLE_INIT_SIZE);

		if (TEST_CHAINING_HASH_TABLE)
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
				{"YZ", "yzyzz"}, //ascii sum 179
				{"Od", "crash"} //ascii sum 179
			}; //입력 데이터 (키, 값)

			for (int i = 0; i < LENGTH(inputData); i++)
			{
				HT_Chaining_InsertData(hashTable, (HT_KEY_TYPE)inputData[i][0], (HT_DATA_TYPE)inputData[i][1]);
			}

			for (int i = 0; i < LENGTH(inputData); i++)
			{
				std::cout << "Key Search : " << inputData[i][0] << "\n";
				std::cout << "Index [" << HT_Common_DigitFolding_Hash(TABLE_INIT_SIZE, (HT_KEY_TYPE)inputData[i][0]) << "] : "
					<< HT_Chaining_SearchData(hashTable, (HT_KEY_TYPE)inputData[i][0]) << "\n";
			}

			for (int i = 0; i < LENGTH(inputData); i++)
			{
				if (i == 1) //삭제 된 키
					continue;

				std::cout << "Remove : " << inputData[i][0] << "\n";
				HT_Chaining_RemoveData(hashTable, (HT_KEY_TYPE)inputData[i][0]);
			}
		}

		if (TEST_RAND_PATTERN_DATA)
		{
			const int RAND_KEY_COUNT = 900; //임의 길이의 키 개수
			const int MIN_KEY_LENGTH = 5; //최소 키 길이
			const int MAX_KEY_LENGTH = 30; //최대 키 길이

			char** randKeyList = new char* [RAND_KEY_COUNT]; //임의 길이의 키 목록

			for (int i = 0; i < RAND_KEY_COUNT; i++)
			{
				randKeyList[i] = new char[MAX_STR_LEN];
				
				utils::GenRandStr(randKeyList[i], MAX_STR_LEN, utils::GenUnsignedRandNum(MIN_KEY_LENGTH, MAX_KEY_LENGTH));
				//std::cout << randKeyList[i]<<"\n";

				HT_Chaining_InsertData(hashTable,
					(HT_KEY_TYPE)randKeyList[i],
					(HT_DATA_TYPE)"foo data");
			}

			if (DISP_NODE_LIST)
			{
				HT_Chaining_DispNodeList(hashTable);
			}

			if (DISP_OCCUPIED_INDEX_LIST)
			{
				std::cout << "--- Occupied Index List ---\n";
				HT_Chaining_DispOccupiedIndexList(hashTable);
				std::cout << "---------------------------\n";
			}

			if (DISP_EMPTY_INDEX_LIST)
			{
				std::cout << "--- Empty Index List ---\n";
				HT_Chaining_DispEmptyIndexList(hashTable);
				std::cout << "---------------------------\n";
			}

			for (int i = 0; i < RAND_KEY_COUNT; i++)
				delete randKeyList[i];
			delete[] randKeyList;
		}

#ifdef HT_DEBUG_MODE
		std::cout << "총 해시 충돌 발생 횟수 : " << HT_Common_GetHashCollisionCount() << "\n";
#endif

		HT_Chaining_DeallocateHashTable(&hashTable);

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