#pragma once
#include "OpenGLRendererConfig.h"

#include "OpenGLMaterial.h"

std::vector<GLint> OpenGLMaterial::uniforms = std::vector<GLint>(32);

OpenGLMaterial::~OpenGLMaterial()
{
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

	if (texUniformLoc != 0)
	{
		glUniform1i(texUniformLoc, 0);
	}


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}