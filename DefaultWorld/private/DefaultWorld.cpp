#define DefaultWorldSource 1
#include "DefaultWorld.h"

#include <ImageLoader.h>


#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

World* AddWorld(std::string folder)
{
	return new DefaultWorld(folder);
}



const Chunk& DefaultWorld::getPersistentChunk() const
{
	return *persistentCurrent;
}


DefaultWorld::DefaultWorld(std::string folder):folderLocation(folder)
{
	// Make sure a world folder was supplied.
	if (folderLocation == "") {
		FATAL_ERR("No world loaded");
	}


	std::string currentLine;
	// Loads .ini file for world system.
	std::ifstream world(folderLocation + "world.ini");

	ENG_LOG("Loading .ini file...");
	if (!world.is_open()) {
		FATAL_ERR("World ini file doesn't exist. \n\tFolder:" + folderLocation);
	}
	ENG_LOG(".ini Loaded!");

	// Ignore any leading comments
	do
	{
		getline(world, currentLine);
	} while (currentLine[0] == '#' || isspace(currentLine[0]));

	ENG_LOG("Loading chunk size...");
	// First line should be chunk size.
	// Parses to int.
	boost::lexical_cast<uint32>(currentLine);

	ENG_LOG("Chunk size loaded!");
	ENG_LOG("Loading images...");
	std::vector<std::string> imageNames;
	// Image loading loop
	while (getline(world, currentLine)) {
		if (currentLine[0] != '#' || currentLine[0] != '\n') {
			imageNames.push_back(currentLine);
		}
	}
	// Give images to renderer
	ModuleManager::get().getRenderer().loadTextures(imageNames);

	ENG_LOG("Images loaded!");
	world.close();

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
		// Create world file reader.
		std::ifstream worldReader(folderLocation + "/" + name + ".WORLD");
		std::string onLine;

		// Ignore leading comments.
		do
		{
			getline(worldReader, onLine);
		} while (onLine[0] != '#');
		// First line should be amount of chunks x and y.
		std::vector<std::string> line;
		boost::algorithm::split(line, onLine, boost::algorithm::is_any_of("_"));

		uint32 chunksX = boost::lexical_cast<uint32>(line[0]);
		
		uint32 chunksY = boost::lexical_cast<uint32>(line[1]);
		

		// Initialize all chunks.
		for (uint32 x = 0; x < chunksX; x++) {
			for (uint32 y = 0; y < chunksY; y++) {
				currentWorld[x][y] = new Chunk(ivec2(x, y));
			}
		}
		std::vector<uint8> imgData;

		uvec2 size = ImageLoader::load("world.png", imgData);

		//// Create color to image map.
		//map<Color, string> imageMap;
		//do
		//{
		//	// Read next line.
		//	getline(worldReader, onLine);
		//	// Make sure it is not a comment or ending line.
		//	if (onLine[0] != '#' || onLine != "END") {
		//		line = split<'_'>(onLine);
		//		
		//		// Generate and add color to the map.
		//		Color addColor(reinterpret_cast<uint8*>(&line[0]));
		//		imageMap[addColor] = line[1];
		//	}
		//} while (onLine != "END");



		// Finish loading world.
		// PNG loading and actor loading left.




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
