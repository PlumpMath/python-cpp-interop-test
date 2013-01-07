#include <Python.h>
#include <memory>
#include "LibraryPort.h"

std::auto_ptr<LibraryPort> port_;

PyMODINIT_FUNC initlibrary()
{
	port_.reset(new LibraryPort());
}