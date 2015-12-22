#pragma once

#include <OpenGLRendererConfig.h>

#include <vector>

#include <Engine.h>

#include "OpenGLMaterialSource.h"
#include "OpenGLTexture.h"

#include <QOpenGLFunctions_3_3_Core>

struct OpenGLMaterialInstance
{
	OpenGLMaterialInstance(const OpenGLMaterialSource& source)
		:source(source)
	{}

	OpenGLRenderer_API void use(const mat3& MVP);

	OpenGLMaterialInstance(OpenGLMaterialInstance&& other) = default;
	OpenGLMaterialInstance(const OpenGLMaterialInstance& other) = delete;

	OpenGLMaterialInstance& operator=(OpenGLMaterialInstance&& other) = default;
	OpenGLMaterialInstance& operator=(const OpenGLMaterialInstance& other) = delete;

	std::vector<OpenGLTexture> textures;
	std::vector<std::function<void()>> properties;

	OpenGLMaterialSource source;
	
};

