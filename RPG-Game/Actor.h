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
	// Make Chunk a freind so it can access private methods (namely tick)
	friend Chunk;

	

private:
	
	glm::vec2 location;

	/// <summary> called every frame if the variable ticks is true </summary>
	/// <param name='deltaTime'> the time in milliseconds since the previous time. </param>
	virtual void tick(GLfloat deltaTime);

	
};