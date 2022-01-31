#ifndef _CORE_H_
#define _CORE_H_

// https://docs.microsoft.com/ko-kr/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2022
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <stdio.h>
#include <iostream>

#include <stdexcept> // https://en.cppreference.com/w/cpp/header/stdexcept
#include <cfenv> // https://www.cplusplus.com/reference/cfenv

#include "Utils.h"
#include "TokenGen.h"
#include "EXPRT.h"
#endif