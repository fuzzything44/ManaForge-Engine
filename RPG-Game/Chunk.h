#pragma once

#include <glm/glm.hpp>

#include "Helper.h"

class Chunk;
#include "Actor.h"

// the chunk width
#define CHUNK_WIDTH 50

class Chunk
{
public:

	// freind the actor class so we can share private methods
	friend Actor;

	// static function to add a chunk -- there is no default constructor so all get stored in chunks
	static GLvoid addChunk(GLuint programIn, glm::mat4* viewMatIn, glm::vec2 locationIn = glm::vec2(0.f, 0.f));
	
	// inits the persistant chunk
	static GLvoid initPersistent(GLuint programIn, glm::mat4* viewMatIn);

	static void draw();

	// getter for location
	glm::vec2 getLocation();

protected:
	// opengl handles
	GLuint locBufferID, UVBufferID, eboID, vaoID, program;

	// location of the uniform variables so we can set them
	GLint renderOrderUniID, viewMatUniID;

	// the amount of elemetns
	size_t elementCount;

	// a vector of all of the actors in the scene. Naive implemntation -- will fix later
	std::vector<Actor*> actors;

	glm::mat4* viewMat;
	GLuint renderOrder = 0;

	glm::vec2 location;

	GLvoid drawChunk();

	// vector to store refrences to all of the chunks
	static std::vector<Chunk*> chunks;
	static Chunk* persistentChunk;

	/// <summary> Constructor giving a location </summary>
	/// <param name='location'> The location of the new Chunk </param>
	Chunk(GLuint program, glm::mat4* viewMatIn, glm::vec2 locationIn = glm::vec2(0.f, 0.f));

};