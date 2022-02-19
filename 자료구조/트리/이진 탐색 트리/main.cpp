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
			
			전위 순회 (Root -> Left -> Right) : 23 10 1 0 9 16 12 20 100 50
			중위 순회 (Left -> Root -> Right) : 0 1 9 10 12 16 20 23 50 100
			후위 순회 (Left -> Right -> Root) : 0 9 1 12 20 16 10 50 100 23

			---

			중간 노드 10 삭제 후,

										23 (Root)
						12									100
				1				16						50
			0		9				20

			중간 노드 10 삭제 후 중위 순회 : 0 1 9 12 16 20 23 50 100

			---

			루트 노드 23 삭제 후 중위 순회 : 0 1 9 12 16 20 50 100
			단말 노드 9 삭제 후 중위 순회 : 0 1 12 16 20 50 100
			중간 노드 1 삭제 후 중위 순회 : 0 12 16 20 50 100
		***/

		int inputData[] = 
		{
			23, 10,100,1,16,50,0,9,12,20
		}; //입력 데이터 (루트 노드 23)

		Node* rootNode = BST_CreateNode(inputData[0]); //최상위 루트 노드

		for (int i = 1; i < LENGTH(inputData); i++)
		{
			BST_InsertNode(&rootNode, BST_CreateNode(inputData[i]));
		}

		std::cout << "\n전위 순회 (Root -> Left -> Right) : ";
		BST_DispOrderedTree(rootNode, TRAVERSAL_MODE::PREORDER);

		std::cout << "\n중위 순회 (Left -> Root -> Right) : ";
		BST_DispOrderedTree(rootNode, TRAVERSAL_MODE::INORDER);
		
		std::cout << "\n후위 순회 (Left -> Right -> Root) : ";
		BST_DispOrderedTree(rootNode, TRAVERSAL_MODE::POSTORDER);

		std::cout << "\n중간 노드 10 삭제 후 중위 순회 (Left -> Root -> Right) : ";
		BST_RemoveNode(&rootNode, 10);
		BST_DispOrderedTree(rootNode, TRAVERSAL_MODE::INORDER);
		
		std::cout << "\n루트 노드 23 삭제 후 중위 순회 (Left -> Root -> Right) : ";
		BST_RemoveNode(&rootNode, 23);
		BST_DispOrderedTree(rootNode, TRAVERSAL_MODE::INORDER);
		
		std::cout << "\n단말 노드 9 삭제 후 중위 순회 (Left -> Root -> Right) : ";
		BST_RemoveNode(&rootNode, 9);
		BST_DispOrderedTree(rootNode, TRAVERSAL_MODE::INORDER);

		std::cout << "\n중간 노드 1 삭제 후 중위 순회 (Left -> Root -> Right) : ";
		BST_RemoveNode(&rootNode, 1);
		BST_DispOrderedTree(rootNode, TRAVERSAL_MODE::INORDER);

		std::cout << "\n";
		BST_DeallocateTree(&rootNode);
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