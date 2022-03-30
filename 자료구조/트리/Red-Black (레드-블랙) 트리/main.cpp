#include "RBT_Core.h"

#define TREE_COUNT 2 //트리 개수

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
			// https://www.geeksforgeeks.org/red-black-tree-set-3-delete-2/?ref=lbp
			7, 3, 18, 10, 22, 8, 11, 26, 2, 6, 13
		}; //입력 데이터

		NODE* tree[TREE_COUNT] = { NULL, };

		for (int treeIndex = 0; treeIndex < TREE_COUNT; treeIndex++)
		{
			for (int i = 0; i < LENGTH(inputData); i++)
			{
				//std::cout << "Current Insert : " << inputData[i] << std::endl;
				RBT_InsertNode(&tree[treeIndex], RBT_CreateNode(inputData[i]));

#ifdef DEBUG_MODE
				RBT_ValidateTree(tree[treeIndex]);
#endif
			}

			RBT_DispOrderedTree(tree[treeIndex], TRAVERSAL_METHOD::INORDER);
			std::cout << "\n\n";
		}

		for (int treeIndex = 0; treeIndex < TREE_COUNT; treeIndex++)
		{
			switch (treeIndex)
			{
			case 0:
				std::cout << "< 입력 데이터에 대한 정방향 삭제 테스트 >\n";

				for (int i = 0; i < LENGTH(inputData); i++)
				{
					//std::cout << "Current Remove : " << inputData[i] << std::endl;
					RBT_DispOrderedTree(tree[treeIndex], TRAVERSAL_METHOD::INORDER);
					RBT_RemoveNode(&tree[treeIndex], inputData[i]);
					std::cout << "\n";

#ifdef DEBUG_MODE
					RBT_ValidateTree(tree[treeIndex]);
#endif
				}

				break;

			case 1:
				std::cout << "< 입력 데이터에 대한 역방향 삭제 테스트 >\n";

				for (int i = LENGTH(inputData) - 1; i >= 0; i--)
				{
					//std::cout << "Current Remove : " << inputData[i] << std::endl;
					RBT_DispOrderedTree(tree[treeIndex], TRAVERSAL_METHOD::INORDER);
					RBT_RemoveNode(&tree[treeIndex], inputData[i]);
					std::cout << "\n";

#ifdef DEBUG_MODE
					RBT_ValidateTree(tree[treeIndex]);
#endif
				}

				break;

			default:
				break;
			}

			RBT_DeallocateTree(&tree[treeIndex]);
		}

		RBT_DeallocateNode(&dummyBlackTerminalNode);

#ifdef COLOR_VISUALIZATION
		CONSOLE_SCREEN_MANAGER::GetInstance().Dispose();
#endif
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