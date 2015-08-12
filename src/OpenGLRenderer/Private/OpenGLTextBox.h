#pragma once
#include "OpenGLRendererConfig.h"

#include <TextBox.h>

#include <string>

class Font;
class OpenGLFont;
class OpenGLRenderer;

class OpenGLTextBox : public TextBox
{
	friend class OpenGLFont;
public:
	OpenGLTextBox(OpenGLRenderer& renderer);
	virtual ~OpenGLTextBox();

	// start TextBox interface
	virtual void setText(const std::u16string& text) override;

	virtual const std::u16string& getText() const override;
	// end TextBox interface

	virtual void setFont(std::shared_ptr<Font> newFont) override;
	virtual std::shared_ptr<Font> getFont() const override;

	void render();

private:
	void regenerateBuffers();

	std::u16string text;

	uint32 vertexArray;
	uint32 vertLocBuffer;
	uint32 texCoordBuffer;
	uint32 elemBuffer;

	float cutoff;

	OpenGLRenderer& renderer;

	std::list<OpenGLTextBox*>::iterator location;

	std::shared_ptr<OpenGLFont> font;
};