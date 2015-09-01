#include "OpenGLRendererPCH.h"

#include "OpenGLMaterialInstance.h"
#include "OpenGLTexture.h"
#include "OpenGLMaterialSource.h"
#include "OpenGLRenderer.h"

#include <Logging.h>
#include <Helper.h>
#include <Texture.h>

#include <OpenGLTexture.h>

OpenGLMaterialInstance::OpenGLMaterialInstance(
	OpenGLRenderer& renderer, std::shared_ptr<MaterialSource> source)
	: renderer(renderer)
{
	if (source) init(source);
}

OpenGLMaterialInstance::~OpenGLMaterialInstance() {}

void OpenGLMaterialInstance::setTexture(uint32 ID, Texture* texture)
{
	assert(texture);
	textures[ID] = static_cast<OpenGLTexture*>(texture);
}

void OpenGLMaterialInstance::setTexture(uint32 ID, std::shared_ptr<Texture> texture)
{
	assert(texture);
	refCountedTextures[ID] = std::static_pointer_cast<OpenGLTexture>(texture);
}

void OpenGLMaterialInstance::init(std::shared_ptr<MaterialSource> source)
{
	program = std::static_pointer_cast<OpenGLMaterialSource>(source);
}

std::shared_ptr<MaterialSource> OpenGLMaterialInstance::getSource() { return program; }
std::shared_ptr<const MaterialSource> OpenGLMaterialInstance::getSource() const { return program; }

void OpenGLMaterialInstance::setUpdateCallback(std::function<void(MaterialInstance&)> func)
{
	updateCallback = func;
}

