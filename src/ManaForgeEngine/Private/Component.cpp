#include "EnginePCH.h"

#include "Component.h"


Component::~Component()
{
	if (location != (owner.components.size() - 1))
	{
		// store a copy of the last elem
		Component* toMove = *(owner.components.end() - 1);

		// assign it to our index
		owner.components[location] = toMove;
		toMove->location = location;
	}

	owner.components.pop_back(); // remove the last element
}
