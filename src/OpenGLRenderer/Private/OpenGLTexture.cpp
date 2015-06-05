#include "OpenGLTexture.h"
#include "SOIL/SOIL.h"


#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

std::map<std::string, OpenGLTexture*> OpenGLTexture::textures = std::map<std::string, OpenGLTexture*>();

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

OpenGLTexture::~OpenGLTexture()
{
	glDeleteTextures(1, &ID);
}

uint32 OpenGLTexture::getID()
{
	return ID;
}

void OpenGLTexture::setFilterMode(FilterMode newMode)
{
	glBindTexture(GL_TEXTURE_2D, ID);

	switch (newMode)
	{
	case FilterMode::LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case FilterMode::NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	case FilterMode::MIPMAP_LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case FilterMode::MIPMAP_NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	default:
		break;

	}
}

Texture::FilterMode OpenGLTexture::getFilterMode() const
{
	glBindTexture(GL_TEXTURE_2D, ID);

	int mode;
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &mode);

	switch (mode)
	{
	case GL_LINEAR:
		return Texture::FilterMode::LINEAR;
		break;
	case GL_NEAREST:
		return Texture::FilterMode::NEAREST;
		break;
	case GL_LINEAR_MIPMAP_LINEAR:
		return Texture::FilterMode::MIPMAP_LINEAR;
		break;
	case GL_NEAREST_MIPMAP_NEAREST:
		return Texture::FilterMode::MIPMAP_NEAREST;
		break;
	default:
		return Texture::FilterMode::LINEAR; // IDK
		break;
	}
}

void OpenGLTexture::setWrapMode(WrapMode newMode)
{
	glBindTexture(GL_TEXTURE_2D, ID);
	switch(newMode)
	{
	case WrapMode::CLAMP_TO_EDGE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	case WrapMode::MIRRORED_REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;
	case WrapMode::REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	default:
		break;
	}
}

Texture::WrapMode OpenGLTexture::getWrapMode() const
{
	glBindTexture(GL_TEXTURE_2D, ID);

	GLint wrap;

	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &wrap); // retreive the data

	switch (wrap)
	{
	case GL_CLAMP_TO_EDGE:
		return WrapMode::CLAMP_TO_EDGE;
	case GL_MIRRORED_REPEAT:
		return WrapMode::MIRRORED_REPEAT;
	case GL_REPEAT:
		return WrapMode::REPEAT;
	default:
		return WrapMode::REPEAT; // just a deault fallback
	}
}


void OpenGLTexture::deleteAll()
{
	for (auto& elem : textures)
	{
		delete elem.second;
	}
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

