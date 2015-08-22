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
	virtual const std::u16string getText() const override;

	virtual void setSize(float newSize) override;
	virtual float getSize() const override;

	virtual void setThickness(Clampf<0, 0, 1, 0> thickness) override;
	virtual Clampf<0, 0, 1, 0> getThickness() const override;

	virtual void setColor(vec4 color) override;
	virtual vec4 getColor() const override;

	// on a scale from 0-1
	virtual void setLocation(vec2 loc) override;
	virtual vec2 getLocation() const override;

	virtual void setFont(std::shared_ptr<Font> newFont) override;
	virtual std::shared_ptr<Font> getFont() const override;
	// end TextBox interface

	void render();

private:
	void regenerateBuffers();
	void reallocateBuffers();
	mat3 getMatrix();

	std::u16string text;

	uint32 vertexArray;
	uint32 vertLocBuffer;
	uint32 texCoordBuffer;
	uint32 elemBuffer;

	size_t currentMaxLetters = 0;

	vec4 color;
	float size;
	Clampf<0, 0, 1, 0> thickness;
	vec2 location;

	OpenGLRenderer& renderer;

	std::list<OpenGLTextBox*>::iterator locationIter;

	std::shared_ptr<OpenGLFont> font;
};