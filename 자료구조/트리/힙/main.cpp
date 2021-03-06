#include "HEAP_Core.h"

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
		int inputData[] =
		{
			30,18,33,60,100,19,30,5,7
		}; //입력 데이터

		HEAP* heap = HEAP_CreateHeap(1);

		for (int i = 0; i < LENGTH(inputData); i++)
		{
			std::cout << "Push : " << inputData[i] << std::endl;
			HEAP_Push(heap, inputData[i]);
			HEAP_DispTotalNode(heap);
			std::cout << "\n";
		}

		for (int i = 0; i < LENGTH(inputData); i++)
		{
			std::cout << "Pop : " << HEAP_Pop(heap) << std::endl;
			HEAP_DispTotalNode(heap);
			std::cout << "\n";
		}

		HEAP_DeallocateHeap(&heap);
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