#include "CQ_Core.h"

#define SIZE 5

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
		CircularQueue* queue = NULL; //순환 큐
		CQ_CreateQueue(&queue, SIZE);

		for (int i = 0; i < SIZE; i++)
		{
			std::cout << "Enqueue data : " << i <<
				", Front : " << queue->_front << ", Rear : " << queue->_rear << "\n";
			CQ_Enqueue(&queue, i);
		}
		std::cout << "전체 노드 데이터 수 : " << CQ_GetTotalNodeDataCount(&queue) << std::endl;
		std::cout << "---\n";

		while (!CQ_IsEmpty(&queue))
		{
			std::cout << "Dequeue data : " << CQ_Dequeue(&queue) <<
				", Front : " << queue->_front << ", Rear : " << queue->_rear << "\n";
		}
		std::cout << "전체 노드 데이터 수 : " << CQ_GetTotalNodeDataCount(&queue) << std::endl;
		std::cout << "---\n";

		CQ_Enqueue(&queue, 1234);
		std::cout << "Enqueue data : " << 1234 <<
			", Front : " << queue->_front << ", Rear : " << queue->_rear << "\n";

		CQ_DeallocateQueue(&queue);
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