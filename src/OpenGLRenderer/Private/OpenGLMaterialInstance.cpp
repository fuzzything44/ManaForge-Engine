#include "OpenGLRendererPCH.h"

#include "OpenGLMaterialInstance.h"
#include "OpenGLTexture.h"
#include "OpenGLMaterialSource.h"

#include <Logging.h>
#include <Helper.h>
#include <Texture.h>

#include <OpenGLTexture.h>


OpenGLMaterialInstance::OpenGLMaterialInstance(std::shared_ptr<MaterialSource> source)
{
	if (source)
		init(source);
}

OpenGLMaterialInstance::~OpenGLMaterialInstance()
{

}

void OpenGLMaterialInstance::setTexture(uint32 ID, std::shared_ptr<Texture> texture)
{
	assert(texture);
	textures[ID] = std::static_pointer_cast<OpenGLTexture>(texture);

}

void OpenGLMaterialInstance::init(std::shared_ptr<MaterialSource> source)
{
	program = std::static_pointer_cast<OpenGLMaterialSource>(source);


}

std::shared_ptr<MaterialSource> OpenGLMaterialInstance::getSource()
{
	return program;
}
std::shared_ptr<const MaterialSource> OpenGLMaterialInstance::getSource() const
{
	return program;
}


void OpenGLMaterialInstance::setUpdateCallback(std::function<void(MaterialInstance&)> func)
{
	updateCallback = func;
}

