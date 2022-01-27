#include "BT_Core.h"

#define NODE_COUNT 6

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����

	try
	{
		/***
					A
				B		C
			D	E		F	G
		***/

		Node* rootNode = BT_CreateNode('A'); //�ֻ��� ��Ʈ ���
		Node* dummyNode[NODE_COUNT] = { NULL, }; //���� Ʈ���� ���� �� ���
		for (int i = 0; i < NODE_COUNT; i++) //�ֻ��� ��Ʈ ��Ʈ�� �ڽ� ��� ����
		{
			dummyNode[i] = BT_CreateNode((char)(66 + i)); //B���� �Ҵ�
		}

		rootNode->left = dummyNode[0]; //root -> B
		rootNode->right = dummyNode[1]; //root -> C

		dummyNode[0]->left = dummyNode[2]; //B -> D
		dummyNode[0]->right = dummyNode[3]; //B -> E

		dummyNode[1]->left = dummyNode[4]; //C -> F
		dummyNode[1]->right = dummyNode[5]; //C -> G


		std::cout << "\n���� ��ȸ (Root -> Left -> Right) : ";
		BT_DispTree(rootNode, TRAVERSAL_MODE::PREORDER);
		std::cout << "\n���� ��ȸ (Left -> Root -> Right) : ";
		BT_DispTree(rootNode, TRAVERSAL_MODE::INORDER);
		std::cout << "\n���� ��ȸ (Left -> Right -> Root) : ";
		BT_DispTree(rootNode, TRAVERSAL_MODE::POSTORDER);
		std::cout << "\n";
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