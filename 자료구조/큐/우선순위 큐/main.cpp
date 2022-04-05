#include "PQ_Core.h"

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
		NODE inputData[] =
		{
			{0, (void*)"숨쉬기"},
			{1, (void*)"잠자기"},
			{2, (void*)"foo"},
			{3, (void*)"bar"},
			{4, (void*)"test"},
			{5, (void*)"abcd"}
		}; //입력 데이터
		
		PRIORITY_QUEUE* pq = PQ_CreateQueue(1);

		std::cout << "--- Enqueue ---\n";
		for (int i = 0; i < LENGTH(inputData); i++)
		{
			PQ_Enqueue(pq, &inputData[i]);
			const NODE& peekedNode = PQ_Peek(pq);
			std::cout << "Peek - Priority : " << peekedNode._priority << 
				", Data : " << (const char*)peekedNode._data <<"\n";
		}

		std::cout << "--- Dequeue ---\n";
		while (!PQ_IsEmpty(pq))
		{
			const NODE& peekedNode = PQ_Peek(pq);
			std::cout << "Peek - Priority : " << peekedNode._priority << ", Data : " << (const char*)peekedNode._data << "\n";

			NODE result;
			PQ_Dequeue(pq, &result);
			std::cout << "Dequeue - Priority : " << result._priority << ", Data : " << (const char*)result._data <<"\n";
		}
	
		PQ_DeallocateQueue(&pq);
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