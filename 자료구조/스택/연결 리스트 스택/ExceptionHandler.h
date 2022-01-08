#ifndef _EXCEPTIONHANDLER_H_
#define _EXCEPTIONHANDLER_H_

namespace exceptionhandler
{
	typedef enum class EXCEPTION : const int
	{
		UNKNOWN_ERR = -999,
		NOT_ENOUGH_HEAP_MEMORY = -10,
		MEM_LEAK = -11,
		INVALID_DATA = -12,
		FAILED_TO_STRCPY = -13,
		NOT_ASSIGNED_STACK_ACCESS = -1
	}EX;

	void ThrowCriticalException(EXCEPTION);
}
#endif