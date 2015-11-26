#pragma once

#include "Engine.h"
#include "boost/filesystem.hpp"

#include <Helper.h>

#include <stdexcept>
#include <string>
#include <map>

struct ModuleHandler;


class Module
{
	// I don't know of an operating system that doens't use a pointer for shared library handles.
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
		handler = std::move(other.handler);
		handle = std::move(other.handle);
		name = std::move(other.name);
		other.handle = nullptr;
		other.handler = nullptr;
		other.name = "";
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


struct ModuleHandler
{
	void init(const std::vector<path_t> modulesToLoad)
	{
		for (auto&& elem : modulesToLoad)
		{
			loadModule(elem);
		}
	}

	void loadModule(const path_t& path)
	{
		auto&& iter = modules.find(path);

		// if the module isn't a thing, then load it.
		if(iter == modules.end())
			modules.emplace(std::make_pair(path, Module{ *this, path }));
	}
	
	std::map<path_t, Module> modules;
};
