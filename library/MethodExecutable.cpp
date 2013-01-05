#include "MethodExecutable.h"

// TODO: load the dll, get proc address and execute

MethodExecutable::MethodExecutable(const std::wstring& dll_name)
{
	dll_ = LoadLibraryW(dll_name.c_str());
	exec_ = (ExecuteFn*)GetProcAddress(dll_, "Execute");
}

MethodExecutable::~MethodExecutable(void)
{
	if(dll_)
		FreeLibrary(dll_);
}

int MethodExecutable::Execute(int param)
{
	if(dll_ && exec_)
		return (*exec_)(param);
	return -1;
}
