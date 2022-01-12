#include "AS_Core.h"

#define STACK_SIZE 3

int main()
{
	ArrayStack* stack = NULL; //�迭 ����

	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����

	try
	{
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
			std::cout << "Top Index : " << stack->top << std::endl;
			std::cout << "IsEmpty : " << (AS_IsEmpty(&stack) ? "true" : "false") << std::endl;
			std::cout << "IsFull : " << (AS_IsFull(&stack) ? "true" : "false") << std::endl;
			std::cout << "��ü ��� ������ ���� : " << AS_GetTotalNodeDataCount(&stack) << std::endl;
			std::cout << "--- After Pop ---\n";
			std::cout << "Pop Data : " << AS_Pop(&stack) << std::endl;
			std::cout << "Top Index : " << stack->top << std::endl;
			std::cout << "IsEmpty : " << (AS_IsEmpty(&stack) ? "true" : "false") << std::endl;
			std::cout << "IsFull : " << (AS_IsFull(&stack) ? "true" : "false") << std::endl;
			std::cout << "��ü ��� ������ ���� : " << AS_GetTotalNodeDataCount(&stack) << std::endl;
			std::cout << "-------------------------------------------\n";
		}

		AS_DeallocateArrayStack(&stack);
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		exit(EXIT_FAILURE);
	}

<<<<<<< HEAD
=======
	AS_DeallocateArrayStack(&stack);

>>>>>>> d7a7bc6e75e62cce5e2f1f506204e1aca326980a
	_CrtMemCheckpoint(&newState); //�Ҵ� ���� �� ����
	_CrtDumpMemoryLeaks();
	if (_CrtMemDifference(&lastState, &oldState, &newState))
		_CrtMemDumpStatistics(&lastState);

	system("pause");
	return EXIT_SUCCESS;
}