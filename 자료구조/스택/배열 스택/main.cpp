#include "AS_Core.h"

#define STACK_SIZE 3

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����

	try
	{
		ArrayStack* stack = NULL; //�迭 ����
		AS_CreateStack(&stack, STACK_SIZE); //STACK_SIZE��ŭ ���� ���� �� ũ�� �Ҵ�

		for (int i = 0; i < STACK_SIZE; i++)
		{
			AS_Push(&stack, i);
		}

		StackIndexType count = AS_GetTotalNodeDataCount(&stack);
		for (int i = 0; i < count; i++)
		{
			std::cout << "-------------------------------------------\n";
			std::cout << "--- Before Pop ---\n";
			std::cout << "Peek Data : " << AS_Peek(&stack) << std::endl;
			std::cout << "Top Index : " << stack->_top << std::endl;
			std::cout << "IsEmpty : " << (AS_IsEmpty(&stack) ? "true" : "false") << std::endl;
			std::cout << "IsFull : " << (AS_IsFull(&stack) ? "true" : "false") << std::endl;
			std::cout << "��ü ��� ������ ���� : " << AS_GetTotalNodeDataCount(&stack) << std::endl;
			std::cout << "--- After Pop ---\n";
			std::cout << "Pop Data : " << AS_Pop(&stack) << std::endl;
			std::cout << "Top Index : " << stack->_top << std::endl;
			std::cout << "IsEmpty : " << (AS_IsEmpty(&stack) ? "true" : "false") << std::endl;
			std::cout << "IsFull : " << (AS_IsFull(&stack) ? "true" : "false") << std::endl;
			std::cout << "��ü ��� ������ ���� : " << AS_GetTotalNodeDataCount(&stack) << std::endl;
			std::cout << "-------------------------------------------\n";
		}

		std::cout << "--- realloc test ---\n";
		int reallocTestSize = STACK_SIZE * 2;
		for (int i = 0; i < reallocTestSize; i++) //�ʱ� �Ҵ� ũ�⸦ �ʰ��Ͽ� ������ ����
		{
			AS_Push(&stack, i);
			std::cout << "Current Capacity : " << stack->_capacity << std::endl;
		}
		for (int i = 0; i < reallocTestSize; i++)
		{
			AS_Pop(&stack);
			std::cout << "Current Capacity : " << stack->_capacity << std::endl;
		}

		AS_DeallocateArrayStack(&stack);
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