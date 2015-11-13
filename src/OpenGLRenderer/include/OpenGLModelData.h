#pragma once
#include <Engine.h>

#include <RefCounted.h>

#include <gl/glew.h>

struct OpenGLModelData : RefCounted<OpenGLModelData>
{
	OpenGLModelData(const vec2* locations = nullptr, const vec2* UVs = nullptr, size_t numVerts = 0, const uvec3* triangleIndicies = nullptr, size_t numTriangles = 0)
		:numTriangles((GLsizei)numTriangles)
	{
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		// init location buffer
		glGenBuffers(1, &locBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, locBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * numVerts, locations, GL_STATIC_DRAW);

		// init UV buffer
		glGenBuffers(1, &UVbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, UVbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * numVerts, UVs, GL_STATIC_DRAW);

		// init elem buffer
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uvec3) * numTriangles, triangleIndicies, GL_STATIC_DRAW);
	}

	void destroy()
	{
		glDeleteBuffers(1, &vertexArray);
		glDeleteBuffers(1, &locBuffer);
		glDeleteBuffers(1, &indexBuffer);

		glDeleteVertexArrays(1, &vertexArray);
	}
	void invalidate()
	{
		vertexArray = locBuffer = UVbuffer = indexBuffer = 0;
	}

	GLsizei numTriangles;

	GLuint vertexArray;

	GLuint locBuffer;
	GLuint UVbuffer;
	GLuint indexBuffer;
};
