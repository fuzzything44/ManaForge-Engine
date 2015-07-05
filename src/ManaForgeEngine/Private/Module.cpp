#include "EnginePCH.h"

#include "Module.h"

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

		registerModuleFunctionAddress = SharedLibrary::getFunctionPtr<registerModuleFun>
			(libraryHandle, "registerModule");

		getModuleEngineVersionAddress = SharedLibrary::getFunctionPtr<getModuleEngineVersionFun>
			(libraryHandle, "getModuleEngineVersion");


	}
	catch (std::exception& e)
	{
		logger<Fatal>() << e.what();

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
		logger<Warning>() << "Module: " << name << " Needs to be rebuilt -- using old engine";
	};
	
	registerModuleFunctionAddress(mm);

	logger<Trace>() << "Module loaded: " << name;

}

void Module::addClass(const std::string& name, const std::function<void*()>& fun)
{
	classes.insert({ name, fun });
}

void* Module::spawnClass(const std::string& className)
{
	auto iter = classes.find(className);

	if (iter != classes.end())
	{
		return iter->second();
	}
	return nullptr;
}


Module::~Module()
{
	// deincrement refcount
	int remainingRefrences = --(*refrenceCount);

	if (remainingRefrences == 0)
	{
		try{

			// make sure that these are deleted before the code is unloaded
			for (auto& elem : classes)
			{
				elem.second.~function();
			}

			SharedLibrary::Unload(libraryHandle);

			delete refrenceCount;


		}
		catch (std::exception& e)
		{
			logger<Error>() << e.what();
		}
	}

}
