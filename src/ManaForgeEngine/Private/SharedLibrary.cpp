#include "EnginePCH.h"
#include "SharedLibrary.h"

#ifdef WIN32
SharedLibrary::SharedLibrary(const std::wstring& name)
{
	std::wstring pathWithExt = name + L".dll";

	using namespace std::string_literals;

	handle = LoadLibraryW(pathWithExt.c_str());

	if (handle == nullptr)
	{

		MFLOG(Error) << "Failed to load library. Name: " << name.c_str();
	}
}
#endif

#ifdef WIN32
void SharedLibrary::Unload(SharedLibHandle handle)
{
	BOOL result = FreeLibrary(handle);
	if (result == FALSE)
	{
		throw std::runtime_error("Could not unload DLL");
	}
}
#endif