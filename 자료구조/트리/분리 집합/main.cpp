#include "DS_Core.h"

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����

	try
	{
		/***
			< ���� ���� >

					1
				2
			3

			< ���� ���հ� ���� �� ���� >

				4
			5

			< ���� �� �� ���� >

					1
				2		4
			3				5
		***/

		int dummy[6] = { 0,1,2,3,4,5 };
		Node* dummyNode[6] = { NULL, };

		Node* originSet = NULL; //���� ����
		Node* newSet = NULL; //������ ���� �� ����

		std::cout << "--- ���� ���� ---" << std::endl;
		for (int i = 1; i <= 3; i++) //���� ����
		{
			dummyNode[i] = DS_CreateNode((int*)dummy[i]);
			DS_UnionSet(&originSet, dummyNode[i]);
			std::cout << (int)dummyNode[i]->data << std::endl;
		}
		std::cout << "���� ������ �ֻ��� �θ� ��� : "<< (int)DS_FindSet(dummyNode[1])->data << std::endl;

		std::cout << "\n--- ���� ���հ� ���� �� ���� ---" << std::endl;
		for (int i = 4; i <= 5; i++) //������ ���� �� ����
		{
			dummyNode[i] = DS_CreateNode((int*)dummy[i]);
			DS_UnionSet(&newSet, dummyNode[i]);
			std::cout << (int)dummyNode[i]->data << std::endl;
		}
		std::cout << "���� ���հ� ���� �� ������ �ֻ��� �θ� ��� : " << (int)DS_FindSet(dummyNode[4])->data << std::endl;

		DS_UnionSet(&originSet, newSet);
		std::cout << "\n���� �� �� ������ �ֻ��� �θ� ��� : " << (int)DS_FindSet(dummyNode[4])->data << std::endl;
		
		for(int i=0;i<6;i++)
			DS_DeallocateNode(&dummyNode[i]);
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