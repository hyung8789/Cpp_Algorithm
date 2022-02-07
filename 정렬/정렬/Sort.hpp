#ifndef _SORT_HPP_
#define _SORT_HPP_

#define LENGTH(array) ((sizeof(array)) / (sizeof(array[0])))
#define COMPARE(x, y) ((x) > (y) ? 1 : (x) == (y) ? 0 : -1) //x > y : 1, x == y : 0, x < y : -1
#define SWAP(x, y, tmp) ((tmp) = (x), (x) = (y), (y) = (tmp))

#define RECURSIVE_METHOD //재귀적 방법 (주석 처리 : 반복적 방법 사용)
#ifndef RECURSIVE_METHOD 
#define ITERATIVE_METHOD //반복적 방법
#endif

#define LOGGING_COMPARE_COUNT //비교 횟수 카운트 출력 (주석 처리 : 비활성화)
#ifdef LOGGING_COMPARE_COUNT

extern const int LOGGING_LEVEL;
extern std::mutex mutex;

//TODO : XML 미 입력 주석 수정, 일부 잘못 된 XML 주석 발견 전체 재확인
//TODO : 가능 할 경우 이하 비교 횟수 카운트를 TRACE_RESULT와 통합 혹은 별도의 개체로 분리 할 것
//TODO : 주석에서 하위 설명 : 로 된거 depth를 늘려서 줄바꿈 줄이기 (Partion 참조)
//TODO : 정렬 함수 내 주석을 간소화 할것

/***
	< 정렬 성능 비교 >

***/

static size_t compareCount[3] = { 0, }; //비교 횟수 카운트

/// <summary>
/// 정렬 함수 이름 문자열을 비교 횟수 카운트의 인덱스로 변환
/// </summary>
/// <param name="sortFuncNameStr">정렬 함수 이름 문자열</param>
/// <returns>정렬 함수 이름 문자열로부터 변환 된 비교 횟수 카운트의 인덱스</returns>
size_t SortFuncNameStrToCompareCountIndex(const char* sortFuncNameStr)
{
	//TODO : promise future, traceResult 배열화, 정렬 함수명에 따라 공통 인덱스 사상 함수 만들것
	return strcmp(sortFuncNameStr, "BubbleSort") == 0 ? 0 :
		strcmp(sortFuncNameStr, "InsertionSort") == 0 ? 1 :
		strcmp(sortFuncNameStr, "QuickSort") == 0 ? 2 :
		strcmp(sortFuncNameStr, "PartitioningProc") == 0 ? 2 : //퀵 소트에서 호출
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));
}

/// <summary>
/// 현재 비교 횟수 초기화
/// </summary>
void ClearCompareCount()
{
	memset(compareCount, 0, sizeof(size_t) * LENGTH(compareCount));
}

/// <summary>
/// 정렬에 따른 현재 비교 횟수 출력
/// </summary>
/// <param name="sortFuncNameStr">정렬 함수 이름 문자열</param>
void DispCompareCount(const char* sortFuncNameStr)
{
	std::cout << ">> " << sortFuncNameStr << "의 총 비교 횟수 : " << \
		compareCount[SortFuncNameStrToCompareCountIndex(sortFuncNameStr)] << "회" << std::endl;
}

/// <summary>
/// 정렬에 따른 현재 비교 횟수 반환
/// </summary>
/// <param name="sortFuncNameStr"></param>
/// <returns></returns>
size_t GetCompareCount(const char* sortFuncNameStr)
{
	return compareCount[SortFuncNameStrToCompareCountIndex(sortFuncNameStr)];
}

/// <summary>
/// 정렬에 따른 비교 횟수 증가 및 비교 결과 반환
/// </summary>
/// <param name="sortFuncNameStr">정렬 함수 이름 문자열</param>
template<typename SortElementType>
int CompareAndLoggingCompareCount(SortElementType x, SortElementType y, const char* sortFuncNameStr)
{
	compareCount[SortFuncNameStrToCompareCountIndex(sortFuncNameStr)]++;

	if (LOGGING_LEVEL == 2)
	{
		mutex.lock();

		std::cout << sortFuncNameStr << " : " << x << "과(와) " << y << " 비교 발생\n";

		mutex.unlock();
	}

	return COMPARE(x, y);
}

