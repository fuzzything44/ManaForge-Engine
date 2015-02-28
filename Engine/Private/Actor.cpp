#include "stdafx.h"
#include "Actor.h"


template <typename T> 
GLvoid Actor::addActor(const vec4& bounds, const vec2& velocity, const float& rotation,
	GLboolean collides, UVData UVs, GLboolean isPersisitent)
{
	ActorData newDat(boundsIn, velocityIn, rotationIn, UVs, NULL, collides);


	if (isPersisitent)
	{

		Actor* newAct = new T(newDat, Chunk::persistentChunk);
		
	}
	else
	{
		Actor* newAct = new T(newDat, Chunk::chunks[floorf(bounds.y / CHUNK_WIDTH)][floorf(bounds.y / CHUNK_WIDTH)]);
	}
}

Actor::Actor(const ActorData& dataIn, Chunk* chunkIn) 
	: data(dataIn),
	chunk(chunkIn)
{
	// set the pointer to this class
	data.actorPtr = this;

	// add this class to the chunk
	chunk->actors.push_back(this);


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
}

void Actor::setSize(vec2 newSize)
{
	data.size = newSize;
}



void Actor::setRotation(float newRot)
{
	data.rotation = newRot;
}


void Actor::setVelocity(vec2 newVelocity)
{
	data.velocity = newVelocity;
}

