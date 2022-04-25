#ifndef _HT_CORE_H_
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
#include "../../트리/Red-Black (레드-블랙) 트리/RBT_Core.h"

#define HT_DEBUG_MODE
#define DISP_COLLISION_KEY //충돌이 발생한 키 출력

#include "HT_Common.h"
#include "HT_Chaining.h"
#include "HT_Chaining_RBT.h"
#include "HT_OpenAddr.h"
#endif