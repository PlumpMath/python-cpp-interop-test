#pragma once
#include <string>
#include "ExecutionManager.h"


struct _object;
typedef _object PyObject;

class LibraryModule
{
public:
	LibraryModule(ExecutionManager* manager);
	~LibraryModule(void);

	bool RunScript(
		const std::string& module, 
		const std::string& func, 
		const Metadata& args, 
		Metadata& ret
		);

private:
	PyObject* module_;
};

class LibraryPort
{
public:
	LibraryPort() : exec_mgr_(), module_(&exec_mgr_) {}

	ExecutionManager exec_mgr_;
	LibraryModule module_;
};

