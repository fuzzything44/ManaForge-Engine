#pragma once

#include "Engine.h"
#include "boost/filesystem.hpp"

#include <Helper.h>

#include <stdexcept>

#include <boost/container/flat_map.hpp>

struct ModuleHandler;


class Module
{
	using SharedLibHandle = void*;
public:
	Module() = default;
	ENGINE_API Module(ModuleHandler& handler, const path_t& name);

	// make it move only
	Module(const Module&) = delete;
	Module(Module&& other)
		:handle(other.handle)
		, name(other.name)
		, handler(other.handler)
	{
		other.handle = nullptr;
		other.name = "";
		other.handler = nullptr;
	}

	const Module& operator=(const Module& other) = delete;
	const Module& operator=(Module&& other)
	{
		handler = other.handler;
		handle = other.handle;
		other.handle = nullptr;
		return *this;
	}

	ENGINE_API ~Module();

	const path_t& getName() const { return name; }

private:

	using InitFuncPtr_t = void(*)(ModuleHandler&);
	using CleanupFuncPtr_t = void(*)(ModuleHandler&);


	SharedLibHandle handle = nullptr;
	path_t name;
	
	ModuleHandler* handler;
};

inline bool operator<(const Module& lhs, const Module& rhs)
{
	return lhs.getName().wstring() < rhs.getName().wstring();
}


struct ModuleHandler
{
	void init(const std::vector<path_t> modulesToLoad)
	{
		for (auto&& elem : modulesToLoad)
		{
			modules.emplace(std::make_pair(elem, Module{ *this, elem }));
		}
	}

	void loadModule(const path_t& path)
	{
		auto&& iter = modules.find(path);

		if(iter == modules.end())
			modules.emplace(std::make_pair(path, Module{ *this, path }));
	}

	boost::container::flat_map<path_t, Module> modules;
};
