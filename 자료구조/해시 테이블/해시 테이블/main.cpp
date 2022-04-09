#include "HT_Core.h"

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
		const char* inputData[][2] =
		{
			{"abcd", "foo data1"},
			{"abcd", "duplicate key test (overwrite)"},
			{"dbca", "foo data2"},
			{"foo key", "foo data3"},
			{"bar key", "foo data4"}
		}; //입력 데이터 (키, 값)

		CHAINING_HASH_TABLE* hashTable = HT_Chaining_CreateHashTable(123456);

		for (int i = 0; i < LENGTH(inputData); i++)
		{
			HT_Chaining_InsertData(hashTable, (HT_KEY_TYPE)inputData[i][0], (HT_DATA_TYPE)inputData[i][1]);
		}

		for (int i = 0; i < LENGTH(inputData); i++)
		{
			std::cout << "---\n";
			std::cout << "Search : " << inputData[i][0] <<"\n";
			std::cout << "Hash Index : " << HT_Common_DigitFolding_Hash(123456, (HT_KEY_TYPE)inputData[i][0]) <<"\n";
			std::cout << "Data : " << HT_Chaining_SearchData(hashTable, (HT_KEY_TYPE)inputData[i][0]) <<"\n";
			std::cout << "---\n";
		}

		for (int i = 0; i < LENGTH(inputData); i++)
		{
			if (i == 1) //삭제 된 키
				continue;

			std::cout << "Remove : " << inputData[i][0] << "\n";
			HT_Chaining_RemoveData(hashTable, (HT_KEY_TYPE)inputData[i][0]);
		}


		HT_Chaining_DeallocateHashTable(&hashTable);

		char DUMMY_KEY[5][10] =
		{
			{NULL},
		};
		const char* DUMMY_DATA = "dummy";
		for (int i = 0; i < LENGTH(DUMMY_KEY); i++)
		{
			
			utils::GenRandStr(DUMMY_KEY[i], LENGTH(DUMMY_KEY[i]), 9);
			std::cout << DUMMY_KEY[i] << "\n";
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