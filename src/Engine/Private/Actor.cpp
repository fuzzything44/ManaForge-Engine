#include "Actor.h"

#include "Helper.h"

BOOST_CLASS_EXPORT_IMPLEMENT(Actor);

Actor::Actor(const Transform& trans)
	: trans(trans)
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
