#include "LLQ_Core.h"

#define COUNT 5

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����

	/***
		< ��ȯ ť�� ���� ����Ʈ ť�� ���� �� >

		1) ��ȯ ť
		: ť�� ũ�⸦ �̸� ���� ���� �� ���, ���ο� ������ ���� �ø���
		������ Heap �޸� �Ҵ翡 ���� ������尡 �����Ƿ�, ���� ����Ʈ ť���� �ð������� �켼
		�׷���, ť�� ũ�⸦ ���� ������� ���� ���, ����Ǵ� �޸𸮸� ���̱� ���ؼ�
		���� �Ҵ� ũ�⿡ ���� ���Ұ� �߻� �� �� ���� �Ӱ� ���� (�迭 ���ÿ����� ��������)�� ����
		���������� �Ҵ� ũ�⸦ �����ϸ�, ���� ����Ʈ ť�� ����ϴ� �Ͱ� ���Ͽ� �켼�� ���� ���� ���̴�.

		2) ���� ����Ʈ ť
		: ť�� ũ�⸦ �̸� ���� �� �� ���� ���, ���������� ��ȯ ť���� �켼�ϳ�,
		���ο� ������ ���� �ø��� ������ Heap �޸� �Ҵ翡 ���� ������忡 ���� �ð������� �Ҹ�
	***/

	try
	{
		LinkedListQueue* queue = NULL; //���� ����Ʈ ť
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
		std::cout << "��ü ��� �� : " << LLQ_GetTotalNodeCount(&queue) << std::endl;
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
		std::cout << "��ü ��� �� : " << LLQ_GetTotalNodeCount(&queue) << std::endl;

		LLQ_DeallocateQueue(&queue);
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	_CrtMemCheckpoint(&newState); //�Ҵ� ���� �� ����
	_CrtDumpMemoryLeaks();
	if (_CrtMemDifference(&lastState, &oldState, &newState))
		_CrtMemDumpStatistics(&lastState);

	system("pause");
	return EXIT_SUCCESS;
}