// start property interface
void OpenGLMaterialInstance::setProperty(const std::string& propName, int i)  
{
	properties[propName] = 
	std::tuple<int32, std::function<void(int32)> >{ 
		glGetUniformLocation(**program, propName.c_str()),
		[i](int32 loc)
		{
			glUniform1i(loc, i);
		}
	};
}
void OpenGLMaterialInstance::setProperty(const std::string& propName, ivec2 i)  
{
	properties[propName] =
		std::tuple<int32, std::function<void(int32)> >{
		glGetUniformLocation(**program, propName.c_str()),
		[i](int32 loc)
		{
			glUniform2i(loc, i.x, i.y);
		}
	};

}
void OpenGLMaterialInstance::setProperty(const std::string& propName, ivec3 i)  
{	
	properties[propName] =
		std::tuple<int32, std::function<void(int32)> >{
		glGetUniformLocation(**program, propName.c_str()),
		[i](int32 loc)
		{
			glUniform3i(loc, i.x, i.y, i.z);
		}
	};

}
void OpenGLMaterialInstance::setProperty(const std::string& propName, ivec4 i)  
{
	properties[propName] =
		std::tuple<int32, std::function<void(int32)> >{
		glGetUniformLocation(**program, propName.c_str()),
		[i](int32 loc)
		{
			glUniform4i(loc, i.x, i.y, i.z, i.w);
		}
	};
}
void OpenGLMaterialInstance::setProperty(const std::string& propName, int* i, size_t size)  
{

	switch (size)
	{
	case 1:
		properties[propName] =
			std::tuple<int32, std::function<void(int32)> >{
			glGetUniformLocation(**program, propName.c_str()),
			[i](int32 loc)
			{
				glUniform1iv(loc, 1, i);
			}
		};
		break;
	case 2:
		properties[propName] =
			std::tuple<int32, std::function<void(int32)> >{
			glGetUniformLocation(**program, propName.c_str()),
			[i](int32 loc)
			{
				glUniform2iv(loc, 1, i);
			}
		};
		break;
	case 3:
		properties[propName] =
			std::tuple<int32, std::function<void(int32)> >{
			glGetUniformLocation(**program, propName.c_str()),
			[i](int32 loc)
			{
				glUniform3iv(loc, 1, i);
			}
		};
		break;
	case 4:
		properties[propName] =
			std::tuple<int32, std::function<void(int32)> >{
			glGetUniformLocation(**program, propName.c_str()),
			[i](int32 loc)
			{
				glUniform4iv(loc, 1, i);
			}
		};
		break;
	}
}
void OpenGLMaterialInstance::setProperty(const std::string& propName, float i)  
{
	properties[propName] =
		std::tuple<int32, std::function<void(int32)> >{
		glGetUniformLocation(**program, propName.c_str()),
		[i](int32 loc)
		{
			glUniform1f(loc, i);
		}
	};

}
void OpenGLMaterialInstance::setProperty(const std::string& propName, vec2 i)  
{
	properties[propName] =
		std::tuple<int32, std::function<void(int32)> >{
		glGetUniformLocation(**program, propName.c_str()),
		[i](int32 loc)
		{
			glUniform2f(loc, i.x, i.y);
		}
	};

}
void OpenGLMaterialInstance::setProperty(const std::string& propName, vec3 i)  
{
	properties[propName] =
		std::tuple<int32, std::function<void(int32)> >{
		glGetUniformLocation(**program, propName.c_str()),
		[i](int32 loc)
		{
			glUniform3f(loc, i.x, i.y, i.z);
		}
	};


}
void OpenGLMaterialInstance::setProperty(const std::string& propName, vec4 i)  
{

	properties[propName] =
		std::tuple<int32, std::function<void(int32)> >{
		glGetUniformLocation(**program, propName.c_str()),
		[i](int32 loc)
		{
			glUniform4f(loc, i.x, i.y, i.z, i.w);
		}
	};

}
void OpenGLMaterialInstance::setProperty(const std::string& propName, float* i, size_t size)  
{
	switch (size)
	{
	case 1:
		properties[propName] =
			std::tuple<int32, std::function<void(int32)> >{
			glGetUniformLocation(**program, propName.c_str()),
			[i](int32 loc)
			{
				glUniform1fv(loc, 1, i);
			}
		};
		break;
	case 2:
		properties[propName] =
			std::tuple<int32, std::function<void(int32)> >{
			glGetUniformLocation(**program, propName.c_str()),
			[i](int32 loc)
			{
				glUniform2fv(loc, 1, i);
			}
		};
		break;
	case 3:
		properties[propName] =
			std::tuple<int32, std::function<void(int32)> >{
			glGetUniformLocation(**program, propName.c_str()),
			[i](int32 loc)
			{
				glUniform3fv(loc, 1, i);
			}
		};
		break;
	case 4:
		properties[propName] =
			std::tuple<int32, std::function<void(int32)> >{
			glGetUniformLocation(**program, propName.c_str()),
			[i](int32 loc)
			{
				glUniform4fv(loc, 1, i);
			}
		};
		break;
	}

}
void OpenGLMaterialInstance::setPropertyMatrix(const std::string& propName, mat2 i)  
{

	properties[propName] =
		std::tuple<int32, std::function<void(int32)> >{
		glGetUniformLocation(**program, propName.c_str()),
		[i](int32 loc)
		{
			glUniformMatrix2fv(loc, 1, GL_FALSE, &i[0][0]);
		}
	};
}
void OpenGLMaterialInstance::setPropertyMatrix(const std::string& propName, mat3 i)  
{
	properties[propName] =
		std::tuple<int32, std::function<void(int32)> >{
		glGetUniformLocation(**program, propName.c_str()),
		[i](int32 loc)
		{
			glUniformMatrix3fv(loc, 1, GL_FALSE, &i[0][0]);
		}
	};
}
void OpenGLMaterialInstance::setPropertyMatrix(const std::string& propName, mat4 i)  
{
	properties[propName] =
		std::tuple<int32, std::function<void(int32)> >{
		glGetUniformLocation(**program, propName.c_str()),
		[i](int32 loc)
		{
			glUniformMatrix4fv(loc, 1, GL_FALSE, &i[0][0]);
		}
	};
}
void OpenGLMaterialInstance::setPropertyMatrix2ptr(const std::string& propName, float* i)  
{
	properties[propName] =
		std::tuple<int32, std::function<void(int32)> >{
		glGetUniformLocation(**program, propName.c_str()),
		[i](int32 loc)
		{
			glUniformMatrix2fv(loc, 1, GL_FALSE, i);
		}
	};
}
void OpenGLMaterialInstance::setPropertyMatrix3ptr(const std::string& propName, float* i)  
{
	properties[propName] =
		std::tuple<int32, std::function<void(int32)> >{
		glGetUniformLocation(**program, propName.c_str()),
		[i](int32 loc)
		{
			glUniformMatrix3fv(loc, 1, GL_FALSE, i);
		}
	};
}
void OpenGLMaterialInstance::setPropertyMatrix4ptr(const std::string& propName, float* i)  
{
	properties[propName] =
		std::tuple<int32, std::function<void(int32)> >{
		glGetUniformLocation(**program, propName.c_str()),
		[i](int32 loc)
		{
			glUniformMatrix4fv(loc, 1, GL_FALSE, i);
		}
	};
}
// end property interface

void OpenGLMaterialInstance::use()
{
	if (updateCallback) updateCallback(*this);

	for (auto&& elem : properties)
	{
		auto&& fun = std::get<1>(elem.second);
		auto&& loc = std::get<0>(elem.second);

		fun(loc);
	}

	glUseProgram(**program);

	for (uint32 i = 0; i < maxTextures && textures[i]; i++)
	{
		assert(*textures[i]);

		glUniform1i(program->startTexUniform + i, i);

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, (*textures[i])->getID());
	}
	
}
