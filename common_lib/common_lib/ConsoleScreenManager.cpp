#include "ConsoleScreenManager.h"

CONSOLE_SCREEN_MANAGER* CONSOLE_SCREEN_MANAGER::_instance = NULL; //CONSOLE_SCREEN_MANAGER 고유 인스턴스 초기화

/// <summary>
/// 자신의 고유 인스턴스 참조 반환
/// </summary>
/// <returns>자신의 고유 인스턴스 참조</returns>
CONSOLE_SCREEN_MANAGER& CONSOLE_SCREEN_MANAGER::GetInstance()
{
	if (_instance == NULL)
		_instance = new CONSOLE_SCREEN_MANAGER();

	return *_instance;
}

/// <summary>
/// 자신의 고유 인스턴스 메모리 할당 해제
/// </summary>
void CONSOLE_SCREEN_MANAGER::Dispose()
{
	if (_instance != NULL)
	{
		delete _instance;
		_instance = NULL;
	}
}

/// <summary>
/// 대상 색으로 콘솔 텍스트 색상 설정
/// </summary>
/// <param name="color">대상 색</param>
void CONSOLE_SCREEN_MANAGER::SetConsoleTextColor(const unsigned color) const
{
	SetConsoleTextAttribute(this->_hConsoleOutput, color);
}

/// <summary>
/// 콘솔 텍스트 색상 복구
/// </summary>
void CONSOLE_SCREEN_MANAGER::UnsetConsoleTextColor() const
{
	SetConsoleTextAttribute(this->_hConsoleOutput, this->_oldWAttributes);
}

/// <summary>
/// CONSOLE_SCREEN_MANAGER 생성자
/// </summary>
CONSOLE_SCREEN_MANAGER::CONSOLE_SCREEN_MANAGER()
{
	this->_hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(this->_hConsoleOutput, &(this->_consoleScreenBuferInfo));
	this->_oldWAttributes = this->_consoleScreenBuferInfo.wAttributes;
}