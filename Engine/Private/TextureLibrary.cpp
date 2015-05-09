#include "TextureLibrary.h"
#include "Logging.h"


uint32 TextureLibrary::texture = 0;
std::map<std::string, UVData> TextureLibrary::UVDataMap = std::map<std::string, UVData>();
uvec2 TextureLibrary::nextLocation = uvec2(0, 0);

uint32 TextureLibrary::getTextureHandle()
{
	return texture;
}

void TextureLibrary::addTexture(std::string key, const char* filename)
{
	// if we need to make a new texture, do it
	if (texture == 0)
	{
		//texture = allocateCompressedTextureLibraryFromDDS(TEXTURE_WIDTH, filename);

		nextLocation.x = nextLocation.y = 0;
	}

	ENG_LOG("\nLoading Texture " << filename);

	//appendDDS(texture, nextLocation.x * IMAGE_WIDTH, nextLocation.y * IMAGE_WIDTH, filename);
	
	// width of one texture in UV coordinates
	float pitch = 1.f / TEXTURE_WIDTH;

	// populate the data for the UVs -- Flip the UVs vertically because DDS stores them in the opposite direction.
	UVData data;
	data.bottomLeft =	vec2(pitch *  nextLocation.x, pitch * (nextLocation.y + 1));

	data.topLeft =		vec2(pitch *  nextLocation.x, pitch * (nextLocation.y));

	data.bottomRight =	vec2(pitch * (nextLocation.x + 1), pitch * (nextLocation.y + 1));

	data.topRight =		vec2(pitch * (nextLocation.x + 1), pitch * (nextLocation.y));

	// add it to the datamap
	UVDataMap[key] = data;

	// proceed to the next location
	nextLocation.x++;

	if (nextLocation.x >= TEXTURE_WIDTH)
	{
		nextLocation.x = 0;
		nextLocation.y++;

		if (nextLocation.y >= TEXTURE_WIDTH)
		{
			nextLocation.y = 0;
		}
	}
	ENG_LOG("\nTexture Loaded!\n");
}

UVData TextureLibrary::getUVData(std::string key)
{
	return UVDataMap[key];
}



