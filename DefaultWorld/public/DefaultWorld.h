#include "DefaultWorldConfig.h"
#include <ENGException.h>
#include <World.h>
#include <PropertyManager.h>
#include <Runtime.h>
#include <Actor.h>

class DefaultWorld : public World
{
private:
	std::string folderLocation;

	PropertyManager propManager;

public:
	explicit DefaultWorld(std::string folder = ""); 

	virtual void loadWorld(std::string name) override;
	virtual void addActor(Actor& toAdd) override;

	virtual ~DefaultWorld();
};
