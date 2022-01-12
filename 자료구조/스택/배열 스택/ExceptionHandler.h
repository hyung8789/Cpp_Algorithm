#ifndef _EXCEPTIONHANDLER_H_
#define _EXCEPTIONHANDLER_H_

namespace exceptionhandler
{
	typedef enum class EXCEPTION : const int
	{
		NOT_ENOUGH_HEAP_MEMORY = -10,
		MEM_LEAK = -11,
		NOT_ASSIGNED_STACK_ACCESS = -1,
		TRY_PUSH_ON_FULL_STACK = -2,
		TRY_POP_ON_EMPTY_STACK = -3,
		TRY_PEEK_ON_EMPTY_STACK = -4,
	}EX;

	void ThrowCriticalException(EXCEPTION);
}
#endif