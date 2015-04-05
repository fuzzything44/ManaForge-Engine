#define DefaultWorldSource 1
#include "DefaultWorldConfig.h"
#include "WorldModule.h"

using std::string;

class DefaultWorld : public WorldModule
{
private:
	string folderLocation;
	Chunk*** mainWorld = nullptr;
	Chunk*** currentWorld = nullptr;
	int chunkSize = 50;
public:
	DefaultWorld(string folder = "") {
		if (folder == "") {
			FATAL_ERR("No world loaded", 42);
		}
		folderLocation = folder;
		// Load main world here.
		currentWorld = mainWorld;
		
	}	// End constructor

	virtual void loadWorld(string name) override {
		if (name == "main") {
			currentWorld = mainWorld;
		}
		else {
			// Load world here.
		}
	}	// End loadWorld

	virtual int getChunkSize() override {
		return chunkSize;
	}
	virtual Chunk* getChunk(vec2 actorLocation) override{
		return currentWorld[0][0];
		// Actually get correct chunk.
	}

	virtual ~DefaultWorld() {
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

DefaultWorld* AddWorld(string folder)
{
	return new DefaultWorld(folder);
}

extern "C" DefaultWorldPlugin_API void registerModule(ModuleManager& mm) {
	mm.AddWorld(AddWorld);
}