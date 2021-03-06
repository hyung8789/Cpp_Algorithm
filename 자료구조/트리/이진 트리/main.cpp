#include "BT_Core.h"

#define NODE_COUNT 6

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
		/***
					A
				B		C
			D	E		F	G
		***/

		NODE* rootNode = BT_CreateNode('A'); //최상위 루트 노드
		NODE* dummyNode[NODE_COUNT] = { NULL, }; //이진 트리에 연결 할 노드
		for (int i = 0; i < NODE_COUNT; i++) //최상위 루트 노트에 자식 노드 연결
		{
			dummyNode[i] = BT_CreateNode((char)(66 + i)); //B부터 할당
		}

		rootNode->_left = dummyNode[0]; //root -> B
		rootNode->_right = dummyNode[1]; //root -> C

		dummyNode[0]->_left = dummyNode[2]; //B -> D
		dummyNode[0]->_right = dummyNode[3]; //B -> E

		dummyNode[1]->_left = dummyNode[4]; //C -> F
		dummyNode[1]->_right = dummyNode[5]; //C -> G


		std::cout << "\n전위 순회 (Root -> Left -> Right) : ";
		BT_DispOrderedTree(rootNode, TRAVERSAL_METHOD::PREORDER);
		std::cout << "\n중위 순회 (Left -> Root -> Right) : ";
		BT_DispOrderedTree(rootNode, TRAVERSAL_METHOD::INORDER);
		std::cout << "\n후위 순회 (Left -> Right -> Root) : ";
		BT_DispOrderedTree(rootNode, TRAVERSAL_METHOD::POSTORDER);
		std::cout << "\n";
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