#pragma once

#include "Engine.h"

#include "Widget.h"

class TextBox;

class TextBoxWidget : public Widget
{
	TextBoxWidget();
	TextBoxWidget(const std::u16string& text);

	void setText(const std::u16string& text);
	std::u16string getText() const;

private:

	inline virtual void draw() override;
	
	TextBox* box;
};

#include "TextBox.h"

inline void TextBoxWidget::setText(const std::u16string& text)
{
	box->setText
}

inline void TextBoxWidget::draw()
{
	box->draw();
}
