#pragma once

#include "Engine.h"

#include <stdexcept>

#ifdef _WIN32
	#include <Windows.h>
#else
	#error "OOPS -- win only pls"
#endif

#ifdef _WIN32


class SharedLibrary
{
public:
	// typedef it to make sure we don't do any platform specific crap
	typedef HINSTANCE SharedLibHandle;

	static SharedLibHandle Load(const std::string& path)
	{STACK
		std::string pathWithExt = path + ".dll";

		SharedLibHandle handle = LoadLibraryA(pathWithExt.c_str());

		if (handle == NULL)
		{
			__debugbreak();
			throw std::runtime_error("Library Failed to load");
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

		if (addr == NULL)
		{
			__debugbreak();
			throw std::runtime_error("Cannot find function" + functionName);
		}

		return reinterpret_cast<T*>(addr);
	}
};


#endif