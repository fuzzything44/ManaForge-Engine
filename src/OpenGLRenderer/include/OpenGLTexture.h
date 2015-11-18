#pragma once

#include <Engine.h>

#include <RefCounted.h>

#include "SOIL/SOIL.h"

#include <GL/glew.h>

struct OpenGLTexture : RefCounted<OpenGLTexture>
{
	OpenGLTexture() : handle(0) { };

	OpenGLTexture(path_t path)
	{
		path_t qualifiedPath = L"textures\\" + path.wstring() + L".dds";

		handle = SOIL_load_OGL_texture(qualifiedPath.string().c_str(), // path
			4,													   // 4 channels
			0,													   // create a new texture ID in OGL
			SOIL_FLAG_DDS_LOAD_DIRECT | SOIL_FLAG_MIPMAPS
			| SOIL_FLAG_POWER_OF_TWO // It is a dds and we want mipmaps
			);

		glBindTexture(GL_TEXTURE_2D, handle);

		// set params
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	}

	void destroy()
	{
		if (handle) glDeleteTextures(1, &handle);
	}
	void invalidate()
	{
		handle = 0;
	}

	GLuint handle;
};
