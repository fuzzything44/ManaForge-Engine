#pragma once
#include "Engine.h"
#include "Logging.h"
#include "Helper.h"
#include "TextureLibrary.h"

class Chunk;
#include "Actor.h"

// the chunk width
static const GLuint CHUNK_WIDTH = 50;
static const GLuint MAX_ACTORS = 20;

class Chunk
{
public:

	// freind the actor class so we can share private methods
	friend Actor;

	/// <summary> Initalizes all the chunks. </summary>
	/// <param name='programIn'> The program to use </param>
	/// <param name='viewMatIn'> A pointer to the view matrix to be used </param>
	/// <param name='chunksSizeIn'> The amount of chunks to initalize </param>
	static GLvoid ENGINE_API initChunks(GLuint programIn, mat4* viewMatIn, const uvec2& chunksSizeIn);

	static void ENGINE_API draw(vec2 characterLoc);

	// getter for location
	vec2 ENGINE_API getLocation();

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

	static mat4* viewMat;
	GLuint renderOrder = 0;

	vec2 location;

	GLvoid drawChunk();

	// a 2D array of pointers to chunks
	static Chunk*** chunks;

	// size of the array
	static uvec2 chunksSize;

	static Chunk* persistentChunk;

	/// <summary> Constructor giving a location </summary>
	/// <param name='location'> The location of the new Chunk </param>
	Chunk(vec2 locationIn = vec2(0.f, 0.f));

	static GLint texUniformHandle;
	static GLint characterLocUniformHandle;

};