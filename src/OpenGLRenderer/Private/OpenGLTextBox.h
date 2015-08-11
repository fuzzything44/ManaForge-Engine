#pragma once
#include "OpenGLRendererConfig.h"

#include <TextBox.h>

#include <string>

class Font;
class OpenGLFont;

class OpenGLTextBox : public TextBox
{
public:
	OpenGLTextBox();

	// start TextBox interface
	virtual void setText(const std::u16string& text) override;

	virtual const std::u16string& getText() const override;
	// end TextBox interface

	virtual void setFont(std::shared_ptr<Font> newFont) override;
	virtual std::shared_ptr<Font> getFont() const override;

private:
	void regenerateBuffers();

	std::u16string text;

	uint32 vertexArray;
	uint32 vertLocBuffer;
	uint32 texCoordBuffer;
	uint32 elemBuffer;

	std::shared_ptr<OpenGLFont> font;
};