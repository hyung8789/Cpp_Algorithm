#include "SLL_Core.h"

#define COUNT 5

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����

	try
	{
		NODE* list = NULL; //��� ����Ʈ

		for (int i = 0; i < COUNT; i++)
		{
			NODE* newNode = SLL_CreateNode(i); //����
			SLL_AppendNode(&list, newNode); //����
		}

		std::cout << "��ü ����� �� : " << SLL_GetTotalNodeCount(&list) << std::endl;
		SLL_DispNodeList(&list);

		std::cout << "--- 0�� ��ġ ��� ���� (���� ��ġ ����) ---\n";
		SLL_RemoveNodeAt(&list, 0);
		std::cout << "��ü ����� �� : " << SLL_GetTotalNodeCount(&list) << std::endl;
		SLL_DispNodeList(&list);

		std::cout << "--- 1�� ��ġ ��� ���� (���� ��ġ ����) ---\n";
		SLL_RemoveNodeAt(&list, 1);
		std::cout << "��ü ����� �� : " << SLL_GetTotalNodeCount(&list) << std::endl;
		SLL_DispNodeList(&list);

		std::cout << "--- 2�� ��ġ ��� ���� (���� ��� ����) ---\n";
		SLL_RemoveNode(&list, SLL_GetNodeAt(&list, 2));
		std::cout << "��ü ����� �� : " << SLL_GetTotalNodeCount(&list) << std::endl;
		SLL_DispNodeList(&list);

		std::cout << "--- ���ο� ��� ��� ���� (������ : 100) ---\n";
		SLL_InsertNewHead(&list, SLL_CreateNode(100));
		std::cout << "��ü ����� �� : " << SLL_GetTotalNodeCount(&list) << std::endl;
		SLL_DispNodeList(&list);

		std::cout << "--- 2�� ��ġ �ڿ� �� ��� ���� (������ : 200) ---\n";
		SLL_InsertNodeAfter(SLL_GetNodeAt(&list, 2), SLL_CreateNode(200));
		std::cout << "��ü ����� �� : " << SLL_GetTotalNodeCount(&list) << std::endl;
		SLL_DispNodeList(&list);

		std::cout << "--- 2�� ��ġ �տ� �� ��� ���� (������ : 300) ---\n";
		SLL_InsertNodeBefore(&list, SLL_GetNodeAt(&list, 2), SLL_CreateNode(300));
		std::cout << "��ü ����� �� : " << SLL_GetTotalNodeCount(&list) << std::endl;
		SLL_DispNodeList(&list);

		std::cout << "--- 0�� ��ġ �տ� �� ��� ���� (������ : 400) ---\n";
		SLL_InsertNodeBefore(&list, SLL_GetNodeAt(&list, 0), SLL_CreateNode(400));
		std::cout << "��ü ����� �� : " << SLL_GetTotalNodeCount(&list) << std::endl;
		SLL_DispNodeList(&list);

		std::cout << "--- ����Ʈ ��ü �޸� ���� ---\n";
		SLL_DeallocateNodeList(&list);
		std::cout << "��ü ����� �� : " << SLL_GetTotalNodeCount(&list) << std::endl;
		SLL_DispNodeList(&list);

		_CrtMemCheckpoint(&newState); //�Ҵ� ���� �� ����
		_CrtDumpMemoryLeaks();
		if (_CrtMemDifference(&lastState, &oldState, &newState))
			_CrtMemDumpStatistics(&lastState);
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	system("pause");
	return EXIT_SUCCESS;
}