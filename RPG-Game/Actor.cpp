#include "Actor.h"


GLvoid Actor::addActor(const glm::vec4& boundsIn, GLfloat* UVs, GLboolean isPersisitent)
{
}

Actor::Actor(const glm::vec4& boundsIn, GLfloat* UVs, Chunk* chunkIn) : bounds(boundsIn), chunk(chunkIn)
{

}

GLvoid Actor::tick(GLfloat deltaTime)
{

}