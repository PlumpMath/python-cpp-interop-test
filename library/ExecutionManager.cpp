#include "ExecutionManager.h"
#include "ScriptExecutable.h"
#include "MethodExecutable.h"
#include <iostream>

// TODO: manage the execution of a script or a method

ExecutionManager::ExecutionManager()
{
	// The following can be auto-detected by searching .py's and .dll's. 
	executables_[L"D.foo"] = new ScriptExecutable(L"D", L"foo", this);
	executables_[L"D.bar"] = new ScriptExecutable(L"D", L"bar", this);
	executables_[L"C"] = new MethodExecutable(L"C");
}

ExecutionManager::~ExecutionManager()
{
	for(auto it=executables_.begin(); it!=executables_.end(); ++it)
		delete it->second;
	executables_.clear(); // extra-careful?
}

bool ExecutionManager::Execute(const std::wstring& name, const Metadata& args, Metadata& ret)
{
	auto it = executables_.find(name);
	if(it == executables_.end())
	{
		std::wcout << L"No " << name << L" in hte registry." << std::endl;
		return false;
	}
	return it->second->Execute(args, ret);
}
