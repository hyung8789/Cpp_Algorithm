#ifndef _SORT_HPP_
#define _SORT_HPP_

#define LENGTH(array) ((sizeof(array)) / (sizeof(array[0])))
#define COMPARE(x, y) ((x) > (y) ? 1 : (x) == (y) ? 0 : -1) //x > y : 1, x == y : 0, x < y : -1
#define SWAP(x, y, tmp) ((tmp) = (x), (x) = (y), (y) = (tmp))

enum class ORDER_BY : const int
{
	ASCENDING, //오름차순
	DESCENDING //내림차순
}; //정렬 모드

/// <summary>
/// 버블 정렬
/// </summary>
/// <typeparam name="SortElementType">정렬 요소 타입</typeparam>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param name="orderBy">정렬 모드</param>
template<typename SortElementType>
void BubbleSort(SortElementType targetEnumerableSet[], size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	/***
		< 버블 정렬 - 오름차순 >

		1) 최초 비교 대상 범위는 순차적으로 열거 가능 한 요소들의 집합 내의 모든 요소들에 대해 시작

		2) 현재 비교 대상 범위 내에서 이웃 요소끼리 순차적으로 비교
		: 인덱스 0와 인덱스 1의 요소, 인덱스 1과 인덱스 2의 요소...

			2-1) 비교 중인 뒤의 요소가 이웃 한 바로 앞의 요소보다 작을 경우
			: 두 요소를 SWAP

		3) 현재 비교 대상 범위 내에서 이웃 요소들 간 비교 및 SWAP 이 끝났을 경우
		: 현재 비교 대상 범위 내의 마지막 요소는 정렬 된 조건을 만족하므로, 다음 비교 대상 범위에서 제외

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

		---

		집합 내에서 앞, 뒤의 요소 쌍의 합 == n
		집합 내에서 앞, 뒤의 요소 쌍의 개수 (n이 짝수 일 경우, 쌍이 되지 못한 나머지가 포함) == (n-1) / 2
		이므로,
		(n-1) + (n-2) + (n-3) + ... + (n-(n-2) + (n-(n-1)) == (n-1) + (n-2) + (n-3) + ... + 2 + 1 == n * ((n-1) / 2)
	***/

	SortElementType tmp;
	bool sortPerformed = false; //정렬 수행 여부

	for (size_t i = 0; i < (elementCount - 1); i++) //마지막 요소는 직접 접근 하지 않으며, 비교 시에만 간접 접근
	{
		sortPerformed = false;

		for (size_t j = 0; j < ((elementCount - 1) - i); j++) //현재 비교 대상 범위 내의 마지막 요소는 정렬 된 조건을 만족하므로, 다음 비교 대상 범위에서 제외
		{
			switch (orderBy)
			{
			case ORDER_BY::ASCENDING:
				if (COMPARE(targetEnumerableSet[j], targetEnumerableSet[j + 1]) == 1) //a > b
				{
					SWAP(targetEnumerableSet[j], targetEnumerableSet[j + 1], tmp);
					sortPerformed = true;
				}

				break;

			case ORDER_BY::DESCENDING:
				if (COMPARE(targetEnumerableSet[j], targetEnumerableSet[j + 1]) == -1) //a < b
				{
					SWAP(targetEnumerableSet[j], targetEnumerableSet[j + 1], tmp);
					sortPerformed = true;
				}

				break;
			}
		}

		if (!sortPerformed) //정렬이 한 번도 수행되지 않은 경우 (이미 내부 요소가 모두 정렬 되어 있을 경우)
			break;
	}
}

