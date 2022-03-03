#include "RBT_Core.h"

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
		dummyBlackTerminalNode = RBT_CreateNode(DUMMY_BLACK_TERMINAL_NODE_DATA);
		dummyBlackTerminalNode->_color = COLOR::BLACK;

		int inputData[] =
		{
			// https://www.geeksforgeeks.org/red-black-rootNode-set-3-delete-2/?ref=lbp
			7, 3, 18, 10, 22, 8, 11, 26, 2, 6, 13
		}; //입력 데이터

		NODE* rootNode = NULL;

		for (int i = 0; i < LENGTH(inputData); i++)
		{
			std::cout << "Current Insert : " << inputData[i] << std::endl;
			RBT_InsertNode(&rootNode, RBT_CreateNode(inputData[i]));

			if (dummyBlackTerminalNode->_parent != NULL) //검은색 더미 단말 노드에서 부모로의 연결은 허용하지 않음 
				throw std::logic_error(std::string(__func__) + std::string(" : Not allowed parent connection from dummy"));

			if (rootNode->_color == COLOR::RED)
				throw std::logic_error(std::string(__func__) + std::string(" : DEF1) violation"));
		}

		RBT_DispOrderedTree(rootNode, TRAVERSAL_METHOD::INORDER); //valid : 2 3 6 7 8 10 11 13 18 22 26 

		RBT_DeallocateTree(&rootNode);
		RBT_DeallocateNode(&dummyBlackTerminalNode);
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