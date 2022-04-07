﻿#ifndef _HT_CORE_H_
#define _HT_CORE_H_

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <stdio.h>
#include <iostream>

#include <stdexcept>
#include <math.h>

//해당 프로젝트 속성에서 정적 라이브러리 (lib)로 빌드 할 것
#include "../../../common_lib/common_lib/Common_LIB_Core.h"

#define DEBUG_MODE
#ifdef DEBUG_MODE
static size_t hashCollisionCount = 0; //해시 충돌 횟수 카운트
#endif

#include "HT_Common.h"
#include "HT_Chaining.h"
#include "HT_Chaining_RBT.h"
#endif