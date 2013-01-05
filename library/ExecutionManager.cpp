#include "ExecutionManager.h"
#include "ScriptExecutable.h"
#include "MethodExecutable.h"

// TODO: manage the execution of a script or a method

ExecutionManager::ExecutionManager()
{
	// The following can be auto-detected by searching .py's and .dll's. 
	executables_[L"A"] = new ScriptExecutable(L"A");
	executables_[L"B"] = new ScriptExecutable(L"B");
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
	return 0;
}
