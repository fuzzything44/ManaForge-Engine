#include "EnginePCH.h"

#include "ModuleHandler.h"

#ifdef WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>


SharedLibrary::SharedLibrary(const path_t& name)
	:name(name)
{
	path_t pathWithExt = name.wstring() + L".dll";

	handle = LoadLibraryW(pathWithExt.c_str());

	if (handle == nullptr) {
		MFLOGW(Error) << L"Failed to load library. Name: " << name.c_str() << L" Error: " << GetLastError();
	}
}

template <typename FunctionType>
FunctionType* SharedLibrary::getFunctionPtr(const std::string& functionName)
{
	assert(handle);

	// FARPROC is a generic fucntion pointer
	// gets the pointer to the function name specified
	FARPROC addr = GetProcAddress(handle, functionName.c_str());

	if (addr == nullptr) {
		MFLOG(Fatal) << "Failed to get function address. Name: " << functionName;
	}

	return reinterpret_cast<FunctionType*>(addr);
}

SharedLibrary::~SharedLibrary()
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
