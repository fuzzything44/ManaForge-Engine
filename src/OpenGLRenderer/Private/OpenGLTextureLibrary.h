#pragma once

#include "OpenGLRendererConfig.h"

#include <TextureLibrary.h>

#include <map>

class OpenGLTextureLibrary : public TextureLibrary
{
public:
	explicit OpenGLTextureLibrary(uint16 maxNumElements, uint16 individualSize = 256);

	virtual ~OpenGLTextureLibrary() override;

	// from TextureLibrary
	void addImage(const std::string& name) override;
	virtual QuadUVCoords getUVCoords(const std::string& name) override;

	// from Texture
	virtual uint32 getID() override;
	virtual void setFilterMode(FilterMode mode) override;
	virtual FilterMode getFilterMode() const override;

	virtual void setWrapMode(WrapMode newMode) override;
	virtual WrapMode getWrapMode() const override;

private:

	GLuint texHandle;

	uvec2 nextLocation;

	uint16 individualSize;
	uint16 maxElements;
	uint16 width;

	std::map<std::string, QuadUVCoords> UVs;


	void appendDDS(uint32 texToAppend, uint32 Xoffset, uint32 Yoffset, const char* filepath);
	uint32 allocateCompressedTextureLibraryFromDDS(uint32 num, const char* filepath);
};