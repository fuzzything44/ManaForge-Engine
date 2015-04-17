#pragma once

#include "Engine.h"
#include "ENGException.h"
#include <stdexcept>

#if defined _WIN32 || defined WIN32

class SharedLibrary
{
public:
	// typedef it to make sure we don't do any platform specific crap
	typedef HINSTANCE SharedLibHandle;

	static SharedLibHandle Load(const std::string& path)
	{STACK
		std::string pathWithExt = path + ".dll";

		SharedLibHandle handle = LoadLibraryA(pathWithExt.c_str());

		if (handle == nullptr)
		{
			
			FATAL_ERR("Failed to load library", 2);
		}

		return handle;
	}

	static void Unload(SharedLibHandle handle)
	{STACK
		BOOL result = FreeLibrary(handle);
		if (result == FALSE)
		{
			throw std::runtime_error("Could not unload DLL");
		}
	}

	template<typename T>
	static T* getFunctionPtr(SharedLibHandle handle, const std::string& functionName)
	{STACK
		// FARPROC is a generic fucntion pointer
		// gets the pointer to the function name specified
		FARPROC addr = GetProcAddress(handle, functionName.c_str());

		if (addr == nullptr)
		{
			FATAL_ERR("Failed to get function address", -2);
		}

		return (T*) (addr);
	}
};


#endif