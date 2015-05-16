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
#include <boost/serialization/nvp.hpp>

#include <boost/archive/polymorphic_xml_oarchive.hpp>
#include <boost/archive/polymorphic_xml_iarchive.hpp>
#include <boost/archive/polymorphic_binary_oarchive.hpp>
#include <boost/archive/polymorphic_binary_iarchive.hpp>


// Defines for in and out streams.
#ifdef SAVE_TYPE_XML
typedef boost::archive::polymorphic_xml_oarchive oarchive;
typedef boost::archive::polymorphic_xml_iarchive iarchive;
#else
typedef boost::archive::polymorphic_binary_oarchive oarchive;
typedef boost::archive::polymorphic_binary_iarchive iarchive;
#endif

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

}

void DefaultWorld::loadWorld(std::string name)
{
	ENG_LOG("Loading world " << name << "...");

	/////////////////////////////////////////////////////
	// Begin static actor loading.
	{
		// File location of static actors
		std::ifstream i_stream{ folderLocation + "/" + name + ".WORLD" };

		// Create archive.
		iarchive i_archive{ i_stream };

		// List it will read things into.
		std::list<Actor*> staticActors;
		i_archive >> BOOST_SERIALIZATION_NVP(staticActors);

		// Move actors to the map.
		for (std::list<Actor*>::iterator i = staticActors.begin(); i != staticActors.end(); i++) {
			(**i).mapKey = index;
			*(actors[index]) = **i;
			index++;
		}
	}
	// End static actor loading
	////////////////////////////////////////////////////////
	// Begin dynamic actor loading
	{
		// File location of dynamic actors.
		std::ifstream i_stream{ folderLocation + "/" + name + ".SAVE" };
		// Create archive.
		iarchive i_archive{ i_stream };

		// List actors will be read into.
		std::list<Actor*> dynamicActors;
		i_archive >> BOOST_SERIALIZATION_NVP(dynamicActors);

		// move actors to map.
		for (std::list<Actor*>::iterator i = dynamicActors.begin(); i != dynamicActors.end(); i++) {
			(**i).mapKey = index;
			*(actors[index]) = **i;
			index++;
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
