#include "Core.h"

int main()
{
	char infixExpr[MAX_STR_LEN]; //중위 표현식
	char postfixExpr[MAX_STR_LEN]; //후위 표현식

	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
		memset(infixExpr, '\0', MAX_STR_LEN);
		memset(postfixExpr, '\0', MAX_STR_LEN);

		printf("중위 표현식 입력 (최대 %d 자) : ", MAX_STR_LEN - 1);
		scanf_s("%[^\n]", infixExpr, MAX_STR_LEN - 1);

		GenPostfixExpr(infixExpr, postfixExpr);
		printf("중위 표현식 : %s\n후위 표현식 : %s\n", infixExpr, postfixExpr);
		printf("계산 결과 : %f\n", CalcPostfixExpr(postfixExpr));
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