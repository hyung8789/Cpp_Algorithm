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
		};

		CHAINING_HASH_TABLE* hashTable = HT_Chaining_CreateHashTable(123456);

		for (int i = 0; i < LENGTH(inputData); i++)
		{
			HT_Chaining_InsertData(hashTable, (KEY_TYPE)inputData[i][0], (DATA_TYPE)inputData[i][1]);
		}

		for (int i = 0; i < LENGTH(inputData); i++)
		{
			std::cout << "Searching : " << inputData[i][0] <<"\n";

			//CHAINING_NODE* result = HT_Chaining_SearchData(hashTable, (KEY_TYPE)inputData[i][0]);
			//std::cout << "Key : " << result->_key << ", Data : " << result->_data <<"\n";
		}

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