#pragma once
#include <map>
#include <string>
#include "Metadata.h"

class IExecutable
{
public:
	virtual bool Execute(const Metadata& args, Metadata& ret) = 0;
	virtual ~IExecutable(void) {}
};

class ExecutionManager
{
public:
	ExecutionManager();
	~ExecutionManager();
	bool Execute(const std::wstring& name, const Metadata& args, Metadata& ret);

private:
	std::map<std::wstring, IExecutable*> executables_;
};