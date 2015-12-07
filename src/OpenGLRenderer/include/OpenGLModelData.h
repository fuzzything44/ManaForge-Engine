#pragma once
#include <Engine.h>
#include "OpenGLRendererConfig.h"

#include <RefCounted.h>

#include <GL/glew.h>

struct OpenGLModelData : RefCounted<OpenGLModelData>
{
	OpenGLRenderer_API OpenGLModelData(const vec2* locations = nullptr, const vec2* UVs = nullptr, size_t numVerts = 0, 
		const uvec3* triangleIndicies = nullptr, size_t numTriangles = 0);

	OpenGLRenderer_API void destroy();


	GLsizei numTriangles;

	GLuint vertexArray;

	GLuint locBuffer;
	GLuint UVbuffer;
	GLuint indexBuffer;
};
