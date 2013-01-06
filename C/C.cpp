#include <iostream>

extern "C" int __cdecl Execute(int)
{
	std::cout << "Hello, I'm C." << std::endl;
	return 1;
}