#include "Chunk.h"

#include <GLTools/glfw3.h>
#include <random>

// set inital value for static variables
Chunk*** Chunk::chunks = NULL;
Chunk* Chunk::persistentChunk = NULL;
glm::uvec2 Chunk::chunksSize = glm::uvec2(0, 0);
GLint Chunk::texUniformHandle = -1;
GLuint Chunk::program = 0;


GLvoid Chunk::initChunks(GLuint programIn, glm::mat4* viewMatIn, const glm::uvec2& chunksSizeIn)
{
	chunksSize = chunksSizeIn;
	program = programIn;

	// allocate the pointer
	chunks = (Chunk***)malloc(sizeof(Chunk**) * chunksSize.y);

	for (GLuint x = 0; x < chunksSize.x; x++)
	{
		// allocate the column -- you can read about malloc: http://www.cplusplus.com/reference/cstdlib/malloc/
		chunks[x] = (Chunk**)malloc(sizeof(Chunk*) * chunksSize.y);
		for (GLuint y = 0; y < chunksSize.y; y++)
		{
			chunks[x][y] = new Chunk(viewMatIn, glm::vec2(x * CHUNK_WIDTH, y * CHUNK_WIDTH));
		}
	}


	persistentChunk = new Chunk(viewMatIn);


	// initalize all handles
	texUniformHandle = glGetUniformLocation(program, "texArray");
}

Chunk::Chunk(glm::mat4* viewMatIn, glm::vec2 locationIn)
	: location(locationIn), viewMat(viewMatIn)
{
	// the persistent chunk doesn't get anything
	if (this != persistentChunk)
	{
		srand((unsigned int)this);

		// init the generator -- will remove with loading
		std::mersenne_twister_engine < std::uint_fast32_t, 32, 624, 397, 31,
			0x9908b0df, 11,
			0xffffffff, 7,
			0x9d2c5680, 15,
			0xefc60000, 18, 1812433253 > gen(rand());

		// the data for locations 
		std::vector<GLfloat> locData;
		locData.reserve(8 * CHUNK_WIDTH * CHUNK_WIDTH);

		// allocate the same amount as above for texture coordinates
		std::vector<UVData> texCoords;
		texCoords.reserve(CHUNK_WIDTH * CHUNK_WIDTH);

		// the element data 
		std::vector<GLuint> eboData;
		eboData.reserve(6 * CHUNK_WIDTH * CHUNK_WIDTH);

		// init vbos, vaos, and textures
		for (int y = 0; y < CHUNK_WIDTH; y++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
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

				// use a random texture -- will use loading
				// currently 17 textures
				GLuint tex = std::uniform_int_distribution<int>(0, 16)(gen);
				std::stringstream stream;
				stream << tex;
				texCoords.push_back(TextureLibrary::getUVData(stream.str()));

				
				GLuint startIdx = (y * CHUNK_WIDTH + x) * 4;

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
		glBufferData(GL_ARRAY_BUFFER, sizeof(UVData) * texCoords.size(), &texCoords[0].bottomLeft.x, GL_STATIC_DRAW);


		// init element buffer
		glGenBuffers(1, &eboID);

		// bind the GL_ELEMENT_ARRAY_BUFFER to eboID so all calls to GL_ELEMENT_ARRAY_BUFFERS relate to eboID
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);

		// send element data to the GPU
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * eboData.size(), &eboData[0], GL_STATIC_DRAW);

		elementCount = eboData.size();


		viewMatUniID = glGetUniformLocation(program, "viewMat");
		renderOrderUniID = glGetUniformLocation(program, "renderOrder");
	}
}

GLvoid Chunk::drawChunk()
{
	if (this != persistentChunk)
	{

		glUseProgram(program);
		
		glUniform1i(glGetUniformLocation(program, "texArray"), 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureLibrary::getTextureHandle());

		glBindVertexArray(vaoID);

		// set the viewMat in the shader to the view mat. We ned to derefrence, get first element, then turn back into pointer
		glUniformMatrix4fv(viewMatUniID, 1, GL_FALSE, &(*viewMat)[0][0]);


		// set the renderOrder in the shader -- render order is always 0 for landscape actors
		glUniform1i(renderOrderUniID, renderOrder);


		// bind location data to the element attrib array so it shows up in our shaders -- the location is zero (look in shader)
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, locBufferID);
		glVertexAttribPointer(
			0, // location 0 (see shader)
			2, // two elements per vertex (x,y)
			GL_FLOAT, // they are floats
			GL_FALSE, // not normalized (look up vector normalization)
			sizeof(GLfloat) * 2, // the next element is 2 floats later
			0 // dont copy -- use the GL_ARRAY_BUFFER instead
			);

		// bind UV data to the element attrib array so it shows up in our sahders -- the location is  (look in shader)
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, UVBufferID);
		glVertexAttribPointer(
			1, // location 1 (see shader)
			2, // two elements per vertex (u,v)
			GL_FLOAT, // they are floats
			GL_FALSE, // not normalized (look up vector normalization)
			sizeof(GLfloat) * 2, // the next element is 2 floats later
			0 // use the GL_ARRAY_BUFFER instead of copying on the spot
			);

		// bind the element buffer so it is used to make our draw call
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);

		// draw our chunk -- only one draw call
		glDrawElements(
			GL_TRIANGLES, // they are trianges
			elementCount, // these many verticies
			GL_UNSIGNED_INT, // the data is GLuint - unsigned int
			0 // use the buffer instead of raw data
			);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
}

void Chunk::draw()
{

	// return if there is no persistent chunk
	if (persistentChunk == NULL)
	{
		std::cout << "There is no persistent chunk" << std::endl;

		return;
	}

	// always draw the persistent chunk
	persistentChunk->drawChunk();

	// Currently only loops through all of the chunks and draws them -- soon to be better handling 
	for (GLuint y = 0; y < chunksSize.y; y++)
	{
		for (GLuint x = 0; x < chunksSize.x; x++)
		{
			chunks[x][y]->drawChunk();
		}
	}
}

glm::vec2 Chunk::getLocation()
{
	// return a copy of the location variable
	return location;
}