#pragma once

#include "OpenGLRendererConfig.h"

#include <TextureLibrary.h>

class OpenGLTextureLibrary : public TextureLibrary
{
public:
	explicit OpenGLTextureLibrary(uint16 maxNumElements, uint16 individualSize = 256);

	virtual ~OpenGLTextureLibrary() override;

	void addImage(const std::string& name) override;

	virtual uint32 getID() override;

private:

	GLuint texHandle;

	uvec2 nextLocation;

	uint16 individualSize;
	uint16 maxElements;
	uint16 width;


	void appendDDS(uint32 texToAppend, uint32 Xoffset, uint32 Yoffset, const char* filepath);
	uint32 allocateCompressedTextureLibraryFromDDS(uint32 num, const char* filepath);
};