#pragma once
#include "Engine.h"

#include <GLTools/glew.h>

#include <glm/glm.hpp>

// you will learn about this later -- ask me if you want to know
class Actor;
#include "Chunk.h"

// the actor class -- by default dynamic 
class Actor
{
public:
	// Make Chunk a freind so it can access private methods (namely tick)
	friend Chunk;

	static ENGINE_API GLvoid addActor(const glm::vec4& boundsIn, GLfloat* UVs, GLboolean isPersisitent);

protected:
	
	// first two elements are X, Y, and second two are width, height
	glm::vec4 bounds;

	// refrence to the chunk that it resides in
	Chunk* chunk;

	/// <summary> called every frame if the variable ticks is true </summary>
	/// <param name='deltaTime'> the time in milliseconds since the previous time. </param>
	virtual void tick(GLfloat deltaTime);

	Actor(const glm::vec4& boundsIn, GLfloat* UVs, Chunk* chunkIn);

	// the velocity of the actor
	glm::vec2 velocity;

	
	
};