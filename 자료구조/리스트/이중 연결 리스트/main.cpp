#include "DLL_Core.h"

#define COUNT 5

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����

	try
	{
		Node* list = NULL; //��� ����Ʈ

		for (int i = 0; i < COUNT; i++)
		{
			Node* newNode = DLL_CreateNode(i); //����
			DLL_AppendNode(&list, newNode); //����
		}

		std::cout << "��ü ����� �� : " << DLL_GetTotalNodeCount(&list) << std::endl;
		DLL_DispNodeList(&list);

		std::cout << "--- 0�� ��ġ ��� ���� (���� ��ġ ����) ---\n";
		DLL_RemoveNodeAt(&list, 0);
		std::cout << "��ü ����� �� : " << DLL_GetTotalNodeCount(&list) << std::endl;
		DLL_DispNodeList(&list);

		std::cout << "--- 1�� ��ġ ��� ���� (���� ��ġ ����) ---\n";
		DLL_RemoveNodeAt(&list, 1);
		std::cout << "��ü ����� �� : " << DLL_GetTotalNodeCount(&list) << std::endl;
		DLL_DispNodeList(&list);

		std::cout << "--- 2�� ��ġ ��� ���� (���� ��� ����) ---\n";
		DLL_RemoveNode(&list, DLL_GetNodeAt(&list, 2));
		std::cout << "��ü ����� �� : " << DLL_GetTotalNodeCount(&list) << std::endl;
		DLL_DispNodeList(&list);

		std::cout << "--- ���ο� ��� ��� ���� (������ : 100) ---\n";
		DLL_InsertNewHead(&list, DLL_CreateNode(100));
		std::cout << "��ü ����� �� : " << DLL_GetTotalNodeCount(&list) << std::endl;
		DLL_DispNodeList(&list);

		std::cout << "--- 2�� ��ġ �ڿ� �� ��� ���� (������ : 200) ---\n";
		DLL_InsertNodeAfter(DLL_GetNodeAt(&list, 2), DLL_CreateNode(200));
		std::cout << "��ü ����� �� : " << DLL_GetTotalNodeCount(&list) << std::endl;
		DLL_DispNodeList(&list);

		std::cout << "--- 2�� ��ġ �տ� �� ��� ���� (������ : 300) ---\n";
		DLL_InsertNodeBefore(&list, DLL_GetNodeAt(&list, 2), DLL_CreateNode(300));
		std::cout << "��ü ����� �� : " << DLL_GetTotalNodeCount(&list) << std::endl;
		DLL_DispNodeList(&list);

		std::cout << "--- 0�� ��ġ �տ� �� ��� ���� (������ : 400) ---\n";
		DLL_InsertNodeBefore(&list, DLL_GetNodeAt(&list, 0), DLL_CreateNode(400));
		std::cout << "��ü ����� �� : " << DLL_GetTotalNodeCount(&list) << std::endl;
		DLL_DispNodeList(&list);

		std::cout << "--- ����Ʈ ��ü �޸� ���� ---\n";
		DLL_DeallocateNodeList(&list);
		std::cout << "��ü ����� �� : " << DLL_GetTotalNodeCount(&list) << std::endl;
		DLL_DispNodeList(&list);
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