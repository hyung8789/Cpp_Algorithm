#include "Search_Core.h"

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
		
		std::cout << "========== 이중 연결 리스트 순차탐색 - 전진 이동법 (Move To Front) ==========\n";
		for(int targetData = 4; targetData >= 0; targetData--)
		{
			std::cout << "현재 검색 대상 데이터 : " << targetData;
			Node* result = DLL_MTF_SequentialSearch(&list, targetData);
			if (result->data != targetData)
				throw std::logic_error(std::string(__func__) + std::string(" : Search Logic Error"));
			else
				std::cout << " - found\n";

			std::cout << "--- 탐색 완료 후 리스트 상태 ---\n";
			DLL_DispNodeList(&list);
		}

		std::cout << "\n========== 이중 연결 리스트 순차탐색 - 전위법 (Transpose) ==========\n";
		for (int targetData = 4; targetData >= 0; targetData--)
		{
			std::cout << "현재 검색 대상 데이터 : " << targetData;
			Node* result = DLL_Transpose_SequentialSearch(&list, targetData);
			if (result->data != targetData)
				throw std::logic_error(std::string(__func__) + std::string(" : Search Logic Error"));
			else
				std::cout << " - found\n";

			std::cout << "--- 탐색 완료 후 리스트 상태 ---\n";
			DLL_DispNodeList(&list);
		}


		DLL_DeallocateNodeList(&list);
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