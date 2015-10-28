#pragma once

#include "Engine.h"
#include "boost/filesystem.hpp"


#pragma once
#include "Engine.h"

#include <Helper.h>

#include <stdexcept>



#ifdef WIN32
class SharedLibrary // windows impl
{
	using SharedLibHandle = HINSTANCE;
public:

	SharedLibrary() = default;
	SharedLibrary(const path_t& name);

	// make it move only
	SharedLibrary(const SharedLibrary& other) = delete;
	SharedLibrary(SharedLibrary&& other) = default;

	const SharedLibrary& operator=(const SharedLibrary& other) = delete;
	const SharedLibrary& operator=(SharedLibrary&& other)
	{
		handle = other.handle;
		other.handle = nullptr;
		return *this;
	}

	template <typename FunctionType>
	FunctionType* getFunctionPtr(const std::string& functionName);

	~SharedLibrary();

	const path_t& getName() { return name; }

private:
	SharedLibHandle handle = nullptr;
	const path_t name;
};
#endif

struct ModuleHandler
{
	void init(const std::vector<path_t> modulesToLoad)
	{
		modules.resize(modulesToLoad.size());

		std::transform(modulesToLoad.cbegin(), modulesToLoad.cend(), modules.begin(), 
			[](const path_t& path)
		{
			return SharedLibrary{ L"modules\\" + path.wstring() };
		});
	}

	std::vector<SharedLibrary> modules;
};
