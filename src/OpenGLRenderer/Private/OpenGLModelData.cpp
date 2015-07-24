#include "OpenGLRendererPCH.h"

#include "OpenGLModelData.h"

#include "OpenGLRenderer.h"

#include <Helper.h>

void OpenGLModelData::init(const vec2* vertLocs_, const vec2* UVs_, size_t numVerts_, const uvec3* elems_, size_t numElems_)
{
	numVerts = numVerts_;
	numElems = numElems_;


	// make sure they aren't zero
	check(numVerts);
	check(numElems);

	renderer.runOnRenderThreadSync([this, vertLocs_, UVs_, elems_]
	{


		// init GL buffers
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		// init location buffer
		glGenBuffers(1, &vertexLocationBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexLocationBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * numVerts, vertLocs_, GL_STATIC_DRAW);

		// init UV buffer
		glGenBuffers(1, &texCoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * numVerts, UVs_, GL_STATIC_DRAW);

		// init elem buffer
		glGenBuffers(1, &elemBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uvec3) * numElems, elems_, GL_STATIC_DRAW);


	});

	bisInitialized = true;
	
}

bool OpenGLModelData::isInitialized()
{
	return bisInitialized;
}

OpenGLModelData::~OpenGLModelData()
{
	renderer.runOnRenderThreadSync([this]
	{
		glDeleteBuffers(1, &vertexLocationBuffer);
		glDeleteBuffers(1, &texCoordBuffer);
		glDeleteBuffers(1, &elemBuffer);

		glDeleteVertexArrays(1, &vertexArray);
	});

}
