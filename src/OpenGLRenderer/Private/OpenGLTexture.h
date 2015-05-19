#pragma once

#include "OpenGLRendererConfig.h"

#include <Texture.h>

#include <map>

// forward declaration
class OpenGLMaterial;

class OpenGLTexture : public Texture
{
	friend OpenGLMaterial;
public:

	explicit OpenGLTexture(const std::string& path);

	~OpenGLTexture() override;

	std::string getPath() const override;

private:

	GLuint ID;

	std::string path;

	std::map<std::string, OpenGLTexture> textures;




	int32 loadDDS(const std::string& filename);
	void appendDDS(uint32 texToAppend, uint32 Xoffset, uint32 Yoffset, const char* filepath);
	uint32 allocateCompressedTextureLibraryFromDDS(uint32 num, const char* filepath);
};
