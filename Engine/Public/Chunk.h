#pragma once
#include "Engine.h"
#include "Logging.h"
#include "Helper.h"
#include "TextureLibrary.h"

#include <random>

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

	/**
	 * \fn	static GLvoid ENGINE_API Chunk::initChunks(GLuint programIn, mat4* viewMatIn, const uvec2& chunksSizeIn);
	 *
	 * \brief	Initialises the chunks.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param	programIn		 	The program in.
	 * \param [in,out]	viewMatIn	If non-null, the view matrix in.
	 * \param	chunksSizeIn	 	The chunks size in.
	 *
	 * \return	An ENGINE_API.
	 */

	static GLvoid ENGINE_API initChunks(GLuint chunkProgramIn, GLuint actorProgramIn, mat4* viewMatIn, const uvec2& chunksSizeIn);

	/**
	 * \fn	static void ENGINE_API Chunk::draw(vec2 characterLoc);
	 *
	 * \brief	Draws the given character location.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param	characterLoc	The character location.
	 *
	 * \return	An ENGINE_API.
	 */

	static void ENGINE_API draw(vec2 characterLoc, float deltaTime);

	/**
	 * \fn	vec2 ENGINE_API Chunk::getLocation();
	 *
	 * \brief	getter for location.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \return	The location.
	 */

	vec2 ENGINE_API getLocation();

protected:

	

	// opengl handles
	GLuint locBufferID, UVBufferID, eboID, vaoID;

	static GLuint chunkProgram;
	static GLuint actorProgram;

	// location of the uniform variables so we can set them
	GLint viewMatUniID;

	// the amount of elemetns
	size_t elementCount;

	// a vector of all of the actors in the scene. Naive implemntation -- will fix later
	std::vector<Actor*> actors;

	static mat4* viewMat;

	vec2 location;

	// draw a certian chunk
	GLvoid drawChunk(std::vector<ActorData>& data);

	// a 2D array of pointers to chunks
	static ENGINE_API Chunk*** chunks;

	static ENGINE_API Chunk* persistentChunk;

	// size of the array
	static uvec2 chunksSize;


	Chunk(vec2 locationIn = vec2(0.f, 0.f));

	static GLint texUniformHandleChunk;
	static GLint characterLocUniformHandleChunk;

	static GLint texUniformHandleActor;

	static GLuint vertexArrayActors;
	static GLuint locBufferActors;
	static GLuint UVBufferActors;
	static GLuint elemBufferActors;

};