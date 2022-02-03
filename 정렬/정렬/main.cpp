#include "Core.h"

static const int ELEMENT_COUNT = 10; //요소 개수
static const int TEST_PASSES = 10; //테스트 횟수
static const bool LOGGING_DEBUG_RESULT = false; //디버그용 결과 출력

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
		/***
			이론 상 정렬하고자 하는 방법과 역순으로 정렬 된 데이터 (Worst Case)에 대해 정렬 시도 시
			Average Case 보다 오래 걸려야 정상이지만 CPU의 Branch Prediction에 의해 더 빠르게 나옴
		***/

		int originData[ELEMENT_COUNT]; //원본 데이터
		int bubbleSortData[ELEMENT_COUNT], insertionSortData[ELEMENT_COUNT], quickSortData[ELEMENT_COUNT];
		
		TRACE_RESULT bubbleSortResult, insertionSortResult, quickSortResult;
		std::mutex mutex; //디버그용 결과 출력 위한 뮤텍스
		
		for (size_t i = 0; i < TEST_PASSES; i++)
		{
			RandGenEnumerableSet<int>(originData, ELEMENT_COUNT); //각 pass 간 새로 생성
			memcpy(bubbleSortData, originData, sizeof(int) * ELEMENT_COUNT);
			memcpy(insertionSortData, originData, sizeof(int) * ELEMENT_COUNT);
			memcpy(quickSortData, originData, sizeof(int) * ELEMENT_COUNT);

			std::promise<TRACE_RESULT> p1, p2, p3; //thread에 의해 결과가 저장 될 것이라는 약속
			std::future<TRACE_RESULT>
				f1 = p1.get_future(),
				f2 = p2.get_future();
				//f3 = p3.get_future(); //미래의 thread 종료 시점에 약속에 의해 결과를 받을 개체

			std::thread bubbleSortThread(
				RunSinglePassSortTrace<int>, 
				"버블 정렬",
				BubbleSort<int>, bubbleSortData, ELEMENT_COUNT,
				std::ref(p1), std::ref(mutex));

			std::thread insertionSortThread(
				RunSinglePassSortTrace<int>, 
				"삽입 정렬",
				InsertionSort<int>, insertionSortData, ELEMENT_COUNT,
				std::ref(p2), std::ref(mutex));

			//단일 pass에 대해 모든 스레드가 종료 될 떄까지 대기
			bubbleSortThread.join();
			insertionSortThread.join();
		
			bubbleSortResult += f1.get();
			insertionSortResult += f2.get();
		
		}
		
		bubbleSortResult.dispTraceResult("버블 정렬", TEST_PASSES);
		insertionSortResult.dispTraceResult("삽입 정렬", TEST_PASSES);
	
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