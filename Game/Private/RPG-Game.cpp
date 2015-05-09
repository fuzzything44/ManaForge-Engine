// local includes
#include "Engine.h"
#include "MainWindow.h"
#include "ENGException.h"
#include "Logging.h"

#include "Helper.h"

#include <fstream>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/list.hpp>

#if defined _WIN32 || defined WIN32
void changeDir()
{
	// changes the path so everything we open will be in Resoruce/
	char ownPth[MAX_PATH];

	// Will contain exe path
	HMODULE hModule = GetModuleHandle(nullptr);
	if (hModule == nullptr)
	{
		FATAL_ERR("getModuleHandle failed");
	}
	// When passing NULL to GetModuleHandle, it returns handle of exe itself
	GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));

	std::string path = ownPth;
	//path = path.substr(0, path.size() - 14);

	// remove the exe and the directory
	for (int i = 0; i < 3; i++){


		do {
			path = path.substr(0, path.size() - 1);
		} while (path[path.size() - 1] != '\\');

	}
	int err = _chdir(path.append("Resource\\").c_str());

	if (err)
	{
		FATAL_ERR("chdir failed. Error num: " + err);
	}

}
#elif defined __linux__

#error "Linux change directory code not implemented"

void changeDir()
{
	//TODO write linux code

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
	//path = path.substr(0, path.size() - 14);

	// remove the exe and the directory
	for (int i = 0; i < 3; i++){

		do {
			path = pathStr.substr(0, pathStr.size() - 1);
		} while (pathStr[pathStr.size() - 1] != '\\');

	}


	chdir(pathStr.c_str());

}
#endif

int32 main()
{
	changeDir();
	
	std::list<Actor*> list;
	list.push_back(new Actor(Transform(vec2(1.f, 323.f), 425.f, vec2(129.f, 3.33f)), true));
	list.push_back(new Actor(Transform(vec2(2.f, 63.f), 425.f, vec2(139.f, 23.2133f)), true));
	list.push_back(new Actor(Transform(vec2(5.f, 32.f), 452.f, vec2(914.f, 123.33f)), true));
	list.push_back(new Actor(Transform(vec2(6.f, 364.f), 415.f, vec2(139.f, 1233.33f)), true));
	list.push_back(new Actor(Transform(vec2(11.f, 33.f), 415.f, vec2(139.f, 43.33f)), true));
	list.push_back(new Actor(Transform(vec2(34.f, 33.f), 455.f, vec2(941.f, 3123.33f)), true));

	std::ofstream ostream{ "file.txt" };
	boost::archive::xml_oarchive oarch{ ostream };
	
	oarch << BOOST_SERIALIZATION_NVP(list);

	ostream.close();

	logging::init();

	auto window = MainWindow("RPG-Simulator", WindowMode::WINDOWED, uvec2(800, 600));

	try{

		// run the window. consumes the thread until it returns
		window.run();
	}
	catch (std::exception& e)
	{
		ENG_LOG(e.what());

		return -1;
	}
	
}


