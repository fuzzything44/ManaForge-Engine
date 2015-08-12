#pragma once
#include "OpenGLRendererConfig.h"

#include "OpenGLCharacterData.h"

#include <Font.h>

#include <unordered_map>

class OpenGLMaterialSource;

class OpenGLFont : public Font
{
public:
	OpenGLFont();

	OpenGLCharacterData getCharacterData(wchar_t ch);
	std::shared_ptr<OpenGLMaterialSource> getMaterialSource();

private:
	std::string fontName;

	std::unordered_map<wchar_t, OpenGLCharacterData> charData;

	std::shared_ptr<OpenGLMaterialSource> matSource;
};
