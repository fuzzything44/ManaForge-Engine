#include "stdafx.h"
#include "Actor.h"


template <typename T>
GLvoid Actor::addActor(const vec4& boundsIn, const vec2& velocityIn, const float& rotationIn,
	GLboolean collides, UVData UVs, GLboolean isPersisitent)
{
	if (isPersisitent)
	{
		T* newAct = (T*)_aligned_malloc(sizeof(T), 16);

		// build actorData
		ActorData data(boundsIn, velocityIn, rotationIn, UVs, collides)

		*newAct = Actor(boundsIn, UVs, Chunk::persistentChunk);
		Chunk::persistentChunk->actors.push_back(newAct);
	}
	else
	{

	}
}

Actor::Actor(const ActorData& dataIn, Chunk* chunkIn) : data(dataIn), chunk(chunkIn)
{

}

