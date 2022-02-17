#ifndef _MY_EXCEPTION_H_
#define _MY_EXCEPTION_H_

#include <exception>

namespace myexception //사용자 정의 에외
{
	class NOT_FOUND_EXCEPTION : public std::runtime_error
	{
	public:
		NOT_FOUND_EXCEPTION(const std::string& what) : std::runtime_error(what)
		{}
	};
}
#endif