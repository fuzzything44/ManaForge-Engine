#pragma once

#include "Engine.h"

#include "SharedLibrary.h"

class ModuleManager;

class Module
{

public:

	// typedef for the register plugin function type
	typedef void registerModuleFun(ModuleManager&);

	ENGINE_API void registerModule(ModuleManager& mm)
	{
		registerModuleFunctionAddress(mm);
	}

	ENGINE_API Module(const std::string& filename);

	ENGINE_API Module(const Module &other);

	ENGINE_API ~Module();

private:
	// address to the function to register the plugin
	registerModuleFun* registerModuleFunctionAddress;

	SharedLibrary::SharedLibHandle libraryHandle;

	int* refrenceCount;
};