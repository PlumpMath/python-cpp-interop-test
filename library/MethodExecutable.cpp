#include "MethodExecutable.h"

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

bool MethodExecutable::Execute(const Metadata& args, Metadata& ret)
{
	if(dll_ && exec_)
		return (*exec_)(args, ret);
	return false;
}
