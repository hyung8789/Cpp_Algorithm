#ifndef _MY_EXCEPTION_H_
#define _MY_EXCEPTION_H_

#include <exception>

namespace myexception //����� ���� ����
{
	/// <summary>
	/// ����ڷκ��� �Է°��� ���� �ùٸ� ������� ã�� ���� ����
	/// </summary>
	class NOT_FOUND_EXCEPTION : public std::runtime_error
	{
	public:
		NOT_FOUND_EXCEPTION(const std::string& what) : std::runtime_error(what)
		{}
	};

	/// <summary>
	/// ������ ���� �ߺ� ������ ����
	/// </summary>
	class NOT_ALLOWED_DUPLICATE_DATA_EXCEPTION : public std::runtime_error
	{
	public:
		NOT_ALLOWED_DUPLICATE_DATA_EXCEPTION(const std::string& what) : std::runtime_error(what)
		{}
	};

	/// <summary>
	/// �޸� �ջ�, �޸� ħ��, �޸� ����, �߸� �� ���꿡 ���� ����ġ ���� ������ ����
	/// </summary>
	class MEM_CORRUPTION_EXCEPTION : public std::runtime_error 
	{
	public:
		MEM_CORRUPTION_EXCEPTION(const std::string& what) : std::runtime_error(what)
		{}
	};
}
#endif