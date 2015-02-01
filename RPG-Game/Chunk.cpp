#include "Chunk.h"

#include <GLFW/glfw3.h>

// set inital value for static variable
std::vector<Chunk*> Chunk::chunks = std::vector<Chunk*>();

GLvoid Chunk::addChunk(GLuint programIn, glm::vec2 locationIn = glm::vec2(0.f, 0.f))
{
	// add the chunk to the vector of chunks
	chunks.push_back(new Chunk(programIn, locationIn));

}

Chunk::Chunk(GLuint programIn, glm::vec2 locationIn) 
	: location(locationIn), program(programIn)
{

	// the data for locations + allocate || four vertices per actor || two values per vertex || there are CHUNK_WIDTH squared actors
	std::vector<GLfloat> locData		 (			4				*				2		 *	 CHUNK_WIDTH * CHUNK_WIDTH);

	// allocate the same amount as above for texture coordinates
	std::vector<GLfloat> texCoords(12 * CHUNK_WIDTH * CHUNK_WIDTH);

	// the element data + allocate || two trianges per actor || three vertices per triangle || there are CHUNK_WIDTH squared actors
	std::vector<GLuint> eboData(2 * 3 * CHUNK_WIDTH * CHUNK_WIDTH);


	// init vbos, vaos, and textures
	for (int x = - CHUNK_WIDTH / 2; x < CHUNK_WIDTH / 2; x++)
	{
		for (int y = -CHUNK_WIDTH / 2; y < CHUNK_WIDTH / 2; y++)
		{
			GLfloat startX = x + location.x;
			GLfloat startY = y + location.y;


			locData.push_back(startX);
			locData.push_back(startY); // bottom left

			locData.push_back(startX);
			locData.push_back(startY + 1); // top left

			locData.push_back(startX + 1);
			locData.push_back(startY); // bottom right

			locData.push_back(startX + 1);
			locData.push_back(startY + 1); // top right


			// add default texture coordinates -- will change with texture libraries
			texCoords.push_back(0);
			texCoords.push_back(0); // bottom left

			texCoords.push_back(0);
			texCoords.push_back(1); // top left

			texCoords.push_back(1);
			texCoords.push_back(0); // botom right

			texCoords.push_back(1);
			texCoords.push_back(1); // top right


			GLuint startIdx = ((y + CHUNK_WIDTH / 2) * CHUNK_WIDTH + (x + CHUNK_WIDTH / 2)) * 4;

			eboData.push_back(startIdx);
			eboData.push_back(startIdx + 1);
			eboData.push_back(startIdx + 2);

			eboData.push_back(startIdx + 1);
			eboData.push_back(startIdx + 2);
			eboData.push_back(startIdx + 3);

		}
	}

	// init openGL buffers

	// create and bind the Vertex Array Object.
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);


	// init location buffer
	// generate a buffer for our buffer for the locations.
	glGenBuffers(1, &locBufferID);

	// bind the GL_ARRAY_BUFFER to locBufferID so all calls to GL_ARRAY_BUFFER relate to locBufferID 
	glBindBuffer(GL_ARRAY_BUFFER, locBufferID);

	// bind loc data to be sent to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * locData.size(), &locData[0], GL_STATIC_DRAW);


	// init UV buffer
	glGenBuffers(1, &UVBufferID);

	// bind the GL_ARRAY_BUFFER to UVBufferID so all calls to GL_ARRAY_BUFFER relate to UVBufferID 
	glBindBuffer(GL_ARRAY_BUFFER, UVBufferID);

	// bind UV data to be sen tot the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * locData.size(), &locData[0], GL_STATIC_DRAW);


	// init element buffer
	glGenBuffers(1, &eboID);
	
	// bind the GL_ELEMENT_ARRAY_BUFFER to eboID so all calls to GL_ELEMENT_ARRAY_BUFFERS relate to eboID
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);

	// send element data to the GPU
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * locData.size(), &locData[0], GL_STATIC_DRAW);

}

GLvoid Chunk::drawChunk()
{

}

glm::vec2 Chunk::getLocation()
{
	// return a copy of the location variable
	return location;
}