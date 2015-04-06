#include "stdafx.h"

#include "Engine.h"

#include "Module.h"


#include <iostream>

Module::Module(const std::string& filename) :
	registerModuleFunctionAddress(0),
	libraryHandle(0),
	refrenceCount(0)
{STACK
	try{

		// load the library
		libraryHandle = SharedLibrary::Load(filename);

		registerModuleFunctionAddress = SharedLibrary::getFunctionPtr<registerModuleFun>(libraryHandle, "registerModule");

	}
	catch (std::exception& e)
	{
		ENG_LOG(e.what())

	}

	refrenceCount = new int(1);
}

Module::Module(const Module& other) :
	libraryHandle(other.libraryHandle),
	refrenceCount(other.refrenceCount),
	registerModuleFunctionAddress(other.registerModuleFunctionAddress)
{STACK
	if (refrenceCount)
	{
		(*refrenceCount)++;
	}
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