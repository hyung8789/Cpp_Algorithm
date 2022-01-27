#include "pch.h"
#include "CppUnitTest.h"

#include "../TokenGen.cpp"
#include "../EXPRT.cpp"

#define FP_DIFF_THRESHOLD 0.0001 //부동 소수점 차이 임계값 (epsilon)
#define length(array) ((sizeof(array)) / (sizeof(array[0])))
static const bool LOGGING_EX = true; //예외 내용 출력

// 속성 매크로 : https://2ry53.tistory.com/entry/%EB%B9%84%EC%A5%AC%EC%96%BC-%EC%8A%A4%ED%8A%9C%EB%94%94%EC%98%A4-%EB%B9%8C%EB%93%9C-%EB%AA%85%EB%A0%B9-%EB%98%90%EB%8A%94-%EB%A7%A4%ED%81%AC%EB%A1%9CSolutionDir-ProjectFileName%EB%93%B1
// https://docs.microsoft.com/ko-kr/visualstudio/test/writing-unit-tests-for-c-cpp?view=vs-2022
// https://docs.microsoft.com/ko-kr/visualstudio/test/microsoft-visualstudio-testtools-cppunittestframework-api-reference?view=vs-2022#general_asserts
// https://stackoverflow.com/questions/47722333/linker-error-while-unit-testing-with-visual-studio-c

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace 수식_트리_계산기_UnitTest
{
	TEST_CLASS(수식_트리_계산기_UnitTest)
	{
	public:
		TEST_METHOD(ReverseInplaceStr_TestMethod)
		{
			char input[] = { '1','.','0', '\0' };
			const char* expected = "0.1";

			ReverseInplaceStr(input, strlen(input));
			Assert::AreEqual(input, expected);

			char input2[] = { '\0' };
			try
			{
				ReverseInplaceStr(input2, strlen(input2));
				Assert::Fail();
			}
			catch (const std::invalid_argument& ex)
			{
				if (LOGGING_EX)
					Logger::WriteMessage(ex.what()); //success
			}
		}
	};
}
