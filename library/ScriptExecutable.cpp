#include "ScriptExecutable.h"
#include <iostream>
#include "Metadata.h"


ScriptExecutable::ScriptExecutable(const std::wstring& script_name, const std::wstring& func_name, ExecutionManager* manager) :
	port_(manager)
{
	script_name_ = _MakeStr(script_name);
	func_name_ = _MakeStr(func_name);
}

ScriptExecutable::~ScriptExecutable(void)
{
}

bool ScriptExecutable::Execute(const Metadata& args, Metadata& ret)
{
	if(script_name_.empty())
		return false;

	return port_.RunScript(script_name_, func_name_, args, ret);
}
