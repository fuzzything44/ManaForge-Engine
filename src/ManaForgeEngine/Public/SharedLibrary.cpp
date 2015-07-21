#include "EnginePCH.h"
#include "SharedLibrary.h"

SharedLibrary::SharedLibrary(const path_t & path)
{
#ifdef WIN32
	path_t pathWithExt = path.wstring() + (L".dll");

	handle = LoadLibraryW(pathWithExt.c_str());

	if (handle == nullptr)
	{

		logger<Fatal>() << "Failed to load library. Path: " << path;
	}
#endif
}

void SharedLibrary::Unload(SharedLibHandle handle)
{
#ifdef WIN32
	BOOL result = FreeLibrary(handle);
	if (result == FALSE)
	{
		throw std::runtime_error("Could not unload DLL");
	}
#endif
}
