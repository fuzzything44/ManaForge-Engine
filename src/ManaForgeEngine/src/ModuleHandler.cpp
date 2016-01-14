#include "EnginePCH.h"

#include "ModuleHandler.h"

#ifdef WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>


Module::Module(ModuleHandler& handler_, const path_t& name)
	:name(name)
	, handler(&handler_)
{
	path_t pathWithExt = name.wstring() + L".dll";

	handle = LoadLibraryW(pathWithExt.c_str());

	if (handle == nullptr) {
		MFLOGW(L"Failed to load library. Name: " << name.c_str() << L" Error: " << GetLastError());
	}

	// FARPROC is a generic fucntion pointer
	// gets the pointer to the function name specified
	InitFuncPtr_t addr = reinterpret_cast<InitFuncPtr_t>(GetProcAddress(handle, "init"));

	if (addr == nullptr) {
		MFLOG("Failed to get init function address!");
	}

	addr(*handler);
	
}


Module::~Module()
{
	if (handle)
	{
		
		// FARPROC is a generic fucntion pointer
		// gets the pointer to the function name specified
		CleanupFuncPtr_t addr = reinterpret_cast<CleanupFuncPtr_t>(GetProcAddress(handle, "cleanup"));
	
		if (addr == nullptr) {
			MFLOG(Error) << "Failed to get cleanup function address!";
		}
	
		addr(*handler);
		
		
		BOOL result = FreeLibrary(handle);
		if (result == FALSE) {
			MFLOG(Error) << "Could not unload dll.";
		}
	}

}

// START LINUX IMPL
///////////////////

#elif defined __GNUC__

#include <dlfcn.h>

Module::Module(ModuleHandler& handler_, const path_t& name)
	:name(name)
	, handler(&handler_)
{
	
	path_t pathWithExt = "lib" + name.string() + ".so";
	
	handle = dlopen(pathWithExt.string().c_str(), RTLD_NOW);
	
	
	if (handle == nullptr) {
		throw std::runtime_error("Failed to load library. Name: " + pathWithExt.string() + " Error: " + dlerror());
	}
	
	InitFuncPtr_t addr = reinterpret_cast<InitFuncPtr_t>(dlsym(handle, "init_module"));
	
	if (addr == nullptr) {
		throw std::runtime_error("Failed to get init_module function address in module: " + pathWithExt.string());
	}
	
	addr(*handler);
	
}


Module::~Module()
{
	
	if (handle)
	{
		InitFuncPtr_t addr = reinterpret_cast<InitFuncPtr_t>(dlsym(handle, "cleanup_module"));

		if (addr == nullptr) {
			throw std::runtime_error("Failed to get cleanup function address!");
		}
	
		addr(*handler);
	
		
		int result = dlclose(handle);
		if (result != 0) {
			throw std::runtime_error("Could not unload dll.");
		}
	}
	

}


#endif


 