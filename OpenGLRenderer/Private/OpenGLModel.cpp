#include "OpenGLRendererConfig.h"

#include <OpenGLRenderer.h>
#include <Transform.h>
#include <ModuleManager.h>
#include <Helper.h>
#include <CameraComponent.h>



// define static variables
int32 OpenGLModel::viewMatUniformLoc = 0;
int32 OpenGLModel::texUniformLoc = -1;
uint32 OpenGLModel::program = 0;


OpenGLModel::OpenGLModel(vec2* locationsIn, vec2* UVsIn, uint32* elemsIn, uint32 numVertsIn, uint32 numElemsIn, OpenGLRenderer* rendererIn)
	:numVerts(numVertsIn),
	numElems(numElemsIn),
	renderer(rendererIn)
{
	// make sure they aren't zero
	check(numVerts);
	check(numElems);

	// allocate data
	locations = static_cast<vec2*>(malloc(sizeof(vec2) * numVerts));
	UVs = static_cast<vec2*>(malloc(sizeof(vec2) * numVerts));
	elems = static_cast<uint32*>(malloc(sizeof(uint32) * numElems));

	// copy data over
	memcpy(locations, locationsIn, numVerts * sizeof(vec2));
	memcpy(UVs, UVsIn, numVerts * sizeof(vec2));
	memcpy(elems, elemsIn, numElems * sizeof(uint32));

	// add model to renderer's list
	renderer->models.push_front(this);

	// init GL buffers
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	// init location buffer
	glGenBuffers(1, &vertexLocationBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexLocationBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * numVerts, locations, GL_STATIC_DRAW);

	// init UV buffer
	glGenBuffers(1, &texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * numVerts, locations, GL_STATIC_DRAW);

	// init elem buffer
	glGenBuffers(1, &elemBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * numElems, elems, GL_STATIC_DRAW);



	// load program and get uniform locations == only first time
	if (program == 0)
	{
		// TODO: add custom shaders

		program = renderer->loadShaderProgram("actor");

		texUniformLoc = glGetUniformLocation(program, "texture");
		viewMatUniformLoc = glGetUniformLocation(program, "viewMat");
	}
}



Transform OpenGLModel::getTransform() const
{
	return trans;
}

vec2 OpenGLModel::getLocation() const
{
	return trans.location;
}

float OpenGLModel::getRotation() const
{
	return trans.rotation;
}

vec2 OpenGLModel::getScale() const
{
	return trans.scale;
}

void OpenGLModel::setTransform(Transform newTransform)
{
	trans = newTransform;
}


void OpenGLModel::setLocation(vec2 newLocation)
{
	trans.location = newLocation;
}

void OpenGLModel::setRotation(float newRotation)
{
	trans.rotation = newRotation;
}

void OpenGLModel::setScale(vec2 newScale)
{
	trans.scale = newScale;
}

void OpenGLModel::addRelativeTransform(Transform add)
{
	trans += add;
}

void OpenGLModel::addRelativeLocation(vec2 locToAdd)
{
	trans.location += locToAdd;
}

void OpenGLModel::addRelativeRotation(float rotToAdd)
{
	trans.rotation += rotToAdd;
}

void OpenGLModel::addRelativeScale(vec2 scaleToAdd)
{
	trans.scale += scaleToAdd;
}

OpenGLModel::~OpenGLModel()
{
	check(renderer);

	free(locations);
	free(UVs);
	free(elems);

	renderer->models.remove(this);
}

void OpenGLModel::draw()
{
	// use the global program
	glUseProgram(program);

	if (texUniformLoc != 0)
	{
		glUniform1i(texUniformLoc, 0);
	}


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindVertexArray(vertexArray);

	// set the viewMat in the shader to the view mat defined by the camera that is current
	glUniformMatrix4fv(viewMatUniformLoc, 1, GL_FALSE, &(renderer->getCurrentCamera()->getViewMat())[0][0]);

	// bind location data to the element attrib array so it shows up in our shaders -- the location is zero (look in shader)
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexLocationBuffer);
	glVertexAttribPointer(
		0, // location 0 (see shader)
		2, // two elements per vertex (x,y)
		GL_FLOAT, // they are floats
		GL_FALSE, // not normalized (look up vector normalization)
		sizeof(float) * 2, // the next element is 2 floats later
		nullptr // dont copy -- use the GL_ARRAY_BUFFER instead
		);

	// bind UV data to the element attrib array so it shows up in our sahders -- the location is  (look in shader)
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glVertexAttribPointer(
		1, // location 1 (see shader)
		2, // two elements per vertex (u,v)
		GL_FLOAT, // they are floats
		GL_FALSE, // not normalized (look up vector normalization)
		sizeof(float) * 2, // the next element is 2 floats later
		nullptr // use the GL_ARRAY_BUFFER instead of copying on the spot
		);

	// bind the element buffer so it is used to make our draw call
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer);

	glDrawElements(
		GL_TRIANGLES, // they are trianges
		numElems, // these many verticies
		GL_UNSIGNED_INT, // the data is uint32 - unsigned int
		nullptr // use the buffer instead of raw data
		);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
