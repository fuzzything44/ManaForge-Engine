#include "DefaultWorldConfig.h"

#include <World.h>

struct DefaultWorldLocation : public ActorLocation
{
public:
	DefaultWorldLocation(uint64 loc = 0) : location(loc) {};
	virtual Actor* getActor() override;
	virtual void remove() override;
private:
	uint64 location;
};
