#pragma once
#include "Engine.h"
#include "Clamp.h"

class Font;

class TextBox
{
public:
	virtual ~TextBox(){};

	virtual void setText(const std::u16string& text) = 0;
	virtual const std::u16string getText() const = 0;

	virtual void setSize(float newSize) = 0;
	virtual float getSize() const = 0;

	virtual void setThickness(Clampf<0, 0, 1, 0> thickness) = 0;
	virtual Clampf<0, 0, 1, 0> getThickness() const = 0;

	virtual void setColor(vec4 color) = 0;
	virtual vec4 getColor() const = 0;

	// on a scale from 0-1
	virtual void setLocation(vec2 loc) = 0;
	virtual vec2 getLocation() const = 0;

	virtual void setFont(Font* newFont) = 0;
	virtual Font* getFont() const = 0;

	virtual void draw() = 0;
};
