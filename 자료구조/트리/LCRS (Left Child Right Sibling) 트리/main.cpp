#include "LCRS_Core.h"

#define NODE_COUNT 5

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
		Node* rootNode = NULL; //최상위 루트 노드
		rootNode = LCRS_CreateNode('A');

		Node* rootChildNode[NODE_COUNT] = { NULL, }; //최상위 루트 노드의 자식 노드
		for (int i = 0; i < NODE_COUNT; i++) //최상위 루트 노트에 자식 노드 연결
		{
			rootChildNode[i] = LCRS_CreateNode((char)(66 + i)); //B부터 할당
			LCRS_AddChildNode(rootNode, rootChildNode[i]);
		}

		for (int i = 0; i < NODE_COUNT; i++)
		{
			for (int j = 0; j < NODE_COUNT; j++) //최상위 루트 노드의 각 자식 노드들에 대해 다시 자식 노드 연결
			{
				LCRS_AddChildNode(rootChildNode[i], LCRS_CreateNode((char)(98 + i))); //b부터 할당
			}
		}

		LCRS_DispTree(rootNode);

		TreeDepthType TARGET_DEPTH = 2; //탐색 할 깊이
		std::cout << "--- 깊이 " << TARGET_DEPTH << "에 있는 전체 노드 목록 출력 --- \n";
		LCRS_DispTreeNodesAt(rootNode, TARGET_DEPTH);

		LCRS_DeallocateTree(&rootNode);
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