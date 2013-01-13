#include "LibraryPort.h"
#include <Python.h>
#include "ExecutionManager.h"
#include <iostream>
#include <stdio.h>
#include "Metadata.h"


const char module_name[] = "_internal";
const char module_docs[] = "library-Python internal interop module";
static PyObject* execute(PyObject *self, PyObject *args);
static PyMethodDef module_methods[] = {
	{"execute", ::execute, METH_VARARGS, "Execute another script/calculation method by name."},
	{NULL, NULL, 0, NULL}
};
const char exec_mgr_name[] = "_exec_mgr";


bool _DictToMetadata(PyObject* obj, Metadata& md)
{
	if(!PyDict_Check(obj))
	{
		PyErr_SetString(PyExc_TypeError, "the metadata object should be a dict");
		std::cout << "the metadata object should be a dict" << std::endl;
		return false;
	}
	PyObject *key, *value;
	Py_ssize_t pos = 0;
	Metadata exec_args_cpp;
	while(PyDict_Next(obj, &pos, &key, &value)) 
	{
		//std::cout << PyString_AS_STRING(key) << ": "; PyObject_Print(value, stdout, 0); std::cout << std::endl;

		if(PyInt_Check(value))
			md.Add(PyString_AS_STRING(key), PyInt_AS_LONG(value));
		else if(PyFloat_Check(value))
			md.Add(PyString_AS_STRING(key), PyFloat_AS_DOUBLE(value));
		else if(PyString_Check(value))
			md.Add(PyString_AS_STRING(key), _MakeWstr(PyString_AS_STRING(value)).c_str());
		else
		{
			PyErr_Format(PyExc_TypeError, "unsupported type of the value under the key '%s' (%s)", 
				PyString_AS_STRING(key), value->ob_type->tp_name);
			std::cout << "unsupported type of the value under the key '" << PyString_AS_STRING(key) << \
				"' (" << value->ob_type->tp_name << ")" << std::endl;
			return false;
		}
	}
	return true;
}

bool _MetadataToDict(const Metadata& md, PyObject* obj)
{
	md.ForEach(
		[&](const wchar_t* name, int i) { PyDict_SetItemString(obj, _MakeStr(name).c_str(), PyInt_FromLong(i)); }, 
		[&](const wchar_t* name, float f) { PyDict_SetItemString(obj, _MakeStr(name).c_str(), PyFloat_FromDouble(f)); }, 
		[&](const wchar_t* name, const wchar_t* s) { PyDict_SetItemString(obj, _MakeStr(name).c_str(), PyString_FromString(_MakeStr(s).c_str())); }
		);
	return true;
}


// called from the Python domain
static PyObject* execute(PyObject *self, PyObject *args)
{
	Metadata exec_args_cpp, exec_ret_cpp;
	PyObject* ret = NULL;
	const char* what_to_exec = NULL;
	PyObject* exec_args = NULL;
	long param = -1;
	PyObject* manager_capsule = self;

	if(!PyArg_ParseTuple(args, "sO", &what_to_exec, &exec_args))
	{
		PyErr_SetString(PyExc_TypeError, "need a function name and kwargs");
		goto error;
	}

	// convert Python args into C++ args
	if(!_DictToMetadata(exec_args, exec_args_cpp))
	{
		PyErr_SetString(PyExc_RuntimeError, "couldn't digest the arguments");
		goto error;
	}

	// call the thing
	ExecutionManager* manager = (ExecutionManager*)PyCapsule_GetPointer(manager_capsule, exec_mgr_name);
	if(!manager)
	{
		PyErr_SetString(PyExc_RuntimeError, "internal error - couldn't retreive the Execution Manager capsule");
		goto error;
	}
	std::wcout << L"calling '"<< _MakeWstr(what_to_exec) << L"' with args: " << exec_args_cpp.ToString() << std::endl;
	bool succ = manager->Execute(std::wstring(what_to_exec, what_to_exec+strlen(what_to_exec)), exec_args_cpp, exec_ret_cpp);
	if(succ)
	{
		std::cout << "'" << what_to_exec; std::wcout << L"' returned: " << exec_ret_cpp.ToString() << std::endl;
	}
	else
	{
		std::wcout << _MakeWstr(what_to_exec) << L" failed";
		PyErr_SetString(PyExc_RuntimeError, "function failed");
		goto error;
	}

	// convert results to a Python object
	ret = PyDict_New();
	if(!_MetadataToDict(exec_ret_cpp, ret))
	{
		PyErr_SetString(PyExc_RuntimeError, "couldn't form the result");
		goto error;
	}

error:
	Py_XDECREF(what_to_exec);
	Py_XDECREF(exec_args);

	return ret;
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

// called from the C++ domain
bool LibraryModule::RunScript(const std::string& module, const std::string& func, const Metadata& exec_args_cpp, Metadata& exec_ret_cpp)
{
	bool ret = false;
	PyObject* exec_args = NULL;
	PyObject* exec_ret = NULL;

	// import the module with the script
	PyObject* func_name = PyString_FromString(func.c_str());
	PyObject* fromlist = Py_BuildValue("(s)", func_name);
	PyObject* module_obj = PyImport_ImportModuleEx((char*)module.c_str(), NULL, NULL, fromlist);
	if(!module_obj)
	{
		std::cout << "couldn't import the '" << module << "' module";
		goto error;
	}
	PyObject* func_obj = PyObject_GetAttr(module_obj, func_name);
	if(!func_obj || !PyFunction_Check(func_obj))
	{
		std::cout << "couldn't fetch the function '" << module << "." << func << "'";
		goto error;
	}

	// prepare the arguments
	if(!exec_args_cpp.IsEmpty())
	{
		exec_args = PyDict_New();
		if(!_MetadataToDict(exec_args_cpp, exec_args))
		{
			std::cout << "couldn't digest the arguments" << std::endl;
			goto error;
		}
	}

	// call it
	std::wcout << L"calling '"<< _MakeWstr(func) << L"' with args: " << exec_args_cpp.ToString() << std::endl;
	PyObject* vargs = Py_BuildValue("()");
	//std::wcout << L"  object-wise: "<< (void*)func_obj << L"(" << (void*)exec_args << ")" << std::endl;
	exec_ret = PyObject_Call(func_obj, vargs, exec_args);
	Py_XDECREF(vargs);
	if(!exec_ret)
	{
		PyErr_Print();
		goto error;
	}

	// form the result
	if(Py_None!=exec_ret && !_DictToMetadata(exec_ret, exec_ret_cpp))
	{
		std::cout << "couldn't form the result" << std::endl;
		goto error;
	}

	ret = true;
error:
	Py_XDECREF(exec_ret);
	Py_XDECREF(exec_args);
	Py_XDECREF(module_obj);
	Py_XDECREF(fromlist);
	Py_XDECREF(func_name);

	return ret;
}
