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

namespace 스택_계산기_UnitTest
{
	TEST_CLASS(모듈_단위_테스트)
	{
	public:
		/// <summary>
		/// 대상 피연산자 1 및 대상 피연산자 2에 대해 연산자 문지에 따른 계산 결과 테스트 메소드
		/// </summary>
		TEST_METHOD(CalcOperation_TestMethod)
		{
			double result1 = CalcOperation(1, '+', 2);
			double result2 = CalcOperation(1.0, SYMBOL_TYPE::PLUS, 2.0);

			Assert::AreEqual(result1, result2);

			try
			{
				CalcOperation(1, '/', 0);
				Assert::Fail();
			}
			catch (const std::invalid_argument& ex) //success
			{
				if (LOGGING_EX)
					Logger::WriteMessage(ex.what());
			}
		}
		/// <summary>
		/// 대상 문자를 10진 아스키 코드로 변환 테스트 메소드
		/// </summary>
		TEST_METHOD(CharToDecAscii_TestMethod)
		{
			Assert::AreEqual((int)'a', utils::CharToDecAscii('a'));
			Assert::AreEqual((int)'z', utils::CharToDecAscii('z'));
		}
		/// <summary>
		/// 대상 0~9 범위의 단일 숫자를 10진 아스키 코드로 변환 테스트 메소드
		/// </summary>
		TEST_METHOD(SingleNumToDecAscii_TestMethod)
		{
			for (int i = -1; i <= 10; i++)
			{
				switch (i)
				{
				case -1:
				case 10:
					try //경계값 검사
					{
						utils::SingleNumToDecAscii(i);
						Assert::Fail();
					}
					catch (const std::invalid_argument& ex) //success
					{
						if (LOGGING_EX)
							Logger::WriteMessage(ex.what());
					}
					break;

				default:
					Assert::AreEqual('0' + i, utils::SingleNumToDecAscii(i));
					break;
				}
			}
		}
		/// <summary>
		/// 대상 문자를 기호 타입으로 변환 테스트 메소드
		/// </summary>
		TEST_METHOD(CharToSymbolType_TestMethod)
		{
			for (int i = 47; i <= 58; i++) //10진 아스키 코드 범위
			{
				switch (i)
				{
				case 47: //'/'
					Assert::AreEqual((const char)SYMBOL_TYPE::DIVIDE, (const char)CharToSymbolType(i));
					break;

				case 58: //':'
					try
					{
						CharToSymbolType(i);
						Assert::Fail();
					}
					catch (const std::invalid_argument& ex) //success
					{
						if (LOGGING_EX)
							Logger::WriteMessage(ex.what());
					}
					break;

				default:
					Assert::AreEqual((const char)SYMBOL_TYPE::OPERAND, (const char)CharToSymbolType(i));
					break;
				}
			}
		}
		/// <summary>
		/// 대상 중위 표현식으로부터 후위 표현식 생성 테스트 메소드
		/// </summary>
		TEST_METHOD(GenPostfixExpr_TestMethod)
		{
			const char* input = "1+3.334/(4.28*(110-7729))"; //입력값
			const char* expected = "1 3.334 4.28 110 7729 -*/+"; //예측값
			char actual[MAX_STR_LEN] = { '\0', }; //실제값

			GenPostfixExpr(input, actual);
			Assert::AreEqual(expected, actual);
		}
	};

	TEST_CLASS(통합_테스트)
	{
	public:
		/// <summary>
		/// 중위 표현식에 대한 유효 데이터 통합 테스트 메소드
		/// </summary>
		TEST_METHOD(InfixExpr_Valid_Integration_TestMethod)
		{
			const char* input[] =
			{
				"1 + 3.334 / (4.28 * (110 - 7729))"
			}; //유효 데이터 입력값

			double expected[] =
			{
				1 + 3.334 / (4.28 * (110 - 7729))
			}; //유효 데이터 입력값에 대한 예측값

			char buffer[MAX_STR_LEN] = { '\0', };

			for (int i = 0; i < LENGTH(input); i++)
			{
				try
				{
					memset(buffer, '\0', sizeof(buffer));
					Logger::WriteMessage(
						(std::string(__func__) + std::string(" : ") + std::string(input[i]))
						.c_str()
					);

					GenPostfixExpr(input[i], buffer);
					Assert::AreEqual(expected[i], CalcPostfixExpr(buffer), FP_DIFF_THRESHOLD);
				}
				catch (const std::exception& ex)
				{
					Logger::WriteMessage(
						(std::string("Failed at : ") + std::string(input[i]))
						.c_str()
					);
					if (LOGGING_EX)
						Logger::WriteMessage(ex.what());
					Assert::Fail();
				}
			}
		}
		/// <summary>
		/// 중위 표현식에 대한 무효 데이터 통합 테스트 메소드
		/// </summary>
		TEST_METHOD(InfixExpr_Invalid_Integration_TestMethod)
		{
			const char* input[] =
			{
				"(",
				"+12*",
				"1+*2",
				"(1+)2",
				")1 + 2",
				"1) + 2",
				"1 + )2",
				"1 + 2)",
				"((1 + 2",
				"(1(+2",
				"(1 + (2",
				"(1 + 2("
			}; //오류 데이터 입력값
			char buffer[MAX_STR_LEN] = { '\0', };

			for (int i = 0; i < LENGTH(input); i++)
			{
				try
				{
					memset(buffer, '\0', sizeof(buffer));
					Logger::WriteMessage(
						(std::string(__func__) + std::string(" : ") + std::string(input[i]))
						.c_str()
					);

					GenPostfixExpr(input[i], buffer);
					CalcPostfixExpr(buffer);

					Logger::WriteMessage(
						(std::string("Failed at : ") + std::string(input[i]))
						.c_str()
					);
					Assert::Fail();
				}
				catch (const std::exception& ex) //success
				{
					if (LOGGING_EX)
						Logger::WriteMessage(ex.what());
				}
			}
		}
	};
}