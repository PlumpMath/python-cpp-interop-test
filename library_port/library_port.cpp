#include <Python.h>
#include <memory>
#include "LibraryPort.h"

std::auto_ptr<LibraryPort> port_;

PyMODINIT_FUNC init_internal()
{
	port_.reset(new LibraryPort());
}