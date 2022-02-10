#include "Core.h"

static const int ELEMENT_COUNT = 500; //��� ����
static const int TEST_PASSES = 10; //�׽�Ʈ Ƚ��
static const int LOGGING_LEVEL = 1; //�α� ���� (0 : ��� ����, 1 : ������ ����, 2 : �� ���� (�߰� ���� ��� ���� ���� �ð� ���� �߻�))
static const bool VALIDATE_AFTER_SORT = true; //���� �� ���� �� ���տ� ���� ��ȿ�� �˻� ����

//TODO : �� bool�� ���� ���� �� future, join�� ���� ó�� �� ��
static const bool RUN_BUBBLE_SORT = true;
static const bool RUN_INSERTION_SORT = true;
static const bool RUN_QUICK_SORT = true;
static const bool RUN_SELECTION_SORT = true;
static const bool RUN_MERGE_SORT = true;

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����

	try
	{
		MySortElementType* originData = new MySortElementType[ELEMENT_COUNT];
		MySortElementType* copiedData = new MySortElementType[ELEMENT_COUNT * TOTAL_SORT_FUNC_COUNT];
		TRACE_RESULT result[TOTAL_SORT_FUNC_COUNT];
		
		for (size_t i = 0; i < TEST_PASSES; i++)
		{
			GenRandPatternEnumerableSet<MySortElementType>(originData, ELEMENT_COUNT); 
			
			/***
				< �̹� ���� �Ǿ� �ִ� ��Ȳ (Best Case) : 0 1 2 �� ���� �������� ������ �� Ƚ�� �׽�Ʈ >

				1) ELEMENT_COUNT �� 3���� �� ��
				2) SequntialPattern ���� �� ORDER_BY::ASCENDING ���� �� ��
				3) RunSinglePassSortTrace �� ���� ȣ�� ������ ORDER_BY::ASCENDING �� ���� ���� �� ��
				4) LOGGING_LEVEL�� 2�� �� ��
				5) �� ���� �� LOGGING_LEVEL == 2�� ���� �߰��� �� �߻� ������ ��� ���� ���� �ð��� ������ �߻��ϹǷ�, ���� �ð��� ���� �� ��
			***/
			
			//GenSequentialPatternEnumerableSet<MySortElementType>(originData, ELEMENT_COUNT, ORDER_BY::ASCENDING);

			/***
				< �����ϰ��� �ϴ� ����� �ݴ�� ���� �Ǿ� �ִ� ��Ȳ (Worst Case) : 2 1 0 �� ���� �������� ������ �� Ƚ�� �׽�Ʈ >
				
				1) ELEMENT_COUNT �� 3���� �� ��
				2) SequntialPattern ���� �� ORDER_BY::DESCENDING ���� �� ��
				3) RunSinglePassSortTrace �� ���� ȣ�� ������ ORDER_BY::ASCENDING �� ���� ���� �� ��
				4) LOGGING_LEVEL �� 2�� �� ��
				5) �� ���� �� LOGGING_LEVEL == 2�� ���� �߰��� �� �߻� ������ ��� ���� ���� �ð��� ������ �߻��ϹǷ�, ���� �ð��� ���� �� ��
			***/

			//GenSequentialPatternEnumerableSet<MySortElementType>(originData, ELEMENT_COUNT, ORDER_BY::DESCENDING);
			
			for (size_t sortFuncIndex = 0; sortFuncIndex < TOTAL_SORT_FUNC_COUNT; sortFuncIndex++) //�� sort���� ����ϱ� ���� ���� ������ ����
			{
				memcpy_s(&copiedData[sortFuncIndex * ELEMENT_COUNT], sizeof(MySortElementType) * ELEMENT_COUNT,
					originData, sizeof(MySortElementType) * ELEMENT_COUNT);
			}

			std::promise<TRACE_RESULT> promiseArray[TOTAL_SORT_FUNC_COUNT];  //thread�� ���� ����� ���� �� ���̶�� ���
			std::future<TRACE_RESULT> futureArray[TOTAL_SORT_FUNC_COUNT]; //��ӿ� ���� �̷��� thread�κ��� ����� ���� ��ü
			std::thread threadArray[TOTAL_SORT_FUNC_COUNT];
			for (size_t sortFuncIndex = 0; sortFuncIndex < TOTAL_SORT_FUNC_COUNT; sortFuncIndex++)
			{
				futureArray[sortFuncIndex] = promiseArray[sortFuncIndex].get_future();
				//TODO : ���� ��� �ε����� ���� ���ο��� ���� �� ��
			}

			
			std::thread sortThread(RunSinglePassSortTrace<MySortElementType>,
				UniqueMappedIndexToSortFuncNameStr(sortFuncIndex),
				BubbleSort<MySortElementType>,
				copiedData[SortFuncNameStrToUniqueMappedIndex("BubbleSort") * ELEMENT_COUNT], ELEMENT_COUNT,
				std::ref(promiseArray[]));

			std::thread insertionSortThread(
				RunSinglePassSortTrace<MySortElementType>,
				"InsertionSort",
				InsertionSort<MySortElementType>, 
				insertionSortData, ELEMENT_COUNT,
				std::ref(promise2));

			// https://stackoverflow.com/questions/44049407/c-compilation-fails-on-calling-overloaded-function-in-stdthread
			//�����ε� �� ���ø� �Լ��� �Ʒ� thread ���� �� �����Ϸ��� �߷� �� �� �����Ƿ�, ������ Ÿ�ӿ� ���� ĳ��Ʈ
		
			std::thread quickSortThread(
				RunSinglePassSortTrace<MySortElementType>,
				"QuickSort",
				static_cast<void(*)(MySortElementType[], size_t, ORDER_BY)>(QuickSort<MySortElementType>),
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