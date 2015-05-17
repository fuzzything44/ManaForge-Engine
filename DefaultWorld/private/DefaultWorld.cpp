#include "DefaultWorld.h"

#include <Helper.h>
#include <ImageLoader.h>
#include <PropertyManager.h>
#include <Runtime.h>
#include <Color.h>

#include <list>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/nvp.hpp>

#ifdef SAVE_TYPE_XML
#	include <boost/archive/polymorphic_xml_oarchive.hpp>
#	include <boost/archive/polymorphic_xml_iarchive.hpp>
#else
#	include <boost/archive/polymorphic_binary_oarchive.hpp>
#	include <boost/archive/polymorphic_binary_iarchive.hpp>
#endif

// Defines for in and out archives. May want to change to defines and include the istreams.
#ifdef SAVE_TYPE_XML
	typedef boost::archive::polymorphic_xml_oarchive oarchive;
	typedef boost::archive::polymorphic_xml_iarchive iarchive_t;
#	define IS_SAVE_BINARY 0
#else
	typedef boost::archive::polymorphic_binary_oarchive oarchive;
	typedef boost::archive::polymorphic_binary_iarchive iarchive;
#	define IS_SAVE_BINARY 1
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
		FATAL_ERR("No world specified");
	}

	ENG_LOG("Loading images...");
	
	// We should probably just have the images we use in the same file as chunk size.
	std::ifstream stream{ folderLocation + "images.txt" };
	boost::archive::xml_iarchive arch{ stream }; // this might want to be not xml, maybe text or binary

	std::map<Color, std::string> valuePairs;

	// load the map from the file
	arch >> BOOST_SERIALIZATION_NVP(valuePairs);

	// Give images to renderer
	//Runtime::get().moduleManager.getRenderer().loadTextures(valuePairs);

	ENG_LOG("Images loaded!");

	// virtual functuion -- call THIS version of it -- we need this because it is inside a constructor
	DefaultWorld::loadWorld("main");

}

void DefaultWorld::loadWorld(std::string name)
{
	worldName = name;
	ENG_LOG("Loading world " << name << "...");

	/////////////////////////////////////////////////////
	// Begin static actor loading.
	{
		// File location of static actors -- if we are binary, then use a binary stream
#		if IS_SAVE_BINARY
			std::ifstream i_stream{ folderLocation + "/" + name + ".WORLD", std::ifstream::binary};
#		else
			std::ifstream i_stream{ folderLocation + "/" + name + ".WORLD" };
#		endif
		
		// Create archive.
		iarchive_t i_archive{ i_stream };

		// List it will read things into.
		std::list<Actor*> staticActors;

		// this might fail, so put it into try catch block
		try{

			i_archive >> BOOST_SERIALIZATION_NVP(staticActors);

		} 
		catch (boost::archive::archive_exception& e)
		{
			ENG_LOG("ARCHIVE ERROR ENCOUNTERED! Reason: " << e.what() << " Error code: " << e.code);
		}
		catch (std::exception& e)
		{
			ENG_LOG("ERROR ENCOUNTERED! Reason: " << e.what());
		}

		// Move actors to the map.
		for (auto i = staticActors.begin(); i != staticActors.end(); ++i) {
			(*i)->GUID = nextIndex;
			*(actors[nextIndex]) = **i;
			nextIndex++;
		}
	}
	// End static actor loading
	////////////////////////////////////////////////////////
	// Begin dynamic actor loading
	{
		// File location of dynamic actors.
#		if IS_SAVE_BINARY
		std::ifstream i_stream{ folderLocation + "/" + name + ".SAVE", std::ifstream::binary };
#		else
		std::ifstream i_stream{ folderLocation + "/" + name + ".SAVE" };
#		endif
		// Create archive.
		iarchive_t i_archive{ i_stream };

		// List actors will be read into.
		std::list<Actor*> dynamicActors;
		i_archive >> BOOST_SERIALIZATION_NVP(dynamicActors);

		// move actors to map.
		for (std::list<Actor*>::iterator i = dynamicActors.begin(); i != dynamicActors.end(); i++) {
			(**i).GUID = nextIndex;
			*(actors[nextIndex]) = **i;
			nextIndex++;
		}
	}
	ENG_LOG("World Loaded!");
}

void DefaultWorld::addActor(Actor& toAdd)
{
}

void DefaultWorld::save()
{
	ENG_LOG("Saving world");
	// Create list to save
	std::list<Actor*> toSave;
	for (std::map<map_ID_t, Actor*>::iterator i = actors.begin(); i != actors.end(); i++) {
		if (i->second->needsSave()) {
			toSave.push_back(i->second);
		}
	} // End for

	// Create ostream to save to.
	std::ofstream o_stream{ folderLocation + "/" + worldName + ".SAVE" };
	// Create archive.
	oarchive o_archive{ o_stream };
	// Save.
	o_archive << BOOST_SERIALIZATION_NVP(toSave);
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
