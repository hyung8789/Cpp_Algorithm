#include "pch.h"
#include "CppUnitTest.h"

#include "../Core.h"

/***
	! 이하, 단위 테스트 대상 프로젝트의 내부 디렉터리에 단위 테스트 프로젝트 생성 기준

	1) 단위 테스트 프로젝트 대상의 프로젝트 및 해당 프로젝트가 참조하는 모든 하위 프로젝트들을 참조로 추가 할 것
	: 해당 프로젝트가 참조하는 모든 하위 프로젝트들은 미리 해당 프로젝트 속성에서 정적 라이브러리 (lib)로 빌드 할 것

	2) 단위 테스트 프로젝트의 속성 - 링커 - 입력 - 추가 종속성에서 ..\x64\Debug\*.obj 로 단위 테스트 대상 프로젝트의 obj 를 포함시킬 것
	: obj - 컴파일러에 의한 출력 결과 파일, 이후 링커에 의해 obj들을 결합하여 정적 라이브러리 (lib), 동적 라이브러리 (dll), 실행 파일 (exe) 생성

	3) 단위 테스트 대상 프로젝트의 코어 헤더 파일만 포함
***/

static const bool LOGGING_DEBUG_RESULT = true; //디버그용 결과 출력
static const bool LOGGING_EX = true; //예외 내용 출력

