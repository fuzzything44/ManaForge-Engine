#pragma once

#include <array>

#include <Engine.h>

#include "OpenGLMaterialSource.h"
#include "OpenGLTexture.h"

struct OpenGLMaterialInstance
{
	OpenGLMaterialInstance(const OpenGLMaterialSource& source)
		:source(source)
	{
		
	}

	void use(const mat3& MVP)
	{
		glUseProgram(source.program);

		// apply textures
		for (auto&& tex : textures)
		{
			if (tex.handle)
			{
				glUniform1i(source.startTexUniform, tex.handle);
			}
			else
			{
				break;
			}
		}

		// apply all other properties
		std::for_each(properties.begin(), properties.end(), [](auto&& elem) { elem(); });

		// set the MVP matrix
		glUniformMatrix3fv(source.MVPUniformLocation, 1, GL_FALSE, &MVP[0][0]);
	}

	OpenGLMaterialInstance(OpenGLMaterialInstance&& other) = default;
	OpenGLMaterialInstance(const OpenGLMaterialInstance& other) = delete;

	OpenGLMaterialInstance& operator=(OpenGLMaterialInstance&& other) = default;
	OpenGLMaterialInstance& operator=(const OpenGLMaterialInstance& other) = delete;

	std::array<OpenGLTexture, 32> textures;
	std::vector<std::function<void()>> properties;

	OpenGLMaterialSource source;
};

