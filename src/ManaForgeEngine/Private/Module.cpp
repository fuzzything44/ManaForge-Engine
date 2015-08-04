#include "EnginePCH.h"

#include "Module.h"

#include "Helper.h"

#include <iostream>

Module::Module(const std::string& filename)
    : registerModuleFunctionAddress(nullptr), getModuleEngineVersionAddress(nullptr),
      name("modules\\" + filename)  // Append the modules prefiex
{
	try
	{

		// load the library
		libraryHandle = SharedLibrary{logdetail::stringToWstring(name)};

		registerModuleFunctionAddress = libraryHandle.getFunctionPtr<registerModuleFun>("registerModule");

		getModuleEngineVersionAddress = libraryHandle.getFunctionPtr<getModuleEngineVersionFun>("getModuleEngineVersion");
	}
	catch (std::exception& e)
	{
		MFLOG(Fatal) << e.what();
	}
}


void Module::registerModule(ModuleManager& mm)
{


	assert(getModuleEngineVersionAddress);
	assert(registerModuleFunctionAddress);

	if (getModuleEngineVersionAddress() != ENGINE_VERSION) {
		MFLOG(Warning) << "Module: " << name << " Needs to be rebuilt -- using old engine";
	};

	registerModuleFunctionAddress(mm);

	MFLOG(Trace) << "Module loaded: " << name;
}

void Module::addClass(const std::string& name, const std::function<void*()>& fun) { classes.insert({name, fun}); }

void* Module::spawnClass(const std::string& className)
{
	auto iter = classes.find(className);

	if (iter != classes.end()) {
		return iter->second();
	}
	return nullptr;
}


Module::~Module() { classes.erase(classes.begin(), classes.end()); }
