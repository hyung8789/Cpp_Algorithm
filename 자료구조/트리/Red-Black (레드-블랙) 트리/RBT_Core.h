#ifndef _RBT_CORE_H_
#define _RBT_CORE_H_

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <stdio.h>
#include <iostream>

#include <stdexcept>

//#define STR_STORAGE_TREE_TYPE //문자열 저장 트리 타입
#define COLOR_VISUALIZATION //색상 가시화
#define RBT_DEBUG_MODE //유효성 검사

//해당 프로젝트 속성에서 정적 라이브러리 (lib)로 빌드 할 것
#include "../../../common_lib/common_lib/Common_LIB_Core.h"

#ifdef COLOR_VISUALIZATION
#ifndef USE_CONSOLE_SCREEN_MANAGER
#error Must define USE_CONSOLE_SCREEN_MANAGER at Common_LIB_Core.h
#endif
#endif

#ifdef RBT_DEBUG_MODE
#include <stack>
#include <tuple>
#endif

#include "RBT.h"
#endif