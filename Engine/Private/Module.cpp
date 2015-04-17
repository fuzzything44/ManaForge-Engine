#include "Engine.h"
#include "Module.h"
#include "Logging.h"
#include "Helper.h"
#include <iostream>

Module::Module(const std::string& filename) :
	registerModuleFunctionAddress(nullptr),
	getModuleEngineVersionAddress(nullptr),
	name(std::string("Modules").append(filename)), // Append the modules prefiex
	libraryHandle(nullptr),
	refrenceCount(nullptr)
{STACK
	try{

		// load the library
		libraryHandle = SharedLibrary::Load(filename);

		registerModuleFunctionAddress = SharedLibrary::getFunctionPtr<registerModuleFun>(libraryHandle, "registerModule");

		getModuleEngineVersionAddress = SharedLibrary::getFunctionPtr < getModuleEngineVersionFun >
			(libraryHandle, "getModuleEngineVersion");
	}
	catch (std::exception& e)
	{
		ENG_LOG(e.what())

	}

	refrenceCount = new int(1);
}

Module::Module(const Module& other) :
	registerModuleFunctionAddress(other.registerModuleFunctionAddress),
	getModuleEngineVersionAddress(other.getModuleEngineVersionAddress),
	name(other.name),
	libraryHandle(other.libraryHandle),
	refrenceCount(other.refrenceCount)
{STACK
	if (refrenceCount)
	{
		(*refrenceCount)++;
	}
}

void Module::registerModule(ModuleManager& mm)
{
	STACK

	check(getModuleEngineVersionAddress);
	check(registerModuleFunctionAddress);

	if(getModuleEngineVersionAddress() != ENGINE_VERSION)
	{
		ENG_LOG("Module: " << name << " Needs to be rebuilt -- using old engine");
	};



	registerModuleFunctionAddress(mm);

}

Module::~Module()
{STACK
	// deincrement refcount
	int remainingRefrences = --(*refrenceCount);

	if (remainingRefrences == 0)
	{
		try{

			SharedLibrary::Unload(libraryHandle);
		}
		catch (std::exception& e)
		{
			ENG_LOG(e.what() << "\n");
		}
	}

}