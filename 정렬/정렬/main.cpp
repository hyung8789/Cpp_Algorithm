#include "Core.h"

static const int ELEMENT_COUNT = 1000; //요소 개수
static const int TEST_PASSES = 10; //테스트 횟수
static const int LOGGING_LEVEL = 0; //로깅 레벨 (0 : 출력 안함, 1 : 간략한 내용, 2 : 상세 내용 (중간 과정 출력 위한 수행 시간 오차 발생))
static const bool VALIDATE_AFTER_SORT = true; //정렬 후 정렬 된 집합에 대한 유효성 검사 수행

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //할당 전 상태

	try
	{
		const int totalSortFuncCount = (const int)SORT_UNIQUE_MAPPED_INDEX::TOTAL_SORT_FUNC_COUNT;

		MySortElementType* originData = new MySortElementType[ELEMENT_COUNT];
		MySortElementType* copiedData = new MySortElementType[ELEMENT_COUNT * totalSortFuncCount];
		
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
			
			for (int i = 0; i < totalSortFuncCount; i++) //각 sort에서 사용하기 위해 원본 데이터 복사
			{
				memcpy_s(&copiedData[i * ELEMENT_COUNT], sizeof(MySortElementType) * ELEMENT_COUNT,
					originData, sizeof(MySortElementType) * ELEMENT_COUNT);
			}

			std::promise<TRACE_RESULT> promiseArray[totalSortFuncCount]; //thread에 의해 결과가 저장 될 것이라는 약속
			std::future<TRACE_RESULT> futureArray[totalSortFuncCount]; //약속에 의해 미래에 thread로부터 결과를 받을 개체
			std::thread threadArray[totalSortFuncCount];
			
			for (int i = 0; i < totalSortFuncCount; i++)
			{
				futureArray[i] = promiseArray[i].get_future(); //약속으로부터 미래 사상

				SORT_UNIQUE_MAPPED_INDEX sortUniqueMappedIndex = (SORT_UNIQUE_MAPPED_INDEX)i;

				threadArray[i] = std::thread(RunSinglePassSortTrace<MySortElementType>,
					SORT_MAPPER::GetInstance().SortUniqueMappedIndexToSortFuncNameStr(sortUniqueMappedIndex),
					SORT_MAPPER::GetInstance().GetRefSortMetaData(sortUniqueMappedIndex)._sortFuncAddr,
					&copiedData[i * ELEMENT_COUNT], ELEMENT_COUNT,
					std::ref(promiseArray[i]));
			}

			for (int i = 0; i < totalSortFuncCount; i++)
			{
				SORT_UNIQUE_MAPPED_INDEX sortUniqueMappedIndex = (SORT_UNIQUE_MAPPED_INDEX)i;

				//thread에 의해 결과가 반환되는 시점까지 대기하였다가 할당 (소요 시간 누적 계산)
				SORT_MAPPER::GetInstance().GetRefTraceResult(sortUniqueMappedIndex) += futureArray[i].get();

				//단일 pass에 대해 해당 thread가 완전히 종료 될 때까지 대기
				threadArray[i].join();
			}
		}

		/***
			이론 상 정렬하고자 하는 방법과 역순으로 정렬 된 데이터 (Worst Case)에 대해 정렬 시도 시
			Average Case 보다 오래 걸려야 정상이지만 CPU의 Branch Prediction에 의해 더 빠르게 나옴
		***/

		for (size_t i = 0; i < totalSortFuncCount; i++)
		{
			SORT_UNIQUE_MAPPED_INDEX sortUniqueMappedIndex = (SORT_UNIQUE_MAPPED_INDEX)i;
			const char* sortFuncNameStr = SORT_MAPPER::GetInstance().SortUniqueMappedIndexToSortFuncNameStr(sortUniqueMappedIndex);
			SORT_METADATA& sortMetaData = SORT_MAPPER::GetInstance().GetRefSortMetaData(sortUniqueMappedIndex);

			sortMetaData._traceResult.DispTotalTestPassTraceResult(sortFuncNameStr, TEST_PASSES);
		}

		delete[](originData);
		delete[](copiedData);

		SORT_MAPPER::GetInstance().Dispose();
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