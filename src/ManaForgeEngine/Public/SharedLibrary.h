#pragma once
#include "Engine.h"

#include <Helper.h>

#include <stdexcept>

#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#endif

class SharedLibrary
{
#if defined WIN32
	// typedef it to make sure we don't do any platform specific crap
	using SharedLibHandle = HINSTANCE;
#endif
  public:
	SharedLibrary()
		: handle(nullptr){};
	ENGINE_API explicit SharedLibrary(const std::wstring& name);

	// make it move only
	SharedLibrary(const SharedLibrary& other) = delete;
	SharedLibrary(SharedLibrary&& other) = default;

	const SharedLibrary& operator=(const SharedLibrary& other) = delete;
	const SharedLibrary& operator=(SharedLibrary&& other)
	{
		handle = other.handle;
		other.handle = nullptr;
		return *this;
	}

	ENGINE_API static void Unload(SharedLibHandle handle);

	template <typename T> inline T* getFunctionPtr(const std::string& functionName);

  private:
	SharedLibHandle handle;
};

#if defined WIN32
template <typename T> inline T* SharedLibrary::getFunctionPtr(const std::string& functionName)
{
	assert(handle);

	// FARPROC is a generic fucntion pointer
	// gets the pointer to the function name specified
	FARPROC addr = GetProcAddress(handle, functionName.c_str());

	if (addr == nullptr) {
		MFLOG(Fatal) << "Failed to get function address. Name: " << functionName;
	}

	return reinterpret_cast<T*>(addr);
}
#endif
