#include "../../common_lib/common_lib/Common_LIB_Core.h"
#include <sstream>

/// <summary>
/// 테스트용
/// </summary>
/// <param name="srcTargetArray">대상 배열</param>
/// <param name="srcRowElementCount">행에 대한 전체 요소 개수</param>
/// <param name="srcColElementCount">열에 대한 전체 요소 개수</param>
void Test(size_t srcTargetArray[], size_t srcRowElementCount, size_t srcColElementCount)
{
	int data = 0;

	for (size_t row = 0; row < srcRowElementCount; row++)
	{
		for (size_t col = 0; col < srcColElementCount; col++)
		{
			size_t index = utils::TwoDimensionIndexToOneDimensionIndex(srcRowElementCount, srcColElementCount, row, col);
			srcTargetArray[index] = data++;
		}
	}
}

/// <summary>
/// 배열 출력
/// </summary>
/// <param name="srcTargetArray">대상 배열</param>
/// <param name="srcRowElementCount">행에 대한 전체 요소 개수</param>
/// <param name="srcColElementCount">열에 대한 전체 요소 개수</param>
void DispArray(const size_t srcTargetArray[], size_t srcRowElementCount, size_t srcColElementCount)
{
	for (size_t rowIndex = 0; rowIndex < srcRowElementCount; rowIndex++)
	{
		for (size_t colIndex = 0; colIndex < srcColElementCount; colIndex++)
		{
			size_t index = utils::TwoDimensionIndexToOneDimensionIndex(srcRowElementCount, srcColElementCount, rowIndex, colIndex);
			std::cout << srcTargetArray[index] << "\t";
		}

		std::cout << "\n";
	}
}

/// <summary>
/// 이동 시퀸스에 따른 이동 방향
/// </summary>
enum class MOVE_DIRECTION : const int
{
	RIGHT = 0, //우
	BOTTOM, //하
	LEFT, //좌
	TOP, //상
	TOTAL_MOVE_DIRECTION_COUNT,
};

void CreateSnailArray(size_t srcTargetArray[], size_t srcRowElementCount, size_t srcColElementCount)
{
	/***
		ex) 행의 수 : 2, 열의 수 : 3

		0	1	2
		5	4	3

		1)	0	1	2	=> 열에 대해 3번 이동
		2)	3	=> 행에 대해 1번 이동
		3)	4	5	=> 열에 대해 2번 이동

		---

		ex) 행의 수 : 3, 열의 수 : 2

		0	1
		5	2
		4	3

		1)	0	1	=> 열에 대해 2번 이동
		2)	2	3	=> 행에 대해 2번 이동
		3)	4	=> 열에 대해 1번 이동
		4)	5	=> 행에 대해 1번 이동

		---

		ex) 행의 수 : 3, 열의 수 : 3

		0	1	2
		7	8	3
		6	5	4

		1)	0	1	2	=> 열에 대해 3번 이동
		2)	3	4	=> 행에 대해 2번 이동
		3)	5	6	=> 열에 대해 2번 이동
		4)	7	=> 행에 대해 1번 이동
		5)	8	=> 열에 대해 1번 이동

		---

		! 전체 요소 개수 : n

		1) 열에 대해 n번 우로 이동
		: 이동 완료 후 열의 위치 고정

		2) 행에 대해 n-1번 하로 이동
		: 이동 완료 후 행에 대해 위치 고정

		3) 열에 대해 n-1번 좌로 이동
		: 이동 완료 후 열의 위치 고정

		4) 행에 대해 n-2번 상으로 이동
		: 이동 완료 후 행의 위치 고정

		...

		- 이동 시퀸스 : 우 => 하 => 좌 => 상
	***/

	int data = 0;

	size_t leftIndex = 0; //좌측 시작 인덱스
	size_t rightIndex = srcColElementCount - 1; //우측 끝 인덱스
	size_t topIndex = 0; //상단 시작 인덱스
	size_t bottomIndex = srcRowElementCount - 1; //하단 끝 인덱스

	MOVE_DIRECTION moveDirection = MOVE_DIRECTION::RIGHT; //이동 방향

	while (leftIndex <= rightIndex && topIndex <= bottomIndex) //이동 가능 한 범위 내에서
	{
		switch (moveDirection)
		{
		case MOVE_DIRECTION::RIGHT:
			for (size_t col = leftIndex; col <= rightIndex; col++)
			{
				size_t index = utils::TwoDimensionIndexToOneDimensionIndex(srcRowElementCount, srcColElementCount, topIndex, col); //상단 행 고정
				srcTargetArray[index] = data++;
			}
			
			topIndex++; //상단 행 제외
			break;

		case MOVE_DIRECTION::BOTTOM:
			for (size_t row = topIndex; row <= bottomIndex; row++)
			{
				size_t index = utils::TwoDimensionIndexToOneDimensionIndex(srcRowElementCount, srcColElementCount, row, rightIndex); //우측 열 고정
				srcTargetArray[index] = data++;
			}

			rightIndex--; //우측 열 제외
			break;

		case MOVE_DIRECTION::LEFT:
			for (size_t col = rightIndex; col >= leftIndex; col--)
			{
				size_t index = utils::TwoDimensionIndexToOneDimensionIndex(srcRowElementCount, srcColElementCount, bottomIndex, col); //하단 행 고정
				srcTargetArray[index] = data++;

				if (col == 0) //underflow 방지
					break;
			}

			bottomIndex--; //하단 행 제외
			break;

		case MOVE_DIRECTION::TOP:
			for (size_t row = bottomIndex; row >= topIndex; row--)
			{
				size_t index = utils::TwoDimensionIndexToOneDimensionIndex(srcRowElementCount, srcColElementCount, row, leftIndex); //좌측 열 고정
				srcTargetArray[index] = data++;

				if (row == 0) //underflow 방지
					break;
			}
	
			leftIndex++; //좌측 열 제외
			break;
		}

		moveDirection = (MOVE_DIRECTION)(((const int)moveDirection + 1) % (const int)MOVE_DIRECTION::TOTAL_MOVE_DIRECTION_COUNT);
	}
}

int main()
{
	/***
		입력 : 공백으로 구분 된 행과 열에 대한 요소 개수

		출력 :
		ex) 행의 수 : 4, 열의 수 : 4

		0	1	2	3
		11	12	13	4
		10	15	14	5
		9	8	7	6
	***/

	try
	{
		size_t rowElementCount, colElementCount; //행과 열에 대한 요소 개수
		std::string inputStr;

		std::cout << "공백으로 구분 된 행과 열에 대한 요소 개수 (행, 열) >>";
		std::getline(std::cin, inputStr);
		std::stringstream ss(inputStr);
		std::cout << "\n";

		ss >> rowElementCount;
		ss >> colElementCount;

		size_t* fooArray = new size_t[rowElementCount * colElementCount];

		//Test(fooArray, rowElementCount, colElementCount); //테스트용
		CreateSnailArray(fooArray, rowElementCount, colElementCount);
		DispArray(fooArray, rowElementCount, colElementCount);

		delete[] fooArray;
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}