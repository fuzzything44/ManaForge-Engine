#pragma once

#include <cstdlib>

#include <Engine.h>

#include <RefCounted.h>

#include <QOpenGLFunctions_3_3_Core>

struct OpenGLTexture : RefCounted<OpenGLTexture>
{
	OpenGLTexture(path_t path)
	{
		path_t qualifiedPath = L"textures\\" + path.wstring() + L".dds";
		
		// get header data
		unsigned char header[124];
  
		FILE *fp;

		/* try to open the file */
		fp = fopen(qualifiedPath.c_str(), "rb");
		if (fp == NULL)
			MFLOG(Error) << "File could not be opened: " << qualifiedPath;

		/* verify the type of file */
		char filecode[4];
		fread(filecode, 1, 4, fp);
		if (strncmp(filecode, "DDS ", 4) != 0) {
			fclose(fp);
			MFLOG(Error) << "Error loading DSS file: " << qualifiedPath << ".";
			
		}

		/* get the surface desc */
		fread(&header, 124, 1, fp); 

		unsigned int height			= *(unsigned int*)&(header[8 ]);
		unsigned int width			= *(unsigned int*)&(header[12]);
		unsigned int linearSize		= *(unsigned int*)&(header[16]);
		unsigned int mipMapCount	= *(unsigned int*)&(header[24]);
		unsigned int fourCC			= *(unsigned int*)&(header[80]);
		
		// read to the buffer
		unsigned char * buffer;
		unsigned int bufsize;
		/* how big is it going to be including all mipmaps? */
		bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
		buffer = (unsigned char*)std::malloc(bufsize * sizeof(unsigned char));
		fread(buffer, 1, bufsize, fp);
		/* close the file pointer */
		fclose(fp);
		
		constexpr unsigned int DXT1 = 0x44585431;
		constexpr unsigned int DXT3 = 0x44585433;
		constexpr unsigned int DXT5 = 0x44585435;
		
		unsigned int components  = (fourCC == DXT1) ? 3 : 4;
		unsigned int format;
		switch(fourCC)
		{
		case DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			free(buffer);
			MFLOG(Error) << "Unrecognized format in DDS: " << fourCC;
		}
		
		QOpenGLFunctions_3_3_Core funs;
		
		 // Create one OpenGL texture
		funs.glGenTextures(1, &handle);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, handle);
		
		unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
		unsigned int offset = 0;

		/* load the mipmaps */
		for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
		{
			unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
			funs.glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 
				0, size, buffer + offset);

			offset += size;
			width  /= 2;
			height /= 2;
		}
		free(buffer); 

		
	}

	void destroy()
	{
		if (handle) QOpenGLFunctions_3_3_Core{}.glDeleteTextures(1, &handle);
	}

	GLuint handle;
};