#define COMPARE(x, y) CompareAndLoggingCompareCount(x,y, __func__) //비교 횟수 카운트 위한 호출 측 함수명 이용
#endif

enum class ORDER_BY : const int
{
	ASCENDING, //오름차순
	DESCENDING //내림차순
}; //정렬 방향

/// <summary>
/// 버블 정렬
/// </summary>
/// <typeparam name="SortElementType">정렬 요소 타입</typeparam>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param name="orderBy">정렬 방향</param>
template<typename SortElementType>
void BubbleSort(SortElementType targetEnumerableSet[], 
	size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	/***
		< 버블 정렬 - 오름차순 >

		1) 최초 비교 대상 범위는 순차적으로 열거 가능 한 요소들의 집합 내의 모든 요소들에 대해 시작

		2) 현재 비교 대상 범위 내의 모든 요소들을 이웃 요소끼리 순차적으로 비교 (정렬 방향에 따라 비교 조건 변동)
		: 인덱스 0와 인덱스 1의 요소, 인덱스 1과 인덱스 2의 요소...

			2-1) 비교 중인 뒤의 요소가 이웃 한 바로 앞의 요소보다 작을 경우
			: 두 요소를 SWAP

			2-2) 비교 중인 뒤의 요소가 이웃 한 바로 앞의 요소보다 크거나 같을 경우
			: 두 요소를 SWAP 하지 않고, 다음 이웃 한 요소들에 대해 계속 비교

		3) 현재 비교 대상 범위 내의 마지막 요소는 정렬 된 조건을 만족하므로, 다음 비교 대상 범위에서 제외 (비교 대상 범위 1 감소)

		---

		< 요소들 간 비교 횟수 >

		n : 순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수

		ex1) 요소가 6개일 때, 첫 번째 루프에서 비교 범위 내의 모든 요소들을 5번 비교
		각 루프 완료 후 비교 범위 1씩 감소

		각 루프 완료 후 맨 뒤부터 순차적으로 정렬 된 요소를 제외하여 비교 할 요소 개수 하나씩 감소
		5 + 4 + 3 + 2 + 1
		== 6 (집합 내에서 앞, 뒤의 요소 쌍의 합) * 2 (집합 내에서 앞, 뒤의 요소 쌍의 개수) + 3 (쌍이 되지 못한 나머지)
		== 6 (집합 내에서 앞, 뒤의 요소 쌍의 합) * 2.5 (집합 내에서 앞, 뒤의 요소 쌍의 개수 (n이 짝수 일 경우, 쌍이 되지 못한 나머지가 포함))

		ex2) 요소가 7개일 때,  첫 번째 루프에서 비교 범위 내의 모든 요소들을 6번 비교
		각 루프 완료 후 비교 범위 1씩 감소
		6 + 5 + 4 + 3 + 2 + 1
		== 7 (집합 내에서 앞, 뒤의 요소 쌍의 합) * 3 (집합 내에서 앞, 뒤의 요소 쌍의 개수 (n이 짝수 일 경우, 쌍이 되지 못한 나머지가 포함))

		집합 내에서 앞, 뒤의 요소 쌍의 합 == n
		집합 내에서 앞, 뒤의 요소 쌍의 개수 (n이 짝수 일 경우, 쌍이 되지 못한 나머지가 포함) == (n-1) / 2
		이므로,
		(n-1) + (n-2) + (n-3) + ... + (n-(n-2) + (n-(n-1)) == (n-1) + (n-2) + (n-3) + ... + 2 + 1 == n * ((n-1) / 2)
	***/

	SortElementType tmp;
	bool swapPerformed = false; //현재 비교 대상 범위에 대해 SWAP 발생 여부

	for (size_t i = 0; i < (elementCount - 1); i++) //마지막 요소는 직접 접근 하지 않으며, 비교 시에만 간접 접근
	{
		swapPerformed = false;

		for (size_t j = 0; j < ((elementCount - 1) - i); j++) //현재 비교 대상 범위 내의 마지막 요소는 정렬 된 조건을 만족하므로, 다음 비교 대상 범위에서 제외
		{
			switch (orderBy)
			{
			case ORDER_BY::ASCENDING:
				if (COMPARE(targetEnumerableSet[j], targetEnumerableSet[j + 1]) == 1) //a > b
				{
					SWAP(targetEnumerableSet[j], targetEnumerableSet[j + 1], tmp);
					swapPerformed = true;
				}

				break;

			case ORDER_BY::DESCENDING:
				if (COMPARE(targetEnumerableSet[j], targetEnumerableSet[j + 1]) == -1) //a < b
				{
					SWAP(targetEnumerableSet[j], targetEnumerableSet[j + 1], tmp);
					swapPerformed = true;
				}

				break;
			}
		}

		if (!swapPerformed) //현재 비교 대상 범위에 대해 SWAP이 한 번도 수행되지 않은 경우 (이미 내부 요소가 모두 정렬 되어 있을 경우)
			break;
	}
}