// 속성 매크로 : https://2ry53.tistory.com/entry/%EB%B9%84%EC%A5%AC%EC%96%BC-%EC%8A%A4%ED%8A%9C%EB%94%94%EC%98%A4-%EB%B9%8C%EB%93%9C-%EB%AA%85%EB%A0%B9-%EB%98%90%EB%8A%94-%EB%A7%A4%ED%81%AC%EB%A1%9CSolutionDir-ProjectFileName%EB%93%B1
// https://docs.microsoft.com/ko-kr/visualstudio/test/writing-unit-tests-for-c-cpp?view=vs-2022
// https://docs.microsoft.com/ko-kr/visualstudio/test/microsoft-visualstudio-testtools-cppunittestframework-api-reference?view=vs-2022#general_asserts
// https://stackoverflow.com/questions/47722333/linker-error-while-unit-testing-with-visual-studio-c
// https://stackoverflow.com/questions/1322473/creating-lib-file-in-visual-studio
// https://stackoverflow.com/questions/66222196/using-native-cpp-testframework-in-visual-studio-to-test-c-code-lnk2019

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace 수식_트리_계산기_UnitTest
{
	TEST_CLASS(모듈_단위_테스트)
	{
	public:
		/// <summary>
		/// 부동 소수점으로 이루어진 대상 문자열을 부동 소수점으로 변환 테스트 메소드
		/// </summary>
		TEST_METHOD(StrToDouble_TestMethod)
		{
			try
			{
				double invalid = StrToDouble("invalid");
				Logger::WriteMessage((std::string("invalid data : ") + std::to_string(invalid)).c_str());
				Assert::Fail();
			}
			catch (const std::exception& ex)
			{
				if (LOGGING_EX)
					Logger::WriteMessage(ex.what());
			}

			try
			{
				double valid = StrToDouble("1.0");
				Logger::WriteMessage((std::string("valid data : ") + std::to_string(valid)).c_str());
			}
			catch (const std::exception& ex)
			{
				if (LOGGING_EX)
					Logger::WriteMessage(ex.what());

				Assert::Fail();
			}
		}
		/// <summary>
		/// 대상 문자열 제자리 좌우반전 테스트 메소드
		/// </summary>
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
		/// <summary>
		/// 대상 문자열을 기호 타입으로 변환에 대한 유효 데이터 테스트 메소드
		/// </summary>
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
				for (int i = 0; i < LENGTH(input); i++)
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
		/// <summary>
		/// 대상 문자열을 기호 타입으로 변환에 대한 무효 데이터 테스트 메소드
		/// </summary>
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
				for (int i = 0; i < LENGTH(input); i++)
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
		/// <summary>
		/// 대상 표현식으로부터 토큰 생성 테스트 메소드
		/// </summary>
		TEST_METHOD(GenNextToken_TestMethod)
		{
			const char* input = "7 1 * 5 2 - /"; //유효한 입력값
			size_t inputLen = strlen(input);
			size_t currentReadCount = 0; //입력값에 대해 현재까지 읽은 개수

			Token token;

			while (currentReadCount < inputLen)
			{
				try
				{
					GenNextToken(&input[currentReadCount], &token, EXPR_READ_DIRECTION::LEFT_TO_RIGHT);

					if (LOGGING_DEBUG_RESULT)
						Logger::WriteMessage((std::string("Current Token Str (Left -> Right) : ") + std::string(token._str)).c_str());
				}
				catch (const std::exception& ex)
				{
					if (LOGGING_EX)
						Logger::WriteMessage(ex.what());

					Assert::Fail();
				}

				currentReadCount += token._readCount;
			}
		}
		/// <summary>
		/// 대상 표현식으로부터 토큰 생성 역방향 테스트 메소드
		/// </summary>
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
						Logger::WriteMessage((std::string("Current Token Str (Right -> Left) : ") + std::string(token._str)).c_str());
				}
				catch (const std::exception& ex)
				{
					if (LOGGING_EX)
						Logger::WriteMessage(ex.what());

					Assert::Fail();
				}

				currentReadCount += token._readCount;
			}
		}
	};

	TEST_CLASS(통합_테스트)
	{
	public:
		/// <summary>
		/// 수식 트리 유효 데이터 통합 테스트 메소드
		/// </summary>
		TEST_METHOD(EXPRT_Valid_Integration_TestMethod)
		{
			const char* input[] =
			{
				"7 1*5 2-/", //(7 * 1) / (5 - 2) = 2.33333....
				"1 3.334 4.28 110 7729 - */ +", //1 + 3.334 / (4.28 * (110 - 7729)) = 0.99989775929
			}; //유효한 입력값 (후위 표현식)

			double expected[] =
			{
				2.33333,
				0.99989775929,
			}; //유효한 입력값에 대한 예측값

			try
			{
				Node* rootNode = NULL;
				char buffer[MAX_STR_LEN] = { '\0' };
				double result = 0.0;
				std::stringstream ss;

				for (int i = 0; i < LENGTH(input); i++)
				{
					memset(buffer, '\0', sizeof(buffer));
					memcpy(buffer, input[i], strlen(input[i]));

					if (LOGGING_DEBUG_RESULT)
						Logger::WriteMessage((std::string("current valid input : ") + std::string(buffer)).c_str());

					EXPRT_BulidTreeFromPostfixExpr(&rootNode, buffer);
					result = EXPRT_EvaluateTree(rootNode);

					if (LOGGING_DEBUG_RESULT)
					{
						EXPRT_DispOrderedTree(rootNode, TRAVERSAL_MODE::PREORDER, ss);
						Logger::WriteMessage((std::string("PREORDER : ") + std::string(ss.str())).c_str());
						ss.str(std::string());
						ss.clear();

						EXPRT_DispOrderedTree(rootNode, TRAVERSAL_MODE::INORDER, ss);
						Logger::WriteMessage((std::string("INORDER : ") + std::string(ss.str())).c_str());
						ss.str(std::string());
						ss.clear();

						EXPRT_DispOrderedTree(rootNode, TRAVERSAL_MODE::POSTORDER, ss);
						Logger::WriteMessage((std::string("POSTORDER : ") + std::string(ss.str())).c_str());
						ss.str(std::string());
						ss.clear();
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
		/// <summary>
		/// 수식 트리 무효 데이터 통합 테스트 메소드
		/// </summary>
		TEST_METHOD(EXPRT_Invalid_Integration_TestMethod)
		{
			const char* input[] =
			{
				"13.3344.281107729- */ +",
				"(123",
				".012+",
				"71*52-/",
			}; //무효한 입력값 (후위 표현식)

			try
			{
				Node* rootNode = NULL;
				char buffer[MAX_STR_LEN] = { '\0' };
				double result = 0.0;

				for (int i = 0; i < LENGTH(input); i++)
				{
					memset(buffer, '\0', sizeof(buffer));
					memcpy(buffer, input[i], strlen(input[i]));

					if (LOGGING_DEBUG_RESULT)
						Logger::WriteMessage((std::string("current invalid input : ") + std::string(buffer)).c_str());

					EXPRT_BulidTreeFromPostfixExpr(&rootNode, buffer);
					result = EXPRT_EvaluateTree(rootNode);
					EXPRT_DeallocateTree(&rootNode);

					Assert::Fail();
				}
			}
			catch (const std::exception& ex) //success
			{
				if (LOGGING_EX)
					Logger::WriteMessage(ex.what());
			}
		}
	};
}
