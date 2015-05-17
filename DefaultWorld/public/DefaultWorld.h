#include "DefaultWorldConfig.h"
#include <ENGException.h>
#include <World.h>
#include <PropertyManager.h>
#include <Runtime.h>
#include <Actor.h>

#define SAVE_TYPE_XML

class DefaultWorld : public World
{
private:
	std::string folderLocation;
	std::string worldName;

	PropertyManager propManager;
	

	map_ID_t nextIndex;

public:
	explicit DefaultWorld(std::string folder = ""); 

	virtual void loadWorld(std::string name) override;
	virtual void addActor(Actor& toAdd) override; 
	virtual void save() override;
	virtual ~DefaultWorld();
};
