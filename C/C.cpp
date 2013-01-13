#include <iostream>
#include "Metadata.h"

extern "C" bool __cdecl Execute(const Metadata& args, Metadata& ret)
{
	std::cout << "C: I'm a null-device" << std::endl;
	ret = args;
	return true;
}