// start property interface
void OpenGLMaterialInstance::setProperty(const std::string& propName, int32 i)
{

	renderer.runOnRenderThreadAsync([this, propName, i]
		{
			properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]()
			{
				assert(loc != -1);
				glUniform1i(loc, i);
			};
		});
}
void OpenGLMaterialInstance::setProperty(const std::string& propName, const ivec2& i)
{
	renderer.runOnRenderThreadAsync([this, propName, i]
		{
			properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
			{
				assert(loc != -1);
				glUniform2i(loc, i.x, i.y);
			};
		});
}
void OpenGLMaterialInstance::setProperty(const std::string& propName, const ivec3& i)
{

	renderer.runOnRenderThreadAsync([this, propName, i]
		{
			properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
			{
				assert(loc != -1);
				glUniform3i(loc, i.x, i.y, i.z);
			};
		});
}
void OpenGLMaterialInstance::setProperty(const std::string& propName, const ivec4& i)
{

	renderer.runOnRenderThreadAsync([this, propName, i]()
		{
			properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
			{
				assert(loc != -1);
				glUniform4i(loc, i.x, i.y, i.z, i.w);
			};
		});
}
void OpenGLMaterialInstance::setProperty(const std::string& propName, int* i, size_t size)
{
	renderer.runOnRenderThreadAsync([this, propName, i, size]
		{
			switch (size)
			{
			case 1:
				properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
				{
					assert(loc != -1);
					glUniform1iv(loc, 1, i);
				};
				break;
			case 2:
				properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
				{
					assert(loc != -1);
					glUniform2iv(loc, 1, i);
				};
				break;
			case 3:
				properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
				{
					assert(loc != -1);
					glUniform3iv(loc, 1, i);
				};
				break;
			case 4:
				properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
				{
					assert(loc != -1);
					glUniform4iv(loc, 1, i);
				};
				break;
			}
		});
}
void OpenGLMaterialInstance::setProperty(const std::string& propName, float i)
{

	renderer.runOnRenderThreadAsync([this, propName, i]
		{
			properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
			{
				assert(loc != -1);
				glUniform1f(loc, i);
			};
		});
}
void OpenGLMaterialInstance::setProperty(const std::string& propName, const vec2& i)
{
	renderer.runOnRenderThreadAsync([this, propName, i]
		{
			properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
			{
				assert(loc != -1);
				glUniform2f(loc, i.x, i.y);
			};
		});
}
void OpenGLMaterialInstance::setProperty(const std::string& propName, const vec3& i)
{
	renderer.runOnRenderThreadAsync([this, propName, i]
		{
			properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
			{
				assert(loc != -1);
				glUniform3f(loc, i.x, i.y, i.z);
			};
		});
}
void OpenGLMaterialInstance::setProperty(const std::string& propName, const vec4& i)
{
	renderer.runOnRenderThreadAsync([this, propName, i]
		{
			properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
			{
				assert(loc != -1);
				glUniform4f(loc, i.x, i.y, i.z, i.w);
			};
		});
}
void OpenGLMaterialInstance::setProperty(const std::string& propName, float* i, size_t size)
{
	renderer.runOnRenderThreadAsync([this, propName, i, size]
		{
			switch (size)
			{
			case 1:
				properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
				{
					assert(loc != -1);
					glUniform1fv(loc, 1, i);
				};
				break;
			case 2:
				properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
				{
					assert(loc != -1);
					glUniform2fv(loc, 1, i);
				};
				break;
			case 3:
				properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
				{
					assert(loc != -1);
					glUniform3fv(loc, 1, i);
				};
				break;
			case 4:
				properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
				{
					assert(loc != -1);
					glUniform4fv(loc, 1, i);
				};
				break;
			}
		});
}
void OpenGLMaterialInstance::setPropertyMatrix(const std::string& propName, const mat2& i)
{
	renderer.runOnRenderThreadAsync([this, propName, i]
		{
			properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
			{
				assert(loc != -1);
				glUniformMatrix2fv(loc, 1, GL_FALSE, &i[0][0]);
			};
		});
}
void OpenGLMaterialInstance::setPropertyMatrix(const std::string& propName, const mat3& i)
{
	renderer.runOnRenderThreadAsync([this, propName, i]
		{
			properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
			{
				assert(loc != -1);
				glUniformMatrix3fv(loc, 1, GL_FALSE, &i[0][0]);
			};
		});
}
void OpenGLMaterialInstance::setPropertyMatrix(const std::string& propName, const mat4& i)
{
	renderer.runOnRenderThreadAsync([this, propName, i]
		{
			properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
			{
				assert(loc != -1);
				glUniformMatrix4fv(loc, 1, GL_FALSE, &i[0][0]);
			};
		});
}
void OpenGLMaterialInstance::setPropertyMatrix2ptr(const std::string& propName, float* i)
{
	renderer.runOnRenderThreadAsync([this, propName, i]
		{
			properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
			{
				assert(loc != -1);
				glUniformMatrix2fv(loc, 1, GL_FALSE, i);
			};
		}); // fds fdas
}
void OpenGLMaterialInstance::setPropertyMatrix3ptr(const std::string& propName, float* i)
{
	renderer.runOnRenderThreadAsync([this, propName, i]
		{
			properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
			{
				assert(loc != -1);
				glUniformMatrix3fv(loc, 1, GL_FALSE, i);
			};
		});
}
void OpenGLMaterialInstance::setPropertyMatrix4ptr(const std::string& propName, float* i)
{
	renderer.runOnRenderThreadAsync([this, propName, i]
		{
			properties[propName] = [ i, loc = glGetUniformLocation(**program, propName.c_str()) ]
			{
				assert(loc != -1);
				glUniformMatrix4fv(loc, 1, GL_FALSE, i);
			};
		});
}
// end property interface

void OpenGLMaterialInstance::use()
{
	if (updateCallback) updateCallback(*this);

	renderer.runOnRenderThreadAsync([this]
		{
			glUseProgram(**program);
			for (auto&& elem : properties)
			{
				elem.second();
			}

			for (uint32 i = 0; i < maxTextures && textures[i]; i++)
			{
				glUniform1i(program->startTexUniform + i, i);

				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, textures[i]->getID());
			}

			for (uint32 i = 0; i < maxTextures && refCountedTextures[i]; i++)
			{
				glUniform1i(program->startTexUniform + i, i);

				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, refCountedTextures[i]->getID());
			}
		});
}
