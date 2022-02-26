#include "LCRS_Core.h"

#define NODE_COUNT 5

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����

	try
	{
		NODE* rootNode = NULL; //�ֻ��� ��Ʈ ���
		rootNode = LCRS_CreateNode('A');

		NODE* rootChildNode[NODE_COUNT] = { NULL, }; //�ֻ��� ��Ʈ ����� �ڽ� ���
		for (int i = 0; i < NODE_COUNT; i++) //�ֻ��� ��Ʈ ��Ʈ�� �ڽ� ��� ����
		{
			rootChildNode[i] = LCRS_CreateNode((char)(66 + i)); //B���� �Ҵ�
			LCRS_AppendNode(rootNode, rootChildNode[i]);
		}

		for (int i = 0; i < NODE_COUNT; i++)
		{
			for (int j = 0; j < NODE_COUNT; j++) //�ֻ��� ��Ʈ ����� �� �ڽ� ���鿡 ���� �ٽ� �ڽ� ��� ����
			{
				LCRS_AppendNode(rootChildNode[i], LCRS_CreateNode((char)(98 + i))); //b���� �Ҵ�
			}
		}

		LCRS_DispTree(rootNode);

		TREE_DEPTH_TYPE TARGET_DEPTH = 2; //Ž�� �� ����
		std::cout << "--- ���� " << TARGET_DEPTH << "�� �ִ� ��ü ��� ��� ��� --- \n";
		LCRS_DispTreeNodesAt(rootNode, TARGET_DEPTH);

		LCRS_DeallocateTree(&rootNode);
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