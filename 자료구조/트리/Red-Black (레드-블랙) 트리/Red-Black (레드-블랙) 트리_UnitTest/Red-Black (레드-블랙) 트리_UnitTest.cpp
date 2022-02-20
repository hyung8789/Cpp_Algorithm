#include "pch.h"
#include "CppUnitTest.h"

#include "../RBT_Core.h"

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

namespace RedBlack레드블랙트리UnitTest
{
	TEST_CLASS(모듈_단위_테스트)
	{
	public:
		
		/// <summary>
		/// 레드 블랙 트리 회전 테스트 메소드
		/// </summary>
		TEST_METHOD(RBT_RotateTree_TestMethod)
		{
			/***
			ex)
						8 (회전 대상 부모 노드)
					5				9
				3		6

				<= 5를 기준으로 좌회전 후,
				=> 8을 기준으로 우회전 후,

						5 (회전 대상 부모 노드)
				3				8
							6		9
			***/

			int inputData[] =
			{
				8,5,9,3,6
			};

			Assert::AreEqual(1, 1);
		}
	};
}
