#pragma once
#include "ExecutionManager.h"
#include <windows.h>

class MethodExecutable :
	public IExecutable
{
public:
	MethodExecutable(const std::wstring& dll_name);
	virtual ~MethodExecutable(void);

	virtual bool Execute(const Metadata& args, Metadata& ret);

private:
	typedef bool ExecuteFn(const Metadata& args, Metadata& ret);
	ExecuteFn* exec_;
	HMODULE dll_;
};

