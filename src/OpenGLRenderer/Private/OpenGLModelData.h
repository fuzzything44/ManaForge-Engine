#pragma once

#include "OpenGLRendererConfig.h"

class OpenGLModelData
{
public:
	OpenGLModelData(const vec2* vertLocs, const vec2* UVs, size_t numVerts, const uvec3* elems, size_t numElems)
	{
		assert(vertLocs);
		assert(UVs);
		assert(elems);

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

	inline void draw();

private:
	uint32 vertexArray;
	uint32 vertexLocationBuffer;
	uint32 texCoordBuffer;
	uint32 elemBuffer;

	size_t numVerts;
	size_t numElems;
};

inline void OpenGLModelData::draw()
{

	glBindVertexArray(vertexArray);

	// bind location data to the element attrib array so it shows up in our shaders -- the location is
	// zero
	// (look in shader)
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexLocationBuffer);
	glVertexAttribPointer(0, // location 0 (see shader)
		2,					 // two elements per vertex (x,y)
		GL_FLOAT,			 // they are floats
		GL_FALSE,			 // not normalized
		sizeof(float) * 2,   // the next element is 2 floats later
		nullptr				 // dont copy -- use the GL_ARRAY_BUFFER instead
		);

	// bind UV data to the element attrib array so it shows up in our sahders -- the location is
	// (look in
	// shader)
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glVertexAttribPointer(1, // location 1 (see shader)
		2,					 // two elements per vertex (u,v)
		GL_FLOAT,			 // they are floats
		GL_FALSE,			 // not normalized
		sizeof(float) * 2,   // the next element is 2 floats later
		nullptr				 // use the GL_ARRAY_BUFFER instead of copying on the spot
		);

	// bind the element buffer so it is used to make our draw call
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer);

	glDrawElements(GL_TRIANGLES, // they are trianges
		(GLsizei)numElems * 3,   // these many trainges - three verts / triangle
		GL_UNSIGNED_INT,		 // the data is uint32 - unsigned int
		nullptr					 // use the buffer instead of raw data
		);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
