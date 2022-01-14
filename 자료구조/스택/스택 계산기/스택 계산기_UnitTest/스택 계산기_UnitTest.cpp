#include "pch.h"
#include "CppUnitTest.h"

#include "../Core.h"
#include "../Calculator.cpp"
#include "../LLS.cpp"

// 속성 매크로 : https://2ry53.tistory.com/entry/%EB%B9%84%EC%A5%AC%EC%96%BC-%EC%8A%A4%ED%8A%9C%EB%94%94%EC%98%A4-%EB%B9%8C%EB%93%9C-%EB%AA%85%EB%A0%B9-%EB%98%90%EB%8A%94-%EB%A7%A4%ED%81%AC%EB%A1%9CSolutionDir-ProjectFileName%EB%93%B1
// https://docs.microsoft.com/ko-kr/visualstudio/test/writing-unit-tests-for-c-cpp?view=vs-2022
// https://docs.microsoft.com/ko-kr/visualstudio/test/microsoft-visualstudio-testtools-cppunittestframework-api-reference?view=vs-2022#general_asserts
// https://stackoverflow.com/questions/47722333/linker-error-while-unit-testing-with-visual-studio-c

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace 스택_계산기_UnitTest
{
	TEST_CLASS(스택_계산기_UnitTest)
	{
	public:
		TEST_METHOD(CalcOperation_TestMethod)
		{
			double result1 = CalcOperation(1, '+', 2);
			double result2 = CalcOperation(1.0, SYMBOL_TYPE::PLUS, 2.0);

			Assert::AreEqual(result1, result2);
		}

		TEST_METHOD(CharToDecAscii_TestMethod)
		{
			int expected = (int)'a'; //예측값
			int actual = CharToDecAscii('a'); //실제값

			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(SingleNumToDecAscii_TestMethod)
		{
			for (int i = -1; i <= 10; i++)
			{
				if (i == -1 || i == 10) //경계값 검사
				{
					try
					{
						SingleNumToDecAscii(i);
						Assert::Fail();
					}
					catch (const std::invalid_argument& ex)
					{
						Logger::WriteMessage(ex.what()); //success
					}
				}
				else
				{
					Assert::AreEqual('0' + i, SingleNumToDecAscii(i));
				}
			}
		}

		TEST_METHOD(CharToSymbolType_TestMethod)
		{

		}

		TEST_METHOD(GenPostfixExpr_TestMethod)
		{
			const char* input = "1+3.334/(4.28*(110-7729))"; //입력값
			const char* expected = "1 3.334 4.28 110 7720 -*/+"; //예측값
			char actual[MAX_STR_LEN] = { '\0', }; //실제값

			//Infix Expr : 1+3.334/(4.28*(110-7729))
			//Postfix Expr : 1 3.334 4.28 110 7729 -*/+
			GenPostfixExpr(input, actual);
			Assert::AreEqual(expected, actual);

			//FIXME : 잘못 된 우선순위 할당? 연산 과정 논리 오류?
		}

		TEST_METHOD(CalcPostfixExpr_TestMethod)
		{

		}
	};
}