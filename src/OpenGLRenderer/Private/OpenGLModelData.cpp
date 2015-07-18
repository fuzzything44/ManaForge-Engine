#include "OpenGLModelData.h"
#include "Helper.h"

void OpenGLModelData::init(const vec2* vertLocs, const vec2* UVs, uint32 numVerts, const uvec3* elems, uint32 numElems)
{

	this->numVerts = numVerts;
	this->numElems = numElems;


	// make sure they aren't zero
	check(numVerts);
	check(numElems);

	// init GL buffers
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	// init location buffer
	glGenBuffers(1, &vertexLocationBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexLocationBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * numVerts, vertLocs, GL_STATIC_DRAW);

	// init UV buffer
	glGenBuffers(1, &texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * numVerts, UVs, GL_STATIC_DRAW);

	// init elem buffer
	glGenBuffers(1, &elemBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uvec3) * numElems, elems, GL_STATIC_DRAW);


}

OpenGLModelData::~OpenGLModelData()
{

	glDeleteBuffers(1, &vertexLocationBuffer);
	glDeleteBuffers(1, &texCoordBuffer);
	glDeleteBuffers(1, &elemBuffer);

	glDeleteVertexArrays(1, &vertexArray);


}
