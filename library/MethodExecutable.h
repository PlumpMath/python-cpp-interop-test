#pragma once
#include "ExecutionManager.h"

class MethodExecutable :
	public IExecutable
{
public:
	MethodExecutable(const std::wstring& dll_name);
	virtual ~MethodExecutable(void);

	virtual int Execute(int param);
};

