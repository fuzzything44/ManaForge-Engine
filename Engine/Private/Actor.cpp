#include "Actor.h"

#include "Helper.h"

BOOST_CLASS_EXPORT_IMPLEMENT(Actor);

Actor::Actor(const Transform& trans, bool isPersistent)
	: trans(trans)
{

}


void Actor::tick(float deltaTime)
{
	
}



Actor::~Actor()
{

}
