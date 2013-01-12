#pragma once
#include <map>
#include <string>
#include <afxwin.h>

typedef std::map<std::wstring, VARIANT> ExecutableArgs;

class IExecutable
{
public:
	virtual int Execute(int param) = 0;
	virtual ~IExecutable(void) {}
};

class ExecutionManager
{
public:
	ExecutionManager();
	~ExecutionManager();
	int Execute(const std::wstring& name, int param);

private:
	std::map<std::wstring, IExecutable*> executables_;
};