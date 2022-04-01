#include "LLS_Core.h"

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태
	
	try
	{
		LINKED_LIST_STACK* stack = NULL; //단일 연결 리스트 스택
		LLS_CreateStack(&stack);
		LLS_Push(stack, LLS_CreateNode("abc"));
		LLS_Push(stack, LLS_CreateNode("123"));
		LLS_Push(stack, LLS_CreateNode("test"));

		STACK_INDEX_TYPE count = LLS_GetTotalNodeCount(stack);
		for (int i = 0; i < count; i++)
		{
			NODE* poppedNode = NULL;

			std::cout << "-------------------------------------------\n";
			std::cout << "--- Before Pop ---\n";
			std::cout << "Peek Data : " << LLS_Peek(stack)->_data << std::endl;
			std::cout << "IsEmpty : " << (LLS_IsEmpty(stack) ? "true" : "false") << std::endl;
			std::cout << "전체 노드 개수 : " << LLS_GetTotalNodeCount(stack) << std::endl;
			poppedNode = LLS_Pop(stack);
			std::cout << "--- After Pop ---\n";
			std::cout << "Pop Data : " << poppedNode->_data << std::endl;
			std::cout << "IsEmpty : " << (LLS_IsEmpty(stack) ? "true" : "false") << std::endl;
			std::cout << "전체 노드 개수 : " << LLS_GetTotalNodeCount(stack) << std::endl;
			std::cout << "-------------------------------------------\n";
			LLS_DeallocateNode(&poppedNode);
		}

		LLS_Push(stack, LLS_CreateNode("tmp"));
		LLS_DeallocateLinkedListStack(&stack);
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