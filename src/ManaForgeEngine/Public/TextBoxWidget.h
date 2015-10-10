#pragma once
#include "Engine.h"

#include "Widget.h"

#include "Clamp.h"
namespace MFUI
{

	class Font;

	class TextBoxWidget : public Widget
	{
	public:
		TextBoxWidget(Widget* owner)
			: Widget{ owner }
		{
		}

		inline virtual void setText(const std::u16string& text) = 0;
		inline virtual const std::u16string getText() const = 0;

		inline virtual void setSize(float newSize) = 0;
		inline virtual float getSize() const = 0;

		inline virtual void setThickness(Clampf<0, 0, 1, 0> thickness) = 0;
		inline virtual Clampf<0, 0, 1, 0> getThickness() const = 0;

		inline virtual void setColor(vec4 color) = 0;
		inline virtual vec4 getColor() const = 0;

		inline virtual Widget* getOwner() = 0;
		inline virtual const Widget* getOwner() const = 0;

		inline virtual void setFont(Font* newFont) = 0;
		inline virtual Font* getFont() const = 0;
	};

} // End namespace.