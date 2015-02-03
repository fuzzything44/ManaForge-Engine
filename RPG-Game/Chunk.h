#include <glm/glm.hpp>

#include "Helper.h"

// the chunk width
#define CHUNK_WIDTH 2

class Chunk
{
public:


	// static function to add a chunk -- there is no default constructor so all get stored in chunks
	static GLvoid addChunk(GLuint programIn, glm::mat4* viewMatIn, GLfloat* scale, glm::vec2 locationIn = glm::vec2(0.f, 0.f));
	
	// inits the persistant chunk
	static GLvoid initPersistent(GLuint programIn, glm::mat4* viewMatIn, GLfloat* scale);

	static void draw();

	// getter for location
	glm::vec2 getLocation();

private:
	// opengl handles
	GLuint locBufferID, UVBufferID, eboID, vaoID, program;

	// location of the uniform variables so we can set them
	GLint renderOrderUniID, viewMatUniID, scaleUniID;

	// the amount of elemetns
	size_t elementCount;

	glm::mat4* viewMat;
	GLfloat* scale;
	GLuint renderOrder = 0;

	glm::vec2 location;

	GLvoid drawChunk();

	// vector to store refrences to all of the chunks
	static std::vector<Chunk*> chunks;
	static Chunk* persistentChunk;

	/// <summary> Constructor giving a location </summary>
	/// <param name='location'> The location of the new Chunk </param>
	Chunk(GLuint program, glm::mat4* viewMatIn, GLfloat* scale, glm::vec2 locationIn = glm::vec2(0.f, 0.f));

};