#include "OpenGLRendererPCH.h"

#include "OpenGLTextBox.h"

#include "OpenGLFont.h"
#include "OpenGLMaterialSource.h"
#include "OpenGLRenderer.h"
#include "OpenGLWindow.h"

#include <glm-ortho-2d.h>

OpenGLTextBox::OpenGLTextBox(OpenGLRenderer& renderer)
	: renderer(renderer)
	, thickness(.5f)
	, size(1.f)
{
	renderer.runOnRenderThreadSync([this]
		{
			locationIter = this->renderer.textBoxes.insert(this->renderer.textBoxes.begin(), this);

			// just generate the buffers, there is no data yet.
			glGenVertexArrays(1, &vertexArray);
			glGenBuffers(1, &vertLocBuffer);
			glGenBuffers(1, &texCoordBuffer);
			glGenBuffers(1, &elemBuffer);
		});
}

OpenGLTextBox::~OpenGLTextBox()
{
	renderer.runOnRenderThreadSync([this]
		{
			renderer.textBoxes.erase(locationIter);
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

const std::u16string OpenGLTextBox::getText() const { return text; }

void OpenGLTextBox::setSize(float newSize) { size = newSize; }

float OpenGLTextBox::getSize() const { return size; }

void OpenGLTextBox::setThickness(Clampf<0, 0, 1, 0> thicknessIn) { thickness = thicknessIn; }

Clampf<0, 0, 1, 0> OpenGLTextBox::getThickness() const { return thickness; }

void OpenGLTextBox::setColor(vec4 colorIn) { color = colorIn; }

vec4 OpenGLTextBox::getColor() const { return color; }

void OpenGLTextBox::setLocation(vec2 loc) { location = loc; }

vec2 OpenGLTextBox::getLocation() const { return location; }

void OpenGLTextBox::setFont(std::shared_ptr<Font> newFont)
{
	// if this fails we have problems anyway...
	font = std::static_pointer_cast<OpenGLFont>(newFont);
}

std::shared_ptr<Font> OpenGLTextBox::getFont() const { return font; }

void OpenGLTextBox::render() { font->render(*this); }

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

	if (text.size() > currentMaxLetters) reallocateBuffers();

	renderer.runOnRenderThreadAsync([
		this,
		locations = std::vector<vec2>(std::move(locations)),
		uvs = std::vector<vec2>(std::move(uvs)),
		elements = std::vector<uvec3>(std::move(elements))
	]
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertLocBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec2) * locations.size(), locations.data());

			glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec2) * uvs.size(), uvs.data());

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uvec3) * elements.size(), elements.data());
		});
}

void OpenGLTextBox::reallocateBuffers()
{
	currentMaxLetters = text.size() + 5;

	renderer.runOnRenderThreadAsync([this]
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertLocBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * currentMaxLetters * 4, nullptr, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * currentMaxLetters * 4, nullptr, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer);
			glBufferData(
				GL_ELEMENT_ARRAY_BUFFER, sizeof(uvec3) * currentMaxLetters * 2, nullptr, GL_DYNAMIC_DRAW);
		});
}

mat3 OpenGLTextBox::getMatrix()
{
	auto windowSize = uvec2{renderer.window->getWindowProps().size};
	float aspectRatio = ((float)windowSize.x) / ((float)windowSize.y);
	mat3 ret = glm::ortho2d(0.f, aspectRatio, 0.f, 1.f);
	ret = glm::translate(ret, location);
	ret = glm::scale(ret, vec2{size, size});

	auto a = ret * vec3{0.f, 0.f, 1.f};

	return ret;
}
