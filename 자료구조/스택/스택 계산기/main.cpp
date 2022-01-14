#include "Core.h"

int main()
{
	char infixExpr[MAX_STR_LEN]; //���� ǥ����
	char postfixExpr[MAX_STR_LEN]; //���� ǥ����

	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����

	try
	{
		memset(infixExpr, '\0', MAX_STR_LEN);
		memset(postfixExpr, '\0', MAX_STR_LEN);

		printf("���� ǥ���� �Է� (�ִ� %d ��) : ", MAX_STR_LEN - 1);
		scanf_s("%[^\n]", infixExpr, MAX_STR_LEN - 1);

		GenPostfixExpr(infixExpr, postfixExpr);
		printf("���� ǥ���� : %s\n���� ǥ���� : %s\n", infixExpr, postfixExpr);
		printf("��� ��� : %f\n", CalcPostfixExpr(postfixExpr));
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