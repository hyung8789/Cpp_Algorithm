#include "Core.h"

static const int ELEMENT_COUNT = 500; //요소 개수
static const int TEST_PASSES = 10; //테스트 횟수
static const int LOGGING_LEVEL = 1; //로깅 레벨 (0 : 출력 안함, 1 : 간략한 내용, 2 : 상세 내용 (중간 과정 출력 위한 수행 시간 오차 발생))
static const bool VALIDATE_AFTER_SORT = true; //정렬 후 정렬 된 집합에 대한 유효성 검사 수행

//TODO : 각 bool에 대해 생성 및 future, join에 대해 처리 할 것
static const bool RUN_BUBBLE_SORT = true;
static const bool RUN_INSERTION_SORT = true;
static const bool RUN_QUICK_SORT = true;
static const bool RUN_SELECTION_SORT = true;
static const bool RUN_MERGE_SORT = true;

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
		MySortElementType* originData = new MySortElementType[ELEMENT_COUNT];
		MySortElementType* copiedData = new MySortElementType[ELEMENT_COUNT * TOTAL_SORT_FUNC_COUNT];
		TRACE_RESULT result[TOTAL_SORT_FUNC_COUNT];
		
		for (size_t i = 0; i < TEST_PASSES; i++)
		{
			GenRandPatternEnumerableSet<MySortElementType>(originData, ELEMENT_COUNT); 
			
			/***
				< 이미 정렬 되어 있는 상황 (Best Case) : 0 1 2 에 대한 오름차순 정렬의 비교 횟수 테스트 >

				1) ELEMENT_COUNT 를 3개로 할 것
				2) SequntialPattern 생성 시 ORDER_BY::ASCENDING 으로 할 것
				3) RunSinglePassSortTrace 의 내부 호출 순서를 ORDER_BY::ASCENDING 을 먼저 수행 할 것
				4) LOGGING_LEVEL을 2로 할 것
				5) 비교 과정 시 LOGGING_LEVEL == 2에 따른 중간에 비교 발생 내용을 출력 위해 수행 시간에 오차가 발생하므로, 수행 시간은 무시 할 것
			***/
			
			//GenSequentialPatternEnumerableSet<MySortElementType>(originData, ELEMENT_COUNT, ORDER_BY::ASCENDING);

			/***
				< 정렬하고자 하는 방법과 반대로 정렬 되어 있는 상황 (Worst Case) : 2 1 0 에 대한 오름차순 정렬의 비교 횟수 테스트 >
				
				1) ELEMENT_COUNT 를 3개로 할 것
				2) SequntialPattern 생성 시 ORDER_BY::DESCENDING 으로 할 것
				3) RunSinglePassSortTrace 의 내부 호출 순서를 ORDER_BY::ASCENDING 을 먼저 수행 할 것
				4) LOGGING_LEVEL 을 2로 할 것
				5) 비교 과정 시 LOGGING_LEVEL == 2에 따른 중간에 비교 발생 내용을 출력 위해 수행 시간에 오차가 발생하므로, 수행 시간은 무시 할 것
			***/

			//GenSequentialPatternEnumerableSet<MySortElementType>(originData, ELEMENT_COUNT, ORDER_BY::DESCENDING);
			
			for (size_t sortFuncIndex = 0; sortFuncIndex < TOTAL_SORT_FUNC_COUNT; sortFuncIndex++) //각 sort에서 사용하기 위해 원본 데이터 복사
			{
				memcpy_s(&copiedData[sortFuncIndex * ELEMENT_COUNT], sizeof(MySortElementType) * ELEMENT_COUNT,
					originData, sizeof(MySortElementType) * ELEMENT_COUNT);
			}

			std::promise<TRACE_RESULT> promiseArray[TOTAL_SORT_FUNC_COUNT];  //thread에 의해 결과가 저장 될 것이라는 약속
			std::future<TRACE_RESULT> futureArray[TOTAL_SORT_FUNC_COUNT]; //약속에 의해 미래에 thread로부터 결과를 받을 개체
			std::thread threadArray[TOTAL_SORT_FUNC_COUNT];
			for (size_t sortFuncIndex = 0; sortFuncIndex < TOTAL_SORT_FUNC_COUNT; sortFuncIndex++)
			{
				futureArray[sortFuncIndex] = promiseArray[sortFuncIndex].get_future();
				//TODO : 고유 사상 인덱스에 따라 내부에서 생성 할 것
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
			//오버로딩 된 템플릿 함수를 아래 thread 생성 시 컴파일러가 추론 할 수 없으므로, 컴파일 타임에 정적 캐스트
		
			std::thread quickSortThread(
				RunSinglePassSortTrace<MySortElementType>,
				"QuickSort",
				static_cast<void(*)(MySortElementType[], size_t, ORDER_BY)>(QuickSort<MySortElementType>),
				quickSortData, ELEMENT_COUNT,
				std::ref(promise3));
			
			//thread에 의해 결과가 반환되는 시점까지 대기하였다가 할당
			bubbleSortResult += future1.get();
			insertionSortResult += future2.get();
			quickSortResult += future3.get();

			//단일 pass에 대해 모든 thread가 종료 될 때까지 대기
			bubbleSortThread.join();
			insertionSortThread.join();
			quickSortThread.join();
		}

		/***
			이론 상 정렬하고자 하는 방법과 역순으로 정렬 된 데이터 (Worst Case)에 대해 정렬 시도 시
			Average Case 보다 오래 걸려야 정상이지만 CPU의 Branch Prediction에 의해 더 빠르게 나옴
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

	_CrtMemCheckpoint(&newState); //할당 해제 후 상태
	_CrtDumpMemoryLeaks();
	if (_CrtMemDifference(&lastState, &oldState, &newState))
		_CrtMemDumpStatistics(&lastState);

	system("pause");
	return EXIT_SUCCESS;
}