#include "ScriptExecutable.h"
#include <iostream>


std::string _conv(const std::wstring& wstr)
{
	std::string str;
	str.resize(wstr.size()+1); // +1 for a null terminator
	size_t num_conv = 0;
	if(wcstombs_s(&num_conv, &str[0], str.size(), &wstr[0], wstr.size()) != 0)
		return "";
	str.resize(num_conv-1);
	return str;
}


ScriptExecutable::ScriptExecutable(const std::wstring& script_name, const std::wstring& func_name, ExecutionManager* manager) :
	port_(manager)
{
	script_name_ = _conv(script_name);
	func_name_ = _conv(func_name);
}

ScriptExecutable::~ScriptExecutable(void)
{
}

int ScriptExecutable::Execute(int param)
{
	if(script_name_.empty())
		return -1;

// 	// load the script
// 	FILE* file = fopen((script_name_+".py").c_str(), "rt");
// 	if(!file)
// 		return -1;
// 	fseek(file, 0, SEEK_END);
// 	size_t file_size = ftell(file);
// 	fseek(file, 0, SEEK_SET);
// 	std::string str;
// 	str.resize(file_size);
// 	fread(&str[0], 1, file_size, file);
// 	fclose(file);

	// run the script
	return port_.RunScript(script_name_, func_name_, param);
}
