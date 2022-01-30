#include "pch.h"
#include "CppUnitTest.h"

#include "../TokenGen.cpp"
#include "../EXPRT.cpp"

#define FP_DIFF_THRESHOLD 0.0001 //부동 소수점 차이 임계값 (epsilon)
#define length(array) ((sizeof(array)) / (sizeof(array[0])))

static const bool LOGGING_DEBUG_RESULT = true; //디버그용 결과 출력
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
			char input[] = { '1','.','0','\0' }; //유효한 입력값
			const char* expected = "0.1"; //예측값

			try
			{
				ReverseInplaceStr(input);
			}
			catch (const std::exception& ex)
			{
				if (LOGGING_EX)
					Logger::WriteMessage(ex.what());

				Assert::Fail();
			}

			Assert::AreEqual(input, expected);
		}

		TEST_METHOD(StrToSymbolType_Valid_TestMethod)
		{
			const char* input[] =
			{
				"123.123",
				"+",
				"12345678",
				"("
			}; //유효한 입력값

			try
			{
				for (int i = 0; i < length(input); i++)
				{
					SYMBOL_TYPE actual = StrToSymbolType(input[i]);
					if (LOGGING_DEBUG_RESULT)
						Logger::WriteMessage((std::string(input[i]) + std::string(" : ") + std::string(1, (const char)actual)).c_str());

				}
			}
			catch (const std::exception& ex)
			{
				if (LOGGING_EX)
					Logger::WriteMessage(ex.what());

				Assert::Fail();
			}	
		}

		TEST_METHOD(StrToSymbolType_Invalid_TestMethod)
		{
			const char* input[] =
			{
				".123",
				"1..2",
				"1(",
				"123."
			}; //잘못 된 입력값

			try
			{
				for (int i = 0; i < length(input); i++)
				{
					SYMBOL_TYPE actual = StrToSymbolType(input[i]);
					Assert::Fail();
				}
			}
			catch (const std::exception& ex) //success
			{
				if (LOGGING_EX)
					Logger::WriteMessage(ex.what());
			}
		}

		TEST_METHOD(GenNextToken_TestMethod)
		{
			const char* input = "(1.0 + 2.0)"; //유효한 입력값
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

				currentReadCount += (token.readCount + token.ignoredCount);
			}
		}
		TEST_METHOD(GenNextToken_Reverse_TestMethod)
		{
			const char* input = "7 1 * 5 2 - /"; //유효한 입력값
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

				currentReadCount += (token.readCount + token.ignoredCount);
			}
		}
		
		TEST_METHOD(EXPRT_Valid_Integration_TestMethod)
		{
			const char* input[] =
			{
				"7 1*5 2-/" //2.33333....
			}; //유효한 입력값 (후위 표현식)
			
			double expected[] =
			{
				2.33333
			}; //유효한 입력값에 대한 예측값

			try
			{
				Node* rootNode = NULL;
				char buffer[MAX_STR_LEN] = { '\0' };
				double result = 0.0;
				std::ostringstream oss; //문자열 출력 스트림

				for (int i = 0; i < length(input); i++)
				{
					memset(buffer, '\0', sizeof(buffer));
					memcpy(buffer, input[i], sizeof(input[i]));
					
					EXPRT_BulidTreeFromPostfixExpr(&rootNode, buffer);
					result = EXPRT_EvaluateTree(rootNode);

					if (LOGGING_DEBUG_RESULT)
					{
						Logger::WriteMessage((std::string("current input : ") + std::string(input[i])).c_str());

						EXPRT_DispOrderedTree(rootNode, TRAVERSAL_MODE::PREORDER, oss);
						Logger::WriteMessage((std::string("PREORDER : ") + std::string(oss.str())).c_str());
						EXPRT_DispOrderedTree(rootNode, TRAVERSAL_MODE::INORDER, oss);
						Logger::WriteMessage((std::string("INORDER : ") + std::string(oss.str())).c_str());
						EXPRT_DispOrderedTree(rootNode, TRAVERSAL_MODE::POSTORDER, oss);
						Logger::WriteMessage((std::string("POSTORDER : ") + std::string(oss.str())).c_str());
					}

					Assert::AreEqual(expected[i], result, FP_DIFF_THRESHOLD);
					EXPRT_DeallocateTree(&rootNode);
				}
			}
			catch (const std::exception& ex)
			{
				if (LOGGING_EX)
					Logger::WriteMessage(ex.what());

				Assert::Fail();
			}
			
		}
	};
}
