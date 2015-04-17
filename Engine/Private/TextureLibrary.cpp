#include "TextureLibrary.h"


uint32 TextureLibrary::texture = 0;
std::map<std::string, UVData> TextureLibrary::UVDataMap = std::map<std::string, UVData>();
uvec2 TextureLibrary::nextLocation = uvec2(0, 0);

uint32 TextureLibrary::getTextureHandle()
{STACK
	return texture;
}

void TextureLibrary::addTexture(std::string key, const char* filename)
{STACK
	// if we need to make a new texture, do it
	if (texture == 0)
	{
		texture = allocateCompressedTextureLibraryFromDDS(TEXTURE_WIDTH, filename);

		nextLocation.x = nextLocation.y = 0;
	}

	ENG_LOG("\nLoading Texture " << filename);

	appendDDS(texture, nextLocation.x * IMAGE_WIDTH, nextLocation.y * IMAGE_WIDTH, filename);
	
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
{STACK
	return UVDataMap[key];
}


void TextureLibrary::appendDDS(uint32 texToAppend, uint32 Xoffset, uint32 Yoffset, const char* filepath)
{STACK

	unsigned char header[124];

	FILE *fp;


	/* try to open the file */
	fopen_s(&fp, filepath, "rb");
	if (fp == nullptr)
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

	unsigned int height = *reinterpret_cast<unsigned int*>(&(header[8]));
	unsigned int width = *reinterpret_cast<unsigned int*>(&(header[12]));
	unsigned int linearSize = *reinterpret_cast<unsigned int*>(&(header[16]));
	unsigned int mipMapCount = *reinterpret_cast<unsigned int*>(&(header[24]));
	unsigned int fourCC = *reinterpret_cast<unsigned int*>(&(header[80]));

	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = static_cast<unsigned char*>(malloc(bufsize * sizeof(unsigned char)));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	//unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
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


uint32 TextureLibrary::allocateCompressedTextureLibraryFromDDS(uint32 num, const char* filepath)
{STACK
	unsigned char header[124];

	FILE *fp;


	/* try to open the file */
	fopen_s(&fp, filepath, "rb");
	if (fp == nullptr)
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

	unsigned int height = *reinterpret_cast<unsigned int*>(&(header[8]));
	unsigned int width = *reinterpret_cast<unsigned int*>(&(header[12]));
	//unsigned int linearSize = *reinterpret_cast<unsigned int*>(&(header[16]));
	unsigned int mipMapCount = *reinterpret_cast<unsigned int*>(&(header[24]));
	unsigned int fourCC = *reinterpret_cast<unsigned int*>(&(header[80]));


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
	//unsigned int offset = 0;

	uint32 texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);


	for (uint32 i = 0; i < mipMapCount; i++)
	{

		uint32 size = ((width + 3) / 4)*((height + 3) / 4) * blockSize;

		glCompressedTexImage2D(GL_TEXTURE_2D, i, format, width * num, height * num, 0, size * num * num, nullptr);

		width /= 2;
		height /= 2;

	}

	return texture;
}
