#ifndef _SEARCH_CORE_H_
#define _SEARCH_CORE_H

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <stdio.h>
#include <iostream>

#include <stdexcept>

#include <random> // https://www.cplusplus.com/reference/random/
#include <chrono> // https://www.cplusplus.com/reference/chrono/
#include <mutex> // https://www.cplusplus.com/reference/mutex
#include <future> // https://www.cplusplus.com/reference/future

//해당 프로젝트 속성에서 정적 라이브러리 (lib)로 빌드 할 것
#include "../../자료구조/리스트/이중 연결 리스트/DLL_Core.h";
#include "../../정렬/정렬/Sort.hpp";
#undef LOGGING_COMPARE_COUNT

#include "Search.hpp";
#endif