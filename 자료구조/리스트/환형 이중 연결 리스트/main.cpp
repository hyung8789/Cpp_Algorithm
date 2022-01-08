#include "CDLL_Core.h"

#define COUNT 5

int main()
{
	Node* list = NULL; //노드 리스트

	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	for (int i = 0; i < COUNT; i++)
	{
		Node* newNode = CDLL_CreateNode(i); //생성
		CDLL_AppendNode(&list, newNode); //삽입
	}

	std::cout << "전체 노드의 수 : " << CDLL_GetTotalNodeCount(&list) << std::endl;
	CDLL_DispNodeList(&list);

	std::cout << "--- 0번 위치 노드 삭제 (간접 위치 접근) ---\n";
	CDLL_RemoveNodeAt(&list, 0);
	std::cout << "전체 노드의 수 : " << CDLL_GetTotalNodeCount(&list) << std::endl;
	CDLL_DispNodeList(&list);

	std::cout << "--- 1번 위치 노드 삭제 (간접 위치 접근) ---\n";
	CDLL_RemoveNodeAt(&list, 1);
	std::cout << "전체 노드의 수 : " << CDLL_GetTotalNodeCount(&list) << std::endl;
	CDLL_DispNodeList(&list);

	std::cout << "--- 2번 위치 노드 삭제 (직접 노드 접근) ---\n";
	CDLL_RemoveNode(&list, CDLL_GetNodeAt(&list, 2));
	std::cout << "전체 노드의 수 : " << CDLL_GetTotalNodeCount(&list) << std::endl;
	CDLL_DispNodeList(&list);

	std::cout << "--- 새로운 헤드 노드 삽입 (데이터 : 100) ---\n";
	CDLL_InsertNewHead(&list, CDLL_CreateNode(100));
	std::cout << "전체 노드의 수 : " << CDLL_GetTotalNodeCount(&list) << std::endl;
	CDLL_DispNodeList(&list);

	std::cout << "--- 2번 위치 뒤에 새 노드 삽입 (데이터 : 200) ---\n";
	CDLL_InsertNodeAfter(CDLL_GetNodeAt(&list, 2), CDLL_CreateNode(200));
	std::cout << "전체 노드의 수 : " << CDLL_GetTotalNodeCount(&list) << std::endl;
	CDLL_DispNodeList(&list);

	std::cout << "--- 2번 위치 앞에 새 노드 삽입 (데이터 : 300) ---\n";
	CDLL_InsertNodeBefore(&list, CDLL_GetNodeAt(&list, 2), CDLL_CreateNode(300));
	std::cout << "전체 노드의 수 : " << CDLL_GetTotalNodeCount(&list) << std::endl;
	CDLL_DispNodeList(&list);

	std::cout << "--- 0번 위치 앞에 새 노드 삽입 (데이터 : 400) ---\n";
	CDLL_InsertNodeBefore(&list, CDLL_GetNodeAt(&list, 0), CDLL_CreateNode(400));
	std::cout << "전체 노드의 수 : " << CDLL_GetTotalNodeCount(&list) << std::endl;
	CDLL_DispNodeList(&list);

	std::cout << "--- 리스트 전체 메모리 해제 ---\n";
	CDLL_DeallocateNodeList(&list);
	std::cout << "전체 노드의 수 : " << CDLL_GetTotalNodeCount(&list) << std::endl;
	CDLL_DispNodeList(&list);

	_CrtMemCheckpoint(&newState); //할당 해제 후 상태
	_CrtDumpMemoryLeaks();
	if (_CrtMemDifference(&lastState, &oldState, &newState))
		_CrtMemDumpStatistics(&lastState);

	system("pause");
	return EXIT_SUCCESS;
}