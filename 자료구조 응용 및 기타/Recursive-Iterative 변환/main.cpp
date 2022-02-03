#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>

// https://www.cplusplus.com/reference/valarray/valarray/
// https://www.cplusplus.com/reference/valarray/slice/
#include <valarray> 

//#define RECURSIVE_METHOD //재귀적 방법 (주석 처리 : 반복적 방법 사용)
#ifndef RECURSIVE_METHOD
#define ITERATIVE_METHOD //반복적 방법
#endif

#define LENGTH(array) ((sizeof(array)) / (sizeof(array[0])))

// https://www.baeldung.com/cs/convert-recursion-to-iteration

/// <summary>
/// n번째 피보나치 수 반환 (피보나치 수는 첫째 및 둘째 항이 1이며 그 뒤의 모든 항은 바로 앞 두 항의 합인 수열, 처음 여섯 항은 1, 1, 2, 3, 5, 8)
/// </summary>
/// <param name="n">반환 할 피보나치 수의 항의 위치</param>
/// <returns>n번째 피보나치 수</returns>
unsigned int Fibonacci(unsigned int n)
{
#ifdef RECURSIVE_METHOD
	if (n == 0 || n == 1)
		return n;

	if (n == 2)
		return 1;

	return Fibonacci(n - 1) + Fibonacci(n - 2);

#elif defined ITERATIVE_METHOD
	int f1 = 1;
	int f2 = 1;

	for (int i = 2; i < n; i++) //앞의 두 항을 더하여 누적
	{
		int acc = f2 + f1;
		f1 = f2;
		f2 = acc;
	}

	return f2;
#endif
}

/// <summary>
/// n! (팩토리얼) 계산
/// </summary>
/// <param name="n">계승</param>
/// <param name="acc">누적 연산 결과 (Tail-Recursive)</param>
/// <returns>n!</returns>
unsigned int Factorial(unsigned int n, unsigned int acc = 1)
{
#ifdef RECURSIVE_METHOD
	if (n == 0) //base case
		return acc;

	/***
		Factorial(3) : n == 3, acc == 1
			=> Factorial(2, 3) : n == 2, acc == 3
				=> Factorial(1, 6) : n == 1, acc == 6
					=> Factorial(0, 6) : n == 0, acc == 6
	***/

	acc = n * acc;
	return Factorial(n - 1, acc);

#elif defined ITERATIVE_METHOD
	unsigned int retVal = 1;

	while (n > 0)
	{
		retVal = n * retVal;
		n--;
	}

	return retVal;
#endif
}

/// <summary>
/// 대상 배열 내의 요소들의 합 반환
/// https://www.baeldung.com/cs/tail-vs-non-tail-recursion
/// </summary>
/// <param name="srcArray">대상 배열</param>
/// <param name="acc">누적 연산 결과 (Tail-Recursive)</param>
/// <returns>대상 배열 내의 요소들의 합</returns>
int SumArray(std::valarray<int> srcArray, int acc = 0)
{
#ifdef RECURSIVE_METHOD
	if (srcArray.size() == 0) //base case (더 이상 분할이 불가능한 경우)
		return acc;

	/***
		모든 재귀 호출이 종료 시까지 각 호출자의 프레임이 Call Stack에 유지 될 경우,
		Stack Overflow, 메모리 사용량의 문제가 발생한다.
		이에 따라, 재귀 호출 시 각 호출자의 프레임을 Call Stack에 유지하지 않기 위해,
		각 호출자는 재귀 호출이 끝나는 시점에 단순히 결과를 반환만 한다. (Tail-Recursive)
	***/

	return SumArray(srcArray[std::slice(0, srcArray.size() - 1, 1)],
		acc + srcArray[srcArray.size() - 1]);

#elif defined ITERATIVE_METHOD
	int retVal = 0;

	for (size_t i = 0; i < srcArray.size(); i++)
	{
		retVal += srcArray[i];
	}

	return retVal;
#endif
}

int main()
{
	unsigned int FIBONACCI_N = 20;
	unsigned int FACTORIAL_N = 3;

	std::cout << FIBONACCI_N << "번째 피보나치 수 = " << Fibonacci(FIBONACCI_N) << std::endl;
	std::cout << FACTORIAL_N << "! = " << Factorial(FACTORIAL_N) << std::endl;

	int fooArray[] = { 1,2,3,4,5 };
	std::valarray<int> barArray(fooArray, LENGTH(fooArray));
	std::cout << "barArray 내의 요소들의 합 : " << SumArray(barArray) << std::endl;

	return EXIT_SUCCESS;
}