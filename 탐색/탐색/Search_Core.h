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

//�ش� ������Ʈ �Ӽ����� ���� ���̺귯�� (lib)�� ���� �� ��
#include "../../�ڷᱸ��/����Ʈ/���� ���� ����Ʈ/DLL_Core.h";
#include "../../����/����/Sort.hpp";
#undef LOGGING_COMPARE_COUNT

#include "Search.hpp";
#endif