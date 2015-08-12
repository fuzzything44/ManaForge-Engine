#include "OpenGLRendererPCH.h"

#include "OpenGLTextBox.h"

#include "OpenGLFont.h"
#include "OpenGLMaterialSource.h"

OpenGLTextBox::OpenGLTextBox()
{
	// just generate the buffers, there is no data yet.
	glGenBuffers(1, &vertLocBuffer);
	glGenBuffers(1, &texCoordBuffer);
	glGenBuffers(1, &elemBuffer);
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
	auto matID = **font->getMaterialSource();

	glUseProgram(matID);

	auto loccutoff = glGetUniformLocation(matID, "cutoff");
	auto locsize = glGetUniformLocation(matID, "size");
	assert(loccutoff != -1);
	assert(locsize != -1);
	//glUniform1f(loccutoff, cutoff);
	//glUniform1f(locsize, size);

	glBindVertexArray(vertexArray);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertLocBuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer);
	glDrawElements(GL_TRIANGLES, text.size() * 2 * 3, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
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
}
