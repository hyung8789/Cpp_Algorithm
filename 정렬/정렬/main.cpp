#include "Core.h"

static const int ELEMENT_COUNT = 10000; //��� ����
static const int TEST_PASSES = 2; //�׽�Ʈ Ƚ��
static const int LOGGING_LEVEL = 1; //�α� ���� (0 : ��� ����, 1 : ������ ����, 2 : �� ����)

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����

	try
	{
		//TODO : branch prediction ȸ�� volatile �׽�Ʈ

		int* originData = new int[ELEMENT_COUNT];
		int* bubbleSortData = new int[ELEMENT_COUNT];
		int* insertionSortData = new int[ELEMENT_COUNT];
		int* quickSortData = new int[ELEMENT_COUNT];

		TRACE_RESULT bubbleSortResult, insertionSortResult, quickSortResult;
		std::mutex mutex; //����׿� ��� ��� ���� ���ؽ�

		for (size_t i = 0; i < TEST_PASSES; i++)
		{
			RandGenEnumerableSet<int>(originData, ELEMENT_COUNT); //�� pass �� ���� ����
			memcpy(bubbleSortData, originData, sizeof(int) * ELEMENT_COUNT);
			memcpy(insertionSortData, originData, sizeof(int) * ELEMENT_COUNT);
			memcpy(quickSortData, originData, sizeof(int) * ELEMENT_COUNT);

			std::promise<TRACE_RESULT> promise1, promise2, promise3; //thread�� ���� ����� ���� �� ���̶�� ���
			std::future<TRACE_RESULT>
				future1 = promise1.get_future(),
				future2 = promise2.get_future();
			//future3 = promise3.get_future(); //��ӿ� ���� �̷��� thread�κ��� ����� ���� ��ü

			std::thread bubbleSortThread(
				RunSinglePassSortTrace<int>,
				"���� ����",
				BubbleSort<int>, bubbleSortData, ELEMENT_COUNT,
				std::ref(promise1), std::ref(mutex));

			std::thread insertionSortThread(
				RunSinglePassSortTrace<int>,
				"���� ����",
				InsertionSort<int>, insertionSortData, ELEMENT_COUNT,
				std::ref(promise2), std::ref(mutex));

			//thread�� ���� ����� ��ȯ�Ǵ� �������� ����Ͽ��ٰ� �Ҵ�
			bubbleSortResult += future1.get();
			insertionSortResult += future2.get();

			//���� pass�� ���� ��� thread�� ���� �� ������ ���
			bubbleSortThread.join();
			insertionSortThread.join();
		}

		/***
			�̷� �� �����ϰ��� �ϴ� ����� �������� ���� �� ������ (Worst Case)�� ���� ���� �õ� ��
			Average Case ���� ���� �ɷ��� ���������� CPU�� Branch Prediction�� ���� �� ������ ����
		***/

		bubbleSortResult.dispTraceResult("���� ����", TEST_PASSES);
		insertionSortResult.dispTraceResult("���� ����", TEST_PASSES);

		delete[](originData);
		delete[](bubbleSortData);
		delete[](insertionSortData);
		delete[](quickSortData);
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