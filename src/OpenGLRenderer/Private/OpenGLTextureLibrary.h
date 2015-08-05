#pragma once

#include "OpenGLRendererConfig.h"

#include <TextureLibrary.h>

#include <map>

class OpenGLRenderer;

class OpenGLTextureLibrary : public TextureLibrary
{
  public:
	explicit OpenGLTextureLibrary(OpenGLRenderer& renderer);

	virtual ~OpenGLTextureLibrary() override;

	// from TextureLibrary
	virtual void addImage(const std::string& name) override;
	virtual boost::optional<QuadUVCoords> getUVCoords(const std::string& name) override;
	virtual void init(uint16 maxElems, uint16 individualSize) override;

	// from Texture
	virtual void setFilterMode(FilterMode mode) override;
	virtual FilterMode getFilterMode() const override;

	virtual void setWrapMode(WrapMode newMode) override;
	virtual WrapMode getWrapMode() const override;

	uint32 getID();

  private:
	GLuint texHandle;

	uvec2 nextLocation;

	uint16 individualSize;
	uint16 maxElements;
	uint16 width;

	std::map<std::string, QuadUVCoords> UVs;

	OpenGLRenderer& renderer;

	void appendDDS(uint32 texToAppend, uint32 Xoffset, uint32 Yoffset, const char* filepath);
	uint32 allocateCompressedTextureLibraryFromDDS(uint32 num, const char* filepath);
};