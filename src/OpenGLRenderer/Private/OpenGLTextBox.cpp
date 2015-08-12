#include "OpenGLRendererPCH.h"

#include "OpenGLTextBox.h"

#include "OpenGLFont.h"
#include "OpenGLMaterialSource.h"
#include "OpenGLRenderer.h"

OpenGLTextBox::OpenGLTextBox(OpenGLRenderer& renderer)
	: renderer(renderer)
{
	renderer.runOnRenderThreadSync([this]
		{
			location = this->renderer.textBoxes.insert(this->renderer.textBoxes.begin(), this);

			// just generate the buffers, there is no data yet.
			glGenBuffers(1, &vertLocBuffer);
			glGenBuffers(1, &texCoordBuffer);
			glGenBuffers(1, &elemBuffer);
		});
}

OpenGLTextBox::~OpenGLTextBox()
{
	renderer.runOnRenderThreadSync([this]
	{
		renderer.textBoxes.erase(location);
	});
}

void OpenGLTextBox::setText(const std::u16string& textIn)
{
	// if the texts are the same, forget it.
	if (text != textIn) {

		text = textIn;

		regenerateBuffers();
	}
}

const std::u16string& OpenGLTextBox::getText() const { return text; }

void OpenGLTextBox::setFont(std::shared_ptr<Font> newFont)
{
	// if this fails we have problems anyway...
	font = std::static_pointer_cast<OpenGLFont>(newFont);
}

std::shared_ptr<Font> OpenGLTextBox::getFont() const { return font; }

void OpenGLTextBox::render()
{
	font->render(*this);
}

void OpenGLTextBox::regenerateBuffers()
{
	// this MUST be a vector because contiguity is important
	std::vector<vec2> locations;
	std::vector<vec2> uvs;
	std::vector<uvec3> elements;

	float cursorpos = 0;

	for (decltype(text.size()) i = 0; i < text.size(); ++i)
	{
		char16_t c = text[i];

		OpenGLCharacterData d = font->getCharacterData(c);

		// add uvs
		uvs.emplace_back(d.uvBegin);
		uvs.emplace_back(d.uvEnd.x, d.uvBegin.y);
		uvs.emplace_back(d.uvBegin.x, d.uvEnd.y);
		uvs.emplace_back(d.uvEnd);

		locations.emplace_back(cursorpos + d.offset.x, d.offset.y);
		locations.emplace_back(cursorpos + d.offset.x + d.size.x, d.offset.y);
		locations.emplace_back(cursorpos + d.offset.x, d.offset.y - d.size.y);
		locations.emplace_back(cursorpos + d.offset.x + d.size.x, d.offset.y - d.size.y);

		elements.emplace_back(i * 4, i * 4 + 1, i * 4 + 2);
		elements.emplace_back(i * 4 + 1, i * 4 + 2, i * 4 + 3);

		cursorpos += d.advance;
	}

	renderer.runOnRenderThreadSync([this, &locations, &uvs, &elements]
	{
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glGenBuffers(1, &vertLocBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertLocBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * locations.size(), &locations[0], GL_STATIC_DRAW);

		glGenBuffers(1, &texCoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * uvs.size(), &uvs[0], GL_STATIC_DRAW);

		glGenBuffers(1, &elemBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uvec3) * elements.size(), &elements[0], GL_STATIC_DRAW);
	});
		
}
