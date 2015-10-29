#pragma once

#include "Engine.h"
#include "boost/filesystem.hpp"


#pragma once
#include "Engine.h"

#include <Helper.h>

#include <stdexcept>



#ifdef WIN32
#define WIN32_MEAN_AND_LEAN 1
#include <Windows.h>
class SharedLibrary // windows impl
{
	using SharedLibHandle = HINSTANCE;
public:

	SharedLibrary() = default;
	SharedLibrary(const path_t& name);

	// make it move only
	SharedLibrary(const SharedLibrary& other) = delete;
	SharedLibrary(SharedLibrary&& other)
		:handle(other.handle)
		, name(other.name)
	{
		other.handle = nullptr;
		other.name = "";
	}

	const SharedLibrary& operator=(const SharedLibrary& other) = delete;
	const SharedLibrary& operator=(SharedLibrary&& other)
	{
		handle = other.handle;
		other.handle = nullptr;
		return *this;
	}

	template <typename FunctionType>
	ENGINE_API FunctionType* getFunctionPtr(const std::string& functionName);

	~SharedLibrary();

	const path_t& getName() { return name; }

private:
	SharedLibHandle handle = nullptr;
	path_t name;
};
#endif

struct ModuleHandler
{
	void init(const std::vector<path_t> modulesToLoad)
	{

		std::transform(modulesToLoad.begin(), modulesToLoad.end(), std::back_inserter(modules),
			[](const path_t& path)
		{
			return SharedLibrary{ L"modules\\" + path.wstring() };
		});

		for (auto&& elem : modules)
		{
			elem.getFunctionPtr<void()>("init")();
		}
	}

	std::vector<SharedLibrary> modules;
};
