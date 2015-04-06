#pragma once

#include "Engine.h"

#include "SharedLibrary.h"

class ModuleManager;

class Module
{

public:

	// typedef for the register plugin function type
	typedef void registerModuleFun(ModuleManager&);

	/// <summary> Registers the module with the ModuleManager specified</summary>
	///
	/// <param name="mm"> The module manager to register the module with</param>
	ENGINE_API void registerModule(ModuleManager& mm)
	{STACK
		registerModuleFunctionAddress(mm);
	}

	/// <summary> Modules the given file.</summary>
	///
	/// <param name="filename"> Filename of the module to load.</param>
	ENGINE_API Module(const std::string& filename);

	/// <summary> Copys the module from the one specified.</summary>
	///
	/// <param name="other"> The other.</param>
	ENGINE_API Module(const Module &other);

	/// <summary> Destructor.</summary>
	ENGINE_API ~Module();

private:
	// address to the function to register the plugin
	registerModuleFun* registerModuleFunctionAddress;

	SharedLibrary::SharedLibHandle libraryHandle;

	int* refrenceCount;
};