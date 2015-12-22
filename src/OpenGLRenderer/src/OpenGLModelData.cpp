#include "OpenGLModelData.h"

#include <QOpenGLFunctions_3_3_Core>

#include <GL/gl.h>

OpenGLModelData::OpenGLModelData(QOpenGLFunctions_3_3_Core& funs_, const vec2* locations, const vec2* UVs, size_t numVerts, 
		const uvec3* triangleIndicies, size_t numTriangles)
	:numTriangles(numTriangles)
	,funs{funs_}
{
	funs.glGenVertexArrays(1, &vertexArray);
	funs.glBindVertexArray(vertexArray);

	// init location buffer
	funs.glGenBuffers(1, &locBuffer);
	funs.glBindBuffer(GL_ARRAY_BUFFER, locBuffer);
	funs.glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * numVerts, locations, GL_STATIC_DRAW);

	// init UV buffer
	funs.glGenBuffers(1, &UVbuffer);
	funs.glBindBuffer(GL_ARRAY_BUFFER, UVbuffer);
	funs.glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * numVerts, UVs, GL_STATIC_DRAW);

	// init elem buffer
	funs.glGenBuffers(1, &indexBuffer);
	funs.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	funs.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uvec3) * numTriangles, triangleIndicies, GL_STATIC_DRAW);
}

void OpenGLModelData::destroy()
{
	funs.glDeleteBuffers(1, &vertexArray);
	funs.glDeleteBuffers(1, &locBuffer);
	funs.glDeleteBuffers(1, &indexBuffer);

	funs.glDeleteVertexArrays(1, &vertexArray);
}