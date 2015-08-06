#pragma once

#include "OpenGLRendererConfig.h"

#include <Texture.h>

#include <map>

// forward declaration
class OpenGLMaterialInstance;
class OpenGLRenderer;

class OpenGLTexture : public Texture
{
	friend OpenGLMaterialInstance;
	friend OpenGLRenderer;

public:
	explicit OpenGLTexture(const path_t& path);

	uint32 getID();

	virtual void setFilterMode(FilterMode mode) override;
	virtual FilterMode getFilterMode() const override;

	virtual void setWrapMode(WrapMode newMode) override;
	virtual WrapMode getWrapMode() const override;

	~OpenGLTexture() override;

private:
	GLuint ID;

	path_t path;

	int32 loadDDS(const std::string& filename);
	// void appendDDS(uint32 texToAppend, uint32 Xoffset, uint32 Yoffset, const char* filepath);
	// uint32 allocateCompressedTextureLibraryFromDDS(uint32 num, const char* filepath);			// keeping
	// just
	// in
	// case
};