/// <summary>
/// 삽입 정렬
/// </summary>
/// <typeparam name="SortElementType">정렬 요소 타입</typeparam>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param name="orderBy">정렬 방향</param>
template<typename SortElementType>
void InsertionSort(SortElementType targetEnumerableSet[],
	size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	/***
		< 삽입 정렬 - 오름차순 >

		1) 최초 비교 대상 범위는 순차적으로 열거 가능 한 요소들의 집합 내의 첫 번째 요소 ~ 두 번째 요소의 범위부터 시작

		2) 현재 비교 대상 범위 내에서 마지막 요소를 이웃 한 바로 앞의 요소와 비교 (정렬 방향에 따라 비교 조건 변동)

			2-1) 비교 중인 뒤의 요소가 이웃 한 바로 앞의 요소보다 작을 경우
			: 현재 비교 대상 범위 내에서 비교 중인 뒤의 요소가 삽입 될 적절한 위치를 현재 비교 대상 범위의 처음부터 순차적으로 탐색하여,
			삽입 될 위치의 기존 요소부터 비교 중인 뒤의 요소가 이웃 한 바로 앞의 요소 내의 범위의 요소들을 뒤로 한 칸씩 이동하고
			비교 중인 뒤의 요소를 빈 위치에 삽입

			2-2) 비교 중인 뒤의 요소가 이웃 한 바로 앞의 요소보다 클 경우
			: 현재 비교 대상 범위 내의 마지막 요소는 정렬 된 조건을 만족하므로, 현재 비교 대상 범위 1 증가

		---

		< 요소들 간 비교 횟수 >

		n : 순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수

		ex) 요소가 6개일 때, 첫 번째 루프에서 비교 범위 내의 두 요소를 1번 비교
		각 루프 완료 후 비교 범위 1씩 증가
		1 + 2 + 3 + 4 + 5
		== 6 (집합 내에서 앞, 뒤의 요소 쌍의 합) * 2 (집합 내에서 앞, 뒤의 요소 쌍의 개수) + 3 (쌍이 되지 못한 나머지)
		== 6 (집합 내에서 앞, 뒤의 요소 쌍의 합) * 2.5 (집합 내에서 앞, 뒤의 요소 쌍의 개수 (n이 짝수 일 경우, 쌍이 되지 못한 나머지가 포함))

		1 + 2 + 3 + ... + (n-2) + (n-1)
		집합 내에서 앞, 뒤의 요소 쌍의 합 == n
		집합 내에서 앞, 뒤의 요소 쌍의 개수 (n이 짝수 일 경우, 쌍이 되지 못한 나머지가 포함) == (n-1) / 2 이므로,
		1 + 2 + 3 + ... + (n-2) + (n-1) == (n-1) + (n-2) + ... + 2 + 1 == n * ((n-1) / 2)

		---

		< 버블 정렬과 삽입 정렬 비교 >

		1) 이미 정렬 되어 있는 상황 (Best Case)
		: 0, 1, 2 에 대한 오름차순 정렬

			1-1) 버블 정렬
			: 현재 비교 대상 범위에 대해 한 번도 SWAP이 발생하지 않았을 경우 (!swapPerformed),
			이는 이미 내부 데이터가 모두 정렬되어있음을 의미하고, 비교를 중단하므로 (n-1) 의 비교 횟수

			< 실제 수행 결과 >

			BubbleSort : 0과(와) 1 비교 발생
			BubbleSort : 1과(와) 2 비교 발생

			1-2) 삽입 정렬
			: (n-1) 의 비교 횟수

			< 실제 수행 결과 >

			InsertionSort : 0과(와) 1 비교 발생
			InsertionSort : 1과(와) 2 비교 발생

		2) 정렬하고자 하는 방법과 반대로 정렬 되어 있는 상황 (Worst Case)
		: 2 1 0 에 대한 오름차순 정렬

			2-1) 버블 정렬
			: 2 1 0 => 2과(와) 1를 비교하여 총 1회 비교
				1 2 0 => 2과(와) 0을 비교하여 총 1회 비교
					1 0 2 => 1과(와) 0을 비교하여 총 1회 비교
						0 1 2

			< 실제 수행 결과 >

			BubbleSort : 2과(와) 1 비교 발생
			BubbleSort : 2과(와) 0 비교 발생
			BubbleSort : 1과(와) 0 비교 발생

			2-2) 삽입 정렬
			: 2 1 0 => 2과(와) 1를 비교하고, 1의 적절한 위치 탐색에 따른 2과(와) 비교하여 총 2회 비교
				1 2 0 => 0과(와) 2을 비교하고, 0의 적절한 위치 탐색에 따른 1과(와) 비교하여 총 2회 비교
					0 1 2

			< 실제 수행 결과 >

			InsertionSort : 2과(와) 1 비교 발생
			InsertionSort : 2과(와) 1 비교 발생
			InsertionSort : 2과(와) 0 비교 발생
			InsertionSort : 1과(와) 0 비교 발생

		이론적으로 버블 정렬 및 삽입 정렬은 n * ((n-1) / 2) 의 비교 횟수를 가짐
		그러나 요소의 개수가 커질수록 Best Case 에서는 항상 동일하나, Worst Case, Average Case 는 극단적으로 삽입 정렬이 더 적은 비교 횟수를 가짐

		버블 정렬은 거의 대부분 상황에서 n * ((n-1) / 2) 이거나 거의 가까운 수치
		(정렬 과정 중 현재 비교 대상 범위에 대해 SWAP 이 한 번도 수행되지 않은 경우 (이미 내부 요소가 모두 정렬 되어 있을 경우)
		더 이상 비교를 중지하므로 n * ((n-1) / 2) 보다 적게 나올 수 있음)

		삽입 정렬의 현재 비교 대상 범위 내에서 비교 중인 뒤의 요소가 삽입 될 적절한 위치를 현재 비교 대상 범위의 처음부터 순차적으로 탐색 과정에서
		최초 삽입이 발생한 시점부터 항상 현재 비교 대상 범위의 모든 요소들은 정렬 된 순서를 유지하고,
		다음 비교 시 비교 대상 범위가 증가하나, 비교 대상 범위 증가 전의 모든 기존 요소들은 마찬가지 정렬 된 순서를 유지하므로,
		임의의 데이터 패턴에 대해 정렬되어 있는 현재 비교 대상 범위 내의 요소들에 대한 삽입 할 위치를 즉시 찾을 수 있거나,
		평균적으로 (n-1) + n * ((n-1) / 2) / 2
	***/

	SortElementType tmp;

	for (size_t i = 1; i < elementCount; i++)
	{
		switch (orderBy)
		{
		case ORDER_BY::ASCENDING:
			if (COMPARE(targetEnumerableSet[i - 1], targetEnumerableSet[i]) == 1) //a > b
			{
				tmp = targetEnumerableSet[i]; //비교 중인 뒤의 요소

				for (size_t j = 0; j < i; j++) //현재 비교 대상 범위 내에서 비교 중인 뒤의 요소가 삽입 될 적절한 위치 탐색
				{
					if (COMPARE(targetEnumerableSet[j], tmp) == 1) //기존 요소 > 비교 중인 뒤의 요소
					{
						memmove_s(
							&targetEnumerableSet[j + 1],
							sizeof(targetEnumerableSet[0]) * (i - j),
							&targetEnumerableSet[j],
							sizeof(targetEnumerableSet[0]) * (i - j)
						); //삽입 될 위치의 기존 요소부터 비교 중인 뒤의 요소가 이웃 한 바로 앞의 요소 내의 범위의 요소들을 뒤로 한 칸씩 이동
						targetEnumerableSet[j] = tmp;
						break;
					}
				}
			}

			break;

		case ORDER_BY::DESCENDING:
			if (COMPARE(targetEnumerableSet[i - 1], targetEnumerableSet[i]) == -1) //a < b
			{
				tmp = targetEnumerableSet[i]; //비교 중인 뒤의 요소

				for (size_t j = 0; j < i; j++) //현재 비교 대상 범위 내에서 비교 중인 뒤의 요소가 삽입 될 적절한 위치 탐색
				{
					if (COMPARE(targetEnumerableSet[j], tmp) == -1) //기존 요소 < 비교 중인 뒤의 요소
					{
						memmove_s(
							&targetEnumerableSet[j + 1],
							sizeof(targetEnumerableSet[0]) * (i - j),
							&targetEnumerableSet[j],
							sizeof(targetEnumerableSet[0]) * (i - j)
						); //삽입 될 위치의 기존 요소부터 비교 중인 뒤의 요소가 이웃 한 바로 앞의 요소 내의 범위의 요소들을 뒤로 한 칸씩 이동
						targetEnumerableSet[j] = tmp;
						break;
					}
				}
			}

			break;
		}
	}
}

