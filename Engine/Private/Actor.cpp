#include "stdafx.h"
#include "Actor.h"


GLvoid Actor::addActor(const vec4& boundsIn, GLfloat* UVs, GLboolean isPersisitent)
{
	if (isPersisitent)
	{
		Actor* newAct = (Actor*)_aligned_malloc(sizeof(Actor), 16);
		*newAct = Actor(boundsIn, UVs, Chunk::persistentChunk);
		Chunk::persistentChunk->actors.push_back(newAct);
	}
	else
	{

	}
}

Actor::Actor(const vec4& boundsIn, GLfloat* UVs, Chunk* chunkIn) : bounds(boundsIn), chunk(chunkIn)
{

}

