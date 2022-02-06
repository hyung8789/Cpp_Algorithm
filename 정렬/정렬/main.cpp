#include "Core.h"

static const int ELEMENT_COUNT = 1000; //��� ����
static const int TEST_PASSES = 1; //�׽�Ʈ Ƚ��
static const int LOGGING_LEVEL = 1; //�α� ���� (0 : ��� ����, 1 : ������ ����, 2 : �� ���� (�߰� ���� ��� ���� ���� �ð� ���� �߻�))

static const bool RUN_BUBBLE_SORT = true;
static const bool RUN_INSERTION_SORT = true;
static const bool RUN_QUICK_SORT = true;
static const bool RUN_SELECTION_SORT = true;
static const bool RUN_MERGE_SORT = true;


#define SortElementType int

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����

	try
	{
		SortElementType* originData = new SortElementType[ELEMENT_COUNT];
		SortElementType* bubbleSortData = new SortElementType[ELEMENT_COUNT];
		SortElementType* insertionSortData = new SortElementType[ELEMENT_COUNT];
		SortElementType* quickSortData = new SortElementType[ELEMENT_COUNT];

		TRACE_RESULT bubbleSortResult, insertionSortResult, quickSortResult;

		for (size_t i = 0; i < TEST_PASSES; i++)
		{
			GenRandPatternEnumerableSet<SortElementType>(originData, ELEMENT_COUNT); 
			
			/***
				< �̹� ���� �Ǿ� �ִ� ��Ȳ (Best Case) : 0 1 2 �� ���� �������� ������ �� Ƚ�� �׽�Ʈ >

				1) ELEMENT_COUNT �� 3���� �� ��
				2) SequntialPattern ���� �� ORDER_BY::ASCENDING ���� �� ��
				3) RunSinglePassSortTrace �� ���� ȣ�� ������ ORDER_BY::ASCENDING �� ���� ���� �� ��
				4) LOGGING_LEVEL�� 2�� �� ��
				5) �� ���� �� LOGGING_LEVEL == 2�� ���� �߰��� �� �߻� ������ ��� ���� ���� �ð��� ������ �߻��ϹǷ�, ���� �ð��� ���� �� ��
			***/
			
			//GenSequentialPatternEnumerableSet<SortElementType>(originData, ELEMENT_COUNT, ORDER_BY::ASCENDING);

			/***
				< �����ϰ��� �ϴ� ����� �ݴ�� ���� �Ǿ� �ִ� ��Ȳ (Worst Case) : 2 1 0 �� ���� �������� ������ �� Ƚ�� �׽�Ʈ >
				
				1) ELEMENT_COUNT �� 3���� �� ��
				2) SequntialPattern ���� �� ORDER_BY::DESCENDING ���� �� ��
				3) RunSinglePassSortTrace �� ���� ȣ�� ������ ORDER_BY::ASCENDING �� ���� ���� �� ��
				4) LOGGING_LEVEL �� 2�� �� ��
				5) �� ���� �� LOGGING_LEVEL == 2�� ���� �߰��� �� �߻� ������ ��� ���� ���� �ð��� ������ �߻��ϹǷ�, ���� �ð��� ���� �� ��
			***/

			//GenSequentialPatternEnumerableSet<SortElementType>(originData, ELEMENT_COUNT, ORDER_BY::DESCENDING);

			memcpy_s(bubbleSortData, sizeof(SortElementType) * ELEMENT_COUNT, 
				originData, sizeof(SortElementType) * ELEMENT_COUNT);
			memcpy_s(insertionSortData, sizeof(SortElementType) * ELEMENT_COUNT, 
				originData, sizeof(SortElementType) * ELEMENT_COUNT);
			memcpy_s(quickSortData, sizeof(SortElementType) * ELEMENT_COUNT, 
				originData, sizeof(SortElementType) * ELEMENT_COUNT);

			std::promise<TRACE_RESULT> promise1, promise2, promise3; //thread�� ���� ����� ���� �� ���̶�� ���
			std::future<TRACE_RESULT>
				future1 = promise1.get_future(),
				future2 = promise2.get_future(),
				future3 = promise3.get_future(); //��ӿ� ���� �̷��� thread�κ��� ����� ���� ��ü

			std::thread bubbleSortThread(
				RunSinglePassSortTrace<SortElementType>,
				"BubbleSort",
				BubbleSort<SortElementType>, 
				bubbleSortData, ELEMENT_COUNT,
				std::ref(promise1));

			std::thread insertionSortThread(
				RunSinglePassSortTrace<SortElementType>,
				"InsertionSort",
				InsertionSort<SortElementType>, 
				insertionSortData, ELEMENT_COUNT,
				std::ref(promise2));

			// https://stackoverflow.com/questions/44049407/c-compilation-fails-on-calling-overloaded-function-in-stdthread
			//�����ε� �� ���ø� �Լ��� �Ʒ� thread ���� �� �����Ϸ��� �߷� �� �� �����Ƿ�, ������ Ÿ�ӿ� ���� ĳ��Ʈ
			std::thread quickSortThread(
				RunSinglePassSortTrace<SortElementType>,
				"QuickSort",
				static_cast<void(*)(SortElementType[], size_t, ORDER_BY)>(QuickSort<SortElementType>),
				quickSortData, ELEMENT_COUNT,
				std::ref(promise3));
			
			//thread�� ���� ����� ��ȯ�Ǵ� �������� ����Ͽ��ٰ� �Ҵ�
			bubbleSortResult += future1.get();
			insertionSortResult += future2.get();
			quickSortResult += future3.get();

			//���� pass�� ���� ��� thread�� ���� �� ������ ���
			bubbleSortThread.join();
			insertionSortThread.join();
			quickSortThread.join();
		}

		/***
			�̷� �� �����ϰ��� �ϴ� ����� �������� ���� �� ������ (Worst Case)�� ���� ���� �õ� ��
			Average Case ���� ���� �ɷ��� ���������� CPU�� Branch Prediction�� ���� �� ������ ����
		***/

		bubbleSortResult.DispTotalTestPassTraceResult("BubbleSort", TEST_PASSES);
		insertionSortResult.DispTotalTestPassTraceResult("InsertionSort", TEST_PASSES);
		quickSortResult.DispTotalTestPassTraceResult("QuickSort", TEST_PASSES);

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