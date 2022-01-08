#include "CDLL_Core.h"

#define COUNT 5

int main()
{
	Node* list = NULL; //��� ����Ʈ

	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����

	for (int i = 0; i < COUNT; i++)
	{
		Node* newNode = CDLL_CreateNode(i); //����
		CDLL_AppendNode(&list, newNode); //����
	}

	std::cout << "��ü ����� �� : " << CDLL_GetTotalNodeCount(&list) << std::endl;
	CDLL_DispNodeList(&list);

	std::cout << "--- 0�� ��ġ ��� ���� (���� ��ġ ����) ---\n";
	CDLL_RemoveNodeAt(&list, 0);
	std::cout << "��ü ����� �� : " << CDLL_GetTotalNodeCount(&list) << std::endl;
	CDLL_DispNodeList(&list);

	std::cout << "--- 1�� ��ġ ��� ���� (���� ��ġ ����) ---\n";
	CDLL_RemoveNodeAt(&list, 1);
	std::cout << "��ü ����� �� : " << CDLL_GetTotalNodeCount(&list) << std::endl;
	CDLL_DispNodeList(&list);

	std::cout << "--- 2�� ��ġ ��� ���� (���� ��� ����) ---\n";
	CDLL_RemoveNode(&list, CDLL_GetNodeAt(&list, 2));
	std::cout << "��ü ����� �� : " << CDLL_GetTotalNodeCount(&list) << std::endl;
	CDLL_DispNodeList(&list);

	std::cout << "--- ���ο� ��� ��� ���� (������ : 100) ---\n";
	CDLL_InsertNewHead(&list, CDLL_CreateNode(100));
	std::cout << "��ü ����� �� : " << CDLL_GetTotalNodeCount(&list) << std::endl;
	CDLL_DispNodeList(&list);

	std::cout << "--- 2�� ��ġ �ڿ� �� ��� ���� (������ : 200) ---\n";
	CDLL_InsertNodeAfter(CDLL_GetNodeAt(&list, 2), CDLL_CreateNode(200));
	std::cout << "��ü ����� �� : " << CDLL_GetTotalNodeCount(&list) << std::endl;
	CDLL_DispNodeList(&list);

	std::cout << "--- 2�� ��ġ �տ� �� ��� ���� (������ : 300) ---\n";
	CDLL_InsertNodeBefore(&list, CDLL_GetNodeAt(&list, 2), CDLL_CreateNode(300));
	std::cout << "��ü ����� �� : " << CDLL_GetTotalNodeCount(&list) << std::endl;
	CDLL_DispNodeList(&list);

	std::cout << "--- 0�� ��ġ �տ� �� ��� ���� (������ : 400) ---\n";
	CDLL_InsertNodeBefore(&list, CDLL_GetNodeAt(&list, 0), CDLL_CreateNode(400));
	std::cout << "��ü ����� �� : " << CDLL_GetTotalNodeCount(&list) << std::endl;
	CDLL_DispNodeList(&list);

	std::cout << "--- ����Ʈ ��ü �޸� ���� ---\n";
	CDLL_DeallocateNodeList(&list);
	std::cout << "��ü ����� �� : " << CDLL_GetTotalNodeCount(&list) << std::endl;
	CDLL_DispNodeList(&list);

	_CrtMemCheckpoint(&newState); //�Ҵ� ���� �� ����
	_CrtDumpMemoryLeaks();
	if (_CrtMemDifference(&lastState, &oldState, &newState))
		_CrtMemDumpStatistics(&lastState);

	system("pause");
	return EXIT_SUCCESS;
}