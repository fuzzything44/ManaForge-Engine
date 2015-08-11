
#pragma once
#include "Engine.h"

class Font;

class TextBox
{
public:
	virtual ~TextBox() {};

	virtual void setText(const std::u16string& text) = 0;
	virtual const std::u16string& getText() const = 0;

	virtual void setFont(std::shared_ptr<Font> newFont) = 0;
	virtual std::shared_ptr<Font> getFont() const = 0;
};