#include "BST_Core.h"

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����

	try
	{
		/***
										23 (Root)
						10 (Remove Target)				100
				1				16					50
			0		9		12		20
			
			10 ���� �� ���� ��ȸ : 0 1 9 10 12 16 20 23 50 100
			10 ���� ��,

										23 (Root)
						12									100
				1				16						50
			0		9				20

			10 ���� �� ���� ��ȸ : 0 1 9 12 16 20 23 50 100
			��Ʈ ��� 23 ���� �� ���� ��ȸ : 0 1 9 12 16 20 50 100
		***/

		int inputData[] = 
		{
			10,100,1,16,50,0,9,12,20
		}; //�Է� ������ (��Ʈ ��� 23)

		Node* rootNode = BT_CreateNode(23); //�ֻ��� ��Ʈ ���

		for (int i = 0; i < LENGTH(inputData); i++)
		{
			BST_InsertNode(&rootNode, BT_CreateNode(inputData[i]));
		}

		std::cout << "\n10 ���� �� ���� ��ȸ (Left -> Root -> Right) : ";
		BT_DispOrderedTree(rootNode, TRAVERSAL_MODE::INORDER);
		
		std::cout << "\n10 ���� �� ���� ��ȸ (Left -> Root -> Right) : ";
		BST_RemoveNode(&rootNode, 10);
		BT_DispOrderedTree(rootNode, TRAVERSAL_MODE::INORDER);

		
		std::cout << "\n��Ʈ ��� 23 ���� �� ���� ��ȸ (Left -> Root -> Right) : ";
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

	_CrtMemCheckpoint(&newState); //�Ҵ� ���� �� ����
	_CrtDumpMemoryLeaks();
	if (_CrtMemDifference(&lastState, &oldState, &newState))
		_CrtMemDumpStatistics(&lastState);

	system("pause");
	return EXIT_SUCCESS;
}