#include "LLS_Core.h"

int main()
{
	LinkedListStack* stack = NULL; //���� ����Ʈ ����

	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����
	
	try
	{
		LLS_CreateStack(&stack);
		LLS_Push(&stack, LLS_CreateNode("abc"));
		LLS_Push(&stack, LLS_CreateNode("123"));
		LLS_Push(&stack, LLS_CreateNode("test"));

		StackIndexType count = LLS_GetTotalNodeCount(&stack);
		for (int i = 0; i < count; i++)
		{
			Node* poppedNode = NULL;

			std::cout << "-------------------------------------------\n";
			std::cout << "--- Before Pop ---\n";
			std::cout << "Peek Data : " << LLS_Peek(&stack)->data << std::endl;
			std::cout << "IsEmpty : " << (LLS_IsEmpty(&stack) ? "true" : "false") << std::endl;
			std::cout << "��ü ��� ������ ���� : " << LLS_GetTotalNodeCount(&stack) << std::endl;
			poppedNode = LLS_Pop(&stack);
			std::cout << "--- After Pop ---\n";
			std::cout << "Pop Data : " << poppedNode->data << std::endl;
			std::cout << "IsEmpty : " << (LLS_IsEmpty(&stack) ? "true" : "false") << std::endl;
			std::cout << "��ü ��� ������ ���� : " << LLS_GetTotalNodeCount(&stack) << std::endl;
			std::cout << "-------------------------------------------\n";
			LLS_DeallocateNode(&poppedNode);
		}

		LLS_Push(&stack, LLS_CreateNode("tmp"));
		LLS_DeallocateLinkedListStack(&stack);
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