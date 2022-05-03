#include "RBT_Core.h"

#define TREE_COUNT 2 //트리 개수
#define TEST_RAND_STR_KEY
#define RAND_STR_KEY_COUNT 50
#define RAND_STR_KEY_LEN 5

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
#ifdef STR_STORAGE_RBT_TYPE
#ifdef TEST_RAND_STR_KEY
		char inputKey[RAND_STR_KEY_COUNT][RAND_STR_KEY_LEN];

		for (int i = 0; i < RAND_STR_KEY_COUNT; i++)
		{
			utils::GenRandStr(inputKey[i], sizeof(inputKey[i]), (RAND_STR_KEY_LEN - 1));
		}
#else
		const char* inputKey[] =
		{
			"c1234", "b223", "i12", "f132", "j1", "e22", "g333", "k44", "a55", "d6666", "h77"
		}; //입력 키
#endif
		const char* inputData = "dummy data"; //입력 데이터
#else
		const int inputKey[] =
		{
			// https://www.geeksforgeeks.org/red-black-tree-set-3-delete-2/?ref=lbp
			//7, 3, 18, 10, 22, 8, 11, 26, 2, 6, 13
			7, 5, 3, 4, 6, 8, 9, 10, 100, 50, 20, 30, 43, 34, 35, 55
		}; //입력 키

		const int inputData = -1; //입력 데이터
#endif

		RBT_NODE* tree[TREE_COUNT] = { NULL, };

		for (int treeIndex = 0; treeIndex < TREE_COUNT; treeIndex++)
		{
			for (int i = 0; i < LENGTH(inputKey); i++)
			{
				std::cout << "Current Insert : " << inputKey[i] << std::endl;
				RBT_InsertNode(&tree[treeIndex], RBT_CreateNode((RBT_KEY_TYPE)inputKey[i], (RBT_DATA_TYPE)inputData));
				RBT_DispOrderedTree(tree[treeIndex], TRAVERSAL_METHOD::INORDER);
				std::cout << "\n\n";
			}

			RBT_DispOrderedTree(tree[treeIndex], TRAVERSAL_METHOD::INORDER);
			std::cout << "\n\n";
		}

		for (int treeIndex = 0; treeIndex < TREE_COUNT; treeIndex++)
		{
			switch (treeIndex)
			{
			case 0:
				std::cout << "< 입력 키 및 데이터에 대한 정방향 삭제 테스트 >\n";

				for (int i = 0; i < LENGTH(inputKey); i++)
				{
					std::cout << "Current Remove : " << inputKey[i] << std::endl;
					RBT_DispOrderedTree(tree[treeIndex], TRAVERSAL_METHOD::INORDER);
					std::cout << "\n\n";
					RBT_RemoveNode(&tree[treeIndex], (RBT_KEY_TYPE)inputKey[i]);
				}

				break;

			case 1:
				std::cout << "< 입력 키 및 데이터에 대한 역방향 삭제 테스트 >\n";

				for (int i = LENGTH(inputKey) - 1; i >= 0; i--)
				{
					std::cout << "Current Remove : " << inputKey[i] << std::endl;
					RBT_DispOrderedTree(tree[treeIndex], TRAVERSAL_METHOD::INORDER);
					std::cout << "\n\n";
					RBT_RemoveNode(&tree[treeIndex], (RBT_KEY_TYPE)inputKey[i]);
				}

				break;

			default:
				break;
			}

			RBT_DeallocateTree(&tree[treeIndex], false);
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