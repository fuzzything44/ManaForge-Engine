#pragma once

#include <Engine.h>
#include "OpenGLRendererConfig.h"

#include <RefCounted.h>

#include <GL/glew.h>

struct OpenGLMaterialSource : RefCounted<OpenGLMaterialSource>
{
	OpenGLRenderer_API OpenGLMaterialSource(const path_t& path);

	OpenGLRenderer_API void destroy();

	int32 startTexUniform;
	int32 MVPUniformLocation;

	GLuint program;
};
