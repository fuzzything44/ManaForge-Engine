#pragma once
#include "Engine.h"

#include <stdexcept>

#if defined _WIN32 || defined WIN32

class SharedLibrary
{
public:
	// typedef it to make sure we don't do any platform specific crap
	using SharedLibHandle = HINSTANCE;

	static SharedLibHandle Load(const std::string& path)
	{
		std::string pathWithExt = path + ".dll";

		SharedLibHandle handle = LoadLibraryA(pathWithExt.c_str());

		if (handle == nullptr)
		{
			
			logger<Fatal>() << "Failed to load library. Path: " << path;
		}

		return handle;
	}

	static void Unload(SharedLibHandle handle)
	{
		BOOL result = FreeLibrary(handle);
		if (result == FALSE)
		{
			throw std::runtime_error("Could not unload DLL");
		}
	}

	template<typename T>
	static T* getFunctionPtr(SharedLibHandle handle, const std::string& functionName)
	{
		// FARPROC is a generic fucntion pointer
		// gets the pointer to the function name specified
		FARPROC addr = GetProcAddress(handle, functionName.c_str());

		if (addr == nullptr)
		{
			logger<Fatal>() << "Failed to get function address. Name: " << functionName;
		}

		return reinterpret_cast<T*>(addr);
	}
};


#endif
