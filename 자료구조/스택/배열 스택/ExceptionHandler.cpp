#include "AS_Core.h"

void exceptionhandler::ThrowException(EXCEPTION ex, bool terminate)
{
	std::cout << "Exception Code : " << (const int)ex << std::endl;
	
	if (terminate)
	{
		exit((const int)ex);
	}
}