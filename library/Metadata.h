#pragma once
#include <json/json.h>
#include <stdlib.h>

enum ExecArgType {
	Int, 
	Float, 
	String
};

template < class Type >
inline ExecArgType _GetType();
template <> inline ExecArgType _GetType<long>() { return Int; }
template <> inline ExecArgType _GetType<double>() { return Float; }
template <> inline ExecArgType _GetType<std::wstring>() { return String; }
template <> inline ExecArgType _GetType<const wchar_t*>() { return String; }


static inline std::wstring _MakeWstr(const std::string& str)
{
	std::wstring wstr;
	wstr.resize(str.size()+1); // +1 for a null terminator
	size_t num_conv = 0;
	if(mbstowcs_s(&num_conv, &wstr[0], wstr.size(), &str[0], str.size()) != 0)
		return L"";
	wstr.resize(num_conv-1);
	return wstr;
}
static inline std::wstring _MakeWstr(const std::wstring& wstr)
{
	return wstr;
}

static std::string _MakeStr(const std::wstring& wstr)
{
	std::string str;
	str.resize(wstr.size()+1); // +1 for a null terminator
	size_t num_conv = 0;
	if(wcstombs_s(&num_conv, &str[0], str.size(), &wstr[0], wstr.size()) != 0)
		return "";
	str.resize(num_conv-1);
	return str;
}


class Metadata
{
public:
	Metadata(void);
	~Metadata(void);

	template < class Type, class StrType >
	Metadata& Add(const StrType& name, const Type& value)
	{
		Json::Value arg(Json::objectValue);
		arg[L"name"] = _MakeWstr(name);
		arg[L"type"] = (long)_GetType<Type>();
		arg[L"value"] = value;
		root_.append(arg);
		return *this;
	}
	std::wstring ToString() const
	{
		Json::FastWriter w;
		return w.write(root_);
	}
	template < class Fn1, class Fn2, class Fn3 >
	void ForEach(Fn1& fnInt, Fn2& fnFloat, Fn3& fnString) const
	{
		for(auto it=root_.begin(), end=root_.end(); it!=end; ++it)
		{
			const Json::Value& elem = (*it);
			const Json::Value& name = elem[L"name"];
			const Json::Value& type = elem[L"type"];
			const Json::Value& value = elem[L"value"];
			if(type.asInt() == Int)
				fnInt(name.asCString(), value.asLargestInt());
			else if(type.asInt() == Float)
				fnFloat(name.asCString(), value.asDouble());
			else if(type.asInt() == String)
				fnString(name.asCString(), value.asCString());
		}
	}
	bool IsEmpty() const
	{
		return root_.isNull();
	}
private:
	Json::Value root_;
};

