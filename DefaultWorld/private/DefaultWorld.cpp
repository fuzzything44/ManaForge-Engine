#define DefaultWorldSource 1
#include "DefaultWorld.h"

#include <ImageLoader.h>

#include <PropertyManager.h>

#include <sstream>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/map.hpp>

#include <Runtime.h>
#include <Color.h>
#include <forward_list>

World* AddWorld(std::string folder)
{
	return new DefaultWorld(folder);
}



const Chunk& DefaultWorld::getPersistentChunk() const
{
	return *persistentCurrent;
}


DefaultWorld::DefaultWorld(std::string folder)
	:folderLocation(std::string("Worlds/") + folder),
	propManager(folderLocation + "/world.json")
{

	// Make sure a world folder was supplied.
	if (folderLocation == "") {
		FATAL_ERR("No world loaded");
	}

	ENG_LOG("Loading chunk size...");
	// First line should be chunk size.
	// Parses to int.
	chunkSize = propManager.queryValue<uint32>("chunk.size");

	ENG_LOG("Chunk size: " << chunkSize);

	ENG_LOG("Loading images...");
	
	std::ifstream stream{ folderLocation + "/images.txt" };
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
		
		// load static actors
		std::ifstream static_stream{ folderLocation + "/static.txt" };
		boost::archive::xml_iarchive static_arch{ static_stream };

		std::forward_list<Actor*> static_actors;

		static_arch >> BOOST_SERIALIZATION_NVP(static_actors);

		// load dynamic actors
		std::ifstream dynamic_stream{ folderLocation + "/archive.txt" };
		boost::archive::xml_iarchive dynamic_arch{ static_stream };

		std::forward_list<Actor*> dynamic_actors;

		static_arch >> BOOST_SERIALIZATION_NVP(dynamic_actors);

	}

	ENG_LOG("World Loaded!");
}



int DefaultWorld::getChunkSize() const
{
	return chunkSize;
}

const Chunk& DefaultWorld::getChunk(vec2 actorLocation) const
{
	// Actually get correct chunk.
	actorLocation /= chunkSize;

	ivec2 index = floor(actorLocation);

	// if it is in bounds, get that
	if (0 <= index.x && index.x < (int)numChunks.x && 0 <= index.y && index.y < (int)numChunks.y)
	{
		return *currentWorld[index.x][index.y];
	}

	// if not, return persistent chunk
	return getPersistentChunk();

}

DefaultWorld::~DefaultWorld()
{
	// loops through and frees the main world.
	for (int x = 0; mainWorld[x] != nullptr; x++) {
		for (int y = 0; mainWorld[x][y] != nullptr; y++) {
			free(mainWorld[x][y]);
		}
		free(mainWorld[x]);
	}
	free(mainWorld);
	// If you weren't in the main world, free the current world.
	if (currentWorld != nullptr) {
		for (int x = 0; currentWorld[x] != nullptr; x++) {
			for (int y = 0; currentWorld[x][y] != nullptr; y++) {
				free(currentWorld[x][y]);
			}
			free(currentWorld[x]);
		}
	}

	free(currentWorld);
}

extern "C" DefaultWorldPlugin_API void registerModule(ModuleManager& mm)
{
	mm.setWorld(AddWorld);
}

extern "C" DefaultWorldPlugin_API float getModuleEngineVersion()
{
	return ENGINE_VERSION;
}