#ifndef _SEARCH_CORE_H_
#define _SEARCH_CORE_H

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <stdio.h>
#include <iostream>

#include <random> // https://www.cplusplus.com/reference/random/
#include <chrono> // https://www.cplusplus.com/reference/chrono/
#include <mutex> // https://www.cplusplus.com/reference/mutex
#include <future> // https://www.cplusplus.com/reference/future

//해당 프로젝트 속성에서 정적 라이브러리 (lib)로 빌드 할 것
#include "../../common_lib/common_lib/Common_LIB_Core.h"
#include "../../자료구조/리스트/이중 연결 리스트/DLL_Core.h"

#include "Search.hpp"
#endif