#pragma once
#include "ExecutionManager.h"
#include <windows.h>

class MethodExecutable :
	public IExecutable
{
public:
	MethodExecutable(const std::wstring& dll_name);
	virtual ~MethodExecutable(void);

	virtual int Execute(int param);

private:
	typedef int ExecuteFn(int);
	ExecuteFn* exec_;
	HMODULE dll_;
};

