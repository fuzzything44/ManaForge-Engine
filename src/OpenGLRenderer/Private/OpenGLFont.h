#pragma once
#include "OpenGLRendererConfig.h"

#include "OpenGLCharacterData.h"

#include <Font.h>

#include <unordered_map>

class OpenGLFont : public Font
{
public:
	OpenGLFont(const std::wstring& name = std::wstring());
	void init(const std::wstring& name);

	inline OpenGLCharacterData getCharacterData(wchar_t ch);

private:
	std::string fontName;

	std::unordered_map<wchar_t, OpenGLCharacterData> charData;
};	

inline OpenGLCharacterData OpenGLFont::getCharacterData(wchar_t ch)
{
	auto iter = charData.find(ch);

	if (iter != charData.end()) {
		return iter->second;
	}

	MFLOG(Error) << "Cannot find char " << ch << " in font " << fontName;
}