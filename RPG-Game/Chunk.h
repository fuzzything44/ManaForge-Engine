#include <glm/glm.hpp>

#include "Helper.h"

// the chunk width
#define CHUNK_WIDTH 10

class Chunk
{
public:


	// static function to add a chunk -- there is no default constructor so all get stored in chunks
	static GLvoid addChunk(GLuint programIn, glm::vec2 locationIn = glm::vec2(0.f, 0.f));


	static void draw();

	// getter for location
	glm::vec2 getLocation();

private:
	// opengl handles
	GLuint locBufferID, UVBufferID, eboID, vaoID, program;

	glm::vec2 location;

	GLvoid drawChunk();

	// vector to store refrences to all of the chunks
	static std::vector<Chunk*> chunks;

	/// <summary> Constructor giving a location </summary>
	/// <param name='location'> The location of the new Chunk </param>
	Chunk(GLuint program, glm::vec2 locationIn = glm::vec2(0.f, 0.f));

};