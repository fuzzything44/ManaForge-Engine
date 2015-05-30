#include "OpenGLTexture.h"
#include "SOIL/SOIL.h"


#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

OpenGLTexture::OpenGLTexture(const std::string& path)
	: path(path)
{

	if (path != "")
	{
		auto iter = textures.find(path);
		if (iter == textures.end())
		{
			std::string qualifiedPath = "textures\\" + path + ".dds";

			ID = SOIL_load_OGL_texture(
				qualifiedPath.c_str(),		// path
				4,							// 4 channels
				0,							// create a new texture ID in OGL
				SOIL_FLAG_DDS_LOAD_DIRECT	// It is a dds
			);


			textures[path] = this;
		}
		else
		{
			// copy it from the other already existing one
			*this = *(iter->second);
		}
	}
}

uint32 OpenGLTexture::getID()
{
	return ID;
}

OpenGLTexture::~OpenGLTexture()
{

}

int32 OpenGLTexture::loadDDS(const std::string& filename)
{

	unsigned char header[124];

	FILE *fp;


	/* try to open the file */
	fopen_s(&fp, filename.c_str(), "rb");
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

	auto height = reinterpret_cast<unsigned int>(&(header[8]));
	auto width = reinterpret_cast<unsigned int>(&(header[12]));
	auto linearSize = reinterpret_cast<unsigned int>(&(header[16]));
	auto mipMapCount = reinterpret_cast<unsigned int>(&(header[24]));
	auto fourCC = reinterpret_cast<unsigned int>(&(header[80]));

	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = static_cast<unsigned char*>(malloc(bufsize * sizeof(unsigned char)));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

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
		return 0;
	}

	// Create one OpenGL texture
	uint32 textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;
	}
	free(buffer);

	return textureID;
}


//void OpenGLTexture::appendDDS(uint32 texToAppend, uint32 Xoffset, uint32 Yoffset, const char* filepath)
//{
//
//	unsigned char header[124];
//
//	FILE *fp;
//
//
//	/* try to open the file */
//	fopen_s(&fp, filepath, "rb");
//	if (fp == nullptr)
//		return;
//
//	/* verify the type of file */
//	char filecode[4];
//	fread(filecode, 1, 4, fp);
//	if (strncmp(filecode, "DDS ", 4) != 0) {
//		fclose(fp);
//		return;
//	}
//
//	/* get the surface desc */
//	fread(&header, 124, 1, fp);
//
//	unsigned int height = *reinterpret_cast<unsigned int*>(&(header[8]));
//	unsigned int width = *reinterpret_cast<unsigned int*>(&(header[12]));
//	unsigned int linearSize = *reinterpret_cast<unsigned int*>(&(header[16]));
//	unsigned int mipMapCount = *reinterpret_cast<unsigned int*>(&(header[24]));
//	unsigned int fourCC = *reinterpret_cast<unsigned int*>(&(header[80]));
//
//	unsigned char * buffer;
//	unsigned int bufsize;
//	/* how big is it going to be including all mipmaps? */
//	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
//	buffer = static_cast<unsigned char*>(malloc(bufsize * sizeof(unsigned char)));
//	fread(buffer, 1, bufsize, fp);
//	/* close the file pointer */
//	fclose(fp);
//
//	//unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
//	unsigned int format;
//	switch (fourCC)
//	{
//	case FOURCC_DXT1:
//		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
//		break;
//	case FOURCC_DXT3:
//		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
//		break;
//	case FOURCC_DXT5:
//		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
//		break;
//	default:
//		free(buffer);
//		return;
//	}
//
//	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
//	unsigned int offset = 0;
//
//	glBindTexture(GL_TEXTURE_2D, texToAppend);
//
//	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
//	{
//
//		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
//
//		glCompressedTexSubImage2D(GL_TEXTURE_2D, level, Xoffset, Yoffset, width, height, format, size, buffer + offset);
//
//		offset += size;
//		width /= 2;
//		height /= 2;
//		Xoffset /= 2;
//		Yoffset /= 2;
//	}
//}
//
//uint32 OpenGLTexture::allocateCompressedTextureLibraryFromDDS(uint32 num, const char* filepath)
//{
//	unsigned char header[124];
//
//	FILE *fp;
//
//
//	/* try to open the file */
//	fopen_s(&fp, filepath, "rb");
//	if (fp == nullptr)
//		return 0;
//
//	/* verify the type of file */
//	char filecode[4];
//	fread(filecode, 1, 4, fp);
//	if (strncmp(filecode, "DDS ", 4) != 0) {
//		fclose(fp);
//		return 0;
//	}
//
//	/* get the surface desc */
//	fread(&header, 124, 1, fp);
//
//	unsigned int height = *reinterpret_cast<unsigned int*>(&(header[8]));
//	unsigned int width = *reinterpret_cast<unsigned int*>(&(header[12]));
//	//unsigned int linearSize = *reinterpret_cast<unsigned int*>(&(header[16]));
//	unsigned int mipMapCount = *reinterpret_cast<unsigned int*>(&(header[24]));
//	unsigned int fourCC = *reinterpret_cast<unsigned int*>(&(header[80]));
//
//
//	unsigned int format;
//	switch (fourCC)
//	{
//	case FOURCC_DXT1:
//		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
//		break;
//	case FOURCC_DXT3:
//		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
//		break;
//	case FOURCC_DXT5:
//		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
//		break;
//	default:
//		return 0;
//	}
//
//	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
//	//unsigned int offset = 0;
//
//	uint32 texture;
//	glGenTextures(1, &texture);
//	glBindTexture(GL_TEXTURE_2D, texture);
//
//
//	for (uint32 i = 0; i < mipMapCount; i++)
//	{
//
//		uint32 size = ((width + 3) / 4)*((height + 3) / 4) * blockSize;
//
//		glCompressedTexImage2D(GL_TEXTURE_2D, i, format, width * num, height * num, 0, size * num * num, nullptr);
//
//		width /= 2;
//		height /= 2;
//
//	}
//
//	return texture;
//}
//
//
