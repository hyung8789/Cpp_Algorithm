#ifndef _COMMON_LIB_CORE_H_
#define _COMMON_LIB_CORE_H_

#include "Macro.h"
#include "MyException.h"
#include "Utils.h"

#define USE_CONSOLE_SCREEN_MANAGER
#ifdef USE_CONSOLE_SCREEN_MANAGER
#include "ConsoleScreenManager.h"
#endif

//#define IGNORE_VAR_DUMP
#ifdef IGNORE_VAR_DUMP
#define __VAR_DUMP(name, value, os) //do nothing
#endif
#endif