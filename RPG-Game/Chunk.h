#pragma once

#include <glm/glm.hpp>

#include <sstream>

#include "Helper.h"
#include "TextureLibrary.h"

class Chunk;
#include "Actor.h"

// the chunk width
#define CHUNK_WIDTH 50
#define MAX_ACTORS 20

class Chunk
{
public:

	// freind the actor class so we can share private methods
	friend Actor;

	/// <summary> Initalizes all the chunks. </summary>
	/// <param name='programIn'> The program to use </param>
	/// <param name='viewMatIn'> A pointer to the view matrix to be used </param>
	/// <param name='chunksSizeIn'> The amount of chunks to initalize </param>
	static GLvoid initChunks(GLuint programIn, glm::mat4* viewMatIn, const glm::uvec2& chunksSizeIn);

	static void draw();

	// getter for location
	glm::vec2 getLocation();

protected:

	

	// opengl handles
	GLuint locBufferID, UVBufferID, eboID, vaoID;

	static GLuint program;

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

	// a 2D array of pointers to chunks
	static Chunk*** chunks;

	// size of the array
	static glm::uvec2 chunksSize;

	static Chunk* persistentChunk;

	/// <summary> Constructor giving a location </summary>
	/// <param name='location'> The location of the new Chunk </param>
	Chunk(glm::mat4* viewMatIn, glm::vec2 locationIn = glm::vec2(0.f, 0.f));

	static GLint texUniformHandle;

};