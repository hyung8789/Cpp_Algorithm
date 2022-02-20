#include "DLL_Core.h"

#define COUNT 5

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
		Node* list = NULL; //노드 리스트

		for (int i = 0; i < COUNT; i++)
		{
			Node* newNode = DLL_CreateNode(i); //생성
			DLL_AppendNode(&list, newNode); //삽입
		}

		std::cout << "전체 노드의 수 : " << DLL_GetTotalNodeCount(&list) << std::endl;
		DLL_DispNodeList(&list);

		std::cout << "--- 0번 위치 노드 삭제 (간접 위치 접근) ---\n";
		DLL_RemoveNodeAt(&list, 0);
		std::cout << "전체 노드의 수 : " << DLL_GetTotalNodeCount(&list) << std::endl;
		DLL_DispNodeList(&list);

		std::cout << "--- 1번 위치 노드 삭제 (간접 위치 접근) ---\n";
		DLL_RemoveNodeAt(&list, 1);
		std::cout << "전체 노드의 수 : " << DLL_GetTotalNodeCount(&list) << std::endl;
		DLL_DispNodeList(&list);

		std::cout << "--- 2번 위치 노드 삭제 (직접 노드 접근) ---\n";
		DLL_RemoveNode(&list, DLL_GetNodeAt(&list, 2));
		std::cout << "전체 노드의 수 : " << DLL_GetTotalNodeCount(&list) << std::endl;
		DLL_DispNodeList(&list);

		std::cout << "--- 새로운 헤드 노드 삽입 (데이터 : 100) ---\n";
		DLL_InsertNewHead(&list, DLL_CreateNode(100));
		std::cout << "전체 노드의 수 : " << DLL_GetTotalNodeCount(&list) << std::endl;
		DLL_DispNodeList(&list);

		std::cout << "--- 2번 위치 뒤에 새 노드 삽입 (데이터 : 200) ---\n";
		DLL_InsertNodeAfter(DLL_GetNodeAt(&list, 2), DLL_CreateNode(200));
		std::cout << "전체 노드의 수 : " << DLL_GetTotalNodeCount(&list) << std::endl;
		DLL_DispNodeList(&list);

		std::cout << "--- 2번 위치 앞에 새 노드 삽입 (데이터 : 300) ---\n";
		DLL_InsertNodeBefore(&list, DLL_GetNodeAt(&list, 2), DLL_CreateNode(300));
		std::cout << "전체 노드의 수 : " << DLL_GetTotalNodeCount(&list) << std::endl;
		DLL_DispNodeList(&list);

		std::cout << "--- 0번 위치 앞에 새 노드 삽입 (데이터 : 400) ---\n";
		DLL_InsertNodeBefore(&list, DLL_GetNodeAt(&list, 0), DLL_CreateNode(400));
		std::cout << "전체 노드의 수 : " << DLL_GetTotalNodeCount(&list) << std::endl;
		DLL_DispNodeList(&list);

		std::cout << "--- 리스트 전체 메모리 해제 ---\n";
		DLL_DeallocateNodeList(&list);
		std::cout << "전체 노드의 수 : " << DLL_GetTotalNodeCount(&list) << std::endl;
		DLL_DispNodeList(&list);
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