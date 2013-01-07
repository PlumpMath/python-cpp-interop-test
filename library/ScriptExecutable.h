#pragma once
#include "ExecutionManager.h"
#include <string>
#include "LibraryPort.h"

class ScriptExecutable :
	public IExecutable
{
public:
	ScriptExecutable(const std::wstring& script_name, ExecutionManager* manager);
	virtual ~ScriptExecutable(void);

	virtual int Execute(int param);

private:
	std::string script_name_;
	LibraryModule port_;
};