/// <summary>
/// 순차적으로 열거 가능 한 요소들의 집합에 대한 분할 및 이동 처리
/// </summary>
/// <typeparam name="SortElementType">정렬 요소 타입</typeparam>
/// <param name="targetEnumereableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="partitionLeftIndex">순차적으로 열거 가능 한 요소들의 집합의 분할 된 왼쪽 인덱스 (최초 0)</param>
/// <param name="partitionRightIndex">순차적으로 열거 가능 한 요소들의 집합의 분할 된 오른쪽 인덱스 (최초 요소들의 개수 - 1)</param>
/// <param name="orderBy">정렬 방향</param>
/// <returns>분할 및 이동 처리 완료 후 변동 된 순차적으로 열거 가능 한 요소들의 집합의 분할 된 오른쪽 인덱스</returns>
template<typename SortElementType>
size_t PartitioningProc(SortElementType targetEnumereableSet[], 
	size_t partitionLeftIndex, size_t partitionRightIndex, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	/***
		< Partitioning Proc - 오름차순 >

		! partitionLeftIndex : 왼쪽 인덱스
		! partitionRightIndex : 오른쪽 인덱스

		1) 최초 호출 시점의 초기값
			
			1-1) 왼쪽 인덱스는 순차적으로 열거 가능 한 요소들의 집합의 처음부터 시작
			
			1-2) 오른쪽 인덱스는 순차적으로 열거 가능 한 요소들의 집합의 마지막부터 시작

		2) 기준 (pivot)을 현재 순차적으로 열거 가능 한 요소들의 집합의 첫 번째 요소 (왼쪽 인덱스)로 선택
		
			2-1) 왼쪽 인덱스는 기준 (pivot)이 되는 요소가 제외 된 다음 위치 (왼쪽 인덱스 + 1)부터 시작

		3) 기준 (pivot)이 되는 요소를 제외한 나머지 요소들에 대해 왼쪽 인덱스와 오른쪽 인덱스가 교차되는 시점까지 이하, 반복

		////
		4) 왼쪽 인덱스와 오른쪽 인덱스의 각각 반대 방향으로 서로 탐색 수행 (정렬 방향에 따라 비교 조건 변동)

			4-1) 왼쪽 인덱스는 기준 (pivot)보다 같거나 큰 요소가 나올 때까지 오른쪽으로 탐색
		
			4-2) 오른쪽 인덱스는 기준 (pivot)보다 작은 요소가 나올 때까지 왼쪽으로 탐색

			4-3) 위의 탐색 과정 간 현재 왼쪽 인덱스와 오른쪽 인덱스가 교차 여부에 따라,

				4-3-1) 왼쪽 인덱스와 오른쪽 인덱스가 교차 될 경우
				: 기준 (pivot)에 대해 나머지 요소들이 이미 정렬되어있는 경우에 발생하므로 탐색 중지

				4-3-2) 왼쪽 인덱스와 오른쪽 인덱스가 교차되지 않았을 경우
				: 정렬 조건을 만족하기위해 현재 왼쪽 인덱스의 요소와 오른쪽 인덱스의 요소를 서로 SWAP

		5) 현재 왼쪽 인덱스와 오른쪽 인덱스가 아직 교차되지 않았을 경우, 다시 탐색 수행
		/////

		6) 왼쪽 인덱스와 오른쪽 인덱스가 교차 된 시점에, 기준 (pivot)과 오른쪽 인덱스의 요소 SWAP
		: 이에 따라, 기준 (pivot)이 되는 요소의 오른쪽에 기준 (pivot)이 되는 요소보다 큰 요소들만 존재, 
		기준 (pivot)이 되는 요소의 왼쪽에 기준 (pivot)이 되는 요소보다 작거나 같은 요소들만 존재

		---

		TODO : 논리 오류?

		5) 현재 왼쪽 인덱스의 요소와 기준 (pivot)이 되는 요소 비교 (정렬 방향에 따라 비교 조건 변동)

			5-1) 현재 왼쪽 인덱스의 요소가 기준 (pivot)보다 작거나 같을 경우
			: 다음 왼쪽 인덱스의 위치로 이동 (왼쪽 인덱스 + 1)

			5-2) 현재 왼쪽 인덱스의 요소가 기준 (pivot)보다 클 경우
			: 현재 왼쪽 인덱스의 요소는 기준 (pivot)이 되는 요소의 오른쪽으로 이동해야하므로,
			현재 왼쪽 인덱스의 요소와 현재 오른쪽 인덱스의 요소 SWAP
			이에 따라, 현재 오른쪽 인덱스의 요소는 기준 (pivot)이 되는 요소보다 큰 요소이므로 다음 오른쪽 인덱스의 위치로 이동 (오른쪽 인덱스 - 1)
			현재 왼쪽 인덱스의 요소는 기준 (pivot)이 되는 요소보다 작거나 같을 수도 있지만, 클 수도 있으므로, 왼쪽 인덱스는 변동되지 않음

		7) 왼쪽 인덱스와 오른쪽 인덱스가 교차되는 시점에 대해,
		: 현재 왼쪽 인덱스의 요소 왼쪽에는 기준 (pivot)보다 작거나 같은 요소들만 존재하며, 
		오른쪽 인덱스의 오른쪽에는 기준 (pivot)보다 큰 요소들만 존재
		
		기준 (pivot)과 오른쪽 인덱스의 요소 SWAP
		
		이에 따라, 기준 (pivot)의 왼쪽에는 기준 (pivot)보다 작거나 같은 요소들만 존재,
		기준 (pivot)의 오른쪽에는 기준보다 큰 요소들만 존재

		TODO : 이하 명확하지 않으므로 수정
		
		8) 기준 (pivot)을 제외한 남아있는 요소들에 대한 다음 Partitioning 처리를 위해 현재 partitionRightIndex 반환

			8-1) 해당 반환 값을 다음 입력 시 기준 (pivot)을 제외한 남아있는 왼쪽 요소에 대해 처리를 위해 다음과 같이 입력 할 것
			: partitionLeftIndex (Arg) = Caller Left Index, partitionRightIndex (Arg) = partitionRightIndex (retVal) - 1

			8-2) 해당 반환 값을 다음 입력 시 기준 (pivot)을 제외한 남아있는 오른쪽 요소에 대해 처리를 위해 다음과 같이 입력 할 것
			: partitionLeftIndex (Arg) = partitionRightIndex (retVal) + 1, partitionRightIndex (Arg) = Caller Right Index
	***/

	if(partitionLeftIndex < 0 || partitionRightIndex < 0 || (partitionLeftIndex > partitionRightIndex))
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	size_t pivotIndex = partitionLeftIndex++; //기준 (pivot)이 되는 요소 인덱스
	size_t tmp;

	while (partitionLeftIndex < partitionRightIndex) //기준 (pivot)이 되는 요소를 제외한 나머지 요소들에 대해
	{
		switch (orderBy)
		{
		case ORDER_BY::ASCENDING:
			if (COMPARE(targetEnumereableSet[partitionLeftIndex], targetEnumereableSet[pivotIndex]) <= 0) //a <= b (0 or -1)
				partitionLeftIndex++;
			else //a > b
				SWAP(targetEnumereableSet[partitionLeftIndex], targetEnumereableSet[partitionRightIndex--], tmp);

			break;

		case ORDER_BY::DESCENDING:
			if (COMPARE(targetEnumereableSet[partitionLeftIndex], targetEnumereableSet[pivotIndex]) >= 0) //a >= b (0 or 1)
				partitionLeftIndex++;
			else //a < b
				SWAP(targetEnumereableSet[partitionLeftIndex], targetEnumereableSet[partitionRightIndex--], tmp);

			break;
		}
	}

	SWAP(targetEnumereableSet[pivotIndex],
		targetEnumereableSet[partitionRightIndex],
		tmp); //최종 정렬 조건을 만족하기 위해, 기준 (pivot)과 partitionLeftIndex 의 요소 SWAP
	return partitionRightIndex;
}

