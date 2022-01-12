#include "LLS_Core.h"

void exceptionhandler::ThrowCriticalException(EXCEPTION ex)
{
	std::cout << "Exception Code : " << (const int)ex << std::endl;
	exit((const int)ex);
}