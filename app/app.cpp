#include <ExecutionManager.h>
#include <iostream>

int wmain(int argc, wchar_t* argv[])
{
	ExecutionManager em;
	std::cout << em.Execute(L"A", 50) << std::endl;
	std::cout << em.Execute(L"B", 50) << std::endl;
	return 0;
}

