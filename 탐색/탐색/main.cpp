#include "Search_Core.h"

#define COUNT 5

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
		NODE* list = NULL; //노드 리스트
		for (int i = 0; i < COUNT; i++)
		{
			NODE* newNode = DLL_CreateNode(i); //생성
			DLL_AppendNode(&list, newNode); //삽입
		}

		std::cout << "\n========== 이중 연결 리스트 순차탐색 - 전진 이동법 (Move To Front) ==========\n";
		for (int targetData = COUNT - 1; targetData >= 0; targetData--)
		{
			std::cout << "현재 검색 대상 데이터 : " << targetData;
			NODE* result = DLL_SequentialSearch_MTF(&list, targetData);
			if (result->_data != targetData)
				throw std::logic_error(std::string(__func__) + std::string(" : Search Logic Error"));
			else
				std::cout << " - found\n";

			std::cout << "--- 탐색 완료 후 리스트 상태 ---\n";
			DLL_DispNodeList(list);
		}

		std::cout << "\n========== 이중 연결 리스트 순차탐색 - 전위법 (Transpose) ==========\n";
		for (int targetData = COUNT - 1; targetData >= 0; targetData--)
		{
			std::cout << "현재 검색 대상 데이터 : " << targetData;
			NODE* result = DLL_SequentialSearch_Transpose(&list, targetData);
			if (result->_data != targetData)
				throw std::logic_error(std::string(__func__) + std::string(" : Search Logic Error"));
			else
				std::cout << " - found\n";

			std::cout << "--- 탐색 완료 후 리스트 상태 ---\n";
			DLL_DispNodeList(list);
		}

		DLL_DeallocateNodeList(&list);

		int orderedFooArray[COUNT];
		for (int i = 0; i < COUNT; i++) //오름차순
		{
			orderedFooArray[i] = i;
		}

		std::cout << "\n========== 순차탐색 - 전진 이동법 (Move To Front) ==========\n";
		for (int targetData = COUNT - 1; targetData >= 0; targetData--)
		{
			std::cout << "현재 검색 대상 데이터 : " << targetData;
			int result = SequentialSearch_MTF<int>(orderedFooArray, COUNT, targetData);
			if (result != targetData)
				throw std::logic_error(std::string(__func__) + std::string(" : Search Logic Error"));
			else
				std::cout << " - found\n";

			std::cout << "--- 탐색 완료 후 배열 상태 ---\n";
			for (int i = 0; i < COUNT; i++)
				std::cout << orderedFooArray[i] << " ";
			std::cout << std::endl;
		}

		std::cout << "\n========== 순차탐색 - 전위법 (Transpose) ==========\n";
		for (int targetData = COUNT - 1; targetData >= 0; targetData--)
		{
			std::cout << "현재 검색 대상 데이터 : " << targetData;
			int result = SequentialSearch_Transpose<int>(orderedFooArray, COUNT, targetData);
			if (result != targetData)
				throw std::logic_error(std::string(__func__) + std::string(" : Search Logic Error"));
			else
				std::cout << " - found\n";

			std::cout << "--- 탐색 완료 후 배열 상태 ---\n";
			for (int i = 0; i < COUNT; i++)
				std::cout << orderedFooArray[i] << " ";
			std::cout << std::endl;
		}

		//이 시점에서, 위의 리스트와 동일하게 정렬 된 순서가 유지되어있음을 보장해야 함

		std::cout << "\n========== 오름차순 정렬 된 데이터에 대한 이진 탐색 ==========\n";
		for (int targetData = COUNT - 1; targetData >= 0; targetData--)
		{
			std::cout << "현재 검색 대상 데이터 : " << targetData;
			int result = BinarySearch<int>(orderedFooArray, COUNT, targetData);
			if (result != targetData)
				throw std::logic_error(std::string(__func__) + std::string(" : Search Logic Error"));
			else
				std::cout << " - found\n";
		}

		for (int i = 0; i < COUNT; i++) //내림차순
		{
			orderedFooArray[i] = (COUNT - 1) - i;
		}
		std::cout << "\n========== 내림차순 정렬 된 데이터에 대한 이진 탐색 ==========\n";
		for (int targetData = COUNT - 1; targetData >= 0; targetData--)
		{
			std::cout << "현재 검색 대상 데이터 : " << targetData;
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

	_CrtMemCheckpoint(&newState); //할당 해제 후 상태
	_CrtDumpMemoryLeaks();
	if (_CrtMemDifference(&lastState, &oldState, &newState))
		_CrtMemDumpStatistics(&lastState);

	system("pause");
	return EXIT_SUCCESS;
}