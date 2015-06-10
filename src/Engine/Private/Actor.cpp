#include "EnginePCH.h"

#include "Actor.h"

#include "Helper.h"
#include "Runtime.h"

BOOST_CLASS_EXPORT_IMPLEMENT(Actor);

Actor::Actor()
{
}

bool Actor::needsSave()
{
	// by default returns false
	return false;
}

void Actor::tick(float deltaTime)
{
	
}



Actor::~Actor()
{

}
