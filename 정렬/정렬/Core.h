#ifndef _CORE_H_
#define _CORE_H_

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <stdio.h>
#include <iostream>

#include <random> // https://www.cplusplus.com/reference/random/
#include <chrono> // https://www.cplusplus.com/reference/chrono/
#include <mutex> // https://www.cplusplus.com/reference/mutex
#include <future> // https://www.cplusplus.com/reference/future
//#include <source_location> // https://en.cppreference.com/w/cpp/utility/source_location
// 
//해당 프로젝트 속성에서 정적 라이브러리 (lib)로 빌드 할 것
#include "../../lib/lib/LIB_Core.h"

#include "Sort.hpp"
#include "TraceResult.h"
#include "Trace.hpp"
#include "SortMapper.h"
#endif