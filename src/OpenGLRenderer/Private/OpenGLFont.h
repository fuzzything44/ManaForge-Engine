#pragma once
#include "OpenGLRendererConfig.h"

#include "OpenGLCharacterData.h"

#include <Font.h>

#include <unordered_map>

class OpenGLFont : public Font
{
public:
	OpenGLFont();

	OpenGLCharacterData getCharacterData(wchar_t ch);

private:
	std::string fontName;

	std::unordered_map<wchar_t, OpenGLCharacterData> charData;
};
