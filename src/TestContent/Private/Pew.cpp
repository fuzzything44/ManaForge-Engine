#include "Pew.h"
#include "Gate.h"
void Pew::tick(float deltaTime)
{
	timeToSpawn -= deltaTime;
	if (timeToSpawn <= 0) {
		Gate* g = new Gate();
		g->addWorldLocation(vec2(rand() % 10, rand() % 10) );
		timeToSpawn = .1f;
	}
}
