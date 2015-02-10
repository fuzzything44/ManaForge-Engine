#include "TextureLibrary.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

std::vector<GLuint> TextureLibrary::textures = std::vector<GLuint>();
std::map<std::string, UVData> TextureLibrary::UVDataMap = std::map<std::string, UVData>();
glm::uvec2 TextureLibrary::nextLocation = glm::uvec2(0, 0);
GLint TextureLibrary::currentTexture = 0;

GLuint TextureLibrary::getTextureHandle(GLuint idx)
{
	return textures[idx];
}

void TextureLibrary::addTexture(std::string key, const char* filename)
{
	// if we need to make a new texture, do it
	if (currentTexture >= textures.size())
	{
		textures.push_back(allocateCompressedTextureLibraryFromDDS(TEXTURE_WIDTH, filename));

		nextLocation.x = nextLocation.y = 0;
	}

	appendDDS(textures[currentTexture], nextLocation.x * IMAGE_WIDTH, nextLocation.y * IMAGE_WIDTH, filename);
	
	// width of one texture in UV coordinates
	float pitch = 1.f / TEXTURE_WIDTH;

	// populate the data for the UVs
	UVData data;
	data.bottomLeft =	glm::vec2(pitch *  nextLocation.x		, pitch *  nextLocation.y);
	data.topLeft =		glm::vec2(pitch *  nextLocation.x		, pitch * (nextLocation.y + 1));
	data.bottomRight =	glm::vec2(pitch * (nextLocation.x + 1)	, pitch *  nextLocation.y);
	data.topRight =		glm::vec2(pitch * (nextLocation.x + 1)	, pitch * (nextLocation.y + 1));

	// add it to the datamap
	UVDataMap[key] = data;

	// proceed to the next location
	nextLocation.x++;

	if (nextLocation.x >= TEXTURE_WIDTH)
	{
		nextLocation.x = 0;
		nextLocation.y++;

		if (nextLocation.y > TEXTURE_WIDTH)
		{
			nextLocation.y = 0;
			currentTexture++;
		}
	}
}

UVData TextureLibrary::getUVData(std::string key)
{
	return UVDataMap.find(key)->second;
}


void TextureLibrary::appendDDS(GLuint texToAppend, GLuint Xoffset, GLuint Yoffset, const GLchar* filepath)
{

	unsigned char header[124];

	FILE *fp;


	/* try to open the file */
	fopen_s(&fp, filepath, "rb");
	if (fp == NULL)
		return;

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);

	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return;
	}

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	glBindTexture(GL_TEXTURE_2D, texToAppend);

	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{

		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;

		glCompressedTexSubImage2D(GL_TEXTURE_2D, level, Xoffset, Yoffset, width, height, format, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;
		Xoffset /= 2;
		Yoffset /= 2;
	}
}


GLuint TextureLibrary::allocateCompressedTextureLibraryFromDDS(GLuint num, const char* filepath)
{
	unsigned char header[124];

	FILE *fp;


	/* try to open the file */
	fopen_s(&fp, filepath, "rb");
	if (fp == NULL)
		return 0;

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);


	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		return 0;
	}

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);


	for (int i = 0; i < mipMapCount; i++)
	{

		unsigned int size = ((width + 3) / 4)*((height + 3) / 4) * blockSize;

		glCompressedTexImage2D(GL_TEXTURE_2D, i, format, width * num, height * num, 0, size * num * num, 0);

		width /= 2;
		height /= 2;

	}

	return texture;
}
