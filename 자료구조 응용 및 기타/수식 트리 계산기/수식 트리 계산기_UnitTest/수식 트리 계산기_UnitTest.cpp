#include "pch.h"
#include "CppUnitTest.h"

#include "../TokenGen.cpp"
#include "../EXPRT.cpp"

#define FP_DIFF_THRESHOLD 0.0001 //부동 소수점 차이 임계값 (epsilon)
#define length(array) ((sizeof(array)) / (sizeof(array[0])))

static const bool LOGGING_DEBUG_RESULT = true; //디버그용 결과 출력
static const bool LOGGING_EX = false; //예외 내용 출력

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

			ReverseInplaceStr(input);
			Assert::AreEqual(input, expected);
		}

		TEST_METHOD(GenNextToken_TestMethod)
		{
			const char* input = "(1.0 + 2.0)"; //입력값
			size_t inputLen = strlen(input);
			size_t currentReadCount = 0; //입력값에 대해 현재까지 읽은 개수

			Token token;

			while (currentReadCount < inputLen)
			{
				try
				{
					GenNextToken(&input[currentReadCount], &token, EXPR_READ_DIRECTION::LEFT_TO_RIGHT);

					if (LOGGING_DEBUG_RESULT)
						Logger::WriteMessage((std::string("Current Token Str (Left -> Right) : ") + std::string(token.str)).c_str());
				}
				catch (const std::exception& ex)
				{
					if (LOGGING_EX)
						Logger::WriteMessage(ex.what());

					Assert::Fail();
				}

				currentReadCount += token.readCount;
			}
		}
		TEST_METHOD(GenNextToken_Reverse_TestMethod)
		{
			const char* input = "(1.0 + 2.0)"; //입력값
			size_t inputLen = strlen(input);
			size_t currentReadCount = 0; //입력값에 대해 현재까지 읽은 개수

			Token token;

			while (currentReadCount < inputLen)
			{
				std::string slicedStr(input, 0, strlen(input) - currentReadCount);

				if (LOGGING_DEBUG_RESULT)
					Logger::WriteMessage((std::string("tmp sliced string : ") + slicedStr).c_str());

				try
				{
					GenNextToken(slicedStr.c_str(), &token, EXPR_READ_DIRECTION::RIGHT_TO_LEFT);

					if (LOGGING_DEBUG_RESULT)
						Logger::WriteMessage((std::string("Current Token Str (Right -> Left) : ") + std::string(token.str)).c_str());
				}
				catch (const std::exception& ex)
				{
					if (LOGGING_EX)
						Logger::WriteMessage(ex.what());

					Assert::Fail();
				}

				currentReadCount += token.readCount;
			}
		}
	};
}
