#include <ExecutionManager.h>
#include <iostream>

int wmain(int argc, wchar_t* argv[])
{
	ExecutionManager em;
	Metadata in, out;
	if(em.Execute(L"D.bar", in, out))
		std::wcout << out.ToString() << std::endl;
	else
		std::cout << "failed" << std::endl;
	return 0;
}