/// <summary>
/// 삽입 정렬
/// </summary>
/// <typeparam name="SortElementType">정렬 요소 타입</typeparam>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param name="orderBy">정렬 모드</param>
template<typename SortElementType>
void InsertionSort(SortElementType targetEnumerableSet[], size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{
	/***
	* TODO : 주석 수정
		< 삽입 정렬 - 오름차순 >

		1) 최초 비교 대상 범위는 순차적으로 열거 가능 한 요소들의 집합 내의 첫 번째 요소 ~ 두 번째 요소의 범위부터 시작

		2) 현재 비교 대상 범위 내에서 마지막 요소를 이웃 한 바로 앞의 요소와 비교

			2-1) 비교 중인 뒤의 요소가 이웃 한 바로 앞의 요소보다 작을 경우
			: 현재 비교 대상 범위 내에서 비교 중인 뒤의 요소가 삽입 될 적절한 위치 탐색

				2-1-1) 비교 중인 뒤의 요소가 삽입 될 빈 공간을 만들어서 해당 위치에 삽입
				: 삽입 될 위치의 기존 요소부터 비교 중인 뒤의 요소가 이웃 한 바로 앞의 요소 내의 범위의 요소들을 뒤로 한 칸씩 이동

			2-2) 비교 중인 뒤의 요소가 이웃 한 바로 앞의 요소보다 클 경우
			: do nothing

		3) 현재 비교 대상 범위 1 증가 및 2)로 이동

		---

		< 요소들 간 비교 횟수 >

		n : 순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수

		ex) 요소가 6개일 때, 첫 번째 루프에서 비교 범위 내의 두 요소를 1번 비교
		각 루프 완료 후 비교 범위 1씩 증가
		1 + 2 + 3 + 4 + 5
		== 6 (집합 내에서 앞, 뒤의 요소 쌍의 합) * 2 (집합 내에서 앞, 뒤의 요소 쌍의 개수) + 3 (쌍이 되지 못한 나머지)
		== 6 (집합 내에서 앞, 뒤의 요소 쌍의 합) * 2.5 (집합 내에서 앞, 뒤의 요소 쌍의 개수 (n이 짝수 일 경우, 쌍이 되지 못한 나머지가 포함))

		---

		1 + 2 + 3 + ... + (n-2) + (n-1)
		집합 내에서 앞, 뒤의 요소 쌍의 합 == n
		집합 내에서 앞, 뒤의 요소 쌍의 개수 (n이 짝수 일 경우, 쌍이 되지 못한 나머지가 포함) == (n-1) / 2
		이므로,
		1 + 2 + 3 + ... + (n-2) + (n-1) == (n-1) + (n-2) + ... + 2 + 1 == n * ((n-1) / 2)

		---

		버블 정렬과 비교하여 삽입 정렬은 이론 상 동일 한 비교 횟수를 가지나,

		1) 모든 데이터가 이미 정렬 되어 있는 상황 (Best Case)

			1-1) 버블 정렬
			: 최초, 루프의 비교 과정에서 한 번도 정렬이 발생하지 않았을 경우 (!sortPerformed),
			이는 이미 내부 데이터가 모두 정렬되어있음을 의미하고,
			이에 따라, 비교를 중단하므로 (n-1) 의 비교 횟수

			1-2) 삽입 정렬
			:


		2) 모든 데이터가 정렬하고자 하는 방법과 반대로 정렬 되어 있는 상황 (Worst Case)

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
						memmove(
							&targetEnumerableSet[j + 1],
							&targetEnumerableSet[j],
							sizeof(targetEnumerableSet[0]) * (i-j)
						);//삽입 될 위치의 기존 요소부터 비교 중인 뒤의 요소가 이웃 한 바로 앞의 요소 내의 범위의 요소들을 뒤로 한 칸씩 이동
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
						memmove(
							&targetEnumerableSet[j + 1],
							&targetEnumerableSet[j],
							sizeof(targetEnumerableSet[0]) * (i - j)
						);//삽입 될 위치의 기존 요소부터 비교 중인 뒤의 요소가 이웃 한 바로 앞의 요소 내의 범위의 요소들을 뒤로 한 칸씩 이동
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
/// 퀵 정렬
/// </summary>
/// <typeparam name="SortElementType">정렬 요소 타입</typeparam>
/// <param name="targetEnumerableSet">순차적으로 열거 가능 한 요소들의 집합</param>
/// <param name="elementCount">순차적으로 열거 가능 한 요소들의 집합의 요소들의 개수</param>
/// <param name="orderBy">정렬 모드</param>
template<typename SortElementType>
void QuickSort(SortElementType targetEnumerableSet[], size_t elementCount, ORDER_BY orderBy = ORDER_BY::ASCENDING)
{

}
#endif