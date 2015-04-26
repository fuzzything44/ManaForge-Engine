#include "Chunk.h"
#include "TextureLibrary.h"
#include "UVData.h"
#include "Logging.h"
#include <random>


// set inital value for static variables
Chunk*** Chunk::chunks = nullptr;
Chunk* Chunk::persistentChunk = nullptr;
uvec2 Chunk::chunksSize = uvec2(0, 0);
int32 Chunk::characterLocUniformHandleChunk = -1;
int32 Chunk::texUniformHandleChunk = -1;
uint32 Chunk::program = 0;
mat4* Chunk::viewMat = nullptr;



void Chunk::initChunks(uint32 programIn, mat4* viewMatIn, const uvec2& chunksSizeIn)
{
	chunksSize = chunksSizeIn;
	program = programIn;
	viewMat = viewMatIn;

	ENG_LOG("\nInitalizing Chunks:")

	// allocate the pointer
	chunks = static_cast<Chunk***>(malloc(sizeof(Chunk**) * chunksSize.y));

	for (uint32 x = 0; x < chunksSize.x; x++)
	{
		// allocate the column -- you can read about malloc: http://www.cplusplus.com/reference/cstdlib/malloc/
		chunks[x] = static_cast<Chunk**>(malloc(sizeof(Chunk*) * chunksSize.y));
		for (uint32 y = 0; y < chunksSize.y; y++)
		{
			ENG_LOG("\nInit Chunk (" << x << ", " << y << ")");
			chunks[x][y] = new Chunk(vec2(x * CHUNK_WIDTH, y * CHUNK_WIDTH));
		}
	}
	ENG_LOG("\nDone Initalizing Chunks.\n")

	persistentChunk = new Chunk();

	// initalize all handles
	texUniformHandleChunk = glGetUniformLocation(program, "texArray");
	characterLocUniformHandleChunk = glGetUniformLocation(program, "characterLoc");


}

Chunk::Chunk(ivec2 locationIn)
	: location(locationIn)
{STACK
	
	// the persistent chunk doesn't get anything
	if (this != persistentChunk)
	{
		srand(reinterpret_cast<unsigned int>(this));

		// init the generator -- will remove with loading
		std::mersenne_twister_engine < std::uint_fast32_t, 32, 624, 397, 31,
			0x9908b0df, 11,
			0xffffffff, 7,
			0x9d2c5680, 15,
			0xefc60000, 18, 1812433253 > gen(rand());

		// the data for locations 
		std::vector<float> locData;
		locData.reserve(8 * CHUNK_WIDTH * CHUNK_WIDTH);

		// allocate the same amount as above for texture coordinates
		std::vector<UVData> texCoords;
		texCoords.reserve(CHUNK_WIDTH * CHUNK_WIDTH);

		// the element data 
		std::vector<uint32> eboData;
		eboData.reserve(6 * CHUNK_WIDTH * CHUNK_WIDTH);

		// init vbos, vaos, and textures
		for (int y = 0; y < CHUNK_WIDTH; y++)
		{
			for (int x = 0; x < CHUNK_WIDTH; x++)
			{
				float startX = x + location.x;
				float startY = y + location.y;


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
				uint32 tex = std::uniform_int_distribution<int>(0, 16)(gen);
				std::stringstream stream;
				stream << tex;
				texCoords.push_back(TextureLibrary::getUVData(stream.str()));

				

				uint32 startIdx = (y * CHUNK_WIDTH + x) * 4;

				eboData.push_back(startIdx);
				eboData.push_back(startIdx + 1);
				eboData.push_back(startIdx + 2);

				eboData.push_back(startIdx + 1);
				eboData.push_back(startIdx + 2);
				eboData.push_back(startIdx + 3);

			}
		}



	}
}

void Chunk::draw(Actor* character, float deltaTime)
{STACK
	
}

vec2 Chunk::getLocation()
{STACK
	// return a copy of the location variable
	return location;
}

uvec2 Chunk::getChunkSize()
{STACK
	return chunksSize;
}
