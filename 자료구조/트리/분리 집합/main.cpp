#include "DS_Core.h"

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
		/***
			< 원본 집합 >

					1
				2
			3

			< 원본 집합과 통합 할 집합 >

				4
			5

			< 통합 된 후 집합 >

					1
				2		4
			3				5
		***/

		int dummy[6] = { 0,1,2,3,4,5 };
		Node* dummyNode[6] = { NULL, };

		Node* originSet = NULL; //원본 집합
		Node* newSet = NULL; //원본과 통합 할 집합

		std::cout << "--- 원본 집합 ---" << std::endl;
		for (int i = 1; i <= 3; i++) //원본 집합
		{
			dummyNode[i] = DS_CreateNode((int*)dummy[i]);
			DS_UnionSet(&originSet, dummyNode[i]);
			std::cout << (int)dummyNode[i]->data << std::endl;
		}
		std::cout << "원본 집합의 최상위 부모 노드 : "<< (int)DS_FindSet(dummyNode[1])->data << std::endl;

		std::cout << "\n--- 원본 집합과 통합 할 집합 ---" << std::endl;
		for (int i = 4; i <= 5; i++) //원본과 통합 할 집합
		{
			dummyNode[i] = DS_CreateNode((int*)dummy[i]);
			DS_UnionSet(&newSet, dummyNode[i]);
			std::cout << (int)dummyNode[i]->data << std::endl;
		}
		std::cout << "원본 집합과 통합 할 집합의 최상위 부모 노드 : " << (int)DS_FindSet(dummyNode[4])->data << std::endl;

		DS_UnionSet(&originSet, newSet);
		std::cout << "\n통합 된 후 집합의 최상위 부모 노드 : " << (int)DS_FindSet(dummyNode[4])->data << std::endl;
		
		for(int i=0;i<6;i++)
			DS_DeallocateNode(&dummyNode[i]);
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