#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>

// https://www.cplusplus.com/reference/valarray/valarray/
// https://www.cplusplus.com/reference/valarray/slice/
#include <valarray> 

//#define RECURSIVE_METHOD //����� ��� (�ּ� ó�� : �ݺ��� ��� ���)
#ifndef RECURSIVE_METHOD
#define ITERATIVE_METHOD //�ݺ��� ���
#endif

#define LENGTH(array) ((sizeof(array)) / (sizeof(array[0])))

// https://www.baeldung.com/cs/convert-recursion-to-iteration

/// <summary>
/// n��° �Ǻ���ġ �� ��ȯ (�Ǻ���ġ ���� ù° �� ��° ���� 1�̸� �� ���� ��� ���� �ٷ� �� �� ���� ���� ����, ó�� ���� ���� 1, 1, 2, 3, 5, 8)
/// </summary>
/// <param name="n">��ȯ �� �Ǻ���ġ ���� ���� ��ġ</param>
/// <returns>n��° �Ǻ���ġ ��</returns>
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

	for (int i = 2; i < n; i++) //���� �� ���� ���Ͽ� ����
	{
		int acc = f2 + f1;
		f1 = f2;
		f2 = acc;
	}

	return f2;
#endif
}

/// <summary>
/// n! (���丮��) ���
/// </summary>
/// <param name="n">���</param>
/// <param name="acc">���� ���� ��� (Tail-Recursive)</param>
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
/// ��� �迭 ���� ��ҵ��� �� ��ȯ
/// https://www.baeldung.com/cs/tail-vs-non-tail-recursion
/// </summary>
/// <param name="srcArray">��� �迭</param>
/// <param name="acc">���� ���� ��� (Tail-Recursive)</param>
/// <returns>��� �迭 ���� ��ҵ��� ��</returns>
int SumArray(std::valarray<int> srcArray, int acc = 0)
{
#ifdef RECURSIVE_METHOD
	if (srcArray.size() == 0) //base case (�� �̻� ������ �Ұ����� ���)
		return acc;

	/***
		��� ��� ȣ���� ���� �ñ��� �� ȣ������ �������� Call Stack�� ���� �� ���,
		Stack Overflow, �޸� ��뷮�� ������ �߻��Ѵ�.
		�̿� ����, ��� ȣ�� �� �� ȣ������ �������� Call Stack�� �������� �ʱ� ����,
		�� ȣ���ڴ� ��� ȣ���� ������ ������ �ܼ��� ����� ��ȯ�� �Ѵ�. (Tail-Recursive)
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

	std::cout << FIBONACCI_N << "��° �Ǻ���ġ �� = " << Fibonacci(FIBONACCI_N) << std::endl;
	std::cout << FACTORIAL_N << "! = " << Factorial(FACTORIAL_N) << std::endl;

	int fooArray[] = { 1,2,3,4,5 };
	std::valarray<int> barArray(fooArray, LENGTH(fooArray));
	std::cout << "barArray ���� ��ҵ��� �� : " << SumArray(barArray) << std::endl;

	return EXIT_SUCCESS;
}