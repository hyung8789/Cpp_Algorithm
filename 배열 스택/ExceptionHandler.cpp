#include "AS_Core.h"

void exceptionhandler::ThrowException(EXCEPTION ex, bool isCriticalException)
{
	std::cout << "Exception Code : " << (const int)ex << std::endl;
	
	if (isCriticalException)
	{
		exit((const int)ex);
	}
}