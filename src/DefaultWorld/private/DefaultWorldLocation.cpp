#include "DefaultWorldLocation.h"

DefaultWorldLocation::~DefaultWorldLocation()
{
	if (!inWorld.isDestructing) {
		// Last actual element in the map.
		auto last = --inWorld.actors.end();

		location->second = last->second;
		location->second->GUID = new DefaultWorldLocation(location, inWorld);
		inWorld.actors.erase(last);
	}
}
