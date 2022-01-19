#include "LCRS_Core.h"

#define NODE_COUNT 5

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����

	try
	{
		Node* rootNode = NULL; //�ֻ��� ��Ʈ ���
		rootNode = LCRS_CreateNode(9999);

		Node* rootChildNode[NODE_COUNT] = { NULL, }; //�ֻ��� ��Ʈ ����� �ڽ� ���
		for (int i = 0; i < NODE_COUNT; i++) //�ֻ��� ��Ʈ ��Ʈ�� �ڽ� ��� ����
		{
			rootChildNode[i] = LCRS_CreateNode(i);
			LCRS_AddChildNode(rootNode, rootChildNode[i]);
		}

		for (int i = 0; i < NODE_COUNT; i++)
		{
			for (int j = 0; j < NODE_COUNT; j++) //�ֻ��� ��Ʈ ����� �� �ڽ� ���鿡 ���� �ٽ� �ڽ� ��� ����
			{
				LCRS_AddChildNode(rootChildNode[i], LCRS_CreateNode(rootChildNode[i]->data * 10));
			}
		}
		
		LCRS_DispTree(rootNode, 0);

		std::cout << "--- Ư�� ������ ��� ��� ��� ---\n";

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