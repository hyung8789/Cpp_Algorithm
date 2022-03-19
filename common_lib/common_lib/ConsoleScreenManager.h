﻿#ifndef _CONSOLE_SCREEN_MANAGER_H_
#define _CONSOLE_SCREEN_MANAGER_H_

#include <Windows.h>

/// <summary>
/// WinAPI 전경색
/// </summary>
enum class FG_COLOR : const unsigned
{
    FG_BLACK = 0,
    FG_BLUE = 1,
    FG_GREEN = 2,
    FG_CYAN = 3,
    FG_RED = 4,
    FG_MAGENTA = 5,
    FG_BROWN = 6,
    FG_LIGHTGRAY = 7,
    FG_GRAY = 8,
    FG_LIGHTBLUE = 9,
    FG_LIGHTGREEN = 10,
    FG_LIGHTCYAN = 11,
    FG_LIGHTRED = 12,
    FG_LIGHTMAGENTA = 13,
    FG_YELLOW = 14,
    FG_WHITE = 15
};

/// <summary>
/// WinAPI 배경색
/// </summary>
enum class BG_COLOR : const unsigned
{
    BG_NAVYBLUE = 16,
    BG_GREEN = 32,
    BG_TEAL = 48,
    BG_MAROON = 64,
    BG_PURPLE = 80,
    BG_OLIVE = 96,
    BG_SILVER = 112,
    BG_GRAY = 128,
    BG_BLUE = 144,
    BG_LIME = 160,
    BG_CYAN = 176,
    BG_RED = 192,
    BG_MAGENTA = 208,
    BG_YELLOW = 224,
    BG_WHITE = 240
};

/// <summary>
/// WinAPI 콘솔 창 관리자
/// </summary>
class CONSOLE_SCREEN_MANAGER
{
public:
    static CONSOLE_SCREEN_MANAGER& GetInstance();
    void Dispose();

    void SetConsoleTextColor(const unsigned);
    void UnsetConsoleTextColor();

private:
    CONSOLE_SCREEN_MANAGER();
    static CONSOLE_SCREEN_MANAGER* _instance;

private:
    HANDLE _hConsoleOutput; //콘솔 출력 핸들
    CONSOLE_SCREEN_BUFFER_INFO _consoleScreenBuferInfo; //콘솔 화면 버퍼

    WORD _oldWAttributes; //이전 문자 속성 값
};
#endif