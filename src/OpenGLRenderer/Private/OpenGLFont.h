#pragma once
#include "OpenGLRendererConfig.h"

#include "OpenGLCharacterData.h"

#include <Font.h>

#include <unordered_map>
#include <string>

class OpenGLMaterialSource;
class OpenGLTexture;
class OpenGLRenderer;
class OpenGLTextBox;

class OpenGLFont : public Font
{
public:
	OpenGLFont(OpenGLRenderer& renderer, const path_t& name = path_t());
	OpenGLFont(const OpenGLFont& other) = delete;
	OpenGLFont(OpenGLFont&& other) = default;

	virtual ~OpenGLFont();

	OpenGLCharacterData getCharacterData(wchar_t ch);
	std::shared_ptr<OpenGLMaterialSource> getMaterialSource();

	void render(OpenGLTextBox& box);

private:
	path_t fontName;

	std::unordered_map<wchar_t, OpenGLCharacterData> charData;

	std::shared_ptr<OpenGLMaterialSource> matSource;
	GLuint tex;

	OpenGLRenderer& renderer;
};
