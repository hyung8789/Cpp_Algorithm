#include "Core.h"

static const int ELEMENT_COUNT = 10; //��� ����
static const int TEST_PASSES = 10; //�׽�Ʈ Ƚ��
static const bool LOGGING_DEBUG_RESULT = false; //����׿� ��� ���

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����

	try
	{
		/***
			�̷� �� �����ϰ��� �ϴ� ����� �������� ���� �� ������ (Worst Case)�� ���� ���� �õ� ��
			Average Case ���� ���� �ɷ��� ���������� CPU�� Branch Prediction�� ���� �� ������ ����
		***/

		int originData[ELEMENT_COUNT]; //���� ������
		int bubbleSortData[ELEMENT_COUNT], insertionSortData[ELEMENT_COUNT], quickSortData[ELEMENT_COUNT];
		
		TRACE_RESULT bubbleSortResult, insertionSortResult, quickSortResult;
		std::mutex mutex; //����׿� ��� ��� ���� ���ؽ�
		
		for (size_t i = 0; i < TEST_PASSES; i++)
		{
			RandGenEnumerableSet<int>(originData, ELEMENT_COUNT); //�� pass �� ���� ����
			memcpy(bubbleSortData, originData, sizeof(int) * ELEMENT_COUNT);
			memcpy(insertionSortData, originData, sizeof(int) * ELEMENT_COUNT);
			memcpy(quickSortData, originData, sizeof(int) * ELEMENT_COUNT);

			std::promise<TRACE_RESULT> p1, p2, p3; //thread�� ���� ����� ���� �� ���̶�� ���
			std::future<TRACE_RESULT>
				f1 = p1.get_future(),
				f2 = p2.get_future();
				//f3 = p3.get_future(); //�̷��� thread ���� ������ ��ӿ� ���� ����� ���� ��ü

			std::thread bubbleSortThread(
				RunSinglePassSortTrace<int>, 
				"���� ����",
				BubbleSort<int>, bubbleSortData, ELEMENT_COUNT,
				std::ref(p1), std::ref(mutex));

			std::thread insertionSortThread(
				RunSinglePassSortTrace<int>, 
				"���� ����",
				InsertionSort<int>, insertionSortData, ELEMENT_COUNT,
				std::ref(p2), std::ref(mutex));

			//���� pass�� ���� ��� �����尡 ���� �� ������ ���
			bubbleSortThread.join();
			insertionSortThread.join();
		
			bubbleSortResult += f1.get();
			insertionSortResult += f2.get();
		
		}
		
		bubbleSortResult.dispTraceResult("���� ����", TEST_PASSES);
		insertionSortResult.dispTraceResult("���� ����", TEST_PASSES);
	
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