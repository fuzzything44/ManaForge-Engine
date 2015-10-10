#pragma once
#include "Engine.h"

#include "Widget.h"

#include "Clamp.h"

class Font;

class TextBoxWidget : public Widget
{
public:
	TextBoxWidget(Widget* owner)
		: Widget{owner}
	{
	}

	virtual void setText(const std::u16string& text) = 0;
	virtual const std::u16string getText() const = 0;

	virtual void setSize(float newSize) = 0;
	virtual float getSize() const = 0;

	virtual void setThickness(Clampf<0, 0, 1, 0> thickness) = 0;
	virtual Clampf<0, 0, 1, 0> getThickness() const = 0;

	virtual void setColor(vec4 color) = 0;
	virtual vec4 getColor() const = 0;

	virtual Widget* getOwner() = 0;
	virtual const Widget* getOwner() const = 0;

	virtual void setFont(Font* newFont) = 0;
	virtual Font* getFont() const = 0;
};