/// <summary>
/// 퀵 정렬
/// </summary>
/// <typeparam name="SortElementType">정렬 요소 타입</typeparam>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="leftIndex">순차적으로 열거 가능 한 요소들의 집합의 왼쪽 인덱스 (최초 0)</param>
/// <param name="rightIndex">순차적으로 열거 가능 한 요소들의 집합의 오른쪽 인덱스 (최초 요소들의 개수 - 1)</param>
/// <param name="orderBy">정렬 방향</param>
template<typename SortElementType>
void QuickSort(SortElementType targetEnumerableSet[], 
	size_t leftIndex, size_t rightIndex, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	if (leftIndex < 0 || rightIndex < 0)
		throw std::invalid_argument(std::string(__func__) + std::string(" : Invalid Args"));

	if (leftIndex < rightIndex) //순차적으로 열거 가능 한 요소들의 집합에 대해 분할 할 수 있는 경우
	{
		size_t partitionRightIndex = PartitioningProc(targetEnumerableSet, leftIndex, rightIndex); //분할 된 오른쪽 인덱스

		QuickSort<SortElementType>(targetEnumerableSet, 
			leftIndex, partitionRightIndex - 1, orderBy); //현재 분할 된 오른쪽 인덱스 기준 남아있는 왼쪽 요소들에 대한 처리
		QuickSort<SortElementType>(targetEnumerableSet, 
			partitionRightIndex + 1, rightIndex, orderBy); //현재 분할 된 오른쪽 인덱스 기준 남아있는 오른쪽 요소들에 대한 처리
	}
}

