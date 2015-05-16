#include "Gate.h"
#include <Runtime.h>

#define ACTOR_SAVE_DATA Gate, Actor, isOpen

#include ACTOR_REGISTER_FOR_SAVING_SOURCE()

Gate::Gate(const Transform& trans)
	: Actor(trans)
{
	vec2 locations[] = { vec2(0.f, 0.f), vec2(0.f, 1.f), vec2(1.f, 0.f), vec2(1.f, 1.f) };
	uvec3 elems[] =
	{	
		uvec3(0, 1, 2),
		uvec3(1, 2, 3) 
	};

	gateMesh = new MeshComponent(this, Transform{}, ModelData(Transform{}, locations, locations, elems, 4, 6));
}
