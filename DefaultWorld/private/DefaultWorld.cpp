#define DefaultWorldSource 1
#include "DefaultWorldConfig.h"
#include "World.h"
#include "lodepng.h"
#include <fstream>
#include <sstream>
#include <map>
#include <Color.h>

using namespace std;

// Splits a string on a given character.
template<char onSplit>
vector<string> split(string toSplit) {
	vector<string> ret;
	string buildstring;
	for (auto& elem : toSplit) {
		if (elem == onSplit) {
			if (buildstring != "") {
				ret.push_back(buildstring);
			}
		}
		else {
			buildstring.push_back(elem);
		}
	}
	if (buildstring != "") {
		ret.push_back(buildstring);
	}
	return ret;
}

class DefaultWorld : public World
{
private:
	string folderLocation;
	Chunk*** mainWorld = nullptr;
	Chunk*** currentWorld = nullptr;
	uint8 chunkSize = 50;
	ModuleManager* manager;
public:
	explicit DefaultWorld(ModuleManager& mm, string folder = "")
		:manager(&mm),
		folderLocation(folder) 
	{STACK
		// Make sure a world folder was supplied.
		if (folderLocation == "") {
			FATAL_ERR("No world loaded", 40);
		}
		

		string currentLine;
		// Loads .ini file for world system.
		ifstream world(folderLocation + "world.ini");

		ENG_LOG("Loading .ini file...");
		if (!world.is_open() ) {
			FATAL_ERR("World ini file doesn't exist. \n\tFolder:" + folderLocation, 41);
		}
		ENG_LOG(".ini Loaded!");

		// Ignore any leading comments
		do
		{
			getline(world, currentLine);
		} while (currentLine[0] == '#' || isspace(currentLine[0]) );
		
		ENG_LOG("Loading chunk size...");
		// First line should be chunk size.
		// Parses to int.
		stringstream parser;
		parser << currentLine;
		parser >> chunkSize;
		ENG_LOG("Chunk size loaded!");
		ENG_LOG("Loading images...");
		vector<string> imageNames;
		// Image loading loop
		while (getline(world, currentLine)) {
			if (currentLine[0] != '#' || currentLine[0] != '\n') {
				imageNames.push_back(currentLine);
			}
		}
		// Give images to renderer
		manager->getRenderer()->loadTextures(imageNames);

		ENG_LOG("Images loaded!");
		world.close();
		
		// virtual functuion -- call THIS version of it
		DefaultWorld::loadWorld("main");

		mainWorld = currentWorld;
		
	}	// End constructor

	virtual void loadWorld(string name) override 
	{STACK
		ENG_LOG("Loading world " << name << "...");
		// Skip re-loading main world if possible.
		if (name == "main" && mainWorld != nullptr) {
			currentWorld = mainWorld;
		}
		else {
			// Create world file reader.
			ifstream worldReader(folderLocation + "/" + name + ".WORLD");
			string onLine;

			// Ignore leading comments.
			do
			{
				getline(worldReader, onLine);
			} while (onLine[0] != '#');
			// First line should be amount of chunks x and y.
			vector<string> line = split<'_'>(onLine);

			// Parse the line.
			stringstream parser;
			parser << line[0];
			int chunksX;
			parser >> chunksX;
			parser << line[1];
			int chunksY;
			parser >> chunksY;

			// Initialize all chunks.
			for (int x = 0; x < chunksX; x++) {
				for (int y = 0; y < chunksY; y++) {
					currentWorld[x][y] = new Chunk(ivec2(x, y) );
				}
			}
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
	}	// End loadWorld

	virtual int getChunkSize() override
	{STACK
		return chunkSize;
	}
	virtual Chunk* getChunk(vec2 actorLocation) override
	{STACK
		return currentWorld[0][0];
		// Actually get correct chunk.
	}

	virtual ~DefaultWorld() 
	{STACK
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
	}	// End destructor
};

World* AddWorld(ModuleManager& mm, string folder)
{STACK
	return new DefaultWorld(mm, folder);
}

extern "C" DefaultWorldPlugin_API void registerModule(ModuleManager& mm) 
{STACK
	mm.AddWorld(AddWorld);
}