#ifndef _SORT_HPP_
#define _SORT_HPP_

//#define RECURSIVE_METHOD //재귀적 방법 (주석 처리 : 반복적 방법 사용)
#ifndef RECURSIVE_METHOD 
#define ITERATIVE_METHOD //반복적 방법
#endif

#define LOGGING_COMPARE_COUNT //비교 횟수 카운트 기록 및 출력 (주석 처리 : 비활성화)
#ifdef LOGGING_COMPARE_COUNT
extern class TRACE_RESULT;
extern enum class SORT_UNIQUE_MAPPED_INDEX;
extern struct SORT_METADATA;
extern class SORT_MAPPER;

#define ALTERNATIVE_COMPARE(x, y) SORT_MAPPER::GetInstance(). \
GetRefTraceResult(SORT_MAPPER::GetInstance().SortFuncNameStrToSortUniqueMappedIndex(__func__)). \
IncreaseCompareCount(), \
((x) > (y) ? 1 : (x) == (y) ? 0 : -1) //x > y : 1, x == y : 0, x < y : -1
#define COMPARE(x, y) ALTERNATIVE_COMPARE(x, y) //비교 횟수 카운트 위한 호출 측 함수명 이용
#endif

/***
	< 버블 정렬 요소들 간 비교 횟수 >

	n : 순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수

	ex1) 요소가 6개일 때, 첫 번째 루프에서 비교 범위 내의 모든 요소들을 5번 비교
	각 루프 완료 후 비교 범위 1씩 감소

	각 루프 완료 후 맨 뒤부터 순차적으로 정렬 된 요소를 제외하여 비교 할 요소 개수 하나씩 감소
	5 + 4 + 3 + 2 + 1
	= 6 (집합 내에서 앞, 뒤의 요소 쌍의 합) * 2 (집합 내에서 앞, 뒤의 요소 쌍의 개수) + 3 (쌍이 되지 못한 나머지)
	= 6 (집합 내에서 앞, 뒤의 요소 쌍의 합) * 2.5 (집합 내에서 앞, 뒤의 요소 쌍의 개수 (n이 짝수 일 경우, 쌍이 되지 못한 나머지가 포함))

	ex2) 요소가 7개일 때,  첫 번째 루프에서 비교 범위 내의 모든 요소들을 6번 비교
	각 루프 완료 후 비교 범위 1씩 감소
	6 + 5 + 4 + 3 + 2 + 1
	= 7 (집합 내에서 앞, 뒤의 요소 쌍의 합) * 3 (집합 내에서 앞, 뒤의 요소 쌍의 개수 (n이 짝수 일 경우, 쌍이 되지 못한 나머지가 포함))

	집합 내에서 앞, 뒤의 요소 쌍의 합 = n
	집합 내에서 앞, 뒤의 요소 쌍의 개수 (n이 짝수 일 경우, 쌍이 되지 못한 나머지가 포함) = (n-1) / 2
	이므로,
	(n-1) + (n-2) + (n-3) + ... + (n-(n-2) + (n-(n-1)) = (n-1) + (n-2) + (n-3) + ... + 2 + 1 = n * ((n-1) / 2)

	---

	< 삽입 정렬 요소들 간 비교 횟수 >

	n : 순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수

	ex) 요소가 6개일 때, 첫 번째 루프에서 비교 범위 내의 두 요소를 1번 비교
	각 루프 완료 후 비교 범위 1씩 증가
	1 + 2 + 3 + 4 + 5
	= 6 (집합 내에서 앞, 뒤의 요소 쌍의 합) * 2 (집합 내에서 앞, 뒤의 요소 쌍의 개수) + 3 (쌍이 되지 못한 나머지)
	= 6 (집합 내에서 앞, 뒤의 요소 쌍의 합) * 2.5 (집합 내에서 앞, 뒤의 요소 쌍의 개수 (n이 짝수 일 경우, 쌍이 되지 못한 나머지가 포함))

	1 + 2 + 3 + ... + (n-2) + (n-1)
	집합 내에서 앞, 뒤의 요소 쌍의 합 = n
	집합 내에서 앞, 뒤의 요소 쌍의 개수 (n이 짝수 일 경우, 쌍이 되지 못한 나머지가 포함) = (n-1) / 2 이므로,
	1 + 2 + 3 + ... + (n-2) + (n-1) = (n-1) + (n-2) + ... + 2 + 1 = n * ((n-1) / 2)

	---

	< 버블 정렬과 삽입 정렬 비교 >

	이론적으로 버블 정렬 및 삽입 정렬은 n * ((n-1) / 2) 의 비교 횟수를 가짐
	그러나 요소의 개수가 커질수록 Best Case 에서는 항상 동일하나,
	Worst Case, Average Case 는 극단적으로 삽입 정렬이 더 적은 비교 횟수를 가짐

	버블 정렬은 거의 대부분 상황에서 n * ((n-1) / 2) 이거나 거의 가까운 수치
	(정렬 과정 중 현재 비교 대상 범위에 대해 SWAP 이 한 번도 수행되지 않은 경우
	즉, 이미 내부 요소가 모두 정렬 되어 있을 경우
	더 이상 비교를 중지하므로 n * ((n-1) / 2) 보다 적게 나올 수 있음)

	삽입 정렬의 현재 비교 대상 범위 내에서 비교 중인 뒤의 요소가 삽입 될 적절한 위치를
	현재 비교 대상 범위의 처음부터 순차적으로 탐색 과정에서
	최초 삽입이 발생한 시점부터 항상 현재 비교 대상 범위의 모든 요소들은 정렬 된 순서를 유지하고,
	다음 비교 시 비교 대상 범위가 증가하나, 비교 대상 범위 증가 전의 모든 기존 요소들은 마찬가지 정렬 된 순서를 유지하므로,
	임의의 데이터 패턴에 대해 정렬되어 있는 현재 비교 대상 범위 내의 요소들에 대한 삽입 할 위치를 즉시 찾을 수 있거나,
	평균적으로 (n-1) + n * ((n-1) / 2) / 2

	---

	< 선택 정렬의 요소들 간 비교 횟수 >

	n : 순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수

	ex) 요소가 6개일 때, 첫 번째 루프에서 정렬 대상과 정렬 대상을 제외한 나머지 요소들 중 정렬 된 조건을 만족하는 요소를 찾기 위해 5번 비교
	각 루프 완료 후 비교 범위 1씩 감소
	5 + 4 + 3 + 2 + 1
	= 6 (집합 내에서 앞, 뒤의 요소 쌍의 합) * 2 (집합 내에서 앞, 뒤의 요소 쌍의 개수) + 3 (쌍이 되지 못한 나머지)
	= 6 (집합 내에서 앞, 뒤의 요소 쌍의 합) * 2.5 (집합 내에서 앞, 뒤의 요소 쌍의 개수 (n이 짝수 일 경우, 쌍이 되지 못한 나머지가 포함))

	1 + 2 + 3 + ... + (n-2) + (n-1)
	집합 내에서 앞, 뒤의 요소 쌍의 합 = n
	집합 내에서 앞, 뒤의 요소 쌍의 개수 (n이 짝수 일 경우, 쌍이 되지 못한 나머지가 포함) = (n-1) / 2 이므로,
	1 + 2 + 3 + ... + (n-2) + (n-1) = (n-1) + (n-2) + ... + 2 + 1 = n * ((n-1) / 2)

	선택 정렬은 항상 정렬 대상을 제외한 나머지 요소들 중 정렬 된 조건을 만족하는 요소를 찾기 위해
	정렬 대상을 제외한 나머지 요소들에 대해 모두 비교가 발생하므로, 
	어떠한 데이터 패턴에도 동일 한 비교 횟수를 가짐

	---

	< 퀵 정렬의 요소들 간 비교 횟수 >

	https://www.interviewbit.com/tutorial/quicksort-algorithm/#h827ske1ht0ea2fcemd1dxxl7u1ndu40q

	n : 순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수

	ex) 요소가 8개일 때, 모든 분할마다 동일 한 개수의 집합으로 분리 될 경우 (Best Case)
	최대 분할 횟수 = 4회 (1, 2, 4, 8) = log2(n)
	각 분할 과정 발생 시마다 비교 횟수의 합 = n회
	각 레벨 간에서의 비교 횟수의 합 * 최대 분할 횟수 = n * log2(n)

	각 분할마다 하나의 요소와 해당 요소를 제외한 요소들의 집합 (1 : n-1) 으로 분리 될 경우 (Worst Case)
	이미 정렬 된 요소들에 대해 첫 번째 혹은 마지막 요소를 기준 (pivot)으로 선정 된 상황에서 정렬을 시도 할 경우 발생하며,

	비교 횟수 = 7 + 6 + ... 2 + 1 = (n-1) + (n-2) + ... + 2 + 1 = n * ((n-1) / 2)

	중앙값을 기준 (pivot)으로 선정하거나 난수를 생성하여 임의의 기준 (pivot)을 선정 할 경우,
	비교 연산 및 필요 할 경우 SWAP 연산에 있어 상수 시간만큼 더 소요되지만,
	O(n^2)에서 O(nlog2(n)) 으로 낮출 수 있음
***/

