#include "ScriptExecutable.h"
#include <Python.h>
#include <iostream>

const char module_name[] = "library";
const char module_docs[] = "library-Python interop module.";
static PyObject* execute(PyObject *self, PyObject *args);
static PyMethodDef module_methods[] = {
	{"execute", ::execute, METH_VARARGS, "Execute another script/calculation method by name."},
	{NULL, NULL, 0, NULL}
};
const char exec_mgr_name[] = "_exec_mgr";



static PyObject* execute(PyObject *self, PyObject *args)
{
	const char* what_to_exec = NULL;
	long param = -1;
	if(!PyArg_ParseTuple(args, "sl", &what_to_exec, &param))
		return (Py_INCREF(PyExc_TypeError), PyExc_TypeError);
	if(!what_to_exec)
		return (Py_INCREF(PyExc_TypeError), PyExc_TypeError);

	PyObject* manager_capsule = self;
	ExecutionManager* manager = (ExecutionManager*)PyCapsule_GetPointer(manager_capsule, exec_mgr_name);
	if(manager)
		manager->Execute(std::wstring(what_to_exec, what_to_exec+strlen(what_to_exec)), param);

	Py_RETURN_NONE;
}

ScriptExecutable::ScriptExecutable(const std::wstring& script_name, ExecutionManager* manager) :
	manager_(manager)
{
	script_name_.resize(script_name.size()+1); // +1 for a null terminator
	size_t num_conv = 0;
	if(wcstombs_s(&num_conv, &script_name_[0], script_name_.size(), &script_name[0], script_name.size()) == 0)
		script_name_.resize(num_conv-1);
	else
		script_name_ = "";
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

	PyObject* manager_capsule = PyCapsule_New(manager_, exec_mgr_name, NULL);
	PyObject* module = Py_InitModule4(module_name, module_methods, module_docs, manager_capsule, PYTHON_API_VERSION);

	int ret = PyRun_SimpleString(str.c_str());
	if(ret == -1)
		PyErr_Print();

	// TODO: learn how and use this
	//Py_XDECREF(manager_capsule);
	//Py_XDECREF(module);

	return ret;
}
