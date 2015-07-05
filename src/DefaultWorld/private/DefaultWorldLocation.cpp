#include "DefaultWorldLocation.h"

DefaultWorldLocation::~DefaultWorldLocation()
{
	if (!inWorld.isDestructing) {
		if (loc != inWorld.actors.size() - 1)
		{
			// store a copy of the last elem
			Actor* last = inWorld.actors[inWorld.actors.size() - 1];
			inWorld.actors.pop_back(); // remove the last element

			// assign it to our index
			inWorld.actors[loc] = last;
			static_cast<DefaultWorldLocation*>(last->GUID.get())->loc = loc;
		}

	}
}
