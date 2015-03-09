#include "stdafx.h"
#include "Actor.h"

Actor::Actor(const ActorData& dataIn, Chunk* chunkIn) 
	: data(dataIn),
	chunk(chunkIn)
{
	// set the pointer to this class
	data.actorPtr = this;

	// add this class to the chunk
	chunk->actors.push_back(this);

	ENG_LOG("\nCreate Buffer For Actor" << std::endl);

	cl_int err;
	buff = cl::Buffer(*CLHandler::context, CL_MEM_READ_WRITE, sizeof(ActorData), &data, &err);

#ifdef _DEBUG
	errChkCL(err, "Create Buffer");
#endif

}

void Actor::tick(float deltaTime)
{
	needsUpdate = false;
}

vec2 Actor::getLocation()
{
	return data.location;
}

vec2  Actor::getSize()
{
	return data.size;
}

float Actor::getRotation()
{
	return data.rotation;
}

vec2  Actor::getVelocity()
{
	return data.velocity;
}

void Actor::setLocation(vec2 newLoc)
{
	data.location = newLoc;
	needsUpdate = true;
}

void Actor::setSize(vec2 newSize)
{
	data.size = newSize;
	needsUpdate = true;
}

void Actor::setRotation(float newRot)
{
	data.rotation = newRot;
	needsUpdate = true;
}

void Actor::setVelocity(vec2 newVelocity)
{
	data.velocity = newVelocity;
	needsUpdate = true;
}
