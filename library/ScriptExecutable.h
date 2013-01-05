#pragma once
#include "ExecutionManager.h"

class ScriptExecutable :
	public IExecutable
{
public:
	ScriptExecutable(const std::wstring& script_name);
	virtual ~ScriptExecutable(void);

	virtual int Execute(int param);
};

