#include "Core.h"

int main()
{
	char infixExpr[MAX_STR_LEN]; //중위 표현식
	char postfixExpr[MAX_STR_LEN]; //후위 표현식

	try
	{
		memset(infixExpr, '\0', MAX_STR_LEN);
		memset(postfixExpr, '\0', MAX_STR_LEN);

		printf("중위 표현식 입력 (최대 %d 자) : ", MAX_STR_LEN - 1);
		scanf_s("%s", infixExpr, MAX_STR_LEN - 1);

		GenPostfixExpr(infixExpr, postfixExpr);
		printf("중위 표현식 : %s\n후위 표현식 : %s\n", infixExpr, postfixExpr);
		//printf("계산 결과 : %f\n", CalcPostfixExpr(postfixExpr));
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	
	return EXIT_SUCCESS;
}