#pragma once
#include "ExecutionManager.h"
#include <string>
#include "LibraryPort.h"

class ScriptExecutable :
	public IExecutable
{
public:
	ScriptExecutable(
		const std::wstring& script_name, 
		const std::wstring& func_name, 
		ExecutionManager* manager
		);
	virtual ~ScriptExecutable(void);

	virtual bool Execute(const Metadata& args, Metadata& ret);

private:
	std::string script_name_, func_name_;
	LibraryModule port_;
};

