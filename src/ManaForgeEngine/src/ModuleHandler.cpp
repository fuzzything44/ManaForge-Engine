#include "EnginePCH.h"

#include "ModuleHandler.h"

#ifdef WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>


Module::Module(ModuleHandler& handler, const path_t& name)
	:name(name)
{
	path_t pathWithExt = L"modules\\" + name.wstring() + L".dll";

	handle = LoadLibraryW(pathWithExt.c_str());

	if (handle == nullptr) {
		MFLOGW(Error) << L"Failed to load library. Name: " << name.c_str() << L" Error: " << GetLastError();
	}

	// FARPROC is a generic fucntion pointer
	// gets the pointer to the function name specified
	InitFuncPtr_t addr = reinterpret_cast<InitFuncPtr_t>(GetProcAddress(handle, "init"));

	if (addr == nullptr) {
		MFLOG(Error) << "Failed to get init function address!";
	}

	addr(handler);
	
}


Module::~Module()
{
	if (handle)
	{
		BOOL result = FreeLibrary(handle);
		if (result == FALSE) {
			MFLOG(Error) << "Could not unload dll.";
		}
	}

}
#endif
