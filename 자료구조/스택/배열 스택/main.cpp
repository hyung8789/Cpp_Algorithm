#include "AS_Core.h"

#define STACK_SIZE 3

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
		ArrayStack* stack = NULL; //배열 스택
		AS_CreateStack(&stack, STACK_SIZE); //STACK_SIZE만큼 삽입 가능 한 크기 할당

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
			std::cout << "전체 노드 데이터 개수 : " << AS_GetTotalNodeDataCount(&stack) << std::endl;
			std::cout << "--- After Pop ---\n";
			std::cout << "Pop Data : " << AS_Pop(&stack) << std::endl;
			std::cout << "Top Index : " << stack->_top << std::endl;
			std::cout << "IsEmpty : " << (AS_IsEmpty(&stack) ? "true" : "false") << std::endl;
			std::cout << "IsFull : " << (AS_IsFull(&stack) ? "true" : "false") << std::endl;
			std::cout << "전체 노드 데이터 개수 : " << AS_GetTotalNodeDataCount(&stack) << std::endl;
			std::cout << "-------------------------------------------\n";
		}

		std::cout << "--- realloc test ---\n";
		int reallocTestSize = STACK_SIZE * 2;
		for (int i = 0; i < reallocTestSize; i++) //초기 할당 크기를 초과하여 데이터 삽입
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

	_CrtMemCheckpoint(&newState); //할당 해제 후 상태
	_CrtDumpMemoryLeaks();
	if (_CrtMemDifference(&lastState, &oldState, &newState))
		_CrtMemDumpStatistics(&lastState);

	system("pause");
	return EXIT_SUCCESS;
}