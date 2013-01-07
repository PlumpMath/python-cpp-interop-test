#include "LibraryPort.h"
#include <Python.h>
#include "ExecutionManager.h"
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


LibraryModule::LibraryModule(ExecutionManager* manager)
{
	if(!Py_IsInitialized())
		Py_Initialize();

	PyObject* manager_capsule = PyCapsule_New(manager, exec_mgr_name, NULL);
	PyObject* module = Py_InitModule4(module_name, module_methods, module_docs, manager_capsule, PYTHON_API_VERSION);
	module_ = manager_capsule;
}

LibraryModule::~LibraryModule(void)
{
	// TODO: learn how and use this
	//Py_XDECREF(manager_capsule);
	//Py_XDECREF(module);
}

int LibraryModule::RunScript( const std::string& script_string, int param )
{
	int ret = PyRun_SimpleString(script_string.c_str());
	if(ret == -1)
		PyErr_Print();
	return ret;
}
