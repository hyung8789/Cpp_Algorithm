#include "BST_Core.h"

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
		/***
										23 (Root)
						10 (Remove Target)				100
				1				16					50
			0		9		12		20
			
			10 삭제 전 중위 순회 : 0 1 9 10 12 16 20 23 50 100
			10 삭제 후,

										23 (Root)
						12									100
				1				16						50
			0		9				20

			10 삭제 후 중위 순회 : 0 1 9 12 16 20 23 50 100
			루트 노드 23 삭제 후 중위 순회 : 0 1 9 12 16 20 50 100
		***/

		int inputData[] = 
		{
			10,100,1,16,50,0,9,12,20
		}; //입력 데이터 (루트 노드 23)

		Node* rootNode = BT_CreateNode(23); //최상위 루트 노드

		for (int i = 0; i < LENGTH(inputData); i++)
		{
			BST_InsertNode(&rootNode, BT_CreateNode(inputData[i]));
		}

		std::cout << "\n10 삭제 전 중위 순회 (Left -> Root -> Right) : ";
		BT_DispOrderedTree(rootNode, TRAVERSAL_MODE::INORDER);
		
		std::cout << "\n10 삭제 후 중위 순회 (Left -> Root -> Right) : ";
		BST_RemoveNode(&rootNode, 10);
		BT_DispOrderedTree(rootNode, TRAVERSAL_MODE::INORDER);

		
		std::cout << "\n루트 노드 23 삭제 후 중위 순회 (Left -> Root -> Right) : ";
		BST_RemoveNode(&rootNode, 23);
		BT_DispOrderedTree(rootNode, TRAVERSAL_MODE::INORDER);
		
		std::cout << "\n";
		BT_DeallocateTree(&rootNode);
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