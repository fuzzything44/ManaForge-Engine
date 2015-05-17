#pragma once
#include "OpenGLRendererConfig.h"

#include "OpenGLMaterial.h"

GLint OpenGLMaterial::startTexUniform = -1;

OpenGLMaterial::~OpenGLMaterial()
{
	if (startTexUniform == -1)
	{
		startTexUniform = glGetUniformLocation(program, "textures");
	}
}

void OpenGLMaterial::addShaderProgramFromFile(std::string filename)
{
}

void OpenGLMaterial::addShaderProgramFromSource(std::string shader)
{
}

void OpenGLMaterial::setTexture(uint32 ID, std::string texture)
{
}

void OpenGLMaterial::use()
{
	glUseProgram(program);

	for (uint32 i = 0; i < textures.size(); i++)
	{
		glUniform1i(startTexUniform + i, i);

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
	}


}

int32 OpenGLMaterial::getUniformLocation(const char* name)
{
	return glGetUniformLocation(program, name);
}