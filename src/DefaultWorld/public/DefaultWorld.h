#include "DefaultWorldConfig.h"
#include <ENGException.h>
#include <World.h>
#include <PropertyManager.h>
#include <Runtime.h>
#include <Actor.h>
#include <functional>
#include <TextureLibrary.h>
#include <map>

#include <array>

#include "ChunkActor.h"

#include <Color.h>


class DefaultWorld : public World
{
	std::string folderLocation;
	std::string worldName;

	PropertyManager propManager; // for world specific properties
	
	// Map of console commands. Maps from a string to function. Function takes in a string.
	std::map<std::string, std::function<void(const std::string)> > commandMap;


	// Contains all actors in the world
	// !!!!! We also need to clear it when you change worlds. We may need to clear other things too (and we have to keep persistent actors).
	std::map<uint64, Actor*> actors;
	uint64 nextIndex = 0;

	std::map<Color, std::string> imageToTextureAssoc;

	TextureLibrary* backgroundImages;
	Material* drawMaterial;

	// array of models -- in row major order
	ChunkActor** background;

	uvec2 numBackgroundChunks;

	uint32 backgroundChunkSize;

	std::string playerControllerClassName;
	std::string pawnClassName;

	bool isDestructing;

public: 
	explicit DefaultWorld();
	virtual ~DefaultWorld();

	virtual void init(const std::string& name) override;

	// World Interface
	virtual void loadWorld(std::string name) override;
	virtual ActorLocation* addActor(Actor* toAdd) override; 
	virtual void removeActor(Actor* toRemove) override;
	virtual void save() override;

	virtual void consoleCommand(std::string& command) override;

	virtual PlayerController* makePlayerController() override;
	virtual Pawn* makePawn() override;
	// End World Interface
};
