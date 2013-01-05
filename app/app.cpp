#include <ExecutionManager.h>

int wmain(int argc, wchar_t* argv[])
{
	return ExecutionManager().Execute(L"C", 50);
}

