#pragma once

#include <Engine.h>
#include "OpenGLRendererConfig.h"

#include <RefCounted.h>

class QOpenGLFunctions_3_3_Core;

struct OpenGLMaterialSource : RefCounted<OpenGLMaterialSource>
{
	OpenGLRenderer_API OpenGLMaterialSource(QOpenGLFunctions_3_3_Core& funs_, const path_t& path);

	OpenGLRenderer_API void destroy();

	int32 startTexUniform;
	int32 MVPUniformLocation;

	uint32 program;
	
	QOpenGLFunctions_3_3_Core& funs;
};
