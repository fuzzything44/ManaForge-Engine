

#include "DefaultWorld.h"

#include <Helper.h>
#include <ImageLoader.h>
#include <PropertyManager.h>
#include <Runtime.h>
#include <Color.h>

#include <list>
#include <sstream>
#include <fstream>

#include <boost/lexical_cast.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>


World* AddWorld(std::string folder)
{
	return new DefaultWorld(folder);
}



DefaultWorld::DefaultWorld(std::string folder)
	:folderLocation(std::string("Worlds\\") + folder + '\\'),
	propManager(folderLocation + "world.json")
{

	// Make sure a world folder was supplied.
	if (folder == "") {
		FATAL_ERR("No world loaded");
	}

	ENG_LOG("Loading images...");
	
	// We should probably just have the images we use in the same file as chunk size.
	std::ifstream stream{ folderLocation + "images.txt" };
	boost::archive::xml_iarchive arch{ stream };

	std::map<Color, std::string> valuePairs;

	// load the map from the file
	arch >> BOOST_SERIALIZATION_NVP(valuePairs);

	// Give images to renderer
	//Runtime::get().moduleManager.getRenderer().loadTextures(valuePairs);

	ENG_LOG("Images loaded!");

	// virtual functuion -- call THIS version of it
	DefaultWorld::loadWorld("main");

	mainWorld = currentWorld;

}

void DefaultWorld::loadWorld(std::string name)
{
	ENG_LOG("Loading world " << name << "...");
	// Skip re-loading main world if possible.
	if (name == "main" && mainWorld != nullptr) {
		currentWorld = mainWorld;
	}
	else {
		
		

		try{
		// load static actors
		

			std::ifstream static_stream{ folderLocation + name + '\\' + name + ".WORLD" };
			if (!static_stream.is_open())
			{
				FATAL_ERR("COULD NOT OPEN STATIC ACTORS");
			}

			boost::archive::xml_iarchive static_arch{ static_stream };

			std::list<Actor*> static_actors;

			static_arch >> BOOST_SERIALIZATION_NVP(static_actors);

		}
		catch (boost::archive::archive_exception& e)
		{
			ENG_LOG(e.what() << "\t CODE: " << e.code);
			__debugbreak(); // REAL ERROR HANDLING
		}
		catch (std::exception& e)
		{
			ENG_LOG(e.what());
			__debugbreak();
		}


		try{

			// load dynamic actors
			std::ifstream dynamic_stream{ folderLocation + name + '\\' + name + ".SAVE" };
			if (!dynamic_stream.is_open())
			{
				FATAL_ERR("COULD NOT OPEN DYNAMIC ACTORS");
			}

			boost::archive::xml_iarchive dynamic_arch{ dynamic_stream };

			std::list<Actor*> dynamic_actors;

			dynamic_arch >> BOOST_SERIALIZATION_NVP(dynamic_actors);
		}
		catch (boost::archive::archive_exception& e)
		{
			ENG_LOG(e.what() << "\t CODE: " << e.code);
			__debugbreak(); // REAL ERROR HANDLING
		}
		catch (std::exception& e)
		{
			ENG_LOG(e.what());
			__debugbreak();
		}

	}

	ENG_LOG("World Loaded!");
}



DefaultWorld::~DefaultWorld()
{

}

extern "C" DefaultWorldPlugin_API void registerModule(ModuleManager& mm)
{
	mm.setWorld(AddWorld);
}

extern "C" DefaultWorldPlugin_API float getModuleEngineVersion()
{
	return ENGINE_VERSION;
}
