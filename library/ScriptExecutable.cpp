#include "ScriptExecutable.h"
#include <Python.h>
#include <iostream>

ExecutionManager* g_manager = NULL;

static PyObject* execute(PyObject *self, PyObject *args)
{
	std::cout << "execute called!" << std::endl;
	std::cout << "self = " << self << std::endl; // a NULL, goddamit!
	std::cout << "args = " << args << std::endl;

	const char* what_to_exec = NULL;
	long param = -1;
	if(!PyArg_ParseTuple(args, "sl", &what_to_exec, &param))
		return (Py_INCREF(PyExc_TypeError), PyExc_TypeError);
	if(!what_to_exec)
		return (Py_INCREF(PyExc_TypeError), PyExc_TypeError);

	std::cout << "executing " << what_to_exec << "..." << std::endl;
	ExecutionManager* manager = g_manager;
	if(self)
	{
		PyObject* manager_capsule = PyObject_GetAttr(self, PyString_FromString("_exec_mgr"));
		manager = (ExecutionManager*)PyCapsule_GetContext(manager_capsule);
	}
	if(manager)
		manager->Execute(std::wstring(what_to_exec, what_to_exec+strlen(what_to_exec)), param);

	Py_RETURN_NONE;
}

static PyMethodDef module_methods[] = {
	{"execute", ::execute, METH_VARARGS, "Execute another script/calculation method by name."},
	{NULL, NULL, 0, NULL}
};


ScriptExecutable::ScriptExecutable(const std::wstring& script_name, ExecutionManager* manager) :
	manager_(manager)
{
	script_name_.resize(script_name.size()+1); // +1 for a null terminator
	size_t num_conv = 0;
	if(wcstombs_s(&num_conv, &script_name_[0], script_name_.size(), &script_name[0], script_name.size()) == 0)
		script_name_.resize(num_conv-1);
	else
		script_name_ = "";

	g_manager = manager_; // TODO: get rid of that
}

ScriptExecutable::~ScriptExecutable(void)
{
}

int ScriptExecutable::Execute(int param)
{
	if(script_name_.empty())
		return -1;

	FILE* file = fopen((script_name_+".py").c_str(), "rt");
	if(!file)
		return -1;
	fseek(file, 0, SEEK_END);
	size_t file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	std::string str;
	str.resize(file_size);
	fread(&str[0], 1, file_size, file);
	fclose(file);

	if(!Py_IsInitialized())
		Py_Initialize();

	PyObject* module = Py_InitModule("frames", module_methods);
	//PyObject* manager_capsule = PyCapsule_New(manager_, "ExecutionManager", NULL);
	//PyObject_SetAttr(module, PyString_FromString("_exec_mgr"), manager_capsule);

	int ret = PyRun_SimpleString(str.c_str());
	if(ret == -1)
		PyErr_Print();

	//Py_XDECREF(manager_capsule);
	Py_XDECREF(module);

	return ret;
}
