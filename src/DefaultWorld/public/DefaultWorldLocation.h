#include "DefaultWorldConfig.h"

#include <World.h>

#include <map>

struct DefaultWorldLocation : public ActorLocation
{
	friend class DefaultWorld;
public:
	DefaultWorldLocation(uint64 loc = 0) : location(loc) {};
	virtual Actor* getActor() override;
	virtual void remove() override;
private:
	uint64 location;
	std::map<uint64, Actor*>::iterator iter;
};
