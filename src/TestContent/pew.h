#include "TestContentConfig.h"
#include <Actor.h>

class pew : public Actor {
public:
	pew() : timeToSpawn(0.f) { Runtime::get().world->registerTickingActor(*this); }
	~pew() override {}
	virtual void tick(float deltaTime) override;
private:
	float timeToSpawn;
};