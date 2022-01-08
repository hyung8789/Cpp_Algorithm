#include "LLS_Core.h"

int main()
{
	LinkedListStack* stack = NULL; //연결 리스트 스택

	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	LLS_CreateStack(&stack);
	LLS_Push(&stack, LLS_CreateNode("abc"));
	LLS_Push(&stack, LLS_CreateNode("123"));
	LLS_Push(&stack, LLS_CreateNode("test"));

	StackIndexType count = LLS_GetTotalNodeCount(&stack);
	for (int i = 0; i < count; i++)
	{
		Node* popedNode = NULL;

		std::cout << "-------------------------------------------\n";
		std::cout << "--- Before Pop ---\n";
		std::cout << "Peek Data : " << LLS_Peek(&stack)->data << std::endl;
		std::cout << "IsEmpty : " << (LLS_IsEmpty(&stack) ? "true" : "false") << std::endl;
		std::cout << "전체 노드 데이터 개수 : " << LLS_GetTotalNodeCount(&stack) << std::endl;
		popedNode = LLS_Pop(&stack);
		std::cout << "--- After Pop ---\n";
		std::cout << "Pop Data : " << popedNode->data << std::endl;
		std::cout << "IsEmpty : " << (LLS_IsEmpty(&stack) ? "true" : "false") << std::endl;
		std::cout << "전체 노드 데이터 개수 : " << LLS_GetTotalNodeCount(&stack) << std::endl;
		std::cout << "-------------------------------------------\n";
		LLS_DeallocateNode(&popedNode);
	}

	LLS_Push(&stack, LLS_CreateNode("tmp"));
	LLS_DeallocateLinkedListStack(&stack);

	_CrtMemCheckpoint(&newState); //할당 해제 후 상태
	_CrtDumpMemoryLeaks();
	if (_CrtMemDifference(&lastState, &oldState, &newState))
		_CrtMemDumpStatistics(&lastState);

	system("pause");
	return EXIT_SUCCESS;
}