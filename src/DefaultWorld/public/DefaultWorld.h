#include "DefaultWorldConfig.h"
#include <ENGException.h>
#include <World.h>
#include <PropertyManager.h>
#include <Runtime.h>
#include <Actor.h>
#include <functional>
#include <map>

#define SAVE_TYPE_XML

class DefaultWorld : public World
{
	std::string folderLocation;
	std::string worldName;

	PropertyManager propManager;
	
	// Map of console commands. Maps from a string to function. Function takes in a string.
	std::map<std::string, std::function<void(const std::string)> > commandMap;

	map_ID_t nextIndex;

	// Contains all actors in the world
	// !!!!! We also need to clear it when you change worlds. We may need to clear other things too (and we have to keep persistent actors).
	std::map<map_ID_t, Actor*> actors;

	TextureLibrary* backgroundImages;
	Material* drawMaterial;

	// array of models -- in row major order
	Model** background;

	uvec2 numBackgroundChunks;

	uint32 backgroundChunkSize;

public:
	explicit DefaultWorld(std::string folder = ""); 

	virtual void loadWorld(std::string name) override;
	virtual void addActor(Actor& toAdd) override; 
	virtual void save() override;

	virtual void consoleCommand(std::string& command) override;

	virtual ~DefaultWorld();
};
