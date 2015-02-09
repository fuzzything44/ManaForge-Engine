#include "Actor.h"


GLvoid Actor::addActor(const glm::vec4& boundsIn, GLfloat* UVs, GLboolean isPersisitent)
{
	if (isPersisitent)
	{
		Chunk::persistentChunk->actors.push_back(new Actor(boundsIn, UVs, Chunk::persistentChunk));
	}
	else
	{

	}
}

Actor::Actor(const glm::vec4& boundsIn, GLfloat* UVs, Chunk* chunkIn) : bounds(boundsIn), chunk(chunkIn)
{

}

GLvoid Actor::tick(GLfloat deltaTime)
{

}