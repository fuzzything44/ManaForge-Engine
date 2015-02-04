#pragma once

#include <GLTools/glew.h>

#include <glm/glm.hpp>

// you will learn about this later -- ask me if you want to know
class Actor;
#include "Chunk.h"

// the actor class -- by default dynamic 
class Actor
{
public:

	/// <summary> adds an actor -- automaitally handles chunk stuff </summary>
	/// <param name='boundsIn'> the bounds for the actor -- first two elements are XY coordinates and the second are length and width. </param>
	/// <param name='UVs'> the UVs for the object </param>
	static void addActor(const glm::vec4& bounds, GLfloat* UVs, GLboolean isPersistent = false);

	// Make Chunk a freind so it can access private methods (namely tick)
	friend Chunk;

	

private:

	// private constructor
	Actor(const glm::vec4& boundsIn, GLfloat* UVs, Chunk* chunkIn);

	// the chunk that the actor resides in
	Chunk* chunk;
	
	glm::vec4 bounds;

	/// <summary> called every frame if the variable ticks is true </summary>
	/// <param name='deltaTime'> the time in milliseconds since the previous time. </param>
	virtual void tick(GLfloat deltaTime);

	
};