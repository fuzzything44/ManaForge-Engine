#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>
#include <map>

// Hex defines for the compression methods.
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

// size of each texture
static const GLuint IMAGE_WIDTH = 256;

// amount of textures in one direction. Size of texture is (IMAGE_WIDTH * TEXTURE_WIDTH)^2
static const GLuint TEXTURE_WIDTH = 2;


struct UVData
{
	glm::vec2 bottomLeft;
	glm::vec2 topLeft;
	glm::vec2 bottomRight;
	glm::vec2 topRight;
};

// handles the texture libraries
class TextureLibrary
{
public:

	static UVData getUVData(std::string key);

	static void addTexture(std::string key, const char* filename);

	static GLuint getTextureHandle(GLuint idx);

private:
	
	// stores the handles to the textures
	static std::vector<GLuint> textures;

	static std::map<std::string, UVData> UVDataMap;

	/// <summary> load a DSS format to the texture specified at the location specified </summary>
	/// <param name='texToAppend'> The handle for the texture to append the image to </param>
	/// <param name='Xoffset'> The X offset of the original texture to place the image </param>
	/// <param name='Yoffset'> The Y offset of the original texture to place the image </param>
	/// <param name='filepath'> The path to the DDS file. </param>
	static void appendDDS(GLuint texToAppend, GLuint Xoffset, GLuint Yoffset, const GLchar* filepath);


	/// <summary> allocates an empty texture library from the format of the given DDS file </summary>
	/// <param name='num'> The amount of textures in one direction. total textures is up to num ^ 2 </param>
	/// <param name='filepath'> The path to the DDS file to load </param>
	static GLuint allocateCompressedTextureLibraryFromDDS(GLuint num, const char* filepath);

	// the location of the next image.
	static glm::uvec2 nextLocation;
	static GLint currentTexture;
};