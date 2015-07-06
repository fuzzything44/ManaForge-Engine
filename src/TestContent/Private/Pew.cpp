#include "Pew.h"
#include "Gate.h"
void Pew::tick(float deltaTime)
{
	timeToSpawn -= deltaTime;
	if (timeToSpawn <= 0) {
		Gate* g = new Gate();
		g->addWorldLocation(vec2(1.f, 1.f) );
		timeToSpawn = 1.f;
	}
}
