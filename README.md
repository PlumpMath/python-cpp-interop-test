# Python-C++ interop test

The project tests the interoperability of Python and C++ for the execution of algorithms in a uniform way. 
It is a proof of concept for a module in another project. 

The framework works like this: 
- you can write a Python script that does something useful,
- you can write a DLL in C++ that does something useful,
- you can invoke either of them from the Python script (i.e. nest an executable), 
- you can run an executable (a script or a DLL function) from any python interpreter, 
- ...including an interpreter embedded in an application provided with a framework. 

**Sample script: [A.py](https://github.com/malleor/python-cpp-interop-test/blob/master/build/A.py)**

**Sample DLL: [C.cpp](https://github.com/malleor/python-cpp-interop-test/blob/master/C/C.cpp)**
