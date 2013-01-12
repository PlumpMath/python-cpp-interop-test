#include "LibraryPort.h"
#include <Python.h>
#include "ExecutionManager.h"
#include <iostream>


const char module_name[] = "_internal";
const char module_docs[] = "library-Python internal interop module";
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
	PyObject* exec_args = NULL;
	if(!PyArg_ParseTuple(args, "sO", &what_to_exec, &exec_args))
		return (Py_INCREF(PyExc_TypeError), PyExc_TypeError);
	if(!what_to_exec)
		return (Py_INCREF(PyExc_TypeError), PyExc_TypeError);

	// convert into the C++ type
	if(!PyDict_Check(exec_args))
		return (Py_INCREF(PyExc_TypeError), PyExc_TypeError);


	PyObject* manager_capsule = self;
	ExecutionManager* manager = (ExecutionManager*)PyCapsule_GetPointer(manager_capsule, exec_mgr_name);
	if(manager)
		manager->Execute(std::wstring(what_to_exec, what_to_exec+strlen(what_to_exec)), param);

	Py_XDECREF(what_to_exec);
	Py_XDECREF(exec_args);

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

int LibraryModule::RunScript(const std::string& module, const std::string& func, int param)
{
	PyObject* func_name = PyString_FromString(func.c_str());
	PyObject* fromlist = Py_BuildValue("(s)", func_name);
	PyObject* module_obj = PyImport_ImportModuleEx((char*)module.c_str(), NULL, NULL, fromlist);

	if(!module_obj)
	{
		std::cout << "Couldn't import " << module << "." << func << std::endl;
		return -1;
	}

	PyObject* result = PyObject_CallMethod(module_obj, (char*)func.c_str(), "()");

	if(!result)
	{
		PyErr_Print();
		return -1;
	}

	Py_XDECREF(result);
	Py_XDECREF(module_obj);
	Py_XDECREF(fromlist);
	Py_XDECREF(func_name);

	return 0;
}
