#include "OpenGLRendererPCH.h"

#include "OpenGLFont.h"
#include "OpenGLTexture.h"
#include "OpenGLRenderer.h"
#include "OpenGLMaterialSource.h"
#include "OpenGLTextBox.h"

#include <boost/filesystem/fstream.hpp>

#include <boost/archive/xml_wiarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <SOIL/SOIL.h>

#include <string>

OpenGLFont::OpenGLFont(OpenGLRenderer& rendererIn, const path_t& name)
	: fontName(name)
	, renderer(rendererIn)
{
	if (fontName.empty()) return;

	// load the drawData
	{
		boost::filesystem::wifstream i_stream{L"fonts\\" + fontName.wstring() + L"\\drawData.txt"};

		if (!i_stream.is_open()) MFLOG(Error) << "Failed to open draw data for font" << fontName;

		try
		{

			boost::archive::xml_wiarchive i_arch{i_stream};

			i_arch >> boost::serialization::make_nvp("charData", charData);
		}
		catch (boost::archive::archive_exception& e)
		{
			MFLOG(Error) << "Archive exception while loading font " << fontName << " Error: " << e.what();
		}
		catch (std::runtime_error& e)
		{
			MFLOG(Error) << "Failed to load font " << fontName << " Error: " << e.what();
		}
	}
	using namespace std::string_literals;

	renderer.runOnRenderThreadSync([this]
		{
			tex = SOIL_load_OGL_texture(("fonts\\" + fontName.string() + "\\font.dds").c_str(),
				1,							// 1 channel
				0,							// create a new texture
				SOIL_FLAG_DDS_LOAD_DIRECT); // load it from dds

			matSource = std::static_pointer_cast<OpenGLMaterialSource>(renderer.getMaterialSource("font"));

			cutoffUniLoc = glGetUniformLocation(**matSource, "cutoff");
			viewMatUniLoc = glGetUniformLocation(**matSource, "viewMat");
			colorUniLoc = glGetUniformLocation(**matSource, "forgroundColor");
		});
}

OpenGLFont::~OpenGLFont()
{
	renderer.runOnRenderThreadSync([this]
		{
			glDeleteTextures(1, &tex);
		});
}

OpenGLCharacterData OpenGLFont::getCharacterData(wchar_t ch)
{
	auto iter = charData.find(ch);

	if (iter != charData.end()) {
		return iter->second;
	}

	MFLOG(Error) << "Cannot find char " << ch << " in font " << fontName;

	return OpenGLCharacterData();
}

std::shared_ptr<OpenGLMaterialSource> OpenGLFont::getMaterialSource() { return matSource; }

void OpenGLFont::render(OpenGLTextBox& box)
{
	auto matID = **matSource;

	renderer.runOnRenderThreadAsync([this, matID, &box]
		{

			glUseProgram(matID);

			assert(cutoffUniLoc != -1);
			glUniform1f(cutoffUniLoc, box.thickness);

			assert(viewMatUniLoc != -1);
			glUniformMatrix3fv(viewMatUniLoc, 1, GL_FALSE, &box.getMatrix()[0][0]);

			assert(colorUniLoc != -1);
			glUniform4f(colorUniLoc, box.color.r, box.color.g, box.color.b, box.color.a);

			glUniform1i(glGetUniformLocation(matID, "tex"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex);

			glBindVertexArray(box.vertexArray);

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, box.vertLocBuffer);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, box.texCoordBuffer);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, box.elemBuffer);
			glDrawElements(GL_TRIANGLES, (GLsizei)box.text.size() * 2 * 3, GL_UNSIGNED_INT, 0);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
		});
}
