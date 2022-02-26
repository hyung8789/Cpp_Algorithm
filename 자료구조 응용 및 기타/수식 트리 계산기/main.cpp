#include "Core.h"

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����

	char postfixExpr[MAX_STR_LEN]; //���� ǥ����
	NODE* rootNode = NULL;

	try
	{
		memset(postfixExpr, '\0', MAX_STR_LEN);

		printf("���� ǥ���� �Է� (�ִ� %d ��, �ǿ����� �� ������ ���� ���� ����) : ", MAX_STR_LEN - 1);
		scanf_s("%[^\n]", postfixExpr, MAX_STR_LEN - 1);

		EXPRT_BulidTreeFromPostfixExpr(&rootNode, postfixExpr);

		std::cout << "\n���� ��ȸ : ";
		EXPRT_DispOrderedTree(rootNode, TRAVERSAL_METHOD::PREORDER);
		std::cout << "\n���� ��ȸ : ";
		EXPRT_DispOrderedTree(rootNode, TRAVERSAL_METHOD::INORDER);
		std::cout << "\n���� ��ȸ : ";
		EXPRT_DispOrderedTree(rootNode, TRAVERSAL_METHOD::POSTORDER);

		printf("\n��� ��� : %f\n", EXPRT_EvaluateTree(rootNode));
		EXPRT_DeallocateTree(&rootNode);
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