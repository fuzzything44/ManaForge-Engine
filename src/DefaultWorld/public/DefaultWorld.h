#include "DefaultWorldConfig.h"
#include <ENGException.h>
#include <World.h>
#include <PropertyManager.h>
#include <Runtime.h>
#include <Actor.h>

#define SAVE_TYPE_XML

class DefaultWorld : public World
{
	std::string folderLocation;
	std::string worldName;

	PropertyManager propManager;
	

	map_ID_t nextIndex;

	// Contains all actors in the world
	// !!!!! We also need to clear it when you change worlds. We may need to clear other things too (and we have to keep persistent actors).
	std::map<map_ID_t, Actor*> actors;

public:
	explicit DefaultWorld(std::string folder = ""); 

	virtual void loadWorld(std::string name) override;
	virtual void addActor(Actor& toAdd) override; 
	virtual void save() override;
	virtual ~DefaultWorld();
};
