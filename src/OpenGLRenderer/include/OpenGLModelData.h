#pragma once
#include <Engine.h>
#include "OpenGLRendererConfig.h"

#include <RefCounted.h>

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class QOpenGLFunctions_3_3_Core;

struct OpenGLModelData : RefCounted<OpenGLModelData>
{
	OpenGLRenderer_API OpenGLModelData(QOpenGLFunctions_3_3_Core* funs_ = nullptr, const vec2* locations = nullptr, const vec2* UVs = nullptr, size_t numVerts = 0, 
		const uvec3* triangleIndicies = nullptr, size_t numTriangles = 0);

	OpenGLModelData(const OpenGLModelData&) = default;
	OpenGLModelData(OpenGLModelData&&) = default;
	
	OpenGLModelData& operator=(const OpenGLModelData& other) = default;
	OpenGLModelData& operator=(OpenGLModelData&&) = default;
	
	OpenGLRenderer_API void destroy();


	size_t numTriangles;

	uint32 vertexArray;

	
	uint32 locBuffer;
	uint32 UVbuffer;
	uint32 indexBuffer;
	
	QOpenGLFunctions_3_3_Core* funs;
};
