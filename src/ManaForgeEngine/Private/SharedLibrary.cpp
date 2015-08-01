#include "EnginePCH.h"
#include "SharedLibrary.h"

#ifdef WIN32
SharedLibrary::SharedLibrary(const path_t & path)
{
	path_t pathWithExt = path.wstring() + (L".dll");

	handle = LoadLibraryW(pathWithExt.c_str());

	if (handle == nullptr)
	{

		MFLOG(Fatal) << "Failed to load library. Path: " << path;
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