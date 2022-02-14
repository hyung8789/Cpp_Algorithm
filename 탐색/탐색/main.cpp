#include "Search_Core.h"

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
		
		std::cout << "========== ���� ���� ����Ʈ ����Ž�� - ���� �̵��� (Move To Front) ==========\n";
		for(int targetData = 4; targetData >= 0; targetData--)
		{
			std::cout << "���� �˻� ��� ������ : " << targetData;
			Node* result = DLL_MTF_SequentialSearch(&list, targetData);
			if (result->data != targetData)
				throw std::logic_error(std::string(__func__) + std::string(" : Search Logic Error"));
			else
				std::cout << " - found\n";

			std::cout << "--- Ž�� �Ϸ� �� ����Ʈ ���� ---\n";
			DLL_DispNodeList(&list);
		}

		std::cout << "\n========== ���� ���� ����Ʈ ����Ž�� - ������ (Transpose) ==========\n";
		for (int targetData = 4; targetData >= 0; targetData--)
		{
			std::cout << "���� �˻� ��� ������ : " << targetData;
			Node* result = DLL_Transpose_SequentialSearch(&list, targetData);
			if (result->data != targetData)
				throw std::logic_error(std::string(__func__) + std::string(" : Search Logic Error"));
			else
				std::cout << " - found\n";

			std::cout << "--- Ž�� �Ϸ� �� ����Ʈ ���� ---\n";
			DLL_DispNodeList(&list);
		}


		DLL_DeallocateNodeList(&list);
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