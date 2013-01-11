#include "ExecutionManager.h"
#include "ScriptExecutable.h"
#include "MethodExecutable.h"

// TODO: manage the execution of a script or a method

ExecutionManager::ExecutionManager()
{
	// The following can be auto-detected by searching .py's and .dll's. 
	executables_[L"D.bar"] = new ScriptExecutable(L"B", L"bar", this);
	executables_[L"C"] = new MethodExecutable(L"C");
}

ExecutionManager::~ExecutionManager()
{
	for(auto it=executables_.begin(); it!=executables_.end(); ++it)
		delete it->second;
	executables_.clear(); // extra-careful?
}

int ExecutionManager::Execute(const std::wstring& name, int param)
{
	auto it = executables_.find(name);
	if(it == executables_.end())
		return -1;
	return it->second->Execute(param);
}
