#include "Search_Core.h"

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
			NODE* newNode = DLL_CreateNode(i); //����
			DLL_AppendNode(&list, newNode); //����
		}

		std::cout << "\n========== ���� ���� ����Ʈ ����Ž�� - ���� �̵��� (Move To Front) ==========\n";
		for (int targetData = COUNT - 1; targetData >= 0; targetData--)
		{
			std::cout << "���� �˻� ��� ������ : " << targetData;
			NODE* result = DLL_SequentialSearch_MTF(&list, targetData);
			if (result->_data != targetData)
				throw std::logic_error(std::string(__func__) + std::string(" : Search Logic Error"));
			else
				std::cout << " - found\n";

			std::cout << "--- Ž�� �Ϸ� �� ����Ʈ ���� ---\n";
			DLL_DispNodeList(&list);
		}

		std::cout << "\n========== ���� ���� ����Ʈ ����Ž�� - ������ (Transpose) ==========\n";
		for (int targetData = COUNT - 1; targetData >= 0; targetData--)
		{
			std::cout << "���� �˻� ��� ������ : " << targetData;
			NODE* result = DLL_SequentialSearch_Transpose(&list, targetData);
			if (result->_data != targetData)
				throw std::logic_error(std::string(__func__) + std::string(" : Search Logic Error"));
			else
				std::cout << " - found\n";

			std::cout << "--- Ž�� �Ϸ� �� ����Ʈ ���� ---\n";
			DLL_DispNodeList(&list);
		}

		DLL_DeallocateNodeList(&list);

		int orderedFooArray[COUNT];
		for (int i = 0; i < COUNT; i++) //��������
		{
			orderedFooArray[i] = i;
		}

		std::cout << "\n========== ����Ž�� - ���� �̵��� (Move To Front) ==========\n";
		for (int targetData = COUNT - 1; targetData >= 0; targetData--)
		{
			std::cout << "���� �˻� ��� ������ : " << targetData;
			int result = SequentialSearch_MTF<int>(orderedFooArray, COUNT, targetData);
			if (result != targetData)
				throw std::logic_error(std::string(__func__) + std::string(" : Search Logic Error"));
			else
				std::cout << " - found\n";

			std::cout << "--- Ž�� �Ϸ� �� �迭 ���� ---\n";
			for (int i = 0; i < COUNT; i++)
				std::cout << orderedFooArray[i] << " ";
			std::cout << std::endl;
		}

		std::cout << "\n========== ����Ž�� - ������ (Transpose) ==========\n";
		for (int targetData = COUNT - 1; targetData >= 0; targetData--)
		{
			std::cout << "���� �˻� ��� ������ : " << targetData;
			int result = SequentialSearch_Transpose<int>(orderedFooArray, COUNT, targetData);
			if (result != targetData)
				throw std::logic_error(std::string(__func__) + std::string(" : Search Logic Error"));
			else
				std::cout << " - found\n";

			std::cout << "--- Ž�� �Ϸ� �� �迭 ���� ---\n";
			for (int i = 0; i < COUNT; i++)
				std::cout << orderedFooArray[i] << " ";
			std::cout << std::endl;
		}

		//�� ��������, ���� ����Ʈ�� �����ϰ� ���� �� ������ �����Ǿ������� �����ؾ� ��

		std::cout << "\n========== �������� ���� �� �����Ϳ� ���� ���� Ž�� ==========\n";
		for (int targetData = COUNT - 1; targetData >= 0; targetData--)
		{
			std::cout << "���� �˻� ��� ������ : " << targetData;
			int result = BinarySearch<int>(orderedFooArray, COUNT, targetData);
			if (result != targetData)
				throw std::logic_error(std::string(__func__) + std::string(" : Search Logic Error"));
			else
				std::cout << " - found\n";
		}

		for (int i = 0; i < COUNT; i++) //��������
		{
			orderedFooArray[i] = (COUNT - 1) - i;
		}
		std::cout << "\n========== �������� ���� �� �����Ϳ� ���� ���� Ž�� ==========\n";
		for (int targetData = COUNT - 1; targetData >= 0; targetData--)
		{
			std::cout << "���� �˻� ��� ������ : " << targetData;
			int result = BinarySearch<int>(orderedFooArray, COUNT, targetData);
			if (result != targetData)
				throw std::logic_error(std::string(__func__) + std::string(" : Search Logic Error"));
			else
				std::cout << " - found\n";
		}

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