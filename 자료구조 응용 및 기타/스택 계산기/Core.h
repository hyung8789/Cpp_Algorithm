#ifndef _CORE_H_
#define _CORE_H_

// https://docs.microsoft.com/ko-kr/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2022
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <stdio.h>
#include <iostream>

#include <cfenv> // https://www.cplusplus.com/reference/cfenv

//해당 프로젝트 속성에서 정적 라이브러리 (lib)로 빌드 할 것
#include "../../common_lib/common_lib/Common_LIB_Core.h"
#include "../../자료구조/스택/연결 리스트 스택/LLS_Core.h"

#include "TokenGen.h"
#include "SCalc.h"
#endif