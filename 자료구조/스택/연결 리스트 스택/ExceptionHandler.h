#ifndef _EXCEPTIONHANDLER_H_
#define _EXCEPTIONHANDLER_H_

namespace exceptionhandler
{
	typedef enum class EXCEPTION : const int
	{
		UNKNOWN_ERR = -200,
		NOT_ENOUGH_HEAP_MEMORY = -100,
		FAILED_TO_STRCPY = -101,
		NOT_ASSIGNED_STACK_ACCESS = -10,
		TRY_POP_ON_EMPTY_STACK = -2,
		TRY_PEEK_ON_EMPTY_STACK = -3,
	}EX;

	void ThrowException(EXCEPTION, bool = true);
}
#endif