enum class ORDER_BY : const int
{
	ASCENDING, //오름차순
	DESCENDING //내림차순
}; //정렬 방향

/// <summary>
/// 버블 정렬 (Best Case : O(n), Worst, Average Case : O(n^2))
/// </summary>
/// <typeparam name="SORT_ELEMENT_TYPE">정렬 요소 타입</typeparam>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param name="orderBy">정렬 방향</param>
template<typename SORT_ELEMENT_TYPE>
void BubbleSort(SORT_ELEMENT_TYPE targetEnumerableSet[],
	size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	/***
		< 버블 정렬 - 오름차순 >

		1) 최초 비교 대상 범위는 순차적으로 열거 가능 한 요소들의 집합 내의 모든 요소들에 대해 시작

		2) 현재 비교 대상 범위 내의 모든 요소들을 이웃 요소끼리 순차적으로 비교 (정렬 방향에 따라 비교 조건 변동)
		: 인덱스 0와 인덱스 1의 요소, 인덱스 1과 인덱스 2의 요소...

			2-1) 비교 중인 뒤의 요소 < 이웃 한 바로 앞의 요소
			: 두 요소를 SWAP

			2-2) 비교 중인 뒤의 요소 >= 이웃 한 바로 앞의 요소
			: 두 요소를 SWAP 하지 않고, 다음 이웃 한 요소들에 대해 계속 비교

		3) 현재 비교 대상 범위 내의 마지막 요소는 정렬 된 조건을 만족하므로, 다음 비교 대상 범위에서 제외 (비교 대상 범위 1 감소)
	***/

	SORT_ELEMENT_TYPE tmp;
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
/// 선택 정렬 (Best, Worst, Average Case : O(n^2))
/// </summary>
/// <typeparam name="SORT_ELEMENT_TYPE">정렬 요소 타입</typeparam>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param name="orderBy">정렬 방향</param>
template<typename SORT_ELEMENT_TYPE>
void SelectionSort(SORT_ELEMENT_TYPE targetEnumerableSet[],
	size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	/***
		< 선택 정렬 - 오름차순 >

		! 인간이 어떠한 집합 내에서 요소들을 정렬할 때 가장 일반적으로 수행하는 것처럼 하되,
		차이점은 정렬 조건을 만족하는 요소를 탐색할 경우, 단순히 앞으로 삽입하는 것이 아니라, 현재 정렬 대상 요소와 SWAP 수행

		1) 최초 비교 대상 범위는 순차적으로 열거 가능 한 요소들의 집합 내의 모든 요소들에 대해 시작

		2) 최초 정렬 대상은 순차적으로 열거 가능 한 요소들의 집합 내의 처음 요소에 대해 시작

		3) 현재 정렬 대상과 현재 비교 대상 범위 내의 모든 요소들에 대해 순차적으로 비교 (정렬 방향에 따라 비교 조건 변동)

			3-1) 현재 정렬 대상 < 비교 대상 범위 내의 비교 중인 어떠한 요소
			: 해당 비교 대상 범위 내의 비교 중인 어떠한 요소를 정렬 된 조건을 만족하는 요소로 마킹

			3-2) 현재 정렬 대상 >= 비교 대상 범위 내의 비교 중인 어떠한 요소
			: do nothing

		4) 마킹 된 정렬 된 조건을 만족하는 요소와 현재 정렬 대상 요소 SWAP

		5) 현재 비교 대상 범위 내의 정렬 대상 요소는 정렬 된 조건을 만족하므로, 다음 비교 대상 범위에서 제외 (비교 대상 범위 1 감소)
	***/

	size_t swapTargetIndex; //정렬 된 조건을 만족하는 SWAP 대상 인덱스
	SORT_ELEMENT_TYPE tmp;

	for (size_t i = 0; i < elementCount; i++) //각 정렬 대상에 대해
	{
		swapTargetIndex = i;

		for (size_t j = i + 1; j < elementCount; j++) //현재 정렬 대상을 제외한 요소들에 대해
		{
			switch (orderBy)
			{
			case ORDER_BY::ASCENDING:
				if (COMPARE(targetEnumerableSet[swapTargetIndex], targetEnumerableSet[j]) == 1) //a > b
					swapTargetIndex = j;

				break;

			case ORDER_BY::DESCENDING:
				if (COMPARE(targetEnumerableSet[swapTargetIndex], targetEnumerableSet[j]) == -1) //a < b
					swapTargetIndex = j;

				break;
			}
		}

		SWAP(targetEnumerableSet[i], targetEnumerableSet[swapTargetIndex], tmp);
	}
}

/// <summary>
/// 삽입 정렬 (Best Case : O(n), Worst, Average Case : O(n^2))
/// </summary>
/// <typeparam name="SORT_ELEMENT_TYPE">정렬 요소 타입</typeparam>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param name="orderBy">정렬 방향</param>
template<typename SORT_ELEMENT_TYPE>
void InsertionSort(SORT_ELEMENT_TYPE targetEnumerableSet[],
	size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	/***
		< 삽입 정렬 - 오름차순 >

		1) 최초 비교 대상 범위는 순차적으로 열거 가능 한 요소들의 집합 내의 첫 번째 요소 ~ 두 번째 요소의 범위부터 시작

		2) 현재 비교 대상 범위 내에서 마지막 요소를 이웃 한 바로 앞의 요소와 비교 (정렬 방향에 따라 비교 조건 변동)

			2-1) 비교 중인 뒤의 요소 < 이웃 한 바로 앞의 요소

				2-1-1) 현재 비교 대상 범위 내에서 비교 중인 뒤의 요소가 삽입 될 적절한 위치를 현재 비교 대상 범위의 처음부터 순차적으로 탐색

				2-1-2) 삽입 될 위치의 기존 요소부터 비교 중인 뒤의 요소가 이웃 한 바로 앞의 요소 내의 범위의 요소들을 뒤로 한 칸씩 이동

				2-1-3) 비교 중인 뒤의 요소를 빈 위치에 삽입

			2-2) 비교 중인 뒤의 요소 >= 이웃 한 바로 앞의 요소
			: 현재 비교 대상 범위 내의 마지막 요소는 정렬 된 조건을 만족하므로, 현재 비교 대상 범위 1 증가
	***/

	SORT_ELEMENT_TYPE tmp;

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
/// 순차적으로 열거 가능 한 요소들의 집합에 대해 기준 (pivot) 선정, 정렬 방향에 따른 정렬 수행 및 기준 (pivot)의 최종 정렬 된 위치 반환
/// </summary>
/// <typeparam name="SORT_ELEMENT_TYPE">정렬 요소 타입</typeparam>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="srcLeftIndex">순차적으로 열거 가능 한 요소들의 집합의 분할 된 왼쪽 인덱스 (최초 0)</param>
/// <param name="srcRightIndex">순차적으로 열거 가능 한 요소들의 집합의 분할 된 오른쪽 인덱스 (최초 요소들의 개수 - 1)</param>
/// <param name="orderBy">정렬 방향</param>
/// <returns>기준 (pivot)의 최종 정렬 된 위치</returns>
template<typename SORT_ELEMENT_TYPE>
size_t PartitioningProc(SORT_ELEMENT_TYPE targetEnumerableSet[],
	size_t srcLeftIndex, size_t srcRightIndex, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	size_t midIndex = srcLeftIndex + ((srcRightIndex - srcLeftIndex) >> 1); //base : srcLeftIndex, offset : srcRightIndex와 srcLeftIndex의 중간만큼 증가 (overflow 방지)
	size_t medianIndex; //중앙값 인덱스 (왼쪽 인덱스, 가운데 인덱스, 오른쪽 인덱스에 대한)

	size_t pivotIndex; //기준 (pivot) 인덱스
	size_t orderedPivotIndex; //기준 (pivot)의 최종 정렬 된 위치 인덱스

	SORT_ELEMENT_TYPE tmp;

	if (srcLeftIndex + 1 == srcRightIndex) //현재 요소가 2개인 경우
	{
		/***
			< 퀵 정렬 (2개의 요소에 대한 대체 정렬 처리) - 오름차순 >

			1) 두 요소들 간 비교 및 SWAP (정렬 방향에 따라 중앙값 선택 위한 비교 조건 변동)

			2) 왼쪽 인덱스 요소와 오른쪽 인덱스 요소 비교

				2-1) 왼쪽 인덱스 요소 > 오른쪽 인덱스 요소
				: 왼쪽 인덱스 요소와 오른쪽 인덱스 요소 SWAP (오른쪽 인덱스의 요소가 왼쪽 인덱스의 요소의 왼쪽에 위치하는 것이 올바른 정렬 된 위치)

				2-2) 왼쪽 인덱스 요소 <= 가운데 인덱스 요소
				: do nothing

			3) 현재 순차적으로 열거 가능 한 요소들의 집합내의 모든 요소들은 정렬되었으므로, 왼쪽 인덱스 반환

			---

			기존의 정렬 처리 루틴의 1회의 제자리 SWAP 연산을 없애기 위해
			두 요소 간 비교 및 필요 할 경우 SWAP 수행
		***/

		switch (orderBy)
		{
		case ORDER_BY::ASCENDING:
			if (COMPARE(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[srcRightIndex]) == 1) //left > right
				SWAP(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[srcRightIndex], tmp);
			break;

		case ORDER_BY::DESCENDING:
			if (COMPARE(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[srcRightIndex]) == -1) //left < right
				SWAP(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[srcRightIndex], tmp);
			break;
		}

		return srcLeftIndex;
	}
	else if ((midIndex + 1) == srcRightIndex) //현재 요소가 3개인 경우
	{
		/***
			< 퀵 정렬 (3개의 요소에 대한 대체 정렬 처리) - 오름차순 >

			1) 각 요소들 간 비교 및 SWAP (정렬 방향에 따라 중앙값 선택 위한 비교 조건 변동)

			2) 왼쪽 인덱스 요소와 가운데 인덱스 요소 비교

				2-1) 왼쪽 인덱스 요소 > 가운데 인덱스 요소
				: 가운데 인덱스 요소와 왼쪽 인덱스 요소 SWAP (가운데 인덱스의 요소가 왼쪽 인덱스의 요소의 왼쪽에 위치하는 것이 올바른 정렬 된 위치)

				2-2) 왼쪽 인덱스 요소 <= 가운데 인덱스 요소
				: do nothing

			3) 왼쪽 인덱스 요소와 오른쪽 인덱스 요소 비교

				3-1) 왼쪽 인덱스 요소 > 오른쪽 인덱스 요소
				: 왼쪽 인덱스 요소와 오른쪽 인덱스 요소 SWAP (오른쪽 인덱스의 요소가 왼쪽 인덱스의 요소의 왼쪽에 위치하는 것이 올바른 정렬 된 위치)

				3-2) 왼쪽 인덱스 요소 <= 오른쪽 인덱스 요소
				: do nothing

			4) 가운데 인덱스 요소와 오른쪽 인덱스 요소 비교

				4-1) 가운데 인덱스 요소 > 오른쪽 인덱스 요소
				: 오른쪽 요소와 가운데 인덱스 요소 SWAP (오른쪽 인덱스의 요소가 가운데 인덱스 요소의 왼쪽에 위치하는 것이 올바른 정렬 된 위치)

				4-2) 가운데 인덱스 요소 <= 오른쪽 인덱스 요소
				: do nothing

			5) 현재 순차적으로 열거 가능 한 요소들의 집합내의 모든 요소들은 정렬되었으므로, 가운데 인덱스 반환

			---

			ex) 3 1 2 의 요소에 대해, 오름차순으로 기존 정렬 처리 루틴을 수행 할 경우,

			- 2번의 비교 발생
			- 2번의 제자리 SWAP 빌생
			- 기준 (pivot)의 정렬 된 위치로 변경을 위한 1번의 SWAP 발생 (정렬 된 순서는 1 2 3)

			기준 (pivot) 3을 기준으로 다시 왼쪽에 대해 분할하여, 1 2 에 대해 다시 정렬을 수행
			1 2 에 대해 다시 오름차순으로 기존 정렬 처리 루틴을 수행 할 경우,

			- 1번의 비교 발생
			- 1번의 제자리 SWAP 발생

			최종적으로, 총 3번의 비교 연산, 4번의 SWAP 연산 수행
			이와 비교하여, 3 1 2 의 요소에 대해 3개의 요소에 대한 대체 정렬 처리 루틴을 수행 할 경우,

			- 3번의 비교 발생
			- 2번의 SWAP 발생

			최악의 경우 (정렬하고자 하는 방법과 역순으로 정렬 된 상황)에 3번의 비교, 3번의 SWAP 연산 발생
		***/

		switch (orderBy)
		{
		case ORDER_BY::ASCENDING:
			if (COMPARE(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[midIndex]) == 1) //left > mid
				SWAP(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[midIndex], tmp);

			if (COMPARE(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[srcRightIndex]) == 1) //left > right
				SWAP(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[srcRightIndex], tmp);

			if (COMPARE(targetEnumerableSet[midIndex], targetEnumerableSet[srcRightIndex]) == 1) //mid > right
				SWAP(targetEnumerableSet[midIndex], targetEnumerableSet[srcRightIndex], tmp);

			break;

		case ORDER_BY::DESCENDING:
			if (COMPARE(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[midIndex]) == -1) //left < mid
				SWAP(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[midIndex], tmp);

			if (COMPARE(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[srcRightIndex]) == -1) //left < right
				SWAP(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[srcRightIndex], tmp);

			if (COMPARE(targetEnumerableSet[midIndex], targetEnumerableSet[srcRightIndex]) == -1) //mid < right
				SWAP(targetEnumerableSet[midIndex], targetEnumerableSet[srcRightIndex], tmp);

			break;
		}

		return midIndex;
	}

	goto SELECT_MEDIAN_PIVOT_PROC; //현재 요소가 3개 초과일 경우 중앙값으로 기준 (pivot) 선택 처리 루틴

SELECT_MEDIAN_PIVOT_PROC: //중앙값으로 기준 (pivot) 선택 처리 루틴
	/***
		< 퀵 정렬 (중앙값 선택) - 오름차순 >

		1) 이하, 방법에 따라 중앙값 선정 및 기준 (pivot)으로 사용하기 위해 맨 왼쪽으로 이동

		2) 왼쪽 인덱스 요소, 가운데 인덱스 요소, 오른쪽 인덱스 요소에 대해, 중앙값인 인덱스 요소 탐색
		: 4번의 비교 발생

			2-1) 왼쪽 인덱스 요소, 가운데 인덱스 요소 중 최소 값 = a

			2-2) 왼쪽 인덱스 요소, 가운데 인덱스 요소 중 최대 값 = b

			2-3) b 와 오른쪽 인덱스 요소 중 최소값 = c

			2-4) a 와 c 중의 최대 값 = 중앙값

		3) 해당 중앙값 인덱스가 왼쪽 인덱스가 아닐 경우 왼쪽 인덱스의 요소와 해당 중앙값 인덱스의 요소 SWAP

		4) 중앙값 인덱스의 요소가 맨 왼쪽으로 이동되었으며, 이를 기준 (pivot)으로 이용하여 정렬 수행

		---

		해당 중앙값이 순차적으로 열거 가능 한 요소들의 집합 내에서 완전한 중앙값이라고 보장 할 수 없지만,
		이미 정렬 된 요소들에 대해 첫 번째 혹은 마지막 요소를 기준 (pivot)으로 선정 된 상황에서 정렬을 시도 할 경우 발생하는
		각 분할마다 하나의 요소와 해당 요소를 제외한 요소들의 집합 (1 : n-1) 으로 분리되는 Worst Case 를 회피
	***/

	medianIndex = MEDIAN_ELEMENT_INDEX(targetEnumerableSet, srcLeftIndex, midIndex, srcRightIndex);

	if (medianIndex != srcLeftIndex) //해당 중앙값이 왼쪽 인덱스가 아닐 경우
		SWAP(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[medianIndex], tmp);

	goto SORT_PROC;

SORT_PROC: //정렬 처리 루틴
	/***
		< 퀵 정렬 - 오름차순 >

		! SWAP 매크로 치환 시에 SWAP 대상 인덱스를 변경하지 않고, 이전에 먼저 변경 할 것

		1) 적절한 방법에 따라 기준 (pivot) 선정
		: 중앙값 선정 등에 의해 필요 할 경우 기준 (pivot)의 위치 SWAP 하여 맨 왼쪽으로 변경할 것

			1-1) 최초 기준 (pivot)의 최종 정렬 된 위치를 기준 (pivot)으로 할당

		2) 기준 (pivot)이 되는 요소를 제외한 나머지 요소들에 대해 왼쪽 인덱스와 오른쪽 인덱스가 최초 교차되는 시점까지 이하, 반복

		3) 현재 왼쪽 인덱스의 요소와 기준 (pivot)이 되는 요소 비교 (정렬 방향에 따라 비교 조건 변동)

			3-1) 현재 왼쪽 인덱스의 요소가 기준 (pivot)보다 작을 경우
			: 현재 왼쪽 인덱스의 요소는 기준 (pivot)이 되는 요소의 왼쪽으로 이동해야 함

				3-1-1) 기준 (pivot)의 최종 정렬 된 위치 증가

				3-1-2) 현재 왼쪽 인덱스의 요소와 기준 (pivot)의 최종 정렬 된 위치의 요소 SWAP
				: 이에 따라, 현재 기준 (pivot)의 최종 정렬 된 위치의 인덱스의 요소의 왼쪽은 항상 기준 (pivot)보다 작은 요소만 존재

			3-2) 현재 왼쪽 인덱스의 요소가 기준 (pivot)보다 크거나 같을 경우
			: do nothing

		4) 왼쪽 인덱스와 오른쪽 인덱스가 최초 교차되는 시점에 대해,
		: 현재 기준 (pivot)의 최종 정렬 된 위치의 인덱스의 요소의 왼쪽은 항상 기준 (pivot)보다 작은 요소만 존재

			4-1) 기준 (pivot)과 현재 기준 (pivot)의 최종 정렬 된 위치의 요소 SWAP

			4-2) 현재 기준 (pivot)의 최종 정렬 된 위치를 기준으로 다시 분할하기 위해 현재 기준 (pivot)의 최종 정렬 된 위치 반환
	***/

	pivotIndex = srcLeftIndex; //기준 (pivot) 인덱스
	orderedPivotIndex = pivotIndex; //기준 (pivot)의 최종 정렬 된 위치

	for (srcLeftIndex += 1; srcLeftIndex <= srcRightIndex; srcLeftIndex++)
	{
		switch (orderBy)
		{
		case ORDER_BY::ASCENDING:
			if (COMPARE(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[pivotIndex]) == -1) //left < pivot
			{
				orderedPivotIndex++;
				SWAP(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[orderedPivotIndex], tmp);
			}

			break;

		case ORDER_BY::DESCENDING:
			if (COMPARE(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[pivotIndex]) == 1) //left > pivot
			{
				orderedPivotIndex++;
				SWAP(targetEnumerableSet[srcLeftIndex], targetEnumerableSet[orderedPivotIndex], tmp);
			}

			break;
		}
	}

	SWAP(targetEnumerableSet[pivotIndex], targetEnumerableSet[orderedPivotIndex], tmp);
	return orderedPivotIndex;
}

/// <summary>
/// 퀵 정렬 (Best Case : O(nlog2(n)), Worst Case : O(n^2), Average Case : O(nlog2(n)))
/// </summary>
/// <typeparam name="SORT_ELEMENT_TYPE">정렬 요소 타입</typeparam>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="srcLeftIndex">순차적으로 열거 가능 한 요소들의 집합의 왼쪽 인덱스 (최초 0)</param>
/// <param name="srcRightIndex">순차적으로 열거 가능 한 요소들의 집합의 오른쪽 인덱스 (최초 요소들의 개수 - 1)</param>
/// <param name="orderBy">정렬 방향</param>
template<typename SORT_ELEMENT_TYPE>
void QuickSort(SORT_ELEMENT_TYPE targetEnumerableSet[],
	size_t srcLeftIndex, size_t srcRightIndex, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
#ifdef RECURSIVE_METHOD
	if (srcLeftIndex >= srcRightIndex) //base case
		return;

	size_t orderedPivotIndex =
		PartitioningProc<SORT_ELEMENT_TYPE>(targetEnumerableSet, srcLeftIndex, srcRightIndex, orderBy); //정렬 된 기준 (pivot)의 인덱스

	if (orderedPivotIndex > 0) //underflow 방지
		QuickSort<SORT_ELEMENT_TYPE>(targetEnumerableSet,
			srcLeftIndex, orderedPivotIndex - 1, orderBy); //정렬 된 기준 (pivot)의 왼쪽에 대해 다시 분할

	if (orderedPivotIndex < SIZE_MAX) //overflow 방지
		QuickSort<SORT_ELEMENT_TYPE>(targetEnumerableSet,
			orderedPivotIndex + 1, srcRightIndex, orderBy); //정렬 된 기준 (pivot)의 오른쪽에 대해 다시 분할

#elif defined ITERATIVE_METHOD
	/***
		이하, 구현사항은 RECURSIVE_METHOD의 실행 순서와 동일하지 않음
		RECURSIVE_METHOD와 동일 한 실행 순서는 LCRS 트리의 < RECURSIVE_METHOD의 Call Stack 구현 > 을 참조 할 것
	***/

	if ((srcRightIndex - srcLeftIndex) == 0)
		return;

	size_t* stack = new size_t[(srcRightIndex - srcLeftIndex) + 1]; //전체 요소 개수만큼의 스택
	size_t top = 0; //스택의 최상위 요소 인덱스
	
	//왼쪽 인덱스, 오른쪽 인덱스 순으로 push, 오른쪽 인덱스, 왼쪽 인덱스 순으로 pop
	stack[top++] = srcLeftIndex;
	stack[top++] = srcRightIndex;

	while (top > 0) //pop, PartitioningProc
	{
		srcRightIndex = stack[--top];
		srcLeftIndex = stack[--top];

		size_t orderedPivotIndex =
			PartitioningProc<SORT_ELEMENT_TYPE>(targetEnumerableSet, srcLeftIndex, srcRightIndex, orderBy); //정렬 된 기준 (pivot)의 인덱스

		if (orderedPivotIndex > 0) //underflow 방지
			if (orderedPivotIndex - 1 > srcLeftIndex) //정렬 된 기준 (pivot)의 왼쪽에 대해 요소가 존재 할 경우 다시 분할
			{
				stack[top++] = srcLeftIndex;
				stack[top++] = orderedPivotIndex - 1;
			}

		if (orderedPivotIndex < SIZE_MAX) //overflow 방지
			if (orderedPivotIndex + 1 < srcRightIndex) //정렬 된 기준 (pivot)의 오른쪽에 대해 요소가 존재 할 경우 다시 분할
			{
				stack[top++] = orderedPivotIndex + 1;
				stack[top++] = srcRightIndex;
			}
	}

	delete[] stack;
#endif
}

/// <summary>
/// 퀵 정렬 (Best Case : O(nlog2(n)), Worst Case : O(n^2), Average Case : O(nlog2(n)))
/// </summary>
/// <typeparam name="SORT_ELEMENT_TYPE">정렬 요소 타입</typeparam>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param name="orderBy">정렬 방향</param>
template<typename SORT_ELEMENT_TYPE>
void QuickSort(SORT_ELEMENT_TYPE targetEnumerableSet[],
	size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	if (elementCount <= 1)
		return;

	QuickSort<SORT_ELEMENT_TYPE>(targetEnumerableSet,
		0, elementCount - 1, orderBy);
}

/// <summary>
/// 병합 정렬
/// </summary>
/// <typeparam name="SORT_ELEMENT_TYPE"></typeparam>
/// <param name="targetEnumerableSet"></param>
/// <param name="elementCount"></param>
/// <param name="orderBy"></param>
template<typename SORT_ELEMENT_TYPE>
void MergeSort(SORT_ELEMENT_TYPE targetEnumerableSet[],
	size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	//TODO : 병합 정렬
}
#endif