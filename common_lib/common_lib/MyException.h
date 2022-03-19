#ifndef _MY_EXCEPTION_H_
#define _MY_EXCEPTION_H_

#include <iostream>
#include <string>
#include <exception>

namespace myexception //사용자 정의 에외
{
	/// <summary>
	/// 사용자로부터 입력값에 대한 올바른 결과값을 찾지 못한 예외
	/// </summary>
	class NOT_FOUND_EXCEPTION : public std::runtime_error
	{
	public:
		NOT_FOUND_EXCEPTION(const std::string& what) : std::runtime_error(what)
		{}
	};

	/// <summary>
	/// 허용되지 않은 중복 데이터 예외
	/// </summary>
	class NOT_ALLOWED_DUPLICATE_DATA_EXCEPTION : public std::runtime_error
	{
	public:
		NOT_ALLOWED_DUPLICATE_DATA_EXCEPTION(const std::string& what) : std::runtime_error(what)
		{}
	};

	/// <summary>
	/// 메모리 손상, 메모리 침범, 메모리 변조, 잘못 된 연산에 의한 예상치 못한 데이터 예외
	/// </summary>
	class MEM_CORRUPTION_EXCEPTION : public std::runtime_error 
	{
	public:
		MEM_CORRUPTION_EXCEPTION(const std::string& what) : std::runtime_error(what)
		{}
	};
}
#endif