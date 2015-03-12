#pragma once
#include "Engine.h"
#include "Logging.h"
#include <GLTools/glew.h>
#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>

// Hex defines for the compression methods.
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

// size of each texture
static const GLuint IMAGE_WIDTH = 256;

// amount of textures in one direction. Size of texture is (IMAGE_WIDTH * TEXTURE_WIDTH)^2
static const GLuint TEXTURE_WIDTH = 16;

/**
 * \struct	UVData
 *
 * \brief	An uv data.
 *
 * \author	Russell
 * \date	3/12/2015
 */

struct UVData
{
	UVData(vec2 bottomLeftIn = vec2(0.f, 0.f), vec2 topLeftIn = vec2(0.f, 0.f),
		vec2 bottomRightIn = vec2(0.f, 0.f), vec2 topRightIn = vec2(0.f, 0.f))
		: bottomLeft(bottomLeftIn),
		topLeft(topLeftIn),
		bottomRight(bottomRightIn),
		topRight(topRightIn) { }


	vec2 bottomLeft;
	vec2 topLeft;
	vec2 bottomRight;
	vec2 topRight;
};

/**
 * \class	TextureLibrary
 *
 * \brief	handles the texture libraries.
 *
 * \author	Russell
 * \date	3/12/2015
 */

class TextureLibrary
{
public:

	/**
	 * \fn	static ENGINE_API UVData TextureLibrary::getUVData(std::string key);
	 *
	 * \brief	Gets uv data.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param	key	The key.
	 *
	 * \return	The uv data.
	 */

	static ENGINE_API UVData getUVData(std::string key);

	/**
	 * \fn	static ENGINE_API void TextureLibrary::addTexture(std::string key, const char* filename);
	 *
	 * \brief	Adds a texture to 'filename'.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param	key			The key.
	 * \param	filename	Filename of the file.
	 */

	static ENGINE_API void addTexture(std::string key, const char* filename);

	/**
	 * \fn	static ENGINE_API GLuint TextureLibrary::getTextureHandle();
	 *
	 * \brief	Gets texture handle.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \return	The texture handle.
	 */

	static ENGINE_API GLuint getTextureHandle();

private:
	

	static std::unordered_map<std::string, UVData> UVDataMap;

	/**
	 * \fn	static void TextureLibrary::appendDDS(GLuint texToAppend, GLuint Xoffset, GLuint Yoffset, const GLchar* filepath);
	 *
	 * \brief	Appends the DDS.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param	texToAppend	The tex to append.
	 * \param	Xoffset	   	The xoffset.
	 * \param	Yoffset	   	The yoffset.
	 * \param	filepath   	The filepath.
	 */

	static void appendDDS(GLuint texToAppend, GLuint Xoffset, GLuint Yoffset, const GLchar* filepath);

	/**
	 * \fn	static GLuint TextureLibrary::allocateCompressedTextureLibraryFromDDS(GLuint num, const char* filepath);
	 *
	 * \brief	Allocate compressed texture library from DDS.
	 *
	 * \author	Russell
	 * \date	3/12/2015
	 *
	 * \param	num			Number of.
	 * \param	filepath	The filepath.
	 *
	 * \return	A GLuint.
	 */

	static GLuint allocateCompressedTextureLibraryFromDDS(GLuint num, const char* filepath);

	// the location of the next image.
	static uvec2 nextLocation;

	// stores the handles to the textures
	static GLuint texture;
};