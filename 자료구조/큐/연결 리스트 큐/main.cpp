#include "LLQ_Core.h"

#define COUNT 5

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	/***
		< 순환 큐와 연결 리스트 큐의 성능 비교 >

		1) 순환 큐
		: 큐의 크기를 미리 예측 가능 할 경우, 새로운 데이터 삽입 시마다
		가변적 Heap 메모리 할당에 따른 오버헤드가 없으므로, 연결 리스트 큐보다 시간적으로 우세
		그러나, 큐의 크기를 전부 사용하지 않을 경우, 낭비되는 메모리를 줄이기 위해서
		기존 할당 크기에 대한 감소가 발생 될 빈 공간 임계 비율 (배열 스택에서의 구현사항)에 따라
		가변적으로 할당 크기를 조절하면, 연결 리스트 큐를 사용하는 것과 비교하여 우세한 점이 없을 것이다.

		2) 연결 리스트 큐
		: 큐의 크기를 미리 예측 할 수 없는 경우, 공간적으로 순환 큐보다 우세하나,
		새로운 데이터 삽입 시마다 가변적 Heap 메모리 할당에 따른 오버헤드에 의해 시간적으로 불리
	***/

	try
	{
		LinkedListQueue* queue = NULL; //연결 리스트 큐
		LLQ_CreateQueue(&queue);

		for (int i = 0; i < COUNT; i++)
		{
			LLQ_Enqueue(&queue, LLQ_CreateNode(i));

			std::cout << "Enqueue data : " << i;
			if (queue->_front != NULL)
				std::cout << ", Front data : " << queue->_front->_data;
			if (queue->_rear != NULL)
				std::cout << ", Rear data : " << queue->_rear->_data;
			std::cout << "\n";
		}
		std::cout << "전체 노드 수 : " << LLQ_GetTotalNodeCount(&queue) << std::endl;
		std::cout << "---\n";

		while (!LLQ_IsEmpty(&queue))
		{
			Node* tmp = LLQ_Dequeue(&queue);

			std::cout << "Dequeue data : " << tmp->_data;
			if (queue->_front != NULL)
				std::cout << ", Front data : " << queue->_front->_data;
			if(queue->_rear != NULL)
				std::cout << ", Rear data : " << queue->_rear->_data;
			std::cout << "\n";

			LLQ_DeallocateNode(&tmp);
		}
		std::cout << "전체 노드 수 : " << LLQ_GetTotalNodeCount(&queue) << std::endl;

		LLQ_DeallocateQueue(&queue);
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