/// <summary>
/// 퀵 정렬
/// </summary>
/// <typeparam name="SortElementType">정렬 요소 타입</typeparam>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param name="orderBy">정렬 방향</param>
template<typename SortElementType>
void QuickSort(SortElementType targetEnumerableSet[], 
	size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	/***
	//TODO : 주석 수정

		< 퀵 정렬 - 오름차순 >

		1) 순차적으로 열거 가능 한 요소들의 집합 내의 모든 요소들에 대한 기준 (pivot)이 되는 임의의 요소를 선택

			M1) 현재 요소들 중 첫 번째 요소를 기준 (pivot)으로 선택하는 방법

			M2) 현재 요소들 중 마지막 요소를 기준 (pivot)으로 선택하는 방법

			M3) 현재 요소들 중 선택되지 않은 기준 (pivot)이 나올 때까지 임의의 난수를 생성 해 기준 (pivot)으로 선택하는 방법

			M4) 현재 요소들 중 중앙값 (median)을 기준 (pivot)으로 선택하는 방법

		2) 순차적으로 열거 가능 한 요소들의 집합에 대해 더 이상 분할 할 수 없을 때까지, 이하 반복

		3) 현재 기준 (pivot)이 되는 요소를 제외한 나머지 요소들에 대해

			3-1) 현재 기준 (pivot)이 되는 요소를 제외한 나머지 요소 < 현재 기준 (pivot)이 되는 요소
			: 기준 (pivot)이 되는 요소를 제외한 나머지 요소를 기준 (pivot)이 되는 요소의 왼쪽으로 이동

			3-2) 현재 기준 (pivot)이 되는 요소를 제외한 나머지 요소 >= 현재 기준 (pivot)이 되는 요소
			: 기준 (pivot)이 되는 요소를 제외한 나머지 요소를 기준 (pivot)이 되는 요소의 오른쪽으로 이동
	***/

	

//	size_t partitionLeftIndex; //분할 된 왼쪽 인덱스
	//size_t partitionRightIndex; //분할 된 오른쪽 인덱스
//	size_t partitionPivotIndex; //분할 된 기준 (pivot)이 되는 요소 인덱스
//	size_t tmp;

#ifdef RECURSIVE_METHOD
	QuickSort<SortElementType>(targetEnumerableSet, 0, elementCount - 1, orderBy);

#elif defined ITERATIVE_METHOD

#endif
}

#endif