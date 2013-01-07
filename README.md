## Python-C++ interop test

The project tests the **interoperability of Python and C++ for the execution of algorithms in a uniform way**. 
It is a proof of concept for a module in another project. 

The framework works like this: 
- you can write a Python script that does something useful,
- you can write a DLL in C++ that does something useful,
- you can invoke either of them from the Python script (i.e. nest an executable), 
- you can run an executable (a script or a DLL function) from any python interpreter, 
- ...including an interpreter embedded in an application provided with a framework. 

### Sample script:

```python
# A.py
import library
for i in xrange(5):
  library.execute("B", 10)
```

### Sample DLL:

```cpp
// B.cpp
extern "C" int __cdecl Execute(int x)
{
  return x*x;
}
```

## Configuration

The project assumes the usage of Windows, Visual C++ 2010, x64 Release build config and Python 2.7.3. 
