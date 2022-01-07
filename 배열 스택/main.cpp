#include "AS_Core.h"

#define STACK_SIZE 3

int main()
{
	ArrayStack* stack = NULL; //�迭 ����

	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����

	AS_CreateStack(&stack, STACK_SIZE); //STACK_SIZE��ŭ ���� ���� �� ũ�� �Ҵ�

	for (int i = 0; i < STACK_SIZE; i++)
	{
		AS_Push(&stack, i);
	}
	for (int i = 0; i < STACK_SIZE; i++)
	{
		std::cout << "-------------------------------------------\n";
		std::cout << "��ü ��� ������ ���� : " << AS_GetTotalNodeDataCount(&stack) << std::endl;
		std::cout << "IsEmpty : " << (AS_IsEmpty(&stack) ? "true" : "false") << std::endl;
		std::cout << "IsFull : " << (AS_IsFull(&stack) ? "true" : "false") << std::endl;
		std::cout << "Top Index : " << stack->top << std::endl;
		std::cout << "Peek Data : " << AS_Peek(&stack) << std::endl;
		std::cout << "Pop Data : " << AS_Pop(&stack) << std::endl;
		std::cout << "-------------------------------------------\n";
	}

	AS_CreateStack(&stack, STACK_SIZE * 2); //STACK_SIZE�� 2�� ũ��� �� �Ҵ�

	for (int i = 0; i < STACK_SIZE * 2; i++)
	{
		AS_Push(&stack, i);
	}
	for (int i = 0; i < STACK_SIZE * 2; i++)
	{
		std::cout << "-------------------------------------------\n";
		std::cout << "��ü ��� ������ ���� : " << AS_GetTotalNodeDataCount(&stack) << std::endl;
		std::cout << "IsEmpty : " << (AS_IsEmpty(&stack) ? "true" : "false") << std::endl;
		std::cout << "IsFull : " << (AS_IsFull(&stack) ? "true" : "false") << std::endl;
		std::cout << "Top Index : " << stack->top << std::endl;
		std::cout << "Peek Data : " << AS_Peek(&stack) << std::endl;
		std::cout << "Pop Data : " << AS_Pop(&stack) << std::endl;
		std::cout << "-------------------------------------------\n";
	}

	AS_DeallocateArrayStack(&stack);

	_CrtMemCheckpoint(&newState); //�Ҵ� ���� �� ����
	_CrtDumpMemoryLeaks();
	if (_CrtMemDifference(&lastState, &oldState, &newState))
		_CrtMemDumpStatistics(&lastState);

	system("pause");
	return EXIT_SUCCESS;
}