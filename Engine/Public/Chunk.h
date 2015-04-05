#pragma once
#include "Engine.h"
#include "Logging.h"
#include "Helper.h"
#include "TextureLibrary.h"

#include <random>

class Chunk;
#include "Actor.h"

/// <summary> Width of the chunk.</summary>
static const GLuint CHUNK_WIDTH = 50;

/// <summary> A chunk.</summary>
class Chunk
{
public:

	// freind the actor class so we can share private methods
	friend Actor;

	/// <summary> Initialises the chunks.</summary>
	///
	/// <param name="programIn">    The program in.</param>
	/// <param name="viewMatIn">    [in,out] If non-null, the view matrix in.</param>
	/// <param name="chunksSizeIn"> The chunks size in.</param>
	///
	/// <returns> An ENGINE_API.</returns>
	static GLvoid ENGINE_API initChunks(GLuint programIn, mat4* viewMatIn, const uvec2& chunksSizeIn);

	/// <summary> Draws.</summary>
	///
	/// <param name="character"> [in,out] If non-null, the character.</param>
	/// <param name="deltaTime"> The delta time.</param>
	///
	/// <returns> An ENGINE_API.</returns>
	static void ENGINE_API draw(Actor* character, float deltaTime);

	/// <summary> Gets the location.</summary>
	///
	/// <returns> The location.</returns>
	vec2 ENGINE_API getLocation();

	static uvec2 ENGINE_API getChunkSize();

protected:

	

	// opengl handles
	GLuint locBufferID, UVBufferID, eboID, vaoID;

	/// <summary> The program.</summary>
	static GLuint program;

	/// <summary> Identifier for the view matrix uni.</summary>
	GLint viewMatUniID;

	/// <summary> Number of elements.</summary>
	size_t elementCount;

	/// <summary> The actors.</summary>
	std::vector<Actor*> actors;

	/// <summary> The view matrix.</summary>
	static mat4* viewMat;

	/// <summary> The location.</summary>
	vec2 location;

	/// <summary> Draw chunk.</summary>
	///
	/// <param name="data"> [in,out] The data.</param>
	///
	/// <returns> A GLvoid.</returns>
	GLvoid drawChunk(std::vector<ActorData>& data);

	/// <summary> the chunks </summary>
	static ENGINE_API Chunk*** chunks;

	/// <summary> the persistent chunk.</summary>
	static ENGINE_API Chunk* persistentChunk;

	/// <summary> Size of the chunks.</summary>
	static uvec2 chunksSize;

	/// <summary> Constructor.</summary>
	///
	/// <param name="locationIn"> The location in.</param>
	Chunk(vec2 locationIn = vec2(0.f, 0.f));

	/// <summary> The tex uniform handle chunk.</summary>
	static GLint texUniformHandleChunk;

	/// <summary> The character location uniform handle chunk.</summary>
	static GLint characterLocUniformHandleChunk;


};