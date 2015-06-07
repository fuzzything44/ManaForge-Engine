#include "EnginePCH.h"

#include "Module.h"

#include "Logging.h"
#include "Helper.h"

#include <iostream>

Module::Module(const std::string& filename) :
	registerModuleFunctionAddress(nullptr),
	getModuleEngineVersionAddress(nullptr),
	name(std::string("Modules\\").append(filename)), // Append the modules prefiex
	libraryHandle(nullptr),
	refrenceCount(nullptr)
{
	try{

		// load the library
		libraryHandle = SharedLibrary::Load(name);

		registerModuleFunctionAddress = SharedLibrary::getFunctionPtr<registerModuleFun>(libraryHandle, "registerModule");

		getModuleEngineVersionAddress = SharedLibrary::getFunctionPtr < getModuleEngineVersionFun >
			(libraryHandle, "getModuleEngineVersion");


	}
	catch (std::exception& e)
	{
		ENG_LOGLN(e.what());

	}

	refrenceCount = new int(1);
}

Module::Module(const Module& other) :
	registerModuleFunctionAddress(other.registerModuleFunctionAddress),
	getModuleEngineVersionAddress(other.getModuleEngineVersionAddress),
	name(other.name),
	libraryHandle(other.libraryHandle),
	refrenceCount(other.refrenceCount)
{
	if (refrenceCount)
	{
		(*refrenceCount)++;
	}
}

void Module::registerModule(ModuleManager& mm)
{
	

	check(getModuleEngineVersionAddress);
	check(registerModuleFunctionAddress);

	if(getModuleEngineVersionAddress() != ENGINE_VERSION)
	{
		ENG_LOGLN("Module: " << name << " Needs to be rebuilt -- using old engine");
	};
	
	registerModuleFunctionAddress(mm);

	ENG_LOGLN("Module loaded: " << name);

}

Module::~Module()
{
	// deincrement refcount
	int remainingRefrences = --(*refrenceCount);

	if (remainingRefrences == 0)
	{
		try{

			SharedLibrary::Unload(libraryHandle);

			delete refrenceCount;

		}
		catch (std::exception& e)
		{
			ENG_LOGLN(e.what() << "\n");
		}
	}

}
