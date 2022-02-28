#include "RBT_Core.h"

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
		int inputData[] =
		{
			// https://www.geeksforgeeks.org/red-black-tree-set-3-delete-2/?ref=lbp
			7, 3, 18, 10, 22, 8, 11, 26, 2, 6, 13
		}; //입력 데이터

		NODE* tree = NULL;

		for (int i = 0; i < LENGTH(inputData); i++)
		{
			std::cout << "Current Insert : " << inputData[i] << std::endl;
			RBT_InsertNode(&tree, RBT_CreateNode(inputData[i]));
		}

		RBT_DispOrderedTree(tree, TRAVERSAL_METHOD::INORDER); //valid : 2 3 6 7 8 10 11 13 18 22 26 
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