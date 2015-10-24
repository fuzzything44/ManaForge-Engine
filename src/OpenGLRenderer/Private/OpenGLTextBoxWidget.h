#pragma once
#include "OpenGLRendererConfig.h"

#include <TextBoxWidget.h>

#include <string>
#include <list>

class Font;
class OpenGLFont;
class OpenGLRenderer;

class OpenGLTextBoxWidget : public MFUI::TextBoxWidget
{
	friend class OpenGLFont;

public:
	OpenGLTextBoxWidget(Widget* owner, OpenGLRenderer& renderer);

	// start TextBox interface
	virtual void setText(const std::u16string& text) override;
	virtual const std::u16string getText() const override;

	virtual void setSize(float newSize) override;
	virtual float getSize() const override;

	virtual void setThickness(Clampf<0, 0, 1, 0> thickness) override;
	virtual Clampf<0, 0, 1, 0> getThickness() const override;

	virtual void setColor(vec4 color) override;
	virtual vec4 getColor() const override;

	virtual Widget* getOwner() override { return owner; }
	virtual const Widget* getOwner() const { return owner; }

	virtual void setFont(Font* newFont) override;
	virtual Font* getFont() const override;
	// end TextBox interface

	virtual void draw(const mat3& matAtOriginOfParent) override;

private:
	void regenerateBuffers();
	void reallocateBuffers();

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

	std::list<OpenGLTextBoxWidget*>::iterator locationIter;

	OpenGLFont* font;
};