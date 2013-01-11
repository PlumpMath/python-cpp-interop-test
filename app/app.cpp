#include <ExecutionManager.h>
#include <iostream>

int wmain(int argc, wchar_t* argv[])
{
	ExecutionManager em;
	std::cout << em.Execute(L"D.bar", 50) << std::endl;
	return 0;
}

