#include "Core.h"

static const int ELEMENT_COUNT = 1000; //��� ����
static const int TEST_PASSES = 10; //�׽�Ʈ Ƚ��
static const int LOGGING_LEVEL = 0; //�α� ���� (0 : ��� ����, 1 : ������ ����, 2 : �� ���� (�߰� ���� ��� ���� ���� �ð� ���� �߻�))
static const bool VALIDATE_AFTER_SORT = true; //���� �� ���� �� ���տ� ���� ��ȿ�� �˻� ����

int main()
{
	_CrtMemState oldState, newState, lastState;
	_CrtMemCheckpoint(&oldState); //�Ҵ� �� ����

	try
	{
		const int totalSortFuncCount = (const int)SORT_UNIQUE_MAPPED_INDEX::TOTAL_SORT_FUNC_COUNT;

		MySortElementType* originData = new MySortElementType[ELEMENT_COUNT];
		MySortElementType* copiedData = new MySortElementType[ELEMENT_COUNT * totalSortFuncCount];
		
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
			
			for (int i = 0; i < totalSortFuncCount; i++) //�� sort���� ����ϱ� ���� ���� ������ ����
			{
				memcpy_s(&copiedData[i * ELEMENT_COUNT], sizeof(MySortElementType) * ELEMENT_COUNT,
					originData, sizeof(MySortElementType) * ELEMENT_COUNT);
			}

			std::promise<TRACE_RESULT> promiseArray[totalSortFuncCount]; //thread�� ���� ����� ���� �� ���̶�� ���
			std::future<TRACE_RESULT> futureArray[totalSortFuncCount]; //��ӿ� ���� �̷��� thread�κ��� ����� ���� ��ü
			std::thread threadArray[totalSortFuncCount];
			
			for (int i = 0; i < totalSortFuncCount; i++)
			{
				futureArray[i] = promiseArray[i].get_future(); //������κ��� �̷� ���

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

				//thread�� ���� ����� ��ȯ�Ǵ� �������� ����Ͽ��ٰ� �Ҵ� (�ҿ� �ð� ���� ���)
				SORT_MAPPER::GetInstance().GetRefTraceResult(sortUniqueMappedIndex) += futureArray[i].get();

				//���� pass�� ���� �ش� thread�� ������ ���� �� ������ ���
				threadArray[i].join();
			}
		}

		/***
			�̷� �� �����ϰ��� �ϴ� ����� �������� ���� �� ������ (Worst Case)�� ���� ���� �õ� ��
			Average Case ���� ���� �ɷ��� ���������� CPU�� Branch Prediction�� ���� �� ������ ����
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

	_CrtMemCheckpoint(&newState); //�Ҵ� ���� �� ����
	_CrtDumpMemoryLeaks();
	if (_CrtMemDifference(&lastState, &oldState, &newState))
		_CrtMemDumpStatistics(&lastState);

	system("pause");
	return EXIT_SUCCESS;
}