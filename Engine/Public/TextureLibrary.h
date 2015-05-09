#pragma once
#include "Engine.h"
#include "UVData.h"

#include <string>
#include <map>

// Hex defines for the compression methods.
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

// size of each texture
static const uint32 IMAGE_WIDTH = 256;

// amount of textures in one direction. Size of texture is (IMAGE_WIDTH * TEXTURE_WIDTH)^2
static const uint32 TEXTURE_WIDTH = 16;

// TODO: remove
class TextureLibrary
{
public:

	static ENGINE_API UVData getUVData(std::string key);

	static ENGINE_API void addTexture(std::string key, const char* filename);


	static ENGINE_API uint32 getTextureHandle();

private:
	

	static std::map<std::string, UVData> UVDataMap;

	// the location of the next image.
	static uvec2 nextLocation;

	// stores the handles to the textures
	static uint32 texture;
};