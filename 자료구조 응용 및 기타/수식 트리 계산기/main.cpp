#include "Core.h"

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	char postfixExpr[MAX_STR_LEN]; //후위 표현식
	NODE* rootNode = NULL;

	try
	{
		memset(postfixExpr, '\0', MAX_STR_LEN);

		printf("후위 표현식 입력 (최대 %d 자, 피연산자 간 구분을 위한 공백 포함) : ", MAX_STR_LEN - 1);
		scanf_s("%[^\n]", postfixExpr, MAX_STR_LEN - 1);

		EXPRT_BulidTreeFromPostfixExpr(&rootNode, postfixExpr);

		std::cout << "\n전위 순회 : ";
		EXPRT_DispOrderedTree(rootNode, TRAVERSAL_METHOD::PREORDER);
		std::cout << "\n중위 순회 : ";
		EXPRT_DispOrderedTree(rootNode, TRAVERSAL_METHOD::INORDER);
		std::cout << "\n후위 순회 : ";
		EXPRT_DispOrderedTree(rootNode, TRAVERSAL_METHOD::POSTORDER);

		printf("\n계산 결과 : %f\n", EXPRT_EvaluateTree(rootNode));
		EXPRT_DeallocateTree(&rootNode);
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