#pragma once
#include "ExecutionManager.h"
#include <string>

class ScriptExecutable :
	public IExecutable
{
public:
	ScriptExecutable(const std::wstring& script_name);
	virtual ~ScriptExecutable(void);

	virtual int Execute(int param);

private:
	std::string script_name_;
};

