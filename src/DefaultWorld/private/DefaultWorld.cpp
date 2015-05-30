#include "DefaultWorld.h"

#include <Helper.h>
#include <ImageLoader.h>
#include <PropertyManager.h>
#include <Runtime.h>
#include <Color.h>
#include <TextureLibrary.h>

#include <list>
#include <fstream>
#include <functional>
#include <map>
#include <vector>

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
	typedef boost::archive::polymorphic_xml_oarchive oarchive_t;
	typedef boost::archive::polymorphic_xml_iarchive iarchive_t;
#	define IS_SAVE_BINARY 0
#else
	typedef boost::archive::polymorphic_binary_oarchive oarchive_t;
	typedef boost::archive::polymorphic_binary_iarchive iarchive_t;
#	define IS_SAVE_BINARY 1
#endif

using namespace std;

DefaultWorld::DefaultWorld(std::string folder)
	:folderLocation(std::string("Worlds\\") + folder + '\\'),
	propManager(folderLocation + "world.json"),
	nextIndex(0),
	backgroundImages(Runtime::get().moduleManager.getRenderer().newTextureLibrary(32, 256)), // TODO: less hardcoded values
	drawMaterial(Runtime::get().moduleManager.getRenderer().newMaterial("boilerplate"))
{

	// Make sure a world folder was supplied.
	if (folder == "") {
		FATAL_ERR("No world specified");
	}
	ENG_LOGLN("Setting up console commands...");
	// Code for console commands goes here.
	// We probably want addActor, save, loadWorld.


	ENG_LOGLN("Loading images...");
	
	// We should probably just have the images we use in the same file as chunk size.
	std::ifstream stream{ folderLocation + "images.txt" };
	boost::archive::xml_iarchive arch{ stream }; // this might want to be not xml, maybe text or binary

	std::map<Color, std::string> valuePairs;

	// load the map from the file
	arch >> BOOST_SERIALIZATION_NVP(valuePairs);

	// load the images to the backgroundImages textureLibrary
	for (auto& elem : valuePairs)
	{
		backgroundImages->addImage(elem.second);
	}

	// set the material
	drawMaterial->setTexture(0, reinterpret_cast<Texture*>(backgroundImages));

	ENG_LOGLN("Images loaded!");

	// virtual functuion -- call THIS version of it -- we need this because it is inside a constructor
	DefaultWorld::loadWorld("main");

}

void DefaultWorld::loadWorld(std::string name)
{
	worldName = name;
	ENG_LOGLN("Loading world " << name << "...");
	

	/////////////////////////////////////////////////////
	// Begin static actor loading.
	{


		// List it will read things into.
		std::list<Actor*> staticActors;

		// File location of static actors -- if we are binary, then use a binary stream
#		if IS_SAVE_BINARY
			std::ifstream i_stream{ folderLocation + name + '\\' + name + ".WORLD", std::ifstream::binary};
#		else
			std::ifstream i_stream{ folderLocation + name + '\\' + name + ".WORLD" };
#		endif

		if (!i_stream.is_open())
		{
			FATAL_ERR("SAVE FILE DOESN'T EXIST");
		}

		// this might fail, so put it into try catch block
		try{

			// Create archive.
			iarchive_t i_archive{ i_stream };


			i_archive >> BOOST_SERIALIZATION_NVP(staticActors);

		} 
		catch (boost::archive::archive_exception& e)
		{
			ENG_LOGLN("ARCHIVE ERROR ENCOUNTERED WHILE LOADING WORLD ACTORS! Reason: " << e.what() << " Error code: " << e.code);
		}
		catch (std::exception& e)
		{
			ENG_LOGLN("ERROR ENCOUNTERED WHILE LOADING WORLD ACTORS! Reason: " << e.what());
		}

		// Move actors to the map.
		for (Actor*& elem : staticActors) 
		{
			actors[nextIndex] = elem;
			elem->GUID = nextIndex;
			nextIndex++;
		}
	}
	// End static actor loading
	////////////////////////////////////////////////////////
	// Begin dynamic actor loading
	{
		// File location of dynamic actors.
#	if IS_SAVE_BINARY
		std::ifstream i_stream{ folderLocation + name + '\\' + name + ".SAVE", std::ifstream::binary };
#	else
		std::ifstream i_stream{ folderLocation + name + '\\' + name + ".SAVE" };
#	endif


		if (!i_stream.is_open())
		{
			FATAL_ERR("SAVE FILE DOESN'T EXIST");
		}


		// List actors will be read into.
		std::list<Actor*> dynamicActors;

		try{
			// Create archive.
			iarchive_t i_archive{ i_stream };
			i_archive >> BOOST_SERIALIZATION_NVP(dynamicActors);

		}
		catch (boost::archive::archive_exception& e)
		{
			ENG_LOGLN("ARCHIVE ERROR WHILE LOADING SAVED ACTORS! Reason: " << e.what() << " Error code: " << e.code);
		}
		catch (std::exception& e)
		{
			ENG_LOGLN("ERROR ENCOUNTERED WHILE LOADING SAVED ACTORS! Reason: " << e.what());
		}

		// move actors to map.
		for (Actor*& elem : dynamicActors)
		{
			actors[nextIndex] = elem;
			elem->GUID = nextIndex;
			nextIndex++;
		}
	}
	ENG_LOGLN("World Loaded!");
}

void DefaultWorld::addActor(Actor& toAdd)
{
}

void DefaultWorld::save()
{
	ENG_LOGLN("Saving world");
	// Create list to save
	std::list<Actor*> toSave;
	for (auto& pair : actors) 
	{
		if (pair.second->needsSave()) 
		{
			toSave.push_back(pair.second);
		}
	} // End for

#if IS_SAVE_BINARY
	std::ofstream o_stream{ folderLocation + worldName + '\\' + worldName + ".SAVE", std::ifstream::binary };
#else
	std::ofstream o_stream{ folderLocation + worldName + '\\' + worldName + ".SAVE" };
#endif

	try{

		// Create archive.
		oarchive_t o_archive{ o_stream };
		// Save.
		o_archive << BOOST_SERIALIZATION_NVP(toSave);

	}
	catch (boost::archive::archive_exception& e)
	{
		ENG_LOGLN("ARCHIVE ERROR SAVING ACTORS. Reason: " << e.what() << " Code: " << e.code);
	}
	catch (std::exception& e)
	{
		ENG_LOGLN("ERROR SAVING ACTORS. Reason: " << e.what());
	}
}

void DefaultWorld::consoleCommand(std::string& command)
{
	string giveCommand;
	string args;
	try {
		commandMap[giveCommand](args);
	}
	catch (exception e) {
		ENG_LOGLN("Error in command " << command << ": " << e.what() );
		// We also may want to tell the UI it went wrong.
	}
}

DefaultWorld::~DefaultWorld()
{
	
}

