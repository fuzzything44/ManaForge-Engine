#include "Chunk.h"
#include "TextureLibrary.h"
#include "UVData.h"
#include "Logging.h"

#include <random>
#include <sstream>
#include <Runtime.h>


Chunk::Chunk(ivec2 locationIn, Model* model)
	: model(model),
	location(locationIn)
{
	
	// the persistent chunk doesn't get anything
	if (this != &(world->getPersistentChunk()))
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

Chunk::Chunk(ivec2 chunkLocation, vec2* vertexLocations, vec2* UVs, uint32* elems, uint32 numVerts, uint32 numElems) 
	: model(Runtime::get().moduleManager.getRenderer().newModel(vertexLocations, UVs, elems, numVerts, numElems)),
	location(chunkLocation)
{

}

vec2 Chunk::getLocation()
{
	// return a copy of the location variable
	return location;
}
