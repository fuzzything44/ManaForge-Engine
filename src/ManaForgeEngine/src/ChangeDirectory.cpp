#include "EnginePCH.h"
#include "Engine.h"

#include "ChangeDirectory.h"

#include <boost/filesystem.hpp>

#include <iostream>

#if defined _WIN32 || defined WIN32
#include <Windows.h>

void changeDir()
{
	// changes the path so everything we open will be in Resoruce/
	char ownPth[MAX_PATH];

	GetModuleFileName(NULL, ownPth, MAX_PATH);

	path_t path = ownPth;

	// remove the exe and the directory
	for (int i = 0; i < 3; i++) {
		assert(boost::filesystem::exists(path));
		path = path.parent_path();
	}
	assert(path.filename() == "ManaForge-Engine");
	path += "\\Resource\\";
	assert(boost::filesystem::exists(path));

	boost::filesystem::current_path(path.c_str());
}

#elif defined __linux__

#include <unistd.h>
#include <limits.h>

void changeDir()
{
	char ownPth[PATH_MAX];
	readlink("/proc/self/exe", ownPth, PATH_MAX);

	path_t path = ownPth;

	path = path.parent_path().parent_path();

	if(path.filename() != "ManaForge-Engine")
	{
        std::cout << "Error: cound not find ManaForge-Engine folder! The folder that was found was " << path.string();
        return;
	}

	path += "/Resource/";

	if(!boost::filesystem::exists(path))
	{
        std::cout << "Error: could not find resource direcotry! The folder that was found was " << path.string();
        return;
	}
	boost::filesystem::current_path(path.c_str());


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
