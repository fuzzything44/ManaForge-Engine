#include "EnginePCH.h"
#include "Engine.h"

#include "ChangeDirectory.h"

#include <boost/filesystem.hpp>

#if defined _WIN32 || defined WIN32
#include <Windows.h>

void changeDir()
{
	// changes the path so everything we open will be in Resoruce/
	char ownPth[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, ownPth);

	path_t path = ownPth;

	// remove the exe and the directory
	for (int i = 0; i < 2; i++) {
		assert(boost::filesystem::exists(path));
		path = path.parent_path();
	}
	path += "\\Resource\\";
	assert(boost::filesystem::exists(path));

	boost::filesystem::current_path(path.c_str());
}

#elif defined __linux__

void changeDir()
{
	// TODO write linux code

	return 0;
}

#elif defined __APPLE__
void changeDir()
{
	char path[1024];
	uint32_t size = sizeof(path);
	if (_NSGetExecutablePath(path, &size) == 0)
		printf("executable path is %s\n", path);
	else
		printf("buffer too small; need size %u\n", size);

	std::string pathStr = path;
	// path = path.substr(0, path.size() - 14);

	// remove the exe and the directory
	for (int i = 0; i < 3; i++) {

		do
		{
			path = pathStr.substr(0, pathStr.size() - 1);
		} while (pathStr[pathStr.size() - 1] != '\\');
	}

	boost::filesystem::current_path(path.append("Resources\\"));
}
#endif